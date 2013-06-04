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

#include "mailEntry.h"

namespace core
{

void MailEntry::addAuthor(const QString &name)
{
    m_author.m_name = name;
}

void MailEntry::addTime(const QString &time)
{
    // Get UTC time and convert it to the local time.
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);

    dt.setTimeSpec(Qt::UTC);
    dt = dt.toLocalTime();

    if (dt.isValid())
        m_time = dt;
}

void MailEntry::addSubject(const QString &subject)
{
    m_subject = subject;
}

void MailEntry::addSummary(const QString &summary)
{
    m_summary = summary;
}

void MailEntry::addContributor(const QString &name)
{
    Sender sender;
    sender.m_name = name;

    m_contributors << sender;
}

void MailEntry::clear()
{
    m_time = QDateTime();
    m_summary.clear();
    m_subject.clear();
    m_author.clear();
    m_contributors.clear();
}

QString MailEntry::toString() const
{
    QString from = m_author.m_name;

    foreach (const Sender &contributor, m_contributors)
        from += ", " + contributor.m_name;

    return QString("%1 <small style=\"font-family:courier;color:red\">>></small> <B>%2<BR>%3<BR></B>"
                   "<I>%4</I></a>")
                   .arg(m_time.toString("MMM d hh:mm"))
                   .arg(from)
                   .arg(m_subject)
                   .arg(m_summary);
}

} // namespace core
