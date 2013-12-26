#ifndef USERDIRECTORY_H
#define USERDIRECTORY_H

#include <QtCore>
#include "Metadata.h"

class UserDirectory
{
public:
	UserDirectory();
	explicit UserDirectory(const QString &dirname);
	UserDirectory(const QString &metadataPath, const QString &userID);
	inline const Metadata &metadata() const {
		return _metadata;
	}
	bool isValid() const;
	bool hasMetadata() const;
	bool hasUserId() const;
	bool openMetadata();
	void updateMetadata(quint8 slot, quint8 num, const QByteArray &saveData = QByteArray());
	bool updateSignatures();
	bool saveMetadata();
	void setDirname(const QString &dirname);
	inline const QString &userID() const {
		return _userID;
	}
	const QString &errorString() const;
private:
	static QString extractUserID(const QString &dirname);

	Metadata _metadata;
	QString _dirname;
	QString _userID;
};

#endif // USERDIRECTORY_H
