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

#include "Savecard.h"
#include "GZIP.h"
#include "Parameters.h"

Savecard::Savecard(const QString &path, QWidget *parent, bool slot) :
	QListWidget(parent), _ok(true), _hasPath(true), _dragStart(-1), start(0), notify(true), _isModified(false)
{
	setWidget();

	if(slot)
	{
		setPath(QDir::fromNativeSeparators(QDir::cleanPath(path)) + "/");
		setType(PcDir);

		directory();
	}
	else
	{
		setPath(QDir::fromNativeSeparators(QDir::cleanPath(path)));
		QString extension = this->extension();

		if(extension.isEmpty())
		{
			setType(Pc);
			if(!pc())	addSave(QByteArray());
		}
		else if(extension == "mcr" || extension == "ddf" || extension == "mc"
		   || extension == "mcd"|| extension == "mci" || extension == "ps"
		   || extension == "psm" || extension == "vm1")
		{
			setType(Ps);
			_ok = ps();
		}
		else if(extension == "mem" || extension == "vgs")
		{
			setType(Vgs);
			_ok = ps();
		}
		else if(extension == "gme")
		{
			setType(Gme);
			_ok = ps();
		}
		else if(extension == "vmp")
		{
			setType(Vmp);
			_ok = ps();
		}
		else if(extension == "psv")
		{
			setType(Psv);
			_ok = ps3();
			if(!_ok) {
				_ok = sstate_pSX();
				setType(Undefined);
			}
		}
		else if(extension == "000"
				|| extension == "001"
				|| extension == "002"
				|| extension == "003"
				|| extension == "004")
		{
			_ok = sstate_ePSXe();
			setType(Undefined);
		}
		else
		{
			QMessageBox::StandardButton rep = QMessageBox::question(this, tr("Erreur"), tr("Fichier de type inconnu.\nVoulez-vous l'analyser pour obtenir le bon format ?"), QMessageBox::Yes | QMessageBox::No);
			if(rep != QMessageBox::Yes) {
				_ok = false;
				return;
			}

			_ok = getFormatFromRaw();
		}

		if(_ok && (_type == Psv || _type == Vmp))
			QMessageBox::information(this, tr("Sauvegarde hasardeuse"), tr("Le format %1 est protégé, l'enregistrement sera partiel et risque de ne pas fonctionner.").arg(extension));
	}
	connect(&fileWatcher, SIGNAL(fileChanged(QString)), SLOT(notifyFileChanged(QString)));

//	compare(saves.at(0)->save(), saves.at(1)->save());
//	compare(saves.at(10)->save(), saves.at(11)->save());
}

Savecard::Savecard(int saveCount, QWidget *parent) :
	QListWidget(parent), _ok(true), _hasPath(false), _dragStart(-1), start(0), notify(true), _isModified(false)
{
	setWidget();
	setType(Undefined);

	for(int i=0 ; i<saveCount ; ++i) {
		addSave();
	}
}

Savecard::~Savecard()
{
	foreach(SaveData *save, saves)	delete save;
}

void Savecard::setWidget()
{
	setPalette(QPalette(Qt::black));
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	setSelectionMode(QAbstractItemView::NoSelection);
	setFrameShape(QFrame::NoFrame);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setUniformItemSizes(true);
	// Drag & drop
//	setDragEnabled(true);
//	setDragDropMode(QAbstractItemView::InternalMove);
//	viewport()->setAcceptDrops(true);
//	setDropIndicatorShown(true);
}

QString Savecard::dirname() const
{
	int index = _path.lastIndexOf('/');
	return index != -1 ? _path.left(index + 1) : QString();
}

QString Savecard::name() const
{
	return _path.mid(_path.lastIndexOf('/') + 1);
}

QString Savecard::extension() const
{
	int index = _path.lastIndexOf('.');
	return index != -1 ? _path.mid(index + 1).toLower() : QString();
}

const QString &Savecard::path() const
{
	return _path;
}

void Savecard::setPath(const QString &path)
{
	_path = path;
}

void Savecard::setName(const QString &name)
{
	_path = dirname() + name;
}

bool Savecard::ok() const
{
	return _ok;
}

Savecard::Type Savecard::type() const
{
	return _type;
}

void Savecard::setType(Type type)
{
	switch(type) {
	case Vgs:
		start = 64;
		break;
	case Gme:
		start = 3904;
		break;
	case Vmp:
		start = 128;
		break;
	case Undefined:
		start = 0;
		_hasPath = false;
		setPath(tr("Sans nom"));
		setModified(true);
		break;
	default:
		start = 0;
		break;
	}

	_type = type;
}

bool Savecard::hasPath() const
{
	return _hasPath;
}

bool Savecard::isOneSaveType() const
{
	return type()==Pc || type()==Psv;
}

bool Savecard::isModified() const
{
	if(_isModified)	return true;

	foreach(SaveData *save, saves) {
		if(save->isModified())	return true;
	}
	return false;
}

void Savecard::setModified(bool modified)
{
	_isModified = modified;

	foreach(SaveData *save, saves) {
		save->setModified(modified);
	}
}

void Savecard::setSlotOrder(const QList<int> &order)
{
	if(count() != order.size())		return;

	QList<SaveData *> newSaves;

	foreach(int i, order)
	{
		newSaves.append(saves.at(i));
	}

	saves = newSaves;

	int i=0;
	foreach(SaveData *saveData, saves) {
		saveData->setId(i);
		saveWidget(i)->setSaveData(saveData);
		++i;
	}

	_isModified = true;
}

void Savecard::notifyFileChanged(const QString &path)
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

SaveWidget *Savecard::saveWidget(int row) const
{
	return (SaveWidget *)itemWidget(item(row));
}

void Savecard::moveCursor(int row)
{
	for(int i=0 ; i<row ; ++i) {
		saveWidget(i)->hideCursor();
	}

	for(int i=row+1 ; i<count() ; ++i) {
		saveWidget(i)->hideCursor();
	}
}

void Savecard::updateSaveWidget(int saveID, bool saved, bool allUpdate)
{
	if(!allUpdate) {
		foreach(SaveData *save, saves) {
			if(save->isTheLastEdited()) {
				save->setIsTheLastEdited(false);
				this->saveWidget(save->id())->update();
			}
		}
		saves.at(saveID)->setIsTheLastEdited(true);
	}
	SaveWidget *saveWidget = this->saveWidget(saveID);
	if(saved)	saveWidget->setSaved();
	saveWidget->update();
}

void Savecard::updateSaveWidgets()
{
	int countItems = count();
	for(int i=0 ; i<countItems ; ++i)
		updateSaveWidget(i, false, true);
}

bool Savecard::ps()
{
	QFile fic(_path);

	if(!fic.exists())
	{
		QMessageBox::warning(this, tr("Erreur"), tr("Le fichier n'existe plus.\n%1").arg(_path));
		return false;
	}
	if(!fic.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, tr("Erreur"), tr("Le fichier est protégé en lecture."));
		return false;
	}
	if(fic.size() < start+131072)//start+8192*16
	{
		QMessageBox::warning(this, tr("Erreur"), tr("Fichier trop court"));
		return false;
	}

	fic.seek(start+128);
	QByteArray header = fic.read(1920);//(128*15)
	fic.seek(start+SAVE_SIZE);

	for(quint8 i=0 ; i<15 ; ++i)
		addSave(fic.read(SAVE_SIZE), header.mid(128*i, 127));

	if(fileWatcher.files().size()<30)
		fileWatcher.addPath(_path);

	return true;
}

bool Savecard::ps3()
{
	QFile fic(_path);

	if(!fic.exists())
	{
		QMessageBox::warning(this, tr("Erreur"), tr("Le fichier n'existe plus.\n%1").arg(_path));
		return false;
	}
	if(!fic.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, tr("Erreur"), tr("Le fichier est protégé en lecture."));
		return false;
	}

	// Check format
	fic.seek(1);
	if(QString(fic.peek(3)) != "VSP" && QString(fic.peek(6)) == "RS2CPU")
		return false;

	if(fic.size() < 8324)
	{
		QMessageBox::warning(this, tr("Erreur"), tr("Fichier invalide"));
		return false;
	}

	fic.seek(90);
	QByteArray header = fic.read(30);
	fic.seek(132);

	addSave(fic.read(SAVE_SIZE), header);

	if(fileWatcher.files().size()<30)
		fileWatcher.addPath(_path);

	return true;
}

bool Savecard::pc()
{
	int tailleC;
	QFile f(_path);

	if(!f.exists() || !f.open(QIODevice::ReadOnly))
		return false;

	f.read((char *)&tailleC, 4);
	if(tailleC != f.size()-4)		return false;

	addSave(LZS::decompress(f.readAll(), FF8SAVE_SIZE));

//	if(fileWatcher.files().size()<30)
//		fileWatcher.addPath(_path);

	return true;
}

bool Savecard::getFormatFromRaw()
{
	QFile f(_path);
	start = 0;

	if(!f.exists() || !f.open(QIODevice::ReadOnly))
		return false;

	QByteArray data = f.read(10000);

	int indexMC = data.indexOf("MC");
	if(indexMC != -1) { // Maybe Memory Card format
		start = indexMC;
		switch(indexMC) {
		case 0:
			_type = Ps;
			break;
		case 64:
			_type = Vgs;
			break;
		case 128:
			_type = Vmp;
			break;
		case 3904:
			_type = Gme;
			break;
		default:
			_type = Unknown;
			break;
		}
		return ps();
	} else { // Maybe PC format
		// compressed?
		quint32 lzsSize;
		memcpy(&lzsSize, data.constData(), 4);
		if(lzsSize + 4 == f.size()) {
			const QByteArray &unLzsed = LZS::decompress(data.mid(4), 2);
			if(unLzsed.startsWith("SC")) {
				_type = Pc;
				return pc();
			}
			return false;
		}

		if(data.indexOf("VSP") == 1) {
			_type = Psv;
			return ps3();
		}
	}

	return false;
}

bool Savecard::sstate_ePSXe()
{
	QTemporaryFile temp;
	if(!temp.open()) {
		QMessageBox::warning(this, tr("Erreur"), tr("Impossible de créer le fichier temporaire."));
		return false;
	}
	if(!GZIP::decompress(_path, temp.fileName())) {
		QMessageBox::warning(this, tr("Erreur"), tr("Impossible de décompresser le fichier."));
		return false;
	}
	temp.seek(7);
	QString serial(temp.read(11));

	qDebug() << serial;

	temp.seek(0x779CC);
	QByteArray data;
	data.append("SC");
	data.append(QByteArray(0x180, '\0'));
	data.append(temp.read(0x139E));

	if(data.size() != 0x139E + 0x182)	return false;

	addSave(data);

	return true;
}

bool Savecard::sstate_pSX()
{
	QFile f(_path);
	if(!f.open(QIODevice::ReadOnly)) {
		QMessageBox::warning(this, tr("Erreur"), tr("Le fichier est protégé en lecture."));
		return false;
	}

	f.seek(0x77AC2);
	QByteArray data;
	data.append("SC");
	data.append(QByteArray(0x180, '\0'));
	data.append(f.read(0x139E));

	if(data.size() != 0x139E + 0x182)	return false;

	addSave(data);

	return true;
}

void Savecard::directory()
{
	QTime t;t.start();
	for(quint8 i=0 ; i<30 ; ++i)
	{
		setName(QString("save%1").arg(i+1, 2, 10, QChar('0')));
		if(!pc())	addSave();
	}
//	qDebug() << "time: " << t.elapsed();
}

void Savecard::addSave(const QByteArray &data, const QByteArray &header)
{
	SaveData *saveData = new SaveData(count(), data, header, type()==Psv);
	saves.append(saveData);

	QListWidgetItem *item = new QListWidgetItem(this);
	item->setFlags(Qt::NoItemFlags);

	SaveWidget *saveWidget = new SaveWidget(saveData, this);
	item->setSizeHint(saveWidget->sizeHint());
	setItemWidget(item, saveWidget);

	connect(saveWidget, SIGNAL(released(SaveData*)), parent(), SLOT(editView(SaveData*)));
	connect(saveWidget, SIGNAL(changed()), parent(), SLOT(setModified()));
}

void Savecard::setDragStart(int saveID)
{
	_dragStart = saveID;
}

void Savecard::swapDraggedAndDropped(int saveID, const QByteArray &mimeData, bool isExternal)
{
	if(_dragStart != saveID
			&& saveID >=0 && saveID < saves.size()) {
		if(!isExternal && _dragStart >= 0 && _dragStart < saves.size()) {
			qDebug() << "Savecard::swapDraggedAndDropped" << _dragStart << saveID;
			saves.swap(_dragStart, saveID);
			SaveData *saveData = saves.at(_dragStart);
			saveData->setId(_dragStart);
			saveWidget(_dragStart)->setSaveData(saveData);
			saveData = saves.at(saveID);
			saveData->setId(saveID);
			saveWidget(saveID)->setSaveData(saveData);
			updateSaveWidget(_dragStart, false, true);
			updateSaveWidget(saveID, false, true);

			_isModified = true;
			emit modified();
		} else {
			if(!mimeData.isEmpty()) {
				SaveData *saveData = saves.at(saveID);

				if(!saveData->isDelete()) {
					QMessageBox::StandardButton answer = QMessageBox::question(this, tr("Écraser"), tr("Tout le contenu de la sauvegarde sera écrasé.\nContinuer ?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
					if(answer != QMessageBox::Yes) {
						_dragStart = -1;
						return;
					}
				}

				qDebug() << "Savecard::swapDraggedAndDropped useMimeData" << saveID << mimeData.size();

				if(mimeData.size() > SAVE_SIZE) {
					saveData->open(mimeData.left(SAVE_SIZE), mimeData.mid(SAVE_SIZE));
				} else if(mimeData.size() == SAVE_SIZE) {
					QByteArray MCHeader;
					if(saveData->hasMCHeader()) {
						HeaderDialog dialog(saveData, this, HeaderDialog::CreateView);

						if(dialog.exec() != QDialog::Accepted) {
							_dragStart = -1;
							return;
						}
						MCHeader = saveData->MCHeader();
					}
					saveData->open(mimeData, MCHeader);
				}
				updateSaveWidget(saveID, false, true);

				_isModified = true;
				emit modified();
			}
		}
	}

	_dragStart = -1;
}

void Savecard::scrollToDrag(int saveID, const QPoint &pos)
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

const QList<SaveData *> &Savecard::getSaves() const
{
	return saves;
}

bool Savecard::save(const QString &saveAs, Type newType)
{
	QString path = saveAs.isEmpty() ? _path : saveAs;
	QTemporaryFile temp("hyne");
	QFile fic(_path);

	if(saveAs.isEmpty()) {
		newType = _type;
	}

	if(!fic.exists())
	{
		QMessageBox::warning(this, tr("Erreur"), tr("Le fichier n'existe plus.\n%1").arg(_path));
		return false;
	}
	if(!fic.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, tr("Erreur"), tr("Le fichier est protégé en lecture.\n%1").arg(_path));
		return false;
	}
	if(!temp.open())
	{
		fic.close();
		QMessageBox::warning(this, tr("Erreur"), tr("Impossible de créer un fichier temporaire"));
		return false;
	}

	if(_type==Psv)
	{
		temp.write(fic.read(100));
		temp.write(saves.first()->MCHeader().mid(10, 20));// B + country + code + id
		fic.seek(120);
		temp.write(fic.read(12));
//		compare(fic.peek(FF8SAVE_SIZE), saves.first()->save());
		temp.write(saves.first()->save());
	}
	else if(_type!=Pc && _type!=PcDir)
	{
		bool abort;
		quint8 i;
		SaveData *save;

		QByteArray head = header(&fic, newType, !saveAs.isEmpty(), &abort);
		if(abort) return false;

		temp.write(head);

		temp.write("MC", 2);//MC
		temp.write(QByteArray(125,'\x00'));
		temp.putChar('\x0E');//xor byte

		fic.seek(start+128);

		for(i=0 ; i<15 ; ++i)
		{
			save = saves.at(i);

			if(save->hasMCHeader())
			{
				temp.write(save->saveMCHeader());//128
				fic.seek(fic.pos() + 128);
			}
			else
			{
				temp.write(fic.read(128));//Main header
			}
		}

		temp.write(fic.read(6144));//Padding (8192-16*128)

		for(i=0 ; i<15 ; ++i)
		{
			save = saves.at(i);

//			compare(fic.peek(FF8SAVE_SIZE), save->save());
			temp.write(save->save());
			fic.seek(fic.pos() + SAVE_SIZE);
		}
	}

	fic.close();

	bool readdPath = false;
	if(fileWatcher.files().contains(path))
	{
		readdPath = true;
		fileWatcher.removePath(path);
	}

	if(QFile::exists(path) && !QFile::remove(path))
	{
		temp.close();
		QMessageBox::warning(this, tr("Erreur"), tr("Impossible de supprimer le fichier !\n%1\nÉchec de la sauvegarde.\nVérifiez que le fichier n'est pas utilisé par un autre programme.").arg(path));
		if(readdPath)	fileWatcher.addPath(path);
		return false;
	}
	if(!temp.copy(path))
	{
		QMessageBox::warning(this, tr("Erreur"), tr("Échec de la sauvegarde."));
	}
	if(readdPath)	fileWatcher.addPath(path);

	return true;
}

bool Savecard::saveOne(qint8 id, QString path)
{
	if(path.isEmpty())
		path = _path;

	QTemporaryFile temp("hyneOne");
	if(!temp.open())
	{
		QMessageBox::warning(this, tr("Erreur"), tr("Impossible de créer un fichier temporaire"));
		return false;
	}

	if(id==-1)	id = 0;

	bool readdPath = false;
	if(fileWatcher.files().contains(path))
	{
		readdPath = true;
		fileWatcher.removePath(path);
	}

	if(saves.at(id)->isDelete()) {
		QFile::remove(path);
		return true;
	}

	QByteArray result = LZS::compress(saves.at(id)->save());
	int size = result.size();
	temp.write((char *)&size, 4);
	temp.write(result);

	if(QFile::exists(path) && !QFile::remove(path))
	{
		QMessageBox::warning(this, tr("Erreur"), tr("Impossible de supprimer le fichier !\n%1\nÉchec de la sauvegarde.\nVérifiez que le fichier n'est pas utilisé par un autre programme.").arg(path));
		if(readdPath)	fileWatcher.addPath(path);
		return false;
	}
	if(!temp.copy(path))
	{
		QMessageBox::warning(this, tr("Erreur"), tr("Échec de la sauvegarde."));
	}
	if(readdPath)	fileWatcher.addPath(path);

	if(_type == Undefined) {
		_hasPath = true;
		setPath(path);
		setType(Pc);
	}

	return true;
}

bool Savecard::save2PS(QList<int> ids, const QString &path, Type newType)
{
	QTemporaryFile temp("hynePS");
	SaveData tempSave;
	quint8 i;

	if(!temp.open())
	{
		QMessageBox::warning(this, tr("Erreur"), tr("Impossible de créer un fichier temporaire"));
		return false;
	}

	bool abort;

	QByteArray head = header(0, newType, true, &abort);
	if(abort)	return false;
	temp.write(head);

	if(_type==Psv) {
		tempSave.setMCHeader(true, saves.first()->MCHeaderCountry(), saves.first()->MCHeaderCode(), saves.first()->MCHeaderId());
	}
	else if(!saves.first()->hasMCHeader()) {
		HeaderDialog dialog(&tempSave, this, HeaderDialog::CreateView);

		if(dialog.exec()!=QDialog::Accepted) return false;
	}

	QByteArray MCHeader = tempSave.saveMCHeader();

	temp.write("MC", 2);//MC
	temp.write(QByteArray(125,'\x00'));
	temp.putChar('\x0E');//xor byte

	for(i=0 ; i<15 ; ++i)
	{
		// 128 bytes
		if(i>=ids.size())
		{
			temp.write(SaveData::emptyMCHeader());
		}
		else
		{
			temp.write(MCHeader.replace(26, 2, QString("%1").arg(i, 2, 10, QChar('0')).toLatin1()));
		}
	}

	temp.write(QByteArray(6144, '\x00'));//Padding

	for(i=0 ; i<15 ; ++i)
	{
		// 8192 bytes
		if(i>=ids.size() || !saves.at(ids.at(i))->isFF8())
		{
			temp.write(QByteArray(SAVE_SIZE, '\x00'));
		}
		else
		{
			temp.write(saves.at(ids.at(i))->save());
		}
	}

	bool readdPath = false;
	if(fileWatcher.files().contains(path))
	{
		readdPath = true;
		fileWatcher.removePath(path);
	}

	if(QFile::exists(path) && !QFile::remove(path))
	{
		QMessageBox::warning(this, tr("Erreur"), tr("Impossible de supprimer le fichier !\n%1\nÉchec de la sauvegarde.\nVérifiez que le fichier n'est pas utilisé par un autre programme.").arg(path));
		if(readdPath)	fileWatcher.addPath(path);
		return false;
	}
	if(!temp.copy(path))
	{
		QMessageBox::warning(this, tr("Erreur"), tr("Échec de la sauvegarde."));
	}
	if(readdPath)	fileWatcher.addPath(path);

	if(_type == Undefined) {
		_hasPath = true;
		setPath(path);
		setType(newType);
		for(i=saves.size() ; i<15 ; ++i) {
			addSave();
		}
		foreach(SaveData *save, saves) {
			if(save->isFF8()) {
				save->setMCHeader(MCHeader);
				save->setModified(false);
			}
		}
	}

	return true;
}

QByteArray Savecard::header(QFile *srcFile, Type newType, bool saveAs, bool *abort)
{
	*abort = false;

	if(newType==Vgs)
	{
		// header : 64 bytes
		if(_type==Vgs)
		{
			return srcFile->read(64);
		}
		else
		{
			return QByteArray("\x56\x67\x73\x4d\x01\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x00\x02", 18).append(QByteArray(46, '\x00'));
		}
	}
	else if(newType==Gme)
	{
		QByteArray header;
		// header : 64 bytes
		if(_type==Gme)
		{
			header = srcFile->read(64);
		}
		else
		{
			header = QByteArray("\x31\x32\x33\x2d\x34\x35\x36\x2d\x53\x54\x44\x00\x00\x00\x00\x00\x00\x00\x01\x00\x01\x4d\x51\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 64);
		}

		// description : 3840 bytes
		if(saveAs)
		{
			QString old_desc;
			if(_type==Gme)
			{
				//srcFile->seek(64);
				old_desc = QString(srcFile->read(3840));
			}

			QByteArray new_desc = descGme(old_desc, abort);
			if(*abort)	return QByteArray();
			return header.append(new_desc);
		}
		else
		{
			//srcFile->seek(64);
			return header.append(srcFile->read(3840));
		}
	}
	else if(newType==Vmp)
	{
		// header : 128 bytes
		if(_type==Vmp)
		{
			return srcFile->read(128);
		}
		else
		{
			//Unknown crc
			return QByteArray("\x00\x50\x4d\x56\x80", 5).append(QByteArray(123, '\x00'));
		}
	}
	else if(newType==_type)
	{
		return srcFile->read(start);
	}
	else
		return QByteArray();
}

QByteArray Savecard::descGme(const QString &desc, bool *abort)
{
	QDialog dialog(this, Qt::Dialog | Qt::WindowCloseButtonHint);
	dialog.setWindowTitle(tr("Commentaire"));

	QTextEdit *textEdit = new QTextEdit(&dialog);
	textEdit->setPlainText(desc);

	QPushButton *ok = new QPushButton(tr("OK"), &dialog);
	QPushButton *cancel = new QPushButton(tr("Annuler"), &dialog);

	QGridLayout *layout = new QGridLayout(&dialog);
	layout->addWidget(textEdit, 0, 0, 1, 2);
	layout->addWidget(ok, 1, 0);
	layout->addWidget(cancel, 1, 1);

	connect(ok, SIGNAL(released()), &dialog, SLOT(accept()));
	connect(cancel, SIGNAL(released()), &dialog, SLOT(reject()));

	if(dialog.exec() != QDialog::Accepted) {
		*abort = true;
		return QByteArray();
	}
	*abort = false;

	return textEdit->toPlainText().toLatin1().leftJustified(3840, '\x00', true);
}

void Savecard::saveDir()
{
	for(int i=0 ; i<saves.size() ; ++i)
		saveDir(i);
}

void Savecard::saveDir(quint8 i)
{
	if(saves.at(i)->isModified()) {
		setName(QString("save%1").arg(i+1, 2, 10, QChar('0')));
		saveOne(i);
	}
}

void Savecard::compare(const QByteArray &oldData, const QByteArray &newData)
{
	qDebug() << "Compare ----";
	for(int i=386 ; i<FF8SAVE_SIZE ; ++i) {
		switch(i) {
		case 0:
			qDebug() << "\tHeader1";break;
		case 386:
			qDebug() << "\tHeader2";break;
		case 464:
			qDebug() << "\tGF";break;
		case 1552:
			qDebug() << "\tPerso";break;
		case 2768:
			qDebug() << "\tBoutiques";break;
		case 3168:
			qDebug() << "\tConfig";break;
		case 3188:
			qDebug() << "\tDivers 1";break;
		case 3220:
			qDebug() << "\tLimit Break";break;
		case 3236:
			qDebug() << "\tObjets";break;
		case 3664:
			qDebug() << "\tStats";break;
		case 3808:
			qDebug() << "\tVar map 1";break;
		case 4064:
			qDebug() << "\tVar map 2";break;
		case 5088:
			qDebug() << "\tMappemonde";break;
		case 5216:
			qDebug() << "\tTriple Triad Cards";break;
		case 5344:
			qDebug() << "\tChocobo";break;
		}

		if(oldData.at(i)!=newData.at(i)) {
			qDebug() << QString("Difference at %1: %2 (%3) | %4 (%5)%6").arg(i).arg((quint8)oldData.at(i),2,16,QChar('0')).arg((quint8)oldData.at(i),8,2,QChar('0')).arg((quint8)newData.at(i),2,16,QChar('0')).arg((quint8)newData.at(i),8,2,QChar('0')).arg(i >= 3808 && i<5088 ? QString(" var%1").arg(i-3808) : "").toLatin1().constData();
		}
	}
}
