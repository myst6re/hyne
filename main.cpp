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

#include <QApplication>
#include "Window.h"

// Only for static compilation
//Q_IMPORT_PLUGIN(qjpcodecs) // jp encoding

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	Config::set();

	QTranslator translator;
	QString lang = Config::value("lang", QLocale::system().name().toLower());
	if(translator.load("hyne_" + lang, app.applicationDirPath())) {
		app.installTranslator(&translator);
	} else {
		Config::setValue("lang", "fr");
	}
	Config::translator = &translator;

	FF8Text::fontImage = QImage(QString(":/images/font%1.png").arg(Config::value("font")));
	Data::load();
	Config::loadRecentFiles();

    Window fenetre;
	fenetre.show();

	if(argc>1) {
		fenetre.openFile(argv[1]);
	}

    return app.exec();
}
