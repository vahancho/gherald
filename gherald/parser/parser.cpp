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

#include "parser.h"

namespace core
{

Parser::Parser()
    :
        m_mailCount(0),
        m_needLogin(true)
{}

Parser::~Parser()
{}

bool Parser::needLogin() const
{
    return m_needLogin;
}

const QList<MailEntry> &Parser::mailEntries() const
{
    return m_mailEntries;
}

QStringList Parser::header() const
{
    return m_header;
}

int Parser::newMailCount() const
{
    return m_mailCount;
}

QString Parser::status() const
{
    return m_status;
}

void Parser::setUser(const QString &userName)
{
    m_user = userName;
}

void Parser::setPassword(const QString &password)
{
    m_password = password;
}

QString Parser::user() const
{
    return m_user;
}

QString Parser::password() const
{
    return m_password;
}

} // namespace core
