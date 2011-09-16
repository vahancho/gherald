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
    if (verTokens.size() != 1 && verTokens.size() != 2)
        return false;

    reset();

    if (verTokens.size() == 2)
        m_mode = verTokens.last();

    QStringList numTokens = verTokens.first().split(sNumSeparator, QString::SkipEmptyParts);
    foreach (const QString &token, numTokens) {
        m_versions.push_back(toVersionNumber(token));
    }

    return true;
}

QString Version::toString() const
{
    QString versionString;
    for (int i = 0; i < m_versions.size(); i++) {
        if (i != 0)
            versionString += sNumSeparator;

        versionString += QString::number(m_versions.at(i));
    }

    versionString += sTokenSeparator + m_mode;
    return versionString;
}

void Version::reset()
{
    m_versions.clear();
    m_mode = QString();
}

bool Version::operator>(const Version &ver) const
{
    for (int i = 0; i < m_versions.size(); i++) {
        if (i < ver.m_versions.size()) {
            unsigned int thisVersion = m_versions.at(i);
            unsigned int thatVersion = ver.m_versions.at(i);
            if (thisVersion > thatVersion)
                return true;
            else if (thisVersion < thatVersion)
                return false;
        }
    }

    return false;
}

unsigned int Version::toVersionNumber(const QString &tokens) const
{
    bool converted = false;
    unsigned int verNum = tokens.toInt(&converted);
    if (converted)
        return verNum;
    else
        return 0;
}

} // namespace core
