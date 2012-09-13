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

#ifndef DEF_PERSOEDITOR
#define DEF_PERSOEDITOR

#include "SpinBoxDelegate.h"
#include "PageWidget.h"

class PersoEditor : public PageWidget
{
	Q_OBJECT
public:
	explicit PersoEditor(QWidget *parent=0);
	inline QString name() { return tr("Personnages"); }
	void updateMode(bool mode);
public slots:
	void fillPage();
	void savePage();
private slots:
	void niv_S(int value);
	void exp_S(double value);
	void changeExists(bool exists);
	void updateCompLabels();
	void removeAllMagic();
	void qtyAllMagic();
	void LBindicatorChange(int state);
	void LBautoChange(int state);
protected:
	void buildWidget();
private:
	QWidget *buildPage1();
	QWidget *buildPage2();
	QWidget *buildPage3();
	QWidget *buildPage4();
	QWidget *buildPage5();
	void buildPage6();

	quint8 id;
	PERSONNAGES *perso_data;

	static QList<QIcon> persoIcons;

	QListWidget *persoListe;
	QWidget *lbWidget;
	QTabWidget *tabWidget;
	// Page 1
	QCheckBox *existsE;
	QLineEdit *nameE;
	QDoubleSpinBox *expE;
	QSpinBox *current_HPsE, *nivE;
	QGroupBox *bonus_E;
	QSpinBox *HPs_E, *vgr_E, *dfs_E, *mgi_E, *psy_E, *vts_E, *chc_E;
	QSpinBox *kills_E, *kos_E;
	QComboBox *id_E, *weapon_E;
	QList<QCheckBox *> status_list_E;
	QGroupBox *unknownE;
	QLabel *unknown1LabelE;
	QSpinBox *unknown1E, *unknown2E, *unknown3E, *unknown4E, *unknown5E, *unknown6E;
	QCheckBox *alternativeE, *lock1E, *lock2E;
	// Page 2
	QList<QCheckBox *> poss_E;
	QList<QSpinBox *> comp_E;
	QList<QLabel *> compLabel_E;
	// Page 3
	QTreeView *magie_E_view;
	QStandardItemModel *magie_E_model;
	QSpinBox *qtyAllSpin;
	// Page 4
	QComboBox *commande1_E, *commande2_E, *commande3_E;
	QComboBox *ability1_E, *ability2_E, *ability3_E, *ability4_E;
	// Page 5
	QList<QComboBox *> aptitude_E, element_E, mental_E;
	// Page 6
	QList<QCheckBox *> LB_E;
	QLineEdit *angel_E;
	QCheckBox *angel_disabledE, *a_wing_enabledE;
	QList<QSpinBox *> linoaLB_E;
	QLabel *indicatorlabel;
	QSlider *LBindicator_E;
};

#endif
