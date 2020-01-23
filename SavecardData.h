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
		Pc, PcUncompressed, Switch, Ps, Vgs, Gme, Vmp, Psv, PcSlot, Unknown, Undefined
	};

	inline static bool isOne(Type type) {
		return type == Pc || type == PcUncompressed || type == Switch || type == Psv;
	}

	explicit SavecardData(const QString &path, quint8 slot=0, const FF8Installation &ff8Installation=FF8Installation());
	explicit SavecardData(int saveCount);
	virtual ~SavecardData();

	bool open(const QString &path, quint8 slot=0);
#ifndef Q_OS_WINRT
	const QFileSystemWatcher *watcher() const;
#endif
	inline const QString &errorString() const {
		return _lastError;
	}
	QString description() const;
	void setDescription(const QString &desc);
	const QByteArray &hashSeed() const;
	void setHashSeed(const QByteArray &hashSeed);
	void setIsTheLastEdited(int saveID);
	inline const QList<SaveData *> &getSaves() const {
		return saves;
	}
	void moveSave(int sourceID, int targetID);
	SaveData *getSave(int id) const;
	int saveCount() const;
	bool saveMemoryCard(const QString &saveAs, Type newType);
	bool saveOne(const SaveData *save, const QString &saveAs, Type newType);
	bool save2PS(const QList<int> &ids, const QString &path, const Type newType, const QByteArray &MCHeader);
	bool saveDirectory(const QString &dir = QString());

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
#ifndef Q_OS_WINRT
	QFileSystemWatcher fileWatcher;
#endif
	QList<SaveData *> saves;
	bool _isModified;
	QByteArray _description;
	quint8 _slot;
	QByteArray _hashSeed;
	const FF8Installation _ff8Installation;
};

#endif
