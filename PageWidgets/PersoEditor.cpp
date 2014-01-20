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

#include "PersoEditor.h"

QList<QIcon> PersoEditor::persoIcons;

PersoEditor::PersoEditor(QWidget *parent)
	: PageWidget(parent)
{
	this->id = 255;
}

void PersoEditor::updateMode(bool mode)
{
	unknownE->setVisible(mode);
	unknown1E->setVisible(mode);
	unknown1LabelE->setVisible(mode);
	if(LBindicator_E) {
		LBindicator_E->setVisible(mode);
		indicatorlabel->setVisible(mode);
	}
	qtyAllSpin->setMaximum(mode ? MAX_INT8 : 100);
	for(int i=0 ; i<32 ; ++i) {
		magie_E_model->item(i, 1)->setData(mode ? SpinBoxDelegate::SpinBox255 : SpinBoxDelegate::SpinBox100, Qt::UserRole);
	}

	QList<qint8> abilityFilter;
	if(!mode) {
		abilityFilter << 1; // Command ability
	}
	commande1_E->clear();
	commande2_E->clear();
	commande3_E->clear();
	commande1_E->addItem("-", 0);
	commande2_E->addItem("-", 0);
	commande3_E->addItem("-", 0);
	QMap<int, QIcon> icons = abilityIcons();
	fillAbilities(commande1_E, icons, abilityFilter);
	fillAbilities(commande2_E, icons, abilityFilter);
	fillAbilities(commande3_E, icons, abilityFilter);

	abilityFilter.clear();
	if(!mode) {
		abilityFilter << 2 << 3; // Character ability, Team ability
	}
	ability1_E->clear();
	ability2_E->clear();
	ability3_E->clear();
	ability4_E->clear();
	ability1_E->addItem("-", 0);
	ability2_E->addItem("-", 0);
	ability3_E->addItem("-", 0);
	ability4_E->addItem("-", 0);
	fillAbilities(ability1_E, icons, abilityFilter);
	fillAbilities(ability2_E, icons, abilityFilter);
	fillAbilities(ability3_E, icons, abilityFilter);
	fillAbilities(ability4_E, icons, abilityFilter);
}

void PersoEditor::buildWidget()
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(QMargins());
	persoListe = new QListWidget(this);
	persoListe->setUniformItemSizes(true);
	persoListe->setViewMode(QListView::IconMode);
	persoListe->setFlow(QListView::LeftToRight);
	persoListe->setIconSize(QSize(32,48));
	persoListe->setFixedHeight(57);
	persoListe->setWrapping(false);
	persoListe->setMovement(QListView::Static);
	
	tabWidget = new QTabWidget(this);
	tabWidget->addTab(buildPage1(), tr("État"));
	tabWidget->addTab(buildPage2(), tr("G-Forces"));
	tabWidget->addTab(buildPage3(), tr("Magies"));
	tabWidget->addTab(buildPage4(), tr("Capacités"));
	tabWidget->addTab(buildPage5(), tr("Associations"));
	lbWidget = new QWidget(this);
	tabWidget->addTab(lbWidget, tr("Limit Break"));
	
	layout->addWidget(persoListe);
	layout->addWidget(tabWidget);
	
	QListWidgetItem *item;
	for(quint8 i=0 ; i<8 ; ++i)
	{
		item = new QListWidgetItem(persoListe);
		QIcon icon = QIcon(QString(":/images/icons/perso%1.png").arg(i));
		persoIcons.append(icon);
		item->setIcon(icon);
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	}

	persoListe->setCurrentRow(0);
	
	connect(persoListe, SIGNAL(currentRowChanged(int)), SLOT(fillPage()));
}

QWidget *PersoEditor::buildPage1()
{
	QWidget *ret = new QWidget(this);
	
	existsE = new QCheckBox(ret);
	connect(existsE, SIGNAL(toggled(bool)), SLOT(changeExists(bool)));
	nameE = new QLineEdit(ret);
	current_HPsE = new SpinBox16(ret);
	
	QHBoxLayout *mainEdit_l = new QHBoxLayout;
	mainEdit_l->addWidget(new QLabel(tr("Disponible"), ret));
	mainEdit_l->addWidget(existsE);
	mainEdit_l->addWidget(new QLabel(tr("Nom :"), ret));
	mainEdit_l->addWidget(nameE);
	mainEdit_l->addWidget(new QLabel(tr("HP actuels :"), ret));
	mainEdit_l->addWidget(current_HPsE);
	
	expE = new SpinBox32(ret);
	connect(expE, SIGNAL(valueChanged(double)), SLOT(exp_S(double)));
	nivE = new QSpinBox(ret);
	nivE->setRange(1, 100);
	connect(nivE, SIGNAL(valueChanged(int)), SLOT(niv_S(int)));
	
	QHBoxLayout *mainEdit2_l = new QHBoxLayout;
	mainEdit2_l->addWidget(new QLabel(tr("Niveau :"), ret));
	mainEdit2_l->addWidget(nivE);
	mainEdit2_l->addWidget(new QLabel(tr("EXP :"), ret));
	mainEdit2_l->addWidget(expE);
	
	kills_E = new SpinBox16(ret);
	kos_E = new SpinBox16(ret);
	
	QHBoxLayout *statsEdit_l = new QHBoxLayout;
	statsEdit_l->addWidget(new QLabel(tr("Tués :"), ret));
	statsEdit_l->addWidget(kills_E);
	statsEdit_l->addWidget(new QLabel(tr("Mort :"), ret));
	statsEdit_l->addWidget(kos_E);
	
	id_E = new QComboBox(ret);
	id_E->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	for(quint8 i=0 ; i<11 ; ++i)
		id_E->addItem(QIcon(QString(":/images/icons/perso%1.png").arg(i)), Data::names().at(i), i);

	weapon_E = new QComboBox(ret);
	weapon_E->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	for(quint8 i=0 ; i<28 ; ++i)
		weapon_E->addItem(QIcon(QString(":/images/icons/weapon%1.png").arg(i,2,10,QChar('0'))), Data::weapons().at(i), i);
	for(quint8 i=28 ; i<33 ; ++i)
		weapon_E->addItem(Data::weapons().at(i), i);
	
	QHBoxLayout *statsEdit2_l = new QHBoxLayout;
	statsEdit2_l->addWidget(new QLabel(tr("Personnage :"), ret));
	statsEdit2_l->addWidget(id_E);
	statsEdit2_l->addWidget(new QLabel(tr("Arme :"), ret));
	statsEdit2_l->addWidget(weapon_E);

	QGroupBox *status_E = new QGroupBox(tr("Statut"), ret);
	QGridLayout *statusEdit_l = new QGridLayout(status_E);

	QStringList status;
	status << tr("Mort") << tr("Poison") << tr("Fossile") << tr("Darkness") << tr("Aphasie") << tr("Trouble") << tr("Zombie");
	QCheckBox *status_checkBox;
	for(int i=0 ; i<7 ; ++i)
	{
		status_checkBox = new QCheckBox(status.at(i), status_E);
		status_checkBox->setIcon(QIcon(QString(":/images/icons/mtl0%1.png").arg(i)));
		status_list_E.append(status_checkBox);
		statusEdit_l->addWidget(status_checkBox, i/4, i%4);
	}
	status_checkBox = new QCheckBox(tr("???"), status_E);
	status_list_E.append(status_checkBox);
	statusEdit_l->addWidget(status_checkBox, 1, 3);
	
	unknownE = new QGroupBox(tr("Inconnu"), ret);
	unknown2E = new SpinBox8(unknownE);
	unknown3E = new SpinBox8(unknownE);
	unknown4E = new SpinBox8(unknownE);
	unknown5E = new SpinBox8(unknownE);
	unknown6E = new QSpinBox(unknownE);
	unknown6E->setRange(0, 31);
	
	QHBoxLayout *inconnuEdit_l = new QHBoxLayout(unknownE);
	inconnuEdit_l->addWidget(new QLabel(tr("1 :"), unknownE));
	inconnuEdit_l->addWidget(unknown2E);
	inconnuEdit_l->addWidget(new QLabel(tr("2 :"), unknownE));
	inconnuEdit_l->addWidget(unknown3E);
	inconnuEdit_l->addWidget(new QLabel(tr("3 :"), unknownE));
	inconnuEdit_l->addWidget(unknown4E);
	inconnuEdit_l->addWidget(new QLabel(tr("4 :"), unknownE));
	inconnuEdit_l->addWidget(unknown5E);
	inconnuEdit_l->addWidget(new QLabel(tr("5 :"), unknownE));
	inconnuEdit_l->addWidget(unknown6E);
	
	bonus_E = new QGroupBox(tr("Bonus"), ret);
	HPs_E = new SpinBox16(bonus_E);
	vgr_E = new SpinBox8(bonus_E);
	dfs_E = new SpinBox8(bonus_E);
	mgi_E = new SpinBox8(bonus_E);
	psy_E = new SpinBox8(bonus_E);
	vts_E = new SpinBox8(bonus_E);
	chc_E = new SpinBox8(bonus_E);
	
	QGridLayout *bonusEdit_l = new QGridLayout(bonus_E);
	bonusEdit_l->addWidget(new QLabel(tr("Bonus HP :"), bonus_E), 0, 0);
	bonusEdit_l->addWidget(HPs_E, 0, 1);
	bonusEdit_l->addWidget(new QLabel(tr("Bonus vgr :"), bonus_E), 0, 2);
	bonusEdit_l->addWidget(vgr_E, 0, 3);
	bonusEdit_l->addWidget(new QLabel(tr("Bonus dfs :"), bonus_E), 0, 4);
	bonusEdit_l->addWidget(dfs_E, 0, 5);
	bonusEdit_l->addWidget(new QLabel(tr("Bonus mgi :"), bonus_E), 0, 6);
	bonusEdit_l->addWidget(mgi_E, 0, 7);
	bonusEdit_l->addWidget(new QLabel(tr("Bonus psy :"), bonus_E), 1, 0);
	bonusEdit_l->addWidget(psy_E, 1, 1);
	bonusEdit_l->addWidget(new QLabel(tr("Bonus vts :"), bonus_E), 1, 2);
	bonusEdit_l->addWidget(vts_E, 1, 3);
	bonusEdit_l->addWidget(new QLabel(tr("Bonus chc :"), bonus_E), 1, 4);
	bonusEdit_l->addWidget(chc_E, 1, 5);

	alternativeE = new QCheckBox(tr("Costume alternatif (Seed, galbadien)"), ret);
	lock1E = new QCheckBox(tr("Vérouillé 1 (menu 'remplacer')"), ret);
	lock2E = new QCheckBox(tr("Vérouillé 2 (menu 'remplacer')"), ret);

	QHBoxLayout *alternativeL = new QHBoxLayout();
	alternativeL->addWidget(alternativeE);
	alternativeL->addWidget(lock1E);
	alternativeL->addWidget(lock2E);
	
	QVBoxLayout *layout = new QVBoxLayout(ret);
	layout->addLayout(mainEdit_l);
	layout->addLayout(mainEdit2_l);
	layout->addLayout(statsEdit_l);
	layout->addLayout(statsEdit2_l);
	layout->addWidget(status_E);
	layout->addWidget(bonus_E);
	layout->addWidget(unknownE);
	layout->addLayout(alternativeL);
	layout->addStretch();
	
	return ret;
}

QWidget *PersoEditor::buildPage2()
{
	QWidget *ret = new QWidget(this);

	for(int i=0 ; i<16 ; ++i) {
		QCheckBox *curCheckBox;
		QSpinBox *curSpinBox;
		QLabel *curLabel;
		poss_E.append(curCheckBox = new QCheckBox(Data::gfnames().at(i), ret));
		curCheckBox->setIcon(QIcon(QString(":/images/icons/gf%1.png").arg(i)));
		comp_E.append(curSpinBox = new QSpinBox(ret));
		curSpinBox->setRange(-59535, 6000);
		compLabel_E.append(curLabel = new QLabel(ret));
		curLabel->setTextFormat(Qt::PlainText);
		curLabel->setTextInteractionFlags(Qt::NoTextInteraction);
		connect(curSpinBox, SIGNAL(valueChanged(int)), SLOT(updateCompLabels()));
	}
	possAll_E = new QCheckBox(tr("Sélectionner tout"), ret);
	compAll_E = new QSpinBox(ret);
	compAll_E->setRange(-59535, 6000);
	compLabelAll_E = new QLabel(ret);
	QPushButton *apply = new QPushButton(tr("Appliquer"), ret);
	connect(possAll_E, SIGNAL(toggled(bool)), SLOT(selectAllGFs(bool)));
	connect(compAll_E, SIGNAL(valueChanged(int)), SLOT(updateCompAllLabel()));
	connect(apply, SIGNAL(clicked()), SLOT(updateAllCompatibilities()));

	QGridLayout *grid = new QGridLayout(ret);
	grid->addWidget(new QLabel(tr("Compatibilité"), ret), 0, 1, Qt::AlignRight);
	grid->addWidget(new QLabel(tr("Compatibilité"), ret), 0, 4, Qt::AlignRight);
	for(int i=0 ; i<8 ; ++i) {
		for(int j=0 ; j<2 ; j++) {
			grid->addWidget(poss_E.at(i), i+1, 0);
			grid->addWidget(comp_E.at(i), i+1, 1, Qt::AlignRight);
			grid->addWidget(compLabel_E.at(i), i+1, 2, Qt::AlignLeft);

			grid->addWidget(poss_E.at(i+8), i+1, 3);
			grid->addWidget(comp_E.at(i+8), i+1, 4, Qt::AlignRight);
			grid->addWidget(compLabel_E.at(i+8), i+1, 5, Qt::AlignLeft);
		}
	}
	grid->setRowStretch(9, 1);
	grid->addWidget(compLabelAll_E, 10, 4, Qt::AlignRight);
	grid->addWidget(possAll_E, 11, 3);
	grid->addWidget(compAll_E, 11, 4, Qt::AlignRight);
	grid->addWidget(apply, 11, 5, Qt::AlignLeft);

	return ret;
}

QWidget *PersoEditor::buildPage3()
{
	QWidget *ret = new QWidget(this);

	QFont font;
	font.setPointSize(10);
	
	magie_E_model = new QStandardItemModel(ret);
	magie_E_model->setHorizontalHeaderLabels(QStringList() << tr("Nom") << tr("Qté"));
	magie_E_view = new QTreeView(ret);
	magie_E_view->setModel(magie_E_model);
	magie_E_view->setEditTriggers(QAbstractItemView::DoubleClicked);
	magie_E_view->setFont(font);
	magie_E_view->setItemDelegate(new SpinBoxDelegate(ret));
	magie_E_view->setIndentation(0);
	magie_E_view->setItemsExpandable(false);
	magie_E_view->header()->setStretchLastSection(false);
	magie_E_view->setUniformRowHeights(true);
	magie_E_view->setAllColumnsShowFocus(true);

	for(int i=0 ; i<32 ; ++i)
	{
		QList<QStandardItem *> items;
		QStandardItem *standardItem = new QStandardItem();
		standardItem->setData(SpinBoxDelegate::ComboBoxMagics, Qt::UserRole);
		items.append(standardItem);

		standardItem = new QStandardItem();
		standardItem->setData(SpinBoxDelegate::SpinBox255, Qt::UserRole);
		items.append(standardItem);

		magie_E_model->appendRow(items);
	}
	magie_E_view->header()->HEADER_VIEW_SET_RESIZE_MODE(0, QHeaderView::Stretch);

	font.setPixelSize(10);
	QPushButton *delAll = new QPushButton(tr("Supprimer tout"), ret);
	delAll->setFont(font);

	QWidget *qtyAll = new QWidget(ret);
	QPushButton *qtyAllOK = new QPushButton(tr("Tout"), qtyAll);
	qtyAllOK->setFont(font);
	qtyAllSpin = new SpinBox8(qtyAll);
	QHBoxLayout *qtyLayout = new QHBoxLayout(qtyAll);
	qtyLayout->addWidget(qtyAllSpin, 1);
	qtyLayout->addWidget(qtyAllOK);
	qtyLayout->setContentsMargins(QMargins());

	connect(delAll, SIGNAL(clicked()), SLOT(removeAllMagic()));
	connect(qtyAllOK, SIGNAL(clicked()), SLOT(qtyAllMagic()));

	QGridLayout *layout = new QGridLayout(ret);
	layout->addWidget(magie_E_view, 0, 0, 1, 2);
	layout->addWidget(delAll, 1, 0, Qt::AlignLeft);
	layout->addWidget(qtyAll, 1, 1, Qt::AlignRight);
	
	return ret;
}

QWidget *PersoEditor::buildPage4()
{
	QWidget *ret = new QWidget(this);
	
	QMap<int, QIcon> icons = abilityIcons();

	QGroupBox *commande_E = new QGroupBox(tr("Commandes"), ret);
	commande1_E = new QComboBox(commande_E);
	commande1_E->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	commande2_E = new QComboBox(commande_E);
	commande2_E->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	commande3_E = new QComboBox(commande_E);
	commande3_E->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	unknown1LabelE = new QLabel(tr("Inconnu (commande 4) :"), commande_E);
	unknown1E = new SpinBox8(commande_E);
	
	QGridLayout *commande_L = new QGridLayout(commande_E);
	commande_L->addWidget(new QLabel(tr("Commande 1 :"), commande_E), 0, 0);
	commande_L->addWidget(commande1_E, 0, 1);
	commande_L->addWidget(new QLabel(tr("Commande 2 :"), commande_E), 1, 0);
	commande_L->addWidget(commande2_E, 1, 1);
	commande_L->addWidget(new QLabel(tr("Commande 3 :"), commande_E), 2, 0);
	commande_L->addWidget(commande3_E, 2, 1);
	commande_L->addWidget(unknown1LabelE, 3, 0);
	commande_L->addWidget(unknown1E, 3, 1);
	commande_L->setRowStretch(4, 1);
	
	QGroupBox *ability_E = new QGroupBox(tr("Capacités"), ret);
	ability1_E = new QComboBox(ability_E);
	ability1_E->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	ability2_E = new QComboBox(ability_E);
	ability2_E->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	ability3_E = new QComboBox(ability_E);
	ability3_E->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	ability4_E = new QComboBox(ability_E);
	ability4_E->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	
	QGridLayout *ability_L = new QGridLayout(ability_E);
	ability_L->addWidget(new QLabel(tr("Capacité 1 :"), ability_E), 0, 0);
	ability_L->addWidget(ability1_E, 0, 1);
	ability_L->addWidget(new QLabel(tr("Capacité 2 :"), ability_E), 1, 0);
	ability_L->addWidget(ability2_E, 1, 1);
	ability_L->addWidget(new QLabel(tr("Capacité 3 :"), ability_E), 2, 0);
	ability_L->addWidget(ability3_E, 2, 1);
	ability_L->addWidget(new QLabel(tr("Capacité 4 :"), ability_E), 3, 0);
	ability_L->addWidget(ability4_E, 3, 1);
	ability_L->setRowStretch(4, 1);
	
	QHBoxLayout *layout = new QHBoxLayout(ret);
	layout->addWidget(commande_E);
	layout->addWidget(ability_E);
	
	return ret;
}

QWidget *PersoEditor::buildPage5()
{
	QWidget *ret = new QWidget(this);
	QComboBox *comboBox;
	QGroupBox *apt_E = new QGroupBox(tr("Aptitudes"), ret);
	for(int i=0 ; i<9 ; ++i)
	{
		aptitude_E.append(comboBox = new QComboBox(apt_E));
		int j=0;
		foreach(const QString &mag, Data::magics().list())
			comboBox->addItem(mag, j++);
	}
	QGridLayout *apt_L = new QGridLayout(apt_E);
	apt_L->addWidget(new QLabel(tr("HP :"), apt_E), 0, 0);
	apt_L->addWidget(aptitude_E[0], 0, 1);
	apt_L->addWidget(new QLabel(tr("Vigueur :"), apt_E), 1, 0);
	apt_L->addWidget(aptitude_E[1], 1, 1);
	apt_L->addWidget(new QLabel(tr("Défense :"), apt_E), 2, 0);
	apt_L->addWidget(aptitude_E[2], 2, 1);
	apt_L->addWidget(new QLabel(tr("Magie :"), apt_E), 3, 0);
	apt_L->addWidget(aptitude_E[3], 3, 1);
	apt_L->addWidget(new QLabel(tr("Psychisme :"), apt_E), 4, 0);
	apt_L->addWidget(aptitude_E[4], 4, 1);
	
	apt_L->addWidget(new QLabel(tr("Vitesse :"), apt_E), 1, 2);
	apt_L->addWidget(aptitude_E[5], 1, 3);
	apt_L->addWidget(new QLabel(tr("Esquive :"), apt_E), 2, 2);
	apt_L->addWidget(aptitude_E[6], 2, 3);
	apt_L->addWidget(new QLabel(tr("Protection :"), apt_E), 3, 2);
	apt_L->addWidget(aptitude_E[7], 3, 3);
	apt_L->addWidget(new QLabel(tr("Chance :"), apt_E), 4, 2);
	apt_L->addWidget(aptitude_E[8], 4, 3);
	
	QGroupBox *ele_E = new QGroupBox(tr("Élémental"), ret);
	for(int i=0 ; i<5 ; ++i)
	{
		element_E.append(comboBox = new QComboBox(ele_E));
		int j=0;
		foreach(const QString &mag, Data::magics().list())
			comboBox->addItem(mag, j++);
	}
	QGridLayout *ele_L = new QGridLayout(ele_E);
	ele_L->addWidget(new QLabel(tr("Attaque :"), ele_E), 0, 0);
	ele_L->addWidget(element_E[0], 0, 1);
	ele_L->addWidget(new QLabel(tr("Défense 1 :"), ele_E), 1, 0);
	ele_L->addWidget(element_E[1], 1, 1);
	ele_L->addWidget(new QLabel(tr("Défense 2 :"), ele_E), 2, 0);
	ele_L->addWidget(element_E[2], 2, 1);
	ele_L->addWidget(new QLabel(tr("Défense 3 :"), ele_E), 3, 0);
	ele_L->addWidget(element_E[3], 3, 1);
	ele_L->addWidget(new QLabel(tr("Défense 4 :"), ele_E), 4, 0);
	ele_L->addWidget(element_E[4], 4, 1);
	
	QGroupBox *mtl_E = new QGroupBox(tr("Mental"), ret);
	for(int i=0 ; i<5 ; ++i)
	{
		mental_E.append(comboBox = new QComboBox(mtl_E));
		int j=0;
		foreach(const QString &mag, Data::magics().list())
			comboBox->addItem(mag, j++);
	}
	QGridLayout *mtl_L = new QGridLayout(mtl_E);
	mtl_L->addWidget(new QLabel(tr("Attaque :"), mtl_E), 0, 0);
	mtl_L->addWidget(mental_E[0], 0, 1);
	mtl_L->addWidget(new QLabel(tr("Défense 1 :"), mtl_E), 1, 0);
	mtl_L->addWidget(mental_E[1], 1, 1);
	mtl_L->addWidget(new QLabel(tr("Défense 2 :"), mtl_E), 2, 0);
	mtl_L->addWidget(mental_E[2], 2, 1);
	mtl_L->addWidget(new QLabel(tr("Défense 3 :"), mtl_E), 3, 0);
	mtl_L->addWidget(mental_E[3], 3, 1);
	mtl_L->addWidget(new QLabel(tr("Défense 4 :"), mtl_E), 4, 0);
	mtl_L->addWidget(mental_E[4], 4, 1);
	
	QGridLayout *grid = new QGridLayout(ret);
	grid->addWidget(apt_E, 0, 0, 1, 2);
	grid->addWidget(ele_E, 1, 0);
	grid->addWidget(mtl_E, 1, 1);
	grid->setRowStretch(2, 1);
	
	return ret;
}

void PersoEditor::buildPage6()
{
	foreach(QObject *child, lbWidget->children())	delete child;
	//Cette fonction varie en fonction du personnage, elle est appelée lors du remplissage et pas lors de l'initialisation
	QGridLayout *grid = new QGridLayout(lbWidget);
	
	int cur=0;
	LB_E.clear();
	linoaLB_E.clear();
	tabWidget->setTabEnabled(5, true);
	QLabel *angeloPix;
	HelpWidget *info;

	LBindicator_E = 0;
	indicatorlabel = 0;
	
	switch(id)
	{
	case SQUALL:
		LB_E << new QCheckBox(tr("Gunblade auto"), lbWidget);
		LB_E.first()->setChecked(data->misc2.renzokuken_auto & 1);
		LB_E << new QCheckBox(tr("Indicateur renzokuken"), lbWidget);
		LB_E.at(1)->setChecked(data->misc2.renzokuken_indicator < 128);
		LB_E.at(1)->setEnabled((data->misc2.renzokuken_auto & 1) == 0);
		LBindicator_E = new QSlider(Qt::Horizontal, lbWidget);
		LBindicator_E->setMaximum(127);
		LBindicator_E->setValue(data->misc2.renzokuken_indicator < 128 ? data->misc2.renzokuken_indicator : 0);
		LBindicator_E->setEnabled((data->misc2.renzokuken_auto & 1) == 0 || data->misc2.renzokuken_indicator < 128);
		connect(LB_E.first(), SIGNAL(stateChanged(int)), SLOT(LBautoChange(int)));
		connect(LB_E.at(1), SIGNAL(stateChanged(int)), SLOT(LBindicatorChange(int)));
		indicatorlabel = new QLabel(QString::number(data->misc2.renzokuken_indicator < 128 ? data->misc2.renzokuken_indicator : 0), lbWidget);
		connect(LBindicator_E, SIGNAL(valueChanged(int)), indicatorlabel, SLOT(setNum(int)));
		grid->addWidget(LB_E.first(), 0, 0);
		grid->addWidget(LB_E.at(1), 1, 0);
		grid->addWidget(LBindicator_E, 1, 1);
		grid->addWidget(indicatorlabel, 1, 2);
		grid->setRowStretch(2, 1);
		grid->setColumnStretch(3, 1);
		return;
	case ZELL:
		LB_E << new QCheckBox(tr("Ring Master auto"), lbWidget);
		LB_E.first()->setChecked((data->misc2.renzokuken_auto >> 1) & 1);
		grid->addWidget(LB_E.first(), 0, 0);
		cur++;
		for(int i=0 ; i<2 ; ++i)
		{
			for(int j=0 ; j<5 ; ++j)
			{
				LB_E << new QCheckBox(Data::zellLBs().at(cur - 1), lbWidget);
				grid->addWidget(LB_E.at(cur), 1 + i, j);
				LB_E.at(cur)->setChecked((data->limitb.zell >> (cur - 1)) & 1);
				cur++;
			}
		}
		grid->setRowStretch(3, 1);
		return;
	case IRVINE:
		for(int i=0 ; i<2 ; ++i)
		{
			for(int j=0 ; j<4 ; ++j)
			{
				LB_E << new QCheckBox(Data::irvineLBs().at(cur), lbWidget);
				grid->addWidget(LB_E.at(cur), i, j);
				LB_E.at(cur)->setChecked((data->limitb.irvine >> cur) & 1);
				cur++;
			}
		}
		grid->setRowStretch(2, 1);
		return;
	case QUISTIS:
		for(int i=0 ; i<4 ; ++i)
		{
			for(int j=0 ; j<4 ; ++j)
			{
				LB_E << new QCheckBox(Data::quistisLBs().at(cur), lbWidget);
				grid->addWidget(LB_E.at(cur), i, j);
				LB_E.at(cur)->setChecked((data->limitb.quistis >> cur) & 1);
				cur++;
			}
		}
		grid->setRowStretch(4, 1);
		return;
	case RINOA:
		angel_E = new QLineEdit(saveData->perso(ANGELO), lbWidget);
		angel_disabledE = new QCheckBox(tr("Angel désactivé"), lbWidget);
		angel_disabledE->setChecked((data->misc2.dream >> 4) & 1);
		a_wing_enabledE = new QCheckBox(tr("Canonisation activé"), lbWidget);
		a_wing_enabledE->setChecked((data->misc2.dream >> 5) & 1);
		angeloPix = new QLabel(lbWidget);
		angeloPix->setPixmap(QPixmap(":/images/icons/perso15.png"));

		info = new HelpWidget(32, tr("<b>Case cochée :</b> limit break appris<br/><b>Case partiellement cochée :</b>"
									 " limit break connu<br/><b>Valeur :</b> nombre de points restants pour apprendre "
									 "le limit break"), lbWidget);

		grid->addWidget(new QLabel(Data::names().at(ANGELO)+tr(" :"), lbWidget), 0, 0);
		grid->addWidget(angel_E, 0, 1, 1, 6);
		grid->addWidget(info, 1, 0, 1, 7);
		grid->addWidget(angeloPix, 0, 7, 3, 1, Qt::AlignRight);

		for(int i=2 ; i<4 ; ++i)
		{
			for(int j=0 ; j<4 ; ++j)
			{
				LB_E << new QCheckBox(Data::rinoaLBs().at(cur), lbWidget);
				LB_E.at(cur)->setTristate(true);
				linoaLB_E << new SpinBox8(lbWidget);

				grid->addWidget(LB_E.at(cur), i, j*2);
				grid->addWidget(linoaLB_E.at(cur), i, j*2+1);

				LB_E.at(cur)->setCheckState(((data->limitb.angel_known >> cur) & 1) ? Qt::PartiallyChecked : Qt::Unchecked);
				if((data->limitb.angel_completed >> cur) & 1)	LB_E.at(cur)->setCheckState(Qt::Checked);
				linoaLB_E.at(cur)->setValue(data->limitb.angel_pts[cur]);

				++cur;
			}
		}
		grid->addWidget(angel_disabledE, 4, 0, 1, 8);
		grid->addWidget(a_wing_enabledE, 5, 0, 1, 8);
		grid->setRowStretch(6, 1);
		return;
	case SELPHIE:
		for(int i=0 ; i<2 ; ++i)
		{
			for(int j=0 ; j<3 ; ++j)
			{
				LB_E << new QCheckBox(Data::magics().at(cur+51), lbWidget);
				grid->addWidget(LB_E.at(cur), i, j);
				LB_E.at(cur)->setChecked((data->limitb.selphie >> cur) & 1);
				cur++;
			}
		}
		grid->setRowStretch(2, 1);
		return;
	default:
		tabWidget->setTabEnabled(5, false);
		return;
	}
}

void PersoEditor::fillPage()
{
	savePage();

	if(persoListe->currentRow()<0)	return;

	for(quint8 i=0 ; i<8 ; ++i)
	{
		persoListe->item(i)->setIcon(persoIcons.at(i).pixmap(32, 48, data->persos[i].exists & 1 ? QIcon::Normal : QIcon::Disabled));
	}

	this->id = persoListe->currentRow();
	this->perso_data = &data->persos[id];

	/* PAGE 1 */

	existsE->setChecked(perso_data->exists & 1);
	nameE->setText(saveData->perso(id));
	nameE->setDisabled(id != SQUALL && id != RINOA);

	current_HPsE->setValue(perso_data->current_HPs);

	HPs_E->setValue(perso_data->HPs);
	expE->setValue(perso_data->exp);
	vgr_E->setValue(perso_data->VGR);
	dfs_E->setValue(perso_data->DFS);
	mgi_E->setValue(perso_data->MGI);
	psy_E->setValue(perso_data->PSY);
	vts_E->setValue(perso_data->VTS);
	chc_E->setValue(perso_data->CHC);
	kills_E->setValue(perso_data->kills);
	kos_E->setValue(perso_data->KOs);

	unknown1E->setValue(perso_data->u1);
	unknown2E->setValue(perso_data->u2);
	unknown3E->setValue(perso_data->u3);
	unknown4E->setValue(perso_data->u4);
	unknown5E->setValue(perso_data->u5);
	unknown6E->setValue(perso_data->exists >> 3);
	alternativeE->setChecked(perso_data->alternative_model & 1);
	lock1E->setChecked((perso_data->exists >> 1) & 1);
	lock2E->setChecked((perso_data->exists >> 2) & 1);

	setCurrentIndex(id_E, perso_data->ID);
	setCurrentIndex(weapon_E, perso_data->weaponID);

	for(int i=0 ; i<8 ; ++i)
		status_list_E.at(i)->setChecked((perso_data->status >> i) & 1);

	/* PAGE 2 */
	
	for(int i=0 ; i<16 ; ++i)
	{
		poss_E[i]->setChecked((perso_data->gfs >> i) & 1);
		comp_E[i]->setValue(6000-perso_data->compatibility[i]);
	}

	/* PAGE 3 */
	
	for(int i=0 ; i<32 ; ++i)
	{
		quint8 magID = perso_data->magies[i] & 0xFF;
		QStandardItem *standardItem = magie_E_model->item(i);
		standardItem->setText(magID < 57 ? Data::magics().at(magID) : QString::number(magID));
		standardItem->setData(magID);

		magie_E_model->item(i, 1)->setText(QString::number(perso_data->magies[i] >> 8));
	}
	magie_E_view->resizeColumnToContents(1);

	/* PAGE 4 */

	setCurrentIndex(commande1_E, perso_data->commands[0]);
	setCurrentIndex(commande2_E, perso_data->commands[1]);
	setCurrentIndex(commande3_E, perso_data->commands[2]);
	setCurrentIndex(ability1_E, perso_data->abilities[0]);
	setCurrentIndex(ability2_E, perso_data->abilities[1]);
	setCurrentIndex(ability3_E, perso_data->abilities[2]);
	setCurrentIndex(ability4_E, perso_data->abilities[3]);

	/* PAGE 5 */

	setCurrentIndex(aptitude_E[0], perso_data->j_HP);
	setCurrentIndex(aptitude_E[1], perso_data->j_VGR);
	setCurrentIndex(aptitude_E[2], perso_data->j_DFS);
	setCurrentIndex(aptitude_E[3], perso_data->j_MGI);
	setCurrentIndex(aptitude_E[4], perso_data->j_PSY);
	setCurrentIndex(aptitude_E[5], perso_data->j_VTS);
	setCurrentIndex(aptitude_E[6], perso_data->j_ESQ);
	setCurrentIndex(aptitude_E[7], perso_data->j_PRC);
	setCurrentIndex(aptitude_E[8], perso_data->j_CHC);

	setCurrentIndex(element_E[0], perso_data->j_attEle);
	for(int i=0 ; i<4 ; ++i)	setCurrentIndex(element_E[i+1], perso_data->j_defEle[i]);

	setCurrentIndex(mental_E[0], perso_data->j_attMtl);
	for(int i=0 ; i<4 ; ++i)	setCurrentIndex(mental_E[i+1], perso_data->j_defMtl[i]);

	/* PAGE 6 */

	buildPage6();
	updateMode(Config::mode());
}

void PersoEditor::savePage()
{
	if(!loaded)	return;

	/* PAGE 1 */

	perso_data->exists = (unknown6E->value() << 3) | (lock2E->isChecked() << 2) | (lock1E->isChecked() << 1) | int(existsE->isChecked());

	if(id == SQUALL || id == RINOA) {
		saveData->setPerso(id, nameE->text());
	}

	perso_data->current_HPs = current_HPsE->value();
	perso_data->HPs = HPs_E->value();
	perso_data->exp = expE->value();
	perso_data->VGR = vgr_E->value();
	perso_data->DFS = dfs_E->value();
	perso_data->MGI = mgi_E->value();
	perso_data->PSY = psy_E->value();
	perso_data->VTS = vts_E->value();
	perso_data->CHC = chc_E->value();
	perso_data->kills = kills_E->value();
	data->misc3.kills[id] = kills_E->value();
	perso_data->KOs = kos_E->value();
	data->misc3.ko[id] = kos_E->value();
	perso_data->ID = id_E->itemData(id_E->currentIndex()).toInt();
	perso_data->weaponID = weapon_E->itemData(weapon_E->currentIndex()).toInt();

	perso_data->status = 0;
	for(int i=0 ; i<8 ; ++i)
		perso_data->status |= status_list_E.at(i)->isChecked()<<i;

	perso_data->u1 = unknown1E->value();
	perso_data->u2 = unknown2E->value();
	perso_data->u3 = unknown3E->value();
	perso_data->u4 = unknown4E->value();
	perso_data->u5 = unknown5E->value();
	perso_data->alternative_model = (perso_data->alternative_model & 0xFE) | int(alternativeE->isChecked());

	/* PAGE 2 */

	for(int i=0 ; i<16 ; ++i)
	{
		perso_data->gfs = (poss_E.at(i)->isChecked() ? perso_data->gfs | (1 << i) : perso_data->gfs & ~(1 << i));
		perso_data->compatibility[i] = 6000-comp_E.at(i)->value();
	}

	/* PAGE 3 */

	for(int i=0 ; i<32 ; ++i)
	{
		perso_data->magies[i] = magie_E_model->item(i, 0)->data().toInt() & 0xFF;
		perso_data->magies[i] |= (magie_E_model->item(i, 1)->text().toInt() & 0xFF) << 8;
	}

	/* PAGE 4 */

	perso_data->commands[0] = commande1_E->itemData(commande1_E->currentIndex()).toInt();
	perso_data->commands[1] = commande2_E->itemData(commande2_E->currentIndex()).toInt();
	perso_data->commands[2] = commande3_E->itemData(commande3_E->currentIndex()).toInt();
	perso_data->abilities[0] = ability1_E->itemData(ability1_E->currentIndex()).toInt();
	perso_data->abilities[1] = ability2_E->itemData(ability2_E->currentIndex()).toInt();
	perso_data->abilities[2] = ability3_E->itemData(ability3_E->currentIndex()).toInt();
	perso_data->abilities[3] = ability4_E->itemData(ability4_E->currentIndex()).toInt();

	/* PAGE 5 */

	perso_data->j_HP = aptitude_E[0]->itemData(aptitude_E[0]->currentIndex()).toInt();
	perso_data->j_VGR = aptitude_E[1]->itemData(aptitude_E[1]->currentIndex()).toInt();
	perso_data->j_DFS = aptitude_E[2]->itemData(aptitude_E[2]->currentIndex()).toInt();
	perso_data->j_MGI = aptitude_E[3]->itemData(aptitude_E[3]->currentIndex()).toInt();
	perso_data->j_PSY = aptitude_E[4]->itemData(aptitude_E[4]->currentIndex()).toInt();
	perso_data->j_VTS = aptitude_E[5]->itemData(aptitude_E[5]->currentIndex()).toInt();
	perso_data->j_ESQ = aptitude_E[6]->itemData(aptitude_E[6]->currentIndex()).toInt();
	perso_data->j_PRC = aptitude_E[7]->itemData(aptitude_E[7]->currentIndex()).toInt();
	perso_data->j_CHC = aptitude_E[8]->itemData(aptitude_E[8]->currentIndex()).toInt();

	perso_data->j_attEle = element_E[0]->itemData(element_E[0]->currentIndex()).toInt();
	for(int i=0 ; i<4 ; ++i)	perso_data->j_defEle[i] = element_E[i+1]->itemData(element_E[i+1]->currentIndex()).toInt();

	perso_data->j_attMtl = mental_E[0]->itemData(mental_E[0]->currentIndex()).toInt();
	for(int i=0 ; i<4 ; ++i)	perso_data->j_defMtl[i] = mental_E[i+1]->itemData(mental_E[i+1]->currentIndex()).toInt();

	/* PAGE 6 */

	switch(id)
	{
	case SQUALL:
		data->misc2.renzokuken_auto = (data->misc2.renzokuken_auto & 0xFE) | int(LB_E.first()->isChecked());
		data->misc2.renzokuken_indicator = !LB_E.at(1)->isChecked() ? 128 : LBindicator_E->value();
		break;
	case ZELL:
		data->misc2.renzokuken_auto = (data->misc2.renzokuken_auto & 0xFD) | (LB_E.first()->isChecked() << 1);
		data->limitb.zell = 0;
		for(int i=0 ; i<10 ; ++i)
			data->limitb.zell |= LB_E.at(i + 1)->isChecked() << i;
		break;
	case IRVINE:
		data->limitb.irvine = 0;
		for(int i=0 ; i<8 ; ++i)
			data->limitb.irvine |= LB_E.at(i)->isChecked() << i;
		break;
	case QUISTIS:
		data->limitb.quistis = 0;
		for(int i=0 ; i<16 ; ++i)
			data->limitb.quistis |= LB_E.at(i)->isChecked() << i;
		break;
	case RINOA:
		saveData->setPerso(ANGELO, angel_E->text());
		data->limitb.angel_known = data->limitb.angel_completed = 0;
		for(int i=0 ; i<8 ; ++i)
		{
			data->limitb.angel_known |= (LB_E.at(i)->checkState() != Qt::Unchecked) << i;
			data->limitb.angel_completed |= (LB_E.at(i)->checkState() == Qt::Checked) << i;
			data->limitb.angel_pts[i] = linoaLB_E.at(i)->value();
		}
		data->misc2.dream = (a_wing_enabledE->isChecked() << 5) | (angel_disabledE->isChecked() << 4) | (data->misc2.dream & 0xCF);
		break;
	case SELPHIE:
		data->limitb.selphie = 0;
		for(int i=0 ; i<6 ; ++i)
			data->limitb.selphie |= LB_E.at(i)->isChecked() << i;
		break;
	}
}

void PersoEditor::exp_S(double value)
{
	int niv = ((quint32)value/1000)+1;
	if(niv>100)	niv = 100;
	if(nivE->value() != niv)
	{
		nivE->setValue(niv);
	}
}

void PersoEditor::niv_S(int value)
{
	qint32 exp = (value-1)*1000;
	if(exp > expE->value() || exp + 1000 <= expE->value())
	{
		expE->setValue(exp);
	}
}

void PersoEditor::changeExists(bool exists)
{
	persoListe->currentItem()->setIcon(persoIcons.at(persoListe->currentRow()).pixmap(32, 48, exists ? QIcon::Normal : QIcon::Disabled));
}

void PersoEditor::updateCompLabels()
{
	QSpinBox *comp = (QSpinBox *)sender();
	compLabel_E.at(comp_E.indexOf(comp))->setText(QString("(%1)").arg(comp->value()/5));
}

void PersoEditor::updateCompAllLabel()
{
	compLabelAll_E->setText(QString("(%1)").arg(compAll_E->value()/5));
}

void PersoEditor::selectAllGFs(bool all)
{
	foreach(QCheckBox *poss, poss_E) {
		poss->setChecked(all);
	}
}

void PersoEditor::updateAllCompatibilities()
{
	int val = compAll_E->value();
	foreach(QSpinBox *comp, comp_E) {
		comp->setValue(val);
	}
}

void PersoEditor::removeAllMagic()
{
	for(int i=0 ; i<32 ; ++i)
	{
		QStandardItem *standardItem = magie_E_model->item(i);
		standardItem->setText(Data::magics().at(0));
		standardItem->setData(0);

		magie_E_model->item(i, 1)->setText("0");
	}
}

void PersoEditor::qtyAllMagic()
{
	int qty = qtyAllSpin->value();
	for(quint8 i=0 ; i<32 ; ++i)
	{
		if(qty==0 || magie_E_model->item(i)->data().toInt() != 0)
			magie_E_model->item(i, 1)->setText(QString::number(qty));
	}
}

void PersoEditor::LBautoChange(int state)
{
	LBindicator_E->setEnabled(state == Qt::Unchecked && LB_E.at(1)->isChecked());
	LB_E.at(1)->setEnabled(state == Qt::Unchecked);
}

void PersoEditor::LBindicatorChange(int state)
{
	LBindicator_E->setEnabled(state == Qt::Checked);
}
