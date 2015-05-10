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
#ifdef Q_OS_WIN
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
		_appPath = standardFF8AppPath();
		_savePaths.append(_appPath + "/save"); // TODO: \AppData\Local\VirtualStore\Program Files\Eidos Interactive\Square Soft, Inc\FINAL FANTASY VIII
		break;
	case Steam:
		_appPath = steamFF8AppPath();
		_savePaths = steamFF8UserDataPaths();
		break;
	case Custom:
		break;
	}
}

FF8Installation::FF8Installation(const QString &appPath, const QString &savePath) :
	_type(Custom)
{
	_appPath = appPath;
	_savePaths = QStringList(savePath);
}

bool FF8Installation::isValid() const
{
	return !_appPath.isEmpty() && QFile::exists(_appPath)
			&& !_savePaths.isEmpty();
}

QString FF8Installation::savePath(int slot) const
{
	if(_savePaths.isEmpty()) {
		return QString();
	}
	QString path = _savePaths.first();
	switch(_type) {
	case Standard:
		return QString("%1/Slot%2").arg(path).arg(slot);
	case Steam:
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
		return "FF8.exe";
	case Steam:
		return "FF8_Launcher.exe";
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
	case Steam:
		return QObject::tr("FF8 Steam");
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
		return "save{num}";
	case Steam:
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
	ret[Steam] = FF8Installation(Steam);

	QMutableMapIterator<FF8Installation::Type, FF8Installation> it(ret);
	while(it.hasNext()) {
		if(!it.next().value().isValid()) {
			it.remove();
		}
	}

	return ret;
}

QString FF8Installation::standardFF8AppPath()
{
	return QDir::cleanPath( QDir::fromNativeSeparators( regValue("Square Soft, Inc/Final Fantasy VIII/1.00", "AppPath") ) );
}

QString FF8Installation::steamFF8AppPath()
{
	QStringList apps = searchInstalledApps("FINAL FANTASY VIII", "SQUARE ENIX", 1);
	return apps.isEmpty() ? QString() : apps.first();
}

QStringList FF8Installation::steamFF8UserDataPaths(int max)
{
	QStringList ff8UserDataPaths;

	foreach (const QString &documentsPath, QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)) {
		QDir ff8UserDataDir(QString("%1/Square Enix").arg(documentsPath));

		foreach(const QString &dir, ff8UserDataDir.entryList(QStringList("FINAL FANTASY VIII*"), QDir::Dirs)) {
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

QString FF8Installation::regValue(const QString &regPath, const QString &regKey)
{
#ifdef Q_OS_WIN
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
		RegQueryValueEx(phkResult, (wchar_t *)regKey.utf16(), NULL, &type, value, &cValue);
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
#ifdef Q_OS_WIN
	HKEY phkResult, phkResult2;
	LONG error;

	error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"), 0, KEY_READ, &phkResult);

	if(ERROR_SUCCESS == error) {
		DWORD index = 0;
		WCHAR subKeyName[MAX_PATH];
		DWORD subKeyCName = MAX_PATH;

		while(ERROR_NO_MORE_ITEMS != (error = RegEnumKeyEx(phkResult, index, subKeyName, &subKeyCName, NULL, NULL, NULL, NULL))) {
			QString subKeyNameStr = QString::fromUtf16((ushort *)subKeyName);
			error = RegOpenKeyEx(phkResult, (LPCWSTR)QString("%1\\").arg(subKeyNameStr).utf16(), 0, KEY_READ, &phkResult2);

			if(ERROR_SUCCESS == error) {
				BYTE value[MAX_PATH];
				DWORD cValue = MAX_PATH, type;
				error = RegQueryValueEx(phkResult2, TEXT("DisplayName"), NULL, &type, value, &cValue);

				if(ERROR_SUCCESS == error && REG_SZ == type) {
					QString softwareNameStr = QString::fromUtf16((ushort *)value);

					if(softwareNameStr.compare(appName, Qt::CaseInsensitive) == 0) {
						error = RegQueryValueEx(phkResult2, TEXT("Publisher"), NULL, &type, value, &cValue);

						if(ERROR_SUCCESS == error && REG_SZ == type) {
							QString publisherStr = QString::fromUtf16((ushort *)value);

							if(publisherStr.compare(publisher, Qt::CaseInsensitive) == 0) {
								cValue = MAX_PATH;
								error = RegQueryValueEx(phkResult2, TEXT("InstallLocation"), NULL, &type, value, &cValue);

								if(ERROR_SUCCESS == error && REG_SZ == type) {

									ret.append(QDir::fromNativeSeparators(QDir::cleanPath(QString::fromUtf16((ushort *)value))));

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
