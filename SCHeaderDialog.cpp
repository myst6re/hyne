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

SCHeaderDialog::SCHeaderDialog(SavecardData *savecardData, QWidget *parent)
	: QDialog(parent, Qt::Dialog | Qt::WindowCloseButtonHint),
      savecardData(savecardData)
{
	setWindowTitle(tr("Signature du fichier"));

	seed = new QHexEdit(this);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close);

	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(seed, 0, 0);
	layout->setRowStretch(1, 1);
	layout->addWidget(buttonBox, 2, 0);

	connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));

	fill();
}

void SCHeaderDialog::fill()
{
	seed->setData(savecardData->hashSeed());
}

void SCHeaderDialog::accept()
{
	savecardData->setHashSeed(seed->data());

	QDialog::accept();
}
