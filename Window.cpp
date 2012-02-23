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

#include "Window.h"

Window::Window()
	: QWidget(), currentSaveEdited(0), isPCSlot(false), isOpen(false), firstShow(true)
{
	setTitle();
	setMinimumSize(768, 502);
	resize(768, 502);
	setWindowIcon(QIcon(":/images/hyne.png"));
	setAcceptDrops(true);
	
	menuBar = new QMenuBar(this);
	QMenu *menu;
	QAction *action;

	/* MENU 'FICHIER' */
	
	menu = menuBar->addMenu(tr("&Fichier"));

	bool isInstalled = !Config::ff8Path().isEmpty();
	
	menu->addAction(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton), tr("&Ouvrir..."), this, SLOT(open()), QKeySequence::Open);
	actionReload = menu->addAction(QApplication::style()->standardIcon(QStyle::SP_BrowserReload), tr("&Recharger depuis le disque"), this, SLOT(reload()), QKeySequence::Refresh);
	actionReload->setEnabled(false);
	actionSaveAs = menu->addAction(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton), tr("E&xporter..."), this, SLOT(saveAs()), QKeySequence::SaveAs);
	actionSaveAs->setEnabled(false);
	menu->addSeparator();
	actionProperties = menu->addAction(tr("Propriétés..."), this, SLOT(properties()));
	actionProperties->setEnabled(false);
	actionSorting = menu->addAction(tr("Modifier l'ordre des saves..."), this, SLOT(sorting()));
	actionSorting->setEnabled(false);
	action = menu->addAction(QIcon(":/images/ff8.png"), tr("Lancer Final Fantasy VIII"), this, SLOT(runFF8()), Qt::Key_F8);
	action->setShortcutContext(Qt::ApplicationShortcut);
	action->setEnabled(isInstalled);
	addAction(action);
	action = menu->addAction(tr("Plein écran"), this, SLOT(fullScreen()), Qt::Key_F11);
	action->setShortcutContext(Qt::ApplicationShortcut);
	addAction(action);
	menuRecent = menu->addMenu(tr("&Ouverts récemment"));
	fillMenuRecent();
	connect(menuRecent, SIGNAL(triggered(QAction*)), SLOT(openRecentFile(QAction*)));
	menu->addSeparator();
	actionClose = menu->addAction(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton), tr("&Fermer"), this, SLOT(closeFile()), QKeySequence::Close);
	actionClose->setEnabled(false);
	menu->addAction(tr("&Quitter"), this, SLOT(close()), QKeySequence::Quit);
	
	/* MENU 'SLOT' */
	
	action = menuBar->addAction(tr("Fente &1"), this, SLOT(slot1()));
	action->setEnabled(isInstalled);
	action = menuBar->addAction(tr("Fente &2"), this, SLOT(slot2()));
	action->setEnabled(isInstalled);
	
	/* MENU 'PARAMETRES' */
	
	menu = menuBar->addMenu(tr("&Paramètres"));
	
	actionMode = menu->addAction(tr("Mode Avancé"), this, SLOT(mode(bool)));
	actionMode->setCheckable(true);
	actionMode->setChecked(Config::mode());
	
	menuFrame = menu->addMenu(tr("Images par seconde"));
		action = menuFrame->addAction(tr("Auto"));
		action->setCheckable(true);
		action->setChecked(Config::freq_auto());
		action->setData(0);
		action = menuFrame->addAction(tr("NTSC/PC (60 images/s)"));
		action->setCheckable(true);
		action->setChecked(Config::freq()==60);
		action->setData(60);
		action = menuFrame->addAction(tr("PAL (50 images/s)"));
		action->setCheckable(true);
		action->setChecked(Config::freq()==50);
		action->setData(50);
	connect(menuFrame, SIGNAL(triggered(QAction*)), SLOT(changeFrame(QAction*)));
	
	actionFont = menu->addAction(tr("Police haute résolution"), this, SLOT(font(bool)));
	actionFont->setCheckable(true);
	actionFont->setChecked(!Config::value("font").isEmpty());
	
	menuLang = menu->addMenu(tr("&Langues"));
		QDir dir(qApp->applicationDirPath());
		QStringList stringList = dir.entryList(QStringList("hyne_*.qm"), QDir::Files, QDir::Name);
		action = menuLang->addAction("Français");
		action->setData("fr");
		action->setCheckable(true);
		action->setChecked(Config::value("lang")=="fr");

		QTranslator translator;
		foreach(QString str, stringList) {
			translator.load(str, qApp->applicationDirPath());
			action = menuLang->addAction(translator.translate("Window", "Français", "Your translation language"));
			str = str.mid(5);
			str = str.left(str.size()-3);
			action->setData(str);
			action->setCheckable(true);
			action->setChecked(Config::value("lang")==str);
		}
	connect(menuLang, SIGNAL(triggered(QAction*)), SLOT(changeLanguage(QAction*)));
	
	/* MENU '?' */
	
	menuBar->addAction(tr("?"), this, SLOT(about()));
	
	blackView = new QAbstractScrollArea(this);
	blackView->setPalette(QPalette(Qt::black));
	blackView->setFrameShape(QFrame::NoFrame);
//	blackView->setAlignment(Qt::AlignCenter);
//	blackView->setWidget(new StartWidget(blackView));
	
	editor = new Editor(this);
	editor->hide();

	connect(editor, SIGNAL(accepted()), SLOT(save()));
	connect(editor, SIGNAL(rejected()), SLOT(cancel()));
	
	stackedLayout = new QStackedLayout;
	stackedLayout->addWidget(blackView);
	stackedLayout->addWidget(editor);
	
	QVBoxLayout *gridLayout = new QVBoxLayout(this);
	gridLayout->setMenuBar(menuBar);
	gridLayout->addLayout(stackedLayout, 1);
	gridLayout->setContentsMargins(QMargins());

	restoreGeometry(Config::valueVar("geometry").toByteArray());
}

Window::~Window()
{
	Config::setValue("geometry", saveGeometry());
	Config::saveRecentFiles();
	Config::sync();// flush data
}

void Window::showEvent(QShowEvent *)
{
	if(firstShow && !windowState().testFlag(Qt::WindowMaximized)) {
		QPoint screenCenter = QApplication::desktop()->screenGeometry(this).center();
		move(screenCenter.x() - width()/2, screenCenter.y() - height()/2);
	}
	firstShow = false;
}

void Window::closeEvent(QCloseEvent *event)
{
	if(stackedLayout->currentWidget()==editor) {
		cancel();
		event->ignore();
	}
	else
		event->accept();
}

void Window::dragEnterEvent(QDragEnterEvent *event)
{
	if(stackedLayout->currentWidget()==blackView
	   || stackedLayout->currentWidget()==saves)
		event->acceptProposedAction();
}

void Window::dropEvent(QDropEvent *event)
{
	if(stackedLayout->currentWidget()==editor)	return;

	const QMimeData *mimeData = event->mimeData();

	if(mimeData->hasUrls())
	{
		QList<QUrl> urlList = mimeData->urls();
		if(!urlList.isEmpty()) {
			QString path = QDir::cleanPath(urlList.first().path());
			if(!path.startsWith(QDir::rootPath()))
				path = path.mid(1);
			if(path.startsWith(QDir::rootPath())) {
				this->isPCSlot = false;
				openFile(path);
			}
		}
	}

	event->acceptProposedAction();
}

void Window::setTitle(const bool editor)
{
	setWindowTitle(PROG_FULLNAME %
				   (Data::currentPath.isEmpty() ? QString() : " - "%Data::currentPath) %
				   (editor ? tr(" - save %1").arg(currentSaveEdited+1, 2, 10, QChar('0')) : QString())
				   );
}

void Window::slot1()
{
	open(Slot1);
}

void Window::slot2()
{
	open(Slot2);
}

void Window::open(OpenType slot)
{
	QString path = Config::ff8Path();

	if(slot==Slot1 || slot==Slot2)
	{
		if(path.isEmpty())	return;

		path.append(QString("/Save/Slot%1/").arg((int)slot));

		this->isPCSlot = true;
	}
	else
	{
		if(!Config::value("loadPath").isEmpty())
			path = Config::value("loadPath");
		else if(!path.isEmpty())
			path.append("/Save");
		path = QFileDialog::getOpenFileName(this, tr("Ouvrir"), path, tr("Fichiers compatibles (*.mcr *.ddf *.gme *.mc *.mcd *.mci *.ps *.psm *.psv save?? *.mem *.vgs *.vmp);;FF8 PS memorycard (*.mcr *.ddf *.mc *.mcd *.mci *.ps *.psm);;FF8 PC save (save??);;FF8 vgs memorycard (*.mem *.vgs);;FF8 gme memorycard (*.gme);;FF8 PSN memorycard (*.vmp);;FF8 PS3 memorycard (*.psv);;Tous les fichiers (*)"));
		if(path.isNull())		return;

		int index = path.lastIndexOf('/');
		Config::setValue("loadPath", index == -1 ? path : path.left(index));

		this->isPCSlot = false;
	}
	
	openFile(path);
}

void Window::closeFile()
{
	stackedLayout->setCurrentWidget(blackView);
    editor->hide();
	if(this->isOpen)
    {
		isOpen = false;
		delete saves;
    }
    currentSaveEdited = 0;
    actionClose->setEnabled(false);
    actionSaveAs->setEnabled(false);
	actionProperties->setEnabled(false);
	actionSorting->setEnabled(false);
	actionReload->setEnabled(false);
	Data::currentPath = QString();
	setTitle();
}

void Window::openFile(const QString &path)
{
	QString cleanPath = QDir::cleanPath(path);
	closeFile();
	saves = new Savecard(cleanPath, this, this->isPCSlot);

	Data::currentPath = cleanPath;

	this->isOpen = true;

	if(!saves->ok())
		closeFile();
	else
	{
		stackedLayout->addWidget(saves);
		stackedLayout->setCurrentWidget(saves);
		actionSaveAs->setEnabled(true);
		actionProperties->setEnabled(true);
		actionSorting->setEnabled(!saves->isOneSaveType());
		actionClose->setEnabled(true);
		actionReload->setEnabled(true);
		setTitle();

		if(!isPCSlot) {
			Config::addRecentFile(cleanPath);
			fillMenuRecent();
		}
	}
}

void Window::openRecentFile(QAction *action)
{
	this->isPCSlot = false;
	openFile( Config::recentFile(action->data().toUInt()) );
}

void Window::reload()
{
	QString curPath = Data::currentPath;
	closeFile();
	openFile(curPath);
}

bool Window::saveAs()
{
	QString types, path, selectedFilter,
			ps = tr("PS memorycard (*.mcr *.ddf *.mc *.mcd *.mci *.ps *.psm)"),
			vgs = tr("VGS memorycard (*.vgs *.mem)"),
			gme = tr("GME memorycard (*.gme)"),
			vmp = tr("PSN memorycard (*.vmp)"),
			pc = tr("FF8 PC save (*)"),
			psv = tr("PSN save (*.psv)");
	Savecard::Type type = saves->type(), newType;

	if(type == Savecard::PcDir || type == Savecard::Pc)
		types = pc+";;"+ps+";;"+vgs+";;"+gme+";;"+vmp;
	else if(type == Savecard::Psv)
		types = psv+";;"+ps+";;"+vgs+";;"+gme+";;"+vmp;
	else
	{
		types = ps+";;"+vgs+";;"+gme+";;"+vmp+";;"+pc;
		if(type == Savecard::Vgs)		selectedFilter = vgs;
		else if(type == Savecard::Gme)	selectedFilter = gme;
		else if(type == Savecard::Vmp)	selectedFilter = vmp;
	}
	
	path = Config::value("savePath").isEmpty() ? saves->dirname() : Config::value("savePath")+"/";
	path = QFileDialog::getSaveFileName(this, tr("Exporter"), path+saves->name(), types, &selectedFilter);
	if(path.isNull())		return false;

	if(selectedFilter == ps)			newType = Savecard::Ps;
	else if(selectedFilter == vgs)		newType = Savecard::Vgs;
	else if(selectedFilter == gme)		newType = Savecard::Gme;
	else if(selectedFilter == vmp)		newType = Savecard::Vmp;
	else if(selectedFilter == pc)		newType = Savecard::Pc;
	else if(selectedFilter == psv)		newType = Savecard::Psv;
	else	return false;

	if(newType == Savecard::Vmp || newType == Savecard::Psv) {
		int reponse = QMessageBox::information(this, tr("Sauvegarde hasardeuse"), tr("Les formats VMP et PSV sont protégés, l'enregistrement sera partiel et risque de ne pas fonctionner.\nContinuer quand même ?"), tr("Oui"), tr("Non"));
		if(reponse != 0)  return saveAs();
	}
	
	int index = path.lastIndexOf('/');
	Config::setValue("savePath", index == -1 ? path : path.left(index));

	return saveAs(type, newType, path);
}

bool Window::saveAll()
{
	return saveAs(saves->type(), saves->type(), saves->path());
}

bool Window::saveAs(Savecard::Type type, Savecard::Type newType, const QString &path)
{
	if(newType != Savecard::Pc)
	{
		if(type == Savecard::PcDir) {
			if(newType == Savecard::PcDir) {
				saves->saveDir();
			}
			else {
				QList<int> selected_files = selectSavesDialog(true);
				if(selected_files.isEmpty())	return false;
				saves->save2PS(selected_files, path, newType);
			}
		}
		else if(type == Savecard::Pc || (type == Savecard::Psv && newType != Savecard::Psv))
			saves->save2PS(QList<int>() << 0, path, newType);
		else
			saves->save(path, newType);
	}
	else
	{
		if(type == Savecard::Pc)
			saves->saveOne(0, path);
		else {
			QList<int> selected_files = selectSavesDialog();
			if(selected_files.isEmpty())	return false;
			int id = selected_files.first();
			saves->setName(QString("save%1").arg(id+1, 2, 10, QChar('0')));
			saves->saveOne(id, path);
		}
	}

	return true;
}

void Window::properties()
{
	QList<int> selected_files = selectSavesDialog();
	if(selected_files.isEmpty())	return;

	saves->saveWidget(selected_files.first())->properties();
}

void Window::sorting()
{
	SelectSavesDialog *dialog = new SelectSavesDialog(saves->getSaves(), this);

	if(dialog->exec() == QDialog::Accepted) {
		saves->setSlotOrder(dialog->order());
		saveAll();
	}
}

QList<int> Window::selectSavesDialog(bool multiSelection)
{
	if(saves->isOneSaveType())	 return QList<int>() << 0;
	SelectSavesDialog *dialog = new SelectSavesDialog(saves->getSaves(), multiSelection, this);

	if(dialog->exec() == QDialog::Accepted) {
		return dialog->selectedSaves();
	}
	return QList<int>();
}

void Window::fillMenuRecent()
{
	menuRecent->clear();
	
	for(int i=0 ; i<Config::recentFilesSize() ; ++i)
		menuRecent->addAction(tr("%1 : %2").arg(i+1,2,10).arg(Config::recentFile(i)))->setData(i);

	menuRecent->setEnabled(Config::hasRecentFiles());
}

void Window::editView(SaveData *saveData)
{
	menuBar->hide();
	editor->show();
	editor->load(saveData, saves->type()==Savecard::Pc || saves->type()==Savecard::PcDir);
	currentSaveEdited = saveData->id();
	stackedLayout->setCurrentWidget(editor);
	setTitle(true);
	setWindowIcon(QIcon(saveData->icon()));
}

void Window::saveView()
{
	stackedLayout->setCurrentWidget(saves);
	editor->hide();
	menuBar->show();
	setWindowIcon(QIcon(":/images/hyne.png"));
	setTitle();
}

void Window::save()
{
	bool saved = true;

	if(this->isPCSlot)
	{
		saves->saveDir(currentSaveEdited);
	}
	else
	{
		Savecard::Type type = saves->type();
		if(type == Savecard::Vmp || type == Savecard::Psv)
			saved = saveAs();
		else if(type == Savecard::Pc)
			saves->saveOne(0);
		else
			saves->save();
	}
	if(saved) {
		saveView();
		saves->updateSaveWidget(currentSaveEdited, true);
	}
}

void Window::cancel()
{
	saveView();
	saves->updateSaveWidget(currentSaveEdited);
}

void Window::mode(bool mode)
{
	Config::setValue("mode", mode);
	if(this->isOpen)
	{
		editor->updateMode(mode);
	}
}

void Window::changeFrame(QAction *action)
{
	if(action->data().toInt()==0)
		Config::setValue("freq_auto", true);
	else {
		Config::setValue("freq", action->data());
		Config::setValue("freq_auto", false);
	}

	foreach(QAction *act, menuFrame->actions())
		act->setChecked(false);
	action->setChecked(true);

	if(this->isOpen)
	{
		editor->updateTime();
		saves->updateSaveWidgets();
	}
}

void Window::font(bool font)
{
	Config::setValue("font", font ? "hr" : "");
	FF8Text::fontImage = QImage(QString(":/images/font%1.png").arg(font ? "hr" : ""));
	if(this->isOpen)
	{
		saves->updateSaveWidgets();
	}
}

void Window::changeLanguage(QAction *action)
{
	if(Config::value("lang") != action->data()) {
		Config::setValue("lang", action->data());
		foreach(QAction *act, menuLang->actions())
			act->setChecked(false);
		action->setChecked(true);

		restartNow();
	} else {
		action->setChecked(true);
	}
}

void Window::restartNow()
{
	QString str_title, str_text;
	if(Config::translator->load("hyne_" + Config::value("lang"), qApp->applicationDirPath())) {
		str_title = Config::translator->translate("Window", "Paramètres modifiés");
		str_text = Config::translator->translate("Window", "Relancez le programme pour que les paramètres prennent effet.");
    }
    else {
        str_title = "Paramètres modifiés";
        str_text = "Relancez le programme pour que les paramètres prennent effet.";
    }
	Data::load();
    QMessageBox::information(this, str_title, str_text);
}

/*void Window::changeEvent(QEvent *e)
{
	if(e->type() == QEvent::LanguageChange) {

	} else {
		QWidget::changeEvent(e);
	}
}*/

void Window::runFF8()
{
	if(!QProcess::startDetached("\"" % Config::ff8Path() % "/FF8.exe\"")) {
		QMessageBox::warning(this, tr("Erreur"), tr("Final Fantasy VIII n'a pas pu être lancé.\n%1").arg(Config::ff8Path() % "/FF8.exe"));
	}
}

void Window::fullScreen()
{
	if(isFullScreen())	showNormal();
	else				showFullScreen();
}

void Window::about()
{
	QDialog about(this, Qt::Dialog | Qt::CustomizeWindowHint);
	about.setFixedSize(224, 248);
	
	QFont font;
	font.setPointSize(12);
	
	QLabel image(&about);
	image.setPixmap(QPixmap(":/images/about.png"));
	image.move(about.width()-20-image.sizeHint().height(), 49);
	
	QLabel desc1(PROG_FULLNAME, &about);
	desc1.setFont(font);
	desc1.setFixedWidth(200);
	desc1.setAlignment(Qt::AlignHCenter);

	font.setPointSize(8);

	QLabel desc2(tr("Par myst6re\nmyst6re@gmail.com\n\n75% modifiable\n\nMerci à :\n - Qhimm\n - Cyberman\n - sithlord48\n - Aladore384\n\nTraducteurs :\n - Anglais : myst6re, Vgr\n - Japonais : Asa"), &about);
	desc2.move(9, 24);
	desc2.setFont(font);

	QLabel desc3(QString("Qt %1").arg(QT_VERSION_STR), &about);
	QPalette pal = desc3.palette();
	pal.setColor(QPalette::WindowText, QColor(0xAA,0xAA,0xAA));
	desc3.setPalette(pal);
	desc3.move(about.width()-8-desc3.sizeHint().width(), about.height()-8-desc3.sizeHint().height());
	desc3.setFont(font);
	
	QPushButton button(tr("Fermer"), &about);
	button.move(8, about.height()-8-button.sizeHint().height());
	connect(&button, SIGNAL(released()), &about, SLOT(close()));
	
	about.exec();
}
