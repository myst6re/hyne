/****************************************************************************
 ** Hyne Final Fantasy VIII Save Editor
 ** Copyright (C) 2009-2012 Arzel Jérôme <myst6re@gmail.com>
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

QTranslator *Config::translator;
QStringList Config::recentFiles;
QSettings *Config::settings = 0;
QString Config::_ff8Path;

quint32 Config::sec(const quint32 &time, const int freq_value)
{
	return time%freq(freq_value);
}

quint32 Config::min(const quint32 &time, const int freq_value)
{
	return (time/freq(freq_value))%60;
}

quint32 Config::hour(const quint32 &time, const int freq_value)
{
	return time/(60*freq(freq_value));
}

quint32 Config::time(const quint32 &hour, const quint32 &min, const quint32 &sec, const int freq_value)
{
	int f = freq(freq_value);
	return sec + min*f + hour*f*60;
}

void Config::loadRecentFiles()
{
	recentFiles = settings->value("recentFiles").toStringList();
	// Compatibility with old version (< 1.6)
	if(recentFiles.isEmpty()) {
		for(int i=0 ; i<20 ; ++i) {
			QString filePath = settings->value(QString("recentFile%1").arg(i), QString()).toString();
			if(!filePath.isEmpty())
				recentFiles.append(QDir::cleanPath(filePath));
		}
	}
}

void Config::addRecentFile(const QString &filePath)
{
	int index = recentFiles.indexOf(filePath);
	if(index != -1)					recentFiles.removeAt(index);
	recentFiles.prepend(filePath);
	if(recentFiles.size() > 20)		recentFiles.removeLast();
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

	foreach(const QString &recentFile, recentFiles) {
		if(QFile::exists(recentFile)) {
			rf.append(QDir::cleanPath(recentFile));
			++i;
		}
		if(i > 20)	break;
	}

	settings->setValue("recentFiles", rf);
	// Compatibility with old version (< 1.6)
	for(i=0 ; i<20 ; ++i)
		settings->remove(QString("recentFile%1").arg(i));
}

void Config::set()
{
	settings = new QSettings(QString("%1/%2.ini").arg(qApp->applicationDirPath(), PROG_NAME), QSettings::IniFormat);
}

bool Config::mode()
{
	return settings->value("mode", false).toBool();
}

int Config::freq(const int freq_value)
{
	if(freq_auto())	return freq_value;
	return settings->value("freq", 60).toUInt()==50 ? 50 : 60;
}

bool Config::freq_auto()
{
	return settings->value("freq_auto", true).toBool();
}

QString Config::value(const QString &key, const QString &defaultValue)
{
	return settings->value(key, defaultValue).toString();
}

void Config::setValue(const QString &key, const QVariant &value)
{
	settings->setValue(key, value);
}

QVariant Config::valueVar(const QString &key, const QVariant &defaultValue)
{
	return settings->value(key, defaultValue);
}

void Config::sync()
{
	settings->sync();
}

const QString &Config::ff8Path()
{
	if(_ff8Path.isEmpty())
		_ff8Path = QDir::cleanPath( QSettings("Square Soft, Inc", "Final Fantasy VIII").value("1.00/AppPath").toString() );

	return _ff8Path;
}
