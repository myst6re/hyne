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

#pragma once

#include <QtCore>
#include <QPainter>
#include "Data.h"
#include "Config.h"

class FF8Text
{
public:
	static QString toString(const QByteArray &ff8str, bool jp=false);
	static QByteArray toByteArray(const QString &string, bool jp=false);
	static QString caract(quint8 ord, quint8 table=0);

	static void reloadFont();
	static void drawTextArea(QPainter *painter, const QPoint &point, const QString &ff8Text, int forceLang=0);// 1: latin 2: japanese
	static QByteArray numToBiosText(quint32 num, quint8 width=0);
private:
	static QImage *fontImage;
	static void letter(int *x, int *y, int charId, QPainter *painter, quint8 tableId=0);
	static const char *optimisedDuo[24];
	static const quint8 charWidth[5][224];
	static const char *_caract[240];
	static const char *_caractJp[240];
	static const char *_caractJp19[240];
	static const char *_caractJp1a[240];
	static const char *_caractJp1b[240];
	static const char *names[14];
	static const char *colors[16];
	static const char *locations[8];
};
