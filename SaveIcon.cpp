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

#include "SaveIcon.h"

SaveIconData::SaveIconData() :
	_nbFrames(0)
{
}

SaveIconData::SaveIconData(const QByteArray &data, quint8 nbFrames) :
	_data(data), _nbFrames(nbFrames)
{
}

void SaveIconData::setData(const QByteArray &data)
{
	_data = data;
}

void SaveIconData::setNbFrames(quint8 nbFrames)
{
	_nbFrames = nbFrames;
}

const QByteArray &SaveIconData::data() const
{
	return _data;
}

quint8 SaveIconData::nbFrames() const
{
	return _nbFrames;
}

QPixmap SaveIconData::icon(int curFrame, bool showCW) const
{
	quint16 i;

	if (_data.isEmpty())	return QPixmap();

	if (!showCW)
	{
		//palette
		const char *access_data = _data.constData();
		QVector<QRgb> colors;
		quint16 color;
		for (i = 0; i < 16; ++i)
		{
			memcpy(&color, access_data, 2);
			colors.append(qRgb(qRound((color & 31)*COEFF_COLOR), qRound((color>>5 & 31)*COEFF_COLOR), qRound((color>>10 & 31)*COEFF_COLOR)));
			access_data += 2;
		}

		QImage image(16, 16, QImage::Format_Indexed8);
		uchar *pixels = (uchar *)image.bits();
		quint16 firstPos = 32+curFrame*128, lastPos = firstPos+128, curPx = 0;

		image.setColorTable(colors);

		if (_data.size() < lastPos)	return QPixmap();

		for (i = firstPos; i < lastPos; ++i)
		{
			quint8 index = _data.at(i);
			pixels[curPx++] = index & 0xF;
			pixels[curPx++] = index >> 4;
		}

		return QPixmap::fromImage(image);
	}

	if (_data.size() != 288)	return QPixmap();

	QImage image(32, 32, QImage::Format_MonoLSB);
	uchar *pixels = image.bits();
	quint16 curPx = 0;

	for (i = 160; i < 288; ++i)
	{
		pixels[curPx++] = ~_data.at(i);
	}

	return QPixmap::fromImage(image);
}

SaveIconTimer SaveIcon::timer;

SaveIcon::SaveIcon(bool showCW, QObject *parent) :
	QObject(parent), _curFrame(0), _showCW(showCW)
{
}

SaveIcon::SaveIcon(const SaveIconData &data, bool showCW, QObject *parent) :
	QObject(parent), _curFrame(0), _showCW(showCW)
{
	setData(data);
}

void SaveIcon::setData(const SaveIconData &data)
{
	_data = data;

	if (_data.nbFrames() > 1)
	{
		connect(&timer, SIGNAL(timeout()), SLOT(nextFrame()));
		if (!timer.isActive()) {
			timer.start();
		}
	}
}

void SaveIcon::setCurFrame(quint8 curFrame)
{
	_curFrame = curFrame;
}

void SaveIcon::setCWIsVisible(bool showCW)
{
	_showCW = showCW;
}

const SaveIconData &SaveIcon::data() const
{
	return _data;
}

quint8 SaveIcon::curFrame() const
{
	return _curFrame;
}

bool SaveIcon::CWIsVisible() const
{
	return _showCW;
}

void SaveIcon::nextFrame()
{
	if (_data.nbFrames() == 0)	return;

	_curFrame = (_curFrame + 1) % _data.nbFrames();
	QPixmap pix = pixmap();
	if (!pix.isNull())
		emit nextIcon(pix);
}

QPixmap SaveIcon::pixmap() const
{
	return _data.icon(_curFrame, _showCW);
}
