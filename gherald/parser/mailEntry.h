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

#ifndef __MAILENTRY_H__
#define __MAILENTRY_H__

#include <QString>
#include <QDateTime>

namespace core
{

/// This class implements mail item that holds all available information.
class MailEntry
{
public:

    /// Adds mail author.
    void addAuthor(const QString &name);

    /// Adds mail time.
    void addTime(const QString &time);

    /// Adds mail subject.
    void addSubject(const QString &subject);

    /// Add mail summary - the brief of the mail body.
    void addSummary(const QString &summary);

    /// Add contributor to the contributors list.
    void addContributor(const QString &name);

    /// Clears all data.
    void clear();

    /// Return the formated string representation of mail.
    QString toString() const;

private:

    /// The sender.
    struct Sender
    {
        void clear()
        {
            m_name.clear();
            m_email.clear();
        }

        QString m_name;
        QString m_email;
    };

    /// Store the author.
    Sender m_author;

    /// Store the list of contributors.
    QList<Sender> m_contributors;

    /// Store mail attributes.
    QDateTime m_time;
    QString m_summary;
    QString m_subject;

};

} // namespace core

#endif // __MAILENTRY_H__
