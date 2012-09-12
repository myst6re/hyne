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

#include "SaveIcon.h"

QTimer SaveIcon::timer;

SaveIcon::SaveIcon()
	: nbFrames(0), curFrame(0)
{
}

SaveIcon::SaveIcon(const QByteArray &data, quint8 nbFrames)
	: curFrame(0)
{
	setAll(data, nbFrames);
}

void SaveIcon::setAll(const QByteArray &data, quint8 nbFrames)
{
	this->data = data;
	this->nbFrames = nbFrames;
	if(nbFrames>1)
	{
		connect(&timer, SIGNAL(timeout()), SLOT(nextFrame()));
		timer.start(160);
	}
}

const QByteArray &SaveIcon::save() const
{
	return data;
}

QPixmap SaveIcon::icon(bool chocobo_world_icon) const
{
	quint16 i;

	if(data.isEmpty())	return QPixmap();

	if(!chocobo_world_icon)
	{
		//palette
		const char *access_data = data.constData();
		QVector<QRgb> colors;
		quint16 color;
		for(i=0 ; i<16 ; ++i)
		{
			memcpy(&color, access_data, 2);
			colors.append(qRgb(qRound((color & 31)*COEFF_COLOR), qRound((color>>5 & 31)*COEFF_COLOR), qRound((color>>10 & 31)*COEFF_COLOR)));
			access_data += 2;
		}

		QImage image(16, 16, QImage::Format_Indexed8);
		uchar *pixels = (uchar *)image.bits();
		quint8 index;
		quint16 firstPos = 32+curFrame*128, lastPos = firstPos+128, curPx = 0;

		image.setColorTable(colors);

		if(data.size()<lastPos)	return QPixmap();

		for(i=firstPos ; i<lastPos ; ++i)
		{
			index = data.at(i);
			pixels[curPx++] = index & 0xF;
			pixels[curPx++] = index >> 4;
		}

		return QPixmap::fromImage(image);
	}

	if(data.size()!=288)	return QPixmap();
	
	QImage image(32, 32, QImage::Format_MonoLSB);
	uchar *pixels = image.bits();
	quint16 curPx = 0;

	for(i=160 ; i<288 ; ++i)
	{
		pixels[curPx++] = ~data.at(i);
	}

	return QPixmap::fromImage(image);
}

void SaveIcon::nextFrame()
{
	if(nbFrames==0)	return;

	curFrame = (curFrame + 1) % nbFrames;
	QPixmap pix = icon();

	if(!pix.isNull())
		emit nextIcon(pix);
}
