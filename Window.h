/****************************************************************************
 ** Hyne Final Fantasy VIII Save Editor
 ** Copyright (C) 2009-2012 Arzel Jérôme <myst6re@gmail.com>
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

#ifndef DEF_WINDOW
#define DEF_WINDOW

#include <QtGui>
#include "Savecard.h"
#include "Editor.h"
#include "Parameters.h"
#include "SelectSavesDialog.h"
#include "StartWidget.h"

class Window : public QWidget
{
    Q_OBJECT
public:
	enum OpenType { File, Slot1, Slot2 };

	explicit Window();
	virtual ~Window();

	void openFile(const QString &path, bool isPCSlot=false);
	static QString chooseLangDialog();
	static QStringList availableLanguages();
public slots:
	void editView(SaveData *saveData);
	void setModified(bool modified=true);
private slots:
	void saveView();
	void newFile();
	inline void slot1() { open(Slot1); }
	inline void slot2() { open(Slot2); }
	void open(OpenType=File);
	bool closeFile(bool quit=false);
	void reload();
	void openRecentFile(QAction*);
	void save();
	bool exportAs();
	void properties();
	void mode(bool);
	void changeFrame(QAction *);
	void font(bool);
	void changeLanguage(QAction *);
	void runFF8();
	void fullScreen();
	void about();

private:
	void setIsOpen(bool open);
	bool exportAs(Savecard::Type newType, const QString &path);
	void fillMenuRecent();
	void restartNow();

	void setTitle(const bool editor=false);
	QList<int> selectSavesDialog(bool multiSelection=false);
	
	QMenuBar *menuBar;
	QAction *actionReload, *actionSave, *actionSaveAs;
	QAction *actionProperties, *actionClose, *actionMode, *actionFont;
	QMenu *menuRecent, *menuFrame, *menuLang;
	QStackedLayout *stackedLayout;
	Savecard *saves;
	StartWidget *startWidget;
	Editor *editor;

	quint8 currentSaveEdited;

protected:
	virtual void closeEvent(QCloseEvent *event);
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent *event);
//	virtual void changeEvent(QEvent *event);
};

#endif
