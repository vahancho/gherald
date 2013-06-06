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

#include <QtGui/QApplication>
#include <gui/trayIcon.h>
#include <strings/strings.h>
#include "../gmaillib/imap.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(str::sAppName);
    a.setQuitOnLastWindowClosed(false);

    a.setWindowIcon(QIcon(":/icons/app"));

    // Construct the GUI
    gui::TrayIcon trayIcon;
    trayIcon.show();

    int ret = a.exec();
    return ret;
}