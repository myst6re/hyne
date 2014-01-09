#ifndef SPINBOX_H
#define SPINBOX_H

#include <QSpinBox>

#define MAX_INT8			255
#define MAX_INT16			65535
#define MAX_INT32			(quint32)-1

class SpinBox8 : public QSpinBox
{
	Q_OBJECT
public:
	explicit SpinBox8(QWidget *parent = 0);
};

class SpinBox16 : public QSpinBox
{
	Q_OBJECT
public:
	explicit SpinBox16(QWidget *parent = 0);
};

class SpinBox32 : public QDoubleSpinBox
{
	Q_OBJECT
public:
	explicit SpinBox32(QWidget *parent = 0);
};

#endif // SPINBOX_H
