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
#include "TimeWidget.h"
#include "Config.h"

TimeWidget::TimeWidget(QWidget *parent) :
	QWidget(parent)
{
	_hour = new QSpinBox(this);
	_min = new QSpinBox(this);
	_min->setMaximum(59);
	_sec = new QSpinBox(this);

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(_hour);
	layout->addWidget(new QLabel(":", this));
	layout->addWidget(_min);
	layout->addWidget(new QLabel(":", this));
	layout->addWidget(_sec);
	layout->setContentsMargins(QMargins());

	connect(_hour, SIGNAL(valueChanged(int)), SIGNAL(valueChanged()));
	connect(_min, SIGNAL(valueChanged(int)), SIGNAL(valueChanged()));
	connect(_sec, SIGNAL(valueChanged(int)), SIGNAL(valueChanged()));
}

int TimeWidget::hour() const
{
	return _hour->value();
}

int TimeWidget::min() const
{
	return _min->value();
}

int TimeWidget::sec() const
{
	return _sec->value();
}

void TimeWidget::setTime(quint32 time, int freqValue)
{
	_sec->setMaximum(Config::freq(freqValue)-1);
	_sec->setValue(Config::sec(time, freqValue));
	_min->setValue(Config::min(time, freqValue));
	_hour->setMaximum(Config::freq(freqValue)==60 ? 1193046 : 1431655);
	_hour->setValue(Config::hour(time, freqValue));

	emit valueChanged();
}

quint32 TimeWidget::time(int freqValue) const
{
	return Config::time(hour(), min(), sec(), freqValue);
}
