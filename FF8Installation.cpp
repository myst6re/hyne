#include "FF8Installation.h"
#ifdef Q_OS_WIN
#include <windef.h>
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

QList<FF8Installation> FF8Installation::installations()
{
	QList<FF8Installation> ret = QList<FF8Installation>()
			<< FF8Installation(Standard)
			   << FF8Installation(Steam);

	QMutableListIterator<FF8Installation> it(ret);
	while(it.hasNext()) {
		if(!it.next().isValid()) {
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
	QStringList apps = searchInstalledApps("FINAL FANTASY VIII", 1);
	return apps.isEmpty() ? QString() : apps.first();
}

QStringList FF8Installation::steamFF8UserDataPaths(int max)
{
	QStringList ff8UserDataPaths;
	QDir ff8UserDataDir(QDir::homePath() + "/Documents/Square Enix");

	foreach(const QString &dir, ff8UserDataDir.entryList(QStringList("FINAL FANTASY VIII*"), QDir::Dirs)) {
		QDir userDirs(ff8UserDataDir.absoluteFilePath(dir));

		foreach(const QString &userDir, userDirs.entryList(QStringList("user_*"), QDir::Dirs)) {
			ff8UserDataPaths.append(userDirs.absoluteFilePath(userDir));
			if(max > 0 && ff8UserDataPaths.size() >= max) {
				return ff8UserDataPaths;
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

QStringList FF8Installation::searchInstalledApps(const QString &appName, int max)
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
				if(ERROR_SUCCESS == error) {
					if(type == REG_SZ) {
						QString softwareNameStr = QString::fromUtf16((ushort *)value);
						if(softwareNameStr.compare(appName, Qt::CaseInsensitive) == 0) {
							cValue = MAX_PATH;
							error = RegQueryValueEx(phkResult2, TEXT("InstallLocation"), NULL, &type, value, &cValue);
							if(ERROR_SUCCESS == error) {
								if(type == REG_SZ) {
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
#endif
	return ret;
}
