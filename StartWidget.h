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
#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QtGui>
#include "FF8Text.h"

#define OPTION_WIDTH	250
#define OPTION_HEIGHT	42

class StartWidget : public QWidget
{
	Q_OBJECT
public:
	explicit StartWidget(QWidget *parent = 0);
	void addAction(QAction *action);
	QAction *addAction(const QString &text);
	QAction *addAction(const QString &text, const QObject *receiver, const char *method);
	void setCursorPosition(int actionID);
	inline virtual QSize sizeHint() const { return QSize(OPTION_WIDTH, OPTION_HEIGHT * actions().size()); }
	inline virtual QSize minimumSizeHint() const { return sizeHint(); }
signals:
	void actionTriggered(QAction *action);
public slots:
	
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void leaveEvent(QEvent *event);
private:
	QPoint actionsPosition() const { return QPoint((width() - sizeHint().width()) / 2, (height() - sizeHint().height()) / 2); }
	int actionID(const QPoint &pos) const;
	int _cursorPosition;
};

#endif // STARTWIDGET_H
