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

#include "Editor.h"
#include "PageWidgets/GfEditor.h"
#include "PageWidgets/PersoEditor.h"
#include "PageWidgets/ItemEditor.h"
#include "PageWidgets/TTriadEditor.h"
#include "PageWidgets/CWEditor.h"
#include "PageWidgets/MiscEditor.h"
#include "PageWidgets/ConfigEditor.h"
#include "PageWidgets/AllEditor.h"
#include "PageWidgets/DrawPointEditor.h"
#include "PageWidgets/BattleEditor.h"
#include "PageWidgets/ShopEditor.h"
#include "PageWidgets/WorldmapEditor.h"
#include "PageWidgets/FieldEditor.h"
#include "PageWidgets/PartyEditor.h"
#include "PageWidgets/PreviewEditor.h"

Editor::Editor(QWidget *parent) :
	QWidget(parent)
{
	QFont font;
	font.setPointSize(9);

	liste = new QListWidget(this);
	liste->setFont(font);
	liste->setUniformItemSizes(true);
	liste->setFixedWidth(96);
	liste->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	QPushButton *apply = new QPushButton(tr("&OK"), this);
	apply->setDefault(true);

	// Set default icon if needed
	QStyle *style = this->style();
	if(style->styleHint(QStyle::SH_DialogButtonBox_ButtonsHaveIcons))
		apply->setIcon(style->standardIcon(QStyle::SP_DialogOkButton, 0, this));
	if(style != QApplication::style()) // Propagate style
		apply->setStyle(style);


	QPushButton *cancel = new QPushButton(tr("&Annuler"), this);
	
	// Set default icon if needed
	if(style->styleHint(QStyle::SH_DialogButtonBox_ButtonsHaveIcons))
		cancel->setIcon(style->standardIcon(QStyle::SP_DialogCancelButton, 0, this));
	if(style != QApplication::style()) // Propagate style
		cancel->setStyle(style);

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
	stackedLayout->addWidget(new PartyEditor(this));
	stackedLayout->addWidget(new MiscEditor(this));
	stackedLayout->addWidget(new ConfigEditor(this));
	stackedLayout->addWidget(new PreviewEditor(this));
	stackedLayout->addWidget(new AllEditor(this));

	for(int i=0 ; i<stackedLayout->count() ; ++i)
		liste->addItem(static_cast<PageWidget *>(stackedLayout->widget(i))->name());

	liste->item(liste->count()-1)->setHidden(!Config::mode());
	
	connect(liste, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
	        SLOT(setCurrentSection(QListWidgetItem*,QListWidgetItem*)));
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
		pageWidget = static_cast<PageWidget *>(stackedLayout->widget(liste->row(previous)));
		if(pageWidget->isLoaded()) {
			pageWidget->savePage();
		}
	}

	int id = liste->currentRow();

	pageWidget = static_cast<PageWidget *>(stackedLayout->widget(id));
	if(!pageWidget->isLoaded()) {
		pageWidget->load(&saveDataCopy, pc);
	}
	// AllEditor exception
	if(id == liste->count()-1)
		pageWidget->fillPage();
	stackedLayout->setCurrentIndex(id);
}

void Editor::load(SaveData *saveData, bool pc)
{
	this->pc = pc;
	this->saveData = saveData;
	this->saveDataCopy = *saveData;

	int pageCount = stackedLayout->count();
	for(int i=0 ; i<pageCount ; ++i) {
		static_cast<PageWidget *>(stackedLayout->widget(i))->unload();
	}

	setCurrentSection(liste->currentItem());
}

void Editor::save()
{
	bool saveOneAtLeast = false;
	int pageCount = stackedLayout->count();
	for(int i=0 ; i<pageCount ; ++i) {
		PageWidget *pageWidget = static_cast<PageWidget *>(stackedLayout->widget(i));
		if(pageWidget->isLoaded()) {
			pageWidget->savePage();
			saveOneAtLeast = true;
		}
	}

	if(saveOneAtLeast) {
		*saveData = saveDataCopy;
		saveData->updateDescData();
		saveData->setModified(true);
	}

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
		PageWidget *pageWidget = static_cast<PageWidget *>(stackedLayout->widget(i));
		if(pageWidget->isBuilded()) {
			pageWidget->updateMode(mode);
		}
	}
}

void Editor::updateTime()
{
	int pageCount = stackedLayout->count();
	for(int i=0 ; i<pageCount ; ++i) {
		PageWidget *pageWidget = static_cast<PageWidget *>(stackedLayout->widget(i));
		if(pageWidget->isLoaded()) {
			pageWidget->updateTime();
		}
	}
}
