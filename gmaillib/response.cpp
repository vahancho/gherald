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

#include <QStringList>
#include "response.h"

Response::Response(const QString &input)
    :
        m_status(Unknown)
{
    parse(input);
}

Response::Status Response::status() const
{
    return m_status;
}

QString Response::statusMessage() const
{
    return m_statusMessage;
}

QStringList Response::info() const
{
    return m_info;
}

void Response::parse(const QString &input)
{
    QStringList lines = input.trimmed().split("\r\n");
    Q_ASSERT(lines.size() >= 1);

    QString lastLine = lines.last();
    QStringList tokens = lastLine.split(' ');

    if (tokens.size() < 3) {
        return;
    }

    QString result = tokens.at(1);
    if (result == "OK") {
        m_status = Ok;
    } else if (result == "NO") {
        m_status = No;
    } else if (result == "BAD") {
        m_status = Bad;
    } else {
        m_status = Unknown;
    }

    m_statusMessage = lastLine.section(' ', 2);

    lines.removeLast();
    m_info = lines;
}
