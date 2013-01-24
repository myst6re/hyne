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

#ifndef DEF_TTEDITOR
#define DEF_TTEDITOR

#include "SpinBoxDelegate.h"
#include "PageWidget.h"

class TTriadEditor : public PageWidget
{
	Q_OBJECT
public:
	explicit TTriadEditor(QWidget *parent=0);
	inline QString name() { return tr("Triple Triad"); }
	void updateMode(bool mode);
public slots:
	void fillPage();
	void savePage();
private slots:
	void fillRules(QListWidgetItem*,QListWidgetItem*);
	void selectAll1(bool);
	void selectAll2(bool);
	void qtyAll();
	void squallAll();
	void changeCardPreview();
	void changeCardPreview2();
protected:
	void buildWidget();
private:
	QWidget *buildPage1();
	QWidget *buildPage2();
	void saveRules(int id);
	void setCardPreview(int i);

	QTabWidget *tabWidget;
	// Page 1
	QStandardItemModel *cardE_model, *cardE_model2;
	QTreeView *cardE_view, *cardE_view2;
	QSpinBox *qtyAllSpin;
	QLabel *cardPreview;
	// Page 2
	QListWidget *ruleE_list;
	QComboBox *queenE, *last_regionE, *last2_regionE, *traderating_regionE, *ttcardqueenquestE;
	QSpinBox *traderatingE, *ttvictorycountE, *ttdefeatcountE, *ttegalitycountE, *ttdegenerationE;
	QDoubleSpinBox *unknown2E;
	QSpinBox *unknown1E;
	QLabel *unknown1E_label, *unknown2E_label;
	QGroupBox *group2;
	QList<QCheckBox *> rules;
	QList<QRadioButton *> trade_rules;
};

#endif
