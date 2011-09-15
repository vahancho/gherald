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

#include "propertyTable.h"

namespace core
{

PropertyTable::PropertyTable(QObject *parent)
    :
        QObject(parent)
{}

PropertyTable::PropertyTable(const PropertyTable &other)
{
    // Reparent
    setParent(other.parent());
    // We just copy one container from another
    m_properties = other.m_properties;
}

PropertyTable::~PropertyTable()
{
    // clear properties
    m_properties.clear();
}

void PropertyTable::addProperty(const QString &name,
                                const QVariant &value,
                                const QVariant &defaultValue)
{
    Property pD(name, value, defaultValue);
    m_properties[name] = pD;
}

QVariant PropertyTable::value(const QString &name) const
{
    if (m_properties.contains(name))
    {
        Property prop = m_properties.value(name);
        return prop.value();
    }
    else
        return QVariant();
}

void PropertyTable::setValue(const QString &name, const QVariant &value)
{
    QHash<QString, Property>::iterator i = m_properties.find(name);

    if(i != m_properties.end() && i.key() == name)
    {
        if(value.isValid())
            i.value().setValue(value);
        else
            i.value().resetToDefault();

        // emit signal about default changes
        emit propertyChanged(name, i.value().value());
    }
}

void PropertyTable::resetToDefault()
{
    QHash<QString, Property>::iterator i = m_properties.begin();

    while (i != m_properties.end())
    {
        // Reset the property value
        i.value().resetToDefault();

        // emit signal about property changes
        emit propertyChanged(i.value().name(), i.value().value());
        ++i;
    }
}

QString PropertyTable::toString() const
{
    // Sort the properties by the names
    QMap<QString, QVariant> sortedProperties = propertyMap();

    // Now construct the resulting string using sorted properties values.
    QMap<QString, QVariant>::const_iterator i = sortedProperties.constBegin();
    QString ret;

    while (i != sortedProperties.constEnd())
    {
        ret.append(QString("%1:\t%2\n").arg(i.key()).arg(i.value().toString()));
        ++i;
    }

    return ret;
}

QMap<QString, QVariant> PropertyTable::propertyMap() const
{
    QHash<QString, Property>::const_iterator i = m_properties.constBegin();
    QMap<QString, QVariant> propMap;

    while (i != m_properties.constEnd())
    {
        Property prop = i.value();
        propMap[prop.name()] = prop.value();
        ++i;
    }

    return propMap;
}

} // namespace core