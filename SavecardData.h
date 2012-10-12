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

#ifndef DEF_SAVECARDDATA
#define DEF_SAVECARDDATA

#include <QtCore>
#include "SaveData.h"

class SavecardData
{
public:
	enum Type {
		Pc, Ps, Vgs, Gme, Vmp, Psv, PcDir, Unknown, Undefined
	};

	SavecardData(const QString &path, bool slot=false);
	SavecardData(int saveCount);
	virtual ~SavecardData();

	bool open(const QString &path, bool slot=false);
	const QFileSystemWatcher *watcher() const;
	const QString &errorString() const;
	QString description() const;
	void setDescription(const QString &desc);
	void setIsTheLastEdited(int saveID);
	const QList<SaveData *> &getSaves() const;
	void setSaves(const QList<SaveData *> &saves);
	bool save2PC(qint8 num, QString path=QString());
	bool save2PSV(qint8 id, QString path=QString());
	bool save2PS(QList<int> ids, const QString &path, Type newType, QByteArray MCHeader);
	void saveDir();
	void saveDir(quint8);

	QString dirname() const;
	QString name() const;
	QString extension() const;
	const QString &path() const;
	void setName(const QString &name);
	bool ok() const;
	Type type() const;
	bool hasPath() const;
	bool isOneSaveType() const;
	bool isModified() const;
	void setModified(bool modified);
	static void compare(const QByteArray &oldData, const QByteArray &newData);

	bool getFormatFromRaw();
	bool save(const QString &saveAs=QString(), Type newType=Pc);
private:
	void setPath(const QString &path);
	void setType(Type type);
	bool ps();
	bool ps3();
	bool pc();
	bool sstate_ePSXe();
	bool sstate_pSX();
	bool sstate(const QByteArray &fdata, const QByteArray &MCHeader);
	void directory();
	void addSave(const QByteArray &data=QByteArray(), const QByteArray &header=QByteArray());
	QByteArray header(QFile *srcFile, Type newType, bool saveAs);

	QString _path, _lastError;
	bool _ok;
	Type _type;
	quint16 start;//Départ de la sauvegarde <=> taille du header rajouté
	QFileSystemWatcher fileWatcher;
	QList<SaveData *> saves;
	bool _isModified;
	QByteArray _description;
};

#endif
