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

#include "DrawPointEditor.h"

DrawPointEditor::DrawPointEditor(QWidget *parent)
	: PageWidget(parent)
{
}

void DrawPointEditor::buildWidget()
{
	QFont font;
	font.setPointSize(10);

	drawE_model = new QStandardItemModel(this);
	drawE_model->setHorizontalHeaderLabels(QStringList() << tr("Magie") << tr("Lieu") << tr("État actuel"));

	QTreeView *drawE_list = new QTreeView(this);
	drawE_list->setModel(drawE_model);
	drawE_list->setEditTriggers(QAbstractItemView::DoubleClicked);
	drawE_list->setFont(font);
	drawE_list->setItemDelegate(new SpinBoxDelegate(this));
	drawE_list->setIndentation(0);
	drawE_list->setItemsExpandable(false);
	drawE_list->setUniformRowHeights(true);
	drawE_list->setAllColumnsShowFocus(true);

	for(int i=0 ; i<256 ; ++i)
	{
		QList<QStandardItem *> items;
		QStandardItem *item = new QStandardItem(Data::drawPoints[i] != -1 ? Data::magics().value(Data::drawPoints[i]) : QString::number(i));
		item->setEditable(false);
		items.append(item);

		item = new QStandardItem(Data::drawPointsLoc[i] != -1 ? Data::locations().value(Data::drawPointsLoc[i]) : tr("Nulle part"));
		if(i == 12) {
			item->setText(item->text().append("/" + Data::locations().at(182)));
		}
		item->setEditable(false);
		items.append(item);

		item = new QStandardItem();
		item->setData(SpinBoxDelegate::ComboBoxDraw, Qt::UserRole);
		items.append(item);

		drawE_model->appendRow(items);
	}

	drawE_list->header()->setStretchLastSection(false);
	drawE_list->header()->HEADER_VIEW_SET_RESIZE_MODE(0, QHeaderView::ResizeToContents);
	drawE_list->header()->HEADER_VIEW_SET_RESIZE_MODE(1, QHeaderView::Stretch);

	font.setPixelSize(10);

	listAll = new QComboBox(this);
	listAll->setFont(font);
	listAll->addItem(tr("Toutes pleines"));
	listAll->addItem(tr("Toutes moitié pleines"));
	listAll->addItem(tr("Toutes vides"));
	listAll->addItem(tr("Toutes épuisées"));

	QPushButton *okAll = new QPushButton(tr("OK"), this);
	okAll->setFont(font);
	connect(okAll, SIGNAL(clicked()), SLOT(all()));

	QHBoxLayout *allLayout = new QHBoxLayout;
	allLayout->addStretch();
	allLayout->addWidget(listAll);
	allLayout->addWidget(okAll);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(drawE_list);
	layout->addLayout(allLayout);
	layout->setContentsMargins(QMargins());

	connect(drawE_model, SIGNAL(itemChanged(QStandardItem*)), SLOT(setItemColor(QStandardItem*)));
}

void DrawPointEditor::fillPage()
{
	QStandardItem *item;
	int drawState;
	QStringList drawStates;
	drawStates << tr("Pleine") << tr("Moitié pleine") << tr("Vide") << tr("Épuisée");

	for(int i=0 ; i<256 ; ++i)
	{
		item = drawE_model->item(i, 2);
		drawState = (data->misc3.draw_points[i/4] >> ((i % 4)*2)) & 0x3;
		item->setData(drawState);
		item->setText(drawStates.at(drawState));
	}
}

void DrawPointEditor::savePage()
{
	QStandardItem *item;
	quint8 drawState;

	for(int i=0 ; i<64 ; ++i)
	{
		drawState = 0;
		for(int j=0 ; j<4 ; ++j)
		{
			item = drawE_model->item(i*4 + j, 2);
			drawState |= item->data().toInt() << (j*2);
		}
		data->misc3.draw_points[i] = drawState;
	}
}

void DrawPointEditor::setItemColor(QStandardItem *item)
{
	switch(item->data().toInt()) {
	case 0:		item->setForeground(DRAWPOINT_COLOR_FILLED);		break;
	case 1:		item->setForeground(DRAWPOINT_COLOR_HALFFILLED);	break;
	case 2:		item->setForeground(DRAWPOINT_COLOR_EMPTY);			break;
	case 3:		item->setForeground(DRAWPOINT_COLOR_CEMPTY);		break;
	}
}

void DrawPointEditor::all()
{
	int all = listAll->currentIndex();
	QStringList drawStates;
	drawStates << tr("Pleine") << tr("Moitié pleine") << tr("Vide") << tr("Épuisée");
	QString allText = drawStates.at(all);

	for(int i=0 ; i<256 ; ++i) {
		QStandardItem *item = drawE_model->item(i, 2);
		item->setData(all);
		item->setText(allText);
	}
}
