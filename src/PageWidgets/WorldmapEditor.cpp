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

#include "WorldmapEditor.h"

WorldmapEditor::WorldmapEditor(QWidget *parent)
	: PageWidget(parent)
{
}

void WorldmapEditor::buildWidget()
{
//	QTabWidget *tabWidget = new QTabWidget(this);
//	tabWidget->addTab(buildPage1(), tr("Divers"));
//	tabWidget->addTab(buildPage2(), tr("Positions"));

	QVBoxLayout *layout = new QVBoxLayout(this);
//	layout->addWidget(tabWidget);
	layout->addWidget(buildPage1());
	layout->setContentsMargins(QMargins());

	connect(fix, SIGNAL(toggled(bool)), SLOT(fillPage()));
}

QWidget *WorldmapEditor::buildPage1()
{
	QWidget *ret = new QWidget(this);

	stepsE = new SpinBox16(ret);
	carStepsE = new SpinBox16(ret);
	currentCarE = new SpinBox8(ret);
	currentMapE = new QComboBox(ret);
	currentMapE->addItem(tr("Aucune"), 0);
	currentMapE->addItem(tr("Globe"), 1);
	currentMapE->addItem(tr("Carte"), 2);

	QGroupBox *controlGroupE = new QGroupBox(tr("Véhicule conduit au moins une fois"), ret);
	QGridLayout *controlL = new QGridLayout(controlGroupE);

	QStringList texts;
	texts << tr("Voiture") << tr("???") << tr("BGU") << tr("Chocobo") << tr("Hydre") << tr("???") << tr("???") << tr("???");
	for (int i = 0; i < 8; ++i)
	{
		QCheckBox *controlCheckBox = new QCheckBox(texts.at(i), controlGroupE);
		controlE.append(controlCheckBox);
		controlL->addWidget(controlCheckBox, i/4, i%4);
	}

	QGroupBox *koyokGroupE = new QGroupBox(tr("Quête Koyo-K"), ret);
	QGridLayout *koyokL = new QGridLayout(koyokGroupE);

	texts.clear();
	texts << tr("???") << tr("???") << tr("Mandy Beach") << tr("Winhill") << tr("Trabia") << tr("Désert Kashkabald") << tr("UFO battu") << tr("Koyo-K battu");
	for (int i = 0; i < 8; ++i)
	{
		QCheckBox *koyokCheckBox = new QCheckBox(texts.at(i), koyokGroupE);
		koyokE.append(koyokCheckBox);
		koyokL->addWidget(koyokCheckBox, i/4, i%4);
	}

	QGroupBox *obelGroupE = new QGroupBox(tr("Quête du lac Obel"), ret);
	QGridLayout *obelL = new QGridLayout(obelGroupE);

	texts.clear();
	texts << tr("Avoir parlé à l'ombre une 1ère fois") << tr("Avoir fredonné un air 2 fois") << tr("Accepter de chercher Ryo") << tr("Avoir vu Ryo") << tr("Ne pas avoir dit à l'ombre où se trouve Ryo") << tr("L'ombre a donné indice Balamb") << tr("Pierre plage Balamb") << tr("Ryo vous dit qu'il est plus fort que vous") << tr("Vous avez fait des ricochets") << tr("Vous avez fait un nombre incalculable de ricochets") << tr("Pierre Ryo") << tr("Pierre montagne Galbadia") << tr("Pierre île Timber") << tr("Toutes les pierres") << tr("L'ombre a donné indice Eldbeak") << tr("Pilier vu à Eldbeak") << tr("Trésor île Minde trouvé") << tr("Trésor Mordor trouvé");
	for (int i = 0; i < texts.size(); ++i)
	{
		QCheckBox *obelCheckBox = new QCheckBox(texts.at(i), obelGroupE);
		obelE.append(obelCheckBox);
		obelL->addWidget(obelCheckBox, i/3, i%3);
	}

//	blockLunaticE = new QCheckBox(tr("Bloquer l'accès au Lunatic Pandora"), ret);

	fixGroup = new QWidget(this);
	QRadioButton *normal;
	fixWarnIcon = new QLabel(fixGroup);
	fixWarnIcon->setPixmap(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning).pixmap(16));
	fixLabel = new QLabel(tr("Version boguée détectée, il est recommandé de changer de mode :"), fixGroup);
	normal = new QRadioButton(tr("Version normale"), fixGroup);
	fix = new QRadioButton(tr("Version française PC boguée"), fixGroup);

	QHBoxLayout *bugLayout = new QHBoxLayout(fixGroup);
	bugLayout->addWidget(fixWarnIcon);
	bugLayout->addWidget(fixLabel);
	bugLayout->addStretch();
	bugLayout->addWidget(normal, 0, Qt::AlignBottom | Qt::AlignRight);
	bugLayout->addWidget(fix, 0, Qt::AlignBottom | Qt::AlignRight);
	bugLayout->setContentsMargins(QMargins());

	normal->setChecked(true);

	QGridLayout *layout = new QGridLayout(ret);
	layout->addWidget(new QLabel(tr("Lié aux pas"), ret), 0, 0);
	layout->addWidget(stepsE, 0, 1);
	layout->addWidget(new QLabel(tr("Lié au kmtrage"), ret), 0, 2);
	layout->addWidget(carStepsE, 0, 3);
	layout->addWidget(new QLabel(tr("Voiture de location"), ret), 1, 0);
	layout->addWidget(currentCarE, 1, 1);
	layout->addWidget(new QLabel(tr("Map affichée"), ret), 1, 2);
	layout->addWidget(currentMapE, 1, 3);
	layout->addWidget(controlGroupE, 2, 0, 1, 4);
	layout->addWidget(koyokGroupE, 3, 0, 1, 4);
	layout->addWidget(obelGroupE, 4, 0, 1, 4);
//	layout->addWidget(blockLunaticE, 5, 0, 1, 4);
	layout->setRowStretch(5, 1);
	layout->addWidget(fixGroup, 6, 0, 1, 4);
	layout->setContentsMargins(QMargins());

	return ret;
}

QWidget *WorldmapEditor::buildPage2()
{
	QWidget *ret = new QWidget(this);

	/*QFont font;
	font.setPointSize(10);

	vehiclesList = new QListWidget(ret);
	vehiclesList->setFont(font);
	vehiclesList->addItem(tr("Personnage"));
	vehiclesList->addItem(tr("Inconnu 1"));
	vehiclesList->addItem(tr("Hydre"));
	vehiclesList->addItem(tr("BGU"));
	vehiclesList->addItem(tr("Voiture"));
	vehiclesList->addItem(tr("Inconnu 2"));
	vehiclesList->addItem(tr("Inconnu 3"));
	vehiclesList->addItem(tr("Inconnu 4"));
	vehiclesList->setFixedWidth(90);
	vehiclesList->setCurrentRow(0);

	QSpinBox *spinBox;

	spinBox = new QSpinBox(ret);
	spinBox->setRange(-32768, 32767);
	spinBox->setPrefix(tr("X="));
	coord.append(spinBox);

	spinBox = new QSpinBox(ret);
	spinBox->setRange(-32768, 32767);
	spinBox->setPrefix(tr("Z="));
	coord.append(spinBox);

	spinBox = new QSpinBox(ret);
	spinBox->setRange(-32768, 32767);
	spinBox->setPrefix(tr("Y="));
	coord.append(spinBox);

	spinBox = new QSpinBox(ret);
	spinBox->setRange(-32768, 32767);
	spinBox->setPrefix(tr("?="));
	coord.append(spinBox);

	spinBox = new QSpinBox(ret);
	spinBox->setRange(-32768, 32767);
	spinBox->setPrefix(tr("?="));
	coord.append(spinBox);

	spinBox = new QSpinBox(ret);
	spinBox->setRange(-32768, 32767);
	spinBox->setPrefix(tr("Rot="));
	coord.append(spinBox);

	MapWidget *wm = new MapWidget();

	QGridLayout *layout = new QGridLayout(ret);
	layout->addWidget(vehiclesList, 0, 0, 3, 1, Qt::AlignLeft);
	layout->addWidget(coord.at(0), 0, 1);
	layout->addWidget(coord.at(1), 0, 2);
	layout->addWidget(coord.at(2), 0, 3);
	layout->addWidget(coord.at(3), 1, 1);
	layout->addWidget(coord.at(4), 1, 2);
	layout->addWidget(coord.at(5), 1, 3);
	layout->addWidget(wm, 2, 1, 1, 3);

	connect(vehiclesList, SIGNAL(currentRowChanged(int)), SLOT(fillPositions()));*/

	return ret;
}

void WorldmapEditor::fillPage()
{
	fixGroup->setVisible(pc);

	/* Page 1 */
	stepsE->setValue(data->worldmap.steps_related);
	currentCarE->setValue(data->worldmap.car_rent);

	bool bugMaybe = !fix->isChecked() && (data->worldmap.disp_map_config > 2 || (data->worldmap.vehicles_instructions_worldmap >> 1) & 1) && data->worldmap.car_steps_related==0 && data->worldmap.obel_quest[0]==0 && data->worldmap.obel_quest[1]==0;

	fixWarnIcon->setVisible(bugMaybe);
	fixLabel->setVisible(bugMaybe);

	quint32 car_steps_related;
	quint8 disp_map_config, v_instr, koyok_quest;
	quint8 *obel_quest;
	WORLDMAP_PC wmFix;

	if (!fix->isChecked())
	{
		disp_map_config = data->worldmap.disp_map_config;
		car_steps_related = data->worldmap.car_steps_related;
		v_instr = data->worldmap.vehicles_instructions_worldmap;
		koyok_quest = data->worldmap.koyok_quest;
		obel_quest = data->worldmap.obel_quest;
	}
	else
	{
		memcpy(&wmFix, &data->worldmap.disp_map_config, sizeof(WORLDMAP_PC));

		disp_map_config = wmFix.disp_map_config;
		car_steps_related = wmFix.car_steps_related;
		v_instr = wmFix.vehicles_instructions_worldmap;
		koyok_quest = wmFix.koyok_quest;
		obel_quest = wmFix.obel_quest;
	}

	carStepsE->setValue(car_steps_related);

	setCurrentIndex(currentMapE, disp_map_config);

	for (int i = 0; i < 8; ++i) {
		controlE.at(i)->setChecked((v_instr >> i) & 1);
	}

	for (int i = 0; i < 8; ++i) {
		koyokE.at(i)->setChecked((koyok_quest >> i) & 1);
	}

	obelE.at(0)->setChecked(obel_quest[6] & 1);
	obelE.at(1)->setChecked(obel_quest[0] & 1);
	obelE.at(2)->setChecked((obel_quest[6] >> 1) & 1);
	obelE.at(3)->setChecked((obel_quest[6] >> 2) & 1);
	obelE.at(4)->setChecked((obel_quest[0] >> 6) & 1);
	obelE.at(5)->setChecked((obel_quest[3] >> 6) & 1);
	obelE.at(6)->setChecked((obel_quest[3] >> 1) & 1);
	obelE.at(7)->setChecked((obel_quest[0] >> 7) & 1);
	obelE.at(8)->setChecked((obel_quest[0] >> 4) & 1);
	obelE.at(9)->setChecked((obel_quest[0] >> 5) & 1);
	obelE.at(10)->setChecked((obel_quest[3] >> 2) & 1);
	obelE.at(11)->setChecked((obel_quest[3] >> 3) & 1);
	obelE.at(12)->setChecked((obel_quest[3] >> 4) & 1);
	obelE.at(13)->setChecked((obel_quest[3] >> 5) & 1);
	obelE.at(14)->setChecked((obel_quest[1] >> 6) & 1);
	obelE.at(15)->setChecked((obel_quest[1] >> 7) & 1);
	obelE.at(16)->setChecked(obel_quest[2] & 1);
	obelE.at(17)->setChecked((obel_quest[2] >> 1) & 1);

//	blockLunaticE->setChecked(((obel_quest[5] >> 4) & 1) || ((obel_quest[5] >> 6) & 1));

	/* Page 2 */
//	fillPositions();
}

void WorldmapEditor::fillPositions()
{
	qint16 *tab = *(&data->worldmap.char_pos + vehiclesList->currentRow());

	for (int i = 0; i < 6; ++i) {
		coord.at(i)->setValue(tab[i]);
	}
}

void WorldmapEditor::savePage()
{
	/* Page 1 */
	data->worldmap.steps_related = stepsE->value();
	data->worldmap.car_rent = currentCarE->value();

	quint32 car_steps_related = carStepsE->value();
	quint8 disp_map_config = currentMapE->itemData(currentMapE->currentIndex()).toInt();

	quint8 v_instr = 0;
	for (int i = 0; i < 8; ++i) {
		v_instr |= controlE.at(i)->isChecked() << i;
	}

	quint8 koyok_quest = 0;
	for (int i = 0; i < 8; ++i) {
		koyok_quest |= koyokE.at(i)->isChecked() << i;
	}

	quint8 obel_quest[8];

	// 11110001
	obel_quest[0] = (obelE.at(1)->isChecked() << 0)
			| (obelE.at(8)->isChecked() << 4)
			| (obelE.at(9)->isChecked() << 5)
			| (obelE.at(4)->isChecked() << 6)
			| (obelE.at(7)->isChecked() << 7);
	// 11000000
	obel_quest[1] = (obelE.at(14)->isChecked() << 6)
			| (obelE.at(15)->isChecked() << 7);
	// 00000011
	obel_quest[2] = (obelE.at(16)->isChecked() << 0)
			| (obelE.at(17)->isChecked() << 1);
	// 01111110
	obel_quest[3] = (obelE.at(6)->isChecked() << 1)
			| (obelE.at(10)->isChecked() << 2)
			| (obelE.at(11)->isChecked() << 3)
			| (obelE.at(12)->isChecked() << 4)
			| (obelE.at(13)->isChecked() << 5)
			| (obelE.at(5)->isChecked() << 6);
	// 00000111
	obel_quest[6] = (obelE.at(0)->isChecked() << 0)
			| (obelE.at(2)->isChecked() << 1)
			| (obelE.at(3)->isChecked() << 2);

	if (!fix->isChecked())
	{
		data->worldmap.disp_map_config = disp_map_config;
		data->worldmap.car_steps_related = car_steps_related;
		data->worldmap.vehicles_instructions_worldmap = v_instr;
		data->worldmap.koyok_quest = koyok_quest;
		// 0000 1110
		data->worldmap.obel_quest[0] = (data->worldmap.obel_quest[0] & 0x0E) | obel_quest[0];
		// 0011 1111
		data->worldmap.obel_quest[1] = (data->worldmap.obel_quest[1] & 0x3F) | obel_quest[1];
		// 1111 1100
		data->worldmap.obel_quest[2] = (data->worldmap.obel_quest[2] & 0xFC) | obel_quest[2];
		// 1000 0001
		data->worldmap.obel_quest[3] = (data->worldmap.obel_quest[3] & 0x81) | obel_quest[3];
		// 1010 1111
//		if (!blockLunaticE->isChecked())
//			data->worldmap.obel_quest[5] &= 0xAF;
		// 1111 1000
		data->worldmap.obel_quest[6] = (data->worldmap.obel_quest[6] & 0xF8) | obel_quest[6];
	}
	else
	{
		WORLDMAP_PC wmFix;
		memcpy(&wmFix, &data->worldmap.disp_map_config, sizeof(WORLDMAP_PC));

		wmFix.disp_map_config = disp_map_config;
		wmFix.car_steps_related = car_steps_related;
		wmFix.vehicles_instructions_worldmap = v_instr;
		wmFix.koyok_quest = koyok_quest;
		wmFix.obel_quest[0] = (wmFix.obel_quest[0] & 0xE) | obel_quest[0];
		wmFix.obel_quest[1] = (wmFix.obel_quest[1] & 0x3F) | obel_quest[1];
		wmFix.obel_quest[2] = (wmFix.obel_quest[2] & 0xFC) | obel_quest[2];
		wmFix.obel_quest[3] = (wmFix.obel_quest[3] & 0x81) | obel_quest[3];
//		if (!blockLunaticE->isChecked())
//			wmFix.obel_quest[5] &= 0xAF;
		wmFix.obel_quest[6] = (wmFix.obel_quest[6] & 0xF8) | obel_quest[6];

		memcpy(&data->worldmap.disp_map_config, &wmFix, sizeof(WORLDMAP_PC));
	}

	/* Page 2 */
//	for (int i = 0; i < 6; ++i) {
//		data->worldmap.char_pos[i] = coord.at(i)->value();
//	}
}
