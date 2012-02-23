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

#include "Editor.h"

Editor::Editor(QWidget *parent)
	: QWidget(parent)
{
	QFont font;
	font.setPointSize(9);

	liste = new QListWidget(this);
	liste->setFont(font);
	liste->setUniformItemSizes(true);
	liste->setFixedWidth(96);
	liste->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	liste->addItems(QStringList() << tr("G-Forces") << tr("Personnages") << tr("Objets") << tr("Boutiques")
					<< tr("Triple Triad") << tr("Sources") << tr("Combats") << tr("Terrain") << tr("Mappemonde") << tr("Chocobo World")
					<< tr("Divers") << tr("Configuration") << tr("HexaEdit"));

	QPushButton *apply = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton), tr("&Sauver"), this);
	QPushButton *cancel = new QPushButton(tr("Annuler"), this);
	apply->setShortcut(QKeySequence::Save);
	
	stackedLayout = new QStackedLayout;
	
	QGridLayout *gridLayout = new QGridLayout(this);
	gridLayout->addWidget(liste, 0, 0);
	gridLayout->addWidget(apply, 1, 0);
	gridLayout->addWidget(cancel, 2, 0);
	gridLayout->addLayout(stackedLayout, 0, 1, 3, 1);
	gridLayout->setContentsMargins(QMargins(gridLayout->verticalSpacing(),gridLayout->verticalSpacing(),gridLayout->verticalSpacing(),gridLayout->verticalSpacing()));
	
	stackedLayout->addWidget(new GfEditor(this));
	stackedLayout->addWidget(new PersoEditor(this));
	stackedLayout->addWidget(new ItemEditor(this));
	stackedLayout->addWidget(new ShopEditor(this));
	stackedLayout->addWidget(new TTriadEditor(this));
	stackedLayout->addWidget(new DrawPointEditor(this));
	stackedLayout->addWidget(new BattleEditor(this));
	stackedLayout->addWidget(new FieldEditor(this));
	stackedLayout->addWidget(new WorldmapEditor(this));
	stackedLayout->addWidget(new CWEditor(this));
	stackedLayout->addWidget(new MiscEditor(this));
	stackedLayout->addWidget(new ConfigEditor(this));
	stackedLayout->addWidget(allEditor = new AllEditor(this));

	liste->item(liste->count()-1)->setHidden(!Config::mode());

	widgetsLoaded = false;
	
	connect(liste, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), SLOT(setCurrentSection(QListWidgetItem*,QListWidgetItem*)));
	connect(apply, SIGNAL(released()), SLOT(save()));
	connect(cancel, SIGNAL(released()), SIGNAL(rejected()));
}

void Editor::setCurrentSection(QListWidgetItem *current, QListWidgetItem *previous)
{
	PageWidget *pageWidget;

	if(current == NULL) {
		liste->setCurrentRow(0);
		return;
	}

	if(previous != NULL) {
		pageWidget = (PageWidget *)stackedLayout->widget(liste->row(previous));
		if(pageWidget->isLoaded()) {
			pageWidget->savePage();
		}
	}

	int id = liste->currentRow();

	pageWidget = (PageWidget *)stackedLayout->widget(id);
	if(!pageWidget->isLoaded()) {
		pageWidget->load(&copy, &descCopy, freq_value, jp, pc);
	}
	if(id==liste->count()-1)
		allEditor->fillPage();
	stackedLayout->setCurrentIndex(id);
}

void Editor::load(SaveData *saveData, bool pc)
{
	jp = saveData->isJp();
	this->pc = pc;
	freq_value = saveData->freqValue();
	this->saveData = saveData;
	descCopy = saveData->descData();
	copy = saveData->mainData();

	int pageCount = stackedLayout->count();
	for(int i=0 ; i<pageCount ; ++i) {
		((PageWidget *)stackedLayout->widget(i))->unload();
	}

	setCurrentSection(liste->currentItem());
}

void Editor::save()
{
	int pageCount = stackedLayout->count();
	for(int i=0 ; i<pageCount ; ++i) {
		PageWidget *pageWidget = (PageWidget *)stackedLayout->widget(i);
		if(pageWidget->isLoaded()) {
			pageWidget->savePage();
		}
	}

	saveData->setMainData(copy);
	saveData->setDescData(descCopy);

	emit accepted();
}

void Editor::updateMode(bool mode)
{
	int lastRow = liste->count()-1;
	liste->item(lastRow)->setHidden(!mode);
	if(liste->currentRow()==lastRow && !mode)
		liste->setCurrentRow(0);

	int pageCount = stackedLayout->count();
	for(int i=0 ; i<pageCount ; ++i) {
		PageWidget *pageWidget = (PageWidget *)stackedLayout->widget(i);
		if(pageWidget->isBuilded()) {
			pageWidget->updateMode(mode);
		}
	}
}

void Editor::updateTime()
{
	int pageCount = stackedLayout->count();
	for(int i=0 ; i<pageCount ; ++i) {
		PageWidget *pageWidget = (PageWidget *)stackedLayout->widget(i);
		if(pageWidget->isLoaded()) {
			pageWidget->updateTime();
		}
	}
}
