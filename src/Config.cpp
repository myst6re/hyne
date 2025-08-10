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

#include "Config.h"

const char *Config::keys[KEYS_SIZE] = {
	"recentFiles", "lang", "geometry",
	"font", "freq", "freq_auto", "mode", "lastCountry", "lastGameCode", "selectedFF8Installation",
	"loadPath", "savePath", "savePathIcon"
};

QTranslator *Config::translator;
QStringList Config::recentFiles;
QSettings *Config::settings = nullptr;
QMap<FF8Installation::Type, FF8Installation> Config::_ff8Installations;
FF8Installation::Type Config::_selectedFF8Installation = FF8Installation::Standard;
bool Config::_ff8InstallationsSearched = false;

QString Config::translationDir()
{
	QDir translationDir(QStringLiteral("%1").arg(QCoreApplication::applicationDirPath()));
	QStringList nameFilter{QStringLiteral("hyne_*.qm")};
	if (translationDir.entryList(nameFilter, QDir::Files, QDir::Name).isEmpty()) {
		translationDir.setPath(QStringLiteral("%1/%2").arg(QCoreApplication::applicationDirPath(), QStringLiteral("translations")));
		if (translationDir.entryList(nameFilter, QDir::Files, QDir::Name).isEmpty()) {
			translationDir.setPath(QStringLiteral("%1/../translations").arg(QCoreApplication::applicationDirPath()));
			if (translationDir.entryList(nameFilter, QDir::Files, QDir::Name).isEmpty()) {
				translationDir.setPath(QStringLiteral("%1/../share/hyne/translations").arg(QCoreApplication::applicationDirPath()));
				if (translationDir.entryList(nameFilter, QDir::Files, QDir::Name).isEmpty()) {
					translationDir.setPath(QStringLiteral("%1/%2").arg(QDir::homePath(), QStringLiteral(".local/share/hyne/translations")));
					if (translationDir.entryList(nameFilter, QDir::Files, QDir::Name).isEmpty()) {
						translationDir.setPath(QStringLiteral("/usr/local/share/hyne/translations"));
						if (translationDir.entryList(nameFilter, QDir::Files, QDir::Name).isEmpty()) {
							translationDir.setPath(QStringLiteral("/usr/share/hyne/translations"));
						}
					}
				}
			}
		}
	}
	return translationDir.absolutePath();
}

quint32 Config::sec(quint32 time, int freq_value)
{
	return time % freq(freq_value);
}

quint32 Config::min(quint32 time, int freq_value)
{
	return (time / freq(freq_value)) % 60;
}

quint32 Config::hour(quint32 time, int freq_value)
{
	return time / (60 * freq(freq_value));
}

quint32 Config::time(quint32 hour, quint32 min, quint32 sec, int freq_value)
{
	int f = freq(freq_value);
	return sec + min * f + hour * f * 60;
}

void Config::loadRecentFiles()
{
	recentFiles = settings->value(keyToStr(RecentFiles)).toStringList();
	// Compatibility with old version (< 1.6)
	if (recentFiles.isEmpty()) {
		for (int i = 0; i < 20; ++i) {
			QString filePath = settings->value(QString("recentFile%1").arg(i), QString()).toString();
			if (!filePath.isEmpty()) {
				recentFiles.append(QDir::cleanPath(filePath));
			}
		}
	}
}

void Config::addRecentFile(const QString &filePath)
{
	int index = recentFiles.indexOf(filePath);
	if (index != -1)					recentFiles.removeAt(index);
	recentFiles.prepend(filePath);
	if (recentFiles.size() > 20)		recentFiles.removeLast();
}

const QString &Config::recentFile(const uint &index)
{
	return recentFiles.at(index);
}

bool Config::hasRecentFiles()
{
	return !recentFiles.isEmpty();
}

int Config::recentFilesSize()
{
	return recentFiles.size();
}

void Config::saveRecentFiles()
{
	QStringList rf;
	int i=0;

	for (const QString &recentFile : std::as_const(recentFiles)) {
		if (QFile::exists(recentFile)) {
			rf.append(QDir::cleanPath(recentFile));
			++i;
		}
		if (i > 20)	break;
	}

	setValue(RecentFiles, rf);
	// Compatibility with old version (< 1.6)
	for (i = 0; i < 20; ++i)
		settings->remove(QString("recentFile%1").arg(i));
}

void Config::set()
{
#ifdef Q_OS_WIN
	settings = new QSettings(QString("%1/%2.ini").arg(qApp->applicationDirPath(), QLatin1String(HYNE_NAME)), QSettings::IniFormat);
#else
	settings = new QSettings(QLatin1String(HYNE_NAME));
#endif
	if (int(_KeysSize) != KEYS_SIZE) {
		qWarning() << "Config: invalid keys size!";
		Q_ASSERT(false);
	}
}

bool Config::mode()
{
	return settings->value(keyToStr(Mode), false).toBool();
}

int Config::freq(const int freq_value)
{
	if (freq_auto())	return freq_value;
	return settings->value(keyToStr(Freq), 60).toUInt() == 50 ? 50 : 60;
}

bool Config::freq_auto()
{
	return settings->value(keyToStr(FreqAuto), true).toBool();
}

QString Config::value(Key key, const QString &defaultValue)
{
	return settings->value(keyToStr(key), defaultValue).toString();
}

void Config::setValue(Key key, const QVariant &value)
{
	settings->setValue(keyToStr(key), value);
}

QVariant Config::valueVar(Key key, const QVariant &defaultValue)
{
	return settings->value(keyToStr(key), defaultValue);
}

void Config::sync()
{
	settings->sync();
}

const QMap<FF8Installation::Type, FF8Installation> &Config::ff8Installations()
{
	if (!_ff8InstallationsSearched) {
		_ff8Installations = FF8Installation::installations();
		_selectedFF8Installation = FF8Installation::Type(settings->value(keyToStr(SelectedFF8Installation)).toInt());
		_ff8InstallationsSearched = true;
	}
	return _ff8Installations;
}

bool Config::ff8IsInstalled(bool &hasSlots)
{
	bool ret = false;

	hasSlots = false;

	for (const FF8Installation &installation : Config::ff8Installations()) {
		if (installation.isValid()) {
			ret = true;
		}

		if (installation.hasSlots()) {
			hasSlots = true;
		}

		if (hasSlots && ret) {
			break;
		}
	}

	return ret;
}

FF8Installation Config::ff8Installation()
{
	if (!_ff8InstallationsSearched) {
		ff8Installations();
	}
	return _ff8Installations.value(_selectedFF8Installation, _ff8Installations.constBegin().value());
}

void Config::setSelectedFF8Installation(FF8Installation::Type id)
{
	_selectedFF8Installation = id;
	settings->setValue(keyToStr(SelectedFF8Installation), id);
}
