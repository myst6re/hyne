#ifndef METADATA_H
#define METADATA_H

#include <QtCore>

#define SAVE_FILES_KEY(slot, num)	\
		(((slot) << 8) | (num))
#define GET_SLOT(save_files_key)	\
		(save_files_key >> 8)
#define GET_NUM(save_files_key)		\
		(save_files_key & 0xFF)
#define METADATA_INDENT	2
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
	void setFilename(const QString &filename);
	QString signature(quint8 slot, quint8 num) const;
	void updateSignature(quint8 slot, quint8 num, const QByteArray &saveData, const QString &userID);
	const QString &errorString() const;
private:
	void writeSavefileContents(QXmlStreamWriter *xml, const MetadataSaveFile &saveFile);
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

#endif // METADATA_H
