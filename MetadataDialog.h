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
	void accept();
	void setMetadataPath(const QString &path = QString());

private:
	void build();

	QWidget *_formWidget;
	QLineEdit *_path, *_userID;
	QPushButton *_pathButton;
};

#endif // METADATADIALOG_H
