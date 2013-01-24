/****************************************************************************
 ** Hyne Final Fantasy VIII Save Editor
 ** Copyright (C) 2013 Arzel Jérôme <myst6re@gmail.com>
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

#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QtGui>

class MapWidget : public QWidget
{
	Q_OBJECT
public:
	explicit MapWidget(QWidget *parent = 0);
	
signals:
	void positionChanged(const QPoint &pos);
public slots:
	void setPosition(const QPoint &pos);
private:
	QPoint pos;
	bool entered;
	QPoint mousePos;
protected:
	virtual void paintEvent(QPaintEvent *);
	virtual QSize sizeHint() const;
	virtual QSize minimumSizeHint() const;
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MAPWIDGET_H
