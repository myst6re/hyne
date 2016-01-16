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
#include "SpinBox.h"

SpinBox8::SpinBox8(QWidget *parent) :
	QSpinBox(parent)
{
	setRange(0, MAX_INT8);
}

SpinBox16::SpinBox16(QWidget *parent) :
	QSpinBox(parent)
{
	setRange(0, MAX_INT16);
}

SpinBox32::SpinBox32(QWidget *parent) :
	QDoubleSpinBox(parent)
{
	setRange(0, MAX_INT32);
	setDecimals(0);
}
