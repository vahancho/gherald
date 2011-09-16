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

#include <QStringList>
#include "version.h"

namespace core
{

const QString sNumSeparator('.');
const QString sTokenSeparator(' ');

Version::Version()
    :
        m_major(0),
        m_minor(0),
        m_buildNum(0)
{}

Version::Version(const QString &verStr)
{
    fromString(verStr);
}

Version::~Version()
{}

bool Version::fromString(const QString &verStr)
{
    QStringList verTokens = verStr.split(sTokenSeparator, QString::SkipEmptyParts);

    // Check for format.
    if (verTokens.size() < 1 || verTokens.size() > 2)
        return false;

    if (verTokens.size() == 2)
        m_mode = verTokens.last();

    QStringList numTokens = verTokens.first().split(sNumSeparator, QString::SkipEmptyParts);

    // Check for format correctness - it should be three separated numbers.
    if (numTokens.size() == 3)
    {
        bool converted = false;

        int verNum = numTokens.at(0).toInt(&converted);

        if (converted)
            m_major = verNum;
        else
            return false;

        verNum = numTokens.at(1).toInt(&converted);

        if (converted)
            m_minor = verNum;
        else
            return false;

        verNum = numTokens.at(2).toInt(&converted);

        if (converted)
            m_buildNum = verNum;
        else
            return false;

        return true;
    }
    else
        return false;
}

QString Version::toString() const
{
    return QString("%1%2%3%4%5%6%7")
                    .arg(m_major)
                    .arg(sNumSeparator)
                    .arg(m_minor)
                    .arg(sNumSeparator)
                    .arg(m_buildNum)
                    .arg(sTokenSeparator)
                    .arg(m_mode);
}

void Version::reset()
{
    m_major = 0;
    m_minor = 0;
    m_buildNum = 0;
    m_mode = QString();
}

bool Version::operator>(const Version &ver) const
{
    if (m_major > ver.m_major ||
        m_minor > ver.m_minor ||
        m_buildNum > ver.m_buildNum)
    {
        return true;
    }
    else
        return false;
}

} // namespace core
