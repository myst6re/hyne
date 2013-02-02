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
