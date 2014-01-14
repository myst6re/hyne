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
#ifndef FF8INSTALLATION_H
#define FF8INSTALLATION_H

#include <QtCore>

class FF8Installation
{
public:
	enum Type {
		Standard, Steam, Custom
	};

	FF8Installation();
	FF8Installation(Type type);
	FF8Installation(const QString &appPath, const QString &savePath);

	bool isValid() const;
	QString savePath(int slot) const;
	QString exeFilename() const;
	QString typeString() const;
	QString saveNamePattern(quint8 slot) const;
	bool hasMetadata() const;
	inline Type type() const {
		return _type;
	}
	inline const QString &appPath() const {
		return _appPath;
	}
	inline const QStringList &savePaths() const {
		return _savePaths;
	}
	inline void setType(Type type) {
		_type = type;
	}
	inline void setAppPath(const QString &appPath) {
		_appPath = appPath;
	}
	inline void savePaths(const QStringList &savePaths) {
		_savePaths = savePaths;
	}
	inline bool operator ==(const FF8Installation &i2) const {
		return type() == i2.type();
	}

	static QMap<FF8Installation::Type, FF8Installation> installations();
private:
	static QString standardFF8AppPath();
	static QString steamFF8AppPath();
	static QStringList steamFF8UserDataPaths(int max=-1);
	static QString regValue(const QString &regPath, const QString &regKey);
	static QStringList searchInstalledApps(const QString &appName, const QString &publisher, int max=-1);

	Type _type;
	QString _appPath;
	QStringList _savePaths;
};

#endif // FF8INSTALLATION_H
