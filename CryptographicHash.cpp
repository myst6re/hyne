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
// From vita-mcr2vmp by @dots_tb (https://github.com/dots-tb/vita-mcr2vmp) - signs PSOne MCR files to create VMP files for use with Sony Vita/PSP and exports MCR files from VMP
// With help from the CBPS (https://discord.gg/2nDCbxJ) , especially:
// @AnalogMan151
// @teakhanirons

#include "CryptographicHash.h"

#include "Aes.h"
#include "Sha1.h"

uint8_t CryptographicHash::vmp_key[0x10] = {0xAB, 0x5A, 0xBC, 0x9F, 0xC1, 0xF4, 0x9D, 0xE6, 0xA0, 0x51, 0xDB, 0xAE, 0xFA, 0x51, 0x88, 0x59};
uint8_t CryptographicHash::vmp_iv[0x10] = {0xB3, 0x0F, 0xFE, 0xED, 0xB7, 0xDC, 0x5E, 0xB7, 0x13, 0x3D, 0xA6, 0x0D, 0x1B, 0x6B, 0x2C, 0xDC};

uint8_t CryptographicHash::psv_key[2][0x10] = {
	{0xFA, 0x72, 0xCE, 0xEF, 0x59, 0xB4, 0xD2, 0x98, 0x9F, 0x11, 0x19, 0x13, 0x28, 0x7F, 0x51, 0xC7},
	{0xAB, 0x5A, 0xBC, 0x9F, 0xC1, 0xF4, 0x9D, 0xE6, 0xA0, 0x51, 0xDB, 0xAE, 0xFA, 0x51, 0x88, 0x59}
};
uint8_t CryptographicHash::psv_iv[0x10] = {0xB3, 0x0F, 0xFE, 0xED, 0xB7, 0xDC, 0x5E, 0xB7, 0x13, 0x3D, 0xA6, 0x0D, 0x1B, 0x6B, 0x2C, 0xDC};

#define VMP_SEED_OFFSET 0xC
#define VMP_HASH_OFFSET 0x20
#define VMP_MCR_OFFSET 0x80
#define PMV_MAGIC 0x564D5000
#define VMP_SZ 0x20080
#define MC_SZ 0x20000

#define PSV_SEED_OFFSET 0x8
#define PSV_HASH_OFFSET 0x1c
#define PSV_TYPE_OFFSET 0x3C
#define PSV_MAGIC 0x50535600

void CryptographicHash::xorWithByte(quint8 *buf, quint8 byte, quint8 length)
{
	for (quint8 i = 0; i < length; ++i) {
		buf[i] ^= byte;
	}
}

QByteArray CryptographicHash::hashVmp(const QByteArray &data)
{
	QByteArray ret = data;
	struct AES_ctx aes_ctx;
	AES_init_ctx_iv(&aes_ctx, vmp_key, vmp_iv);

	quint8 salt[0x40];
	quint8 work_buf[0x14];
	const char *salt_seed = ret.constData() + VMP_SEED_OFFSET;

	memcpy(work_buf, salt_seed, 0x10);
	AES_ECB_decrypt(&aes_ctx, work_buf);
	memcpy(salt, work_buf, 0x10);

	memcpy(work_buf, salt_seed, 0x10);
	AES_ECB_encrypt(&aes_ctx, work_buf);
	memcpy(salt + 0x10, work_buf, 0x10);

	XorWithIv(salt, vmp_iv);

	memset(work_buf, 0xFF, sizeof(work_buf));
	memcpy(work_buf, salt_seed + 0x10, 0x4);
	XorWithIv(salt + 0x10, work_buf);

	memset(salt + 0x14, 0, sizeof(salt) - 0x14);
	xorWithByte(salt, 0x36, 0x40);

	SHA1_CTX sha1_ctx_1;
	SHA1Init(&sha1_ctx_1);

	SHA1Update(&sha1_ctx_1, salt, 0x40);

	memset(ret.data() + VMP_HASH_OFFSET, 0, 0x14);
	SHA1Update(&sha1_ctx_1, (const unsigned char *)ret.constData(), VMP_SZ);

	xorWithByte(salt, 0x6A, 0x40);

	SHA1Final(work_buf, &sha1_ctx_1);

	SHA1_CTX sha1_ctx_2;
	SHA1Init(&sha1_ctx_2);
	SHA1Update(&sha1_ctx_2, salt, 0x40);
	SHA1Update(&sha1_ctx_2, work_buf, 0x14);

	SHA1Final((unsigned char *)ret.data() + VMP_HASH_OFFSET, &sha1_ctx_2);

	return ret;
}

QByteArray CryptographicHash::hashPsv(const QByteArray &data)
{
	QByteArray ret = data;
	const char *input = ret.constData(),
	        *salt_seed = input + PSV_SEED_OFFSET;
	char *hash_input = ret.data() + PSV_HASH_OFFSET;
	struct AES_ctx aes_ctx;
	quint8 salt[0x40], work_buf[0x14];

	memset(salt, 0, sizeof(salt));

	//idk why the normal cbc doesn't work.
	AES_init_ctx_iv(&aes_ctx, psv_key[1], psv_iv);
	memcpy(work_buf, salt_seed, 0x10);
	AES_ECB_decrypt(&aes_ctx, work_buf);
	memcpy(salt, work_buf, 0x10);

	memcpy(work_buf, salt_seed, 0x10);
	AES_ECB_encrypt(&aes_ctx, work_buf);
	memcpy(salt + 0x10, work_buf, 0x10);

	XorWithIv(salt, psv_iv);

	memset(work_buf, 0xFF, sizeof(work_buf));
	memcpy(work_buf, salt_seed + 0x10, 0x4);
	XorWithIv(salt + 0x10, work_buf);

	memset(salt + 0x14, 0, sizeof(salt) - 0x14);

	xorWithByte(salt, 0x36, 0x40);

	SHA1_CTX sha1_ctx_1;
	SHA1Init(&sha1_ctx_1);

	SHA1Update(&sha1_ctx_1, salt, 0x40);

	memset(hash_input, 0, 0x14);
	SHA1Update(&sha1_ctx_1, (const unsigned char *)input, quint32(ret.size()));

	xorWithByte(salt, 0x6A, 0x40);

	SHA1Final(work_buf, &sha1_ctx_1);

	SHA1_CTX sha1_ctx_2;
	SHA1Init(&sha1_ctx_2);
	SHA1Update(&sha1_ctx_2, salt, 0x40);
	SHA1Update(&sha1_ctx_2, work_buf, 0x14);

	SHA1Final((unsigned char *)hash_input, &sha1_ctx_2);

	return ret;
}
