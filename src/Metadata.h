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
#pragma once

#include <QtCore>

#define SAVE_FILES_KEY(slot, num)	\
		(((slot) << 8) | (num))
#define GET_SLOT(save_files_key)	\
		(save_files_key >> 8)
#define GET_NUM(save_files_key)		\
		(save_files_key & 0xFF)
#define METADATA_INDENT		2
#define TIMESTAMP_EMPTY		-2
#define TIMESTAMP_INVALID	-1

struct MetadataSaveFile
{
	QString signature;
	qint64 timestamp;
};

class Metadata
{
public:
	Metadata();
	explicit Metadata(const QString &filename);

	bool open();
	bool save();
	const QString &filename() const;
	void setFilename(const QString &filename);
	QString signature(quint8 slot, quint8 num) const;
	void updateSignature(quint8 slot, quint8 num, const QByteArray &saveData, const QString &userID);
	qint64 timestamp(quint8 slot, quint8 num) const;
	void setTimestamp(quint8 slot, quint8 num, qint64 timestamp);
	QString signature() const;
	void updateSignature(const QByteArray &saveData, const QString &userID);
	qint64 timestamp() const;
	void setTimestamp(qint64 timestamp);
	const QString &errorString() const;
private:
	static void writeSavefileContents(QXmlStreamWriter *xml, const MetadataSaveFile &saveFile);
	MetadataSaveFile createSaveFile(quint8 slot, quint8 num);
	MetadataSaveFile createSaveFile();
	void setErrorString(const QString &str);
	static QString md5sum(const QByteArray &lzsData, const QString &userID);

	QString _filename;
	QMap<quint16, MetadataSaveFile> _saveFiles;
	MetadataSaveFile _chocoFile;
	bool _hasChocoFile;
	QString _lastErrorString;
};
