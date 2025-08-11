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
 ** Part of this code was written by Qhimm in its Griever program.
 ** Source code available at http://qhimm.com/
 ****************************************************************************/

#pragma once

#define COUNTRY_JP	'I'
#define COUNTRY_US	'A'
#define COUNTRY_EU	'E'
#define FF8SAVE_SIZE		5408
#define SAVE_SIZE			8192

#include <QtCore>
#include "SaveIcon.h"

#ifdef _MSC_VER
#	define PACK(structure)			\
		__pragma(pack(push, 1))		\
		structure					\
		__pragma(pack(pop))
#else
#	define PACK(structure) structure Q_PACKED
#endif

#define GF_GET_FORGOTTEN(data) \
	quint32((data).forgotten1 | ((data).forgotten2 << 8) | ((data).forgotten3 << 16))

#define GF_SET_FORGOTTEN(data, forgotten)			\
	(data).forgotten1 = forgotten & 0xFF;				\
	(data).forgotten2 = (forgotten >> 8) & 0xFF;		\
	(data).forgotten3 = (forgotten >> 16) & 0xFF;

PACK(
struct GFORCES//68
{
	quint8 name[12];
	quint32 exp;
	quint8 u1;
	quint8 exists;
	quint16 HPs;
	quint8 completeAbilities[16];//115+1 valides, 124 existantes
	quint8 APs[24];// 22 + 2 unused
	quint16 kills;
	quint16 KOs;
	quint8 learning;
	quint8 forgotten1;// 22 + 2 unused
	quint8 forgotten2;
	quint8 forgotten3;
});

PACK(
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
});

PACK(
struct SHOP//20
{
	quint8 items[16];
	quint8 visited;
	quint8 u1[3];// padding ?
});

PACK(
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
});

PACK(
struct MISC1//32
{
	quint8 party[4];// party[3] always=255
	quint32 unlocked_weapons;
	quint8 griever[12];
	quint16 u1;// always 7966?
	quint16 u2;// (START) 0000 0000 0010 0000 -> (CD3) 0000 0011 0010 0000 -> (CD4) 0000 0111 0010 0000
	quint32 gils;
	quint32 dream_gils;
});

PACK(
struct LIMITB//16
{
	quint16 quistis;
	quint16 zell;
	quint8 irvine;
	quint8 selphie;
	quint8 angel_completed;
	quint8 angel_known;
	quint8 angel_pts[8];
});

PACK(
struct ITEMS//428
{
	quint8 battle_order[32];
	quint16 items[198]; // (quint8(itemID), quint8(quantity)) * 198
});

PACK(
struct MISC2//144
{
	quint32 game_time; // var75 (in battle scripts)
	quint32 countdown;
	// Battle vars (75)
	quint32 u1;
	quint32 victory_count;
	quint16 u2;
	quint16 battle_escaped;
	quint32 u3;
	quint32 tomberry_vaincus; // var81 (in battle scripts)
	quint32 tomberry_sr_vaincu; // var82 (in battle scripts)
	quint32 ufo_battle_encountered;
	quint32 elmidea_battle_r1; // var84 (in battle scripts)
	quint32 succube_battle_elemental; // var85 (in battle scripts)
	quint32 trex_battle_mental; // var86 (in battle scripts)
	quint32 battle_irvine;// first battle with Irvine who explains limit break, var87 (in battle scripts)
	quint8 magic_drawn_once[8];
	quint8 ennemy_scanned_once[20]; // TODO: scan pages for some ennemies (Belhelmel/Koatl or Jelleye/Héra for example)
	quint8 renzokuken_auto;// Renzo auto|Ring Master auto|???|???|???|???|???|???
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
});

PACK(
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
	quint8 uD;
	quint8 battle_music; // var203
	quint8 disc; // var204
	quint8 uE;
	quint8 music_is_loaded; // var206
	quint8 battle_off; // var207
	quint8 uF;
	quint8 save_enabled; // var209 (1: save enabled, 2: PHS enabled)
	quint8 uG[3];
	quint8 music_loaded; // var213
	quint8 uH[42];
});

PACK(
struct FIELD//1024
{
	quint16 game_moment;// var256 in field scripts
	quint8 ward_unused;// unused text at gpbigin2
	quint8 unused1[2];
	quint8 save_flag;
	quint8 unused2[2];
	quint8 wm_related[7];
	quint8 unused3;
	quint8 tt_rules[8];
	quint8 tt_traderules[8];
	quint8 tt_lastrules[2];
	quint8 tt_lastregion[2];
	quint8 tt_new_rules_tmp;// Unused in save
	quint8 tt_new_trade_rules_tmp;// Unused in save
	quint8 tt_add_this_rule_queen_tmp;// Unused in save
	quint8 tt_cardqueen_location;
	quint8 tt_traderating_region;
	quint8 tt_traderating;
	quint8 tt_degeneration;
	quint8 tt_curtraderulequeen;
	quint8 tt_cardqueen_quest;
	quint8 unused4[3];
	quint16 timber_maniacs; // var304 in field scripts
	quint8 u1[168];
	quint8 tt_players_bgu_dialogs1; // first cards BGU|CC Ragnarok init CD4|CC Ragnarok Joker|CC Ragnarok Quistis
	quint8 tt_players_bgu_dialogs2; // kadowaki 1|kadowaki 2|joker in prison|joker BGU|joker BGU CC|dormitory to Quistis night|Zell with cards
	quint8 tt_players_bgu_dialogs3; // CC Jack 1|CC Jack 2|CC Clover about Jack|CC Diamonds|CC Spades|CC Shu|Nida about Shu in CC|Running Child is alive!
	quint8 tt_cc_quest; // Jack | Clover | Spades | Shu | Diamonds
	quint8 tt_bgu_victory_count;
	quint8 u2[801];
});

PACK(
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
});

PACK(
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
});

PACK(
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
});

PACK(
struct CHOCOBO//64
{
	quint8 enabled;// Enabled|In world|MiniMog found|Demon King defeated|Koko kidnapped|Hurry!|Koko met|Event Wait off
	quint8 level;
	quint8 current_hp;
	quint8 max_hp;
	quint16 weapon;// 4 bit = 1 weapon
	quint8 rank;
	quint8 move;
	quint32 saveCount;
	quint16 id_related;
	quint8 u1[6];
	quint8 itemClassACount;
	quint8 itemClassBCount;
	quint8 itemClassCCount;
	quint8 itemClassDCount;
	quint8 u2[16];
	quint32 associatedSaveID;
	quint8 u3;
	quint8 boko_attack;// star count (chocobraise | chocoflammes | chocométéore | grochocobo)
	quint8 u4;
	quint8 home_walking;
	quint8 u5[16];
});

/*
  A chercher :
1. Event WAIT : ON ou OFF
2. le Move  : 1, 2, 3, 4, 5, ou 6
3. Etat MiniMog : Stand by ou Sleep (à mon avis y a aussi un 3e caché pour dire si on montre ou pas ce menu qui dépend de si on a trouvé ou pas MiniMog)
*/

PACK(
struct MAIN//4944 (~4242 used)
{
	GFORCES gfs[16];//			(pos=464|ar_pos_fr=pos + 489104)	[1088/1088 editable] 0x1CFD980 FR PC 1.02?
	PERSONNAGES persos[8];//	(pos=1552)		[1216/1216 editable] 0x1CFDDC0 FR PC 1.02
	SHOP shops[20];//			(pos=2768)		[400/400 editable]
	CONFIG config;//			(pos=3168)		[20/20 editable] 0x1CFE410 FR PC 1.02 ?
	MISC1 misc1;//				(pos=3188)		[32/32 editable]
	LIMITB limitb;//			(pos=3220)		[16/16 editable]
	ITEMS items;//				(pos=3236)		[428/428 editable]
	MISC2 misc2;//				(pos=3664)		[104/144 editable]
	MISC3 misc3;//				(pos=3808)		[138/256 editable] 0x1CFE690 FR PC 1.02
	FIELD field;//				(pos=4064)		[30/1024 editable] (702 unused)
	WORLDMAP worldmap;//		(pos=5088)		[13/128 editable]
	TTCARDS ttcards;//			(pos=5216)		[128/128 editable]
	CHOCOBO chocobo;//			(pos=5344)		[16/64 editable]
});

PACK(
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
	quint8 squall[12];
	quint8 rinoa[12];
	quint8 angelo[12];
	quint8 boko[12];
	quint32 disc;// auto
	quint32 curSave;
});

class SaveData
{
public:
	SaveData();
	SaveData(int id, const QByteArray &data, const QByteArray &MCHeader, bool hasExistsInfos, bool isRaw);
	// Operations
	void open(const QByteArray &data, const QByteArray &MCHeader);
	QByteArray save(bool convertAnalogConfig = false) const;
	void remove();
	void restore();
	// Informations
	bool isModified() const;
	bool wasModified() const;
	void setModified(bool modified);
	int freqValue() const;
	int id() const;
	void setId(int id);
	bool isDelete() const;
	void setIsTheLastEdited(bool isTheLast);
	bool isTheLastEdited() const;
	bool hasExistsInfos() const;
	bool isRaw() const;
	// MC Header
	bool hasMCHeader() const;
	const QByteArray &MCHeader() const;
	char MCHeaderCountry() const;
	bool isJp() const;
	QString MCHeaderCode() const;
	QString MCHeaderId() const;
	QByteArray saveMCHeader();// with xor byte
	static QByteArray emptyMCHeader();
	void setMCHeader(const QByteArray &);
	void setMCHeader(bool exists, char country, const QString &code, const QString &id);
	static quint8 xorByte(const char *data);
	// SC Header
	bool hasSCHeader() const;
	quint8 blockCount() const;
	void setBlockCount(quint8 blockCount);
	QString shortDescription() const;
	void setShortDescription(const QString &desc);
	bool isDescriptionAuto() const;
	void setDescriptionAuto(bool descAuto);
	const SaveIconData &saveIcon() const;
	void setSaveIcon(const SaveIconData &saveIconData);
	// FF8
	bool isFF8() const;
	QString perso(quint8 index) const;
	void setPerso(quint8 index, const QString &name);
	QString gf(quint8 index) const;
	void setGf(quint8 index, const QString &name);
	HEADER &descData();
	const HEADER &constDescData() const;
	MAIN &mainData();
	const MAIN &constMainData() const;
	void updateDescData();
	void setSaveData(const HEADER &descData, const MAIN &data);
	bool isPreviewAuto() const;
	void setPreviewAuto(bool prevAuto);
private:
	bool setData(const QByteArray &data);
	static quint16 calcChecksum(const char *data);
	static const quint16 crcTab[256];

	QByteArray _MCHeader;
	QByteArray _header;
	SaveIconData _icon;
	HEADER _descData;
	MAIN _mainData;
	int _freqValue, _id;
	bool _isFF8, _isDelete, _isTheLastEdited;
	bool _hasExistsInfos, _isRaw;
	bool _isModified, _wasModified;
	bool _descriptionAuto, _previewAuto;
	QByteArray _saveData;
};
