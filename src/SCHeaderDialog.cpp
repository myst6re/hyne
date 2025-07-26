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

#include "SCHeaderDialog.h"
#include "HelpWidget.h"

SCHeaderDialog::SCHeaderDialog(SavecardData *savecardData, QWidget *parent)
	: QDialog(parent, Qt::Dialog | Qt::WindowCloseButtonHint),
      savecardData(savecardData)
{
	setWindowTitle(tr("Signature du fichier"));

	seed = new QHexEdit(this);
	seed->setOverwriteMode(true);
	seed->setAsciiArea(false);
	HelpWidget *helpWidget =
	        new HelpWidget(32,
	                       tr("Pour fonctionner sur votre console, le fichier doit être signé.\n"
	                          "Pour cela vous devez fournir la clé de diversification (seed key) "
	                          "associée à votre console."));

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(helpWidget, 0, 0);
	layout->addWidget(seed, 1, 0);
	layout->setRowStretch(2, 1);
	layout->addWidget(buttonBox, 3, 0);

	connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));

	fill();

	resize(640, height());
}

void SCHeaderDialog::fill()
{
	seed->setData(savecardData->hashSeed().leftJustified(20, '\0'));
}

void SCHeaderDialog::accept()
{
	savecardData->setHashSeed(seed->data());

	QDialog::accept();
}
