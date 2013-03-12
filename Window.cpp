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

#include "Window.h"
#include "HeaderDialog.h"

Window::Window() :
	QWidget(), taskBarButton(0), saves(0), saveList(0), editor(0)
{
	setTitle();
	setMinimumSize(768, 502);
	resize(768, 502);
	setAcceptDrops(true);

#ifdef Q_OS_WIN
	if(QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
		taskBarButton = new QTaskBarButton(this);
		//taskBarButton->addList(QTaskBarButton::Recent | QTaskBarButton::Frequent);
	}
#endif

	menuBar = new QMenuBar(0);
	QMenu *menu;
	QAction *action, *actionSlot1=0, *actionSlot2=0;

	/* MENU 'FICHIER' */
	
	menu = menuBar->addMenu(tr("&Fichier"));
#ifdef Q_OS_MAC
	QMenu *fileMenu = menu;
#endif

	bool isInstalled = !Config::ff8Path().isEmpty();
	
	QAction *actionNew = menu->addAction(tr("&Nouveau..."), this, SLOT(newFile()), QKeySequence::New);
	QAction *actionOpen = menu->addAction(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton), tr("&Ouvrir..."), this, SLOT(open()), QKeySequence::Open);
	actionReload = menu->addAction(QApplication::style()->standardIcon(QStyle::SP_BrowserReload), tr("&Recharger depuis le disque"), this, SLOT(reload()), QKeySequence::Refresh);
	actionReload->setEnabled(false);
	actionSave = menu->addAction(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton), tr("&Enregistrer"), this, SLOT(save()), QKeySequence::Save);
	actionSave->setEnabled(false);
	actionSaveAs = menu->addAction(tr("E&xporter..."), this, SLOT(exportAs()), QKeySequence::SaveAs);
	actionSaveAs->setEnabled(false);
	menu->addSeparator();
	actionProperties = menu->addAction(tr("&Propriétés..."), this, SLOT(properties()));
	actionProperties->setEnabled(false);
	if(isInstalled) {
		action = menu->addAction(QIcon(":/images/ff8.png"), tr("&Lancer Final Fantasy VIII"), this, SLOT(runFF8()), Qt::Key_F8);
		action->setShortcutContext(Qt::ApplicationShortcut);
		addAction(action);
	}
	menu->addAction(tr("Nouvelle fenetre"), this, SLOT(newWindow()));
	action = menu->addAction(tr("Ple&in écran"), this, SLOT(fullScreen()), Qt::Key_F11);
	action->setShortcutContext(Qt::ApplicationShortcut);
	addAction(action);
	menuRecent = menu->addMenu(tr("O&uverts récemment"));
	fillMenuRecent();
	connect(menuRecent, SIGNAL(triggered(QAction*)), SLOT(openRecentFile(QAction*)));
	menu->addSeparator();
	actionClose = menu->addAction(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton), tr("&Fermer"), this, SLOT(closeFile()), QKeySequence::Close);
	actionClose->setEnabled(false);
	menu->addAction(tr("&Quitter"), this, SLOT(close()), QKeySequence::Quit)->setMenuRole(QAction::QuitRole);
	
	/* MENU 'SLOT' */
	
	if(isInstalled) {
		actionSlot1 = menuBar->addAction(tr("Fente &1"), this, SLOT(slot1()));
		actionSlot2 = menuBar->addAction(tr("Fente &2"), this, SLOT(slot2()));
	}

	/* MENU 'PARAMETRES' */
	
	menu = menuBar->addMenu(tr("&Paramètres"));
	
	actionMode = menu->addAction(tr("&Mode Avancé"), this, SLOT(mode(bool)));
	actionMode->setCheckable(true);
	actionMode->setChecked(Config::mode());
	
	menuFrame = menu->addMenu(tr("&Images par seconde"));
		action = menuFrame->addAction(tr("&Auto"));
		action->setCheckable(true);
		action->setChecked(Config::freq_auto());
		action->setData(0);
		action = menuFrame->addAction(tr("&NTSC/PC (60 images/s)"));
		action->setCheckable(true);
		action->setChecked(Config::freq()==60);
		action->setData(60);
		action = menuFrame->addAction(tr("&PAL (50 images/s)"));
		action->setCheckable(true);
		action->setChecked(Config::freq()==50);
		action->setData(50);
	connect(menuFrame, SIGNAL(triggered(QAction*)), SLOT(changeFrame(QAction*)));
	
	actionFont = menu->addAction(tr("&Police haute résolution"), this, SLOT(font(bool)));
	actionFont->setCheckable(true);
	actionFont->setChecked(!Config::value("font").isEmpty());
	
	menuLang = menu->addMenu(tr("&Langues"));
	foreach(const QString &str, availableLanguages()) {
		action = menuLang->addAction(str.left(str.lastIndexOf("|")));
		QString lang = str.mid(str.lastIndexOf("|")+1);
		action->setData(lang);
		action->setCheckable(true);
		action->setChecked(Config::value("lang")==lang);
	}
	connect(menuLang, SIGNAL(triggered(QAction*)), SLOT(changeLanguage(QAction*)));
	
	/* MENU '?' */
	
#ifndef Q_OS_MAC
	menuBar->addAction(tr("&?"), this, SLOT(about()))->setMenuRole(QAction::AboutRole);
#else
	fileMenu->addAction(tr("&?"), this, SLOT(about()))->setMenuRole(QAction::AboutRole);
#endif

	startWidget = new StartWidget(this);
	startWidget->addAction(actionNew);
	startWidget->addAction(actionOpen);
	if(isInstalled) {
		startWidget->addAction(actionSlot1);
		startWidget->addAction(actionSlot2);
	}
	
	stackedLayout = new QStackedLayout(this);
	stackedLayout->setMenuBar(menuBar);
	stackedLayout->addWidget(startWidget);

	restoreGeometry(Config::valueVar("geometry").toByteArray());
}

Window::~Window()
{
	Config::setValue("geometry", saveGeometry());
	Config::saveRecentFiles();
	Config::sync();// flush data
}

void Window::closeEvent(QCloseEvent *event)
{
	if(stackedLayout->currentWidget() == editor) {
		saveView();
		event->ignore();
	}
	else if(!closeFile(true))
		event->ignore();
	else
		event->accept();
}

void Window::dragEnterEvent(QDragEnterEvent *event)
{
	if(event->mimeData()->hasUrls() &&
			(stackedLayout->currentWidget() == startWidget
			 || (saveList && stackedLayout->currentWidget() == saveList)))
		event->acceptProposedAction();
}

void Window::dropEvent(QDropEvent *event)
{
	if(stackedLayout->currentWidget() == editor)	return;

	const QMimeData *mimeData = event->mimeData();

	if(mimeData->hasUrls())
	{
		QList<QUrl> urlList = mimeData->urls();
		if(!urlList.isEmpty()) {
			QString path = QDir::cleanPath(urlList.first().path());
			if(!path.startsWith(QDir::rootPath()))
				path = path.mid(1);
			if(path.startsWith(QDir::rootPath())) {
				openFile(path);
			}
		}
	}

	event->acceptProposedAction();
}

void Window::setTitle(const int currentSaveEdited)
{
	setWindowTitle(PROG_NAME %
				   (!saves ? QString() : " - [*]" % saves->path()) %
				   (currentSaveEdited >= 0 ? tr(" - save %1").arg(currentSaveEdited+1, 2, 10, QChar('0')) : QString())
				   );
}

void Window::setModified(bool modified)
{
	setWindowModified(modified);
	actionSave->setEnabled(modified);
}

void Window::newFile()
{
	QDialog dialog(this, Qt::Dialog | Qt::WindowCloseButtonHint);
	dialog.setWindowTitle(tr("Nouveau"));

	QLineEdit name(tr("Sans nom"), &dialog);
	name.selectAll();
	QRadioButton oneSave(tr("1 sauvegarde"), &dialog);
	QRadioButton multiSaves(tr("15 sauvegardes"), &dialog);
	multiSaves.setChecked(true);
	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

	QGridLayout layout(&dialog);
	layout.addWidget(&name, 0, 0, 1, 2);
	layout.addWidget(&oneSave, 1, 0);
	layout.addWidget(&multiSaves, 1, 1);
	layout.addWidget(&buttonBox, 2, 0, 1, 2);

	connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
	connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

	if(dialog.exec() == QDialog::Accepted) {
		if(!closeFile())	return;

		saves = new SavecardData(oneSave.isChecked() ? 1 : 15);
		if(!name.text().isEmpty()) {
			saves->setName(name.text());
		}

		if(!saves->isOpen())
		{
			QMessageBox::warning(this, tr("Erreur"), saves->errorString());
			closeFile();
		}
		else
		{
			saveView();
			saveList->setSavecard(saves);
			setIsOpen(true);
			setModified(saves->isModified());
		}
	}
}

void Window::open(OpenType slot)
{
	QString path = Config::ff8Path();
	bool isPCSlot;

	if(slot==Slot1 || slot==Slot2)
	{
		if(path.isEmpty())	return;

		path.append(QString("/Save/Slot%1/").arg((int)slot));

		isPCSlot = true;
	}
	else
	{
		if(!Config::value("loadPath").isEmpty())
			path = Config::value("loadPath");
		else if(!path.isEmpty())
			path.append("/Save");
		path = QFileDialog::getOpenFileName(this, tr("Ouvrir"), path,
											tr("Fichiers compatibles (*.mcr *.ddf *.gme *.mc *.mcd *.mci *.ps *.psm *.vm1 *.psv save?? *.mem *.vgs *.vmp *.000 *.001 *.002 *.003 *.004);;"
											   "FF8 PS memorycard (*.mcr *.ddf *.mc *.mcd *.mci *.ps *.psm *.vm1);;"
											   "FF8 PC save (save??);;"
											   "FF8 vgs memorycard (*.mem *.vgs);;"
											   "FF8 gme memorycard (*.gme);;"
											   "FF8 PSN memorycard (*.vmp);;"
											   "FF8 PS3 memorycard/pSX save state (*.psv);;"
											   "ePSXe save state (*.000 *.001 *.002 *.003 *.004);;"
											   "Tous les fichiers (*)"));
		if(path.isNull())		return;

		int index = path.lastIndexOf('/');
		Config::setValue("loadPath", index == -1 ? path : path.left(index));

		isPCSlot = false;
	}
	
	openFile(path, isPCSlot);
}

bool Window::closeFile(bool quit)
{
	if(saves && saves->isModified()) {
		QMessageBox::StandardButton b = QMessageBox::question(this, tr("Enregistrer ?"),
															  tr("Voulez-vous enregistrer '%1' avant de fermer ?")
															  .arg(saves->type()==SavecardData::PcDir
																   ? tr("fente")
																   : saves->name()),
															  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		switch(b) {
		case QMessageBox::Yes:
			save();
			break;
		case QMessageBox::Cancel:
			return false;
		default:
			break;
		}
	}

	if(quit)	return true;

	setIsOpen(false);

	return true;
}

void Window::openFile(const QString &path, bool isPCSlot)
{
	if(!closeFile())	return;

	saves = new SavecardData(path, isPCSlot);
	if(saves->type() == SavecardData::Unknown) {
		QMessageBox::StandardButton rep = QMessageBox::question(this, tr("Erreur"), tr("Fichier de type inconnu.\nVoulez-vous l'analyser pour obtenir le bon format ?"), QMessageBox::Yes | QMessageBox::No);
		if(rep != QMessageBox::Yes) {
			return;
		}

		saves->getFormatFromRaw();
	}

	if(!saves->isOpen())
	{
		QMessageBox::warning(this, tr("Erreur"), saves->errorString());
		setIsOpen(false);
	}
	else
	{
		saveView();
		saveList->setSavecard(saves);

		if(saves->type() == SavecardData::Psv || saves->type() == SavecardData::Vmp)
			QMessageBox::information(this, tr("Sauvegarde hasardeuse"), tr("Le format %1 est protégé, l'enregistrement sera partiel et risque de ne pas fonctionner.")
									 .arg(saves->extension()));

		setIsOpen(true);
		setModified(saves->isModified());

		if(!isPCSlot) {
			Config::addRecentFile(saves->path());
			fillMenuRecent();
		}
	}
}

void Window::setIsOpen(bool open)
{
	if(open) {
		stackedLayout->setCurrentWidget(saveList);
		actionSaveAs->setEnabled(true);
		actionProperties->setEnabled(true);
		actionClose->setEnabled(true);
		actionReload->setEnabled(true);

		setTitle();
	} else {
		stackedLayout->setCurrentWidget(startWidget);
		if(editor)	editor->hide();

		if(saves)
		{
			if(saveList)	saveList->setSavecard(0);
			delete saves;
			saves = 0;
		}

		actionSaveAs->setEnabled(false);
		actionProperties->setEnabled(false);
		actionClose->setEnabled(false);
		actionReload->setEnabled(false);

		setTitle();
		setModified(false);
	}
}

void Window::openRecentFile(QAction *action)
{
	openFile( Config::recentFile(action->data().toUInt()) );
}

void Window::reload()
{
	if(saves) {
		openFile(QString(saves->path()), saves->type() == SavecardData::PcDir);
	}
}

bool Window::exportAs()
{
	QString types, path, selectedFilter,
			ps = tr("PS memorycard (*.mcr *.ddf *.mc *.mcd *.mci *.ps *.psm *.vm1)"),
			vgs = tr("VGS memorycard (*.vgs *.mem)"),
			gme = tr("GME memorycard (*.gme)"),
			vmp = tr("PSN memorycard (*.vmp)"),
			pc = tr("FF8 PC save (*)"),
			psv = tr("PSN save (*.psv)");
	SavecardData::Type type = saves->type(), newType;

	types = pc+";;"+ps+";;"+vgs+";;"+gme+";;"+vmp+";;"+psv;

	if(type == SavecardData::PcDir
			|| type == SavecardData::Pc)	selectedFilter = pc;
	else if(type == SavecardData::Psv)		selectedFilter = psv;
	else if(type == SavecardData::Vgs)		selectedFilter = vgs;
	else if(type == SavecardData::Gme)		selectedFilter = gme;
	else if(type == SavecardData::Vmp)		selectedFilter = vmp;
	else								selectedFilter = ps;
	
	path = Config::value("savePath").isEmpty() ? saves->dirname() : Config::value("savePath")+"/";
	if(saves->type() == SavecardData::Undefined) {
		path = path+saves->name()+".mcr";
	} else {
		path = path+saves->name();
	}
	path = QFileDialog::getSaveFileName(this, tr("Exporter"), path, types, &selectedFilter);
	if(path.isNull())		return false;

	if(selectedFilter == ps)			newType = SavecardData::Ps;
	else if(selectedFilter == vgs)		newType = SavecardData::Vgs;
	else if(selectedFilter == gme)		newType = SavecardData::Gme;
	else if(selectedFilter == vmp)		newType = SavecardData::Vmp;
	else if(selectedFilter == pc)		newType = SavecardData::Pc;
	else if(selectedFilter == psv)		newType = SavecardData::Psv;
	else {
		qWarning() << "Bad selected filter!" << selectedFilter;
		return false;
	}

	if(newType == SavecardData::Vmp || newType == SavecardData::Psv) {
		int reponse = QMessageBox::information(this, tr("Sauvegarde hasardeuse"), tr("Les formats VMP et PSV sont protégés, l'enregistrement sera partiel et risque de ne pas fonctionner.\nContinuer quand même ?"), tr("Oui"), tr("Non"));
		if(reponse != 0)  return exportAs();
	}
	
	int index = path.lastIndexOf('/');
	Config::setValue("savePath", index == -1 ? path : path.left(index));

	return exportAs(newType, path);
}

bool Window::exportAs(SavecardData::Type newType, const QString &path)
{
	SavecardData::Type type = saves->type();

	if(newType == SavecardData::Gme) {
		bool abort;
		QByteArray desc = descGme(saves->description(), &abort);
		if(abort)	return false;
		saves->setDescription(desc);
	}

	bool ok = true;

	if(type == newType) {
		switch(type) {
		case SavecardData::Pc:
			ok = saves->save2PC(0, path);
			break;
		case SavecardData::PcDir:
			ok = saves->saveDir();
			break;
		case SavecardData::Psv:
		case SavecardData::Ps:
		case SavecardData::Vgs:
		case SavecardData::Gme:
		case SavecardData::Vmp:
			ok = saves->save(path, newType);
			break;
		case SavecardData::Unknown:
		case SavecardData::Undefined:
			qWarning() << "not possible case";
			return false;
		}
	} else {
		if(newType != SavecardData::Pc && newType != SavecardData::Psv)
		{
			if(type == SavecardData::PcDir || type == SavecardData::Undefined
					|| type == SavecardData::Pc || type == SavecardData::Psv) {
				QList<int> selected_files;
				if(type == SavecardData::Pc || type == SavecardData::Psv) {
					selected_files.append(0);
				} else {
					selected_files = selectSavesDialog(true);
					if(selected_files.isEmpty())	return false;
				}

				QByteArray MCHeader;
				if(type != SavecardData::Psv) {
					SaveData tempSave;
					HeaderDialog dialog(&tempSave, this, HeaderDialog::CreateView);
					if(dialog.exec() != QDialog::Accepted) return false;
					MCHeader = tempSave.saveMCHeader();
				} else {
					MCHeader = saves->getSaves().first()->saveMCHeader();
				}

				ok = saves->save2PS(selected_files, path, newType, MCHeader);
			}
			else
				ok = saves->save(path, newType);
		}
		else // saveOne (PC & PSV)
		{
			// Need selection by user
			QList<int> selected_files = selectSavesDialog(false, newType == SavecardData::Pc);
			if(selected_files.isEmpty())	return false;
			int id = selected_files.first();
			if(newType == SavecardData::Pc) {
				ok = saves->save2PC(id, path);
			} else {
				QByteArray MCHeader;
				if(!saves->getSaves().first()->hasMCHeader()) {
					SaveData tempSave;
					HeaderDialog dialog(&tempSave, this, HeaderDialog::CreateView);
					if(dialog.exec() != QDialog::Accepted) return false;
					MCHeader = tempSave.saveMCHeader();
				} else {
					MCHeader = saves->getSaves().first()->saveMCHeader();
				}
				ok = saves->save2PSV(id, path, MCHeader);
			}
		}
	}

	if(!ok) {
		QMessageBox::warning(this, tr("Erreur"), saves->errorString());
	}

	setTitle();

	return ok;
}

QByteArray Window::descGme(const QString &desc, bool *abort)
{
	QDialog dialog(this, Qt::Dialog | Qt::WindowCloseButtonHint);
	dialog.setWindowTitle(tr("Commentaire"));

	QTextEdit *textEdit = new QTextEdit(&dialog);
	textEdit->setPlainText(desc);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
													   Qt::Horizontal, &dialog);
	QVBoxLayout *layout = new QVBoxLayout(&dialog);
	layout->addWidget(textEdit);
	layout->addWidget(buttonBox);

	connect(buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

	if(dialog.exec() != QDialog::Accepted) {
		*abort = true;
		return QByteArray();
	}
	*abort = false;

	return textEdit->toPlainText().toLatin1().leftJustified(3840, '\x00', true);
}

void Window::properties()
{
	if(!saves)		return;

	int saveCount = 0, firstSaveID = -1, saveID = 0;

	foreach(const SaveData *saveData, saves->getSaves()) {
		if(!saveData->isDelete()) {
			saveCount++;
			if(firstSaveID == -1) {
				firstSaveID = saveID;
			}
		}
		if(saveCount > 1)	break;
		++saveID;
	}

	if(saveCount == 0)	return;
	if(saveCount == 1) {
		saveList->view()->properties(firstSaveID);
		return;
	}

	SelectSavesDialog *dialog = new SelectSavesDialog(saves->getSaves(), false, false, this);

	if(dialog->exec() == QDialog::Accepted) {
		QList<int> selected_files = dialog->selectedSaves();
		saveList->view()->properties(selected_files.first());
	}
}

QList<int> Window::selectSavesDialog(bool multiSelection, bool onlyFF8)
{
	SelectSavesDialog *dialog = new SelectSavesDialog(saves->getSaves(), multiSelection, onlyFF8, this);

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
	menuBar->setEnabled(false);
	if(!editor) {
		editor = new Editor(this);
		connect(editor, SIGNAL(accepted()), SLOT(saveView()));
		connect(editor, SIGNAL(rejected()), SLOT(saveView()));
		stackedLayout->addWidget(editor);
	}
	editor->show();
	editor->load(saveData, saves->type()==SavecardData::Pc || saves->type()==SavecardData::PcDir);
	stackedLayout->setCurrentWidget(editor);
	setTitle(saveData->id());
	saves->setIsTheLastEdited(saveData->id());
	if(taskBarButton)	taskBarButton->setOverlayIcon(saveData->saveIcon().icon());
	else				setWindowIcon(QIcon(saveData->saveIcon().icon()));
}

void Window::saveView()
{
	if(!saveList) {
		saveList = new SavecardWidget(this);
		connect(saveList->view(), SIGNAL(changed()), SLOT(setModified()));
		connect(saveList->view(), SIGNAL(released(SaveData*)), SLOT(editView(SaveData*)));
		stackedLayout->addWidget(saveList);
	} else {
		stackedLayout->setCurrentWidget(saveList);
		if(editor)	editor->hide();
		menuBar->show();
		menuBar->setEnabled(true);
		if(taskBarButton)	taskBarButton->setOverlayIcon(QPixmap());
		else				setWindowIcon(qApp->windowIcon());
		setTitle();
		setModified(saves->isModified());
		saveList->view()->update();
	}
}

void Window::save()
{
	bool saved = true, hasPath = saves->hasPath();
	SavecardData::Type type = saves->type();

	if(!hasPath || type == SavecardData::Vmp || type == SavecardData::Psv) {
		saved = exportAs();
		if(!hasPath && saved) {
			setTitle();
		}
	} else {
		saved = exportAs(type, saves->path());
	}

	if(saved) {
		setModified(false);
		saves->setModified(false);
		saveList->view()->update();
	}
}

void Window::mode(bool mode)
{
	Config::setValue("mode", mode);
	if(saves && editor) {
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

	if(saves) {
		if(editor)	editor->updateTime();
		saveList->view()->update();
	}
}

void Window::font(bool font)
{
	Config::setValue("font", font ? "hr" : "");
	FF8Text::reloadFont();
	if(saves) {
		saveList->view()->update();
	} else {
		startWidget->update();
	}
}

QStringList Window::availableLanguages()
{
	QDir dir(qApp->applicationDirPath());
	QStringList languages, stringList = dir.entryList(QStringList("hyne_*.qm"), QDir::Files, QDir::Name);

	languages.append("Français|fr");

	QTranslator translator;
	foreach(QString str, stringList) {
		translator.load(str, qApp->applicationDirPath());
		QString lang = translator.translate("Window", "Français", "Your translation language");

		str = str.mid(5);
		languages.append(lang + "|" + str.left(str.size()-3));
	}

	return languages;
}

QString Window::chooseLangDialog()
{
	QStringList langs = availableLanguages();
	if(langs.size() <= 1) {
		// Doesn't ask for language if there is just one available
		return langs.isEmpty() ? QString() : langs.first();
	}

	const QString chooseStr("Choose your language");
	QDialog *dialog = new QDialog(0, Qt::Dialog | Qt::WindowCloseButtonHint);
	dialog->setWindowTitle(chooseStr);
	QLabel *label = new QLabel(chooseStr + ":", dialog);
	QComboBox *comboBox = new QComboBox(dialog);
	foreach(const QString &str, langs)
		comboBox->addItem(str.left(str.lastIndexOf("|")), str.mid(str.lastIndexOf("|")+1));

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, dialog);

	QGridLayout *layout = new QGridLayout(dialog);
	layout->addWidget(label, 0, 0);
	layout->addWidget(comboBox, 0, 1);
	layout->addWidget(buttonBox, 1, 0, 1, 2);

	connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));

	if(dialog->exec() == QDialog::Accepted) {
		return comboBox->itemData(comboBox->currentIndex()).toString();
	}

	return QString();
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

void Window::newWindow()
{
	(new Window())->show();
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
	Data::reload();
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
	if(!QProcess::startDetached("\"" % Config::ff8Path() % "/FF8.exe\"", QStringList(), Config::ff8Path())) {
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
	QDialog about(this, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	about.setFixedSize(224, 248);
	about.setWindowTitle(tr("À propos"));
	
	QFont font;
	font.setPointSize(12);
	
	QLabel image(&about);
	image.setPixmap(QPixmap(":/images/about.png"));
	image.move(about.width()-20-image.sizeHint().height(), 80);
	
	QLabel desc1(PROG_FULLNAME, &about);
	desc1.setFont(font);
	desc1.setFixedWidth(about.width());
	desc1.setAlignment(Qt::AlignHCenter);

	font.setPointSize(8);

	QLabel desc2(tr("Par myst6re<br/><a href=\"https://sourceforge.net/projects/hyne/\">https://sourceforge.net/projects/hyne/</a><br/><br/>75% modifiable<br/><br/>Merci à :<br/> - Qhimm<br/> - Cyberman<br/> - sithlord48<br/> - Aladore384<br/><br/>Traducteurs :<br/> - Anglais : myst6re, Vgr<br/> - Japonais : Asa"), &about);
	desc2.setTextInteractionFlags(Qt::LinksAccessibleByMouse | Qt::LinksAccessibleByKeyboard);
	desc2.setTextFormat(Qt::RichText);
	desc2.setOpenExternalLinks(true);
	desc2.move(9, 24);
	desc2.setFont(font);

	QLabel desc3(QString("Qt %1").arg(QT_VERSION_STR), &about);
	QPalette pal = desc3.palette();
	pal.setColor(QPalette::WindowText, QColor(0xAA,0xAA,0xAA));
	desc3.setPalette(pal);
	desc3.move(about.width()-8-desc3.sizeHint().width(), about.height()-8-desc3.sizeHint().height());
	desc3.setFont(font);
	
	QPushButton button(tr("&Fermer"), &about);
	button.move(8, about.height()-8-button.sizeHint().height());
	connect(&button, SIGNAL(released()), &about, SLOT(close()));
	
	// Set default icon if needed
	QStyle *style = about.style();
	if(style->styleHint(QStyle::SH_DialogButtonBox_ButtonsHaveIcons))
		button.setIcon(style->standardIcon(QStyle::SP_DialogCloseButton, 0, &about));
	if(style != QApplication::style()) // Propagate style
		button.setStyle(style);

	about.exec();
}
