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
/****************************************************************************
 **
 ** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial Usage
 ** Licensees holding valid Qt Commercial licenses may use this file in
 ** accordance with the Qt Commercial License Agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Nokia.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Nokia gives you certain
 ** additional rights. These rights are described in the Nokia Qt LGPL
 ** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 3.0 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU General Public License version 3.0 requirements will be
 ** met: http://www.gnu.org/copyleft/gpl.html.
 **
 ** If you are unsure which license is appropriate for your use, please
 ** contact the sales department at http://www.qtsoftware.com/contact.
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#include <QtWidgets>
#include "SpinBoxDelegate.h"

SpinBoxDelegate::SpinBoxDelegate(QObject *parent)
	: QItemDelegate(parent), editor(0)
{
}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
	EditorType type = (EditorType)index.data(Qt::UserRole).toInt();
	if(type == ComboBoxMagics || type == ComboBoxItems || type == ComboBoxLocations || type == ComboBoxDraw)
	{
		QComboBox *comboBox = new QComboBox(parent);
		switch(type) {
		case ComboBoxMagics:comboBox->addItems(Data::magics().list());break;
		case ComboBoxItems:
			comboBox->addItem(Data::items().at(0));
			for(int i=1 ; i<Data::items().size() ; ++i)
				comboBox->addItem(QIcon(QString(":/images/icons/objet%1.png").arg(Data::itemType(i))), Data::items().at(i));
			for(int i=Data::items().size() ; i<256 ; ++i)
				comboBox->addItem(QString::number(i));
			break;
		case ComboBoxLocations:
			for(int i=0 ; i<256 ; ++i) {
				comboBox->addItem(Data::getCardsLocation(i));
			}
			break;
		case ComboBoxDraw:
			comboBox->addItem(tr("Pleine"));
			comboBox->addItem(tr("Moitié pleine"));
			comboBox->addItem(tr("Vide"));
			comboBox->addItem(tr("Épuisée"));
			break;
		default:break;
		}
		comboBox->setCurrentIndex(index.data(Qt::UserRole+1).toInt());
		QFont font;
		font.setPointSize(8);
		comboBox->setFont(font);
		return comboBox;
	}
	QSpinBox *spinBox = new QSpinBox(parent);
	spinBox->setRange(0, spinBoxMax(type));
	return spinBox;
}

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	EditorType type = (EditorType)index.data(Qt::UserRole).toInt();
	if(type != ComboBoxMagics && type != ComboBoxItems && type != ComboBoxLocations && type != ComboBoxDraw)
	{
		QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
		spinBox->setRange(0, spinBoxMax(type));
		spinBox->setValue(index.data(Qt::EditRole).toInt());
	}
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	EditorType type = (EditorType)index.data(Qt::UserRole).toInt();
	if(type == ComboBoxMagics || type == ComboBoxItems || type == ComboBoxLocations || type == ComboBoxDraw)
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);

		model->setData(index, comboBox->currentText(), Qt::EditRole);
		model->setData(index, comboBox->currentIndex(), Qt::UserRole+1);

		if(type == ComboBoxItems)
		{
			model->setData(index,
						   comboBox->currentIndex()!=0
					? QIcon(QString(":/images/icons/objet%1.png").arg(Data::itemType(comboBox->currentIndex())))
					: QIcon()
					  , Qt::DecorationRole);
		}
	}
	else
	{
		QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
		spinBox->setRange(0, spinBoxMax(type));
		model->setData(index, spinBox->value(), Qt::EditRole);
	}
}

QSize SpinBoxDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSize editSize;

	EditorType type = (EditorType)index.data(Qt::UserRole).toInt();
	if(type == ComboBoxMagics || type == ComboBoxItems
			|| type == ComboBoxLocations || type == ComboBoxDraw) {
		editSize = QComboBox().sizeHint();
	} else {
		QSpinBox spinBox;
		spinBox.setRange(0, spinBoxMax(type));
		editSize = spinBox.sizeHint();
	}

	QSize size = QItemDelegate::sizeHint(option, index);

	if(size.width() > editSize.width()) {
		editSize.setWidth(size.width());
	}

	if(size.height() > editSize.height()) {
		editSize.setHeight(size.height());
	}

	return editSize;
}

int SpinBoxDelegate::spinBoxMax(EditorType type)
{
	switch(type) {
	case SpinBox100:
		return 100;
	case SpinBox127:
		return 127;
	default:
		return 255;
	}
}
