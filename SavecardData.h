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
#include "FF8Installation.h"

class SavecardData
{
public:
	enum Type {
		Pc, Ps, Vgs, Gme, Vmp, Psv, PcSlot, Unknown, Undefined
	};

	explicit SavecardData(const QString &path, quint8 slot=0, const FF8Installation &ff8Installation=FF8Installation());
	explicit SavecardData(int saveCount);
	virtual ~SavecardData();

	bool open(const QString &path, quint8 slot=0);
	const QFileSystemWatcher *watcher() const;
	inline const QString &errorString() const {
		return _lastError;
	}
	QString description() const;
	void setDescription(const QString &desc);
	void setIsTheLastEdited(int saveID);
	inline const QList<SaveData *> &getSaves() const {
		return saves;
	}
	void moveSave(int sourceID, int targetID);
	SaveData *getSave(int id) const;
	int saveCount() const;
	bool save2PC(const quint8 id, const QString &saveAs);
	bool save2PSV(const quint8 id, const QString &saveAs, const QByteArray &MCHeader);
	bool save2PS(const QList<int> &ids, const QString &path, const Type newType, const QByteArray &MCHeader);
	bool saveDirectory();

	QString dirname() const;
	QString name() const;
	QString extension() const;
	inline const QString &path() const {
		return _path;
	}
	void setName(const QString &name);
	inline bool isOpen() const {
		return _ok;
	}
	inline Type type() const {
		return _type;
	}
	inline quint8 slotNumber() const {
		return _slot;
	}
	bool hasPath() const;
	bool isModified() const;
	void setModified(bool modified);
	inline const FF8Installation &ff8Installation() const {
		return _ff8Installation;
	}

	void compare(quint8 idLeft, quint8 idRight) const;
	static void compare(const QByteArray &oldData, const QByteArray &newData);

	bool getFormatFromRaw();
	bool save(const QString &saveAs=QString(), Type newType=Pc);
private:
	inline void setPath(const QString &path) {
		_path = path;
	}
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
	inline void setErrorString(const QString &errorString) {
		_lastError = errorString;
	}

	QString _path, _lastError;
	bool _ok;
	Type _type;
	quint16 start; // Start of save data
	QFileSystemWatcher fileWatcher;
	QList<SaveData *> saves;
	bool _isModified;
	QByteArray _description;
	quint8 _slot;
	const FF8Installation _ff8Installation;
};

#endif
