#ifndef METADATADIALOG_H
#define METADATADIALOG_H

#include <QtGui>
#include "HelpWidget.h"
#include "FF8Installation.h"

class MetadataDialog : public QDialog
{
	Q_OBJECT
public:
	explicit MetadataDialog(QWidget *parent = 0);
	explicit MetadataDialog(const QMap<FF8Installation::Type, FF8Installation> &ff8Installations, QWidget *parent = 0);
	void fill(const QMap<FF8Installation::Type, FF8Installation> &ff8Installations);
	inline QString metadataPath() const {
		return _path->text();
	}
	inline QString userID() const {
		return _userID->text();
	}

public slots:
	void setMetadataPath(const QString &path = QString());

private:
	void build();

	QWidget *_formWidget;
	QLineEdit *_path, *_userID;
	QPushButton *_pathButton;
};

#endif // METADATADIALOG_H
