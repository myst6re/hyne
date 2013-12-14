#ifndef FF8INSTALLATION_H
#define FF8INSTALLATION_H

#include <QtCore>

struct FF8Installation
{
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

	static QList<FF8Installation> installations();
private:
	static QString standardFF8AppPath();
	static QString steamFF8AppPath();
	static QStringList steamFF8UserDataPaths(int max=-1);
	static QString regValue(const QString &regPath, const QString &regKey);
	static QStringList searchInstalledApps(const QString &appName, int max=-1);

	Type _type;
	QString _appPath;
	QStringList _savePaths;
};

#endif // FF8INSTALLATION_H
