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
#ifndef QTASKBARBUTTON_H
#define QTASKBARBUTTON_H

#include <QtGlobal>
#if defined(Q_OS_WIN) && (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
#	include <QWinTaskbarButton>

typedef QWinTaskbarButton QTaskbarButton;

#else

#include <QObject>
#include <QString>
#include <QIcon>

class QTaskbarButton : public QObject
{
	Q_OBJECT
public:
	explicit QTaskbarButton(QObject *parent = 0) :
		QObject(parent) {
	}
	virtual ~QTaskbarButton() {
	}
	inline QString overlayAccessibleDescription() const {
		return _accessibleDescription;
	}
	inline QIcon overlayIcon() const {
		return _icon;
	}
	// QWinTaskbarProgress *progress() const;
public slots:
	inline void clearOverlayIcon() {
	}
	inline void setOverlayAccessibleDescription(const QString &description) {
		_accessibleDescription = description;
	}
	inline void setOverlayIcon(const QIcon &icon) {
		_icon = icon;
	}
private:
	QIcon _icon;
	QString _accessibleDescription;
};

#endif

#endif // QTASKBARBUTTON_H
