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
/****************************************************************************
 ** Part of this code was written by Qhimm in its Griever program.
 ** Source code available at http://qhimm.com/
 ****************************************************************************/

#ifndef DEF_SAVEDATA
#define DEF_SAVEDATA

#define COUNTRY_JP	'I'
#define COUNTRY_US	'A'
#define COUNTRY_EU	'E'

#include <QtCore>

struct GFORCES//68
{
	char name[12];
	quint32 exp;
	quint8 u1;
	quint8 exists;
	quint16 HPs;
	quint8 completeAbilities[16];//115+1 valides, 124 existantes
	quint8 APs[24];// 22 + 2 inused
	quint16 kills;
	quint16 KOs;
	quint8 learning;
	quint32 forgotten : 24;// 22 + 2 inused
}__attribute__((__packed__));

struct PERSONNAGES//152
{
	quint16 current_HPs;
	quint16 HPs;
	quint32 exp;
	quint8 ID;
	quint8 weaponID;
	quint8 VGR;
	quint8 DFS;
	quint8 MGI;
	quint8 PSY;
	quint8 VTS;
	quint8 CHC;
	quint16 magies[32];
	quint8 commands[3];
	quint8 u1;// unused command (padding)
	quint8 abilities[4];
	quint16 gfs;
	quint8 u2;// used unknown value
	quint8 alternative_model;// Seed costume/Galbadia costume
	quint8 j_HP;
	quint8 j_VGR;
	quint8 j_DFS;
	quint8 j_MGI;
	quint8 j_PSY;
	quint8 j_VTS;
	quint8 j_ESQ;
	quint8 j_PRC;
	quint8 j_CHC;
	quint8 j_attEle;
	quint8 j_attMtl;
	quint8 j_defEle[4];
	quint8 j_defMtl[4];
	quint8 u3;// padding ?
	quint16 compatibility[16];
	quint16 kills;
	quint16 KOs;
	quint8 exists;
	quint8 u4;
	quint8 status;
	quint8 u5;// padding ?
}__attribute__((__packed__));

struct SHOP//20
{
	quint8 items[16];
	quint8 visited;
	quint8 u1[3];// padding ?
}__attribute__((__packed__));

struct CONFIG//20
{
	quint8 vts_combat;
	quint8 vts_msg_combat;
	quint8 vts_msg;
	quint8 analog_volume;
	quint8 divers;//joystick|vibration|manette|???|???|pointeur|son|ATB
	/* unsigned ATB : 1;
	unsigned son : 1;
	unsigned pointeur : 1;
	unsigned u2 : 1;
	unsigned u1 : 1;
	unsigned manette : 1;
	unsigned vibration : 1;
	unsigned joystick : 1; */
	quint8 scan;
	quint8 camera;
	quint8 u3;
	quint8 L2;
	quint8 R2;
	quint8 L1;
	quint8 R1;
	quint8 TRIANGLE;
	quint8 ROND;
	quint8 CROIX;
	quint8 CARRE;
	quint8 SELECT;
	quint8 u4;
	quint8 u5;
	quint8 START;
}__attribute__((__packed__));

struct MISC1//32
{
	quint8 party[4];// party[3] always=255
	quint32 unlocked_weapons;
	char griever[12];
	quint16 u1;// always 7966?
	quint16 u2;// (START) 0000 0000 0010 0000 -> (CD3) 0000 0011 0010 0000 -> (CD4) 0000 0111 0010 0000
	quint32 gils;
	quint32 dream_gils;
}__attribute__((__packed__));

struct LIMITB//16
{
	quint16 quistis;
	quint16 zell;
	quint8 irvine;
	quint8 selphie;
	quint8 angel_completed;
	quint8 angel_known;
	quint8 angel_pts[8];
}__attribute__((__packed__));

struct ITEMS//428
{
	quint8 battle_order[32];
	quint16 items[198];
}__attribute__((__packed__));

struct MISC2//144
{
	quint32 game_time;
	quint32 countdown;
	// Battle vars (75)
	quint32 u1;
	quint32 victory_count;
	quint16 u2;
	quint16 battle_escaped;
	quint32 u3;
	quint32 tomberry_vaincus;
	quint32 tomberry_sr_vaincu;
	quint32 u4;
	quint32 elmidea_battle_r1;
	quint32 succube_battle_elemental;
	quint32 trex_battle_mental;
	quint32 battle_irvine;// first battle with Irvine who explains limit break
	quint8 magic_drawn_once[8];
	quint8 ennemy_scanned_once[20];
	quint8 renzokuken_auto;
	quint8 renzokuken_indicator;
	quint8 dream;//dream|Odin|Phoenix|Gilgamesh|Angelo disabled|Angel Wing enabled|???|???
	// /Battle vars
	quint8 tutorial_infos[16];
	quint8 testLevel;
	// pos=100
	quint32 u5;
	quint8 party[4];// party[3] always=255
	quint8 u6[4];
	quint16 module;// 1=field, 2=worldmap, 3=battle
	quint16 location;
	quint16 location_last;
	qint16 x[3];// coord x (party1, party2, party3)
	qint16 y[3];// coord y (party1, party2, party3)
	quint16 id[3];// triangle (party1, party2, party3)
	quint8 dir[3];// direction (party1, party2, party3)
	quint8 u7[5];
}__attribute__((__packed__));

struct MISC3//256
{
	quint32 u1;// Const : "FF-8"
	quint32 steps;
	quint32 payslip;
	quint32 u2;
	quint16 seedExp;
	quint16 u3;
	quint32 victory_count;
	quint16 u4;
	quint16 battle_escaped;
	quint16 kills[8];// squall-zell-irvine-quistis-rinoa-selphie-seifer-edea
	quint16 ko[8];// squall-zell-irvine-quistis-rinoa-selphie-seifer-edea
	quint8 u5[8];// 0 ?
	quint32 monster_kills; // var68
	quint32 gils;
	quint32 dream_gils;
	quint32 current_frame;
	quint16 last_field_id;
	quint8 current_car_rent;
	quint8 music_util;
	quint8 move_find_ondine; // var88
	quint8 u6[15];// 0 ?
	quint32 u7;// var104 start with 0x00000018 (18 <=> 0001 1000: SARALYOFF & SARALYDISPOFF | 8 <=> 0000 1000: SARALYDISPON | 0 <=> 0000 0000: SARALYON)
	// 1024 <=> 0000 0100 ...: MUSICLOAD | 512 <=> 0000 0010 ...: PHSPOWER (0)
	quint32 music_related;// start with 0xffffffff
	quint32 u8;// 0xffffffff ?
	quint8 draw_points[64];// 32 field, 32 worldmap
	quint16 steps2;
	quint16 battle_mode; // var182
	quint16 u9;// var184 start with 0xffff
	quint8 uA[11];
	quint8 music_volume; // var197
	quint8 uB;
	quint8 music_played; // var199
	quint8 uC;
	quint8 music_is_played; // var201
	quint8 uD[2];
	quint8 battle_music; // var204
	quint8 disc; // var205
	quint8 uE;
	quint8 music_is_loaded; // var207
	quint8 battle_off; // var208
	quint8 save_enabled; // var209 (1: save enabled, 2: PHS enabled)
	quint8 uF[3];
	quint8 music_loaded; // var213
	quint8 uG[42];
}__attribute__((__packed__));

struct FIELD//1024
{
	quint16 game_moment;// var256 in field scripts
	quint8 ward_unused;// unused text at gpbigin2
	quint8 u1[2];
	quint8 save_flag;
	quint8 u2[10];
	quint8 tt_rules[8];
	quint8 tt_traderules[8];
	quint8 tt_lastrules[2];
	quint8 tt_lastregion[2];
	quint8 u3;// Unused in save
	quint8 tt_currules;// Unused in save
	quint8 tt_curtraderule;// Unused in save
	quint8 tt_cardqueen_location;
	quint8 tt_traderating_region;
	quint8 tt_traderating;
	quint8 tt_degeneration;
	quint8 tt_curtraderulequeen;
	quint8 tt_cardqueen_quest;
	quint8 u4[3];
	quint16 timber_maniacs;
	quint8 u7[974];//			(4285=tt club cc|4286=tt victory count BGU)
}__attribute__((__packed__));

struct WORLDMAP//128
{
	qint16 char_pos[6];// x z y ? ? rot(0->4095)
	qint16 uknown_pos1[6];
	qint16 ragnarok_pos[6];
	qint16 bgu_pos[6];
	qint16 car_pos[6];
	qint16 uknown_pos2[6];
	qint16 uknown_pos3[6];
	qint16 uknown_pos4[6];
	quint16 steps_related;
	quint8 car_rent;// 0x00:forbidden|0xFF:none|0x51:balamb1|0x52:balamb2|0x57:esthar
	quint8 u1[7];//u1[6] = deep sea?
	quint16 u2;// always 0xFFFF?
	quint16 u3;//u3(1] = deep sea?
	// Next bytes don't work with french PC version
	quint8 disp_map_config;// 0:none|1:minisphere|2:minimap
	quint8 u4;
	quint16 car_steps_related;
	quint16 car_steps_related2;
	quint8 vehicles_instructions_worldmap;//voiture|Unused|BGU|Chocobo|Hydre|???|???|Unused
	quint8 koyok_quest;//04 : Mandy Beach|Winhill|Trabia|Kashkabald Desert|UFO? Battu|80 : Koyo K Battu/Soigné/Mangé
	quint8 obel_quest[8];
	/* [0] => avoir fredonné twice|???|Unused|Unused|n ricochets|infini ricochets|Vu ryo|Vu ryo² ("100x + de ricochets que toi !")
	 * [1] => Ryo a donné tablette|Unused|Indices ombre pour trouver l'idiot|Unused|Unused|Unused|Indice ombre pour Eldbeak|Eldbeak trouvé
	 * [2] => Trésor île Minde|Trésor Plaine de Mordor|Unused|Unused|Unused|Unused|Unused|Unused
	 * [3] => ???|Pierre Balamb|Pierre Ryo|Pierre Timber|Pierre Galbadia|Toutes les pierres|Indice Ombre pour Balamb|???
	 * [4] => ??? (mordor var?)
	 * [5] => ???|???|???|???|Block access Lunatic Pandora|???|Block access Lunatic Pandora|???
	 * [6] => avoir parlé à l'ombre|Accepter de chercher l'idiot|Avoir vu l'idiot|...
	 * [7] => ??? (temp var)
	 */
	quint8 u6[2];
}__attribute__((__packed__));

struct WORLDMAP_PC//26 (padding 8)
{
	quint8 padding1[6];
	quint8 disp_map_config;// 0:none|1:minisphere|2:minimap
	quint8 u4;
	quint16 padding2;
	quint16 car_steps_related;
	quint16 car_steps_related2;
	quint8 vehicles_instructions_worldmap;//voiture|Unused|BGU|Chocobo|Hydre|???|???|Unused
	quint8 koyok_quest;//04 : Mandy Beach|Winhill|Trabia|Kashkabald Desert|UFO? Battu|80 : Koyo K Battu/Soigné/Mangé
	quint8 obel_quest[8];
	/* [0] => avoir fredonné twice|???|Unused|Unused|n ricochets|infini ricochets|Vu ryo|Vu ryo² ("100x + de ricochets que toi !")
	 * [1] => Ryo a donné tablette|Unused|Indices ombre pour trouver l'idiot|Unused|Unused|Unused|Indice ombre pour Eldbeak|Eldbeak trouvé
	 * [2] => Trésor île Minde|Trésor Plaine de Mordor|Unused|Unused|Unused|Unused|Unused|Unused
	 * [3] => ???|Pierre Balamb|Pierre Ryo|Pierre Timber|Pierre Galbadia|Toutes les pierres|Indice Ombre pour Balamb|???
	 * [4] => ??? (mordor var?)
	 * [5] => ???|???|???|???|Block access Lunatic Pandora|???|Block access Lunatic Pandora|???
	 * [6] => avoir parlé à l'ombre|Accepter de chercher l'idiot|Avoir vu l'idiot|...
	 * [7] => ??? (temp var)
	 */
	quint8 u6[2];
}__attribute__((__packed__));

struct TTCARDS//128
{
	quint8 cards[77];
	quint8 card_locations[33];
	quint8 cards_rare[5];
	quint8 u1; // padding ?
	quint16 tt_victory_count;
	quint16 tt_defeat_count;
	quint16 tt_egality_count;
	quint16 u2;
	quint32 u3;
}__attribute__((__packed__));

struct CHOCOBO//64
{
	quint8 enabled;// Enabled|Dans le world|MiniMog trouvé|Roi démon vaincu|Koko enlevée|Dépêche-toi !|Koko rencontrée|Event Wait off
	quint8 level;
	quint8 current_hp;
	quint8 max_hp;
	quint16 weapon;// 4 bit = 1 weapon
	quint8 move;
	quint8 u1[5];
	quint16 id_related;
	quint8 u2[31];
	quint8 boko_attack;// star count (chocobraise | chocoflammes | chocométéore | grochocobo)
	quint8 u3[18];
}__attribute__((__packed__));

/*
  A chercher :
1. Event WAIT : ON ou OFF
2. le Move  : 1, 2, 3, 4, 5, ou 6
3. Etat MiniMog : Stand by ou Sleep (à mon avis y a aussi un 3e caché pour dire si on montre ou pas ce menu qui dépend de si on a trouvé ou pas MiniMog)
*/

struct MAIN//4944 (~4242 used)
{
	GFORCES gfs[16];//			(pos=464|ar_pos_fr=pos + 489104)	[1088/1088 editable]
	PERSONNAGES persos[8];//	(pos=1552)		[1216/1216 editable]
	SHOP shops[20];//			(pos=2768)		[400/400 editable]
	CONFIG config;//			(pos=3168)		[20/20 editable]
	MISC1 misc1;//				(pos=3188)		[32/32 editable]
	LIMITB limitb;//			(pos=3220)		[16/16 editable]
	ITEMS items;//				(pos=3236)		[428/428 editable]
	MISC2 misc2;//				(pos=3664)		[104/144 editable]
	MISC3 misc3;//				(pos=3808)		[138/256 editable]
	FIELD field;//				(pos=4064)		[30/1024 editable] (702 unused)
	WORLDMAP worldmap;//		(pos=5088)		[13/128 editable]
	TTCARDS ttcards;//			(pos=5216)		[128/128 editable]
	CHOCOBO chocobo;//			(pos=5344)		[8/64 editable]
}__attribute__((__packed__));

struct HEADER//76	(pos=388)		[58+18(auto)/76 editable]
{
	quint16 locationID;
	quint16 hpLeader;// auto
	quint16 hpMaxLeader;
	quint16 saveCount;
	quint32 gils;// auto
	quint32 time;// auto
	quint8 nivLeader;// auto
	quint8 party[3];// auto
	char squall[12];
	char linoa[12];
	char angelo[12];
	char boko[12];
	quint32 disc;// auto
	quint32 curSave;
}__attribute__((__packed__));

#include "FF8Text.h"
#include "SaveIcon.h"
#include "Data.h"
#include "LZS.h"

class SaveData
{
public:
	SaveData();
	SaveData(int id, const QByteArray &data, const QByteArray &MCHeader, bool isVmp);
	void open(const QByteArray &data, const QByteArray &MCHeader);
	QByteArray save() const;
	void remove();
	QString perso(quint8 id) const;
	QString getShortDescription() const;
	bool exportPC(const QString &path) const;
	void restore();
	const QByteArray &MCHeader() const;
	char MCHeaderCountry() const;
	QString MCHeaderCode() const;
	QString MCHeaderId() const;
	QByteArray saveMCHeader();// with xor byte
	static QByteArray emptyMCHeader();
	const QByteArray &header() const;
	void setMCHeader(const QByteArray &);
	void setMCHeader(bool exists, char country, const QString &code, const QString &id);
	QPixmap icon(bool chocobo_world_icon=false) const;
	const SaveIcon *saveIcon() const;
	const HEADER &descData() const;
	const MAIN &mainData() const;
	void setSaveData(const HEADER &, const MAIN &);
	bool isModified() const;
	void setModified(bool modified);
	int freqValue() const;
	int id() const;
	void setId(int id);
	bool isFF8() const;
	bool isDelete() const;
	void setIsTheLastEdited(bool isTheLast);
	bool isTheLastEdited() const;
	bool hasMCHeader() const;
	bool isJp() const;
	bool isVmp() const;
	static quint8 xorByte(const char *data);
private:
	bool setData(const QByteArray &data);
	static quint16 calcChecksum(const char *data);
	static const quint16 crcTab[256];

	QByteArray _MCHeader;
	QByteArray _header;
	SaveIcon _icon;
	HEADER _descData;
	MAIN _mainData;
	int _freqValue, _id;
	bool _isFF8, _isDelete, _isTheLastEdited;
	bool _isVmp;
	bool _isModified;
	QByteArray _saveData;
};

#endif
