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

#include "MapWidget.h"

MapWidget::MapWidget(QWidget *parent) :
	QWidget(parent), entered(false)
{
	setMouseTracking(true);
}

void MapWidget::setPosition(const QPoint &pos)
{
	this->pos = pos;
}

QSize MapWidget::sizeHint() const
{
	return QSize(1000, 1000);
}

QSize MapWidget::minimumSizeHint() const
{
	return QPixmap(":/images/worldmap.png").size();
}

void MapWidget::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	QPixmap pxm = QPixmap(":/images/worldmap.png");
	if(!pxm.isNull()) {
		pxm = pxm.scaled(width(), height(), Qt::KeepAspectRatio);
		p.drawPixmap(0, 0, pxm);
		if(entered) {
			p.setPen(Qt::red);
			p.drawLine(mousePos.x(), 0, mousePos.x(), pxm.height());
			p.drawLine(0, mousePos.y(), pxm.width(), mousePos.y());
		}
	}
}

void MapWidget::enterEvent(QEvent *)
{
	entered = true;
	setCursor(Qt::CrossCursor);
}

void MapWidget::leaveEvent(QEvent *)
{
	entered = false;
	setCursor(QCursor());
	update();
}

void MapWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(isActiveWindow()) {
		mousePos = event->pos();
		update();
	}
}

void MapWidget::mouseReleaseEvent(QMouseEvent *event)
{
	emit positionChanged(event->pos());
}
