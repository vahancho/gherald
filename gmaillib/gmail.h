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

#ifndef __GMAIL_H__
#define __GMAIL_H__

#include <QObject>
#include <QAbstractSocket>
#include <QSslSocket>
#include <QEventLoop>
#include <QQueue>
#include <QBuffer>
#include "export.h"

class GMAILLIB_EXPORT Gmail : public QObject
{
    Q_OBJECT
public:
    Gmail(QObject *parent = 0);
    ~Gmail();

    void login(const QString &user, const QString &pass);
    int unreadCount();

public slots:
    /// Start parsing for the new mail.
    virtual void parse();

private slots:
    void updateEnabledState();
    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketReadyRead();
    void onProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *auth);

    void sslErrors(const QList<QSslError> &errors);

private:
    void connect();
    void sendCommand(const QString &command);
    QString prefix(const QString &line) const;

    QSslSocket m_socket;
    QEventLoop m_eventLoop;

    /// Map between command and its responce.
    QMap<QString, QString> m_commands;

    struct Command
    {
        Command()
        {
            m_buffer = new QBuffer();
        }
        ~Command()
        {
            delete m_buffer;
        }

        QString m_prefix;
        QBuffer *m_buffer;
    };
    QQueue<Command *> m_commandQueue;
};

#endif // __GMAIL_H__
