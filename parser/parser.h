/**************************************************************************
*   Copyright (C) 2010 by Vahan Aghajanyan                                *
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

#ifndef __PARSER_H__
#define __PARSER_H__

#include <QStringList>
#include "mailEntry.h"

namespace core
{

class Parser : public QObject
{
    Q_OBJECT

public:

    Parser();

    ~Parser();

    /// Returns the unread mail count.
    int newMailCount() const;

    /// Returns the header information of the mail box.
    QStringList header() const;

    /// Returns all parsed mail entries.
    const QList<MailEntry> & mailEntries() const;

    /// Returns the HTTP status of the parser.
    QString status() const;

    bool needLogin() const;

    /// Sets the user name.
    void setUser(const QString &userName);

    /// Returns decoded user name.
    QString user() const;

    /// Sets user password.
    void setPassword(const QString &password);

    /// Returns encoded user password.
    QString password() const;

public slots:

    /// Start parsing for the new mail.
    virtual void parse() = 0;

signals:

    /// Parsing done.
    void done(bool error);

    /// Status changed.
    void statusChanged();

protected:

    /// Current mail entry.
    MailEntry m_mailEntry;

    /// All mail entries parsed.
    QList<MailEntry> m_mailEntries;

    QStringList m_header;

    int m_mailCount;

    QString m_user;

    QString m_password;

    /// Stores the status string.
    QString m_status;

    bool m_needLogin;
};

} // namespace core

#endif // __PARSER_H__
