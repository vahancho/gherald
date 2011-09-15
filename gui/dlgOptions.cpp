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

#include <QCheckBox>
#include <QLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QFileDialog>
#include <QComboBox>

#include "dlgOptions.h"
#include "../strings/guiStrings.h"

namespace gui
{

DlgOptions::DlgOptions(QWidget * parent, Qt::WindowFlags f)
    :
        QDialog(parent, f)
{
    // Sound file group
    m_chkPlaySound = new QCheckBox(TRANSLATE(str::sSound));
    connect(m_chkPlaySound, SIGNAL(stateChanged(int)), this, SLOT(onPlayStateChanged(int)));

    m_leSoundFilePath = new QLineEdit;
    m_leSoundFilePath->setMinimumWidth(200);

    m_browse = new QPushButton(TRANSLATE(str::sBrowse));
    m_browse->setMaximumWidth(25);
    connect(m_browse, SIGNAL(clicked()), this, SLOT(onFileBrowse()));

    QHBoxLayout *soundLayout = new QHBoxLayout;
    soundLayout->addWidget(m_leSoundFilePath);
    soundLayout->addWidget(m_browse);

    QVBoxLayout *mainSoundLayoyut = new QVBoxLayout;
    mainSoundLayoyut->addWidget(m_chkPlaySound);
    mainSoundLayoyut->addLayout(soundLayout);

    // Timeout group
    QLabel *lblTimeout = new QLabel(TRANSLATE(str::sTimeout));
    m_spnTimeout = new QSpinBox;
    m_spnTimeout->setMinimum(1);
    QLabel *lblTimeoutMin = new QLabel(TRANSLATE(str::sTimeoutMin));

    QHBoxLayout *timeoutLayout = new QHBoxLayout;
    timeoutLayout->addWidget(lblTimeout);
    timeoutLayout->addWidget(m_spnTimeout);
    timeoutLayout->addWidget(lblTimeoutMin);
    timeoutLayout->addStretch(1);

    QVBoxLayout *mailLayout = new QVBoxLayout;
    mailLayout->addLayout(mainSoundLayoyut);
    mailLayout->addLayout(timeoutLayout);

    QGroupBox *mailGroupBox = new QGroupBox(TRANSLATE(str::sMailGroup));
    mailGroupBox->setLayout(mailLayout);

    // Language group
    m_cbLanguage = new QComboBox;

    QHBoxLayout *languageLayout = new QHBoxLayout;
    languageLayout->addWidget(m_cbLanguage);
    languageLayout->addStretch(1);

    QGroupBox *languageGroupBox = new QGroupBox(TRANSLATE(str::sGuiLanguage));
    languageGroupBox->setLayout(languageLayout);

    // Run on startup group
    m_chkRunStartUp = new QCheckBox(TRANSLATE(str::sRun));

    // Create grid layout and add all widgets
    QGridLayout *grid = new QGridLayout;

    grid->addWidget(mailGroupBox, 0, 0);
    grid->addWidget(languageGroupBox, 1, 0);
    grid->addWidget(m_chkRunStartUp, 2, 0);

    // Dialog buttons
    QPushButton *btnOk = new QPushButton(TRANSLATE(str::sOk));
    btnOk->setDefault(true);
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *btnCancel = new QPushButton(TRANSLATE(str::sCancel));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch(1);
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(grid);
    mainLayout->addLayout(btnLayout);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(mainLayout);

    setWindowTitle(TRANSLATE(str::sOptionsTitle));
}

DlgOptions::~DlgOptions()
{}

bool DlgOptions::runOnStartUp() const
{
    return (m_chkRunStartUp->checkState() == Qt::Checked);
}

void DlgOptions::setRunOnStartUp(bool run)
{
    m_chkRunStartUp->setChecked(run);
}

void DlgOptions::setTimout(int timout)
{
    m_spnTimeout->setValue(timout);
}

int DlgOptions::timout() const
{
    return m_spnTimeout->value();
}

QString DlgOptions::soundFilePath() const
{
    return m_leSoundFilePath->text();
}

void DlgOptions::setSoundFilePath(const QString &path)
{
    m_leSoundFilePath->setText(path);
}

void DlgOptions::onFileBrowse()
{
    QFileDialog fd(this);

    fd.setDirectory(m_lastBrowseDir);
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setAcceptMode(QFileDialog::AcceptOpen);
    fd.setWindowTitle(TRANSLATE(str::sBrowseTitle));
    fd.setNameFilter(TRANSLATE(str::sFilter));

    if (fd.exec())
    {
        QStringList files = fd.selectedFiles();

        if (files.size() > 0)
        {
            m_leSoundFilePath->setText(files.at(0));
            m_lastBrowseDir = fd.directory().absolutePath();
        }
    }
}

QString DlgOptions::currentLanguage() const
{
    return m_cbLanguage->currentText();
}

void DlgOptions::setLanguages(const QStringList &languages, int currentLanguage)
{
    m_cbLanguage->addItems(languages);
    m_cbLanguage->setCurrentIndex(currentLanguage);
}

bool DlgOptions::playSound() const
{
    return m_chkPlaySound->isChecked();
}

void DlgOptions::setPlaySound(bool play) const
{
    m_chkPlaySound->setChecked(play);
    setSoundGuiEnabled(play);
}

void DlgOptions::onPlayStateChanged(int state)
{
    if (state == Qt::Checked)
        setSoundGuiEnabled(true);
    else
        setSoundGuiEnabled(false);
}

void DlgOptions::setSoundGuiEnabled(bool enable) const
{
    m_leSoundFilePath->setEnabled(enable);
    m_browse->setEnabled(enable);
}

} // namespace gui
