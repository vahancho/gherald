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

#ifndef __DLGOPTIONS_H__
#define __DLGOPTIONS_H__

#include <QDialog>

class QCheckBox;
class QSpinBox;
class QLineEdit;
class QComboBox;

namespace gui
{

class DlgOptions : public QDialog
{
    Q_OBJECT

public:

    DlgOptions(QWidget * parent = 0, Qt::WindowFlags f = 0);

    ~DlgOptions();

    void setTimout(int timout);

    int timout() const;

    void setRunOnStartUp(bool run = true);

    bool runOnStartUp() const;

    QString soundFilePath() const;

    void setSoundFilePath(const QString &path);

    QString currentLanguage() const;

    void setLanguages(const QStringList &languages, int currentLanguage);

    bool playSound() const;

    void setPlaySound(bool play) const;

private slots:

    void onPlayStateChanged(int state);

    void onFileBrowse();

private:

    void setSoundGuiEnabled(bool enable) const;

    QCheckBox *m_chkRunStartUp;

    QCheckBox *m_chkPlaySound;

    QPushButton *m_browse;

    QSpinBox *m_spnTimeout;

    QLineEdit *m_leSoundFilePath;

    QComboBox *m_cbLanguage;

    /// Store the last opened directory for file browsing.
    QString m_lastBrowseDir;
};

} // namespace gui

#endif // __DLGOPTIONS_H__
