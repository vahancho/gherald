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

#include <QString>
#include <QDesktopServices>
#include "versionManager.h"
#include "../strings/strings.h"

namespace core
{

VersionManager::VersionManager()
    :
        m_currentVersion(str::sVersion),
        m_versionUrl(str::sVersionUrl)
{
    connect(&m_http, SIGNAL(readyRead(const QHttpResponseHeader &)), this,
            SLOT(fetchHttpData(const QHttpResponseHeader &)));

    connect(&m_http, SIGNAL(done(bool)),
            this, SLOT(onHttpDone(bool)));

    m_http.setHost(m_versionUrl.host());
}

VersionManager::~VersionManager()
{
    m_http.abort();
}

QString VersionManager::currentVersion() const
{
    return QString::fromStdString(m_currentVersion.toString());
}

QString VersionManager::updatedVersion() const
{
    return QString::fromStdString(m_updatedVersion.toString());
}

void VersionManager::checkForUpdates()
{
    m_http.get(m_versionUrl.path());
}

bool VersionManager::updatesAvailable() const
{
    return m_updatedVersion > m_currentVersion;
}

void VersionManager::fetchHttpData(const QHttpResponseHeader &resp)
{
    if (resp.statusCode() == 200) {
        QByteArray newVersion = m_http.readAll();
        QList<QByteArray> tokens = newVersion.split('\n');
        if (tokens.size() > 1) {
            m_updatedVersion.fromString(tokens.at(0).trimmed().data());
            m_downloadUrl = tokens.at(1).trimmed();
        }
        emit checked();
    } else {
        // Done with fail.
        m_http.abort();
    }
}

void VersionManager::onHttpDone(bool error)
{
    // Reset updated version on Http error.
    if (error)
        m_updatedVersion.reset();
}

void VersionManager::download() const
{
    QDesktopServices::openUrl(QUrl(m_downloadUrl, QUrl::TolerantMode));
}

} // namespace core
