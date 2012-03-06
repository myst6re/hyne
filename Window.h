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
#include "NewFileWizard.h"

class Window : public QWidget
{
    Q_OBJECT
public:
	enum OpenType { File, Slot1, Slot2 };

	explicit Window();
	~Window();

	void openFile(const QString &path);
	static QString chooseLangDialog();
	static QStringList availableLanguages();
public slots:
	void editView(SaveData *saveData);

private slots:
	void saveView();
	void newFile();
	void slot1();
	void slot2();
	void open(OpenType=File);
	void closeFile();
	void reload();
	void openRecentFile(QAction*);
	void save();
	void cancel();
	bool saveAs();
	void properties();
	void sorting();
	void mode(bool);
	void changeFrame(QAction *);
	void font(bool);
	void changeLanguage(QAction *);
	void runFF8();
	void fullScreen();
	void about();

private:
	bool saveAll();
	bool saveAs(Savecard::Type type, Savecard::Type newType, const QString &path);
	void fillMenuRecent();
	void restartNow();

	void setTitle(const bool editor=false);
	QList<int> selectSavesDialog(bool multiSelection=false);
	
	QMenuBar *menuBar;
	QAction *actionReload, *actionSaveAs, *actionProperties, *actionSorting, *actionClose, *actionMode, *actionFont;
	QMenu *menuRecent, *menuFrame, *menuLang;
	QStackedLayout *stackedLayout;
	Savecard *saves;
	QAbstractScrollArea *blackView;
	Editor *editor;
	quint8 currentSaveEdited;
	bool isPCSlot;
	bool isOpen;
	bool firstShow;

protected:
	void showEvent(QShowEvent *);
	void closeEvent(QCloseEvent *);
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);
//	void changeEvent(QEvent *);
};

#endif
