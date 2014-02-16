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

#include <QtGlobal>


namespace str
{
//////////////////////////////////////////////////////////////////////////
// Translatable GUI Strings
//

    const char *sReportTmpl = QT_TRANSLATE_NOOP("GHerald",
            "<table cellspacing=2 cellpadding=2>"
            "<tr valign=\"top\">"
            "<td>"
            "<img src=\":icons/notifier\" width=\"32\" height=\"32\"><br></a>"
            "</td>"
            "<td>"
            "<span foreground=\"#000000\"><small>%1 unread messages...</small></span>"
            "<BR><BR>"
            "%2"
            "<BR>"
            "<BR>"
            "</td>"
            "</tr>"
            "</table>");

    const char *sAbout = QT_TRANSLATE_NOOP("GHerald",
            "Gmail Herald %1"
            "<BR>"
            "Copyright © 2010-2013 <a href=\"mailto:vahancho@gmail.com\">Vahan Aghajanyan</a>."
            "<BR>"
            "All Rights Reserved."
            "<BR><BR>"
            "Gmail Herald is made possible by the <a href=\"http://sourceforge.net/projects/gherald\">GHerald</a> open source project.");

    const char *sNewVersion = QT_TRANSLATE_NOOP("GHerald",
        "<BR><BR><img src=\":icons/warning\" width=\"16\" height=\"16\"><small> Gmail Herald %1 is available. "
        "You can download new version from the project's <a href=\"http://sourceforge.net/projects/gherald/files\">web site</a>.</small>");

    const char *sNewVersion2 = QT_TRANSLATE_NOOP("GHerald",
        "Gmail Herald %1 is available.\n"
        "You can download new version from the project's web site.");

    const char *sNewVersionTitle = QT_TRANSLATE_NOOP("GHerald", "Gmail Herald Update");
    const char *sNewVersionQuestion = QT_TRANSLATE_NOOP("GHerald", "Do you want to download it now?");
    const char *sNoNewVersion = QT_TRANSLATE_NOOP("GHerald", "Gmail Herald is up to date.");
    const char *sCheckUpdates = QT_TRANSLATE_NOOP("GHerald", "Check for Updates");

    // Internationalization strings
    const char *sLanguageEnglishTitle = QT_TRANSLATE_NOOP("GHerald", "English");
    const char *sLanguageRussianTitle = QT_TRANSLATE_NOOP("GHerald", "Russian");

    // Menu action titles
    const char *sMenuViewInbox = QT_TRANSLATE_NOOP("GHerald", "View &Inbox");
    const char *sMenuCheckMail = QT_TRANSLATE_NOOP("GHerald", "&Check Mail Now");
    const char *sMenuTellAgain = QT_TRANSLATE_NOOP("GHerald", "Tell me A&gain...");
    const char *sMenuChangeUser = QT_TRANSLATE_NOOP("GHerald", "Change &User/Password...");
    const char *sMenuOptions = QT_TRANSLATE_NOOP("GHerald", "&Options...");
    const char *sMenuAbout = QT_TRANSLATE_NOOP("GHerald", "&About");
    const char *sMenuExit = QT_TRANSLATE_NOOP("GHerald", "E&xit");

    // Status messages
    const char *sNoUnreadMail = QT_TRANSLATE_NOOP("GHerald", "No unread mail");
    const char *sUnreadConversations = QT_TRANSLATE_NOOP("GHerald", "%1 unread conversation");
    const char *sUnreadLong = QT_TRANSLATE_NOOP("GHerald", "<p>Your inbox contains no unread conversations.</p>");

    // Http status messages
    const char *sHostLookup = QT_TRANSLATE_NOOP("GHerald", "A host name lookup is in progress...");
    const char *sConnecting = QT_TRANSLATE_NOOP("GHerald", "Connecting to the host...");
    const char *sSending = QT_TRANSLATE_NOOP("GHerald", "Sending request to the server...");
    const char *sReading = QT_TRANSLATE_NOOP("GHerald", "Reading the server's response...");
    const char *sConnected = QT_TRANSLATE_NOOP("GHerald", "Connected");

    // Login dialog strings
    const char *sUser = QT_TRANSLATE_NOOP("GHerald", "User name:");
    const char *sPassword = QT_TRANSLATE_NOOP("GHerald", "Password:");
    const char *sRememberPassword = QT_TRANSLATE_NOOP("GHerald", "&Remember my password");
    const char *sLoginTitle = QT_TRANSLATE_NOOP("GHerald", "Connect to Gmail");

    // Options dialog strings
    const char *sSound = QT_TRANSLATE_NOOP("GHerald", "Play sound on new mail");
    const char *sShowPopup = QT_TRANSLATE_NOOP("GHerald", "Show notifications");
    const char *sRun = QT_TRANSLATE_NOOP("GHerald", "&Run on system start up");
    const char *sTimeout = QT_TRANSLATE_NOOP("GHerald", "Check mail every:");
    const char *sMailGroup = QT_TRANSLATE_NOOP("GHerald", "&Mail checking preferences");
    const char *sTimeoutMin = QT_TRANSLATE_NOOP("GHerald", "minutes");
    const char *sBrowse = QT_TRANSLATE_NOOP("GHerald", "...");
    const char *sOptionsTitle = QT_TRANSLATE_NOOP("GHerald", "Gmail Herald Options");
    const char *sBrowseTitle = QT_TRANSLATE_NOOP("GHerald", "Browse for notification sound file");
    const char *sFilter = QT_TRANSLATE_NOOP("GHerald", "Sound (*.wav)");
    const char *sGuiLanguage = QT_TRANSLATE_NOOP("GHerald", "&User interface language");

    // Other strings
    const char *sHideBtnToolTip = QT_TRANSLATE_NOOP("GHerald", "Hide window");
    const char *sMarkAsReadToolTip = QT_TRANSLATE_NOOP("GHerald", "Mark as read");
    const char *sNextToolTip = QT_TRANSLATE_NOOP("GHerald", "Next message");
    const char *sPrevToolTip = QT_TRANSLATE_NOOP("GHerald", "Previous message");
    const char *sMsgOfMsg = QT_TRANSLATE_NOOP("GHerald", "%1 of %2");
    const char *sOk = QT_TRANSLATE_NOOP("GHerald", "OK");
    const char *sCancel = QT_TRANSLATE_NOOP("GHerald", "Cancel");

} // namespace str
