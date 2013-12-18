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

#include "FF8Text.h"

QImage *FF8Text::fontImage = 0;

QString FF8Text::toString(const QByteArray &ff8str, bool jp)
{
	QString ret, character;
	quint8 index, oldIndex;
	int size = ff8str.size();

	for(int i=0 ; i<size ; ++i) {
		index = (quint8)ff8str.at(i);
		if(index==0x00)						break;
		/*else if(index==0x01)
			ret.append("\n{NewPage}\n");
		else if(index==0x02)
			ret.append('\n');
		else if(index==0x03) {//{Name}
			if((++i) < size) {
				index = (quint8)ff8str.at(i);
				if(index>=0x30 && index<=0x3a)
					ret.append(names[index-0x30]);
				else if(index==0x40)
					ret.append(names[11]);
				else if(index==0x50)
					ret.append(names[12]);
				else if(index==0x60)
					ret.append(names[13]);
				else
					ret.append(QString("{x03%1}").arg(index, 2, 16, QChar('0')));
			}
			else
				ret.append("{x03}");
		}
		else if(index==0x04) {//{Var0}, {Var00} et {Varb0}
			if((++i) < size) {
				index = (quint8)ff8str.at(i);
				if(index>=0x20 && index<=0x27)
					ret.append(QString("{Var%1}").arg(index-0x20));
				else if(index>=0x30 && index<=0x37)
					ret.append(QString("{Var0%1}").arg(index-0x30));
				else if(index>=0x40 && index<=0x47)
					ret.append(QString("{Varb%1}").arg(index-0x40));
				else
					ret.append(QString("{x04%1}").arg(index, 2, 16, QChar('0')));
			}
			else
				ret.append("{x04}");
		}
		else if(index==0x06) {//{Color}
			if((++i) < size) {
				index = (quint8)ff8str.at(i);
				if(index>=0x20 && index<=0x2f)
					ret.append(colors[index-0x20]);
				else
					ret.append(QString("{x06%1}").arg(index, 2, 16, QChar('0')));
			}
			else
				ret.append("{x06}");
		}
		else if(index==0x09) {//{Wait000}
			if((++i) < size) {
				index = (quint8)ff8str.at(i);
				if(index>=0x20)
					ret.append(QString("{Wait%1}").arg(index-0x20, 3, 10, QChar('0')));
				else
					ret.append(QString("{x09%1}").arg(index, 2, 16, QChar('0')));
			}
			else
				ret.append("{x09}");
		}
		else if(index==0x0e) {//{Location}
			if((++i) < size) {
				index = (quint8)ff8str.at(i);
				if(index>=0x20 && index<=0x27)
					ret.append(locations[index-0x20]);
				else
					ret.append(QString("{x0e%1}").arg(index, 2, 16, QChar('0')));
			}
			else
				ret.append("{x0e}");
		}*/
		else if(jp && index>=0x19 && index<=0x1b) {//jp19, jp1a, jp1b
			if((++i) < size) {
				oldIndex = index;
				index = (quint8)ff8str.at(i);
				if(index>=0x20) {
					character = caract(index, oldIndex-0x17);
				} else {
					character = QString();
				}
				if(character.isEmpty()) {
					character = QString("{x%1%2}").arg(oldIndex, 2, 16, QChar('0')).arg(index, 2, 16, QChar('0'));
				}
				ret.append(character);
			} else {
				ret.append(QString("{x%1}").arg(index, 2, 16, QChar('0')));
			}
		}
		else if(index>=0x01 && index<=0x1f) {
			if((++i) < size)
				ret.append(QString("{x%1%2}").arg(index, 2, 16, QChar('0')).arg((quint8)ff8str.at(i), 2, 16, QChar('0')));
			else
				ret.append(QString("{x%1}").arg(index, 2, 16, QChar('0')));
		}
		else {
			character = caract(index, jp);
			if(character.isEmpty()) {
				character = QString("{x%1}").arg(index, 2, 16, QChar('0'));
			}
			ret.append(character);
		}
	}

	return ret;
}

QByteArray FF8Text::toByteArray(const QString &string, bool jp)
{
	QByteArray ff8str;
	QChar comp;
	int stringSize = string.size(), i, table;
	bool ok, ok2;
	ushort value, value2;

	for(int c=0 ; c<stringSize ; ++c)
	{
		comp = string.at(c);
		if(comp=='\n') {//\n{NewPage}\n,\n
			if(string.mid(c+1, 10).compare("{NewPage}\n", Qt::CaseInsensitive) == 0) {
				ff8str.append('\x01');
				c += 10;
			}
			else
				ff8str.append('\x02');
			continue;
		}
		else if(comp=='{') {
			QString rest = string.mid(c, 4);
			for(i=0xe8 ; i<=0xff ; ++i)
			{
				if(QString::compare(rest, caract(i))==0)
				{
					ff8str.append((char)i);
					c += 3;
					goto end;
				}
			}

			if(string.at(c+1)=='x') {
				if(string.at(c+4)=='}') {//{xff}
					value = string.mid(c+2,2).toUShort(&ok,16);
					if(ok) {
						ff8str.append((char)value);
						c += 4;
						continue;
					}
				}
				else if(string.at(c+6)=='}') {//{xffff}
					value = string.mid(c+2,2).toUShort(&ok,16);
					value2 = string.mid(c+4,2).toUShort(&ok2,16);
					if(ok && ok2) {
						ff8str.append((char)value);
						ff8str.append((char)value2);
						c += 6;
						continue;
					}
				}
			}

			continue;// character '{' is not in ff8 table
		}

		for(i=0x20 ; i<=0xff ; ++i)
		{
			if(QString::compare(comp, caract(i, jp))==0)
			{
				ff8str.append((char)i);
				goto end;
			}
		}
		if(jp) {
			// Latin conversion
			if(comp.isLetter()) {
				int shift;
				if(comp.isLower()) {
					shift = comp.toAscii() - 'a';
				} else {
					shift = comp.toAscii() - 'A';
				}
				ff8str.append((char)(0xCE + shift));
				goto end;
			}

			if(comp.isDigit()) {
				int shift = comp.toAscii() - '0';
				ff8str.append((char)(0x53 + shift));
				goto end;
			}

			if(comp == ' ') {
				ff8str.append((char)(0x5F));
				goto end;
			}
			if(comp == '!') {
				ff8str.append((char)(0xE8));
				goto end;
			}
			if(comp == '?') {
				ff8str.append((char)(0xE9));
				goto end;
			}
			if(comp == '+') {
				ff8str.append((char)(0xEB));
				goto end;
			}
			if(comp == '-') {
				ff8str.append((char)(0xEC));
				goto end;
			}
			if(comp == '=') {
				ff8str.append((char)(0xED));
				goto end;
			}
			if(comp == '*') {
				ff8str.append((char)(0xEE));
				goto end;
			}
			if(comp == '/') {
				ff8str.append((char)(0xEF));
				goto end;
			}
			if(comp == '%') {
				ff8str.append((char)(0xF0));
				goto end;
			}
			if(comp == '&') {
				ff8str.append((char)(0xF1));
				goto end;
			}
			if(comp == '(') {
				ff8str.append((char)(0xF4));
				goto end;
			}
			if(comp == ')') {
				ff8str.append((char)(0xF5));
				goto end;
			}
			if(comp == '·') {
				ff8str.append((char)(0xFA));
				goto end;
			}
			if(comp == '.') {
				ff8str.append((char)(0xFB));
				goto end;
			}
			if(comp == ',') {
				ff8str.append((char)(0xFC));
				goto end;
			}
			if(comp == ':') {
				ff8str.append((char)(0xFD));
				goto end;
			}
			if(comp == '~') {
				ff8str.append((char)(0xFE));
				goto end;
			}

			for(table=2 ; table<5 ; ++table)
			{
				for(i=0x20 ; i<=0xff ; ++i)
				{
					if(QString::compare(comp, caract(i, table))==0)
					{
						switch(table) {
						case 2:
							ff8str.append('\x19');
							break;
						case 3:
							ff8str.append('\x1a');
							break;
						case 4:
							ff8str.append('\x1b');
							break;
						}
						ff8str.append((char)i);
						goto end;
					}
				}
			}
		}

		end:;
	}

	return ff8str;
}

QString FF8Text::caract(quint8 ord, quint8 table)
{
	switch(table) {
	case 1:
		return QString::fromUtf8(_caractJp[ord-0x20]);
	case 2:
		return QString::fromUtf8(_caractJp19[ord-0x20]);
	case 3:
		return QString::fromUtf8(_caractJp1a[ord-0x20]);
	case 4:
		return QString::fromUtf8(_caractJp1b[ord-0x20]);
	default:
		return QString::fromUtf8(_caract[ord-0x20]);
	}
}

const char *FF8Text::names[14] =
{
	"{Squall}","{Zell}","{Irvine}","{Quistis}","{Rinoa}","{Selphie}","{Seifer}","{Edea}","{Laguna}","{Kiros}","{Ward}","{Angelo}","{Griever}","{Boko}"
};

const char *FF8Text::colors[16] =
{
	"{Darkgrey}","{Grey}","{Yellow}","{Red}","{Green}","{Blue}","{Purple}","{White}",
	"{DarkgreyBlink}","{GreyBlink}","{YellowBlink}","{RedBlink}","{GreenBlink}","{BlueBlink}","{PurpleBlink}","{WhiteBlink}"
};

const char *FF8Text::locations[8] =
{
	"{Galbadia}","{Esthar}","{Balamb}","{Dollet}","{Timber}","{Trabia}","{Centra}","{Horizon}"
};

const quint8 FF8Text::charWidth[5][224] =
{
	{ // International
		5, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 11, 7, 4, 5, 7,
		10, 7, 7, 7, 9, 9, 6, 6, 5, 5, 4, 4, 4, 9, 7, 7,
		4, 9, 7, 4, 9, 8, 8, 8, 8, 8, 8, 8, 8, 5, 7, 8,
		7, 9, 8, 8, 8, 9, 8, 8, 9, 8, 9, 9, 9, 9, 9, 8,
		7, 7, 7, 7, 5, 7, 7, 3, 5, 7, 3, 9, 7, 7, 7, 7,
		6, 7, 6, 7, 7, 7, 7, 8, 7, 8, 8, 8, 8, 8, 8, 8,
		8, 8, 5, 5, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9,
		7, 8, 8, 8, 8, 7, 7, 7, 7, 7, 5, 5, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 9, 13, 5, 5, 9, 9, 9, 6, 6,
		9, 11, 7, 7, 9, 4, 9, 9, 7, 11, 4, 7, 5, 5, 7, 13,
		10, 10, 7, 9, 4, 7, 8, 7, 5, 9, 10, 10, 8, 8, 14, 8,
		12, 8, 10, 8, 8, 8, 8, 8, 10, 8, 15, 8, 8, 8, 12, 8,
		8, 12, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	},{ // Jap - 0
		11, 12, 11, 11, 11, 11, 12, 12, 12, 12, 11, 12, 11, 11, 11, 11,
		11, 11, 11, 11, 12, 11, 11, 10, 11, 12, 11, 12, 11, 11, 11, 11,
		12, 10, 11, 11, 11, 11, 11, 11, 11, 11, 12, 11, 11, 11, 11, 12,
		12, 12, 12, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 7, 6, 8,
		11, 11, 10, 11, 10, 11, 12, 12, 11, 11, 10, 11, 11, 11, 10, 10,
		12, 11, 10, 10, 11, 10, 11, 10, 10, 11, 11, 11, 10, 11, 10, 11,
		12, 11, 11, 11, 11, 11, 11, 11, 10, 10, 11, 11, 10, 11, 11, 11,
		11, 11, 12, 11, 11, 11, 10, 11, 11, 11, 10, 11, 11, 10, 10, 12,
		11, 11, 11, 11, 11, 10, 11, 10, 9, 9, 11, 10, 9, 11, 11, 10,
		11, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 10, 9, 9, 9, 10,
		9, 9, 9, 8, 9, 9, 8, 9, 9, 7, 9, 9, 9, 9, 9, 9,
		10, 9, 9, 9, 10, 10, 7, 8, 10, 9, 11, 9, 11, 9, 11, 9,
		9, 10, 9, 9, 11, 9, 9, 9, 7, 10, 9, 9, 9, 9, 9, 11,
		12, 10, 6, 6, 6, 6, 13, 13, 13, 13, 6, 4, 5, 4, 11, 10
	},{ // Jap - 1
		7, 13, 12, 13, 13, 13, 13, 13, 13, 12, 12, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 12, 12, 13, 12, 13, 13, 12, 13, 12, 13,
		13, 13, 13, 13, 13, 12, 13, 13, 13, 13, 13, 13, 12, 13, 12, 12,
		13, 13, 13, 12, 13, 12, 13, 13, 13, 13, 13, 12, 12, 13, 13, 13,
		11, 12, 11, 13, 13, 12, 13, 13, 12, 13, 13, 13, 11, 13, 13, 13,
		11, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12, 13, 13, 13,
		13, 13, 13, 12, 13, 13, 13, 13, 13, 13, 13, 13, 12, 13, 11, 13,
		13, 13, 13, 13, 13, 13, 13, 12, 12, 13, 13, 13, 13, 13, 13, 13,
		13, 12, 13, 13, 13, 13, 13, 12, 13, 13, 13, 13, 13, 13, 13, 12,
		12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 11, 12,
		12, 12, 13, 13, 13, 13, 12, 13, 13, 13, 13, 13, 13, 13, 13, 12,
		13, 13, 13, 12, 13, 13, 13, 13, 13, 13, 12, 13, 13, 13, 13, 13,
		12, 12, 13, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12, 13, 12, 13, 13, 13
	},{ // Jap - 2
		13, 13, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 12, 13, 13, 12,
		13, 13, 13, 12, 13, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12,
		13, 13, 12, 13, 12, 13, 13, 13, 13, 13, 13, 13, 13, 12, 12, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 12, 13, 12, 13, 13, 13, 11,
		13, 13, 13, 12, 13, 13, 13, 13, 12, 13, 13, 13, 13, 13, 13, 13,
		12, 13, 13, 13, 13, 12, 13, 13, 13, 9, 13, 13, 13, 13, 12, 13,
		13, 13, 13, 12, 13, 13, 13, 13, 13, 13, 13, 12, 13, 13, 12, 13,
		13, 13, 12, 13, 13, 13, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 12, 12, 13, 13, 13, 13, 13, 13, 13, 12, 13, 12, 12, 13,
		13, 13, 13, 13, 12, 12, 13, 13, 13, 13, 13, 13, 12, 13, 13, 13,
		12, 13, 13, 13, 13, 13, 12, 13, 12, 13, 13, 13, 13, 13, 13, 13,
		12, 13, 13, 13, 12, 12, 13, 13, 12, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12, 13, 13, 13, 13,
		14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12, 13
	},{ // Jap - 3
		13, 13, 13, 11, 13, 13, 13, 13, 13, 13, 12, 13, 13, 13, 13, 13,
		13, 13, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 13, 13, 13,
		13, 11, 12, 13, 12, 13, 13, 13, 13, 12, 12, 13, 12, 13, 13, 13,
		13, 12, 13, 13, 13, 13, 13, 13, 12, 12, 13, 13, 13, 13, 12, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 11, 13, 13, 13, 13, 13, 13, 12, 12, 13,
		13, 13, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12, 13, 12,
		13, 13, 13, 13, 13, 13, 13, 13, 12, 13, 13, 13, 13, 13, 13, 13,
		13, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12, 13, 13, 13, 13,
		12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12, 13, 13, 12, 13, 13,
		13, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12, 13, 13,
		13, 12, 13, 13, 13, 13, 13, 13, 13, 13, 12, 13, 12, 13, 13, 13,
		9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	}
};

void FF8Text::reloadFont()
{
	if(fontImage) {
		delete fontImage;
	}
	fontImage = new QImage(QString(":/images/font%1.png").arg(Config::value(Config::Font)));
}

void FF8Text::drawTextArea(QPainter *painter, const QPoint &point, const QString &text, int forceLang)
{
	bool jp = forceLang==2 || (forceLang==0 && QObject::tr("false", "Use Japanese Encoding")=="true");
	QByteArray ff8Text = FF8Text::toByteArray(text, jp);
	if(ff8Text.isEmpty())	return;

	/* Text */

	int charId, x = point.x(), y = point.y(), size = ff8Text.size();

	for(int i=0 ; i<size ; ++i)
	{
		charId = (quint8)ff8Text.at(i);

		if(charId>=32)
		{
			if(jp) {
				letter(&x, &y, charId-32, painter, 1);// 210-32
			} else {
				if(charId<227) {
					letter(&x, &y, charId-32, painter);
				} else if(charId>=232) {
					letter(&x, &y, optimisedDuo[charId-232][0], painter);
					letter(&x, &y, optimisedDuo[charId-232][1], painter);
				}
			}
		}
		else if(charId<=2)//NewPage \n
			break;
		else if(charId<32)
		{
			++i;
			if(jp) {
				switch(charId)
				{
				case 0x19: // Jap 1
					letter(&x, &y, (quint8)ff8Text.at(i)-0x20, painter, 2);
					break;
				case 0x1a: // Jap 2
					letter(&x, &y, (quint8)ff8Text.at(i)-0x20, painter, 3);
					break;
				case 0x1b: // Jap 3
					letter(&x, &y, (quint8)ff8Text.at(i)-0x20, painter, 4);
					break;
				}
			}
		}
	}
}

void FF8Text::letter(int *x, int *y, int charId, QPainter *painter, quint8 tableId)
{
	int charIdImage = charId + 231*tableId;
	int charSrcWidth = Config::value(Config::Font).isEmpty() ? 12 : 24;

	if(!fontImage)
		reloadFont();

	painter->drawImage(QRect(*x, *y, 24, 24), fontImage->copy((charIdImage%21)*charSrcWidth, (charIdImage/21)*charSrcWidth, charSrcWidth, charSrcWidth));
	*x += charWidth[tableId][charId]*2;
}

const char *FF8Text::optimisedDuo[24] =
{
	"\x47\x4c",//in
	"\x43\x00",//e
	"\x4c\x43",//ne
	"\x52\x4d",//to
	"\x50\x43",//re
	"\x2c\x34",//HP
	"\x4a\x00",//l
	"\x4a\x4a",//ll
	"\x2b\x2a",//GF
	"\x4c\x52",//nt
	"\x47\x4a",//il
	"\x4d\x00",//o
	"\x43\x44",//ef
	"\x4d\x4c",//on
	"\x00\x55",// w
	"\x00\x50",// r
	"\x55\x47",//wi
	"\x44\x47",//fi
	"\x29\x27",//EC
	"\x51\x00",//s
	"\x3f\x50",//ar
	"\x2a\x29",//FE
	"\x00\x37",// S
	"\x3f\x45" //ag
};

QByteArray FF8Text::numToBiosText(quint32 num, quint8 width)
{
	QString chaine = QString("%1").arg(num, width, 10, QChar('0')).right(width);
	QByteArray ret;
	for(int i=0 ; i<width ; ++i)
	{
		ret.append('\x82');
		ret.append((char)(chaine.at(i).digitValue()+0x4f));
	}
	return ret;
}
