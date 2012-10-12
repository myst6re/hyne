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

#ifndef DEF_SAVECARD
#define DEF_SAVECARD

#include <QtGui>
#include "SaveWidget.h"
#include "SavecardData.h"

class SaveWidget;

class SavecardWidget : public QListWidget
{
	Q_OBJECT

public:
	SavecardWidget(QWidget *parent=0);
	virtual ~SavecardWidget();

	void updateSaveWidgets();

	SaveWidget *saveWidget(int row) const;

	SavecardData *savecard() const;
	void setSavecard(SavecardData *save);

	void moveCursor(int);
	void setDragStart(int saveID);
	void moveDraggedSave(int saveID);
	void replaceSaveData(int saveID, const QByteArray &mimeData);
	void setDropIndicatorIsVisible(int saveID, bool onTop, bool isVisible);
	void scrollToDrag(int saveID, const QPoint &pos);
private slots:
	void notifyFileChanged(const QString &path);
signals:
	void modified();
private:
	void setWidget();
	void addSave(SaveData *saveData);

	int _dragStart;
	bool notify;
	SavecardData *_data;
};

#endif
