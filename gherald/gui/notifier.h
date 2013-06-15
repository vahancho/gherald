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

#ifndef __NOTIFIER_H__
#define __NOTIFIER_H__

#include <QTextBrowser>
#include <QTimer>
#include <QToolButton>

namespace gui
{

class Notifier : public QTextBrowser
{
    Q_OBJECT

public:
    Notifier(QWidget *parent = 0);
    ~Notifier();

    void setMessages(const QStringList &messages,
                     bool showMarkRead = true);

signals:
    void moved();
    void markAsRead(int);

protected:
    /// Handle the show event.
    void showEvent(QShowEvent *event);

    /// Handle mouse enter event.
    void enterEvent(QEvent *event);

    /// Window begin disappear when mouse leaves the window.
    void leaveEvent(QEvent *event);
    ///
    void mousePressEvent(QMouseEvent* event);
    ///
    void mouseMoveEvent(QMouseEvent* event);
    ///
    void mouseReleaseEvent(QMouseEvent* event);
    ///
    void resizeEvent(QResizeEvent *event);

private slots:
    /// Decrease the window opacity.
    void onTimer();

    void showNext();
    void showPrevious();
    void markAsRead();

protected:
    void changeEvent(QEvent *event);

private:
    /// Adjust the window geometry.
    /*!
        This function resize the window to fit its content and move the window
        for preventing it fall out of screen.
    */
    void adjustGeometry();

    /// The button class for notifier window's navigation.
    class NavigationButton : public QToolButton
    {
    public:
        NavigationButton(QWidget *parent = 0);

    protected:
        /// Highlights the button frame when mouse is over.
        bool event(QEvent *event);
    };

    /// List of messages to show.
    QStringList m_messages;

    /// Current message index.
    int m_current;

    /// Navigation buttons for this window.
    NavigationButton *m_hideButton;
    NavigationButton *m_nextButton;
    NavigationButton *m_prevButton;
    NavigationButton *m_markReadButton;

    /// Timer event handler to change the window transparency.
    QTimer m_timer;

    /// Timer that responsible for messages iteration in the view.
    QTimer m_iterationTimer;

    /// The mouse press offset from the top left corner.
    QPoint m_offset;
};

} // namespace gui

#endif // __TRAYICON_H__
