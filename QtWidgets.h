#ifndef QTWIDGETS_H
#define QTWIDGETS_H

#include <QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#	include <QtWidgets>
#	define HEADER_VIEW_SET_RESIZE_MODE setSectionResizeMode
#else
#	include <QtGui>
#	define HEADER_VIEW_SET_RESIZE_MODE setResizeMode
#endif

#endif // QTWIDGETS_H
