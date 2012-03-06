#include "NewFileWizard.h"

NewFileWizard::NewFileWizard(QWidget *parent) :
	QWizard(parent, Qt::Dialog | Qt::WindowCloseButtonHint)
{
	setMaximumWidth(0.8 * parent->width());
	setMaximumHeight(parent->height());
	setWindowTitle(tr("Créer un nouveau fichier"));
	addPage(createFirstPage());
	addPage(createSecondPage());
}

QWizardPage *NewFileWizard::createFirstPage()
{
	QWizardPage *page = new QWizardPage();
	page->setTitle(tr("Choisir le format"));

	l1 = new QListWidget(page);
	l1->addItem(tr("PlayStation 1"));
	l1->addItem(tr("Final Fantasy VIII PC"));

	l2 = new QListWidget(page);
	l2->setEnabled(false);
	l3 = new QListWidget(page);
	l3->setEnabled(false);

	QGridLayout *layout = new QGridLayout(page);
	layout->addWidget(l1, 0, 0);
	layout->addWidget(l2, 0, 1);
	layout->addWidget(l3, 0, 2);

	connect(l1, SIGNAL(currentRowChanged(int)), SLOT(firstPageFillList2(int)));
	connect(l2, SIGNAL(currentRowChanged(int)), SLOT(firstPageFillList3(int)));

	l1->setCurrentRow(0);

	return page;
}

void NewFileWizard::firstPageFillList2(int list1Row)
{
	l2->clear();
	if(list1Row == 0) {
		l2->addItem(tr("Formats standards"));
		l2->addItem(tr("Formats Connectix Virtual Game Station"));
		l2->addItem(tr("Format DexDrive"));
	} else if(list1Row == 1) {
		l2->addItem(tr("Format standard"));
	}

	l2->setEnabled(l2->count() != 0);
	if(l2->count() == 1) {
		l2->setCurrentRow(0);
	}
}

void NewFileWizard::firstPageFillList3(int list2Row)
{
	int list1Row = l1->currentRow();
	l3->clear();
	if(list1Row == 0) {
		if(list2Row == 0) {
			l3->addItem(tr("*.mcr (émulateur ePSXe)"));
			l3->addItem(tr("*.mcd (émulateur Bleem!"));
			l3->addItem(tr("*.vm1 (PlayStation 3 brut)"));
			l3->addItem(tr("*.mc (outil PSXGameEdit)"));
			l3->addItem(tr("*.ps (matériel WinPSM)"));
			l3->addItem(tr("*.ddf (matériel DataDeck)"));
			l3->addItem(tr("*.psm (matériel SmartLink"));
			l3->addItem(tr("*.mci (MCExplorer)"));
		} else if(list2Row == 1) {
			l3->addItem(tr("*.vgs"));
			l3->addItem(tr("*.mem"));
		} else if(list2Row == 2) {
			l3->addItem(tr("*.gme"));
		}
	} else if(list1Row == 1 && list2Row == 0) {
		l3->addItem(tr("save*"));
	}

	l3->setEnabled(l3->count() != 0);
	if(l3->count() == 1) {
		l3->setCurrentRow(0);
	}
}

QWizardPage *NewFileWizard::createSecondPage()
{
	QWizardPage *page = new QWizardPage();
	page->setTitle(tr("Paramètrer le fichier"));

	return page;
}
