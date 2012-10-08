/****************************************************************************
 ** Hyne Final Fantasy VIII Save Editor
 ** Copyright (C) 2009-2012 Arzel Jérôme <myst6re@gmail.com>
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include "StartWidget.h"

StartWidget::StartWidget(QWidget *parent) :
	QWidget(parent), _cursorPosition(-1)
{
	setFixedSize(38 + 250 + 38, 0);
	setMouseTracking(true);
}

void StartWidget::addAction(QAction *action)
{
	QWidget::addAction(action);
	setFixedHeight(actions().size() * OPTION_HEIGHT);
}

QAction *StartWidget::addAction(const QString &text)
{
	QAction *act = new QAction(text, this);
	addAction(act);
	return act;
}

QAction *StartWidget::addAction(const QString &text, const QObject *receiver, const char *method)
{
	QAction *act = addAction(text);
	connect(act, SIGNAL(triggered()), receiver, method);
	return act;
}

void StartWidget::setCursorPosition(int actionID)
{
	_cursorPosition = actionID;
	update(0, 12, 48, 30 + actions().size() * OPTION_HEIGHT);
}

int StartWidget::actionID(const QPoint &pos) const
{
	if(pos.x() < 38 || pos.x() > 38 + 250)	return -1;

	return pos.y() / OPTION_HEIGHT;
}

void StartWidget::paintEvent(QPaintEvent *)
{
	if(actions().isEmpty())	return;

	QRegExp remAnd("&([^&])");
	bool enabledState = actions().first()->isEnabled();
	int actionID = 0;
	QPainter painter(this);

	painter.setBrush(QPixmap(QString(":/images/menu-fond%1.png").arg(enabledState ? "" : "2")));
	painter.translate(36, 0);

	foreach(const QAction *act, actions()) {
		if(act->isEnabled() != enabledState) {
			enabledState = act->isEnabled();
			painter.setBrush(QPixmap(QString(":/images/menu-fond%1.png").arg(enabledState ? "" : "2")));
		}

		QString text = act->text();
		text.replace(remAnd, "\\1");

		SaveWidget::drawFrame(&painter, 250, OPTION_HEIGHT);
		FF8Text::drawTextArea(&painter, QPoint(12, 12), text, 1);

		if(actionID == _cursorPosition) {
			painter.drawPixmap(-38, 12, QPixmap(":/images/cursor.png"));
		}

		painter.translate(0, OPTION_HEIGHT);
		++actionID;
	}

	painter.end();
}

void StartWidget::mouseMoveEvent(QMouseEvent *event)
{
	int actionID = this->actionID(event->pos());

	if(actionID >= 0 && actionID < actions().size()) {
		QAction *act = actions().at(actionID);
		if(act->isEnabled()) {
			act->hover();
			setCursorPosition(actionID);
		} else {
			setCursorPosition(-1);
		}
	}
}

void StartWidget::mouseReleaseEvent(QMouseEvent *event)
{
	int actionID = this->actionID(event->pos());

	if(actionID >= 0 && actionID < actions().size()) {
		QAction *act = actions().at(actionID);
		if(act->isEnabled()) {
			act->trigger();
			emit actionTriggered(act);
		}
	}
}

void StartWidget::leaveEvent(QEvent *)
{
	setCursorPosition(-1);
}
