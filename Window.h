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

#ifndef DEF_WINDOW
#define DEF_WINDOW

#include <QtGui>
#include "SavecardWidget.h"
#include "Editor.h"
#include "Parameters.h"
#include "SelectSavesDialog.h"
#include "StartWidget.h"
#include "QTaskBarButton.h"

class Window : public QWidget
{
    Q_OBJECT
public:
	enum OpenType { File, Slot1, Slot2 };

	Window();
	virtual ~Window();

	void openFile(const QString &path, OpenType openType=File, const FF8Installation &installation=FF8Installation());
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
	void openRecentFile(QAction *);
	void save();
	bool exportAs();
	void properties();
	void mode(bool);
	void changeFrame(QAction *);
	void font(bool);
	void changeLanguage(QAction *);
	void changeFF8Version(QAction *);
	void runFF8();
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
	
	QTaskBarButton *taskBarButton; // Windows 7+
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
	virtual void closeEvent(QCloseEvent *event);
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent *event);
//	virtual void changeEvent(QEvent *event);
};

#endif
