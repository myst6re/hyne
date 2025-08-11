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

#pragma once

#include <QtCore>
#include "FF8Installation.h"

#define KEYS_SIZE 13

class Config
{
public:
	enum Key {
		RecentFiles, Lang, Geometry, // Application
		Font, Freq, FreqAuto, Mode, LastCountry, LastGameCode, SelectedFF8Installation, // FF8
		LoadPath, SavePath, SavePathIcon, // Load and save
		_KeysSize
	};

	static QString translationDir();
	static QString value(Key key, const QString &defaultValue=QString());
	static QVariant valueVar(Key key, const QVariant &defaultValue=QVariant());
	static void setValue(Key key, const QVariant &value);
	static void sync();
	static quint32 sec(quint32 time, int freq_value);
	static quint32 min(quint32 time, int freq_value);
	static quint32 hour(quint32 time, int freq_value);
	static quint32 time(quint32 hour, quint32 min, quint32 sec, int freq_value);
	static bool mode();
	static int freq(const int freq_value=0);
	static bool freq_auto();
	static void loadRecentFiles();
	static void addRecentFile(const QString &path);
	static const QString &recentFile(const uint &index);
	static bool hasRecentFiles();
	static int recentFilesSize();
	static void saveRecentFiles();
	static void set();
	static const QMap<FF8Installation::Type, FF8Installation> &ff8Installations();
	static FF8Installation ff8Installation();
	static bool ff8IsInstalled(bool &hasSlots);
	static void setSelectedFF8Installation(FF8Installation::Type id);

	static QTranslator *translator;
private:
	static inline QString keyToStr(Key key) {
		return keys[int(key)];
	}
	static QMap<FF8Installation::Type, FF8Installation> _ff8Installations;
	static FF8Installation::Type _selectedFF8Installation;
	static bool _ff8InstallationsSearched;
	static QSettings *settings;
	static QStringList recentFiles;
	static const char *keys[KEYS_SIZE];
};
