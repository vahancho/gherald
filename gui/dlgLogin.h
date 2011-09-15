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

#ifndef __DLGLOGIN_H__
#define __DLGLOGIN_H__

#include <QDialog>

class QCheckBox;
class QLineEdit;

namespace gui
{

class DlgLogin : public QDialog
{
    Q_OBJECT

public:

    DlgLogin(QWidget *parent = 0, Qt::WindowFlags f = 0);

    ~DlgLogin();

    QString user() const;

    void setUser(const QString &user);

    QString password() const;

    void setPassword(const QString &pass);

    bool saveLogin() const;

private:

    QLineEdit *m_leName;

    QLineEdit *m_lePassword;

    QCheckBox *m_chkSave;
};

} // namespace gui

#endif // __DLGLOGIN_H__
