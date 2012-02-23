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

#ifndef DEF_SAVEICON
#define DEF_SAVEICON

#include <QtGui>
#define COEFF_COLOR	8.2258064516129032258064516129032 // 255/31

class SaveIcon : public QObject
{
	Q_OBJECT
public:
	SaveIcon();
	SaveIcon(const QByteArray &data, quint8 nbFrames=1);
	void setAll(const QByteArray &data, quint8 nbFrames=1);
	const QByteArray &save() const;
	QPixmap icon(bool chocobo_world_icon=false) const;
signals:
	void nextIcon(const QPixmap &);
private slots:
	void nextFrame();
private:
	QByteArray data;
	quint8 nbFrames, curFrame;
	static QTimer timer;
};

#endif
