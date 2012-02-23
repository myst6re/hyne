/****************************************************************************
 ** Hyne Final Fantasy VIII Save Editor
 ** Copyright (C) 2012 Arzel Jérôme <myst6re@gmail.com>
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

#include "SelectSavesDialog.h"

SelectSavesDialog::SelectSavesDialog(const QList<SaveData *> &saveFiles, bool multiSelection, QWidget *parent) :
	QDialog(parent, Qt::Dialog | Qt::WindowCloseButtonHint)
{
	QLayout *infoLayout = helpLayout();
	fillList(saveFiles);

	for(int i=0 ; i<list->count() ; ++i) {
		if(!saveFiles.at(i)->isFF8())
			list->item(i)->setFlags(Qt::NoItemFlags);
	}

	if(multiSelection) {
		msg->setText(infoText());
		list->setSelectionMode(QAbstractItemView::MultiSelection);
	}
	else {
		msg->setText(tr("Sélectionnez une sauvegarde :"));
	}

	QPushButton *ok = new QPushButton(tr("OK"), this);
	connect(ok, SIGNAL(released()), SLOT(accept()));

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addLayout(infoLayout);
	layout->addWidget(list);
	layout->addWidget(ok, 0, Qt::AlignCenter);

	if(multiSelection)
		connect(list, SIGNAL(itemSelectionChanged()), SLOT(controlSelection()));
}

SelectSavesDialog::SelectSavesDialog(const QList<SaveData *> &saveFiles, QWidget *parent) :
	QDialog(parent, Qt::Dialog | Qt::WindowCloseButtonHint)
{
	QLayout *infoLayout = helpLayout();
	fillList(saveFiles);

	msg->setText(tr("Déplacez les éléments à la souris pour modifier l'ordre des saves."));
	list->setDragDropMode(QAbstractItemView::InternalMove);

	QPushButton *ok = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton), tr("OK"), this);
	connect(ok, SIGNAL(released()), SLOT(accept()));

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addLayout(infoLayout);
	layout->addWidget(list);
	layout->addWidget(ok, 0, Qt::AlignCenter);
}

void SelectSavesDialog::fillList(const QList<SaveData *> &saveFiles)
{
	list = new QListWidget(this);

	SaveData *save;
	QListWidgetItem *item;
	QString shortDescription;
	for(int i=0 ; i<saveFiles.size() ; ++i) {
		save = saveFiles.at(i);
		item = new QListWidgetItem(tr("Save %1 (%2)").arg(i+1).arg((shortDescription = save->getShortDescription()).isEmpty() ? tr("vide") : shortDescription));
		item->setIcon(save->icon());
		list->addItem(item);
		item->setData(Qt::UserRole, i);
	}
}

QLayout *SelectSavesDialog::helpLayout()
{
	msg = new QLabel(this);
	msg->setTextFormat(Qt::RichText);
	QLabel *infoIcon = new QLabel(this);
	infoIcon->setPixmap(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation).pixmap(16));
	QHBoxLayout *infoLayout = new QHBoxLayout;
	infoLayout->addWidget(infoIcon);
	infoLayout->addWidget(msg);
	infoLayout->addStretch();
	infoLayout->setContentsMargins(QMargins());

	return infoLayout;
}

QString SelectSavesDialog::infoText(bool warn)
{
	return tr("Sélectionnez jusqu'à <font color=\"%1\">15 saves</font> :").arg(warn ? "red" : "");
}

QList<int> SelectSavesDialog::selectedSaves() const
{
	QList<int> selected_saves;
	foreach(QListWidgetItem *item, list->selectedItems())
		selected_saves.append(list->row(item));
	return selected_saves;
}

QList<int> SelectSavesDialog::order() const
{
	QList<int> order;
	int count = list->count();
	for(int row=0 ; row<count ; ++row)
		order.append(list->item(row)->data(Qt::UserRole).toInt());
	return order;
}

void SelectSavesDialog::controlSelection()
{
	QList<QListWidgetItem *> selectedItems = list->selectedItems();
	int count = selectedItems.size();
	if(count > 15) {
		msg->setText(infoText(true));
		for(int i=15 ; i<count ; ++i) {
			selectedItems.at(i)->setSelected(false);
		}
	} else if(count < 15) {
		msg->setText(infoText());
	}
}
