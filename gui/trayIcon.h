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

#ifndef __TRAYICON_H__
#define __TRAYICON_H__

#include <QSystemTrayIcon>
#include <QTimer>
#include "notifier.h"
#include <core/defaultManager.h>
#include <core/versionManager.h>
#include <core/login.h>
#include <parser/parser.h>

class QTranslator;

namespace core
{
    class Parser;
}

namespace gui
{

/// Implements iterative notification.
class Scheduler : public QObject
{
    Q_OBJECT
public:
    Scheduler(Notifier *notifier);

    /// Sets the list of mail items to show.
    void setMail(const QList<core::MailEntry> &mail);

    /// Show iteratively all mail items.
    void show();

private slots:
    void showNext();

private:
    Notifier *m_notifier;
    QList<core::MailEntry> m_mail;
    int m_current;
};

///////////////////////////////////////////////////////////////////////////////

/// This class implements the main GUI of the application.
class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:

    TrayIcon();

    ~TrayIcon();

private slots:

    /// When tray icon activated.
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

    /// Opens Gmail inbox in browser.
    void onViewInbox();

    /// Timer handles the parsing period.
    void onParseTimer();

    /// Timer handles tray icon flickering on unread mail.
    void onIconTimer();

    /// This slot called when data downloaded and parsed.
    void onParsingDone(bool error);

    /// This slot called on parser's status change.
    void onStatusChanged();

    /// This shows the notification window.
    void onShowNotification();

    /// This slot called when menu action for changing user is triggered.
    void onChangeUser();

    /// Opens the Options dialog.
    void onOptions();

    /// Shows application's "About" message.
    void onAbout();

private:
    /// Translates the application.
    void translate(const QString &language);

    /// Returns translation file based on language.
    QString translationFile(const QString &language) const;

    /// Creates context menu.
    void createMenu();

    /// Saves the application state.
    void saveState();

    /// Restores saved application state in the current session.
    void restoreState();

    /// Icons
    QIcon m_noUnreadIcon;
    QIcon m_newMailIcon;
    QIcon m_failureIcon;
    QIcon m_emptyIcon;

    /// The notifier window.
    Notifier m_notifier;

    Scheduler m_schduler;

    /// The HTTP parser.
    core::Parser *m_parser;

    /// The default manager.
    core::DefaultManager m_defaultManager;

    /// The version manager.
    core::VersionManager m_versionManager;

    /// The translator.
    QTranslator *m_translator;

    /// Stores login data.
    core::Login m_login;

    /// The parsing timer.
    QTimer m_parseTimer;

    /// The icon timer.
    QTimer m_iconTimer;

    /// Store the unread email count on the last parsing.
    int m_lastMailCount;
};

} // namespace gui

#endif // __TRAYICON_H__
