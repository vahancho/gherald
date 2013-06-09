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

Gmail::Gmail(QObject *parent)
    :
        QObject(parent)
{
    QObject::connect(&m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    QObject::connect(&m_socket, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(sslErrors(QList<QSslError>)));
    QObject::connect(&m_socket, SIGNAL(readyRead()),
            this, SLOT(socketReadyRead()));
    QObject::connect(&m_socket, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)),
            this, SLOT(onProxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)));
}

Gmail::~Gmail()
{
    m_socket.abort();
}

void Gmail::connect()
{
    m_socket.connectToHostEncrypted("imap.gmail.com", 993);
    if (!m_socket.waitForEncrypted()) {
        qDebug() << m_socket.errorString();
        m_socket.abort();
    }
}

void Gmail::login(const QString &user, const QString &pass)
{
    QString loginStr = QString("LOGIN %1 %2").arg(user).arg(pass);
    sendCommand(loginStr);

    // Need to wait, until we are logged in.
    m_eventLoop.exec();
}

int Gmail::unreadCount()
{
    sendCommand("EXAMINE INBOX");
    sendCommand("SEARCH UNSEEN");
    return 0;
}

void Gmail::markUnread(int id)
{
    sendCommand("SELECT INBOX"); // Read-write access.

    QString setFlag = QString("SELECT %1 +FLAGS (\SEEN)").arg(id);
    sendCommand(setFlag);
}

QList<int> Gmail::unreadMessages()
{
    sendCommand("EXAMINE INBOX");
    sendCommand("SEARCH UNSEEN");
    return QList<int>();
}

void Gmail::updateEnabledState()
{
    bool unconnected = m_socket.state() == QAbstractSocket::UnconnectedState;
    bool connected = m_socket.state() == QAbstractSocket::ConnectedState;
}

void Gmail::parse()
{
    bool connected = m_socket.state() == QAbstractSocket::ConnectedState;

    if (connected) {
        // Get INBOX info by message flags
        // Unread count in inbox.
        sendCommand("STATUS INBOX (MESSAGES UNSEEN RECENT)");
        //sendData(". SELECT INBOX"); // read-write
        sendCommand("EXAMINE INBOX"); // read-only
        // Get the indexes of unseen messages
        sendCommand("SEARCH UNSEEN");
        // Fetch only the body of 88th message
        // . FETCH 88 BODY.PEEK[1]
    }
}

void Gmail::socketStateChanged(QAbstractSocket::SocketState state)
{
    if (state == QAbstractSocket::UnconnectedState)
    {
        //m_socket->deleteLater();
        //m_socket = 0;
    }
}

void Gmail::onProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *auth)
{
}

void Gmail::socketReadyRead()
{
    //QString received = QString::fromUtf8(m_socket.readAll());
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
            delete cmd;
            return;
        }
    }
    // Remove data that has been stored from the buffer.
    qint64 pos = cmd->m_buffer->pos();
    cmd->m_buffer->close();
    cmd->m_buffer->setData(QByteArray(cmd->m_buffer->data()).remove(0, pos));
}

void Gmail::sendCommand(const QString &command)
{
    // Always connect to the server if not connected yet.
    if (m_socket.state() != QAbstractSocket::ConnectedState) {
        connect();
    }

    static int index = 1;
    Command *cmd = new Command;
    cmd->m_prefix = QString("cmd%1").arg(index++);
    m_commandQueue.enqueue(cmd);
    m_commands[cmd->m_prefix] = QString();

    QString commandStr = QString("%1 %2\r\n").arg(cmd->m_prefix).arg(command);
    m_socket.write(commandStr.toUtf8());
    if (!m_socket.waitForBytesWritten()) {
        qDebug() << "failed to write bytes:" << commandStr;
    }
}

void Gmail::sslErrors(const QList<QSslError> &errors)
{
    foreach (const QSslError &error, errors)
        qDebug(error.errorString().toAscii().data());

    m_socket.ignoreSslErrors();

    // did the socket state change?
    if (m_socket.state() != QAbstractSocket::ConnectedState)
        socketStateChanged(m_socket.state());
}

QString Gmail::prefix(const QString &line) const
{
    QStringList tokens = line.split(' ');
    return tokens.first();
}
