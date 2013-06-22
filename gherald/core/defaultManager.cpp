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

#include <QSettings>
#include <QApplication>
#include <QDesktopservices>
#include <QDir>
#include "defaultManager.h"

namespace core
{

const QString defaultsFileName("settings.ini");

DefaultManager::DefaultManager(QObject *parent)
    :
        QObject(parent)
{}

QString DefaultManager::file() const
{
    QString location =
        QDesktopServices::storageLocation(QDesktopServices::DataLocation) +
        '/' + defaultsFileName;
    return QDir::toNativeSeparators(location);
}

QVariant DefaultManager::getDefault(const QString &name,
                                    const QVariant &defaultValue) const
{
    QString settingsFile = file();
    QString oldSettingsFile = QCoreApplication::applicationDirPath() +
                              '/' + defaultsFileName;

    // Settings file of old versions of the tool was located in the same
    // directory with the executable. For the sake of compatibility
    // copy it from the old location to the new one.
    if (!QFile::exists(settingsFile) && QFile::exists(oldSettingsFile)) {
        QString oldSettings = QCoreApplication::applicationDirPath() +
                              '/' + defaultsFileName;
        QFile::copy(oldSettings, settingsFile);
    }

    QSettings defaults(settingsFile, QSettings::IniFormat);
    return defaults.value(name, defaultValue);
}

void DefaultManager::setDefault(const QString &name, const QVariant &value)
{
    QSettings defaults(file(), QSettings::IniFormat);
    defaults.setValue(name, value);
}

} // namespace core
