/****************************************************************************
 ** Hyne Final Fantasy VIII Save Editor
 ** Copyright (C) 2012-2013 Arzel Jérôme <myst6re@gmail.com>
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
#include "GZIP.h"
#include <zlib.h>
#undef compress // conflict with GZIP::compress

#define BUF_SIZE	8192

QByteArray GZIP::decompress(const QByteArray &data, int/* decSize*/)
{
	QByteArray ungzip;

	QTemporaryFile temp;
	if (!temp.open()) {
		return QByteArray();
	}
	temp.write(data);
	temp.close();
	gzFile file = gzopen(temp.fileName().toLatin1(), "rb");
	if (!file) {
		return QByteArray();
	}
	char buffer[BUF_SIZE];
	int r;
	while ((r = gzread(file, buffer, BUF_SIZE)) > 0) {
		ungzip.append(buffer, r);
	}
	gzclose(file);

	return ungzip;
}

QByteArray GZIP::compress(const QByteArray &ungzip)
{
	QString tempPath = QDir::tempPath()+"/qt_temp.gz";

	gzFile file2 = gzopen(tempPath.toLatin1(), "wb9");
	if (!file2) {
		return QByteArray();
	}
	gzwrite(file2, ungzip.constData(), ungzip.size());
	gzclose(file2);
	QFile finalFile(tempPath);
	if (!finalFile.open(QIODevice::ReadOnly)) {
		return QByteArray();
	}

	QByteArray data = finalFile.readAll();
	finalFile.remove();

	return data;
}

bool GZIP::decompress(const QString &pathFrom, const QString &pathTo)
{
	QFile to(pathTo);
	if (!to.open(QIODevice::WriteOnly)) {
		return false;
	}

	gzFile file = gzopen(pathFrom.toLatin1(), "rb");
	if (!file) {
		return false;
	}
	char buffer[BUF_SIZE];
	int r;
	while ((r = gzread(file, buffer, BUF_SIZE)) > 0) {
		to.write(buffer, r);
	}
	gzclose(file);
	to.close();

	return true;
}

bool GZIP::compress(const QString &pathFrom, const QString &pathTo)
{
	QFile from(pathFrom);
	if (!from.open(QIODevice::ReadOnly)) {
		return false;
	}

	gzFile file = gzopen(pathTo.toLatin1(), "wb9");
	if (!file) {
		return false;
	}
	char buffer[BUF_SIZE];
	int r;
	while ((r = from.read(buffer, BUF_SIZE)) > 0) {
		gzwrite(file, buffer, BUF_SIZE);
	}
	gzclose(file);
	from.close();

	return true;
}
