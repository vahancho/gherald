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

#ifndef __GUISTRINGS_H__
#define __GUISTRINGS_H__

#include <QApplication>

#define TRANSLATE(string) qApp->translate("GHerald", string)

namespace str
{
    //////////////////////////////////////////////////////////////////////////
    // Translatable GUI Strings
    //

    // Information strings
    extern const char *sReportTmpl;
    extern const char *sAbout;
    extern const char *sNewVersion;
    extern const char *sNewVersion2;
    extern const char *sNewVersionTitle;
    extern const char *sNewVersionQuestion;
    extern const char *sNoNewVersion;
    extern const char *sCheckUpdates;

    // Internationalization strings
    extern const char *sLanguageEnglishTitle;
    extern const char *sLanguageRussianTitle;

    // Menu action titles
    extern const char *sMenuViewInbox;
    extern const char *sMenuCheckMail;
    extern const char *sMenuTellAgain;
    extern const char *sMenuChangeUser;
    extern const char *sMenuOptions;
    extern const char *sMenuAbout;
    extern const char *sMenuExit;

    // Status messages
    extern const char *sNoUnreadMail;
    extern const char *sUnreadConversations;
    extern const char *sUnreadLong;

    // Http status messages
    extern const char *sHostLookup;
    extern const char *sConnecting;
    extern const char *sSending;
    extern const char *sReading;
    extern const char *sConnected;

    // Login dialog strings
    extern const char *sUser;
    extern const char *sPassword;
    extern const char *sRememberPassword;
    extern const char *sLoginTitle;

    // Options dialog strings
    extern const char *sSound;
    extern const char *sRun;
    extern const char *sTimeout;
    extern const char *sMailGroup;
    extern const char *sTimeoutMin;
    extern const char *sBrowse;
    extern const char *sOptionsTitle;
    extern const char *sBrowseTitle;
    extern const char *sFilter;
    extern const char *sGuiLanguage;

    // Other strings
    extern const char *sHideBtnToolTip;
    extern const char *sMarkAsReadToolTip;
    extern const char *sNextToolTip;
    extern const char *sPrevToolTip;
    extern const char *sMsgOfMsg;
    extern const char *sOk;
    extern const char *sCancel;

} // namespace str

#endif // __GUISTRINGS_H__
