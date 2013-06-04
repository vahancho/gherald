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

#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QCheckBox>
#include <QLineEdit>

#include "dlgLogin.h"
#include <strings/guiStrings.h>

namespace gui
{

DlgLogin::DlgLogin(QWidget * parent, Qt::WindowFlags f)
    :
        QDialog(parent, f)
{
    QLabel *lblName = new QLabel(TRANSLATE(str::sUser));
    m_leName = new QLineEdit;

    QLabel *lblPass = new QLabel(TRANSLATE(str::sPassword));
    m_lePassword = new QLineEdit;
    m_lePassword->setEchoMode(QLineEdit::Password);

    // Watch settings widgets.
    m_chkSave = new QCheckBox(TRANSLATE(str::sRememberPassword));

    // Create grid layout and add all widgets
    QGridLayout *grid = new QGridLayout;

    grid->addWidget(lblName, 0, 0);
    grid->addWidget(m_leName, 0, 1);
    grid->addWidget(lblPass, 1, 0);
    grid->addWidget(m_lePassword, 1, 1);
    grid->addWidget(m_chkSave, 2, 1);

    // Dialog buttons
    QPushButton *btnOk = new QPushButton(TRANSLATE(str::sOk));
    btnOk->setDefault(true);
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *btnCancel = new QPushButton(TRANSLATE(str::sCancel));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(grid);
    mainLayout->addLayout(btnLayout);
    mainLayout->addStretch(1);

    setMinimumWidth(450);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(mainLayout);

    setWindowTitle(TRANSLATE(str::sLoginTitle));
}

DlgLogin::~DlgLogin()
{}

QString DlgLogin::user() const
{
    return m_leName->text();
}

QString DlgLogin::password() const
{
    return m_lePassword->text();
}

bool DlgLogin::saveLogin() const
{
    return m_chkSave->isChecked();
}

void DlgLogin::setUser(const QString &user)
{
    m_leName->setText(user);
}

void DlgLogin::setPassword(const QString &pass)
{
    m_lePassword->setText(pass);
}

} // namespace gui
