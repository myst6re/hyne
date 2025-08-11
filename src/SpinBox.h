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

#include <QSpinBox>

#define MAX_INT8			255
#define MAX_INT16			65535
#define MAX_INT32			(quint32)-1

class SpinBox8 : public QSpinBox
{
	Q_OBJECT
public:
	explicit SpinBox8(QWidget *parent = nullptr);
};

class SpinBox16 : public QSpinBox
{
	Q_OBJECT
public:
	explicit SpinBox16(QWidget *parent = nullptr);
};

class SpinBox32 : public QDoubleSpinBox
{
	Q_OBJECT
public:
	explicit SpinBox32(QWidget *parent = nullptr);
};
