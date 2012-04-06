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

#ifndef __VERSIONMANAGER_H__
#define __VERSIONMANAGER_H__

#include <QHttp>
#include <QUrl>
#include "version.h"

namespace core
{

/// Implements the version manager class.
class VersionManager : public QObject
{
    Q_OBJECT

public:
    VersionManager();

    ~VersionManager();

    QString currentVersion() const;

    QString updatedVersion() const;

    void checkForUpdates();

    bool updatesAvailable() const;

    /// Downloads the software.
    void download() const;
signals:
    /// This signal emitted when version check finished.
    void checked();

private slots:
    /// Fetches the fetched data from HTTP.
    void fetchHttpData(const QHttpResponseHeader &resp);

    /// This function called when fetching is done.
    void onHttpDone(bool error);

private:
    Version m_currentVersion;

    Version m_updatedVersion;

    /// Http
    QHttp m_http;

    /// Stores the version file url.
    QUrl m_versionUrl;

    /// Stores the software download url.
    QString m_downloadUrl;
};

} // namespace core

#endif // __VERSIONMANAGER_H__
