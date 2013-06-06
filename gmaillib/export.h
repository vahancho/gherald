/**************************************************************************
*   Copyright (C) 2011 by Vahan Aghajanyan                                *
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

#ifndef __EXPORT_H__
#define __EXPORT_H__

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  if defined(GMAIL_LIB) // create dll
#    define GMAILLIB_EXPORT __declspec(dllexport)
#  else // use dll
#    define GMAILLIB_EXPORT  __declspec(dllimport)
#  endif
#elif defined(__GNUC__) && __GNUC__ - 0 > 3 && defined(GMAIL_LIB)
#  define GMAILLIB_EXPORT __attribute__ ((visibility("default")))
#else
# define GMAILLIB_EXPORT
#endif

#endif // __EXPORT_H__
