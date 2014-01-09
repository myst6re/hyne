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
