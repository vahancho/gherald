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

#ifndef __STRINGS_H__
#define __STRINGS_H__

namespace str
{
    //////////////////////////////////////////////////////////////////////////
    // Not Translatable Strings
    //

    // Defaults names
    extern const char *sDefInterval;
    extern const char *sDefNotifyPos;
    extern const char *sDefLogin;
    extern const char *sDefSoundFile;
    extern const char *sDefSoundPlay;
    extern const char *sDefLanguage;
    extern const char *sDefLanguages;

    // Internationalization strings
    extern const char *sLanguageEnglishKey;
    extern const char *sLanguageRussianKey;
    // The name of GUI default language.
    extern const char *sLanguage;

    extern const char *sMailBoxUrl;

    extern const char *sReportTmplNoMail;

    // Application name
    extern const char *sAppName;
    extern const char *sStatusMessageTmpl;

    extern const char *sFeedUrl;
    extern const char *sFeedPath;

    // XML Tags
    extern const char *sTagEntry;
    extern const char *sTagContributor;
    extern const char *sTagFullCount;
    extern const char *sTagTagLine;
    extern const char *sTagModified;
    extern const char *sTagSummary;
    extern const char *sTagName;
    extern const char *sTagTitle;

    // Version related strings
    extern const char *sVersionUrl;
    extern const char *sVersion;

} // namespace str

#endif // __STRINGS_H__
