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

#include <QUrl>
#include "atomParser.h"
#include "../strings/strings.h"
#include "../strings/guiStrings.h"

namespace core
{

AtomParser::AtomParser()
{
    connect(&m_http, SIGNAL(readyRead(const QHttpResponseHeader &)), this,
            SLOT(fetchHttpData(const QHttpResponseHeader &)));

    connect(&m_http, SIGNAL(done(bool)),
            this, SLOT(onHttpDone(bool)));

    connect(&m_http, SIGNAL(stateChanged(int)),
            this, SLOT(onStateChanged(int)));

    QUrl url(str::sFeedUrl);

    m_http.setHost(url.host(), QHttp::ConnectionModeHttps);
}

AtomParser::~AtomParser()
{
    if (m_http.state() != QHttp::Unconnected)
        m_http.abort();
}

void AtomParser::parse()
{
    if (m_http.state() != QHttp::Unconnected)
        m_http.abort();

    m_http.setUser(m_user, m_password);

    m_mailCount = 0;
    m_header.clear();
    m_mailEntries.clear();
    m_mailEntry.clear();
    m_xml.clear();
    m_isHeader = true;
    m_needLogin = false;

    m_requestId = m_http.get(str::sFeedPath);
}

void AtomParser::onStateChanged(int state)
{
    switch (state)
    {
    case QHttp::HostLookup :
        m_status = TRANSLATE(str::sHostLookup);
        break;
    case QHttp::Connecting :
        m_status = TRANSLATE(str::sConnecting);
        break;
    case QHttp::Sending :
        m_status = TRANSLATE(str::sSending);
        break;
    case QHttp::Reading :
        m_status = TRANSLATE(str::sReading);
        break;
    case QHttp::Connected :
        m_status = TRANSLATE(str::sConnected);
        break;
    case QHttp::Unconnected :
    case QHttp::Closing :
    default:
        break;
    }

    emit statusChanged();
}

void AtomParser::fetchHttpData(const QHttpResponseHeader &resp)
{
    if (resp.statusCode() == 200)
    {
        m_xml.addData(m_http.readAll());

        // Abort HTTP if XML parsing failed.
        if(!parseXmlData())
            m_http.abort();
    }
    else
    {
        // Done with fail.
        m_status = m_http.errorString();
        m_http.abort();
    }
}

void AtomParser::onHttpDone(bool error)
{
    m_needLogin = (m_http.error() == QHttp::AuthenticationRequiredError);

    if (error)
        m_status = m_http.errorString();
    else
        m_status.clear();

    emit done(error);
}

bool AtomParser::parseXmlData()
{
    while (!m_xml.atEnd())
    {
        m_xml.readNext();

        if (m_xml.isStartElement())
        {
            m_currentTag = m_xml.name().toString();

            if(m_currentTag == str::sTagEntry)
            {
                m_isHeader = false;
                m_isContributor = false;
                m_mailEntry.clear();
            }
            else if (m_currentTag == str::sTagContributor)
                m_isContributor = true;
        }
        else if (m_xml.isEndElement())
        {
            if (m_xml.name() == str::sTagEntry)
            {
                // Save parsed data
                m_mailEntries.push_back(m_mailEntry);

                // Clear temporary data
                m_mailEntry.clear();
            }
        }
        else if (m_xml.isCharacters() && !m_xml.isWhitespace())
        {
            if (m_currentTag == str::sTagFullCount)
            {
                m_mailCount = m_xml.text().toString().toInt();
            }
            else if (m_currentTag == str::sTagTitle)
            {
                if (m_isHeader)
                    m_header << m_xml.text().toString();
                else
                    m_mailEntry.addSubject(m_xml.text().toString());
            }
            else if (m_currentTag == str::sTagTagLine)
            {
                m_header << m_xml.text().toString();
            }
            else if (m_currentTag == str::sTagModified)
            {
                if (m_isHeader)
                    m_header << m_xml.text().toString();
                else
                    m_mailEntry.addTime(m_xml.text().toString());
            }
            else if (m_currentTag == str::sTagSummary)
                m_mailEntry.addSummary(m_xml.text().toString());
            else if (m_currentTag == str::sTagName)
            {
                if (m_isContributor)
                    m_mailEntry.addContributor(m_xml.text().toString());
                else
                    m_mailEntry.addAuthor(m_xml.text().toString());
            }
        }
    }

    if (m_xml.error() && m_xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) 
    {
        m_status = QString("XML ERROR: %1 : %2").arg(m_xml.lineNumber()).arg(m_xml.errorString());

        return false;
    }
    else
        return true;
}

} // namespace core
