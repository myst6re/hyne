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

#include "PageWidget.h"

PageWidget::PageWidget(QWidget *parent) :
    QWidget(parent)
{
	builded = loaded = false;
}

void PageWidget::load(SaveData *saveData, bool pc)
{
	if (!builded) {
		buildWidget();
		builded = true;
	}
	this->data = &saveData->mainData();
	this->descData = &saveData->descData();
	this->saveData = saveData;
	this->pc = pc;
	updateMode(Config::mode());
	fillPage();
	loaded = true;
	updateModeAfter(Config::mode());
	updateTime();
}

void PageWidget::unload()
{
	loaded = false;
}

bool PageWidget::isLoaded() const
{
	return loaded;
}

bool PageWidget::isBuilded() const
{
	return builded;
}

void PageWidget::updateMode(bool)
{
}

void PageWidget::updateModeAfter(bool)
{
}

void PageWidget::updateTime()
{
}

void PageWidget::selectAll(QTreeWidget *list, bool selected)
{
	int count = list->topLevelItemCount();
	for (int i = 0; i < count; ++i) {
		list->topLevelItem(i)->setCheckState(0, selected ? Qt::Checked : Qt::Unchecked);
	}
}

void PageWidget::setCurrentIndex(QComboBox *comboBox, int value)
{
	int index = comboBox->findData(value);
	if (index != -1) {
		comboBox->setCurrentIndex(index);
	} else {
		comboBox->addItem(QString::number(value), value);
		comboBox->setCurrentIndex(comboBox->count() - 1);
	}
}

QMap<int, QIcon> PageWidget::abilityIcons()
{
	QMap<int, QIcon> icons;
	for (int i = 0; i < 6; ++i)
		icons.insert(i, QIcon(QString(":/images/icons/capacity%1.png").arg(i)));
	return icons;
}

void PageWidget::fillAbilities(QComboBox *comboBox, const QMap<int, QIcon> &icons, const QList<qint8> &typeFilter)
{
	int abilityCount = Data::abilities().size();
	for (int i = 1; i < abilityCount; ++i) {
		qint8 type = Data::abilityType(i);
		if (typeFilter.isEmpty() || typeFilter.contains(type)) {
			comboBox->addItem(icons.value(type), Data::abilities().at(i), i);
		}
	}
}

//void PageWidget::changeEvent(QEvent *e)
//{
//	if (e->type() == QEvent::LanguageChange) {
//
//	} else {
//		QWidget::changeEvent(e);
//	}
//}
