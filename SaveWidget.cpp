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

#include "SaveWidget.h"
#include "HeaderDialog.h"

SaveWidget::SaveWidget(SaveData *saveData, Savecard *savecard, QWidget *parent) :
	QWidget(parent), saveData(saveData), _savecard(savecard), mouseMove(0), saved(false), hovered(false),
	blackView(false), hasDragEvent(false), hasDragEventTop(false), hasDragEventBottom(false)
{
	saveIcon = new SaveIcon(saveData->saveIcon());
	connect(saveIcon, SIGNAL(nextIcon(QPixmap)), SLOT(refreshIcon()));
	setAcceptDrops(true);
}

void SaveWidget::hideCursor()
{
	hovered = false;
	update(width()/2 - 372, 16, 48, 30);
}

void SaveWidget::setDropIndicatorIsVisible(bool onTop, bool isVisible)
{
	bool hasDragEventTop, hasDragEventBottom;

	if(isVisible) {
		hasDragEventTop = onTop;
		hasDragEventBottom = !onTop;
	} else {
		hasDragEventTop = hasDragEventBottom = false;
	}

	if(hasDragEventTop != this->hasDragEventTop || hasDragEventBottom != this->hasDragEventBottom) {
		this->hasDragEventTop = hasDragEventTop;
		this->hasDragEventBottom = hasDragEventBottom;
		update();
	}
}

void SaveWidget::setSaved()
{
	saved = true;
//	update(width()/2 - 374 + 38+656, 3, 16, 16);
}

void SaveWidget::setSaveData(SaveData *saveData)
{
	this->saveData = saveData;
}

void SaveWidget::setSavecard(Savecard *savecard)
{
	_savecard = savecard;
}

QSize SaveWidget::sizeHint() const
{
	return QSize(672, 106);
}

QSize SaveWidget::minimumSizeHint() const
{
	return QSize(672, 106);
}

void SaveWidget::enterEvent(QEvent *event)
{
	hovered = true;
	_savecard->moveCursor(saveData->id());
	update(width()/2 - 372, 16, 48, 30);

	event->accept();
}

void SaveWidget::mousePressEvent(QMouseEvent *event)
{
	int xStart = (width() - sizeHint().width())/2;
	int xEnd = xStart + sizeHint().width();

	if(event->x() < xStart || event->x() > xEnd) {
		return;
	}

	if(event->button() == Qt::LeftButton) {
		mouseMove = 1;
		startPos = event->pos();
		event->accept();
	}
}

void SaveWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(!mouseMove || !(event->buttons() & Qt::LeftButton)
			|| (startPos - event->pos()).manhattanLength() < QApplication::startDragDistance())
		return;

	mouseMove = 2;

	event->accept();

	QDrag *drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData;
	QRect rectWidget(QPoint((width() - sizeHint().width())/2, 0), sizeHint());

	_savecard->setDragStart(saveData->id());
	mimeData->setData("application/ff8save", saveData->save() + saveData->MCHeader());
	drag->setMimeData(mimeData);
	drag->setHotSpot(event->pos() - rectWidget.topLeft());
	QPixmap pixmap(rectWidget.size());
	hideCursor();
	render(&pixmap, QPoint(), rectWidget);
	drag->setPixmap(pixmap);

	blackView = true;
	update();
	drag->exec(Qt::MoveAction, Qt::MoveAction);
	blackView = false;
}

void SaveWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if(blackView) {
		blackView = false;
		update();
	}
	if(mouseMove == 2) {
		mouseMove = 0;
		return;
	} else {
		mouseMove = 0;
	}

	int xStart = (width() - sizeHint().width())/2;
	int xEnd = xStart + sizeHint().width();

	if(event->x() < xStart || event->x() > xEnd) {
		return;
	}

	if(event->button() == Qt::LeftButton)
	{
		event->accept();
		if(saveData->isFF8()) {
			if(saveData->isDelete())
				restore();
			else
				emit released(saveData);
		}
		else
		{
			if(saveData->isDelete()) {
				contextMenuEvent(new QContextMenuEvent(QContextMenuEvent::Other, event->pos(), event->globalPos(), event->modifiers()));
				return;
			}
			properties();
		}
	}
	else if(event->button() == Qt::MidButton)
	{
		event->accept();
		if(saveData->isDelete()) {
			contextMenuEvent(new QContextMenuEvent(QContextMenuEvent::Other, event->pos(), event->globalPos(), event->modifiers()));
			return;
		}
		properties();
	}
}

void SaveWidget::contextMenuEvent(QContextMenuEvent *event)
{
	event->accept();
	QMenu menu(this);
	if(!saveData->isDelete() && saveData->isFF8())
		menu.addAction(tr("Exporter en sauv. PC..."), this, SLOT(exportPC()));
	menu.addAction(tr("Nouvelle partie"), this, SLOT(newGame()));
	if(!saveData->isDelete()) {
		menu.addAction(tr("Vider"), this, SLOT(removeSave()));
		menu.addAction(tr("Propriétés..."), this, SLOT(properties()));
	}
	menu.exec(event->globalPos());
}

void SaveWidget::changeEvent(QEvent *event)
{
	if(event->type() == QEvent::LanguageChange) {
		update();
		event->accept();
	} else {
		QWidget::changeEvent(event);
	}
}

void SaveWidget::dragEnterEvent(QDragEnterEvent *event)
{
	if(event->mimeData()->hasFormat("application/ff8save")) {
		hasDragEvent = event->source() == 0; // external source
		update();
		event->acceptProposedAction();
	}
}

void SaveWidget::dragMoveEvent(QDragMoveEvent *event)
{
	if(event->source() != 0) {
		if(event->pos().y() < height() / 2) {
			hasDragEventTop = true;
			hasDragEventBottom = false;
			_savecard->setDropIndicatorIsVisible(saveData->id()-1, false, true);
			_savecard->setDropIndicatorIsVisible(saveData->id()+1, true, false);
		} else {
			hasDragEventTop = false;
			hasDragEventBottom = true;
			_savecard->setDropIndicatorIsVisible(saveData->id()-1, false, false);
			_savecard->setDropIndicatorIsVisible(saveData->id()+1, true, true);
		}

		update();
	}
	_savecard->scrollToDrag(saveData->id(), event->pos());
}

void SaveWidget::dragLeaveEvent(QDragLeaveEvent */*event*/)
{
	bool needUpdate = hasDragEvent || hasDragEventTop || hasDragEventBottom;

	draggedID = hasDragEventBottom ? saveData->id() + 1 : saveData->id();

	if(hasDragEventTop) {
		_savecard->setDropIndicatorIsVisible(saveData->id()-1, false, false);
	}
	if(hasDragEventBottom) {
		_savecard->setDropIndicatorIsVisible(saveData->id()+1, true, false);
	}

	if(hasDragEvent) {
		hasDragEvent = false;
	}
	if(hasDragEventTop) {
		hasDragEventTop = false;
	}
	if(hasDragEventBottom) {
		hasDragEventBottom = false;
	}

	if(needUpdate) {
		update();
	}
}

void SaveWidget::dropEvent(QDropEvent *event)
{
	dragLeaveEvent(0);

	event->acceptProposedAction();
	lastIsExternal = event->source() == 0;
	if(lastIsExternal) {
		lastDropData = event->mimeData()->data("application/ff8save");
	}
	QTimer::singleShot(0, this, SLOT(emitDropped()));// defer function call
}

void SaveWidget::emitDropped()
{
	if(lastIsExternal) {
		_savecard->replaceSaveData(saveData->id(), lastDropData);
		lastDropData = QByteArray();
	} else {
		_savecard->moveDraggedSave(draggedID);
	}
}

void SaveWidget::exportPC()
{
	QString path;
	int index;

	if(Config::value("savePath").isEmpty())
	{
		index = _savecard->path().lastIndexOf('/');
		if(index != -1)		path = _savecard->path().left(index+1);
	}
	else
	{
		path = Config::value("savePath") % "/";
	}

	path = QFileDialog::getSaveFileName(this, tr("Exporter"), path % QString("save%1").arg(saveData->id()+1, 2, 10, QChar('0')), tr("FF8 PC save (*)"));
	if(path.isEmpty())		return;
	
	index = path.lastIndexOf('/');
	Config::value("savePath") = index == -1 ? path : path.left(index);
	
	if(!saveData->exportPC(path))
		QMessageBox::warning(this, tr("Échec"), tr("Enregistrement échoué, vérifiez que le fichier cible n'est pas utilisé."));
}

void SaveWidget::newGame()
{
	if(!saveData->isDelete()) {
		QMessageBox::StandardButton b = QMessageBox::question(this, tr("Nouvelle partie"), tr("Tout le contenu de la sauvegarde sera remplacé par une nouvelle partie.\nContinuer ?"), QMessageBox::Yes | QMessageBox::No);
		if(b != QMessageBox::Yes) {
			return;
		}
	}

	QFile newGameFile(":/data/newGame");
	if(!newGameFile.open(QIODevice::ReadOnly)) {
		qWarning() << "failed to open data/newGame" << newGameFile.errorString();
		return;
	}

	if(saveData->hasMCHeader() && (!saveData->isFF8() || saveData->isDelete())) {
		HeaderDialog dialog(saveData, this, HeaderDialog::RestoreView);
		if(dialog.exec()==QDialog::Accepted)
		{
			saveData->restore();
		}
		else
		{
			return;
		}
	}

	saveData->open(newGameFile.readAll(), saveData->MCHeader());
	saveData->setModified(true);
	emit changed();
	newGameFile.close();

	update();
}

void SaveWidget::removeSave()
{
	if(!saveData->isDelete()) {
		QMessageBox::StandardButton b = QMessageBox::question(this, tr("Vider"), tr("Tout le contenu de la sauvegarde sera vidé.\nContinuer ?"), QMessageBox::Yes | QMessageBox::No);
		if(b != QMessageBox::Yes) {
			return;
		}
	}

	saveData->remove();
	emit changed();

	update();
}

void SaveWidget::properties()
{
	HeaderDialog dialog(saveData, this);

	if(dialog.exec()==QDialog::Accepted)
		emit changed();
}

void SaveWidget::restore()
{
	int reponse = QMessageBox::question(this, tr("Sauvegarde supprimée"), tr("Cette sauvegarde a été supprimée, voulez-vous tenter de la réparer ? (succès non garanti)"), tr("Oui"), tr("Non"));
	if(reponse != 0)  return;

	HeaderDialog dialog(saveData, this, HeaderDialog::RestoreView);

	if(dialog.exec()==QDialog::Accepted)
	{
		saveData->restore();

		emit changed();
	}
}

void SaveWidget::refreshIcon()
{
	qDebug() << "salut";
	if(!saveData->isDelete()) {
		repaint(width()/2 - 372 + 36 + 36, 43, 16, 16);
	}
}

void SaveWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	int xStart = (width() - sizeHint().width())/2;

	if(!blackView) {
		painter.translate(xStart, 0);

		painter.setBrush(QBrush(QPixmap(QString(":/images/menu-fond%1.png").arg(!saveData->isTheLastEdited() && !saveData->isDelete() ? "" : "2"))));
		drawFrame(&painter, 672, 106);

		QString title = QString("%1").arg(saveData->id()+1, 2, 10, QChar('0'));
		QPixmap fontPixmap(":/images/numbers_title.png");
		painter.drawPixmap(4, 0, fontPixmap.copy(title.at(0).digitValue()*16, 0, 16, 22));
		painter.drawPixmap(20, 0, fontPixmap.copy(title.at(1).digitValue()*16, 0, 16, 22));

		if(saveData->isFF8())
		{
			if(saveData->descData().party[0] != 255)
				painter.drawPixmap(44, 4, QPixmap(QString(":/images/icons/perso%1.png").arg(saveData->descData().party[0] & 15)));
			if(saveData->descData().party[1] != 255)
				painter.drawPixmap(112, 4, QPixmap(QString(":/images/icons/perso%1.png").arg(saveData->descData().party[1] & 15)));
			if(saveData->descData().party[2] != 255)
				painter.drawPixmap(180, 4, QPixmap(QString(":/images/icons/perso%1.png").arg(saveData->descData().party[2] & 15)));

			int persoIndex = saveData->descData().party[0] != 255 ? saveData->descData().party[0] : (saveData->descData().party[1] != 255 ? saveData->descData().party[1] : saveData->descData().party[2]);
			bool langIndep = persoIndex==SQUALL || persoIndex==RINOA || persoIndex==GRIEVER || persoIndex==BOKO || persoIndex==ANGELO;
			FF8Text::drawTextArea(&painter, QPoint(271, 8), saveData->perso(persoIndex), langIndep ? (saveData->isJp() ? 2 : 1) : 0);
			FF8Text::drawTextArea(&painter, QPoint(271, 36), tr("NV%1").arg(saveData->descData().nivLeader,3,10,QChar(' ')), 1);

			QImage numberPixmap(":/images/numbers.png");

			QPixmap disc(QString(":/images/disc_%1.png").arg(Config::value("lang")=="fr" ? "fr" : "en"));
			painter.drawPixmap(391, 38, disc);
			num2pix(&painter, &numberPixmap, 395+disc.width(), 38, saveData->descData().disc+1);
			painter.drawPixmap(511, 16, QPixmap(QString(":/images/play_%1.png").arg(Config::value("lang")=="fr" ? "fr" : "en")));

			int hour = Config::hour(saveData->descData().time, saveData->freqValue());
			int color = (hour>=100) + (hour>=200) + (hour>=300) + (hour>=400) + (hour>=500);
			num2pix(&painter, &numberPixmap, 576, 16, hour, 2, QChar(' '), color);

			QImage deux_points(":/images/deux-points.png");
			colors(&deux_points, color);
			painter.drawPixmap(612, 18, QPixmap::fromImage(deux_points));
			num2pix(&painter, &numberPixmap, 624, 16, Config::min(saveData->descData().time, saveData->freqValue()), 2, QChar('0'), color);
			painter.drawPixmap(640, 44, QPixmap(":/images/gils.png"));
			num2pix(&painter, &numberPixmap, 511, 40, saveData->descData().gils, 8);

			painter.translate(256, 62);
			painter.setBrush(QBrush());
			drawFrame(&painter, 416, 44);

			FF8Text::drawTextArea(&painter, QPoint(12, 12), saveData->descData().locationID<251 ? Data::locations.at(saveData->descData().locationID) : QString("??? (%1)").arg(saveData->descData().locationID));
		}
		else
		{
			if(saveData->isDelete())
			{
				FF8Text::drawTextArea(&painter, QPoint(36, 43), tr("Bloc Disponible"));
			}
			else
			{
				painter.drawPixmap(36, 43, saveIcon->pixmap());
				QString short_desc = saveData->getShortDescription();
				if(!short_desc.isEmpty())
				{
					painter.setPen(Qt::white);
					painter.drawText(68, 59, short_desc);
				}
			}
		}

		painter.resetTransform();
		painter.translate(xStart-36, 0);

		if(!saveData->isFF8() && !saveData->isDelete()) {
			painter.drawPixmap(72, 43, saveIcon->pixmap());
		}
		if(saveData->isFF8() && saved) { //TODO: remove
			painter.drawPixmap(692, 3, QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton).pixmap(16));
		}
		if(hovered) {
			painter.drawPixmap(0, 16, QPixmap(":/images/cursor.png"));
		}
	}

	painter.resetTransform();
	painter.translate(xStart, 0);

	if(hasDragEvent) {
		QPen pen(Qt::white, 3);
		painter.setPen(pen);
		painter.setBrush(QBrush());
		painter.drawRect(0, 2, 672, 102);
	} else if(hasDragEventTop && saveData->id() > 0) {
		QPen pen(Qt::white, 4);
		painter.setPen(pen);
		painter.setBrush(QBrush());
		painter.drawLine(0, 2, 672, 2);
	} else if(hasDragEventBottom && saveData->id() < _savecard->getSaves().size()-1) {
		QPen pen(Qt::white, 4);
		painter.setPen(pen);
		painter.setBrush(QBrush());
		painter.drawLine(0, 104, 672, 104);
	}

	painter.end();
}

void SaveWidget::drawFrame(QPainter *painter, int width, int height)
{
	QPen pen(QColor(41,41,41));
	pen.setWidth(2);
	pen.setJoinStyle(Qt::MiterJoin);
	painter->setPen(pen);
	painter->drawRect(1, 1, width-2, height-2);

	painter->setPen(QColor(132,132,132));
	painter->drawLine(4, 2, width-5, 2);
	painter->drawLine(4, 3, width-5, 3);
	painter->drawLine(2, 4, 2, height-5);
	painter->drawLine(3, 4, 3, height-5);

	painter->setPen(QColor(58,58,58));
	painter->drawLine(4, height-4, width-3, height-4);
	painter->drawLine(4, height-3, width-3, height-3);
	painter->drawLine(width-4, 4, width-4, height-5);
	painter->drawLine(width-3, 4, width-3, height-5);

	painter->setPen(QColor(99,99,99));
	painter->drawLine(2, 2, 3, 2);
	painter->drawLine(2, 3, 3, 3);

	painter->setPen(QColor(82,82,82));
	painter->drawLine(width-4, 2, width-3, 2);
	painter->drawLine(width-4, 3, width-3, 3);
	painter->drawLine(2, height-4, 3, height-4);
	painter->drawLine(2, height-3, 3, height-3);
}

void SaveWidget::num2pix(QPainter *painter, QImage *numberPixmap, int x, int y, quint32 num, quint8 space, QChar fill, int color)//Dessine les nombres pour l'affichage des gils et du temps
{
	colors(numberPixmap, color);
	QString strNum = QString("%1").arg(num, space, 10, fill).right(space);

	for(quint8 i=0 ; i<space ; ++i)
	{
		QChar c = strNum.at(i);
		if(c.isDigit())
			painter->drawImage(x+16*i, y, numberPixmap->copy(14*c.digitValue(), 0, 14, 16));
	}
}

void SaveWidget::colors(QImage *image, int color)
{
	switch(color) {
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
