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

class SaveIconData
{
public:
	SaveIconData();
	SaveIconData(const QByteArray &data, quint8 nbFrames=1);
	void setData(const QByteArray &data);
	void setNbFrames(quint8 nbFrames);
	QByteArray data() const;
	quint8 nbFrames() const;
	QPixmap icon(int curFrame=0, bool showCW=false) const;
private:
	QByteArray _data;
	quint8 _nbFrames;
};

class SaveIcon : public QObject
{
	Q_OBJECT
public:
	SaveIcon(bool showCW=false, QObject *parent=0);
	SaveIcon(const SaveIconData &data, bool showCW=false, QObject *parent=0);
	void setData(const SaveIconData &data);
	void setCurFrame(quint8 curFrame);
	void setCWIsVisible(bool showCW);
	const SaveIconData &data() const;
	quint8 curFrame() const;
	bool CWIsVisible() const;
	QPixmap pixmap() const;
signals:
	void nextIcon(const QPixmap &);
private slots:
	void nextFrame();
private:
	SaveIconData _data;
	quint8 _curFrame;
	bool _showCW;
	static QTimer timer;
};

#endif
