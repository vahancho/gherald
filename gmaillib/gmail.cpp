/**************************************************************************
*   Copyright (C) 2013 by Vahan Aghajanyan                                *
*   vahancho@gmail.com                                                    *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include <QtNetwork/QSslCipher>
#include <QCoreApplication>
#include <QStringList>
#include <QBuffer>
#include "gmail.h"
#include "response.h"

// Time to wait for the server response.
static int timeout = 30000;

Gmail::Gmail(QObject *parent)
    :
        QObject(parent),
        m_loggedIn(false)
{
    QObject::connect(&m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    QObject::connect(&m_socket, SIGNAL(readyRead()),
            this, SLOT(socketReadyRead()));
    QObject::connect(&m_socket, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)),
            this, SLOT(onProxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)));

    QObject::connect(&m_timer, SIGNAL(timeout()), SLOT(onTimer()));
    QObject::connect(this, SIGNAL(done()), SLOT(onLogin()));
}

Gmail::~Gmail()
{
}

bool Gmail::connect()
{
    m_socket.connectToHostEncrypted("imap.gmail.com", 993);
    if (!m_socket.waitForEncrypted()) {
        emit error(m_socket.errorString());
        return false;
    }
    return true;
}

void Gmail::logout()
{
    if (loggedIn()) {
        sendCommand("LOGOUT");
        while(m_timer.isActive()) {
            QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
        }
        m_loggedIn = false;
        m_socket.abort();
    }
}

void Gmail::login(const QString &user, const QString &pass)
{
    if (user.isEmpty() || pass.isEmpty()) {
        emit error(tr("User credentials incomplete. "
                      "Please provide user name and password."));
        return;
    }

    QString loginStr = QString("LOGIN %1 %2").arg(user).arg(pass);
    m_loginPrefix = sendCommand(loginStr);

    while(!m_loggedIn && m_timer.isActive()) {
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
    }
}

void Gmail::sendUnreadCount()
{
    if (access() == Unknown) {
        // Read-only access.
        m_accessPrefix = sendCommand("EXAMINE INBOX", false);
    }
    m_commands.remove(m_unreadPrefix);
    m_unreadPrefix = sendCommand("status INBOX (unseen)");
}

int Gmail::unreadCount() const
{
    if (isResponseExist(m_unreadPrefix)) {
        QString responseStr = responseData(m_unreadPrefix);
        Response response(responseStr);
        if (response.status() == Response::Ok) {
            QStringList info = response.info();
            Q_ASSERT(info.size() == 1);
            QString s = info.first();
            QRegExp rx("\\d+");
            int pos = rx.indexIn(s);
            if (pos > -1) {
                QString value = rx.cap(0);
                return value.toInt();
            }
        } else {
            emit error(response.statusMessage());
        }
    }
    return -1;
}

void Gmail::markAsRead(int id)
{
    if (access() != ReadWrite) {
        // Read-write access.
        m_accessPrefix = sendCommand("SELECT INBOX", false);
    }

    QString setFlag = QString("STORE %1 +FLAGS (\\SEEN)").arg(id);
    sendCommand(setFlag);
}

void Gmail::sendUnreadMessages()
{
    // Remove response received from previous invocation if it was not read.
    m_commands.remove(m_accessPrefix);

    // Read-only access.
    m_accessPrefix = sendCommand("EXAMINE INBOX", false);

    m_commands.remove(m_unreadMsgPrefix);
    m_unreadMsgPrefix = sendCommand("SEARCH UNSEEN");
}

QList<int> Gmail::unreadMessages() const
{
    if (isResponseExist(m_unreadMsgPrefix)) {
        QString responseStr = responseData(m_unreadMsgPrefix);
        Response response(responseStr);
        if (response.status() == Response::Ok) {
            QStringList info = response.info();
            Q_ASSERT(info.size() == 1);
            QString s = info.first();
            QList<int> ids;
            QRegExp rx("(\\d+)");
            int pos = 0;
            while ((pos = rx.indexIn(s, pos)) != -1) {
                ids.append(rx.cap(1).toInt());
                pos += rx.matchedLength();
            }
            return ids;
        } else {
            emit error(response.statusMessage());
        }
    }
    return QList<int>();
}

void Gmail::socketStateChanged(QAbstractSocket::SocketState state)
{
    if (state == QAbstractSocket::UnconnectedState) {
        m_loggedIn = false;
    }
}

void Gmail::onProxyAuthenticationRequired(const QNetworkProxy & /*proxy*/,
                                          QAuthenticator * /*auth*/)
{
}

void Gmail::socketReadyRead()
{
    if (m_socket.state() == QAbstractSocket::UnconnectedState ||
        m_commandQueue.isEmpty()) {
        return;
    }
    QByteArray data = m_socket.readAll();

    QBuffer buffer;
    buffer.open(QBuffer::WriteOnly);
    buffer.write(data);
    buffer.close();

    bool shouldReset = false;
    Command cmd = m_commandQueue.head();
    buffer.open(QBuffer::ReadOnly);
    while (buffer.canReadLine()) {
        QString line = QString::fromUtf8(buffer.readLine().data());
        QString pref = prefix(line);

        // Update the command's response.
        QString response = responseData(cmd.m_prefix);
        response.append(line);
        m_commands.insert(cmd.m_prefix, response);

        if (pref == cmd.m_prefix) {
            m_commandQueue.dequeue();

            // Check the response status.
            Response r(response);
            if (r.status() == Response::Ok) {
                if (cmd.m_notify) {
                    emit done();
                }
            } else {
                // An error occurred, so no need to process the rest commands.
                // Rest commands even may not appear at all.
                // Let's reset everything and start over again.
                shouldReset = true;
                if (cmd.m_notify) {
                    emit error(r.statusMessage());
                }
            }

            qint64 pos = buffer.pos();
            if (pos < buffer.size()) {
                // Case when this chunk of data contains responses from
                // more than one command.
                Q_ASSERT(!m_commandQueue.isEmpty());
                cmd = m_commandQueue.head();
            }

            if (m_commandQueue.isEmpty()) {
                m_timer.stop();
            }
        }
    }
    buffer.close();
    if (shouldReset) {
        reset();
    }
}

QString Gmail::sendCommand(const QString &command, bool notify)
{
    // Always connect to the server if not connected yet.
    if (m_socket.state() != QAbstractSocket::ConnectedState && !connect()) {
        emit error(tr("Cannot connect to the server."));
        return QString();
    }

    static int index = 1;
    Command cmd;
    cmd.m_prefix = QString("cmd%1").arg(index++);
    cmd.m_notify = notify;
    m_commandQueue.enqueue(cmd);

    QString commandStr = QString("%1 %2\r\n").arg(cmd.m_prefix).arg(command);

    // Start response timeout counting. If we do not get response after it is
    // timed out, we reset everything to start all over again.
    m_timer.start(timeout);

    m_socket.write(commandStr.toUtf8());
    if (!m_socket.waitForBytesWritten()) {
        emit error(tr("Failed to write into the socket."));
        return QString();
    }
    return cmd.m_prefix;
}

QString Gmail::prefix(const QString &line) const
{
    QStringList tokens = line.split(' ');
    return tokens.first();
}

bool Gmail::loggedIn() const
{
    return m_loggedIn && (m_socket.state() == QAbstractSocket::ConnectedState);
}

Gmail::Access Gmail::access() const
{
    if (loggedIn() && isResponseExist(m_accessPrefix)) {
        QString responseStr = responseData(m_accessPrefix);
        Response response(responseStr);
        if (response.status() == Response::Ok) {
            QString statusMsg = response.statusMessage();
            QString accessStr = statusMsg.section(' ', 0, 0);
            if (accessStr == "[READ-ONLY]") {
                return ReadOnly;
            } else if (accessStr == "[READ-WRITE]") {
                return ReadWrite;
            } else {
                return Unknown;
            }
        }
    }
    return Unknown;
}

void Gmail::onTimer()
{
    reset();
}

void Gmail::reset()
{
    // Reset everything.
    m_commandQueue.clear();
    m_commands.clear();
    m_timer.stop();
}

QString Gmail::responseData(const QString &prefix) const
{
    return m_commands.take(prefix);
}

bool Gmail::isResponseExist(const QString &prefix) const
{
    return m_commands.contains(prefix);
}

void Gmail::onLogin()
{
    if (isResponseExist(m_loginPrefix)) {
        QString responseStr = responseData(m_loginPrefix);
        Response response(responseStr);
        m_loggedIn = (response.status() == Response::Ok);
    }
}
