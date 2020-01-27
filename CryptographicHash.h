/****************************************************************************
 ** Hyne Final Fantasy VIII Save Editor
 ** Copyright (C) 2009-2020 Arzel Jérôme <myst6re@gmail.com>
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
#ifndef CRYPTOGRAPHICHASH_H
#define CRYPTOGRAPHICHASH_H

#include <QtCore>

class CryptographicHash
{
public:
	static QByteArray hashVmp(const QByteArray &data);
	static QByteArray hashPsv(const QByteArray &data);
private:
	static uint8_t vmp_key[0x10];
	static uint8_t vmp_iv[0x10];
	static uint8_t psv_key[2][0x10];
	static uint8_t psv_iv[0x10];
	static void xorWithByte(quint8 *buf, quint8 byte, quint8 length);
	static void generateHash(const char *input, char *dest, size_t sz, int type);
};

#endif // CRYPTOGRAPHICHASH_H
