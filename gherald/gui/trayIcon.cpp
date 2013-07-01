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

#include <QDesktopWidget>
#include <QTranslator>
#include <QLibraryInfo>
#include <QMenu>
#include <QThread>
#include <QSettings>
#include <QDesktopServices>
#include <QUrl>
#include <QSound>
#include <QDir>
#include <QMessageBox>
#include <QPainter>

#include "trayIcon.h"
#include "dlgLogin.h"
#include "dlgOptions.h"
#include <parser/atomParser.h>
#include <strings/guiStrings.h>
#include <strings/strings.h>

namespace gui
{

// Default values
const int defInterval = 2;
const int iconInterval = 700;

TrayIcon::TrayIcon()
    :
        m_translator(0),
        m_lastMailCount(0),
        m_reportNewVersion(false)
{
    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    connect(&m_notifier, SIGNAL(moved()), SLOT(onNotifierMoved()));
    connect(&m_notifier, SIGNAL(markAsRead(int)), SLOT(onMarkedAsRead(int)));

    m_parser = new core::AtomParser;

    // Menu
    createMenu();

    // Icons
    m_noUnreadIcon = QIcon(":/icons/app");
    m_newMailIcon = QIcon(":icons/new_mail");
    m_failureIcon = QIcon(":icons/failure");

    setToolTip(TRANSLATE(str::sNoUnreadMail));
    setIcon(m_failureIcon);

    // Parser
    connect(m_parser, SIGNAL(done(bool)), this, SLOT(onParsingDone(bool)));
    connect(m_parser, SIGNAL(statusChanged()), this, SLOT(onStatusChanged()));

    restoreState();

    // Timers
    connect(&m_parseTimer, SIGNAL(timeout()), this, SLOT(onParseTimer()));
    m_parseTimer.start();

    m_iconTimer.setInterval(iconInterval);
    connect(&m_iconTimer, SIGNAL(timeout()), this, SLOT(onIconTimer()));

    connect(&m_versionManager, SIGNAL(checked()), SLOT(onVersionChecked()));

    connect(&m_gmailClient, SIGNAL(done()), SLOT(onGmailDone()));
    connect(&m_gmailClient, SIGNAL(error(const QString &)),
            SLOT(onGmailError(const QString &)));

    onParseTimer();
    m_versionManager.checkForUpdates();
}

TrayIcon::~TrayIcon()
{
    m_parseTimer.stop();
    if (m_gmailClient.loggedIn()) {
        m_gmailClient.logout();
    }

    if (m_translator) {
        qApp->removeTranslator(m_translator);
        delete m_translator;
    }
}

void TrayIcon::createMenu()
{
    QMenu *menu = contextMenu();
    if (menu) {
        menu->clear();
    } else {
        menu = new QMenu;
        setContextMenu(menu);
    }

    QAction *act = menu->addAction(QIcon(":icons/view"), TRANSLATE(str::sMenuViewInbox), this, SLOT(onViewInbox()));

    // Create a bold font and set it for default action text
    QFont actionFont = QFont();
    actionFont.setBold(true);
    act->setFont(actionFont);

    menu->addAction(QIcon(":icons/check"), TRANSLATE(str::sMenuCheckMail), this, SLOT(onParseTimer()));
    menu->addAction(QIcon(":icons/refresh"), TRANSLATE(str::sMenuTellAgain), &m_notifier, SLOT(show()));
    menu->addAction(QIcon(":icons/user"), TRANSLATE(str::sMenuChangeUser), this, SLOT(onChangeUser()));
    menu->addAction(QIcon(":icons/options"), TRANSLATE(str::sMenuOptions), this, SLOT(onOptions()));
    menu->addSeparator();
    menu->addAction(QIcon(":icons/check_update"), TRANSLATE(str::sCheckUpdates), this, SLOT(onCheckUpdates()));
    menu->addAction(QIcon(":icons/about"), TRANSLATE(str::sMenuAbout), this, SLOT(onAbout()));
    menu->addSeparator();
    menu->addAction(QIcon(":icons/exit"), TRANSLATE(str::sMenuExit), qApp, SLOT(quit()));
}

void TrayIcon::restoreState()
{
    // Notifier position.
    QPoint center = QApplication::desktop()->screenGeometry().center();
    m_notifier.move(m_defaultManager.getDefault(str::sDefNotifyPos, center).toPoint());

    // Timers.
    int interval = m_defaultManager.getDefault(str::sDefInterval, defInterval).toInt();
    m_parseTimer.setInterval(qMax(interval, 1) * 60000);

    // Login.
    QString login = m_defaultManager.getDefault(str::sDefLogin, QString()).toString();
    m_login.decodeAndSet(login);
    m_parser->setUser(m_login.user());
    m_parser->setPassword(m_login.password());

    // Localization.
    QString language = m_defaultManager.getDefault(str::sDefLanguage, str::sLanguage).toString();
    translate(language);
}

void TrayIcon::translate(const QString &language)
{
    if (language == str::sLanguage) {
        qApp->removeTranslator(m_translator);
        delete m_translator;
    } else {
        QTranslator *oldTranslator = m_translator;

        QString file = translationFile(language);
        m_translator = new QTranslator;
        if (m_translator->load(file)) {
            qApp->installTranslator(m_translator);
            qApp->removeTranslator(oldTranslator);
            delete oldTranslator;
        } else {
            // If translator file cannot be loades, reset
            // translator to the old one.
            delete m_translator;
            m_translator = oldTranslator;
        }
    }

    // Recreate context menu to update actions strings.
    createMenu();
}

QString TrayIcon::translationFile(const QString &language) const
{
    return QLibraryInfo::location(QLibraryInfo::TranslationsPath) +
                                  '/' +
                                  QString("gherald_%1").arg(language);
}

void TrayIcon::onParseTimer()
{
    m_parser->parse();
    if (!m_gmailClient.loggedIn()) {
        m_gmailClient.login(m_login.user(), m_login.password());
    }

    if (m_gmailClient.loggedIn()) {
        m_gmailClient.sendUnreadMessages();
    }
}

void TrayIcon::onIconTimer()
{
    if (icon().isNull())
        setIcon(m_newMailIcon);
    else
        setIcon(m_emptyIcon);
}

void TrayIcon::onParsingDone(bool error)
{
    if (m_notifier.isVisible()) {
        return;
    }

    if (!error) {
        int count = m_parser->newMailCount();

        // Set the correct icon and tool tip.
        if (count > 0) {
            setIcon(m_newMailIcon);
            setComplexToolTip(TRANSLATE(str::sUnreadConversations).arg(count));
        } else {
            setComplexToolTip(TRANSLATE(str::sNoUnreadMail));
            setWarningIcon(!m_gmailError.isEmpty() || m_versionManager.updatesAvailable());
            m_notifier.setMessages(QStringList() <<
                                   QString(str::sReportTmplNoMail).arg(TRANSLATE(str::sUnreadLong)),
                                   false);
            m_iconTimer.stop();
        }

        // Show or not the notification?
        if (count > m_lastMailCount) {
            // Play sound and blink, blink the icon and show notification.
            if (m_defaultManager.getDefault(str::sDefSoundPlay, false).toBool()
                && QSound::isAvailable()) {
                QSound::play(m_defaultManager.getDefault(str::sDefSoundFile).toString());
            }

            m_iconTimer.start();
            onShowNotification();
        }

        m_lastMailCount = count;
    } else {
        QString status = m_parser->status();

        setComplexToolTip(status);
        setIcon(m_failureIcon);

        status = QString(str::sStatusMessageTmpl).arg(status);
        m_notifier.setMessages(QStringList() <<
                               QString(str::sReportTmplNoMail).arg(status),
                               false);

        if (m_parser->needLogin()) {
            DlgLogin dlg;
            dlg.setUser(m_parser->user());
            dlg.setPassword(m_parser->password());

            // Stop timer while user is going to input credentials.
            m_parseTimer.stop();

            if (dlg.exec() == QDialog::Accepted) {
                m_login.setUser(dlg.user());
                m_login.setPassword(dlg.password());

                m_parser->setUser(m_login.user());
                m_parser->setPassword(m_login.password());

                if (dlg.saveLogin()) {
                    m_defaultManager.setDefault(str::sDefLogin, m_login.encode());
                }

                // Restart parsing with new user data.
                m_parseTimer.start();
                onParseTimer();
            }
        } else {
            m_notifier.show();
        }
    }
}

void TrayIcon::onChangeUser()
{
    DlgLogin dlg;
    dlg.setUser(m_parser->user());
    dlg.setPassword(m_parser->password());

    if (dlg.exec() == QDialog::Accepted) {
        m_login.setUser(dlg.user());
        m_login.setPassword(dlg.password());
        m_parser->setUser(m_login.user());
        m_parser->setPassword(m_login.password());

        if (dlg.saveLogin()) {
            m_defaultManager.setDefault(str::sDefLogin, m_login.encode());
        } else {
            m_defaultManager.setDefault(str::sDefLogin, QString());
        }

        // Before proceed  logout from the old account.
        m_gmailClient.logout();
        onParseTimer();
        if (!m_parseTimer.isActive()) {
            m_parseTimer.start();
        }
    }
}

void TrayIcon::onStatusChanged()
{
    setComplexToolTip(m_parser->status());
}

void TrayIcon::onShowNotification()
{
    QStringList messages;
    const QList<core::MailEntry> &mails = m_parser->mailEntries();

    // New (unread) mail count can be any, however we show only limited
    // number of items (actually Gmail service shows only first 20
    // messages).
    const int count = mails.count();
    const int unreadCount = m_parser->newMailCount();
    for (int i = 0; i < count; ++i) {
        QString countReport = TRANSLATE(str::sMsgOfMsg).arg(i + 1).arg(unreadCount);
        QString m(TRANSLATE(str::sReportTmpl).arg(countReport).arg(mails.at(i).toString()));
        messages.append(m);
    }
    m_notifier.setMessages(messages, m_gmailClient.loggedIn());
    m_notifier.show();
}

void TrayIcon::onOptions()
{
    DlgOptions dlg;

#ifdef WIN32
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
                       QSettings::NativeFormat);

    dlg.setRunOnStartUp(!settings.value(str::sAppName).toString().isEmpty());
#endif

    // Set timeout
    dlg.setTimout(m_defaultManager.getDefault(str::sDefInterval).toInt());

    // Set sound file settings
    dlg.setSoundFilePath(m_defaultManager.getDefault(str::sDefSoundFile).toString());
    dlg.setPlaySound(m_defaultManager.getDefault(str::sDefSoundPlay, false).toBool());

    // Set language settings.
    QMap<QString, QVariant> defLanguages;
    defLanguages[str::sLanguageEnglishTitle] = str::sLanguageEnglishKey;
    defLanguages[str::sLanguageRussianTitle] = str::sLanguageRussianKey;
    QMap<QString, QVariant> languages =
        m_defaultManager.getDefault(str::sDefLanguages, defLanguages).toMap();
    QStringList languageStrings = languages.keys();

    QString currentLanguage =
        languages.key(m_defaultManager.getDefault(str::sDefLanguage, str::sLanguage).toString());
    int currentIndex = languageStrings.indexOf(currentLanguage);

    dlg.setLanguages(languages.keys(), currentIndex);

    if (dlg.exec() == QDialog::Accepted)
    {
        if (dlg.runOnStartUp())
        {
#ifdef WIN32
            QString appPath = QCoreApplication::applicationFilePath();
            appPath = QDir::toNativeSeparators(appPath);
            settings.setValue(str::sAppName, appPath);
#endif
        }
        else
#ifdef WIN32
            settings.remove(str::sAppName);
#endif

        // Set the mail checking interval
        m_defaultManager.setDefault(str::sDefInterval, dlg.timout());
        m_parseTimer.start(dlg.timout() * 60000);

        // Set the sound file path
        m_defaultManager.setDefault(str::sDefSoundFile, dlg.soundFilePath());
        m_defaultManager.setDefault(str::sDefSoundPlay, dlg.playSound());

        // Set the current language
        QVariant currentLanguageKey = languages.value(dlg.currentLanguage());
        m_defaultManager.setDefault(str::sDefLanguage, currentLanguageKey);

        // Translate GUI
        translate(currentLanguageKey.toString());
    }
}

void TrayIcon::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
        onViewInbox();
}

void TrayIcon::onViewInbox()
{
    // Provide password as a plain text here in url. This is not so safe!
    QString urlStr = QString(str::sMailBoxUrl).arg(m_parser->user())
                                              .arg(m_parser->password());
    QDesktopServices::openUrl(QUrl(urlStr, QUrl::TolerantMode));
}

void TrayIcon::onAbout()
{
    QString about = TRANSLATE(str::sAbout).arg(m_versionManager.currentVersion());

    if (m_versionManager.updatesAvailable())
        about += TRANSLATE(str::sNewVersion).arg(m_versionManager.updatedVersion());

    m_notifier.setMessages(QStringList() << QString(str::sReportTmplNoMail).arg(about),
                           false);
    m_notifier.show();
}

void TrayIcon::onCheckUpdates()
{
    m_reportNewVersion = true;
    m_versionManager.checkForUpdates();
}

void TrayIcon::onVersionChecked()
{
    if (!m_reportNewVersion) {
        return;
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle(TRANSLATE(str::sNewVersionTitle));
    QString message;
    if (m_versionManager.updatesAvailable()) {
        message = TRANSLATE(str::sNewVersion2).arg(m_versionManager.updatedVersion());
        msgBox.setInformativeText(TRANSLATE(str::sNewVersionQuestion));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
    } else {
        message = TRANSLATE(str::sNoNewVersion);
    }
    msgBox.setText(message);
    int ret = msgBox.exec();
    switch (ret)
    {
    case QMessageBox::Yes:
        m_versionManager.download();
        break;
    case QMessageBox::No:
    case QMessageBox::Cancel:
    default:
        break;
    }
}

void TrayIcon::setWarningIcon(bool set)
{
    if (set) {
        QPixmap pix(":/icons/app");
        QPixmap newPix(m_noUnreadIcon.pixmap(QSize(32, 32)));
        QPainter p(&newPix);
        p.setBackgroundMode(Qt::TransparentMode);
        p.translate(8, 8);
        p.scale(2, 2);
        p.drawPixmap(0, 0, QPixmap(":/icons/warning_sm"));
        p.end();
        m_noUnreadIcon = QIcon(newPix);
    } else {
        m_noUnreadIcon = QIcon(":/icons/app");
    }
    setIcon(m_noUnreadIcon);
}

void TrayIcon::onNotifierMoved()
{
    m_defaultManager.setDefault(str::sDefNotifyPos,
                                m_notifier.geometry().topLeft());
}

void TrayIcon::onGmailDone()
{
    m_gmailError.clear();
}

void TrayIcon::onGmailError(const QString &errorMsg)
{
    m_gmailError = errorMsg;
}

void TrayIcon::onMarkedAsRead(int id)
{
    if (!m_gmailClient.loggedIn()) {
        m_gmailClient.login(m_login.user(), m_login.password());
    }

    if (m_gmailClient.loggedIn()) {
        QList<int> unreadId = m_gmailClient.unreadMessages();
        const int size = unreadId.size();
        if(id >= 0 && id < size) {
            int unread = unreadId.at(size - id - 1);
            m_gmailClient.markAsRead(unread);

            // Immediatelly update the unread messages count.
            m_gmailClient.sendUnreadMessages();
        }
    }
}

void TrayIcon::setComplexToolTip(const QString &tooltip)
{
    bool showWarning = false;
    QString msg = tooltip;
    if (!m_gmailError.isEmpty()) {
        msg.append('\n');
        msg.append(m_gmailError);
        showWarning = true;
    }

    if (m_versionManager.updatesAvailable()) {
        msg.append('\n');
        msg.append(QString("%1").arg(TRANSLATE(str::sNewVersion2)
                                     .arg(m_versionManager.updatedVersion())));
        showWarning = true;
    }
    setToolTip(msg);
}

} // namespace gui

