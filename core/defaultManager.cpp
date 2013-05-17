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
        PropertyTable(parent)
{}

DefaultManager::~DefaultManager()
{}

void DefaultManager::saveDefaults() const
{
    QHash<QString, Property>::const_iterator i = m_properties.constBegin();

    QSettings defaultsWriter(file(), QSettings::IniFormat);

    while (i != m_properties.constEnd()) {
        defaultsWriter.setValue(i.value().name(), i.value().value());
        ++i;
    }
}

void DefaultManager::readDefaults()
{
    QString settingsFile = file();

    // The old versions settings file was located in the same
    // directory with the executable. For the sake of compatibility
    // try to read it from the old location if new file does not exist.
    // However we will write settings in the new location anyways.
    if (!QFile::exists(settingsFile)) {
        settingsFile = QCoreApplication::applicationDirPath() +
                       '/' + defaultsFileName;
    }

    // Create QSettings object. We use ini format to store the data.
    QSettings defaultsReader(file(), QSettings::IniFormat);

    QHash<QString, Property>::iterator i = m_properties.begin();

    while (i != m_properties.end()) {
        QString defaultName = i.key();
        QVariant defaultValue = defaultsReader.value(defaultName);

        // Set the new value we read from the file
        setValue(defaultName, defaultValue);
        ++i;
    }
}

QString DefaultManager::file() const
{
    QString location =
        QDesktopServices::storageLocation(QDesktopServices::DataLocation) +
        '/' + defaultsFileName;
    return QDir::toNativeSeparators(location);
}

} // namespace core
