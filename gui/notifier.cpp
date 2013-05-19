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
#include <QMouseEvent>
#include <QAbstractTextDocumentLayout>
#include <QStyle>
#include "notifier.h"
#include <strings/guiStrings.h>

static const int sleepInterval = 3000;

namespace gui
{

Notifier::Notifier(QWidget * parent)
    :
        QTextBrowser(parent)
{
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
    setReadOnly(true);
    setOpenExternalLinks(true);
    setWordWrapMode(QTextOption::WordWrap);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_timer.setInterval(100);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));

    resize(300, 100);

    // The gradient background
    setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                  "stop: 0 #E0E0E0, stop: 1 #FFFFFF);");

    // Create and setup hide button
    m_hideButton = new HideButton(this);
}

Notifier::~Notifier()
{}

void Notifier::onTimer()
{
    qreal op = windowOpacity();
    op -= 0.02;

    if (op > 0)
        setWindowOpacity(op);
    else
    {
        m_timer.stop();
        hide();
    }
}

void Notifier::showEvent(QShowEvent *event)
{
    setWindowModality(Qt::ApplicationModal);
    adjustGeometry();

    setWindowOpacity(1.0);
    m_timer.start();

    activateWindow();
    raise();

    setWindowModality(Qt::NonModal);

    m_current = 0;
    showNext();

    QTextBrowser::showEvent(event);
}

void Notifier::showNext()
{
    if (m_current < m_messages.count()) {
        setHtml(m_messages.at(m_current++));
        adjustGeometry();
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        QTimer::singleShot(sleepInterval, this, SLOT(showNext()));
    }
}

void Notifier::enterEvent(QEvent *event)
{
    m_timer.stop();
    setWindowOpacity(1.0);

    QTextBrowser::enterEvent(event);
}

void Notifier::leaveEvent(QEvent *event)
{
    m_timer.start();

    QTextBrowser::leaveEvent(event);
}

void Notifier::mousePressEvent(QMouseEvent* event)
{
    m_offset = event->globalPos() - pos();
    QTextBrowser::mousePressEvent(event);
}

void Notifier::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_offset.isNull())
        move(event->globalPos() - m_offset);

    QTextBrowser::mouseMoveEvent(event);
}

void Notifier::mouseReleaseEvent(QMouseEvent* event)
{
    m_offset = QPoint();
    QTextBrowser::mouseReleaseEvent(event);
}

void Notifier::resizeEvent(QResizeEvent *event)
{
    QSize sz = m_hideButton->sizeHint();

    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);

    // Move the button to the right top edge of the window
    m_hideButton->move( rect().right() - frameWidth - sz.width(),
                        rect().top() + frameWidth);

    QTextBrowser::resizeEvent(event);
}

void Notifier::adjustGeometry()
{
    // Adjust to the content size.
    resize(document()->documentLayout()->documentSize().toSize());

    // Now we have to decide where to show this pull down. we don't want to
    // show it outside of visible area of the screen

    // Get our screen rectangle
    QRect screen = QApplication::desktop()->availableGeometry(this);

    const QSize mySize = size();
    QPoint position = pos();

    // Handle popup falling "off screen"

    // Left/right side of the screen
    if (position.x() + mySize.width() > screen.right())
        position.setX(qMin(position.x() - mySize.width(), screen.right() - mySize.width()));
    else if (position.x() < screen.left())
        position.setX(position.x());

    // Top/bottom sides
    if (position.y() + mySize.height() > screen.bottom())
        position.setY(qMax(position.y() - (mySize.height()), screen.bottom() - mySize.height()));
    else if (position.y() < screen.top()) 
        position.setY(screen.top());

    // Now set the new position
    setGeometry(QRect(position, mySize));
}

void Notifier::setMessages(const QStringList &messages)
{
    m_messages = messages;
}

//////////////////////////////////////////////////////////////////////////
// Hide Button implementation

const QString sStyleNormal("QToolButton {background-color: #E0E0E0;"
                           "border: none;"
                           "padding: 0px; }");

const QString sStyleHighlight("QToolButton {background-color: #E0E0E0;"
                              "border: 1px solid #FF9933;"
                              "padding: 0px; }");

Notifier::HideButton::HideButton(QWidget *parent)
    :
        QToolButton(parent)
{
    setIcon(QIcon(":icons/close"));
    setCursor(Qt::ArrowCursor);
    setToolTip(qApp->translate("GHerald", str::sHideBtnToolTip));

    // Customize the button style
    setStyleSheet(sStyleNormal);

    connect(this, SIGNAL(clicked()), parentWidget(), SLOT(hide()));
}

bool Notifier::HideButton::event(QEvent *event)
{
    if (event->type() == QEvent::Enter)
        setStyleSheet(sStyleHighlight);
    else if (event->type() == QEvent::Leave)
        setStyleSheet(sStyleNormal);

    return QToolButton::event(event);
}

} // namespace gui
