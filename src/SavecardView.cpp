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

#include "SavecardView.h"
#include "HeaderDialog.h"
#include "Config.h"
#include "Data.h"
#include "FF8Text.h"

SavecardView::SavecardView(SavecardWidget *parent) :
	QWidget(parent), cursorID(-1), blackID(-1),
	dropIndicatorID(-1), isExternalDrag(false),
	_dragStart(-1), notify(true), _data(0), _parent(parent),
	mouseMove(0), lastDropData(0), currentSaveIconFrame(0)
{
	setPalette(QPalette(Qt::black));
	connect(&SaveIcon::timer, SIGNAL(timeout()), SLOT(nextIcon()));
	setMouseTracking(true);
	setAcceptDrops(true);
}

SavecardView::~SavecardView()
{
	if (lastDropData)	delete lastDropData;
}

void SavecardView::clear()
{
	if (_data) {
		_data = 0;
		update();
	}
}

SavecardData *SavecardView::savecard() const
{
	return _data;
}

void SavecardView::setSavecard(SavecardData *save)
{
	_data = save;

	if (_data) {
		setFixedSize(sizeHint());
#ifndef Q_OS_WINRT
		connect(_data->watcher(), SIGNAL(fileChanged(QString)), SLOT(notifyFileChanged(QString)));
#endif
		if (!SaveIcon::timer.isActive()) {
			SaveIcon::timer.start();
		}
	}
}

void SavecardView::notifyFileChanged(const QString &path)
{
	if (!notify)	return;
	notify = false;
	emit externalFileChanged(path);
	notify = true;
}

void SavecardView::updateSave(int saveID, bool withCursor)
{
	if (withCursor) {
		update(QRect(savePoint(saveID) - QPoint(horizontalMargin(), 0), saveSize() + QSize(horizontalMargin(), 0)));
	} else {
		update(saveRect(saveID));
	}
}

void SavecardView::updateSaves(const QList<int> &saveIDs, bool withCursor)
{
	if (saveIDs.isEmpty())	return;
	if (saveIDs.size() == 1) {
		updateSave(saveIDs.first(), withCursor);
		return;
	}

	int minID=_data->saveCount(), maxID=-1;

	for (int id : saveIDs) {
		if (id < 0 || id > _data->saveCount()) {
			continue;
		}
		if (id < minID)	minID = id;
		if (id > maxID)	maxID = id;
	}

	if (minID == maxID) {
		updateSave(minID, withCursor);
	} else {
		if (withCursor) {
			update(QRect(savePoint(minID) - QPoint(horizontalMargin(), 0),
						 QSize(saveWidth() + horizontalMargin(), saveHeight() * (maxID - minID + 1))));
		} else {
			update(QRect(savePoint(minID), QSize(saveWidth(), saveHeight() * (maxID - minID + 1))));
		}
	}
}

void SavecardView::moveCursor(int saveID)
{
	if (saveID == cursorID)	return;

	int oldCursorID = cursorID, minID, maxID;
	cursorID = saveID;

	if (oldCursorID < 0) {
		oldCursorID = cursorID;
	}

	if (oldCursorID < cursorID) {
		minID = oldCursorID;
		maxID = cursorID;
	} else {
		minID = cursorID;
		maxID = oldCursorID;
	}

	if (minID < 0) {
		minID = maxID;
	} else if (maxID < 0) {
		maxID = minID;
	}

	update(QRect(savePoint(minID) + QPoint(-36, 16), QSize(48, (maxID - minID)*saveHeight() + 46)));
}

void SavecardView::setDropIndicator(int saveID)
{
	if (dropIndicatorID == saveID) return;

	dropIndicatorID = saveID;
	update();
}

void SavecardView::setBlackSave(int saveID)
{
	if (blackID == saveID) return;

	int oldBlackID = blackID;
	blackID = saveID;

	if (blackID <= -1) {
		updateSave(oldBlackID, true);
	} else if (oldBlackID <= -1) {
		updateSave(blackID, true);
	} else {
		updateSaves(QList<int>() << oldBlackID << blackID, true);
	}
}

void SavecardView::moveDraggedSave(int saveID)
{
	if (!_data)	return;
	if (_dragStart < saveID) {
		saveID--;
	}

	if (_dragStart != saveID
			&& _dragStart >= 0 && _dragStart < _data->saveCount()) {
		_data->moveSave(_dragStart, saveID);

		updateSaves(QList<int>() << _dragStart << saveID);

		emit changed();
	}

	_dragStart = -1;
}

void SavecardView::replaceSaveData(int saveID, const QByteArray &mimeData)
{
	if (!_data)	return;
	if (saveID >=0 && saveID < _data->saveCount()
			&& !mimeData.isEmpty()) {
		SaveData *saveData = _data->getSave(saveID);

		if (!saveData->isDelete()) {
			QMessageBox::StandardButton answer = QMessageBox::question(this, tr("Écraser"), tr("Tout le contenu de la sauvegarde sera écrasé.\nContinuer ?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
			if (answer != QMessageBox::Yes) {
				return;
			}
		}

		if (mimeData.size() > SAVE_SIZE) {
			saveData->open(mimeData.left(SAVE_SIZE), mimeData.mid(SAVE_SIZE));
		} else if (mimeData.size() == SAVE_SIZE) {
			QByteArray MCHeader;
			if (saveData->hasMCHeader()) {
				HeaderDialog dialog(saveData, this, HeaderDialog::CreateView);

				if (dialog.exec() != QDialog::Accepted) {
					return;
				}
				MCHeader = saveData->MCHeader();
			}
			saveData->open(mimeData, MCHeader);
		} else {
			return;
		}

		_data->setModified(true);
		saveData->setModified(true);
		emit changed();
	}
}

void SavecardView::edit(int saveID)
{
	if (!_data)	return;

	if (saveID == -1) {
		saveID = this->saveID(startPos);
	}

	SaveData *saveData = _data->getSave(saveID);
	if (!saveData)	return;

	if (saveData->isFF8() && !saveData->isDelete())
		emit released(saveData);
}

void SavecardView::exportOne(int saveID)
{
	if (!_data)	return;

	if (saveID == -1) {
		saveID = this->saveID(startPos);
	}

	SaveData *saveData = _data->getSave(saveID);
	if (!saveData)	return;

	QString selectedFilter,
	        pc = tr("FF8 PC save (*.ff8 *)"),
	        ps4 = tr("PS4 Remaster save (*.ff8 *)"),
	        swi = tr("Switch save (ff8slot*)"),
	        psv = tr("PSN save (*.psv)");
	SavecardData::Type type = _data->type();
	QStringList types;
	types << pc << ps4 << swi << psv;

	if (type == SavecardData::PcUncompressed)    selectedFilter = ps4;
	else if (type == SavecardData::Switch)       selectedFilter = swi;
	else if (type == SavecardData::Psv)          selectedFilter = psv;
	else                                        selectedFilter = pc;

	QString path, savePath = Config::value(Config::SavePath);

	path = savePath.isEmpty() ? _data->dirname() : savePath % "/";
	QString name = _data->nameNoExtension();
	name = (name.isEmpty() ? "save" : name) % QString("%1").arg(saveData->id() + 1, 2, 10, QChar('0'));

	path = QFileDialog::getSaveFileName(
	           this, tr("Exporter"),
	           path % name,
	           types.join(";;"),
	           &selectedFilter
	);
	if (path.isEmpty())		return;

	if (selectedFilter == pc)          type = SavecardData::Pc;
	else if (selectedFilter == ps4)    type = SavecardData::PcUncompressed;
	else if (selectedFilter == swi)    type = SavecardData::Switch;
	else if (selectedFilter == psv)    type = SavecardData::Psv;
	else {
		qWarning() << "Bad selected filter!" << selectedFilter;
		return;
	}

	int index = path.lastIndexOf('/');
	Config::setValue(Config::SavePath, index == -1 ? path : path.left(index));

    if (!_data->saveOne(saveData, path, type, _data->type() != type && (type == SavecardData::Pc || type == SavecardData::PcUncompressed))) {
		QMessageBox::warning(this, tr("Échec"), tr("Enregistrement échoué, vérifiez que le fichier cible n'est pas utilisé."));
	}
}

void SavecardView::newGame(int saveID)
{
	if (!_data)	return;

	if (saveID == -1) {
		saveID = this->saveID(startPos);
	}

	SaveData *saveData = _data->getSave(saveID);
	if (!saveData)	return;

	if (!saveData->isDelete()) {
		QMessageBox::StandardButton b = QMessageBox::question(this, tr("Nouvelle partie"), tr("Tout le contenu de la sauvegarde sera remplacé par une nouvelle partie.\nContinuer ?"), QMessageBox::Yes | QMessageBox::No);
		if (b != QMessageBox::Yes) {
			return;
		}
	}

	QFile newGameFile(":/data/newGame");
	if (!newGameFile.open(QIODevice::ReadOnly)) {
		qWarning() << "failed to open data/newGame" << newGameFile.errorString();
		return;
	}

	if (saveData->hasMCHeader() && (!saveData->isFF8() || saveData->isDelete())) {
		HeaderDialog dialog(saveData, this, HeaderDialog::RestoreView);
		if (dialog.exec()==QDialog::Accepted)
		{
			saveData->restore();
		}
		else
		{
			return;
		}
	}

	saveData->open(newGameFile.readAll(), saveData->MCHeader());
	// Set translated names
	saveData->setPerso(SQUALL, Data::names().at(SQUALL));
	saveData->setPerso(RINOA, Data::names().at(RINOA));
	saveData->setPerso(GRIEVER, Data::names().at(GRIEVER));
	saveData->setPerso(BOKO, Data::names().at(BOKO));
	saveData->setPerso(ANGELO, Data::names().at(ANGELO));
	int gfID=0;
	for (const QString &gfName : Data::gfnames().list()) {
		saveData->setGf(gfID++, gfName);
	}
	saveData->setModified(true);
	emit changed();
	newGameFile.close();

	update();
}

void SavecardView::removeSave(int saveID)
{
	if (!_data)	return;

	if (saveID == -1) {
		saveID = this->saveID(startPos);
	}

	SaveData *saveData = _data->getSave(saveID);
	if (!saveData)	return;

	if (!saveData->isDelete()) {
		QMessageBox::StandardButton b = QMessageBox::question(this, tr("Vider"), tr("Tout le contenu de la sauvegarde sera vidé.\nContinuer ?"), QMessageBox::Yes | QMessageBox::No);
		if (b != QMessageBox::Yes) {
			return;
		}
	}

	saveData->remove();
	emit changed();

	update();
}

void SavecardView::properties(int saveID)
{
	if (!_data)	return;

	if (saveID == -1) {
		saveID = this->saveID(startPos);
	}

	SaveData *saveData = _data->getSave(saveID);
	if (!saveData)	return;

	HeaderDialog dialog(saveData, this);

	if (dialog.exec() == QDialog::Accepted
			&& saveData->isModified()) {
		emit changed();
		if (!saveData->isFF8() && !saveData->isDelete()) {
			refreshIcon(saveData); // update icon
		}
	}
}

void SavecardView::restore(int saveID)
{
	if (!_data)	return;
	SaveData *saveData = _data->getSave(saveID);
	if (!saveData)	return;

	QMessageBox::StandardButton reponse = QMessageBox::question(this, tr("Sauvegarde supprimée"), tr("Cette sauvegarde a été supprimée, voulez-vous tenter de la réparer ? (succès non garanti)"),
																QMessageBox::Yes | QMessageBox::No);
	if (reponse != QMessageBox::Yes)  return;

	HeaderDialog dialog(saveData, this, HeaderDialog::RestoreView);

	if (dialog.exec() == QDialog::Accepted)
	{
		saveData->restore();
		emit changed();
	}
}

int SavecardView::saveID(const QPoint &pos) const
{
	if (pos.x() < 0
			|| pos.x() >= width()
			|| pos.y() < 0
			|| pos.y() >= height())
		return -1;

	return pos.y() / saveHeight();
}

QSize SavecardView::sizeHint() const
{
	if (!_data)	return QSize();
	return QSize(horizontalMargin() * 2 + saveWidth(), saveHeight() * _data->saveCount());
}

QSize SavecardView::minimumSizeHint() const
{
	return sizeHint();
}

void SavecardView::refreshIcon(SaveData *saveData)
{
	if (!saveData->isDelete()) {
		repaint(QRect(savePoint(saveData->id()) + QPoint(36, 43), QSize(16, 16)));
	}
}

void SavecardView::nextIcon()
{
	if (!_data) return;

	++currentSaveIconFrame;
	for (SaveData *saveData : qAsConst(_data->getSaves())) {
		refreshIcon(saveData);
	}
}

void SavecardView::renderSave(QPainter *painter, const SaveData *saveData, int currentIconFrame, const QRect &sourceRect)
{
	QPixmap menuBg(QString(":/images/menu-fond%1.png").arg(!saveData->isTheLastEdited() && !saveData->isDelete() ? "" : "2"));
	QPixmap menuTitle(":/images/numbers_title.png");
	QImage numberPixmap(":/images/numbers.png");

	renderSave(painter, saveData, menuBg, menuTitle, numberPixmap, currentIconFrame, sourceRect);
}

void SavecardView::renderSave(QPixmap *pixmap, const SaveData *saveData, int currentIconFrame, const QRect &sourceRect)
{
	QPainter p(pixmap);
	renderSave(&p, saveData, currentIconFrame, sourceRect);
}

void SavecardView::renderSave(QPainter *painter, const SaveData *saveData, const QPixmap &menuBg, const QPixmap &fontPixmap, QImage &numberPixmap, int currentIconFrame, const QRect &sourceRect)
{
	QRect toBePainted;
	if (sourceRect.isNull()) {
		toBePainted = QRect(QPoint(0, 0), saveSize());
	} else {
		toBePainted = sourceRect;
	}

	painter->save();

	// Background
	painter->setBrush(QBrush(menuBg));
	drawFrame(painter, saveWidth(), saveHeight());

	// Save Number (Frame title)
	if (!(toBePainted & QRect(4, 0, 36, 22)).isEmpty()) {
		QString title = QString("%1").arg(saveData->id()+1, 2, 10, QChar('0'));
		painter->drawPixmap(4, 0, fontPixmap.copy(title.at(0).digitValue()*16, 0, 16, 22));
		painter->drawPixmap(20, 0, fontPixmap.copy(title.at(1).digitValue()*16, 0, 16, 22));
	}

	if (saveData->isFF8())
	{
		// Portrait chars
		if (saveData->constDescData().party[0] != 255
				&& !(toBePainted & QRect(44, 4, 64, 96)).isEmpty())
			painter->drawPixmap(44, 4, QPixmap(QString(":/images/icons/perso%1.png").arg(saveData->constDescData().party[0] & 15)));
		if (saveData->constDescData().party[1] != 255
				&& !(toBePainted & QRect(112, 4, 64, 96)).isEmpty())
			painter->drawPixmap(112, 4, QPixmap(QString(":/images/icons/perso%1.png").arg(saveData->constDescData().party[1] & 15)));
		if (saveData->constDescData().party[2] != 255
				&& !(toBePainted & QRect(180, 4, 64, 96)).isEmpty())
			painter->drawPixmap(180, 4, QPixmap(QString(":/images/icons/perso%1.png").arg(saveData->constDescData().party[2] & 15)));

		// Main char name
		if (!(toBePainted & QRect(271, 8, saveWidth()-271, 24)).isEmpty()) {
			int persoIndex = saveData->constDescData().party[0] != 255 ? saveData->constDescData().party[0] : (saveData->constDescData().party[1] != 255 ? saveData->constDescData().party[1] : saveData->constDescData().party[2]);
			bool langIndep = persoIndex==SQUALL || persoIndex==RINOA || persoIndex==GRIEVER || persoIndex==BOKO || persoIndex==ANGELO;
			FF8Text::drawTextArea(painter, QPoint(271, 8), saveData->perso(persoIndex), langIndep ? (saveData->isJp() ? 2 : 1) : 0);
		}

		// Level
		if (!(toBePainted & QRect(271, 36, saveWidth()-271, 24)).isEmpty()) {
			FF8Text::drawTextArea(painter, QPoint(271, 36), tr("NV%1").arg(saveData->constDescData().nivLeader,3,10,QChar(' ')), 1);
		}

		// Disc number
		if (!(toBePainted & QRect(391, 38, saveWidth()-391, 16)).isEmpty()) {
			QPixmap disc(QString(":/images/disc_%1.png").arg(Config::value(Config::Lang)=="fr" ? "fr" : "en"));
			painter->drawPixmap(391, 38, disc);
			num2pix(painter, &numberPixmap, 395+disc.width(), 38, saveData->constDescData().disc+1);
		}

		// Play time
		if (!(toBePainted & QRect(511, 16, saveWidth()-511, 16)).isEmpty()) {
			painter->drawPixmap(511, 16, QPixmap(QString(":/images/play_%1.png").arg(Config::value(Config::Lang)=="fr" ? "fr" : "en")));

			int hour = Config::hour(saveData->constDescData().time, saveData->freqValue());
			int color = (hour>=100) + (hour>=200) + (hour>=300) + (hour>=400) + (hour>=500);
			num2pix(painter, &numberPixmap, 576, 16, hour, 2, QChar(' '), color);

			QImage deux_points(":/images/deux-points.png");
			colors(&deux_points, color);
			painter->drawImage(612, 18, deux_points);
			num2pix(painter, &numberPixmap, 624, 16, Config::min(saveData->constDescData().time, saveData->freqValue()), 2, QChar('0'), color);
		}

		// Gils
		if (!(toBePainted & QRect(511, 40, saveWidth()-511, 16)).isEmpty()) {
			num2pix(painter, &numberPixmap, 511, 40, saveData->constDescData().gils, 8);
			painter->drawPixmap(640, 44, QPixmap(":/images/gils.png"));
		}

		if (!(toBePainted & QRect(256, 62, 416, 44)).isEmpty()) {
			// Sub-Frame
			painter->translate(256, 62);
			painter->setBrush(QBrush());
			drawFrame(painter, 416, 44);

			// Location
			if (!(toBePainted & QRect(256+12, 62+12, saveWidth()-(256+12), 24)).isEmpty()) {
				FF8Text::drawTextArea(painter, QPoint(12, 12), saveData->constDescData().locationID<251 ? Data::locations().at(saveData->constDescData().locationID) : QString("??? (%1)").arg(saveData->constDescData().locationID));
			}
		}
	}
	else
	{
		if (!(toBePainted & QRect(36, 43, saveWidth()-36, 24)).isEmpty()) {
			if (saveData->isRaw())
			{
				// Unavailable block
				FF8Text::drawTextArea(painter, QPoint(36, 43), tr("Bloc occupé"));
			}
			else if (saveData->isDelete())
			{
				// Available block
				FF8Text::drawTextArea(painter, QPoint(36, 43), tr("Bloc Disponible"));
			}
			else
			{
				// Icon + description
				painter->drawPixmap(36, 44, saveData->saveIcon().icon(currentIconFrame % saveData->saveIcon().nbFrames()));
				QString short_desc = saveData->shortDescription();
				if (!short_desc.isEmpty())
				{
					painter->setPen(Qt::white);
					painter->drawText(68, 57, short_desc);
				}
			}
		}
	}

	painter->restore();

	// Save modification state
	if (saveData->isModified() || saveData->wasModified()) {
		QPen pen(saveData->isModified() ? QColor(0xd1,0x1d,0x1d) : QColor(0x1d,0xd1,0x1d), 3);
		painter->setPen(pen);
		painter->drawLine(670, 3, 670, 103);
	}
}

void SavecardView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), palette().color(QPalette::Window));

	if (_data) {
		painter.translate(horizontalMargin(), 0);

		int curSaveID, minSaveID, maxSaveID;
		minSaveID = saveID(event->rect().topLeft());
		maxSaveID = saveID(event->rect().bottomRight());

		QPixmap menuBg(":/images/menu-fond.png");
		QPixmap menuBg2(":/images/menu-fond2.png");
		QPixmap menuTitle(":/images/numbers_title.png");
		QImage numberPixmap(":/images/numbers.png");

		// Translate painter to the first save to be drawn
		if (minSaveID > 0) {
			painter.translate(0, minSaveID * saveHeight());
		}

		for (curSaveID = minSaveID; curSaveID <= maxSaveID && curSaveID < _data->saveCount(); ++curSaveID) {
			if (blackID != curSaveID) {
				SaveData *saveData = _data->getSaves().at(curSaveID);

				QRect sourceRect = (event->rect() & saveRect(curSaveID));
				sourceRect.moveTopLeft(sourceRect.topLeft() - savePoint(curSaveID));

				// Paint save
				renderSave(&painter, saveData,
						   !saveData->isTheLastEdited()
						   && !saveData->isDelete() ? menuBg : menuBg2,
						   menuTitle, numberPixmap,
						   currentSaveIconFrame,
						   sourceRect);

				// Paint cursor hand
				if (cursorID == curSaveID) {
					painter.drawPixmap(-36, 16, QPixmap(":/images/cursor.png"));
				}
			}
			// Drop indicator
			if (dropIndicatorID == curSaveID) {
				if (isExternalDrag) { // Overwrite
					QPen pen(Qt::white, 3);
					painter.setPen(pen);
					painter.setBrush(QBrush());
					painter.drawRect(0, 2, saveWidth(), saveHeight()-4);
				} else { // Insert
					QPen pen(Qt::white, 8);
					painter.setPen(pen);
					painter.drawLine(0, 0, saveWidth(), 0);
				}
			}

			painter.translate(0, saveHeight());
		}
		// Insert indicator after the last item
		if (dropIndicatorID >= curSaveID && curSaveID == _data->saveCount()) {
			if (!isExternalDrag) {
				QPen pen(Qt::white, 8);
				painter.setPen(pen);
				painter.drawLine(0, 0, saveWidth(), 0);
			}
		}
	}
}

void SavecardView::drawFrame(QPainter *painter, int width, int height)
{
	QLine lines[4];
	QPen pen(QColor(41,41,41));

	pen.setWidth(2);
	pen.setJoinStyle(Qt::MiterJoin);
	painter->setPen(pen);
	painter->drawRect(QRect(1, 1, width-2, height-2));

	painter->setPen(QColor(132, 132, 132));
	lines[0] = QLine(4, 2, width-5, 2);
	lines[1] = QLine(4, 3, width-5, 3);
	lines[2] = QLine(2, 4, 2, height-5);
	lines[3] = QLine(3, 4, 3, height-5);
	painter->drawLines(lines, 4);

	painter->setPen(QColor(58, 58, 58));
	lines[0] = QLine(4, height-4, width-3, height-4);
	lines[1] = QLine(4, height-3, width-3, height-3);
	lines[2] = QLine(width-4, 4, width-4, height-5);
	lines[3] = QLine(width-3, 4, width-3, height-5);
	painter->drawLines(lines, 4);

	painter->setPen(QColor(82, 82, 82));
	lines[0] = QLine(width-4, 2, width-3, 2);
	lines[1] = QLine(width-4, 3, width-3, 3);
	lines[2] = QLine(2, height-4, 3, height-4);
	lines[3] = QLine(2, height-3, 3, height-3);
	painter->drawLines(lines, 4);

	painter->setPen(QColor(99, 99, 99));
	painter->drawPoint(QPoint(2, 2));
	painter->drawPoint(QPoint(3, 2));
	painter->drawPoint(QPoint(2, 3));
	painter->drawPoint(QPoint(3, 3));
}

void SavecardView::num2pix(QPainter *painter, QImage *numberPixmap, int x, int y, quint32 num, quint8 space, QChar fill, int color)
{
	colors(numberPixmap, color);
	QString strNum = QString("%1").arg(num, space, 10, fill).right(space);

	for (quint8 i = 0; i < space; ++i)
	{
		QChar c = strNum.at(i);
		if (c.isDigit())
			painter->drawImage(x+16*i, y, numberPixmap->copy(14*c.digitValue(), 0, 14, 16));
	}
}

void SavecardView::colors(QImage *image, int color)
{
	switch (color) {
	case 1://Red
		image->setColorTable(QVector<QRgb>() << qRgb(0xff,0x18,0x18) << qRgb(0x20,0x20,0x20) << qRgb(0x8b,0x18,0x18) << 0);
		return;
	case 2://Blue
		image->setColorTable(QVector<QRgb>() << qRgb(0x6a,0xb4,0xee) << qRgb(0x29,0x29,0x29) << qRgb(0x52,0x73,0x94) << 0);
		return;
	case 3://Yellow
		image->setColorTable(QVector<QRgb>() << qRgb(0xde,0xde,0x08) << qRgb(0x29,0x29,0x29) << qRgb(0x83,0x83,0x18) << 0);
		return;
	case 4://Green
		image->setColorTable(QVector<QRgb>() << qRgb(0x00,0xff,0x00) << qRgb(0x39,0x39,0x31) << qRgb(0x00,0xbd,0x00) << 0);
		return;
	case 5://Purple
		image->setColorTable(QVector<QRgb>() << qRgb(0xff,0x00,0xff) << qRgb(0x29,0x29,0x29) << qRgb(0x94,0x10,0x94) << 0);
		return;
	default:
		image->setColorTable(QVector<QRgb>() << qRgb(0xee,0xee,0xee) << qRgb(0x41,0x41,0x31) << qRgb(0xa4,0xa4,0xa4) << 0);
		return;
	}
}

void SavecardView::mousePressEvent(QMouseEvent *event)
{
	int xStart = horizontalMargin();
	int xEnd = width()-xStart;

	if (event->x() < xStart || event->x() > xEnd) {
		return;
	}

	if (event->button() == Qt::LeftButton) {
		mouseMove = 1;
		startPos = event->pos();
	}
}

void SavecardView::mouseMoveEvent(QMouseEvent *event)
{
	// Change cursor position
	moveCursor(saveID(event->pos()));

	if (!mouseMove || !(event->buttons() & Qt::LeftButton)
			|| (startPos - event->pos()).manhattanLength() < QApplication::startDragDistance())
		return;

	mouseMove = 2;

	SaveData *saveData = _data->getSave(saveID(startPos));
	if (!saveData)	return;

	QDrag *drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData;

	_dragStart = saveData->id();
	mimeData->setData("application/ff8save", saveData->save() + saveData->MCHeader());
	drag->setMimeData(mimeData);
	drag->setHotSpot(QPoint(event->pos().x() - horizontalMargin(), event->pos().y() % saveHeight()));
	QPixmap pixmap(saveSize());
	renderSave(&pixmap, saveData, currentSaveIconFrame);
	drag->setPixmap(pixmap);

	setBlackSave(saveData->id());
	updateSave(saveData->id());
	drag->exec(Qt::MoveAction, Qt::MoveAction);// !Qt event loop blocked on Windows!
	setBlackSave(-1);
	updateSave(saveData->id());
}

void SavecardView::mouseReleaseEvent(QMouseEvent *event)
{
	setBlackSave(-1);
	if (mouseMove == 2) {
		mouseMove = 0;
		return;
	} else {
		mouseMove = 0;
	}

	int xStart = horizontalMargin();
	int xEnd = width()-xStart;

	if (event->x() < xStart || event->x() > xEnd) {
		return;
	}

	SaveData *saveData = _data->getSave(saveID(event->pos()));
	if (!saveData)	return;

	if (event->button() == Qt::LeftButton)
	{
		if (saveData->isFF8()) {
			if (saveData->isDelete())
				restore(saveData->id());
			else
				edit(saveData->id());
		}
		else
		{
			if (saveData->isDelete()) {
				contextMenuEvent(new QContextMenuEvent(QContextMenuEvent::Other, event->pos(), event->globalPos(), event->modifiers()));
				return;
			} else if (!saveData->isRaw()) {
				properties(saveData->id());
			}
		}
	}
	else if (event->button() == Qt::MiddleButton)
	{
		if (saveData->isDelete()) {
			contextMenuEvent(new QContextMenuEvent(QContextMenuEvent::Other, event->pos(), event->globalPos(), event->modifiers()));
			return;
		} else if (!saveData->isRaw()) {
			properties(saveData->id());
		}
	}
}

void SavecardView::contextMenuEvent(QContextMenuEvent *event)
{
	int xStart = horizontalMargin();
	int xEnd = width()-xStart;

	if (event->x() < xStart || event->x() > xEnd) {
		return;
	}

	SaveData *saveData = _data->getSave(saveID(event->pos()));
	if (!saveData)	return;

	startPos = event->pos();

	QMenu menu(this);
	if (!saveData->isDelete() && saveData->isFF8()) {
		menu.setDefaultAction(menu.addAction(tr("&Modifier..."), this, SLOT(edit())));
		menu.addAction(tr("&Exporter cette sauvegarde..."), this, SLOT(exportOne()));
	}
	menu.addAction(tr("&Nouvelle partie"), this, SLOT(newGame()));
	if (!saveData->isDelete()) {
		menu.addAction(tr("&Vider"), this, SLOT(removeSave()));
		if (!saveData->isRaw()) {
			QAction *action = menu.addAction(tr("&Propriétés..."), this, SLOT(properties()));
			if (!saveData->isFF8()) {
				menu.setDefaultAction(action);
			}
		}
	}
	menu.exec(event->globalPos());
}

void SavecardView::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::LanguageChange) {
		update();
		event->accept();
	} else {
		QWidget::changeEvent(event);
	}
}

void SavecardView::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("application/ff8save")) {
		isExternalDrag = event->source() != this; // external source
		event->acceptProposedAction();
	}
}

void SavecardView::dragMoveEvent(QDragMoveEvent *event)
{
	int saveID = this->saveID(event->pos());
	if (saveID >= 0) {
		setDropIndicator(saveID + int(event->pos().y() % saveHeight() > saveHeight()/2));
		_parent->scrollToDrag(event->pos());
	}
}

void SavecardView::dragLeaveEvent(QDragLeaveEvent *)
{
	setDropIndicator(-1);
}

void SavecardView::dropEvent(QDropEvent *event)
{
	lastDropID = dropIndicatorID;
	dragLeaveEvent(0);

	event->acceptProposedAction();
	isExternalDrag = event->source() != this;
	if (isExternalDrag) {
		if (lastDropData)	delete lastDropData;
		lastDropData = new QByteArray(event->mimeData()->data("application/ff8save"));
	}
	QTimer::singleShot(0, this, SLOT(drop()));// defer function call
}

void SavecardView::drop()
{
	if (isExternalDrag) {
		replaceSaveData(lastDropID, *lastDropData);
		delete lastDropData;
		lastDropData = 0;
	} else {
		moveDraggedSave(lastDropID);
	}
}
