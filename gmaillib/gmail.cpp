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
#include <QStringList>
#include "gmail.h"
#include "response.h"

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
    if (m_loggedIn) {
        sendCommand("LOGOUT");
        // Need to wait, until we are logged out.
        m_eventLoop.exec();
        m_loggedIn = false;
        m_socket.abort();
    }
}

void Gmail::login(const QString &user, const QString &pass)
{
    if (user.isEmpty() || pass.isEmpty()) {
        emit error("User credentials incomplete. "
                   "Please provide user name and password.");
        return;
    }

    QString loginStr = QString("LOGIN %1 %2").arg(user).arg(pass);
    QString prefix = sendCommand(loginStr);

    // Need to wait, until we are logged in.
    m_eventLoop.exec();

    if (m_commands.contains(prefix)) {
        QString responseStr = m_commands.value(prefix);
        Response response(responseStr);
        if (response.status() != Response::Ok) {
            emit error(response.statusMessage());
        } else {
            m_loggedIn = true;
        }
    }
}

void Gmail::sendUnreadCount()
{
    if (access() == Unknown) {
        // Read-only asccess.
        m_accessPrefix = sendCommand("EXAMINE INBOX", false);
    }
    m_unreadPrefix = sendCommand("status INBOX (unseen)");
}

int Gmail::unreadCount() const
{
    if (m_commands.contains(m_unreadPrefix)) {
        QString responseStr = m_commands.value(m_unreadPrefix);
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
    if (access() == Unknown) {
        // Read-only asccess.
        m_accessPrefix = sendCommand("EXAMINE INBOX", false);
    }
    m_unreadMsgPrefix = sendCommand("SEARCH UNSEEN");
}

QList<int> Gmail::unreadMessages() const
{
    if (m_commands.contains(m_unreadMsgPrefix)) {
        QString responseStr = m_commands.value(m_unreadMsgPrefix);
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

void Gmail::socketStateChanged(QAbstractSocket::SocketState /*state*/)
{
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
    qDebug() << "Received";

    Command *cmd = m_commandQueue.head();
    cmd->m_buffer->open(QBuffer::WriteOnly | QIODevice::Append);
    cmd->m_buffer->write(m_socket.readAll());
    cmd->m_buffer->close();

    cmd->m_buffer->open(QBuffer::ReadOnly);
    while (cmd->m_buffer->canReadLine()) {
        QString line = QString::fromUtf8(cmd->m_buffer->readLine().data());

        // Update the command's response.
        QString response = m_commands.value(cmd->m_prefix);
        response.append(line);
        m_commands.insert(cmd->m_prefix, response);

        QString pref = prefix(line);
        if (pref == cmd->m_prefix) {
            qDebug() << response;

            m_commandQueue.dequeue();
            if (m_eventLoop.isRunning()) {
                m_eventLoop.quit();
            }
            if (cmd->m_notify) {
                emit done();
            }
            delete cmd;
            return;
        }
    }
    // Remove data that has been stored from the buffer.
    qint64 pos = cmd->m_buffer->pos();
    cmd->m_buffer->close();
    cmd->m_buffer->setData(QByteArray(cmd->m_buffer->data()).remove(0, pos));
}

QString Gmail::sendCommand(const QString &command, bool notify)
{
    // Always connect to the server if not connected yet.
    if (m_socket.state() != QAbstractSocket::ConnectedState && !connect()) {
        emit error("Cannot connect to the server.");
        return QString();
    }

    static int index = 1;
    Command *cmd = new Command;
    cmd->m_prefix = QString("cmd%1").arg(index++);
    cmd->m_notify = notify;
    m_commandQueue.enqueue(cmd);

    QString commandStr = QString("%1 %2\r\n").arg(cmd->m_prefix).arg(command);
    m_socket.write(commandStr.toUtf8());
    if (!m_socket.waitForBytesWritten()) {
        emit error("Failed to write into the socket.");
        return QString();
    }
    return cmd->m_prefix;
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
    if (loggedIn() && m_commands.contains(m_accessPrefix)) {
        QString responseStr = m_commands.value(m_accessPrefix);
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
