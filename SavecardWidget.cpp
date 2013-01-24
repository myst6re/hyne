/****************************************************************************
 ** Hyne Final Fantasy VIII Save Editor
 ** Copyright (C) 2009-2013 Arzel Jérôme <myst6re@gmail.com>
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

#include "SavecardWidget.h"
#include "Parameters.h"
#include "HeaderDialog.h"

SavecardWidget::SavecardWidget(QWidget *parent) :
	QScrollArea(parent)
{
	setWidget(new SavecardView(this));
	setPalette(QPalette(Qt::black));
	setFrameShape(QFrame::NoFrame);
	setAlignment(Qt::AlignHCenter | Qt::AlignTop);
}

SavecardWidget::~SavecardWidget()
{
}

SavecardView *SavecardWidget::view() const
{
	return (SavecardView *)widget();
}

SavecardData *SavecardWidget::savecard() const
{
	return view()->savecard();
}

void SavecardWidget::setSavecard(SavecardData *save)
{
	view()->setSavecard(save);
}

QRect SavecardWidget::visualRect() const
{
	int visualX, visualY;

	if(horizontalScrollBar() && horizontalScrollBar()->maximum() - horizontalScrollBar()->minimum() != 0) {
		visualX = (horizontalScrollBar()->value() - horizontalScrollBar()->minimum()) *
				(view()->width() - width()) / (horizontalScrollBar()->maximum() - horizontalScrollBar()->minimum());
	} else {
		visualX = 0;
	}
	if(verticalScrollBar() && verticalScrollBar()->maximum() - verticalScrollBar()->minimum() != 0) {
		visualY = (verticalScrollBar()->value() - verticalScrollBar()->minimum()) *
				(view()->height() - height()) / (verticalScrollBar()->maximum() - verticalScrollBar()->minimum());
	} else {
		visualY = 0;
	}

	return QRect(QPoint(visualX, visualY), size());
}

void SavecardWidget::scrollToDrag(const QPoint &pos)
{
	QRect visualRect = this->visualRect();
	const int locationHeight = view()->saveHeight() / 2;
	const int posYInView = pos.y() - visualRect.y();

	if(posYInView >= height() - locationHeight) {
		if(verticalScrollBar()->value() >= verticalScrollBar()->maximum()) {
			return;
		}

		const int posYInBottomLocation = locationHeight - (height() - posYInView);

		verticalScrollBar()->setValue(verticalScrollBar()->value() + (verticalScrollBar()->maximum() / 15) * posYInBottomLocation / locationHeight);
	} else if(posYInView <= locationHeight) {
		if(verticalScrollBar()->value() <= verticalScrollBar()->minimum()) {
			return;
		}

		const int posYInTopLocation = locationHeight - posYInView;

		verticalScrollBar()->setValue(verticalScrollBar()->value() - (verticalScrollBar()->maximum() / 15) * posYInTopLocation / locationHeight);
	}
}
