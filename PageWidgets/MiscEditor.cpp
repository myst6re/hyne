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

#include "MiscEditor.h"

MiscEditor::MiscEditor(QWidget *parent)
	: PageWidget(parent)
{
}

void MiscEditor::updateMode(bool mode)
{
	unknownE->setVisible(mode);
	lastFieldLabel->setVisible(mode);
	lastFieldE->setVisible(mode);
	currentFrameLabel->setVisible(mode);
	currentFrameE->setVisible(mode);
	hpMaxLeaderLabel->setVisible(mode);
	hpMaxLeaderE->setVisible(mode);
}

void MiscEditor::updateTime()
{
	quint32 time = data->misc2.game_time;
	tempsSecE->setMaximum(Config::freq(freq_value)-1);
	tempsSecE->setValue(Config::sec(time, freq_value));
	tempsMinE->setValue(Config::min(time, freq_value));
	tempsHourE->setMaximum(Config::freq(freq_value)==60 ? 1193046 : 1431655);
	tempsHourE->setValue(Config::hour(time, freq_value));
	time = data->misc2.countdown;
	countdownSecE->setMaximum(Config::freq(freq_value)-1);
	countdownSecE->setValue(Config::sec(time, freq_value));
	countdownMinE->setValue(Config::min(time, freq_value));
	countdownHourE->setMaximum(Config::freq(freq_value)==60 ? 1193046 : 1431655);
	countdownHourE->setValue(Config::hour(time, freq_value));
}

void MiscEditor::buildWidget()
{
	QVBoxLayout *layout = new QVBoxLayout(this);

	tabWidget = new QTabWidget(this);
	tabWidget->addTab(buildPage1(), tr("Divers"));
	tabWidget->addTab(buildPage2(), tr("Tutoriel"));

	layout->addWidget(tabWidget);
	layout->setContentsMargins(QMargins());
}

QWidget *MiscEditor::buildPage1()
{
	QWidget *ret = new QWidget(this);

	QGroupBox *statsE = new QGroupBox(tr("Statistiques"), ret);
	
	argentE = new QDoubleSpinBox(statsE);
	argentE->setDecimals(0);
	argentE->setRange(0, MAX_INT32);

	tempsHourE = new QSpinBox(statsE);
	tempsMinE = new QSpinBox(statsE);
	tempsMinE->setMaximum(59);
	tempsSecE = new QSpinBox(statsE);

	QHBoxLayout *tempsL = new QHBoxLayout;
	tempsL->addWidget(tempsHourE);
	tempsL->addWidget(new QLabel(":",statsE));
	tempsL->addWidget(tempsMinE);
	tempsL->addWidget(new QLabel(":",statsE));
	tempsL->addWidget(tempsSecE);

	countdownHourE = new QSpinBox(statsE);
	countdownMinE = new QSpinBox(statsE);
	countdownMinE->setMaximum(59);
	countdownSecE = new QSpinBox(statsE);

	QHBoxLayout *countdownL = new QHBoxLayout;
	countdownL->addWidget(countdownHourE);
	countdownL->addWidget(new QLabel(":",statsE));
	countdownL->addWidget(countdownMinE);
	countdownL->addWidget(new QLabel(":",statsE));
	countdownL->addWidget(countdownSecE);

	QHBoxLayout *argent_tempsL = new QHBoxLayout;
	argent_tempsL->addWidget(new QLabel(tr("Temps :"),statsE));
	argent_tempsL->addLayout(tempsL);
	argent_tempsL->addStretch();
	argent_tempsL->addWidget(new QLabel(tr("Compte à rebours :"),statsE));
	argent_tempsL->addLayout(countdownL);
	
	stepsE = new QDoubleSpinBox(statsE);
	stepsE->setDecimals(0);
	stepsE->setRange(0, MAX_INT32);
	
	seedLvlE = new QSpinBox(statsE);
	seedLvlE->setRange(0, 31);
	connect(seedLvlE, SIGNAL(valueChanged(int)), SLOT(nivS(int)));
	seedExpE = new QSpinBox(statsE);
	seedExpE->setRange(0, MAX_INT16);
	connect(seedExpE, SIGNAL(valueChanged(int)), SLOT(expS(int)));

	testSeedE = new QSpinBox(statsE);
	testSeedE->setRange(0, MAX_INT8);
	
	lagunaGilsE = new QDoubleSpinBox(statsE);
	lagunaGilsE->setDecimals(0);
	lagunaGilsE->setRange(0, MAX_INT32);

	lastFieldE = new QSpinBox(statsE);
	lastFieldE->setRange(0, MAX_INT16);

	currentFrameE = new QDoubleSpinBox(statsE);
	currentFrameE->setDecimals(0);
	currentFrameE->setRange(0, MAX_INT32);

	QGridLayout *statsL = new QGridLayout(statsE);
	statsL->addLayout(argent_tempsL, 0, 0, 1, 6);
	statsL->addWidget(new QLabel(tr("Argent :"),statsE), 1, 0);
	statsL->addWidget(argentE, 1, 1);
	statsL->addWidget(new QLabel(tr("Argent Laguna :"),statsE), 1, 2);
	statsL->addWidget(lagunaGilsE, 1, 3);
	statsL->addWidget(new QLabel(tr("Nombre de pas :"),statsE), 1, 4);
	statsL->addWidget(stepsE, 1, 5);
	statsL->addWidget(new QLabel(tr("Niveau Seed :"),statsE), 2, 0);
	statsL->addWidget(seedLvlE, 2, 1);
	statsL->addWidget(new QLabel(tr("EXP Seed :"),statsE), 2, 2);
	statsL->addWidget(seedExpE, 2, 3);
	statsL->addWidget(new QLabel(tr("Niveau test Seed :"),statsE), 2, 4);
	statsL->addWidget(testSeedE, 2, 5);
	statsL->addWidget(lastFieldLabel = new QLabel(tr("Dernier écran visité :"),statsE), 3, 0);
	statsL->addWidget(lastFieldE, 3, 1);
	statsL->addWidget(currentFrameLabel = new QLabel(tr("Current Movie Frame :"),statsE), 3, 2);
	statsL->addWidget(currentFrameE, 3, 3);

	QGroupBox *headerE = new QGroupBox(tr("En-tête"), ret);
	hpMaxLeaderE = new QSpinBox(headerE);
	hpMaxLeaderE->setRange(0, MAX_INT16);
	saveCountE = new QSpinBox(headerE);
	saveCountE->setRange(0, MAX_INT16);
	curSaveE = new QDoubleSpinBox(headerE);
	curSaveE->setDecimals(0);
	curSaveE->setRange(0, MAX_INT32);
	locationIDE = new QComboBox(headerE);
	int i=0;
	foreach(const QString &loc, Data::locations().list())
		locationIDE->addItem(loc, i++);

	QGridLayout *headerL = new QGridLayout(headerE);
	headerL->addWidget(new QLabel(tr("Compteur :"),headerE), 0, 0);
	headerL->addWidget(saveCountE, 0, 1);
	headerL->addWidget(new QLabel(tr("Curseur :"),headerE), 0, 2);
	headerL->addWidget(curSaveE, 0, 3);
	headerL->addWidget(new QLabel(tr("Lieu :"),headerE), 1, 0);
	headerL->addWidget(locationIDE, 1, 1);
	headerL->addWidget(hpMaxLeaderLabel = new QLabel(tr("HP max leader (inutilisé) :"),headerE), 1, 2);
	headerL->addWidget(hpMaxLeaderE, 1, 3);

	unknownE = new QGroupBox(tr("Inconnu"), ret);
	unknown1E = new QSpinBox(unknownE);
	unknown1E->setRange(0, MAX_INT16);
	unknown2E = new QSpinBox(unknownE);
	unknown2E->setRange(0, MAX_INT16);
	unknown4E = new QDoubleSpinBox(unknownE);
	unknown4E->setDecimals(0);
	unknown4E->setRange(0, MAX_INT32);
	unknown5E = new QSpinBox(unknownE);
	unknown5E->setRange(0, MAX_INT16);
	unknown6E = new QDoubleSpinBox(unknownE);
	unknown6E->setDecimals(0);
	unknown6E->setRange(0, MAX_INT32);
	unknown7E = new QDoubleSpinBox(unknownE);
	unknown7E->setDecimals(0);
	unknown7E->setRange(0, MAX_INT32);
	unknown8E = new QDoubleSpinBox(unknownE);
	unknown8E->setDecimals(0);
	unknown8E->setRange(0, MAX_INT32);
	unknown9E = new QDoubleSpinBox(unknownE);
	unknown9E->setDecimals(0);
	unknown9E->setRange(0, MAX_INT32);

	QGridLayout *unknownL = new QGridLayout(unknownE);
	unknownL->addWidget(new QLabel(tr("Inconnu 1 :"),unknownE), 0, 0);
	unknownL->addWidget(unknown1E, 0, 1);
	unknownL->addWidget(new QLabel(tr("Inconnu 2 :"),unknownE), 0, 2);
	unknownL->addWidget(unknown2E, 0, 3);
	unknownL->addWidget(new QLabel(tr("Inconnu 3 :"),unknownE), 0, 4);
	unknownL->addWidget(unknown4E, 0, 5);
	unknownL->addWidget(new QLabel(tr("Inconnu 4 :"),unknownE), 1, 0);
	unknownL->addWidget(unknown5E, 1, 1);
	unknownL->addWidget(new QLabel(tr("Inconnu 5 :"),unknownE), 1, 2);
	unknownL->addWidget(unknown6E, 1, 3);
	unknownL->addWidget(new QLabel(tr("Inconnu 6 :"),unknownE), 1, 4);
	unknownL->addWidget(unknown7E, 1, 5);
	unknownL->addWidget(new QLabel(tr("Inconnu 7 :"),unknownE), 2, 0);
	unknownL->addWidget(unknown8E, 2, 1);
	unknownL->addWidget(new QLabel(tr("Inconnu 8 :"),unknownE), 2, 2);
	unknownL->addWidget(unknown9E, 2, 3);
	
	QVBoxLayout *layout = new QVBoxLayout(ret);
	layout->addWidget(statsE);
	layout->addWidget(headerE);
	layout->addWidget(unknownE);
	layout->addStretch();

	return ret;
}

QWidget *MiscEditor::buildPage2()
{
	QWidget *ret = new QWidget(this);

	QFont font;
	font.setPointSize(10);

	tutoE_list = new QTreeWidget(ret);
	tutoE_list->setFont(font);
	tutoE_list->setHeaderLabel(tr("Catégorie"));
	tutoE_list->setIndentation(0);
	tutoE_list->setUniformRowHeights(true);

	QStringList tutoStrings;
	tutoStrings
			<< tr("Lieux/Mine de souffre") << tr("Lieux/Dollet") << tr("Lieux/Timber")
			<< tr("Lieux/Chaîne TV Timber") << tr("Lieux/Galbadia") << tr("Lieux/Tombe du roi inconnu")
			<< tr("Lieux/Winhill") << tr("Lieux/Prison du désert") << tr("Lieux/Base des missiles")
			<< tr("Lieux/Aqueduc d'Horizon") << tr("Lieux/Horizon") << tr("Lieux/Trabia")
			<< tr("Lieux/Esthar(1)") << tr("Lieux/Esthar(2)") << tr("Lieux/Esthar(3)")
			<< tr("Lieux/Station balnéaire") << tr("Lieux/Salt Lake") << tr("Lieux/Lune")
			<< tr("Lieux/Labo de Deep Sea") << tr("Lieux/Fouilles de Deep Sea") << tr("Termes/Compression Temporelle")
			<< tr("Termes/Origine de la BGU") << tr("Termes/Ondes hertziennes(1)") << tr("Termes/Ondes hertziennes(2)")
			<< tr("Termes/Larme sélénite") << tr("Termes/Centra") << tr("Termes/Compteur Geyser")
			<< tr("Termes/Mages et sorcières") << tr("Termes/Pouvoirs magiques") << tr("Termes/Monolithe")
			<< tr("Termes/Lunatic Pandora") << tr("Termes/Tears Point") << tr("Termes/Sanctuaire d'Adel")
			<< tr("Termes/L'Hydre") << tr("Termes/Niveau MD") << tr("Termes/Refuge de Centra")
			<< tr("Termes/Timber Maniacs") << tr("Termes/Eyes On Me") << tr("Personnages/Dr. Geyser")
			<< tr("Personnages/Hyne") << tr("Personnages/Seeds Blancs") << tr("Personnages/Moomba")
			<< tr("Personnages/Tribu Shumi") << tr("Personnages/Chocobo") << tr("Personnages/Maire Dobe")
			<< tr("Zell") << tr("Quistis") << tr("Seifer")
			<< tr("Selphie") << tr("Linoa") << tr("Irvine")
			<< tr("Edea") << tr("Laguna") << tr("Kiros")
			<< tr("Ward") << tr("Linoa 2 (Canonisation)") << tr("Inutilisé")
			<< tr("Inutilisé") << tr("Termes/Réincarnation occulte") << tr("Rapport de combat")
			<< tr("Rapport Perso") << tr("Rapport G-Force") << tr("Actions/Association G-Force (inutilisé)")
			<< tr("Actions/Association magique") << tr("Actions/Association élémentale") << tr("Actions/Association mentale")
			<< tr("Actions/Tutorial G-Forces") << tr("Actions/Écran situation Squall") << tr("Actions/Écran situation Zell")
			<< tr("Actions/Écran situation Linoa") << tr("Actions/Intervertir") << tr("Inutilisé")
			<< tr("Minimog") << tr("Rapport G-Force/Golgotha") << tr("Rapport G-Force/Shiva")
			<< tr("Rapport G-Force/Ifrit") << tr("Rapport G-Force/Ondine") << tr("Rapport G-Force/Taurus")
			<< tr("Rapport G-Force/Nosferatu") << tr("Rapport G-Force/Ahuri") << tr("Rapport G-Force/Leviathan")
			<< tr("Rapport G-Force/Zéphyr") << tr("Rapport G-Force/Cerberus") << tr("Rapport G-Force/Alexander")
			<< tr("Rapport G-Force/Helltrain") << tr("Rapport G-Force/Bahamut") << tr("Rapport G-Force/Pampa")
			<< tr("Rapport G-Force/Tomberry") << tr("Rapport G-Force/Orbital") << tr("Créa-Mgi-Cél")
			<< tr("Créa-Mgi-Cél 2") << tr("Créa-Mgi-Gla") << tr("Créa-Mgi-Gla 2")
			<< tr("Créa-Mgi-Inc") << tr("Créa-Mgi-Inc 2") << tr("Créa-Mgi-Méd")
			<< tr("Créa-Mgi-Méd 2") << tr("Créa-Mgi-Temp") << tr("Créa-Mgi-Temp 2")
			<< tr("Créa-Mgi-Mtl") << tr("Créa-Mgi-Mtl 2") << tr("Créa-Mgi-Pro")
			<< tr("Créa-Mgi-Pro 2") << tr("Créa-Mgi-Tab") << tr("Créa-Mgi-Tab 2")
			<< tr("Créa-Mgi-Thér") << tr("Créa-Mgi-Thér 2") << tr("Créa-Mgi-Ana")
			<< tr("Créa-Mgi-Ana 2") << tr("Créa-balles") << tr("Créa-balles 2")
			<< tr("Créa-outils") << tr("Créa-outils 2") << tr("Créa-Thér-Tab")
			<< tr("Créa-Thér-Tab 2") << tr("Créa-Thér-GF") << tr("Créa-Thér-GF 2")
			<< tr("Créa-Capa-GF") << tr("Créa-Capa-GF 2") << tr("Créa-Mgi-Plus")
			<< tr("Créa-Mgi-Plus 2") << tr("Thér-Niv +") << tr("Thér-Niv + 2")
			<< tr("Freud") << tr("Créa-Mgi-Max") << tr("Créa-Mgi-Max 2")
			<< tr("Inutilisé") << tr("Diplôme Minotaure");

	QTreeWidgetItem *item;
	foreach(QString tutoString, tutoStrings) {
		item = new QTreeWidgetItem(QStringList(tutoString));
		item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		tutoE_list->addTopLevelItem(item);
	}

	QCheckBox *tutoCheckAll = new QCheckBox(tr("Sélectionner tout"), ret);
	connect(tutoCheckAll, SIGNAL(toggled(bool)), SLOT(selectAll(bool)));

	font.setPixelSize(10);
	tutoCheckAll->setFont(font);

	QGridLayout *layout = new QGridLayout(ret);
	layout->addWidget(tutoE_list, 0, 0);
	layout->addWidget(tutoCheckAll, 1, 0, Qt::AlignLeft);

	return ret;
}

void MiscEditor::fillPage()
{
	argentE->setValue(data->misc1.gils);
	lagunaGilsE->setValue(data->misc1.dream_gils);
	seedExpE->setValue(data->misc3.seedExp);
	
	stepsE->setValue(data->misc3.steps);

	testSeedE->setValue(data->misc2.testLevel);
	lastFieldE->setValue(data->misc3.last_field_id);
	currentFrameE->setValue(data->misc3.current_frame);

	saveCountE->setValue(descData->saveCount);
	curSaveE->setValue(descData->curSave);
	setCurrentIndex(locationIDE, descData->locationID);
	hpMaxLeaderE->setValue(descData->hpMaxLeader);

	unknown1E->setValue(data->misc1.u1);
	unknown2E->setValue(data->misc1.u2);
	unknown4E->setValue(data->misc2.u1);
	unknown5E->setValue(data->misc2.u2);
	unknown6E->setValue(data->misc2.u5);
	unknown7E->setValue(data->misc3.u7);
	unknown8E->setValue(data->misc3.music_related);
	unknown9E->setValue(data->misc3.u8);

	int i, j;
	for(i=0 ; i<16 ; ++i) {
		for(j=0 ; j<8 ; ++j) {
			tutoE_list->topLevelItem(i*8+j)
					->setCheckState(0, (data->misc2.tutorial_infos[i]>>j)&1 ? Qt::Checked : Qt::Unchecked);
		}
	}
}

void MiscEditor::savePage()
{
	data->misc1.gils = argentE->value();
	data->misc3.gils = argentE->value();
	data->misc1.dream_gils = lagunaGilsE->value();
	data->misc3.dream_gils = lagunaGilsE->value();
	data->misc2.game_time = Config::time(tempsHourE->value(), tempsMinE->value(), tempsSecE->value(), freq_value);
	data->misc2.countdown = Config::time(countdownHourE->value(), countdownMinE->value(), countdownSecE->value(), freq_value);

	data->misc3.seedExp = seedExpE->value();
	
	data->misc3.steps = stepsE->value();

	data->misc2.testLevel = testSeedE->value();
	data->misc3.last_field_id = lastFieldE->value();
	data->misc3.current_frame = currentFrameE->value();

	descData->saveCount = saveCountE->value();
	descData->curSave = curSaveE->value();
	descData->locationID = locationIDE->itemData(locationIDE->currentIndex()).toUInt();
	descData->hpMaxLeader = hpMaxLeaderE->value();

	data->misc1.u1 = unknown1E->value();
	data->misc1.u2 = unknown2E->value();
	data->misc2.u1 = unknown4E->value();
	data->misc2.u2 = unknown5E->value();
	data->misc2.u5 = unknown6E->value();
	data->misc3.u7 = unknown7E->value();
	data->misc3.music_related = unknown8E->value();
	data->misc3.u8 = unknown9E->value();

	int i, j;
	quint8 curTuto_info;
	for(i=0 ; i<16 ; ++i) {
		curTuto_info = 0;
		for(j=0 ; j<8 ; ++j) {
			curTuto_info |= (tutoE_list->topLevelItem(i*8+j)->checkState(0)==Qt::Checked)<<j;
		}
		data->misc2.tutorial_infos[i] = curTuto_info;
	}
}

void MiscEditor::expS(int value)
{
	quint16 niv = value/100;
	if(seedLvlE->value() != niv)
	{
		seedLvlE->setValue(niv);
	}
}

void MiscEditor::nivS(int value)
{
	quint16 exp = value*100;
	if(exp > seedExpE->value() || exp + 100 <= seedExpE->value())
	{
		seedExpE->setValue(exp);
	}
}

void MiscEditor::selectAll(bool selected)
{
	PageWidget::selectAll(tutoE_list, selected);
}
