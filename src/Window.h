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

#pragma once

#include <QtWidgets>
#include "SavecardWidget.h"
#include "Editor.h"
#include "StartWidget.h"
#include "QTaskBarButton.h"

class Window : public QWidget
{
    Q_OBJECT
public:
	enum OpenType { File, Slot1, Slot2, Dir };

	explicit Window(bool isNew = false);
	virtual ~Window();

	void openFile(const QString &path, OpenType openType=File, const FF8Installation &installation=FF8Installation());
	static QLocale chooseLangDialog();
	static QList<QLocale> availableLanguages();
public slots:
	void editView(SaveData *saveData);
	void setModified(bool modified=true);
private slots:
	void saveView();
	void newFile();
	inline void slot1() { open(Slot1); }
	inline void slot2() { open(Slot2); }
	inline void openDir() { open(Dir); }
	void open(OpenType=File);
	bool closeFile(bool quit=false);
	void reload();
	void notifyFileChanged(const QString &path);
	void openRecentFile(QAction *);
	void save();
	bool exportAs();
	void properties();
	void mode(bool);
	void changeFrame(QAction *);
	void font(bool);
	void changeLanguage(QAction *);
	void changeFF8Version(QAction *);
#ifndef Q_OS_WINRT
	void runFF8();
#endif
	void updateMetadata();
	void fullScreen();
	void about();
	static void newWindow();

private:
	void setIsOpen(bool open);
	bool exportAs(SavecardData::Type newType, const QString &path);
	QByteArray descGme(const QString &desc, bool *abort);
	void fillMenuRecent();
	void restartNow();

	void setTitle(const int currentSaveEdited=-1);
	QList<int> selectSavesDialog(bool multiSelection=false, bool onlyFF8=false);

	QTaskBarButton *taskbarButton;
	QMenuBar *menuBar;
	QAction *actionReload, *actionSave, *actionSaveAs;
	QAction *actionProperties, *actionClose, *actionMode, *actionFont;
	QMenu *menuRecent, *menuFrame, *menuLang, *menuVersion;
	QStackedLayout *stackedLayout;
	SavecardData *saves;
	SavecardWidget *saveList;
	StartWidget *startWidget;
	Editor *editor;

protected:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent *event);
//	virtual void changeEvent(QEvent *event);
};
