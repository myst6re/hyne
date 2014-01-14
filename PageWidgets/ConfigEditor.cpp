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

#include "ConfigEditor.h"

ConfigEditor::ConfigEditor(QWidget *parent)
	: PageWidget(parent)
{
}

void ConfigEditor::updateMode(bool mode)
{
	inconnu1E->setVisible(mode);
}

void ConfigEditor::buildWidget()
{
	QGroupBox *sonGroupE = new QGroupBox(tr("Son"), this);
	son1E = new QRadioButton(tr("Stéréo"), sonGroupE);
	son2E = new QRadioButton(tr("Mono"), sonGroupE);
	
	QHBoxLayout *sonL = new QHBoxLayout(sonGroupE);
	sonL->addWidget(son1E);
	sonL->addWidget(son2E);
	sonL->addStretch();
	
	manetteGroupE = new QGroupBox(tr("Touches personnalisées"), this);
	manetteGroupE->setCheckable(true);
	QStringList keyNames;
	keyNames << tr("???") << tr("Fuite") << tr("Fuite/Changer vision") << tr("Écran cible") << tr("Gâchette")
			<< tr("Marcher/Annuler") << tr("Menu") << tr("Parler/Confirmer") << tr("Parler/Jeu de cartes")
			<< tr("Select") << tr("???") << tr("???");
	
	QGridLayout *keysL = new QGridLayout(manetteGroupE);
	SpinBox8 *curSpinBox;
	quint8 i, j;
	for(i=0 ; i<3 ; ++i) {
		for(j=0 ; j<4 ; ++j) {
			curSpinBox = new SpinBox8(manetteGroupE);
			keysE.append(curSpinBox);
			keysL->addWidget(new QLabel(keyNames.at(i*4+j), manetteGroupE), i, j*2);
			keysL->addWidget(curSpinBox, i, j*2+1);
		}
	}
	curSpinBox = new SpinBox8(manetteGroupE);
	keysE.append(curSpinBox);
	keysL->addWidget(new QLabel(tr("Pause"), manetteGroupE), 3, 0);
	keysL->addWidget(curSpinBox, 3, 1);
	
	QPushButton *joystickE = new QPushButton(tr("Inverser"), manetteGroupE);
	joystickLbl = new QLabel(manetteGroupE);
	keysL->addWidget(joystickLbl, 4, 0, 1, 4);
	keysL->addWidget(joystickE, 4, 4);

	QPushButton *defaultButton = new QPushButton(tr("Par défaut"), manetteGroupE);
	keysL->addWidget(defaultButton, 4, 6, 1, 2);
	
	QGroupBox *pointeurGroupE = new QGroupBox(tr("Pointeur"), this);
	pointeur1E = new QRadioButton(tr("Initial"), pointeurGroupE);
	pointeur2E = new QRadioButton(tr("Mémoire"), pointeurGroupE);
	
	QHBoxLayout *pointeurL = new QHBoxLayout(pointeurGroupE);
	pointeurL->addWidget(pointeur1E);
	pointeurL->addWidget(pointeur2E);
	pointeurL->addStretch();
	
	QGroupBox *ATBGroupE = new QGroupBox(tr("Jauge ATB"), this);
	ATB1E = new QRadioButton(tr("Active"), pointeurGroupE);
	ATB2E = new QRadioButton(tr("Attente"), pointeurGroupE);
	
	QHBoxLayout *ATBL = new QHBoxLayout(ATBGroupE);
	ATBL->addWidget(ATB1E);
	ATBL->addWidget(ATB2E);
	ATBL->addStretch();
	
	QGroupBox *scanGroupE = new QGroupBox(tr("Scan"), this);
	scan1E = new QRadioButton(tr("1 fois"), scanGroupE);
	scan2E = new QRadioButton(tr("Toujours"), scanGroupE);
	
	QHBoxLayout *scanL = new QHBoxLayout(scanGroupE);
	scanL->addWidget(scan1E);
	scanL->addWidget(scan2E);
	scanL->addStretch();

	QGroupBox *targetMenuGroupE = new QGroupBox(tr("Menu cible (combats)"), this);
	targetMenu1E = new QRadioButton(tr("Non"), targetMenuGroupE);
	targetMenu2E = new QRadioButton(tr("Oui"), targetMenuGroupE);

	QHBoxLayout *targetMenuL = new QHBoxLayout(targetMenuGroupE);
	targetMenuL->addWidget(targetMenu1E);
	targetMenuL->addWidget(targetMenu2E);
	targetMenuL->addStretch();
	
	cameraE = new QSlider(Qt::Horizontal, this);
	cameraE->setMaximum(4);
	
	vts_combatE = new QSlider(Qt::Horizontal, this);
	vts_combatE->setMaximum(4);
	
	vts_msg_combatE = new QSlider(Qt::Horizontal, this);
	vts_msg_combatE->setMaximum(4);
	
	vts_msgE = new QSlider(Qt::Horizontal, this);
	vts_msgE->setMaximum(4);
	
	analog_volumeE = new QSpinBox(this);
	analog_volumeE->setMaximum(100);
	
	QGroupBox *vibrationGroupE = new QGroupBox(tr("Vibration"), this);
	vibration1E = new QRadioButton(tr("OFF"), vibrationGroupE);
	vibration2E = new QRadioButton(tr("ON"), vibrationGroupE);
	
	QHBoxLayout *vibrationL = new QHBoxLayout(vibrationGroupE);
	vibrationL->addWidget(vibration1E);
	vibrationL->addWidget(vibration2E);
	vibrationL->addStretch();
	
	inconnu1E = new QCheckBox(tr("Inconnu 1"), this);
	
	QHBoxLayout *inconnuL = new QHBoxLayout;
	inconnuL->addWidget(inconnu1E);
	inconnuL->addStretch();
	
	QGridLayout *grid = new QGridLayout(this);
	grid->addWidget(sonGroupE, 0, 0, 1, 2);
	grid->addWidget(pointeurGroupE, 0, 2, 1, 2);
	grid->addWidget(ATBGroupE, 0, 4, 1, 2);
	grid->addWidget(scanGroupE, 1, 0, 1, 2);
	grid->addWidget(targetMenuGroupE, 1, 2, 1, 2);
	grid->addWidget(vibrationGroupE, 1, 4, 1, 2);
	grid->addLayout(inconnuL, 2, 0, 1, 6);
	grid->addWidget(new QLabel(tr("Déplacement caméra :"), this), 3, 0, 1, 2);
	grid->addWidget(cameraE, 3, 2, 1, 4);
	grid->addWidget(new QLabel(tr("Vitesse combat :"), this), 4, 0, 1, 2);
	grid->addWidget(vts_combatE, 4, 2, 1, 4);
	grid->addWidget(new QLabel(tr("Message combat :"), this), 5, 0, 1, 2);
	grid->addWidget(vts_msg_combatE, 5, 2, 1, 4);
	grid->addWidget(new QLabel(tr("Message excursion :"), this), 6, 0, 1, 2);
	grid->addWidget(vts_msgE, 6, 2, 1, 4);
	grid->addWidget(new QLabel(tr("Entrée Analogique/Volume :"), this), 7, 0, 1, 2);
	grid->addWidget(analog_volumeE, 7, 2, 1, 4);
	grid->addWidget(manetteGroupE, 8, 0, 1, 6);
	grid->setRowStretch(9, 1);
	QMargins margins = grid->contentsMargins();
	margins.setTop(0);
	margins.setBottom(0);
	grid->setContentsMargins(margins);

	connect(defaultButton, SIGNAL(released()), SLOT(defaultKeys()));
	connect(joystickE, SIGNAL(released()), SLOT(joystickS()));
}

void ConfigEditor::fillPage()
{
	if(!((data->config.divers >> 1) & 1))
		son1E->setChecked(true);
	else
		son2E->setChecked(true);
	manetteGroupE->setChecked((data->config.divers >> 5) & 1);
	if(!((data->config.divers >> 2) & 1))
		pointeur1E->setChecked(true);
	else
		pointeur2E->setChecked(true);
	if(!(data->config.divers & 1))
		ATB1E->setChecked(true);
	else
		ATB2E->setChecked(true);
	if(!data->config.scan)
		scan1E->setChecked(true);
	else
		scan2E->setChecked(true);
	if(!((data->config.divers >> 4) & 1))
		targetMenu1E->setChecked(true);
	else
		targetMenu2E->setChecked(true);
	if(!((data->config.divers >> 6) & 1))
		vibration1E->setChecked(true);
	else
		vibration2E->setChecked(true);
	cameraE->setValue(4-data->config.camera);
	vts_combatE->setValue(4-data->config.vts_combat);
	vts_msg_combatE->setValue(4-data->config.vts_msg_combat);
	vts_msgE->setValue(4-data->config.vts_msg);
	analog_volumeE->setValue(data->config.analog_volume);

	keysE.first()->setValue(data->config.u3);
	keysE.at(1)->setValue(data->config.L2);
	keysE.at(2)->setValue(data->config.R2);
	keysE.at(3)->setValue(data->config.L1);
	keysE.at(4)->setValue(data->config.R1);
	keysE.at(5)->setValue(data->config.TRIANGLE);
	keysE.at(6)->setValue(data->config.ROND);
	keysE.at(7)->setValue(data->config.CROIX);
	keysE.at(8)->setValue(data->config.CARRE);
	keysE.at(9)->setValue(data->config.SELECT);
	keysE.at(10)->setValue(data->config.u4);
	keysE.at(11)->setValue(data->config.u5);
	keysE.at(12)->setValue(data->config.START);

	setJoystickLbl();
	
	inconnu1E->setChecked((data->config.divers >> 3) & 1);
}

void ConfigEditor::savePage()
{
	data->config.divers =
			int(ATB2E->isChecked()) | (son2E->isChecked()<<1) | (pointeur2E->isChecked()<<2) | (inconnu1E->isChecked()<<3) |
			(targetMenu2E->isChecked()<<4) | (manetteGroupE->isChecked()<<5) | (vibration2E->isChecked()<<6) | (data->config.divers&0x80);

	data->config.scan = scan2E->isChecked();
	data->config.camera = 4-cameraE->value();
	data->config.vts_combat = 4-vts_combatE->value();
	data->config.vts_msg_combat = 4-vts_msg_combatE->value();
	data->config.vts_msg = 4-vts_msgE->value();
	data->config.analog_volume = analog_volumeE->value();
	
	data->config.u3 = keysE.first()->value();
	data->config.L2 = keysE.at(1)->value();
	data->config.R2 = keysE.at(2)->value();
	data->config.L1 = keysE.at(3)->value();
	data->config.R1 = keysE.at(4)->value();
	data->config.TRIANGLE = keysE.at(5)->value();
	data->config.ROND = keysE.at(6)->value();
	data->config.CROIX = keysE.at(7)->value();
	data->config.CARRE = keysE.at(8)->value();
	data->config.SELECT = keysE.at(9)->value();
	data->config.u4 = keysE.at(10)->value();
	data->config.u5 = keysE.at(11)->value();
	data->config.START = keysE.at(12)->value();
}

void ConfigEditor::setJoystickLbl()
{
	joystickLbl->setText(((data->config.divers >> 7) & 1) ? tr("Joystick gauche : Véhicule avt/arr | Joystick droit : marcher") : tr("Joystick gauche : marcher | Joystick droit : Véhicule avt/arr"));
}

void ConfigEditor::joystickS()
{
	data->config.divers = ((data->config.divers >> 7) & 1) ? (data->config.divers & 0x7F) : (data->config.divers | 0x80);// switch joystick
	setJoystickLbl();
}

void ConfigEditor::defaultKeys()
{
	for(quint8 i=0 ; i<13 ; ++i)
		keysE.at(i)->setValue(i);
	data->config.divers &= 0x7F;// joystick=0
	setJoystickLbl();
}
