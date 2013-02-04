#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QtGui>

class HelpWidget : public QWidget
{
	Q_OBJECT
public:
	HelpWidget(int iconExtent, const QString &text, QWidget *parent=0);
	HelpWidget(int iconExtent, QWidget *parent=0);
	QString text() const;
	void setText(const QString &text);
private:
	void createLayout(int iconExtent, const QString &text);
	QLabel *textLabel;
};

#endif // HELPWIDGET_H
