#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H

#include <QtGui>

class TimeWidget : public QWidget
{
	Q_OBJECT
public:
	explicit TimeWidget(QWidget *parent = 0);
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
