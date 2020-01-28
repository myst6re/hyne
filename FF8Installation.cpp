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
#include "FF8Installation.h"
#ifdef Q_OS_WIN32
#include <windows.h>
#include <winbase.h>
//#include <winerror.h>
#include <winreg.h>
#endif

FF8Installation::FF8Installation() :
	_type(Custom)
{
}

FF8Installation::FF8Installation(Type type) :
	_type(type)
{
	switch(type) {
	case Standard:
		_appPath = standardFF8AppPath("Square Soft, Inc/Final Fantasy VIII/1.00");
		_savePaths = standardFF8DataPaths(_appPath);
		break;
	case Demo:
		_appPath = standardFF8AppPath("Square Soft, Inc/Final Fantasy VIII Demo/1.00");
		_savePaths = standardFF8DataPaths(_appPath);
		break;
	case Steam:
		_appPath = steamFF8AppPath();
		_savePaths = steamFF8UserDataPaths();
		break;
	case Remaster:
		_appPath = steamFF8RemasterAppPath();
		_savePaths = steamFF8RemasterUserDataPaths();
		break;
	case Custom:
		break;
	}
}

FF8Installation::FF8Installation(const QString &appPath, const QStringList &savePaths) :
	_type(Custom), _appPath(appPath), _savePaths(savePaths)
{
}

bool FF8Installation::isValid() const
{
	return !_appPath.isEmpty() && QFile::exists(_appPath);
}

bool FF8Installation::hasSlots() const
{
	return !_savePaths.isEmpty();
}

QString FF8Installation::savePath(int slot) const
{
	if(_savePaths.isEmpty()) {
		return QString();
	}
	QString path = _savePaths.first();
	switch(_type) {
	case Standard:
	case Demo:
		return QString("%1/Slot%2").arg(path).arg(slot);
	case Steam:
	case Remaster:
		return path;
	case Custom: // TODO: Custom custom!
		return path;
	}
	Q_ASSERT(false);
	return QString();
}

QString FF8Installation::exeFilename() const
{
	switch(_type) {
	case Standard:
	case Demo:
		return "FF8.exe";
	case Steam:
		return "FF8_Launcher.exe";
	case Remaster:
		return "FFVIII_LAUNCHER.exe";
	case Custom: // TODO: Custom custom!
		return QString();
	}
	Q_ASSERT(false);
	return QString();
}

QString FF8Installation::typeString() const
{
	switch(_type) {
	case Standard:
		return QObject::tr("FF8 Standard");
	case Demo:
		return QObject::tr("FF8 Demo");
	case Steam:
		return QObject::tr("FF8 Steam");
	case Remaster:
		return QObject::tr("FF8 Remaster");
	case Custom:
		return QObject::tr("FF8 personnalisé");
	}
	Q_ASSERT(false);
	return QString();
}

QString FF8Installation::saveNamePattern(quint8 slot) const
{
	switch(_type) {
	case Standard:
	case Demo:
		return "save{num}";
	case Steam:
	case Remaster:
		return QString("slot%1_save{num}.ff8").arg(slot);
	case Custom:
		return "save{num}"; // TODO!
	}
	Q_ASSERT(false);
	return QString();
}

bool FF8Installation::hasMetadata() const
{
	return _type == Steam; // TODO: Custom
}

QMap<FF8Installation::Type, FF8Installation> FF8Installation::installations()
{
	QMap<FF8Installation::Type, FF8Installation> ret;
	ret[Standard] = FF8Installation(Standard);
	ret[Demo] = FF8Installation(Demo);
	ret[Steam] = FF8Installation(Steam);
	ret[Remaster] = FF8Installation(Remaster);

	QMutableMapIterator<FF8Installation::Type, FF8Installation> it(ret);
	while(it.hasNext()) {
		const FF8Installation &inst = it.next().value();
		if(!inst.isValid() && !inst.hasSlots()) {
			it.remove();
		}
	}

	return ret;
}

QStringList FF8Installation::standardFF8DataPaths(const QString &appPath)
{
	QString path;
	QStringList appDataLocations = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);

	if (!appDataLocations.isEmpty()) {
		path = appDataLocations.first().append("/VirtualStore/").append(QString(appPath).replace(QRegExp("^\\w+:/"), ""));
		if (!QFile::exists(path)) {
			path = QString();
		}
	}

	if (path.isEmpty()) {
		path = appPath;
	}

	return QStringList(path + "/save");
}

QString FF8Installation::standardFF8AppPath(const QString &path)
{
	return QDir::cleanPath( QDir::fromNativeSeparators( regValue(path, "AppPath") ) );
}

QString FF8Installation::steamFF8AppPath()
{
	QStringList apps = searchInstalledApps("FINAL FANTASY VIII", "SQUARE ENIX", 1);
	return apps.isEmpty() ? QString() : apps.first();
}

QString FF8Installation::steamFF8RemasterAppPath()
{
	QStringList apps = searchInstalledApps("FINAL FANTASY VIII - REMASTERED", "Square Enix", 1);
	return apps.isEmpty() ? QString() : apps.first();
}

QStringList FF8Installation::steamFF8UserDataPaths(int max)
{
	QStringList ff8UserDataPaths;

	for (const QString &documentsPath : QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)) {
		QDir ff8UserDataDir(QString("%1/Square Enix").arg(documentsPath));

		for(const QString &dir : ff8UserDataDir.entryList(QStringList("FINAL FANTASY VIII*"), QDir::Dirs)) {
			QDirIterator it(ff8UserDataDir.absoluteFilePath(dir),
							QStringList("user_*"),
							QDir::Dirs,
							QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);

			while(it.hasNext()) {
				it.next();

				ff8UserDataPaths.append(it.filePath());

				if(max > 0 && ff8UserDataPaths.size() >= max) {
					return ff8UserDataPaths;
				}
			}
		}
	}

	return ff8UserDataPaths;
}

QStringList FF8Installation::steamFF8RemasterUserDataPaths(int max)
{
	QStringList ff8UserDataPaths;

	for (const QString &documentsPath : QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)) {
		QDir ff8UserDataDir(documentsPath + "/My Games");

		for(const QString &dir : ff8UserDataDir.entryList(QStringList("FINAL FANTASY VIII*"), QDir::Dirs)) {
			QDirIterator it(ff8UserDataDir.absoluteFilePath(dir + "/Steam"),
							QStringList("*"),
							QDir::Dirs,
							QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);

			while(it.hasNext()) {
				it.next();

				QString path = it.filePath() + "/game_data/user/saves";

				if (QFile::exists(path)) {
					ff8UserDataPaths.append(path);

					if(max > 0 && ff8UserDataPaths.size() >= max) {
						return ff8UserDataPaths;
					}
				}
			}
		}
	}

	return ff8UserDataPaths;
}

QString FF8Installation::regValue(const QString &regPath, const QString &regKey)
{
#ifdef Q_OS_WIN32
	HKEY phkResult;
	LONG error;
	REGSAM flags = KEY_READ;

#ifdef KEY_WOW64_32KEY
	flags |= KEY_WOW64_32KEY; // if you compile in 64-bit, force reg search into 32-bit entries
#endif

	// Open regPath relative to HKEY_LOCAL_MACHINE
	error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, (wchar_t *)QDir::toNativeSeparators("SOFTWARE/" + regPath).utf16(), 0, flags, &phkResult);
	if(ERROR_SUCCESS == error) {
		BYTE value[MAX_PATH];
		DWORD cValue = MAX_PATH, type;

		// Open regKey which must is a string value (REG_SZ)
		RegQueryValueEx(phkResult, (wchar_t *)regKey.utf16(), nullptr, &type, value, &cValue);
		if(ERROR_SUCCESS == error && type == REG_SZ) {
			RegCloseKey(phkResult);
			return QString::fromUtf16((ushort *)value);
		}
		RegCloseKey(phkResult);
	}
#else
	Q_UNUSED(regPath)
	Q_UNUSED(regKey)
#endif
	return QString();
}

QStringList FF8Installation::searchInstalledApps(const QString &appName, const QString &publisher, int max)
{
	QStringList ret;
#ifdef Q_OS_WIN32
	HKEY phkResult, phkResult2;
	LONG error;
	REGSAM flags = KEY_READ;

#ifdef KEY_WOW64_64KEY
	flags |= KEY_WOW64_64KEY; // if you compile in 64-bit, force reg search into 64-bit entries
#endif

	error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"), 0, flags, &phkResult);

	if(ERROR_SUCCESS == error) {
		DWORD index = 0;
		WCHAR subKeyName[MAX_PATH];
		DWORD subKeyCName = MAX_PATH;

		while(ERROR_SUCCESS == (error = RegEnumKeyEx(phkResult, index, subKeyName, &subKeyCName, nullptr, nullptr, nullptr, nullptr))) {
			QString subKeyNameStr = QString::fromUtf16((ushort *)subKeyName);
			error = RegOpenKeyEx(phkResult, (LPCWSTR)QString("%1\\").arg(subKeyNameStr).utf16(), 0, KEY_READ, &phkResult2);

			if(ERROR_SUCCESS == error) {
				BYTE value[MAX_PATH];
				DWORD cValue = MAX_PATH, type;
				error = RegQueryValueEx(phkResult2, TEXT("DisplayName"), nullptr, &type, value, &cValue);

				if(ERROR_SUCCESS == error && REG_SZ == type) {
					QString softwareNameStr = QString::fromUtf16((ushort *)value);

					if(softwareNameStr.compare(appName, Qt::CaseInsensitive) == 0) {
						error = RegQueryValueEx(phkResult2, TEXT("Publisher"), nullptr, &type, value, &cValue);

						if(ERROR_SUCCESS == error && REG_SZ == type) {
							QString publisherStr = QString::fromUtf16((ushort *)value);

							if(publisherStr.compare(publisher, Qt::CaseInsensitive) == 0) {
								cValue = MAX_PATH;
								error = RegQueryValueEx(phkResult2, TEXT("InstallLocation"), nullptr, &type, value, &cValue);

								if(ERROR_SUCCESS == error && REG_SZ == type) {
									QString appPath = QDir::fromNativeSeparators(QDir::cleanPath(QString::fromUtf16((ushort *)value)));

									if (QFile::exists(appPath)) {
										ret.append(appPath);

										if(max > 0 && ret.size() >= max) {
											RegCloseKey(phkResult2);
											RegCloseKey(phkResult);
											return ret;
										}
									}
								}
							}
						}
					}
				}
			}
			RegCloseKey(phkResult2);
			++index;
			subKeyCName = MAX_PATH;
		}

		RegCloseKey(phkResult);
	}
#else
	Q_UNUSED(appName)
	Q_UNUSED(publisher)
	Q_UNUSED(max)
#endif
	return ret;
}
