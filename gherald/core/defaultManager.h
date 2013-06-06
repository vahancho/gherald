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

#ifndef __DEFAULTMANAGER_H__
#define __DEFAULTMANAGER_H__

#include <QObject>
#include <QVariant>

namespace core
{

/// Implements the application defaults repository.
/*!
    This class stores the application defaults - the application settings.
    Defaults can be saved and restored from session to session.
*/
class DefaultManager : public QObject
{
    Q_OBJECT

public:

    /// Default constructor
    DefaultManager(QObject *parent = 0);

    QVariant default(const QString &name,
                     const QVariant &defaultValue = QVariant()) const;
    void setDefault(const QString &name, const QVariant &value);

private:

    /// Returns defaults file's full path and name.
    QString file() const;
};

} // namespace core

#endif // __DEFAULTMANAGER_H__