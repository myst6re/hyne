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

#include "SavecardWidget.h"
#include "Parameters.h"
#include "HeaderDialog.h"

SavecardWidget::SavecardWidget(QWidget *parent) :
	QListWidget(parent), _dragStart(-1), notify(true), _data(0)
{
	setWidget();
}

SavecardWidget::~SavecardWidget()
{
}

void SavecardWidget::setWidget()
{
	setPalette(QPalette(Qt::black));
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	setSelectionMode(QAbstractItemView::NoSelection);
	setFrameShape(QFrame::NoFrame);
	setUniformItemSizes(true);
	// Drag & drop
//	setDragEnabled(true);
//	setDragDropMode(QAbstractItemView::InternalMove);
//	viewport()->setAcceptDrops(true);
//	setDropIndicatorShown(true);
}

SavecardData *SavecardWidget::savecard() const
{
	return _data;
}

void SavecardWidget::setSavecard(SavecardData *save)
{
	clear();

	_data = save;

	if(_data) {
		foreach(SaveData *save, _data->getSaves()) {
			addSave(save);
		}

		connect(_data->watcher(), SIGNAL(fileChanged(QString)), SLOT(notifyFileChanged(QString)));
	}
}

void SavecardWidget::notifyFileChanged(const QString &path)
{
	if(!notify)	return;
	notify = false;
	if(!QFile::exists(path))
	{
		QMessageBox::warning(this->parentWidget(), tr("Fichier supprimé"), tr("Le fichier '%1' a été supprimé par un programme externe !").arg(path));
	}
	else
	{
		QMessageBox::warning(this->parentWidget(), tr("Fichier modifié"), tr("Le fichier '%1' a été modifié par un programme externe.").arg(path));
	}
	notify = true;
}

SaveWidget *SavecardWidget::saveWidget(int row) const
{
	return (SaveWidget *)itemWidget(item(row));
}

void SavecardWidget::moveCursor(int row)
{
	for(int i=0 ; i<row ; ++i) {
		saveWidget(i)->hideCursor();
	}

	for(int i=row+1 ; i<count() ; ++i) {
		saveWidget(i)->hideCursor();
	}
}

void SavecardWidget::updateSaveWidgets()
{
	int countItems = count();
	for(int saveID=0 ; saveID<countItems ; ++saveID)
		saveWidget(saveID)->update();
}

void SavecardWidget::addSave(SaveData *saveData)
{
	QListWidgetItem *item = new QListWidgetItem(this);
	item->setFlags(Qt::NoItemFlags);

	SaveWidget *saveWidget = new SaveWidget(saveData, this);
	item->setSizeHint(saveWidget->sizeHint());
	setItemWidget(item, saveWidget);

	connect(saveWidget, SIGNAL(released(SaveData*)), parent(), SLOT(editView(SaveData*)));
	connect(saveWidget, SIGNAL(changed()), parent(), SLOT(setModified()));
}

void SavecardWidget::setDragStart(int saveID)
{
	_dragStart = saveID;
}

void SavecardWidget::moveDraggedSave(int saveID)
{
	if(_dragStart < saveID) {
		saveID--;
	}

	if(_dragStart != saveID
			&& _dragStart >= 0 && _dragStart < _data->getSaves().size()) {
		QList<SaveData *> saves = _data->getSaves();
		SaveData *saveData = saves.takeAt(_dragStart);
		saves.insert(saveID, saveData);

		saveID = 0;
		foreach(saveData, saves) {
			saveData->setId(saveID);
			saveWidget(saveID)->setSaveData(saveData);
			saveID++;
		}

		_data->setSaves(saves);

		updateSaveWidgets();

		emit modified();
	}

	_dragStart = -1;
}

void SavecardWidget::replaceSaveData(int saveID, const QByteArray &mimeData)
{
	if(saveID >=0 && saveID < _data->getSaves().size()
			&& !mimeData.isEmpty()) {
		SaveData *saveData = _data->getSaves().at(saveID);

		if(!saveData->isDelete()) {
			QMessageBox::StandardButton answer = QMessageBox::question(this, tr("Écraser"), tr("Tout le contenu de la sauvegarde sera écrasé.\nContinuer ?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
			if(answer != QMessageBox::Yes) {
				return;
			}
		}

		if(mimeData.size() > SAVE_SIZE) {
			saveData->open(mimeData.left(SAVE_SIZE), mimeData.mid(SAVE_SIZE));
		} else if(mimeData.size() == SAVE_SIZE) {
			QByteArray MCHeader;
			if(saveData->hasMCHeader()) {
				HeaderDialog dialog(saveData, this, HeaderDialog::CreateView);

				if(dialog.exec() != QDialog::Accepted) {
					return;
				}
				MCHeader = saveData->MCHeader();
			}
			saveData->open(mimeData, MCHeader);
		} else {
			return;
		}

		saveWidget(saveID)->update();

		_data->setModified(true);
		saveData->setModified(true);
		emit modified();
	}
}

void SavecardWidget::setDropIndicatorIsVisible(int saveID, bool onTop, bool isVisible)
{
	SaveWidget *save = saveWidget(saveID);
	if(save) {
		save->setDropIndicatorIsVisible(onTop, isVisible);
	}
}

void SavecardWidget::scrollToDrag(int saveID, const QPoint &pos)
{
	QListWidgetItem *curItem = item(saveID);
	if(!curItem)	return;

	QRect visualRect = visualItemRect(curItem);
	const int locationHeight = visualRect.height() / 2;
	const int posYInView = visualRect.top() + pos.y();

	if(posYInView >= height() - locationHeight) {
		const int posYInBottomLocation = locationHeight - (height() - posYInView);

		verticalScrollBar()->setValue(verticalScrollBar()->value() + (verticalScrollBar()->maximum() / 15) * posYInBottomLocation / locationHeight);
	} else if(posYInView <= locationHeight) {
		const int posYInTopLocation = locationHeight - posYInView;

		verticalScrollBar()->setValue(verticalScrollBar()->value() - (verticalScrollBar()->maximum() / 15) * posYInTopLocation / locationHeight);
	}
}
