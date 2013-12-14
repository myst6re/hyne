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

#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore>
#include "Parameters.h"
#include "FF8Installation.h"

class Config
{
public:
	static QString translationDir();
	static QString value(const QString &key, const QString &defaultValue=QString());
	static QVariant valueVar(const QString &key, const QVariant &defaultValue=QVariant());
	static void setValue(const QString &key, const QVariant &value);
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
	static const QList<FF8Installation> &ff8Installations();
	static FF8Installation ff8Installation();
	static void setSelectedFF8Installation(int id);

	static QTranslator *translator;
private:
	static QList<FF8Installation> _ff8Installations;
	static int _selectedFF8Installation;
	static bool _ff8InstallationsSearched;
	static QSettings *settings;
	static QStringList recentFiles;
};

#endif // CONFIG_H
