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

namespace str
{
    //////////////////////////////////////////////////////////////////////////
    // Not Translatable Strings
    //

    // Defaults names
    const char *sDefInterval = "Interval";
    const char *sDefNotifyPos = "NotifyPos";
    const char *sDefLogin = "Login";
    const char *sDefSoundFile = "NotifySoundFile";
    const char *sDefSoundPlay = "NotifySoundPlay";
    const char *sDefLanguage = "Language";
    const char *sDefLanguages = "Languages";

    // Internationalization strings
    const char *sLanguageEnglishKey = "en";
    const char *sLanguageRussianKey = "ru";
    const char *sLanguage = "en";

    const char *sMailBoxUrl =
        "https://www.google.com/accounts/ServiceLoginAuth?continue=http://mail.google.com/gmail&service=mail&Email=%1&Passwd=%2&null=Sign+in";

    const char *sReportTmplNoMail =
            "<table cellspacing=2 cellpadding=2>"
            "<tr valign=\"top\">"
            "<td>"
            "<img src=\":icons/notifier\" width=\"32\" height=\"32\"><br></a>"
            "</td>"
            "<td>"
            "%1"
            "</td>"
            "</tr>"
            "</table>";

    // Application name
    const char *sAppName = "Gmail Herald";
    const char *sStatusMessageTmpl = "<p>%1</p>";

    const char *sFeedUrl = "https://mail.google.com/mail/feed/atom/";
    const char *sFeedPath = "/mail/feed/atom/";

    // XML Tags
    const char *sTagEntry = "entry";
    const char *sTagContributor = "contributor";
    const char *sTagFullCount = "fullcount";
    const char *sTagTagLine = "tagline";
    const char *sTagModified = "modified";
    const char *sTagSummary = "summary";
    const char *sTagName = "name";
    const char *sTagTitle = "title";

    // Version related strings
    const char *sVersionUrl = "http://gherald.sourceforge.net/version";
    const char *sVersion = "0.3";

} // namespace str
