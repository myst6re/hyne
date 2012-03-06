#ifndef NEWFILEWIZARD_H
#define NEWFILEWIZARD_H

#include <QtGui>

class NewFileWizard : public QWizard
{
	Q_OBJECT
public:
	explicit NewFileWizard(QWidget *parent = 0);
	
signals:
	
private slots:
	void firstPageFillList2(int list1Row);
	void firstPageFillList3(int list2Row);
private:
	QWizardPage *createFirstPage();
	QWizardPage *createSecondPage();
	QListWidget *l1, *l2, *l3;
};

#endif // NEWFILEWIZARD_H
