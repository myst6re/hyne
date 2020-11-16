/****************************************************************************
 ** Hyne Final Fantasy VIII Save Editor
 ** Copyright (C) 2013 Arzel Jérôme <myst6re@gmail.com>
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

SelectSavesDialog::SelectSavesDialog(const QList<SaveData *> &saveFiles, bool multiSelection, bool onlyFF8, QWidget *parent) :
	QDialog(parent, Qt::Dialog | Qt::WindowCloseButtonHint)
{
	msg = new HelpWidget(16, this);
	fillList(saveFiles);

	for (int i = 0; i < list->count(); ++i) {
		if (!saveFiles.at(i)->isFF8() &&
				(onlyFF8 || saveFiles.at(i)->isRaw() || saveFiles.at(i)->isDelete()))
			list->item(i)->setFlags(Qt::NoItemFlags);
	}

	if (multiSelection) {
		msg->setText(infoText());
		list->setSelectionMode(QAbstractItemView::MultiSelection);
	}
	else {
		msg->setText(tr("Sélectionnez une sauvegarde :"));
	}

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok,
													   Qt::Horizontal, this);

	connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(msg);
	layout->addWidget(list);
	layout->addWidget(buttonBox);

	if (multiSelection)
		connect(list, SIGNAL(itemSelectionChanged()), SLOT(controlSelection()));
}

SelectSavesDialog::SelectSavesDialog(const QList<SaveData *> &saveFiles, QWidget *parent) :
	QDialog(parent, Qt::Dialog | Qt::WindowCloseButtonHint)
{
	msg = new HelpWidget(16, this);
	fillList(saveFiles);

	msg->setText(tr("Déplacez les éléments à la souris pour modifier l'ordre des saves."));
	list->setDragDropMode(QAbstractItemView::InternalMove);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok,
													   Qt::Horizontal, this);

	connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(msg);
	layout->addWidget(list);
	layout->addWidget(buttonBox);
}

void SelectSavesDialog::fillList(const QList<SaveData *> &saveFiles)
{
	list = new QListWidget(this);

	SaveData *save;
	QString shortDescription;
	for (int i = 0; i < saveFiles.size(); ++i) {
		save = saveFiles.at(i);
		shortDescription = save->shortDescription();
		QListWidgetItem *item = new QListWidgetItem(tr("Save %1 (%2)").arg(i+1)
		                                            .arg(shortDescription.isEmpty()
		                                                 ? tr("vide")
		                                                 : shortDescription));
		item->setIcon(save->saveIcon().icon());
		list->addItem(item);
		item->setData(Qt::UserRole, i);
	}
}

QString SelectSavesDialog::infoText(bool warn)
{
	return tr("Sélectionnez jusqu'à <font color=\"%1\">15 saves</font> :")
	        .arg(warn ? "red" : "");
}

QList<int> SelectSavesDialog::selectedSaves() const
{
	QList<int> selected_saves;
	for (const QListWidgetItem *item : list->selectedItems())
		selected_saves.append(list->row(item));
	return selected_saves;
}

QList<int> SelectSavesDialog::order() const
{
	QList<int> order;
	int count = list->count();
	for (int row = 0; row < count; ++row)
		order.append(list->item(row)->data(Qt::UserRole).toInt());
	return order;
}

void SelectSavesDialog::controlSelection()
{
	QList<QListWidgetItem *> selectedItems = list->selectedItems();
	int count = selectedItems.size();
	if (count > 15) {
		msg->setText(infoText(true));
		for (int i = 15; i < count; ++i) {
			selectedItems.at(i)->setSelected(false);
		}
	} else if (count < 15) {
		msg->setText(infoText());
	}
}
