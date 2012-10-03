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

	void openFile(const QString &path);
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
	bool saveAs();
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
	bool saveAs(Savecard::Type newType, const QString &path);
	void fillMenuRecent();
	void restartNow();

	void setTitle(const bool editor=false);
	QList<int> selectSavesDialog(bool multiSelection=false);
	
	QMenuBar *menuBar;
	QAction *actionReload, *actionSave, *actionSaveAs, *actionProperties, *actionClose, *actionMode, *actionFont;
	QMenu *menuRecent, *menuFrame, *menuLang;
	QStackedLayout *stackedLayout;
	Savecard *saves;
	QAbstractScrollArea *blackView;
	Editor *editor;
	quint8 currentSaveEdited;
	bool isPCSlot;
	bool isOpen;
	bool firstShow;
	QIcon savWindowIcon;

protected:
	void showEvent(QShowEvent *);
	void closeEvent(QCloseEvent *);
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);
//	void changeEvent(QEvent *);
};

#endif
