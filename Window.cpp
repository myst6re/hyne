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
#include "Parameters.h"
#include "SelectSavesDialog.h"
#include "HeaderDialog.h"
#include "SCHeaderDialog.h"
#include "MetadataDialog.h"

Window::Window(bool isNew) :
	QWidget(), taskbarButton(nullptr), saves(nullptr), saveList(nullptr), editor(nullptr)
{
	setTitle();
	setMinimumSize(768, 502);
	resize(768, 502);
	setAcceptDrops(true);

	menuBar = new QMenuBar(nullptr);
	QMenu *menu;
	QAction *action, *actionSlot1=nullptr, *actionSlot2=nullptr;

	/* MENU 'FICHIER' */
	
	menu = menuBar->addMenu(tr("&Fichier"));
#ifdef Q_OS_MAC
	QMenu *fileMenu = menu;
#endif

	bool isInstalled, hasSlots;

	isInstalled = Config::ff8IsInstalled(hasSlots);

	QAction *actionNew = menu->addAction(tr("&Nouveau..."), this, SLOT(newFile()), QKeySequence::New);
	QAction *actionOpen = menu->addAction(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton), tr("&Ouvrir..."), this, SLOT(open()), QKeySequence::Open);
	QAction *actionOpenDir = menu->addAction(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon), tr("&Ouvrir un dossier..."), this, SLOT(openDir()), QKeySequence::Open);
	actionReload = menu->addAction(QApplication::style()->standardIcon(QStyle::SP_BrowserReload), tr("&Recharger depuis le disque"), this, SLOT(reload()), QKeySequence::Refresh);
	actionReload->setEnabled(false);
	actionSave = menu->addAction(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton), tr("&Enregistrer"), this, SLOT(save()), QKeySequence::Save);
	actionSave->setEnabled(false);
	actionSaveAs = menu->addAction(tr("E&xporter..."), this, SLOT(exportAs()), QKeySequence::SaveAs);
	actionSaveAs->setEnabled(false);
	menuRecent = menu->addMenu(tr("O&uverts récemment"));
	fillMenuRecent();
	connect(menuRecent, SIGNAL(triggered(QAction*)), SLOT(openRecentFile(QAction*)));
	menu->addSeparator();
	actionProperties = menu->addAction(tr("&Propriétés..."), this, SLOT(properties()));
	actionProperties->setEnabled(false);
	action = menu->addAction(tr("S&igner des sauv. pour le Cloud..."), this, SLOT(updateMetadata()));
	addAction(action);
#ifndef Q_OS_WINRT
	if (isInstalled) {
		action = menu->addAction(QIcon(":/images/ff8.png"), tr("&Lancer Final Fantasy VIII"), this, SLOT(runFF8()));
		if (!isNew) {
			action->setShortcut(Qt::Key_F8);
			action->setShortcutContext(Qt::ApplicationShortcut);
		}
		addAction(action);
	}
#else
	isInstalled = false;
#endif
	menu->addAction(tr("Nou&velle fenêtre"), this, SLOT(newWindow()));
	action = menu->addAction(tr("Ple&in écran"), this, SLOT(fullScreen()));
	if (!isNew) {
		action->setShortcut(Qt::Key_F11);
		action->setShortcutContext(Qt::ApplicationShortcut);
	}
	addAction(action);
	menu->addSeparator();
	actionClose = menu->addAction(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton), tr("&Fermer"), this, SLOT(closeFile()), QKeySequence::Close);
	actionClose->setEnabled(false);
	menu->addAction(tr("&Quitter"), this, SLOT(close()), QKeySequence::Quit)->setMenuRole(QAction::QuitRole);
	
	/* MENU 'SLOT' */
	
	if (hasSlots) {
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
	actionFont->setChecked(!Config::value(Config::Font).isEmpty());
	
	menuLang = menu->addMenu(tr("&Langue"));
	for (const QLocale &locale : availableLanguages()) {
		action = menuLang->addAction(QLocale::languageToString(locale.language()));
		action->setData(locale.bcp47Name());
		action->setCheckable(true);
		action->setChecked(Config::value(Config::Lang) == locale.bcp47Name());
	}
	connect(menuLang, SIGNAL(triggered(QAction*)), SLOT(changeLanguage(QAction*)));

	if (Config::ff8Installations().size() > 1) {
		menuVersion = menu->addMenu(tr("Version PC"));
		for (const FF8Installation &installation : Config::ff8Installations()) {
			action = menuVersion->addAction(installation.typeString());
			action->setCheckable(true);
			action->setChecked(Config::ff8Installation() == installation);
		}
		connect(menuVersion, SIGNAL(triggered(QAction*)), SLOT(changeFF8Version(QAction*)));
	}

	/* MENU '?' */
	
#ifndef Q_OS_MAC
	menuBar->addAction(tr("&?"), this, SLOT(about()))->setMenuRole(QAction::AboutRole);
#else
	fileMenu->addAction(tr("&?"), this, SLOT(about()))->setMenuRole(QAction::AboutRole);
#endif

	startWidget = new StartWidget(this);
	startWidget->addAction(actionNew);
	startWidget->addAction(actionOpen);
	startWidget->addAction(actionOpenDir);
	if (hasSlots) {
		startWidget->addAction(actionSlot1);
		startWidget->addAction(actionSlot2);
	}
	
	stackedLayout = new QStackedLayout(this);
	stackedLayout->setMenuBar(menuBar);
	stackedLayout->addWidget(startWidget);

	restoreGeometry(Config::valueVar(Config::Geometry).toByteArray());
}

Window::~Window()
{
	Config::setValue(Config::Geometry, saveGeometry());
	Config::saveRecentFiles();
	Config::sync();// flush data
}

void Window::showEvent(QShowEvent *event)
{
	event->accept();
#ifdef Q_OS_WIN
	if (!taskbarButton && QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
		taskbarButton = new QTaskbarButton(windowHandle());
	}
#endif
}

void Window::closeEvent(QCloseEvent *event)
{
	if (stackedLayout->currentWidget() == editor) {
		saveView();
		event->ignore();
	}
	else if (!closeFile(true))
		event->ignore();
	else
		event->accept();
}

void Window::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasUrls() &&
			(stackedLayout->currentWidget() == startWidget
			 || (saveList && stackedLayout->currentWidget() == saveList)))
		event->acceptProposedAction();
}

void Window::dropEvent(QDropEvent *event)
{
	if (stackedLayout->currentWidget() == editor)	return;

	const QMimeData *mimeData = event->mimeData();

	if (mimeData->hasUrls())
	{
		QList<QUrl> urlList = mimeData->urls();
		if (!urlList.isEmpty()) {
			QString path = QDir::cleanPath(urlList.first().path().mid(1));
			openFile(path);
		}
	}

	event->acceptProposedAction();
}

void Window::setTitle(const int currentSaveEdited)
{
	setWindowTitle(PROG_NAME %
				   (!saves ? QString() : " - [*]" % QDir::toNativeSeparators(saves->path())) %
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

	if (dialog.exec() == QDialog::Accepted) {
		if (!closeFile())	return;

		saves = new SavecardData(oneSave.isChecked() ? 1 : 15);
		if (!name.text().isEmpty()) {
			saves->setName(name.text());
		}

		if (!saves->isOpen())
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
	QString path;
	FF8Installation installation;

	if (slot == Slot1 || slot == Slot2)
	{
		installation = Config::ff8Installation();
		if (!installation.hasSlots())	return;

		path = installation.savePath(int(slot));
	}
	else
	{
		QString loadPath = Config::value(Config::LoadPath);
		if (!loadPath.isEmpty())
			path = loadPath;
		else if (!path.isEmpty())
			path.append("/Save");

		if (slot == Dir)
		{
			path = QFileDialog::getExistingDirectory(this, tr("Ouvrir"), path);
		}
		else
		{
			path = QFileDialog::getOpenFileName(this, tr("Ouvrir"), path,
			                                    tr("Fichiers compatibles (*.mcr *.ddf *.gme *.mc *.mcd *.mci *.ps *.psm *.vm1 *.srm *.psv save?? *.ff8 ff8slot* *.mem *.vgs *.vmp *.000 *.001 *.002 *.003 *.004);;"
			                                       "FF8 PS memorycard (*.mcr *.ddf *.mc *.mcd *.mci *.ps *.psm *.vm1 *.srm);;"
			                                       "FF8 PC save (save?? *.ff8);;"
			                                       "FF8 Switch save (ff8slot*);;"
			                                       "FF8 vgs memorycard (*.mem *.vgs);;"
			                                       "FF8 gme memorycard (*.gme);;"
			                                       "FF8 PSN memorycard (*.vmp);;"
			                                       "FF8 PS3 memorycard/pSX save state (*.psv);;"
			                                       "ePSXe save state (*.000 *.001 *.002 *.003 *.004);;"
			                                       "Tous les fichiers (*)"));
		}
		if (path.isNull())		return;

		int index = path.lastIndexOf('/');
		Config::setValue(Config::LoadPath, index == -1 ? path : path.left(index));
	}

	openFile(path, slot, installation);
}

bool Window::closeFile(bool quit)
{
	if (saves && saves->isModified()) {
		QMessageBox::StandardButton b = QMessageBox::question(this, tr("Enregistrer ?"),
															  tr("Voulez-vous enregistrer '%1' avant de fermer ?")
															  .arg(saves->type() == SavecardData::PcSlot
																   ? tr("fente")
																   : saves->name()),
															  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		switch (b) {
		case QMessageBox::Yes:
			save();
			break;
		case QMessageBox::Cancel:
			return false;
		default:
			break;
		}
	}

	if (quit)	return true;

	setIsOpen(false);

	return true;
}

void Window::openFile(const QString &path, OpenType openType, const FF8Installation &installation)
{
	if (!closeFile())	return;

	saves = new SavecardData(path, quint8(openType), installation);
	if (saves->type() == SavecardData::Unknown) {
		QMessageBox::StandardButton rep = QMessageBox::question(this, tr("Erreur"), tr("Fichier de type inconnu.\nVoulez-vous l'analyser pour obtenir le bon format ?"), QMessageBox::Yes | QMessageBox::No);
		if (rep != QMessageBox::Yes) {
			return;
		}

		saves->getFormatFromRaw();
	}

	if (!saves->isOpen())
	{
		QMessageBox::warning(this, tr("Erreur"), saves->errorString());
		setIsOpen(false);
	}
	else
	{
		saveView();
		saveList->setSavecard(saves);

		setIsOpen(true);
		setModified(saves->isModified());

		if (openType == File) {
			Config::addRecentFile(saves->path());
			fillMenuRecent();
		}
	}
}

void Window::setIsOpen(bool open)
{
	if (open) {
		stackedLayout->setCurrentWidget(saveList);
		actionSaveAs->setEnabled(true);
		actionProperties->setEnabled(true);
		actionClose->setEnabled(true);
		actionReload->setEnabled(true);

		setTitle();
	} else {
		stackedLayout->setCurrentWidget(startWidget);
		if (editor)	editor->hide();

		if (saves)
		{
			if (saveList)	saveList->setSavecard(nullptr);
			delete saves;
			saves = nullptr;
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
	if (saves) {
		OpenType openType;
		if (saves->type() == SavecardData::PcSlot) {
			openType = saves->slotNumber() == 1 ? Slot1 : Slot2;
		} else {
			openType = File;
		}

		openFile(QString(saves->path()), openType, FF8Installation(saves->ff8Installation()));
	}
}

bool Window::exportAs()
{
	QString types, path, selectedFilter,
	        ps = tr("PS memorycard (*.mcr *.ddf *.mc *.mcd *.mci *.ps *.psm *.vm1 *.srm)"),
	        vgs = tr("VGS memorycard (*.vgs *.mem)"),
	        gme = tr("GME memorycard (*.gme)"),
	        vmp = tr("PSN memorycard (*.vmp)"),
	        pc = tr("FF8 PC save (*.ff8 *)"),
	        ps4 = tr("PS4 Remaster save (*.ff8 *)"),
	        swi = tr("Switch save (ff8slot*)"),
	        psv = tr("PSN save (*.psv)");
	SavecardData::Type type = saves->type(), newType;

	types = pc+";;"+ps+";;"+ps4+";;"+swi+";;"+vgs+";;"+gme+";;"+vmp+";;"+psv;

	if (type == SavecardData::PcSlot
	        || type == SavecardData::Pc)	selectedFilter = pc;
	else if (type == SavecardData::PcUncompressed)	selectedFilter = ps4;
	else if (type == SavecardData::Switch)	selectedFilter = swi;
	else if (type == SavecardData::Psv)		selectedFilter = psv;
	else if (type == SavecardData::Vgs)		selectedFilter = vgs;
	else if (type == SavecardData::Gme)		selectedFilter = gme;
	else if (type == SavecardData::Vmp)		selectedFilter = vmp;
	else									selectedFilter = ps;
	
	path = Config::value(Config::SavePath).isEmpty() ? saves->dirname() : Config::value(Config::SavePath)+"/";
	if (type == SavecardData::Undefined) {
		path = path+saves->name()+".mcr";
	} else {
		path = path+saves->name();
	}
	path = QFileDialog::getSaveFileName(this, tr("Exporter"), path, types, &selectedFilter);
	if (path.isNull())		return false;

	if (selectedFilter == ps)			newType = SavecardData::Ps;
	else if (selectedFilter == vgs)		newType = SavecardData::Vgs;
	else if (selectedFilter == gme)		newType = SavecardData::Gme;
	else if (selectedFilter == vmp)		newType = SavecardData::Vmp;
	else if (selectedFilter == pc)		newType = SavecardData::Pc;
	else if (selectedFilter == ps4)		newType = SavecardData::PcUncompressed;
	else if (selectedFilter == swi)		newType = SavecardData::Switch;
	else if (selectedFilter == psv)		newType = SavecardData::Psv;
	else {
		qWarning() << "Bad selected filter!" << selectedFilter;
		return false;
	}
	
	int index = path.lastIndexOf('/');
	Config::setValue(Config::SavePath, index == -1 ? path : path.left(index));

	return exportAs(newType, path);
}

bool Window::exportAs(SavecardData::Type newType, const QString &path)
{
	SavecardData::Type type = saves->type();

	if (newType == SavecardData::Gme) {
		bool abort;
		QByteArray desc = descGme(saves->description(), &abort);
		if (abort)	return false;
		saves->setDescription(desc);
	}

	bool ok = true;

	if (type == newType) {
		switch (type) {
		case SavecardData::Pc:
		case SavecardData::PcUncompressed:
		case SavecardData::Switch:
		case SavecardData::Psv:
			ok = saves->saveOne(saves->getSave(0), path, newType);
			break;
		case SavecardData::PcSlot:
			ok = saves->saveDirectory();
			break;
		case SavecardData::Ps:
		case SavecardData::Vgs:
		case SavecardData::Gme:
		case SavecardData::Vmp:
			ok = saves->saveMemoryCard(path, newType);
			break;
		case SavecardData::Unknown:
		case SavecardData::Undefined:
			qWarning() << "not possible case";
			return false;
		}
	} else {
		// Need hash seed
		if (newType == SavecardData::Psv || newType == SavecardData::Vmp) {
			SCHeaderDialog dialog(saves, this);
			if (dialog.exec() != QDialog::Accepted) return false;
		}

		if (!SavecardData::isOne(newType)) {
			if (type == SavecardData::PcSlot
					|| type == SavecardData::Undefined
			        || SavecardData::isOne(type)
			        || type == SavecardData::Psv) {
				QList<int> selectedFiles = selectSavesDialog(true);
				if (selectedFiles.isEmpty()) {
					return false;
				}

				QByteArray MCHeader;
				if (type != SavecardData::Psv) {
					SaveData tempSave;
					HeaderDialog dialog(&tempSave, this, HeaderDialog::CreateView);
					if (dialog.exec() != QDialog::Accepted) return false;
					MCHeader = tempSave.saveMCHeader();
				} else {
					MCHeader = saves->getSaves().first()->saveMCHeader();
				}

				ok = saves->save2PS(selectedFiles, path, newType, MCHeader);
			} else {
				ok = saves->saveMemoryCard(path, newType);
			}
		} else { // saveOne (PC & PSV)
			// Need selection by user
			QList<int> selectedFiles = selectSavesDialog(false, newType == SavecardData::Pc);
			if (selectedFiles.isEmpty()) {
				return false;
			}
			int id = selectedFiles.first();

			SaveData *save = saves->getSaves().at(id);

			if (newType == SavecardData::Psv) {
				QByteArray MCHeader;
				if (!save->hasMCHeader()) {
					HeaderDialog dialog(save, this, HeaderDialog::CreateView);
					if (dialog.exec() != QDialog::Accepted) {
						return false;
					}
				}
			}

			ok = saves->saveOne(save, path, newType, true);
		}
	}

	if (!ok) {
		QMessageBox::critical(this, tr("Erreur"), saves->errorString());
	} else if (!saves->errorString().isEmpty()) {
		QMessageBox::warning(this, tr("Attention"), saves->errorString());
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

	if (dialog.exec() != QDialog::Accepted) {
		*abort = true;
		return QByteArray();
	}
	*abort = false;

	return textEdit->toPlainText().toLatin1().leftJustified(3840, '\x00', true);
}

void Window::properties()
{
	if (!saves)		return;

	int saveCount = 0, firstSaveID = -1, saveID = 0;

	for (const SaveData *saveData : saves->getSaves()) {
		if (!saveData->isDelete()) {
			saveCount++;
			if (firstSaveID == -1) {
				firstSaveID = saveID;
			}
		}
		if (saveCount > 1)	break;
		++saveID;
	}

	if (saveCount == 0)	return;
	if (saveCount == 1) {
		saveList->view()->properties(firstSaveID);
		return;
	}

	QList<int> selectedFiles = selectSavesDialog(false, false);

	if (!selectedFiles.isEmpty()) {
		saveList->view()->properties(selectedFiles.first());
	}
}

QList<int> Window::selectSavesDialog(bool multiSelection, bool onlyFF8)
{
	if (saves->saveCount() == 1) {
		return QList<int>() << 0;
	}

	if (saves->saveCount() < 1) {
		return QList<int>();
	}

	SelectSavesDialog *dialog = new SelectSavesDialog(saves->getSaves(), multiSelection, onlyFF8, this);

	if (dialog->exec() == QDialog::Accepted) {
		return dialog->selectedSaves();
	}

	return QList<int>();
}

void Window::fillMenuRecent()
{
	menuRecent->clear();
	
	for (int i = 0; i < Config::recentFilesSize(); ++i)
		menuRecent->addAction(tr("%1 : %2").arg(i+1,2,10).arg(QDir::toNativeSeparators(Config::recentFile(i))))->setData(i);

	menuRecent->setEnabled(Config::hasRecentFiles());
}

void Window::editView(SaveData *saveData)
{
	menuBar->hide();
	menuBar->setEnabled(false);
	if (!editor) {
		editor = new Editor(this);
		connect(editor, SIGNAL(accepted()), SLOT(saveView()));
		connect(editor, SIGNAL(rejected()), SLOT(saveView()));
		stackedLayout->addWidget(editor);
	}
	editor->show();
	editor->load(saveData, saves->type() == SavecardData::Pc || saves->type() == SavecardData::PcSlot);
	stackedLayout->setCurrentWidget(editor);
	setTitle(saveData->id());
	saves->setIsTheLastEdited(saveData->id());
	if (taskbarButton)	taskbarButton->setOverlayIcon(saveData->saveIcon().icon());
	else				setWindowIcon(QIcon(saveData->saveIcon().icon()));
}

void Window::saveView()
{
	if (!saveList) {
		saveList = new SavecardWidget(this);
		connect(saveList->view(), SIGNAL(changed()), SLOT(setModified()));
		connect(saveList->view(), SIGNAL(externalFileChanged(const QString&)), SLOT(notifyFileChanged(const QString&)));
		connect(saveList->view(), SIGNAL(released(SaveData*)), SLOT(editView(SaveData*)));
		stackedLayout->addWidget(saveList);
	} else {
		stackedLayout->setCurrentWidget(saveList);
		if (editor)	editor->hide();
		menuBar->show();
		menuBar->setEnabled(true);
		if (taskbarButton)	taskbarButton->setOverlayIcon(QPixmap());
		else				setWindowIcon(qApp->windowIcon());
		setTitle();
		setModified(saves->isModified());
		saveList->view()->update();
	}
}

void Window::save()
{
	bool saved = true;

	if (!saves->hasPath()) {
		saved = exportAs();
		if (saved) {
			setTitle();
		}
	} else {
		saved = exportAs(saves->type(), saves->path());
	}

	if (saved) {
		setModified(false);
		saves->setModified(false);
		saveList->view()->update();
	}
}

void Window::mode(bool mode)
{
	Config::setValue(Config::Mode, mode);
	if (saves && editor) {
		editor->updateMode(mode);
	}
}

void Window::changeFrame(QAction *action)
{
	if (action->data().toInt()==0)
		Config::setValue(Config::FreqAuto, true);
	else {
		Config::setValue(Config::Freq, action->data());
		Config::setValue(Config::FreqAuto, false);
	}

	for (QAction *act : menuFrame->actions())
		act->setChecked(false);
	action->setChecked(true);

	if (saves) {
		if (editor)	editor->updateTime();
		saveList->view()->update();
	}
}

void Window::font(bool font)
{
	Config::setValue(Config::Font, font ? "hr" : "");
	FF8Text::reloadFont();
	if (saves) {
		saveList->view()->update();
	} else {
		startWidget->update();
	}
}

QList<QLocale> Window::availableLanguages()
{
	QDir dir(Config::translationDir());
	QList<QLocale> languages;
	QStringList qmFiles = dir.entryList(QStringList("hyne_*.qm"), QDir::Files, QDir::Name);

	languages.append(QLocale(QLocale::French));

	for (const QString &qmFile : qAsConst(qmFiles)) {
		QString language = qmFile.mid(5, qmFile.size() - 5 - 3);
		languages.append(QLocale(language));
	}

	return languages;
}

QLocale Window::chooseLangDialog()
{
	QList<QLocale> langs = availableLanguages();
	if (langs.size() <= 1) {
		// Doesn't ask for language if there is just one available
		return langs.isEmpty() ? QLocale() : langs.first();
	}

	const QString chooseStr("Choose your language");
	QDialog *dialog = new QDialog(0, Qt::Dialog | Qt::WindowCloseButtonHint);
	dialog->setWindowTitle(chooseStr);
	QLabel *label = new QLabel(chooseStr + ":", dialog);
	QComboBox *comboBox = new QComboBox(dialog);
	for (const QLocale &lang : qAsConst(langs))
		comboBox->addItem(QLocale::languageToString(lang.language()), lang.bcp47Name());

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, dialog);

	QGridLayout *layout = new QGridLayout(dialog);
	layout->addWidget(label, 0, 0);
	layout->addWidget(comboBox, 0, 1);
	layout->addWidget(buttonBox, 1, 0, 1, 2);

	connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));

	if (dialog->exec() == QDialog::Accepted) {
		return QLocale(comboBox->itemData(comboBox->currentIndex()).toString());
	}

	return QLocale();
}

void Window::changeLanguage(QAction *action)
{
	if (Config::value(Config::Lang) != action->data()) {
		Config::setValue(Config::Lang, action->data());
		for (QAction *act : menuLang->actions())
			act->setChecked(false);
		action->setChecked(true);

		restartNow();
	} else {
		action->setChecked(true);
	}
}

void Window::changeFF8Version(QAction *action)
{
	Config::setSelectedFF8Installation(FF8Installation::Type(menuVersion->actions().indexOf(action)));
	for (QAction *act : menuVersion->actions())
		act->setChecked(false);
	action->setChecked(true);
}

void Window::newWindow()
{
	(new Window(true))->show();
}

void Window::restartNow()
{
	QString title, text;

	if (Config::translator->load("hyne_" + Config::value(Config::Lang), qApp->applicationDirPath())) {
		title = Config::translator->translate("Window", "Paramètres modifiés");
		text = Config::translator->translate("Window", "Relancez le programme pour que les paramètres prennent effet.");
	} else {
		title = "Paramètres modifiés";
		text = "Relancez le programme pour que les paramètres prennent effet.";
	}
	Data::reload();
    QMessageBox::information(this, title, text);
}

void Window::notifyFileChanged(const QString &path)
{
	if (!QFile::exists(path))
	{
		QMessageBox::warning(this, tr("Fichier supprimé"),
		                     tr("Le fichier '%1' a été supprimé par un programme externe !").arg(path));
	}
	else
	{
		QMessageBox::StandardButton button = QMessageBox::warning(this, tr("Fichier modifié"),
		                                                          tr("Le fichier '%1' a été modifié par un programme externe.").arg(path),
		                                                          QMessageBox::Ok | QMessageBox::Reset);
		if (button == QMessageBox::Reset) {
			reload();
		}
	}
}

/*void Window::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange) {

	} else {
		QWidget::changeEvent(e);
	}
}*/

#ifndef Q_OS_WINRT
void Window::runFF8()
{
	if (Config::ff8Installation().isValid()) {
		QString appPath = Config::ff8Installation().appPath(), exeFilename = appPath % "/" % Config::ff8Installation().exeFilename();
		if (!QProcess::startDetached(QString("\"%1\"").arg(exeFilename), QStringList(), appPath)) {
			QMessageBox::warning(this, tr("Erreur"), tr("Final Fantasy VIII n'a pas pu être lancé.\n%1").arg(exeFilename));
		}
	} else {
		QMessageBox::warning(this, tr("Erreur"), tr("Final Fantasy VIII n'a pas pu être lancé.\nImpossible de trouver le chemin du jeu."));
	}
}
#endif

void Window::updateMetadata()
{
	MetadataDialog dialog(Config::ff8Installations(), this);
	dialog.exec();
}

void Window::fullScreen()
{
	if (isFullScreen())	showNormal();
	else				showFullScreen();
}

void Window::about()
{
	QDialog about(this, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	about.setFixedSize(224, 272);
	about.setWindowTitle(tr("À propos"));
	
	QFont font;
	font.setPointSize(12);
	
	QLabel image(&about);
	image.setPixmap(QPixmap(":/images/about.png"));
	image.move(about.width()-20-image.sizeHint().height(), 80);
	
	QLabel desc1(QString("%1 %2 <a href=\"https://www.ff8.fr/\">FF8.fr</a> Edition").arg(PROG_NAME, PROG_VERSION), &about);
	desc1.setTextInteractionFlags(Qt::LinksAccessibleByMouse | Qt::LinksAccessibleByKeyboard);
	desc1.setTextFormat(Qt::RichText);
	desc1.setOpenExternalLinks(true);
	desc1.setFont(font);
	desc1.setFixedWidth(about.width());
	desc1.setAlignment(Qt::AlignHCenter);

	font.setPointSize(8);

	QLabel desc2(tr("Par myst6re<br/><a href=\"https://github.com/myst6re/hyne/\">github.com/myst6re/hyne</a><br/><br/>75% modifiable<br/><br/>Merci à :<br/> - Qhimm<br/> - Cyberman<br/> - sithlord48<br/> - Aladore384<br/> - suloku<br/><br/>Traducteurs :<br/> - Anglais : myst6re, Vgr<br/> - Japonais : Asa, Sharleen"), &about);
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
	if (style->styleHint(QStyle::SH_DialogButtonBox_ButtonsHaveIcons))
		button.setIcon(style->standardIcon(QStyle::SP_DialogCloseButton, 0, &about));
	if (style != QApplication::style()) // Propagate style
		button.setStyle(style);

	about.exec();
}
