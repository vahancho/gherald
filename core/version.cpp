/**************************************************************************
*   Copyright (C) 2011 by Vahan Aghajanyan                                *
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

#include <stdlib.h>
#include <sstream>
#include "version.h"

namespace core
{

const char sNumSeparator = '.';

Version::Version()
{}

Version::Version(const std::string &verStr)
{
    fromString(verStr);
}

Version::~Version()
{}

void Version::fromString(const std::string &verStr)
{
    reset();

    size_t prev = 0;
    size_t pos = 0;

    while(pos < verStr.size()) {
        pos = verStr.find(sNumSeparator, prev);
        std::string s = verStr.substr(prev, pos - prev);
        unsigned int i = atoi(s.c_str());
        m_versions.push_back(i);
        prev = pos + 1;
    }

    pos = verStr.rfind(sNumSeparator);
    while (isdigit(verStr[++pos]))
    {}

    m_mode.clear();
    m_mode = verStr.substr(pos);
}

std::string Version::toString() const
{
    std::string retStr;
    for (size_t i = 0; i < m_versions.size(); ++i) {
        if (i != 0)
            retStr.push_back(sNumSeparator);
        std::ostringstream sin;
        sin << m_versions[i];
        retStr.append(sin.str());
    }
    retStr += m_mode;

    return retStr;
}

void Version::reset()
{
    m_versions.clear();
    m_mode.clear();
}

bool Version::operator>(const Version &ver) const
{
    for (size_t i = 0; i < m_versions.size(); i++) {
        unsigned int thisVersion = m_versions.at(i);
        unsigned int thatVersion = 0;
        if (i < ver.m_versions.size())
            thatVersion = ver.m_versions.at(i);

        if (thisVersion > thatVersion)
            return true;
        else if (thisVersion < thatVersion)
            return false;
    }

    return false;
}

} // namespace core
