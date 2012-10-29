#ifndef QTASKBARBUTTON_H
#define QTASKBARBUTTON_H

#include <QtGui>
#ifdef Q_WS_WIN
#include "shobjidl.h"
#endif

class QTaskBarButton : public QObject
{
    Q_OBJECT
public:
	enum State {
		Invisible, Normal, Indeterminate, Paused, Error
	};

	enum ListCategory {
		Frequent=0x01, Recent=0x02
	};
	Q_DECLARE_FLAGS(ListCategories, ListCategory)

	explicit QTaskBarButton(QWidget *mainWindow);
	virtual ~QTaskBarButton();
	void addList(ListCategories category);
	void clearList();
	void setOverlayIcon(const QPixmap &pixmap, const QString &text=QString());
	void setState(State state);
	int maximum() const;
	int minimum() const;
	State state() const;
	int value() const;
	static void addToRecentDocs(const QString &path);
signals:
	void valueChanged(int value);
public slots:
	void reset();
	void setMaximum(int maximum);
	void setMinimum(int minimum);
	void setRange(int minimum, int maximum);
	void setValue(int value);
private:
	void initDestinationList();
#ifdef Q_WS_WIN
	WId _winId;
	ITaskbarList3 *pITask;
	ICustomDestinationList *destinationList;
	IObjectArray *removedItems;
#endif // Q_WS_WIN
	int _minimum, _maximum, _value;
	State _state;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QTaskBarButton::ListCategories)

#endif // QTASKBARBUTTON_H
