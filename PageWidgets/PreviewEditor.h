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

#ifndef PREVIEWEDITOR_H
#define PREVIEWEDITOR_H

#include "PageWidget.h"
#include "SavecardView.h"

class PreviewWidget : public QWidget
{
	Q_OBJECT
public:
	explicit PreviewWidget(QWidget *parent=0) : QWidget(parent), _saveData(0) {}
	QSize sizeHint() const {
		return minimumSizeHint();
	}
	QSize minimumSizeHint() const {
		return SavecardView::saveSize();
	}
	void setSaveData(const SaveData *saveData) {
		_saveData = saveData;
	}

protected:
	virtual void paintEvent(QPaintEvent *event) {
		if(!_saveData)	return;
		QPainter p(this);
		SavecardView::renderSave(&p, _saveData, event->rect());
	}
private:
	const SaveData *_saveData;
};

class PreviewEditor : public PageWidget
{
	Q_OBJECT
public:
	explicit PreviewEditor(QWidget *parent=0);
	inline QString name() { return tr("Aperçu"); }
public slots:
	void fillPage();
	void savePage();
private slots:
	void setGroupDisabled(bool disable);
	void updatePreview();
protected:
	void buildWidget();
private:
	QGroupBox *autoGroup;
	QGridLayout *autoL;
	QComboBox *locationIDE;
	QSpinBox *hpLeaderE, *hpMaxLeaderE;
	QSpinBox *nivLeaderE, *saveCountE;
	QDoubleSpinBox *gilsE, *timeE, *discE, *curSaveE;
	PreviewWidget *previewWidget;
};

#endif // PREVIEWEDITOR_H
