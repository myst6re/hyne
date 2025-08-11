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

#include "PageWidget.h"
#include "../SpinBox.h"

class GfEditor : public PageWidget
{
	Q_OBJECT
public:
	explicit GfEditor(QWidget *parent = nullptr);
	inline QString name() { return tr("G-Forces"); }
	void updateMode(bool mode);
public slots:
	void fillPage();
	void savePage();
private slots:
	void remove_C();
	void add_C();
	void edit_C(QTreeWidgetItem *);
	void edit_C();
	void restore_C();
	void enableButtons();
	void enableButtons2();
	void changeExp(double);
	void changeNiv(int);
	void changeExists(bool);
	void changeCapacity(int);
	void addCapacity();
	void changeAPs();
	void fullAPs();
	void changeLearning();
	void acquireAll();
protected:
	void buildWidget();
private:
	QWidget *buildPage1();
	QWidget *buildPage2();
	qint8 posAbility(quint8);
	void setCompleteAbility(quint8, bool);
	bool getCompleteAbility(quint8);

	static QList<QIcon> gfIcons;

	quint16 nivSup;
	quint8 id;
	GFORCES *gf_data;

	QTreeWidgetItem *addItem(quint8 abilityID, quint8 type, quint8 innateAbID, const QMap<int, QIcon> &abilityIcons);
	inline QTreeWidgetItem *addItem(quint8 abilityID, quint8 type, quint8 innateAbID=0) {
		return addItem(abilityID, type, innateAbID, QMap<int, QIcon>());
	}

	QListWidget *gfListe;
	QStackedWidget *stackedWidget;

	QCheckBox *existsE;
	QLineEdit *nameEdit;
	SpinBox16 *hpEdit;
	SpinBox32 *expEdit;
	QSpinBox *nivEdit;
	SpinBox8 *APsEdit;
	SpinBox16 *killsEdit, *KOsEdit;
	QLabel *inconnu1_labelE;
	SpinBox8 *unknown1E;
	QCheckBox *abilityCompleted;
	QDialog *dialog;
	QPushButton *okC;
	QTreeWidget *liste, *liste2;
	QPushButton *addC, *removeC, *editC, *learnC, *acquireAllC, *restoreF;
	QComboBox *selection;

	QLineEdit *grieverE;
	QCheckBox *odinE, *gilgameshE, *phoenixE;
};
