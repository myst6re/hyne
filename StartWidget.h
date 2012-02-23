#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QtGui>
#include "FF8Text.h"
#include "SaveWidget.h"

class StartWidget : public QWidget
{
	Q_OBJECT
public:
	explicit StartWidget(QWidget *parent = 0);
	
signals:
	
public slots:
	
protected:
	virtual void paintEvent(QPaintEvent *);
};

#endif // STARTWIDGET_H
