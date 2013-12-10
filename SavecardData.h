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

#ifndef DEF_SAVECARDDATA
#define DEF_SAVECARDDATA

#include <QtCore>
#include "SaveData.h"
#include "UserDirectory.h"

class SavecardData
{
public:
	enum Type {
		Pc, Ps, Vgs, Gme, Vmp, Psv, PcSlot1, PcSlot2, Unknown, Undefined
	};

	SavecardData(const QString &path, quint8 slot=0);
	SavecardData(int saveCount);
	virtual ~SavecardData();

	bool open(const QString &path, quint8 slot=0);
	const QFileSystemWatcher *watcher() const;
	const QString &errorString() const;
	QString description() const;
	void setDescription(const QString &desc);
	void setIsTheLastEdited(int saveID);
	const QList<SaveData *> &getSaves() const;
	void moveSave(int sourceID, int targetID);
	SaveData *getSave(int id) const;
	int saveCount() const;
	bool save2PC(const quint8 id, const QString &saveAs);
	bool save2PSV(const quint8 id, const QString &saveAs, const QByteArray &MCHeader);
	bool save2PS(const QList<int> &ids, const QString &path, const Type newType, const QByteArray &MCHeader);
	bool saveDir();

	QString dirname() const;
	QString name() const;
	QString extension() const;
	const QString &path() const;
	void setName(const QString &name);
	bool isOpen() const;
	Type type() const;
	bool hasPath() const;
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
	bool pc(const QString &path = QString());
	bool sstate_ePSXe();
	bool sstate_pSX();
	bool sstate(const QByteArray &fdata, const QByteArray &MCHeader);
	void directory(const QString &filePattern);
	void addSave(const QByteArray &data=QByteArray(), const QByteArray &header=QByteArray(), bool occupied=false);
	QByteArray header(QFile *srcFile, Type newType, bool saveAs);

	QString _path, _lastError;
	bool _ok;
	Type _type;
	quint16 start; // Start of save data
	QFileSystemWatcher fileWatcher;
	QList<SaveData *> saves;
	bool _isModified;
	QByteArray _description;
};

#endif
