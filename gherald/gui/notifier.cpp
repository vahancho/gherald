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
static const int opacityInterval = 200;

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

    m_timer.setInterval(opacityInterval);
    connect(&m_timer, SIGNAL(timeout()), SLOT(onTimer()));

    m_iterationTimer.setSingleShot(true);
    m_iterationTimer.setInterval(sleepInterval);
    connect(&m_iterationTimer, SIGNAL(timeout()), SLOT(showNext()));

    resize(300, 100);

    // The gradient background
    setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                  "stop: 0 #E0E0E0, stop: 1 #FFFFFF);");

    // Create and setup hide, next and previous buttons.
    m_hideButton = new NavigationButton(this);
    m_hideButton->setIcon(QIcon(":icons/close"));
    m_hideButton->setToolTip(TRANSLATE(str::sHideBtnToolTip));
    connect(m_hideButton, SIGNAL(clicked()), SLOT(hide()));

    m_nextButton = new NavigationButton(this);
    m_nextButton->setIcon(QIcon(":icons/right"));
    m_nextButton->setToolTip(TRANSLATE(str::sNextToolTip));
    connect(m_nextButton, SIGNAL(clicked()), SLOT(showNext()));

    m_prevButton = new NavigationButton(this);
    m_prevButton->setIcon(QIcon(":icons/left"));
    m_prevButton->setToolTip(TRANSLATE(str::sPrevToolTip));
    connect(m_prevButton, SIGNAL(clicked()), SLOT(showPrevious()));

    m_markReadButton = new NavigationButton(this);
    m_markReadButton->setIcon(QIcon(":icons/mark_read"));
    m_markReadButton->setToolTip(TRANSLATE("Mark Message As Read"));
    connect(m_markReadButton, SIGNAL(clicked()), SLOT(markAsRead()));
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
    QTextBrowser::showEvent(event);

    setWindowModality(Qt::ApplicationModal);
    setWindowOpacity(1.0);

    adjustGeometry();
    m_timer.start();

    activateWindow();
    raise();

    setWindowModality(Qt::NonModal);
    m_current = 0;
    QTimer::singleShot(10, this, SLOT(showNext()));
}

void Notifier::showNext()
{
    if (m_current >= 0 && m_current < m_messages.count()) {
        // If sender is not a button, we are in automatic iteration mode.
        bool automatic = (qobject_cast<QToolButton *>(sender()) == 0);

        setHtml(m_messages.at(m_current++));
        adjustGeometry();
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        if (automatic) {
            m_iterationTimer.start();
        } else {
            m_iterationTimer.stop();
        }

        m_prevButton->setEnabled(m_current > 1);
        m_nextButton->setEnabled(m_current < m_messages.count());
    }
}

void Notifier::showPrevious()
{
    m_current = qMax(0, m_current - 2);
    showNext();
}

void Notifier::markAsRead()
{
    emit markAsRead(m_current - 1);
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
    if (!m_offset.isNull()) {
        move(event->globalPos() - m_offset);
        emit moved();
    }

    QTextBrowser::mouseMoveEvent(event);
}

void Notifier::mouseReleaseEvent(QMouseEvent* event)
{
    m_offset = QPoint();
    QTextBrowser::mouseReleaseEvent(event);
}

void Notifier::resizeEvent(QResizeEvent *event)
{
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);

    QSize sz = m_hideButton->sizeHint();
    // Move the button to the right top edge of the window.
    m_hideButton->move(rect().right() - frameWidth - sz.width(),
                       rect().top() + frameWidth);
    QPoint p = m_hideButton->geometry().topLeft();

    sz = m_nextButton->sizeHint();
    m_nextButton->move(p.x() - sz.width(), frameWidth);
    p = m_nextButton->geometry().topLeft();

    sz = m_prevButton->sizeHint();
    m_prevButton->move(p.x() - sz.width(), frameWidth);

    // Move "Mark as Read" button to the bottom of the window.
    m_markReadButton->move(rect().right() - frameWidth - 2 * m_markReadButton->width(),
                           rect().bottom() - frameWidth - m_markReadButton->height());

    QTextBrowser::resizeEvent(event);
}

void Notifier::adjustGeometry()
{
    // Adjust to the content size.
    resize(document()->documentLayout()->documentSize().toSize() + QSize(2, 0));

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

void Notifier::setMessages(const QStringList &messages,
                           bool showMarkRead)
{
    m_messages = messages;

    int count = m_messages.count();
    m_prevButton->setVisible(count > 1);
    m_nextButton->setVisible(count > 1);
    m_markReadButton->setVisible(showMarkRead);
}

void Notifier::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        m_hideButton->setToolTip(TRANSLATE(str::sHideBtnToolTip));
        m_nextButton->setToolTip(TRANSLATE(str::sNextToolTip));
        m_prevButton->setToolTip(TRANSLATE(str::sPrevToolTip));
        m_markReadButton->setToolTip(TRANSLATE("Mark Message As Read"));
    } else {
        QTextBrowser::changeEvent(event);
    }
}

//////////////////////////////////////////////////////////////////////////
// Navigation Button implementation

const QString sStyleNormal("QToolButton {background-color: #E0E0E0;"
                           "border: none;"
                           "padding: 0px; }");

const QString sStyleHighlight("QToolButton {background-color: #E0E0E0;"
                              "border: 1px solid #FF9933;"
                              "padding: 0px; }");

Notifier::NavigationButton::NavigationButton(QWidget *parent)
    :
        QToolButton(parent)
{
    setCursor(Qt::ArrowCursor);

    // Customize the button style
    setStyleSheet(sStyleNormal);
    resize(16, 16);
}

bool Notifier::NavigationButton::event(QEvent *event)
{
    if (event->type() == QEvent::Enter)
        setStyleSheet(sStyleHighlight);
    else if (event->type() == QEvent::Leave)
        setStyleSheet(sStyleNormal);

    return QToolButton::event(event);
}

} // namespace gui
