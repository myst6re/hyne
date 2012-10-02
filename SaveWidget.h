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

#ifndef DEF_SAVEWIDGET
#define DEF_SAVEWIDGET

#include <QtGui>
#include "SaveData.h"
#include "Savecard.h"

class Savecard;

class SaveWidget : public QWidget
{
	Q_OBJECT
public:
	SaveWidget(SaveData *saveData, Savecard *savecard, QWidget *parent=0);
	void hideCursor();
	void setSaved();
	void setSaveData(SaveData *saveData);
	void setSavecard(Savecard *savecard);
	virtual QSize sizeHint() const;
	virtual QSize minimumSizeHint() const;
	static void drawFrame(QPainter *painter, int width, int height);
	static void num2pix(QPainter *painter, QImage *numberPixmap, int x, int y, quint32 num, quint8 space=1, QChar fill=QChar(' '), int color=0);
private:
	void restore();
	static void colors(QImage *image, int color);

	SaveData *saveData;
	Savecard *_savecard;
	int mouseMove;
	bool saved, hovered;
	bool blackView, hasDragEvent;
	QByteArray lastDropData;
	bool lastIsExternal;
public slots:
	void properties();
private slots:
	void exportPC();
	void newGame();
	void removeSave();
	void refreshIcon();
	void emitDropped();
signals:
	void released(SaveData *saveData);
	void changed();
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void enterEvent(QEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void contextMenuEvent(QContextMenuEvent *event);
	virtual void changeEvent(QEvent *event);
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dragMoveEvent(QDragMoveEvent *event);
	virtual void dragLeaveEvent(QDragLeaveEvent *event);
	virtual void dropEvent(QDropEvent *event);
};

#endif
