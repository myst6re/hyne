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
#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H

#include <QtWidgets>

class TimeWidget : public QWidget
{
	Q_OBJECT
public:
	explicit TimeWidget(QWidget *parent = nullptr);
	int hour() const;
	int min() const;
	int sec() const;
	void setTime(quint32 time, int freqValue);
	quint32 time(int freqValue) const;
signals:
	void valueChanged();
private:
	QSpinBox *_hour, *_min, *_sec;
};

#endif // TIMEWIDGET_H
