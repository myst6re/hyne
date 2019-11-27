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

#include "GfEditor.h"

QList<QIcon> GfEditor::gfIcons;

GfEditor::GfEditor(QWidget *parent)
	: PageWidget(parent)
{
	this->id = 255;
}

void GfEditor::updateMode(bool mode)
{
	inconnu1_labelE->setVisible(mode);
	unknown1E->setVisible(mode);
}

void GfEditor::buildWidget()
{
	gfListe = new QListWidget(this);
	gfListe->setUniformItemSizes(true);
	gfListe->setViewMode(QListView::IconMode);
	gfListe->setFlow(QListView::LeftToRight);
	gfListe->setIconSize(QSize(32,48));
	gfListe->setFixedHeight(57);
	gfListe->setWrapping(false);
	gfListe->setMovement(QListView::Static);
	gfListe->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	gfListe->setAutoScroll(false);

	gfIcons.clear();

	for(quint8 i=0 ; i<17 ; ++i)
	{
		QListWidgetItem *item = new QListWidgetItem(gfListe);
		QIcon icon = QIcon(QString(":/images/icons/gf%1.png").arg(i));
		gfIcons.append(icon);
		item->setIcon(icon);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
	gfListe->setCurrentRow(0);

	stackedWidget = new QStackedWidget(this);
	stackedWidget->addWidget(buildPage1());
	stackedWidget->addWidget(buildPage2());
	stackedWidget->setCurrentIndex(0);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(QMargins());
	layout->addWidget(gfListe);
	layout->addWidget(stackedWidget);

	connect(gfListe, SIGNAL(currentRowChanged(int)), SLOT(fillPage()));
}

QWidget *GfEditor::buildPage1()
{
	QWidget *ret = new QWidget(this);

	existsE = new QCheckBox(tr("Disponible"), ret);
	connect(existsE, SIGNAL(toggled(bool)), SLOT(changeExists(bool)));
	nameEdit = new QLineEdit(ret);
	hpEdit = new SpinBox16(ret);

	expEdit = new SpinBox32(ret);
	connect(expEdit, SIGNAL(valueChanged(double)), SLOT(changeExp(double)));
	nivEdit = new QSpinBox(ret);
	nivEdit->setRange(1, 100);
	connect(nivEdit, SIGNAL(valueChanged(int)), SLOT(changeNiv(int)));

	killsEdit = new SpinBox16(ret);
	KOsEdit = new SpinBox16(ret);

	inconnu1_labelE = new QLabel(tr("Inconnu :"), ret);
	unknown1E = new SpinBox8(ret);

	QGridLayout *statEditL = new QGridLayout;
	statEditL->addWidget(new QLabel(tr("Nom :"), ret), 0, 0);
	statEditL->addWidget(nameEdit, 0, 1);
	statEditL->addWidget(new QLabel(tr("HP actuels :"), ret), 0, 2);
	statEditL->addWidget(hpEdit, 0, 3);
	statEditL->addWidget(new QLabel(tr("Niveau :"), ret), 1, 0);
	statEditL->addWidget(nivEdit, 1, 1);
	statEditL->addWidget(new QLabel(tr("EXP :"), ret), 1, 2);
	statEditL->addWidget(expEdit, 1, 3);
	statEditL->addWidget(new QLabel(tr("Tués :"), ret), 2, 0);
	statEditL->addWidget(killsEdit, 2, 1);
	statEditL->addWidget(new QLabel(tr("Mort :"), ret), 2, 2);
	statEditL->addWidget(KOsEdit, 2, 3);
	statEditL->addWidget(inconnu1_labelE, 3, 0);
	statEditL->addWidget(unknown1E, 3, 1);
	statEditL->setColumnStretch(1, 1);
	statEditL->setColumnStretch(3, 1);
	statEditL->setContentsMargins(QMargins());

	QFont font;
	font.setPointSize(10);

	liste = new QTreeWidget(ret);
	liste2 = new QTreeWidget(ret);
	liste->setFont(font);
	liste2->setFont(font);
	liste->setHeaderLabels(QStringList() << QString() << tr("Capacités") << tr("PDC"));
	liste2->setHeaderLabels(QStringList() << QString() << tr("Capacités oubliées"));
	liste->setIndentation(0);
	liste2->setIndentation(0);
	liste->setColumnHidden(0, true);
	liste2->setColumnHidden(0, true);
	liste->header()->setStretchLastSection(false);
	liste2->setFixedWidth(150);
	liste2->resizeColumnToContents(1);
	liste->setUniformRowHeights(true);
	liste2->setUniformRowHeights(true);

	addC = new QPushButton(tr("Ajouter"), ret);
	addC->setIcon(QIcon(":/images/plus.png"));
	connect(addC, SIGNAL(released()), SLOT(add_C()));

	removeC = new QPushButton(tr("Oublier"), ret);
	removeC->setIcon(QIcon(":/images/minus.png"));
	connect(removeC, SIGNAL(released()), SLOT(remove_C()));

	editC = new QPushButton(tr("Modifier"), ret);
	editC->setIcon(QIcon(":/images/edit.png"));
	connect(editC, SIGNAL(released()), SLOT(edit_C()));

	learnC = new QPushButton(tr("Apprendre"), ret);
	learnC->setIcon(QIcon(":/images/icons/learning.png"));
	connect(learnC, SIGNAL(released()), SLOT(changeLearning()));

	acquireAllC = new QPushButton(tr("Tout acquérir"), ret);
	connect(acquireAllC, SIGNAL(released()), SLOT(acquireAll()));

	connect(liste, SIGNAL(itemSelectionChanged()), SLOT(enableButtons()));
	connect(liste, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), SLOT(edit_C(QTreeWidgetItem*)));

	QVBoxLayout *buttonC = new QVBoxLayout;
	buttonC->addWidget(addC);
	buttonC->addWidget(removeC);
	buttonC->addWidget(editC);
	buttonC->addWidget(learnC);
	buttonC->addStretch();
	buttonC->addWidget(acquireAllC);

	restoreF = new QPushButton(tr("Restaurer"), ret);
	restoreF->setIcon(QIcon(":/images/restore.png"));
	connect(restoreF, SIGNAL(released()), SLOT(restore_C()));

	connect(liste2, SIGNAL(itemSelectionChanged()), SLOT(enableButtons2()));
	connect(liste2, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), SLOT(restore_C()));

	QGridLayout *grid = new QGridLayout(ret);
	grid->addWidget(existsE, 0, 0, 1, 4);
	grid->addLayout(statEditL, 2, 0, 1, 4);
	grid->addWidget(liste, 3, 0);
	grid->addLayout(buttonC, 3, 1);
	grid->addWidget(liste2, 3, 2);
	grid->addWidget(restoreF, 3, 3, Qt::AlignTop);
	QMargins margins = grid->contentsMargins();
	margins.setTop(0);
	grid->setContentsMargins(margins);

	return ret;
}

QWidget *GfEditor::buildPage2()
{
	QWidget *ret = new QWidget(this);

	QGroupBox *groupBox = new QGroupBox(tr("Divers"), ret);

	grieverE = new QLineEdit;
	odinE = new QCheckBox(tr("Odin"), groupBox);
	gilgameshE = new QCheckBox(tr("Gilgamesh"), groupBox);
	phoenixE = new QCheckBox(tr("Phénix"), groupBox);

	QHBoxLayout *grieverLayout = new QHBoxLayout;
	grieverLayout->addWidget(new QLabel(Data::names().at(GRIEVER)+tr(" :")));
	grieverLayout->addWidget(grieverE);

	QGridLayout *grid = new QGridLayout(groupBox);
	grid->addLayout(grieverLayout, 0, 0, 1, 3);
	grid->addWidget(odinE, 1, 0);
	grid->addWidget(gilgameshE, 1, 1);
	grid->addWidget(phoenixE, 1, 2);
	grid->setRowStretch(2, 1);

	QVBoxLayout *l = new QVBoxLayout(ret);
	l->addWidget(groupBox);
	l->setContentsMargins(QMargins());

	return ret;
}

void GfEditor::fillPage()
{
	savePage();
	this->id = quint8(gfListe->currentRow());

	for(quint8 i=0 ; i<16 ; ++i)
	{
		gfListe->item(i)->setIcon(gfIcons.at(i).pixmap(32, 48, data->gfs[i].exists ? QIcon::Normal : QIcon::Disabled));
	}

	if(id==16)
	{
		stackedWidget->setCurrentIndex(1);

		grieverE->setText(saveData->perso(GRIEVER));
		odinE->setChecked((data->misc2.dream >> 1) & 1);
		phoenixE->setChecked((data->misc2.dream >> 2) & 1);
		gilgameshE->setChecked((data->misc2.dream >> 3) & 1);
	}
	else if(id < 16)
	{
		stackedWidget->setCurrentIndex(0);

		this->gf_data = &data->gfs[id];

		switch(id) {
		case 15:// Orbital
			this->nivSup = 1000;
			break;
		case 3:case 6:case 9:// Siren, Carbuncle, Cerberus
			this->nivSup = 400;
			break;
		default:
			this->nivSup = 500;
			break;
		}

		existsE->setChecked(gf_data->exists & 1);
		nameEdit->setText(saveData->gf(id));
		hpEdit->setValue(gf_data->HPs);

		expEdit->setValue(gf_data->exp);
		changeExp(gf_data->exp);

		killsEdit->setValue(gf_data->kills);
		KOsEdit->setValue(gf_data->KOs);

		unknown1E->setValue(gf_data->u1);

		quint8 abilityID = 0;
		QList<quint8> capacitesEnPlace;
		liste->clear();
		liste2->clear();
		QMap<int, QIcon> icons = abilityIcons();

		//Listage des capacités aquises
		for(abilityID=0 ; abilityID<116 ; ++abilityID)
		{
			if(getCompleteAbility(abilityID))
			{
				addItem(abilityID, 1, 0, icons);

				capacitesEnPlace.append(abilityID);
			}
		}

		enableButtons();

		quint32 forgotten = GF_GET_FORGOTTEN(*gf_data);

		//Listage des capacités en apprentissage et des capacités oubliées
		for(quint8 i=0 ; i<22 ; ++i)
		{
			abilityID = Data::innateAbilities[id][i];

			if(!capacitesEnPlace.contains(abilityID))//Si pas déjà listé précédemment dans les capacités aquises
			{
				if(!((forgotten >> i) & 1))//En apprentissage
				{
					addItem(abilityID, 0, i, icons);
				}
				else //Oubliées
				{
					addItem(abilityID, 2, i, icons);
				}
			}
		}

		enableButtons2();

		liste->sortByColumn(0, Qt::AscendingOrder);
		liste2->sortByColumn(0, Qt::AscendingOrder);
		liste->header()->setSectionResizeMode(1, QHeaderView::Stretch);
		liste->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
	}
}

void GfEditor::savePage()
{
	if(!loaded)	return;

	if(id==16) {
		saveData->setPerso(GRIEVER, grieverE->text());
		data->misc2.dream = (odinE->isChecked() << 1) | (phoenixE->isChecked() << 2) | (gilgameshE->isChecked() << 3) | (data->misc2.dream & 0xF1);
	}
	else if(id < 16) {
		gf_data->exists = (quint8)existsE->isChecked();
		saveData->setGf(id, nameEdit->text());
		gf_data->exp = expEdit->value();
		gf_data->HPs = hpEdit->value();
		gf_data->kills = killsEdit->value();
		gf_data->KOs = KOsEdit->value();
		gf_data->u1 = unknown1E->value();
	}
}

QTreeWidgetItem *GfEditor::addItem(quint8 abilityID, quint8 type, quint8 innateAbID, const QMap<int, QIcon> &abilityIcons)
{
	QTreeWidgetItem *item;
	QIcon icon;
	if(abilityIcons.isEmpty()) {
		icon = QIcon(QString(":/images/icons/capacity%1.png").arg(Data::abilityType(abilityID)));
	} else {
		icon = abilityIcons.value(Data::abilityType(abilityID));
	}
	
	switch(type)
	{
	case 0://Capacités de la GF non acquises (vert=Apprentissage en cours, noir sinon)
		item = new QTreeWidgetItem(liste, QStringList() << QString("%1").arg(abilityID,3) << Data::abilities().value(abilityID) << QString("%1/%2").arg(gf_data->APs[innateAbID]).arg(Data::apsTab[abilityID]));
		if(gf_data->learning == abilityID)
		{
			item->setForeground(1, Qt::darkGreen);
			item->setForeground(2, Qt::darkGreen);
			item->setIcon(1, QIcon(":/images/icons/learning.png"));
		}
		else
		{
			item->setIcon(1, icon);
		}
		break;
		
	case 1://Capacités acquises de la GF (noir) et capacités acquises en plus (gris)
		item = new QTreeWidgetItem(liste, QStringList() << QString("%1").arg(abilityID,3) << Data::abilities().value(abilityID) << tr("Acquis!"));
		if(posAbility(abilityID) == -1)
		{
			item->setForeground(1, Qt::darkGray);
			item->setForeground(2, Qt::darkGray);
			item->setIcon(1, icon);
		}
		else if(gf_data->learning == abilityID)
		{
			item->setForeground(1, Qt::darkGreen);
			item->setForeground(2, Qt::darkGreen);
			item->setIcon(1, QIcon(":/images/icons/learning.png"));
		}
		else
		{
			item->setIcon(1, icon);
		}
		break;

	default://Capacités oubliées de la GF (rouge)
		item = new QTreeWidgetItem(liste2, QStringList() << QString("%1").arg(abilityID,3) << Data::abilities().value(abilityID));
		item->setForeground(1, Qt::red);
		item->setIcon(1, icon);
		break;
	}
	
	return item;
}

void GfEditor::remove_C()
{
	QList<QTreeWidgetItem *> selectedItems = liste->selectedItems();
	if(selectedItems.isEmpty())	return;
	quint8 abilityID = selectedItems.first()->text(0).toInt();

	delete selectedItems.first();
	
	if(abilityID == gf_data->learning)
	{
		gf_data->learning = 0;
	}
	
	quint32 forgotten = GF_GET_FORGOTTEN(*gf_data);
	qint8 pos;
	if((pos = posAbility(abilityID)) != -1)
	{
		QTreeWidgetItem *item = addItem(abilityID, 2, pos);
		forgotten |= 1 << pos;
		liste2->sortByColumn(0, Qt::AscendingOrder);
		liste2->scrollToItem(item);
		liste2->setCurrentItem(item);
	}
	GF_SET_FORGOTTEN(*gf_data, forgotten & 0xFFFFFF);
	setCompleteAbility(abilityID, false);
}

void GfEditor::add_C()
{
	QDialog dialog(this);
	dialog.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	
	QGridLayout *grid = new QGridLayout(&dialog);
	
	selection = new QComboBox(&dialog);
	fillAbilities(selection, abilityIcons());
	
	okC = new QPushButton(tr("Ajouter"), &dialog);
	okC->setEnabled(posAbility(1) == -1);
	okC->setIcon(QIcon(":/images/plus.png"));
	
	grid->addWidget(selection, 0, 0);
	grid->addWidget(okC, 1, 0, Qt::AlignHCenter);
	
	connect(okC, SIGNAL(released()), SLOT(addCapacity()));
	connect(selection, SIGNAL(currentIndexChanged(int)), SLOT(changeCapacity(int)));
	connect(okC, SIGNAL(released()), &dialog, SLOT(close()));
	
	dialog.exec();
}

void GfEditor::edit_C(QTreeWidgetItem *item)
{
	if(posAbility(item->text(0).toInt()) != -1)
		edit_C();
}

void GfEditor::edit_C()
{
	QList<QTreeWidgetItem *> selectedItems = liste->selectedItems();
	if(selectedItems.isEmpty())	return;

	quint8 abilityID = selectedItems.first()->text(0).toInt();
	
	dialog = new QDialog(this, Qt::Dialog | Qt::WindowCloseButtonHint);
	dialog->setWindowTitle(tr("Modifier PDC"));

	APsEdit = new SpinBox8(dialog);

	qint8 pos = posAbility(abilityID);
	if(pos != -1) {
		APsEdit->setValue(gf_data->APs[pos]);
	}
	
	QLabel *APsMax = new QLabel(QString("/%1").arg(Data::apsTab[abilityID]), dialog);
	abilityCompleted = new QCheckBox(tr("Acquis!"), dialog);
	abilityCompleted->setChecked(getCompleteAbility(abilityID));
	okC = new QPushButton(tr("Fermer"), dialog);
	QPushButton *fullButton = new QPushButton(QString("%1/%1").arg(Data::apsTab[abilityID]), dialog);
	
	QGridLayout *grid = new QGridLayout(dialog);
	grid->addWidget(APsEdit, 0, 0);
	grid->addWidget(APsMax, 0, 1);
	grid->addWidget(abilityCompleted, 1, 0, 1, 2);
	grid->addWidget(okC, 2, 0);
	grid->addWidget(fullButton, 2, 1);

	connect(okC, SIGNAL(released()), SLOT(changeAPs()));
	connect(fullButton, SIGNAL(released()), SLOT(fullAPs()));
	
	dialog->exec();
}

void GfEditor::restore_C()
{
	QList<QTreeWidgetItem *> selectedItems = liste2->selectedItems();
	if(selectedItems.isEmpty())	return;

	quint8 abilityID = selectedItems.first()->text(0).toInt();
	delete selectedItems.first();
	qint8 pos = posAbility(abilityID);
	
	QTreeWidgetItem *item;
	
	if(gf_data->APs[pos] < Data::apsTab[abilityID])
	{
		setCompleteAbility(abilityID, false);
		item = addItem(abilityID, 0, pos);
	}
	else
	{
		setCompleteAbility(abilityID, true);
		item = addItem(abilityID, 1);
	}
	
	liste->sortByColumn(0, Qt::AscendingOrder);
	liste->scrollToItem(item);
	liste->setCurrentItem(item);
	liste->header()->setSectionResizeMode(1, QHeaderView::Stretch);
	liste->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

	quint32 forgotten = GF_GET_FORGOTTEN(*gf_data);
	forgotten ^= 1 << pos;
	GF_SET_FORGOTTEN(*gf_data, forgotten);
}

void GfEditor::enableButtons()
{
	QList<QTreeWidgetItem *> selectedItems = liste->selectedItems();
	if(selectedItems.isEmpty())
	{
		removeC->setEnabled(false);
		learnC->setEnabled(false);
		editC->setEnabled(false);
		return;
	}
	removeC->setEnabled(true);
	learnC->setEnabled(true);
	editC->setEnabled(posAbility(selectedItems.first()->text(0).toInt()) != -1);

	removeC->setText(posAbility(selectedItems.first()->text(0).toInt()) == -1 ? tr("Supprimer") : tr("Oublier"));
}

void GfEditor::enableButtons2()
{
	restoreF->setEnabled(!liste2->selectedItems().isEmpty());
}

void GfEditor::changeExp(double value)
{
	int niv = ((quint32)value/nivSup)+1;
	if(niv>100)	niv = 100;
	if(nivEdit->value() != niv)
	{
		nivEdit->setValue(niv);
	}
}

void GfEditor::changeNiv(int value)
{
	quint32 exp = (value-1)*nivSup;
	if(exp > expEdit->value() || exp + nivSup <= expEdit->value())
	{
		expEdit->setValue(exp);
	}
}

void GfEditor::changeExists(bool exists)
{
	if(gfListe->currentRow() < 16)
		gfListe->currentItem()->setIcon(gfIcons.at(gfListe->currentRow()).pixmap(32, 48, exists ? QIcon::Normal : QIcon::Disabled));
}

void GfEditor::changeCapacity(int index)
{
	okC->setEnabled(posAbility(index+1) == -1);
}

void GfEditor::addCapacity()
{
	quint8 abilityID = selection->currentIndex()+1;
	if(!getCompleteAbility(abilityID))
	{
		QTreeWidgetItem *item = addItem(abilityID, 1);
		liste->sortByColumn(0, Qt::AscendingOrder);
		
		setCompleteAbility(abilityID, true);
		liste->scrollToItem(item);
		liste->setCurrentItem(item);
		liste->header()->setSectionResizeMode(1, QHeaderView::Stretch);
		liste->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
	}
}

void GfEditor::changeAPs()
{
	QList<QTreeWidgetItem *> selectedItems = liste->selectedItems();
	if(selectedItems.isEmpty())	return;
	
	quint8 abilityID = selectedItems.first()->text(0).toInt();
	qint8 pos;
	if((pos = posAbility(abilityID)) != -1)
	{
		int value = APsEdit->value();
		gf_data->APs[pos] = (char)value;
		if(!abilityCompleted->isChecked())
		{
			setCompleteAbility(abilityID, false);
			selectedItems.first()->setText(2, QString("%1/%2").arg(value).arg(Data::apsTab[abilityID]));
		}
		else
		{
			setCompleteAbility(abilityID, true);
			selectedItems.first()->setText(2, tr("Acquis!"));
		}
	}
	dialog->close();
}

void GfEditor::fullAPs()
{
	QList<QTreeWidgetItem *> selectedItems = liste->selectedItems();
	if(selectedItems.isEmpty())	return;
	
	APsEdit->setValue(Data::apsTab[selectedItems.first()->text(0).toInt()]);
}

void GfEditor::changeLearning()
{
	QList<QTreeWidgetItem *> selectedItems = liste->selectedItems();
	if(selectedItems.isEmpty())	return;
	
	QList<QTreeWidgetItem *> findItems = liste->findItems(QString("%1").arg(gf_data->learning,3), Qt::MatchExactly);
	if(!findItems.isEmpty())
	{
		quint8 abilityID = findItems.first()->text(0).toInt();
		findItems.first()->setIcon(1, QIcon(QString(":/images/icons/capacity%1.png").arg(Data::abilityType(abilityID))));
		Qt::GlobalColor color = posAbility(abilityID) != -1 ? Qt::black : Qt::darkGray;
		findItems.first()->setForeground(1, QColor(color));
		findItems.first()->setForeground(2, QColor(color));
	}
	
	if(gf_data->learning != selectedItems.first()->text(0).toInt())
	{
		gf_data->learning = selectedItems.first()->text(0).toInt();
		selectedItems.first()->setIcon(1, QIcon(":/images/icons/learning.png"));
		Qt::GlobalColor color = posAbility(gf_data->learning) != -1 ? Qt::darkGreen : Qt::darkGray;
		selectedItems.first()->setForeground(1, QColor(color));
		selectedItems.first()->setForeground(2, QColor(color));
		liste->scrollToItem(selectedItems.first());
	}
	else
	{
		gf_data->learning = 0;
	}
}

void GfEditor::acquireAll()
{
	int itemCount = liste->topLevelItemCount();

	for(int i=0 ; i<itemCount ; ++i) {
		QTreeWidgetItem *item = liste->topLevelItem(i);
		quint8 abilityID = item->text(0).toInt();
		setCompleteAbility(abilityID, true);
		item->setText(2, tr("Acquis!"));
		// optionnal
		qint8 pos;
		if((pos = posAbility(abilityID)) != -1)
			gf_data->APs[pos] = Data::apsTab[abilityID];
	}
}

qint8 GfEditor::posAbility(quint8 abilityID)
{
	for(quint8 i=0 ; i<22 ; ++i)
	{
		if(Data::innateAbilities[id][i] == abilityID)
			return i;
	}
	return -1;
}

void GfEditor::setCompleteAbility(quint8 pos, bool value)
{
	gf_data->completeAbilities[pos/8] = value ? ( gf_data->completeAbilities[pos/8] | (1 << (pos%8)) ) : ( gf_data->completeAbilities[pos/8] & ~(1 << (pos%8)) );
}

bool GfEditor::getCompleteAbility(quint8 pos)
{
	return ((quint8)gf_data->completeAbilities[pos/8] >> pos%8) & 1;
}
