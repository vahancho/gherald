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

#ifndef __ATOMPARSER_H__
#define __ATOMPARSER_H__

#include <QHttp>
#include <QXmlStreamReader>

#include "parser.h"

namespace core
{

class AtomParser : public Parser
{
    Q_OBJECT

public:

    AtomParser();

    ~AtomParser();

public slots:

    /// Start parsing for the new mail.
    void parse();

private slots:

    /// Fetches the fetched data from HTTP.
    void fetchHttpData(const QHttpResponseHeader &resp);

    /// This function called when fetching is done.
    void onHttpDone(bool error);

    void onStateChanged(int state);

    /// Parse the XML data.
    bool parseXmlData();

private:
    bool m_isHeader;
    bool m_isContributor;

    int m_requestId;

    /// Current XML tag.
    QString m_currentTag;

    /// The XML reader.
    QXmlStreamReader m_xml;

    /// Http
    QHttp m_http;
};

} // namespace core

#endif // __ATOMPARSER_H__
