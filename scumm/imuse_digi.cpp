/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001-2003 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 */

#include "stdafx.h"
#include "common/timer.h"
#include "scumm/actor.h"
#include "scumm/bundle.h"
#include "scumm/imuse_digi.h"
#include "scumm/scumm.h"
#include "scumm/sound.h"
#include "sound/mixer.h"
#include "sound/voc.h"

namespace Scumm {

struct imuse_music_table {
	int room;
	int id;
	int unk1;
	int unk2;
	int unk3;
	int unk4;
	char name[30];
	char title[30];
	char filename[13];
};

struct imuse_music_map {
	int room;
	int table_index;
	int unk1;
	int unk2;
	int unk3;
	int unk4;
};

#ifdef __PALM_OS__
// these games are currently not supported under PalmOS so we can save this space
// to prevent full data segement
static const imuse_music_map *_digStateMusicMap;
static const imuse_music_table *_digStateMusicTable;
static const imuse_music_table *_comiStateMusicTable;
static const imuse_music_table *_comiSeqMusicTable;
static const imuse_music_table *_digSeqMusicTable;
#else
static const imuse_music_map _digStateMusicMap[] = {
	{0,		0,	0,	0,	0,	0	},
	{1,		0,	0,	0,	0,	0	},
	{2,		2,	0,	0,	0,	0	},
	{4,		3,	0,	0,	0,	0	},
	{5,		3,	0,	0,	0,	0	},
	{6,		3,	0,	0,	0,	0	},
	{7,		3,	0,	0,	0,	0	},
	{8,		4,	0,	0,	0,	0	},
	{9,		5,	0,	0,	0,	0	},
	{10,	4,	0,	0,	0,	0	},
	{12,	5,	0,	0,	0,	0	},
	{14,	5,	0,	0,	0,	0	},
	{15,	6,	29,	7,	0,	0	},
	{16,	8,	0,	0,	0,	0	},
	{17,	1,	0,	0,	0,	0	},
	{18,	9,	0,	0,	0,	0	},
	{19,	9,	0,	0,	0,	0	},
	{20,	6,	0,	0,	0,	0	},
	{21,	6,	0,	0,	0,	0	},
	{22,	44,	0,	0,	0,	0	},
	{23,	10,	7,	0,	0,	0	},
	{24,	26,	0,	0,	0,	0	},
	{25,	17,	0,	0,	0,	0	},
	{26,	17,	0,	0,	0,	0	},
	{27,	18,	0,	0,	0,	0	},
	{28,	1,	0,	0,	0,	0	},
	{29,	20,	0,	0,	0,	0	},
	{30,	22,	0,	0,	0,	0	},
	{31,	23,	0,	0,	0,	0	},
	{32,	22,	0,	0,	0,	0	},
	{33,	26,	0,	0,	0,	0	},
	{34,	24,	0,	0,	0,	0	},
	{35,	1,	0,	0,	0,	0	},
	{36,	1,	0,	0,	0,	0	},
	{37,	42,	0,	0,	0,	0	},
	{38,	43,	0,	0,	0,	0	},
	{39,	44,	0,	0,	0,	0	},
	{40,	1,	0,	0,	0,	0	},
	{41,	43,	0,	0,	0,	0	},
	{42,	44,	0,	0,	0,	0	},
	{43,	43,	0,	0,	0,	0	},
	{44,	45,	117,45,	114,46},
	{47,	1,	0,	0,	0,	0	},
	{48,	43,	0,	0,	0,	0	},
	{49,	44,	0,	0,	0,	0	},
	{51,	1,	0,	0,	0,	0	},
	{53,	28,	0,	0,	0,	0	},
	{54,	28,	0,	0,	0,	0	},
	{55,	29,	0,	0,	0,	0	},
	{56,	29,	0,	0,	0,	0	},
	{57,	29,	0,	0,	0,	0	},
	{58,	31,	0,	0,	0,	0	},
	{59,	1,	0,	0,	0,	0	},
	{60,	37,	0,	0,	0,	0	},
	{61,	39,	0,	0,	0,	0	},
	{62,	38,	0,	0,	0,	0	},
	{63,	39,	0,	0,	0,	0	},
	{64,	39,	0,	0,	0,	0	},
	{65,	40,	0,	0,	0,	0	},
	{67,	40,	0,	0,	0,	0	},
	{68,	39,	0,	0,	0,	0	},
	{69,	1,	0,	0,	0,	0	},
	{70,	49,	0,	0,	0,	0	},
	{73,	50,	0,	0,	0,	0	},
	{75,	51,	0,	0,	0,	0	},
	{76,	1,	0,	0,	0,	0	},
	{77,	52,	7,	0,	0,	0	},
	{78,	63,	0,	0,	0,	0	},
	{79,	1,	0,	0,	0,	0	},
	{82,	21,	0,	0,	0,	0	},
	{85,	1,	0,	0,	0,	0	},
	{86,	0,	0,	0,	0,	0	},
	{89,	33,	6,	35,	5,	34},
	{90,	16,	0,	0,	0,	0	},
	{91,	57,	0,	0,	0,	0	},
	{88,	32,	0,	0,	0,	0	},
	{92,	25,	0,	0,	0,	0	},
	{93,	0,	0,	0,	0,	0	},
	{95,	19,	0,	0,	0,	0	},
	{80,	41,	0,	0,	0,	0	},
	{81,	48,	0,	0,	0,	0	},
	{83,	27,	0,	0,	0,	0	},
	{94,	36,	0,	0,	0,	0	},
	{40,	1,	0,	0,	0,	0	},
	{96,	13,	0,	0,	0,	0	},
	{97,	14,	0,	0,	0,	0	},
	{98,	11,	0,	0,	0,	0	},
	{99,	15,	0,	0,	0,	0	},
	{100,	17,	0,	0,	0,	0	},
	{101,	38,	0,	0,	0,	0	},
	{103,	0,	0,	0,	0,	0	},
	{104,	0,	0,	0,	0,	0	},
	{11,	44,	0,	0,	0,	0	},
	{3,		47,	0,	0,	0,	0	},
	{105,	30, 128,29,	0,	0	},
	{106,	0,	0,	0,	0,	0	},
	{107,	1,	0,	0,	0,	0	},
	{108,	1,	0,	0,	0,	0	},
	{47,	1,	0,	0,	0,	0	},
	{50,	1,	0,	0,	0,	0	},
	{52,	0,	0,	0,	0,	0	},
	{71,	1,	0,	0,	0,	0	},
	{13,	1,	0,	0,	0,	0	},
	{72,	1,	0,	0,	0,	0	},
	{46,	33,	6,	35,	5,	34},
	{74,	1,	0,	0,	0,	0	},
	{84,	1,	0,	0,	0,	0	},
	{66,	1,	0,	0,	0,	0	},
	{102,	1,	0,	0,	0,	0	},
	{109,	1,	0,	0,	0,	0	},
	{110,	2,	0,	0,	0,	0	},
	{45,	1,	0,	0,	0,	0	},
	{87,	1,	0,	0,	0,	0	},
	{111,	1,	0,	0,	0,	0	},
	{-1,	1,	0,	0,	0,	0	}
};

static const imuse_music_table _digStateMusicTable[] = {
	{0,		1000,	0, 0, 0, 0, "STATE_NULL",						"",										  ""},
	{1,		1001,	0, 0, 0, 0, "stateNoChange",				"",											""},
	{2,		1100,	0, 3, 2, 0, "stateAstShip",					"Asteroid (amb-ship)",	"ASTERO~1.IMU"},
	{3,		1120,	0, 3, 2, 0, "stateAstClose",				"Asteroid (amb-close)",	"ASTERO~2.IMU"},
	{4,		1140,	0, 3, 0, 0, "stateAstInside",				"Asteroid (inside)",		"ASTERO~3.IMU"},
	{5,		1150,	0, 3, 0, 0, "stateAstCore",					"Asteroid (core)",			"ASTERO~4.IMU"},
	{6,		1200,	0, 3, 0, 0, "stateCanyonClose",			"Canyon (close)",				"CANYON~1.IMU"},
	{7,		1205,	0, 3, 0, 0, "stateCanyonClose_m",		"Canyon (close-m)",			"CANYON~2.IMU"},
	{8,		1210,	0, 3, 0, 0, "stateCanyonOver",			"Canyon (over)",				"CANYON~3.IMU"},
	{9,		1220,	0, 3, 0, 0, "stateCanyonWreck",			"Canyon (wreck)",				"CANYON~4.IMU"},
	{10,	1300,	0, 3,10, 0, "stateNexusCanyon",			"Nexus (plan)",					"NEXUS(~1.IMU"},
	{11,	1310,	0, 3,10, 0, "stateNexusPlan",				"Nexus (plan)",					"NEXUS(~1.IMU"},
	{12,	1320,	0, 3,10, 0, "stateNexusRamp",				"Nexus (ramp)",					"NEXUS(~2.IMU"},
	{13,	1330,	0, 3,10, 0, "stateNexusMuseum",			"Nexus (museum)",				"NEXUS(~3.IMU"},
	{14,	1340,	0, 3,10, 0, "stateNexusMap",				"Nexus (map)",					"NEXUS(~4.IMU"},
	{15,	1350,	0, 3,10, 0, "stateNexusTomb",				"Nexus (tomb)",					"NE3706~5.IMU"},
	{16,	1360,	0, 3,10, 0, "stateNexusCath",				"Nexus (cath)",					"NE3305~5.IMU"},
	{17,	1370,	0, 3, 0, 0, "stateNexusAirlock",		"Nexus (airlock)",			"NE2D3A~5.IMU"},
	{18,	1380,	0, 3, 0, 0, "stateNexusPowerOff",		"Nexus (power)",				"NE8522~5.IMU"},
	{19,	1400,	0, 3, 0, 0, "stateMuseumTramNear",	"Tram (mu-near)",				"TRAM(M~1.IMU"},
	{20,	1410,	0, 3, 0, 0, "stateMuseumTramFar",		"Tram (mu-far)",				"TRAM(M~2.IMU"},
	{21,	1420,	0, 3, 0, 0, "stateMuseumLockup",		"Museum (lockup)",			"MUSEUM~1.IMU"},
	{22,	1433,	0, 3,22, 0, "stateMuseumPool",			"Museum (amb-pool)",		"MUSEUM~2.IMU"},
	{23,	1436,	0, 3,22, 0, "stateMuseumSpire",			"Museum (amb-spire)",		"MUSEUM~3.IMU"},
	{24,	1440,	0, 3,22, 0, "stateMuseumMuseum",		"Museum (amb-mu)",			"MUSEUM~4.IMU"},
	{25,	1450,	0, 3, 0, 0, "stateMuseumLibrary",		"Museum (library)",			"MUB575~5.IMU"},
	{26,	1460,	0, 3, 0, 0, "stateMuseumCavern",		"Museum (cavern)",			"MUF9BE~5.IMU"},
	{27,	1500,	0, 3, 0, 0, "stateTombTramNear",		"Tram (tomb-near)",			"TRAM(T~1.IMU"},
	{28,	1510,	0, 3,28, 0, "stateTombBase",				"Tomb (amb-base)",			"TOMB(A~1.IMU"},
	{29,	1520,	0, 3,28, 0, "stateTombSpire",				"Tomb (amb-spire)",			"TOMB(A~2.IMU"},
	{30,	1530,	0, 3,28, 0, "stateTombCave",				"Tomb (amb-cave)",			"TOMB(A~3.IMU"},
	{31,	1540,	0, 3,31, 0, "stateTombCrypt",				"Tomb (crypt)",					"TOMB(C~1.IMU"},
	{32,	1550,	0, 3,31, 0, "stateTombGuards",			"Tomb (crypt-guards)",	"TOMB(C~2.IMU"},
	{33,	1560,	0, 3, 0, 0, "stateTombInner",				"Tomb (inner)",					"TOMB(I~1.IMU"},
	{34,	1570,	0, 3, 0, 0, "stateTombCreator1",		"Tomb (creator 1)",			"TOMB(C~3.IMU"},
	{35,	1580,	0, 3, 0, 0, "stateTombCreator2",		"Tomb (creator 2)",			"TOMB(C~4.IMU"},
	{36,	1600,	0, 3, 0, 0, "statePlanTramNear",		"Tram (plan-near)",			"TRAM(P~1.IMU"},
	{37,	1610,	0, 3, 0, 0, "statePlanTramFar",			"Tram (plan-far)",			"TRAM(P~2.IMU"},
	{38,	1620,	0, 3,38, 0, "statePlanBase",				"Plan (amb-base)",			"PLAN(A~1.IMU"},
	{39,	1630,	0, 3,38, 0, "statePlanSpire",				"Plan (amb-spire)",			"PLAN(A~2.IMU"},
	{40,	1650,	0, 3, 0, 0, "statePlanDome",				"Plan (dome)",					"PLAN(D~1.IMU"},
	{41,	1700,	0, 3, 0, 0, "stateMapTramNear",			"Tram (map-near)",			"TRAM(M~3.IMU"},
	{42,	1710,	0, 3, 0, 0, "stateMapTramFar",			"Tram (map-far)",				"TRAM(M~4.IMU"},
	{43,	1720,	0, 3,43, 0, "stateMapCanyon",				"Map (amb-canyon)",			"MAP(AM~1.IMU"},
	{44,	1730,	0, 3,43, 0, "stateMapExposed",			"Map (amb-exposed)",		"MAP(AM~2.IMU"},
	{45,	1750,	0, 3,43, 0, "stateMapNestEmpty",		"Map (amb-nest)",				"MAP(AM~4.IMU"},
	{46,	1760,	0, 3, 0, 0, "stateMapNestMonster",	"Map (monster)",				"MAP(MO~1.IMU"},
	{47,	1770,	0, 3, 0, 0, "stateMapKlein",				"Map (klein)",					"MAP(KL~1.IMU"},
	{48,	1800,	0, 3, 0, 0, "stateCathTramNear",		"Tram (cath-near)",			"TRAM(C~1.IMU"},
	{49,	1810,	0, 3, 0, 0, "stateCathTramFar",			"Tram (cath-far)",			"TRAM(C~2.IMU"},
	{50,	1820,	0, 3,50, 0, "stateCathLab",					"Cath (amb-inside)",		"CATH(A~1.IMU"},
	{51,	1830,	0, 3,50, 0, "stateCathOutside",			"Cath (amb-outside)",		"CATH(A~2.IMU"},
	{52,	1900,	0, 3,52, 0, "stateWorldMuseum",			"World (museum)",				"WORLD(~1.IMU"},
	{53,	1901,	0, 3,52, 0, "stateWorldPlan",				"World (plan)",					"WORLD(~2.IMU"},
	{54,	1902,	0, 3,52, 0, "stateWorldTomb",				"World (tomb)",					"WORLD(~3.IMU"},
	{55,	1903,	0, 3,52, 0, "stateWorldMap",				"World (map)",					"WORLD(~4.IMU"},
	{56,	1904,	0, 3,52, 0, "stateWorldCath",				"World (cath)",					"WO3227~5.IMU"},
	{57,	1910,	0, 3, 0, 0, "stateEye1",						"Eye 1",								"EYE1~1.IMU"},
	{58,	1911,	0, 3, 0, 0, "stateEye2",						"Eye 2",								"EYE2~1.IMU"},
	{59,	1912,	0, 3, 0, 0, "stateEye3",						"Eye 3",								"EYE3~1.IMU"},
	{60,	1913,	0, 3, 0, 0, "stateEye4",						"Eye 4",								"EYE4~1.IMU"},
	{61,	1914,	0, 3, 0, 0, "stateEye5",						"Eye 5",								"EYE5~1.IMU"},
	{62,	1915,	0, 3, 0, 0, "stateEye6",						"Eye 6",								"EYE6~1.IMU"},
	{63,	1916,	0, 3, 0, 0, "stateEye7",						"Eye 7",								"EYE7~1.IMU"},
	{-1,	-1,		0, 0, 0, 0, "",											"",											""}
};

static const imuse_music_table _digSeqMusicTable[] = {
	{0, 2000,	0, 0, 0, 0,	"SEQ_NULL",							"",												""						},
	{0, 2005,	0, 0, 0, 0, "seqLogo",							"",												""						},
	{0, 2010,	0, 0, 0, 0, "seqIntro",							"",												""						},
	{0, 2020,	0, 6, 0, 0, "seqExplosion1b",				"",												""						},
	{0, 2030,	0, 3, 0, 0, "seqAstTunnel1a",				"Seq (ast tunnel 1a)",		"SEQ(AS~1.IMU"},
	{0, 2031,	0, 6, 0, 0, "seqAstTunnel2b",				"",												""						},
	{0, 2032,	0, 4, 0, 0, "seqAstTunnel3a",				"Seq (ast tunnel 3a)",		"SEQ(AS~2.IMU"},
	{0, 2040,	0, 5, 0, 0, "seqToPlanet1b",				"",												""						},
	{0, 2045,	0, 4, 0, 0, "seqArgBegin",					"Seq (arg begin)",				"SEQ(AR~1.IMU"},
	{0, 2046,	0, 4, 0, 0, "seqArgEnd",						"Seq (arg end)",					"SEQ(AR~2.IMU"},
	{0, 2050,	0, 4, 0, 0, "seqWreckGhost",				"Seq (ghost-wreck)",			"SEQ(GH~1.IMU"},
	{0, 2060,	0, 4, 0, 0, "seqCanyonGhost",				"Seq (ghost-canyon)",			"SEQ(GH~2.IMU"},
	{0, 2070,	0, 0, 0, 0, "seqBrinkFall",					"",												""						},
	{0, 2080,	0, 4, 0, 0, "seqPanUpCanyon",				"Seq (pan up canyon)",		"SEQ(PA~1.IMU"},
	{0, 2091,	0, 6, 0, 0, "seqAirlockTunnel1b",		"",												""						},
	{0, 2100,	0, 6, 0, 0, "seqTramToMu",					"",												""						},
	{0, 2101,	0, 6, 0, 0, "seqTramFromMu",				"",												""						},
	{0, 2102,	0, 6, 0, 0, "seqTramToTomb",				"",												""						},
	{0, 2103,	0, 6, 0, 0, "seqTramFromTomb",			"",												""						},
	{0, 2104,	0, 6, 0, 0, "seqTramToPlan",				"",												""						},
	{0, 2105,	0, 6, 0, 0, "seqTramFromPlan",			"",												""						},
	{0, 2106,	0, 6, 0, 0, "seqTramToMap",					"",												""						},
	{0, 2107,	0, 6, 0, 0, "seqTramFromMap",				"",												""						},
	{0, 2108,	0, 6, 0, 0, "seqTramToCath",				"",												""						},
	{0, 2109,	0, 6, 0, 0, "seqTramFromCath",			"",												""						},
	{0, 2110,	0, 0, 0, 0, "seqMuseumGhost",				"",												""						},
	{0, 2120,	0, 0, 0, 0, "seqSerpentAppears",		"",												""						},
	{0, 2130,	0, 0, 0, 0, "seqSerpentEats",				"",												""						},
	{0, 2140,	0, 6, 0, 0, "seqBrinkRes1b",				"",												""						},
	{0, 2141,	0, 4, 0, 0, "seqBrinkRes2a",				"Seq (brink's madness)",  "SEQ(BR~1.IMU"},
	{0, 2150,	0, 3, 0, 0, "seqLockupEntry",				"Seq (brink's madness)",  "SEQ(BR~1.IMU"},
	{0, 2160,	0, 0, 0, 0, "seqSerpentExplodes",		"",												""						},
	{0, 2170,	0, 4, 0, 0, "seqSwimUnderwater",		"Seq (descent)",					"SEQ(DE~1.IMU"},
	{0, 2175,	0, 4, 0, 0, "seqWavesPlunge",				"Seq (plunge)",						"SEQ(PL~1.IMU"},
	{0, 2180,	0, 0, 0, 0, "seqCryptOpens",				"",												""						},
	{0, 2190,	0, 0, 0, 0, "seqGuardsFight",				"",												""						},
	{0, 2200,	0, 3, 0, 0, "seqCreatorRes1.1a",		"Seq (creator res 1.1a)", "SEQ(CR~1.IMU"},
	{0, 2201,	0, 6, 0, 0, "seqCreatorRes1.2b",		"",												""						},
	{0, 2210,	0, 6, 0, 0, "seqMaggieCapture1b",		"",												""						},
	{0, 2220,	0, 3, 0, 0, "seqStealCrystals",			"Seq (brink's madness)",  "SEQ(BR~1.IMU"},
	{0, 2230,	0, 0, 0, 0, "seqGetByMonster",			"",												""						},
	{0, 2240,	0, 6, 0, 0, "seqKillMonster1b",			"",												""						},
	{0, 2250,	0, 3, 0, 0, "seqCreatorRes2.1a",		"Seq (creator res 2.1a)", "SEQ(CR~2.IMU"},
	{0, 2251,	0, 6, 0, 0, "seqCreatorRes2.2b",		"",												""						},
	{0, 2252,	0, 4, 0, 0, "seqCreatorRes2.3a",		"Seq (creator res 2.3a)", "SEQ(CR~3.IMU"},
	{0, 2260,	0, 0, 0, 0, "seqMaggieInsists",			"",												""						},
	{0, 2270,	0, 0, 0, 0, "seqBrinkHelpCall",			"",												""						},
	{0, 2280,	0, 3, 0, 0, "seqBrinkCrevice1a",		"Seq (brink crevice 1a)", "SEQ(BR~2.IMU"},
	{0, 2281,	0, 3, 0, 0, "seqBrinkCrevice2a",		"Seq (brink crevice 2a)", "SEQ(BR~3.IMU"},
	{0, 2290,	0, 6, 0, 0, "seqCathAccess1b",			"",												""						},
	{0, 2291,	0, 4, 0, 0, "seqCathAccess2a",			"Seq (cath access 2a)",		"SEQ(CA~1.IMU"},
	{0, 2300,	0, 3, 0, 0, "seqBrinkAtGenerator",  "Seq (brink's madness)",  "SEQ(BR~1.IMU"},
	{0, 2320,	0, 6, 0, 0, "seqFightBrink1b",			"",												""						},
	{0, 2340,	0, 6, 0, 0, "seqMaggieDies1b",			"",												""						},
	{0, 2346,	0, 6, 0, 0, "seqMaggieRes1b",				"",												""						},
	{0, 2347,	0, 4, 0, 0, "seqMaggieRes2a",				"Seq (maggie res 2a)",		"SEQ(MA~1.IMU"},
	{0, 2350,	0, 0, 0, 0, "seqCreatureFalls",			"",												""						},
	{0, 2360,	0, 5, 0, 0, "seqFinale1b",					"",												""						},
	{0, 2370,	0, 3, 0, 0, "seqFinale2a",					"Seq (finale 2a)",				"SEQ(FI~1.IMU"},
	{0, 2380,	0, 6, 0, 0, "seqFinale3b1",					"",												""						},
	{0, 2390,	0, 6, 0, 0, "seqFinale3b2",					"",												""						},
	{0, 2400,	0, 3, 0, 0, "seqFinale4a",					"Seq (finale 4a)",				"SEQ(FI~2.IMU"},
	{0, 2410,	0, 3, 0, 0, "seqFinale5a",					"Seq (finale 5a)",				"SEQ(FI~3.IMU"},
	{0, 2420,	0, 3, 0, 0, "seqFinale6a",					"Seq (finale 6a)",				"SEQ(FI~4.IMU"},
	{0, 2430,	0, 3, 0, 0, "seqFinale7a",					"Seq (finale 7a)",				"SE3D2B~5.IMU"},
	{0, 2440,	0, 6, 0, 0, "seqFinale8b",					"",												""						},
	{0, 2450,	0, 4, 0, 0, "seqFinale9a",					"Seq (finale 9a)",				"SE313B~5.IMU"},
	{-1,  -1,	0, 0, 0, 0,	"",											"",												""						}
};

static const imuse_music_table _comiStateMusicTable[] = {
	{0,	1000,	0, 0, 0, 0,  "STATE_NULL",				 "",											""},
	{0,	1001,	0, 0, 0, 0,  "stateNoChange",			 "",											""},
	{0,	1098,	0, 3, 0, 0,  "stateCredits1",			 "1098-Credits1",					"1098-C~1.IMX"},
	{0,	1099,	0, 3, 0, 0,  "stateMenu",					 "1099-Menu",							"1099-M~1.IMX"},
	{0,	1100,	0, 3, 0, 0,  "stateHold1",				 "1100-Hold1",						"1100-H~1.IMX"},
	{0,	1101,	0, 3, 1, 0,  "stateWaterline1",		 "1101-Waterline1",				"1101-W~1.IMX"},
	{0, 1102,	0, 3, 0, 0,  "stateHold2",				 "1102-Hold2",						"1102-H~1.IMX"},
	{0, 1103,	0, 3, 0, 0,  "stateWaterline2",		 "1103-Waterline2",				"1103-W~1.IMX"},
	{0, 1104,	0, 3, 0, 0,  "stateCannon",				 "1104-Cannon",						"1104-C~1.IMX"},
	{0, 1105,	0, 3, 0, 0,  "stateTreasure",			 "1105-Treasure",					"1105-T~1.IMX"},
	{0, 1200,	0, 3, 1, 0,  "stateFortBase",			 "1200-Fort Base",				"1200-F~1.IMX"},
	{0, 1201,	0, 3, 1, 0,  "statePreFort",			 "1201-Pre-Fort",					"1201-P~1.IMX"},
	{0, 1202,	0, 3, 0, 0,  "statePreVooOut",		 "1202-PreVoo Out",				"1202-P~1.IMX"},
	{0, 1203,	0, 3, 0, 0,  "statePreVooIn",			 "1203-PreVoo In",				"1203-P~1.IMX"},
	{0, 1204,	0, 3, 0, 0,  "statePreVooLad",		 "1204-PreVoo Lady",			"1204-P~1.IMX"},
	{0, 1205,	0, 3, 0, 0,  "stateVoodooOut",		 "1205-Voodoo Out",				"1205-V~1.IMX"},
	{0, 1210,	0, 3, 0, 0,  "stateVoodooIn",			 "1210-Voodoo In",				"1210-V~1.IMX"},
	{0, 1212,	0,12, 1, 0,  "stateVoodooInAlt",	 "1210-Voodoo In",				"1210-V~1.IMX"},
	{0, 1215,	0, 3, 0, 0,  "stateVoodooLady",		 "1215-Voodoo Lady",			"1215-V~1.IMX"},
	{0, 1219,	0, 3, 0, 0,  "statePrePlundermap", "1219-Pre-Map",					"1219-P~1.IMX"},
	{0, 1220,	0, 3, 0, 0,  "statePlundermap",		 "1220-Plunder Map",			"1220-P~1.IMX"},
	{0, 1222,	0, 3, 0, 0,  "statePreCabana",		 "1222-Pre-Cabana",				"1222-P~1.IMX"},
	{0, 1223,	0, 3, 0, 0,  "stateCabana",				 "1223-Cabana",						"1223-C~1.IMX"},
	{0, 1224,	0, 3, 0, 0,  "statePostCabana",		 "1224-Post-Cabana",			"1224-P~1.IMX"},
	{0, 1225,	0, 3, 0, 0,  "stateBeachClub",		 "1225-Beach Club",				"1225-B~1.IMX"},
	{0, 1230,	0, 3, 0, 0,  "stateCliff",				 "1230-Cliff",						"1230-C~1.IMX"},
	{0, 1232,	0, 3, 0,800, "stateBelly",				 "1232-Belly",						"1232-B~1.IMX"},
	{0, 1235,	0, 3, 0, 0,  "stateQuicksand",		 "1235-Quicksand",				"1235-Q~1.IMX"},
	{0, 1240,	0, 3, 0,800, "stateDangerBeach",   "1240-Danger Beach",			"1240-D~1.IMX"},
	{0, 1241,	0,12, 2,800, "stateDangerBeachAlt","1240-Danger Beach",			"1240-D~1.IMX"},
	{0, 1245,	0, 3, 0, 0,  "stateRowBoat",			 "1245-Row Boat",					"1245-R~1.IMX"},
	{0, 1247,	0, 3, 0,800, "stateAlongside",		 "1247-Alongside",				"1247-A~1.IMX"},
	{0, 1248,	0,12, 1,800, "stateAlongsideAlt",	 "1247-Alongside",				"1247-A~1.IMX"},
	{0, 1250,	0, 3, 0,500, "stateChimpBoat",		 "1250-Chimp Boat",				"1250-C~1.IMX"},
	{0, 1255,	0, 3, 0,800, "stateMrFossey",			 "1255-Mr Fossey",				"1255-M~1.IMX"},
	{0, 1259,	0, 3, 0, 0,  "statePreTown",			 "1259-Pre-Town",					"1259-P~1.IMX"},
	{0, 1260,	0, 3, 0, 0,  "stateTown",					 "1260-Town",							"1260-T~1.IMX"},
	{0, 1264,	0, 3, 0, 0,  "statePreMeadow",		 "1264-Pre-Meadow",				"1264-P~1.IMX"},
	{0, 1265,	0, 3, 0, 0,  "stateMeadow",				 "1265-Meadow",						"1265-M~1.IMX"},
	{0, 1266,	0, 3, 0, 0,  "stateMeadowAmb",		 "1266-Meadow Amb",				"1266-M~1.IMX"},
	{0, 1270,	0, 3, 0, 0,  "stateWardrobePre",	 "1270-Wardrobe-Pre",			"1270-W~1.IMX"},
	{0, 1272,	0, 3, 0, 0,  "statePreShow",			 "1272-Pre-Show",					"1272-P~1.IMX"},
	{0, 1274,	0, 3, 0, 0,  "stateWardrobeShow",	 "1274-Wardrobe-Show",		"1274-W~1.IMX"},
	{0, 1276,	0, 3, 0, 0,  "stateShow",					 "1276-Show",							"1276-S~1.IMX"},
	{0, 1277,	0, 3, 0, 0,  "stateWardrobeJug",	 "1277-Wardrobe-Jug",			"1277-W~1.IMX"},
	{0, 1278,	0, 3, 0, 0,  "stateJuggling",			 "1278-Juggling",					"1278-J~1.IMX"},
	{0, 1279,	0, 3, 0, 0,  "statePostShow",			 "1279-Post-Show",				"1279-P~1.IMX"},
	{0, 1280,	0, 3, 0, 0,  "stateChickenShop",	 "1280-Chicken Shop",			"1280-C~1.IMX"},
	{0, 1285,	0, 3, 0, 0,  "stateBarberShop",		 "1285-Barber Shop",			"1285-B~1.IMX"},
	{0, 1286,	0, 3, 0, 0,  "stateVanHelgen",		 "1286-Van Helgen",				"1286-V~1.IMX"},
	{0, 1287,	0, 3, 0, 0,  "stateBill",					 "1287-Bill",							"1287-B~1.IMX"},
	{0, 1288,	0, 3, 0, 0,  "stateHaggis",				 "1288-Haggis",						"1288-H~1.IMX"},
	{0, 1289,	0, 3, 0, 0,  "stateRottingham",		 "1289-Rottingham",				"1289-R~1.IMX"},
	{0, 1305,	0, 3, 0, 0,  "stateDeck",					 "1305-Deck",							"1305-D~1.IMX"},
	{0, 1310,	0, 3, 0, 0,  "stateCombatMap",		 "1310-Combat Map",				"1310-C~1.IMX"},
	{0, 1320,	0, 3, 0, 0,  "stateShipCombat",		 "1320-Ship Combat",			"1320-S~1.IMX"},
	{0, 1325,	0, 3, 0, 0,  "stateSwordfight",		 "1325-Swordfight",				"1325-S~1.IMX"},
	{0, 1327,	0, 3, 0, 0,  "stateSwordRott",		 "1327-Sword Rott",				"1327-S~1.IMX"},
	{0, 1330,	0, 3, 0, 0,  "stateTownEdge",			 "1330-Town Edge",				"1330-T~1.IMX"},
	{0, 1335,	0, 3, 0, 0,  "stateSwordLose",		 "1335-Sword Lose",				"1335-S~1.IMX"},
	{0, 1340,	0, 3, 0, 0,  "stateSwordWin",			 "1340-Sword Win",				"1340-S~1.IMX"},
	{0, 1345,	0, 3, 0, 0,  "stateGetMap",				 "1345-Get Map",					"1345-G~1.IMX"},
	{0, 1400,	0, 3, 0, 0,  "stateWreckBeach",		 "1400-Wreck Beach",			"1400-W~1.IMX"},
	{0, 1405,	0, 3, 0, 0,  "stateBloodMap",			 "1405-Blood Map",				"1405-B~1.IMX"},
	{0, 1410,	0, 3, 0, 0,  "stateClearing",			 "1410-Clearing",					"1410-C~1.IMX"},
	{0, 1415,	0, 3, 0, 0,  "stateLighthouse",		 "1415-Lighthouse",				"1415-L~1.IMX"},
	{0, 1420,	0, 3, 0, 0,  "stateVillage",			 "1420-Village",					"1420-V~1.IMX"},
	{0, 1423,	0, 3, 0, 0,  "stateVolcano",			 "1423-Volcano",					"1423-V~1.IMX"},
	{0, 1425,	0, 3, 0, 0,  "stateAltar",				 "1425-Altar",						"1425-A~1.IMX"},
	{0, 1430,	0, 3, 0, 0,  "stateHotelOut",			 "1430-Hotel Out",				"1430-H~1.IMX"},
	{0, 1435,	0, 3, 0, 0,  "stateHotelBar",			 "1435-Hotel Bar",				"1435-H~1.IMX"},
	{0, 1440,	0, 3, 0, 0,  "stateHotelIn",			 "1440-Hotel In",					"1440-H~1.IMX"},
	{0, 1445,	0, 3, 0, 0,  "stateTarotLady",		 "1445-Tarot Lady",				"1445-T~1.IMX"},
	{0, 1447,	0, 3, 0, 0,  "stateGoodsoup",			 "1447-Goodsoup",					"1447-G~1.IMX"},
	{0, 1448,	0, 3, 0, 0,  "stateGuestRoom",		 "1448-Guest Room",				"1448-G~1.IMX"},
	{0, 1450,	0, 3, 0, 0,  "stateWindmill",			 "1450-Windmill",					"1450-W~1.IMX"},
	{0, 1455,	0, 3, 0, 0,  "stateCemetary",			 "1455-Cemetary",					"1455-C~1.IMX"},
	{0, 1460,	0, 3, 0, 0,  "stateCrypt",				 "1460-Crypt",						"1460-C~1.IMX"},
	{0, 1463,	0, 3, 0, 0,  "stateGraveDigger",	 "1463-Gravedigger",			"1463-G~1.IMX"},
	{0, 1465,	0, 3, 0, 0,  "stateMonkey1",			 "1465-Monkey1",					"1465-M~1.IMX"},
	{0, 1475,	0, 3, 0, 0,  "stateStanDark",			 "1475-Stan Dark",				"1475-S~1.IMX"},
	{0, 1477,	0, 3, 0, 0,  "stateStanLight",		 "1477-Stan",							"1477-S~1.IMX"},
	{0, 1480,	0, 3, 0, 0,  "stateEggBeach",			 "1480-Egg Beach",				"1480-E~1.IMX"},
	{0, 1485,	0, 3, 0, 0,  "stateSkullIsland",	 "1485-Skull Island",			"1485-S~1.IMX"},
	{0, 1490,	0, 3, 0, 0,  "stateSmugglersCave", "1490-Smugglers",				"1490-S~1.IMX"},
	{0, 1500,	0, 3, 0, 0,  "stateLeChuckTalk",	 "1500-Le Talk",					"1500-L~1.IMX"},
	{0, 1505,	0, 3, 0, 0,  "stateCarnival",			 "1505-Carnival",					"1505-C~1.IMX"},
	{0, 1511,	0, 3, 0, 0,  "stateHang",					 "1511-Hang",							"1511-H~1.IMX"},
	{0, 1512,	0, 3, 0, 0,  "stateRum",					 "1512-Rum",							"1512-RUM.IMX"},
	{0, 1513,	0, 3, 0, 0,  "stateTorture",			 "1513-Torture",					"1513-T~1.IMX"},
	{0, 1514,	0, 3, 0, 0,  "stateSnow",					 "1514-Snow",							"1514-S~1.IMX"},
	{0, 1515,	0, 3, 0, 0,  "stateCredits",			 "1515-Credits (end)",		"1515-C~1.IMX"},
	{0, 1520,	0, 3, 0, 0,  "stateCarnAmb",			 "1520-Carn Amb",					"1520-C~1.IMX"},
	{-1,  -1,	0, 0, 0, 0,  "",									 "",											""}
};

static const imuse_music_table _comiSeqMusicTable[] = {
	{0, 2000,	0, 0, 0, 0,  "SEQ_NULL",					"",												""},
	{0, 2100,	0, 0, 0, 0,  "seqINTRO",					"",												""},
	{0, 2105,	0, 3, 0, 0,  "seqInterlude1",			"2105-Interlude1",				"2105-I~1.IMX"},
	{0, 2110,	0, 8, 1, 0,  "seqLastBoat",				"",												""},
	{0, 2115,	0, 0, 0, 0,  "seqSINK_SHIP",			"",												""},
	{0, 2120,	0, 0, 0, 0,  "seqCURSED_RING",		"",												""},
	{0, 2200,	0, 3, 0, 0,  "seqInterlude2",			"2200-Interlude2",				"2200-I~1.IMX"},
	{0, 2210,	0, 3, 0, 0,  "seqKidnapped",			"2210-Kidnapped",					"2210-K~1.IMX"},
	{0, 2220,	0, 8, 1, 0,  "seqSnakeVomits",		"",												""},
	{0, 2222,	0, 8, 1, 0,  "seqPopBalloon",			"",												""},
	{0, 2225,	0, 3, 0, 0,  "seqDropBalls",			"2225-Drop Balls",				"2225-D~1.IMX"},
	{0, 2232,	0, 4, 0, 0,  "seqArriveBarber",		"2232-Arrive Barber",			"2232-A~1.IMX"},
	{0, 2233,	0, 3, 0, 0,  "seqAtonal",					"2233-Atonal",						"2233-A~1.IMX"},
	{0, 2235,	0, 3, 0, 0,  "seqShaveHead1",			"2235-Shave Head",				"2235-S~1.IMX"},
	{0, 2236,	0, 2, 2, 0,  "seqShaveHead2",			"2235-Shave Head",				"2235-S~1.IMX"},
	{0, 2245,	0, 3, 0, 0,  "seqCaberLose",			"2245-Caber Lose",				"2245-C~1.IMX"},
	{0, 2250,	0, 3, 0, 0,  "seqCaberWin",				"2250-Caber Win",					"2250-C~1.IMX"},
	{0, 2255,	0, 3, 0, 0,  "seqDuel1",					"2255-Duel",							"2255-D~1.IMX"},
	{0, 2256,	0, 2, 2, 0,  "seqDuel2",					"2255-Duel",							"2255-D~1.IMX"},
	{0, 2257,	0, 2, 3, 0,  "seqDuel3",					"2255-Duel",							"2255-D~1.IMX"},
	{0, 2260,	0, 3, 0, 0,  "seqBlowUpTree1",		"2260-Blow Up Tree",			"2260-B~1.IMX"},
	{0, 2261,	0, 2, 2, 0,  "seqBlowUpTree2",		"2260-Blow Up Tree",			"2260-B~1.IMX"},
	{0, 2275,	0, 3, 0, 0,  "seqMonkeys",				"2275-Monkeys",						"2275-M~1.IMX"},
	{0, 2277,	0, 9, 1, 0,  "seqAttack",					"",												""},
	{0, 2285,	0, 3, 0, 0,  "seqSharks",					"2285-Sharks",						"2285-S~1.IMX"},
	{0, 2287,	0, 3, 0, 0,  "seqTowelWalk",			"2287-Towel Walk",				"2287-T~1.IMX"},
	{0, 2293,	0, 0, 0, 0,  "seqNICE_BOOTS",			"",												""},
	{0, 2295,	0, 0, 0, 0,  "seqBIG_BONED",			"",												""},
	{0, 2300,	0, 3, 0, 0,  "seqToBlood",				"2300-To Blood",					"2300-T~1.IMX"},
	{0, 2301,	0, 3, 0, 0,  "seqInterlude3",			"2301-Interlude3",				"2301-I~1.IMX"},
	{0, 2302,	0, 3, 0, 0,  "seqRott1",					"2302-Rott Attack",				"2302-R~1.IMX"},
	{0, 2304,	0, 2, 2, 0,  "seqRott2",					"2302-Rott Attack",				"2302-R~1.IMX"},
	{0, 2305,	0, 2,21, 0,  "seqRott2b",					"2302-Rott Attack",				"2302-R~1.IMX"},
	{0, 2306,	0, 2, 3, 0,  "seqRott3",					"2302-Rott Attack",				"2302-R~1.IMX"},
	{0, 2308,	0, 2, 4, 0,  "seqRott4",					"2302-Rott Attack",				"2302-R~1.IMX"},
	{0, 2309,	0, 2, 5, 0,  "seqRott5",					"2302-Rott Attack",				"2302-R~1.IMX"},
	{0, 2311,	0, 3, 0, 0,  "seqVerse1",					"2311-Song",							"2311-S~1.IMX"},
	{0, 2312,	0, 2, 2, 0,  "seqVerse2",					"2311-Song",							"2311-S~1.IMX"},
	{0, 2313,	0, 2, 3, 0,  "seqVerse3",					"2311-Song",							"2311-S~1.IMX"},
	{0, 2314,	0, 2, 4, 0,  "seqVerse4",					"2311-Song",							"2311-S~1.IMX"},
	{0, 2315,	0, 2, 5, 0,  "seqVerse5",					"2311-Song",							"2311-S~1.IMX"},
	{0, 2316,	0, 2, 6, 0,  "seqVerse6",					"2311-Song",							"2311-S~1.IMX"},
	{0, 2317,	0, 2, 7, 0,  "seqVerse7",					"2311-Song",							"2311-S~1.IMX"},
	{0, 2318,	0, 2, 8, 0,  "seqVerse8",					"2311-Song",							"2311-S~1.IMX"},
	{0, 2319,	0, 2, 9, 0,  "seqSongEnd",				"2311-Song",							"2311-S~1.IMX"},
	{0, 2336,	0, 2, 0, 0,  "seqRiposteLose",		"2336-Riposte Lose",			"2336-R~1.IMX"},
	{0, 2337,	0, 2, 0, 0,  "seqRiposteWin",			"2337-Riposte Win",				"2337-R~1.IMX"},
	{0, 2338,	0, 2, 0, 0,  "seqInsultLose",			"2338-Insult Lose",				"2338-I~1.IMX"},
	{0, 2339,	0, 2, 0, 0,  "seqInsultWin",			"2339-Insult Win",				"2339-I~1.IMX"},
	{0, 2340,	0, 3, 0, 0,  "seqSwordLose",			"1335-Sword Lose",				"1335-S~1.IMX"},
	{0, 2345,	0, 3, 0, 0,  "seqSwordWin",				"1340-Sword Win",					"1340-S~1.IMX"},
	{0, 2347,	0, 3, 0, 0,  "seqGetMap",					"1345-Get Map",						"1345-G~1.IMX"},
	{0, 2400,	0, 3, 0, 0,  "seqInterlude4",			"2400-Interlude4",				"2400-I~1.IMX"},
	{0, 2405,	0, 0, 0, 0,  "seqSHIPWRECK",			"",												""},
	{0, 2408,	0, 3, 0, 0,  "seqFakeCredits",		"2408-Fake Credits",			"2408-F~1.IMX"},
	{0, 2410,	0, 3, 0, 0,  "seqPassOut",				"2410-Pass Out",					"2410-P~1.IMX"},
	{0, 2414,	0, 3, 0, 0,  "seqGhostTalk",			"2414-Ghost Talk",				"2414-G~1.IMX"},
	{0, 2415,	0, 2, 1, 0,  "seqGhostWedding",		"2414-Ghost Talk",				"2414-G~1.IMX"},
	{0, 2420,	0, 3, 0, 0,  "seqEruption",				"2420-Eruption",					"2420-E~1.IMX"},
	{0, 2425,	0, 3, 0, 0,  "seqSacrifice",			"2425-Sacrifice",					"2425-S~1.IMX"},
	{0, 2426,	0, 2, 1, 0,  "seqSacrificeEnd",		"2425-Sacrifice",					"2425-S~1.IMX"},
	{0, 2430,	0, 3, 0, 0,  "seqScareDigger",		"2430-Scare Digger",			"2430-S~1.IMX"},
	{0, 2445,	0, 3, 0, 0,  "seqSkullArrive",		"2445-Skull Arrive",			"2445-S~1.IMX"},
	{0, 2450,	0, 3, 0, 0,  "seqFloat",					"2450-Cliff Fall",				"2450-C~1.IMX"},
	{0, 2451,	0, 2, 1, 0,  "seqFall",						"2450-Cliff Fall",				"2450-C~1.IMX"},
	{0, 2452,	0, 2, 2, 0,  "seqUmbrella",				"2450-Cliff Fall",				"2450-C~1.IMX"},
	{0, 2460,	0, 3, 0, 0,  "seqFight",					"2460-Fight",							"2460-F~1.IMX"},
	{0, 2465,	0, 0, 0, 0,  "seqLAVE_RIDE",			"",												""},
	{0, 2470,	0, 0, 0, 0,  "seqMORE_SLAW",			"",												""},
	{0, 2475,	0, 0, 0, 0,  "seqLIFT_CURSE",			"",												""},
	{0, 2500,	0, 3, 0, 0,  "seqInterlude5",			"2500-Interlude5",				"2500-I~1.IMX"},
	{0, 2502,	0, 3, 0, 0,  "seqExitSkycar",			"2502-Exit Skycar",				"2502-E~1.IMX"},
	{0, 2504,	0, 3, 0, 0,  "seqGrow1",					"2504-Grow",							"2504-G~1.IMX"},
	{0, 2505,	0, 2, 1, 0,  "seqGrow2",					"2504-Grow",							"2504-G~1.IMX"},
	{0, 2508,	0, 3, 0, 0,  "seqInterlude6",			"2508-Interlude6",				"2508-I~1.IMX"},
	{0, 2515,	0, 0, 0, 0,  "seqFINALE",					"",												""},
	{0, 2520,	0, 3, 0, 0,  "seqOut",						"2520-Out",								"2520-OUT.IMX"},
	{0, 2530,	0, 3, 0, 0,  "seqZap1a",					"2530-Zap1",							"2530-Z~1.IMX"},
	{0, 2531,	0, 2, 1, 0,  "seqZap1b",					"2530-Zap1",							"2530-Z~1.IMX"},
	{0, 2532,	0, 2, 2, 0,  "seqZap1c",					"2530-Zap1",							"2530-Z~1.IMX"},
	{0, 2540,	0, 3, 0, 0,  "seqZap2a",					"2540-Zap2",							"2540-Z~1.IMX"},
	{0, 2541,	0, 2, 1, 0,  "seqZap2b",					"2540-Zap2",							"2540-Z~1.IMX"},
	{0, 2542,	0, 2, 2, 0,  "seqZap2c",					"2540-Zap2",							"2540-Z~1.IMX"},
	{0, 2550,	0, 3, 0, 0,  "seqZap3a",					"2550-Zap3",							"2550-Z~1.IMX"},
	{0, 2551,	0, 2, 1, 0,  "seqZap3b",					"2550-Zap3",							"2550-Z~1.IMX"},
	{0, 2552,	0, 2, 2, 0,  "seqZap3c",					"2550-Zap3",							"2550-Z~1.IMX"},
	{0, 2560,	0, 3, 0, 0,  "seqZap4a",					"2560-Zap4",							"2560-Z~1.IMX"},
	{0, 2561,	0, 2, 1, 0,  "seqZap4b",					"2560-Zap4",							"2560-Z~1.IMX"},
	{0, 2562,	0, 2, 2, 0,  "seqZap4c",					"2560-Zap4",							"2560-Z~1.IMX"},
	{-1, -1,  0, 0, 0, 0,  "",									"",												""}
};

#endif

struct imuse_ft_music_table {
	int index;
	char audioname[15];
	int unk1;
	int volume;
	char name[30];
};

#ifdef __PALM_OS__
// these games are currently not supported under PalmOS so we can save this space
// to prevent full data segement
static const imuse_ft_music_table *_ftStateMusicTable;
static const imuse_ft_music_table *_ftSeqMusicTable;
#else
static const imuse_ft_music_table _ftStateMusicTable[] = {
	{0,		"",					0,	0,		"STATE_NULL"					},
	{1,		"",					4,	127,	"stateKstandOutside"	},
	{2,		"kinside",	2,	127,	"stateKstandInside"		},
	{3,		"moshop",		3,	64,		"stateMoesInside"			},
	{4,		"melcut",		2,	127,	"stateMoesOutside"		},
	{5,		"mellover",	2,	127,	"stateMellonAbove"		},
	{6,		"radloop",	3,	28,		"stateTrailerOutside"	},
	{7,		"radloop",	3,	58,		"stateTrailerInside"	},
	{8,		"radloop",	3,	127,	"stateTodShop"				},
	{9,		"junkgate",	2,	127,	"stateJunkGate"				},
	{10,	"junkover",	3,	127,	"stateJunkAbove"			},
	{11,	"gastower",	2,	127,	"stateGasTower"				},
	{12,	"",					4,	0,		"stateTowerAlarm"			},
	{13,	"melcut",		2,	127,	"stateCopsOnGround"		},
	{14,	"melcut",		2,	127,	"stateCopsAround"			},
	{15,	"melcut",		2,	127,	"stateMoesRuins"			},
	{16,	"melcut",		2,	127,	"stateKstandNight"		},
	{17,	"trukblu2",	2,	127,	"stateTruckerTalk"		},
	{18,	"stretch",	2,	127,	"stateMumblyPeg"			},
	{19,	"kstand",		2,	100, 	"stateRanchOutside"		},
	{20,	"kinside",	2,	127,	"stateRanchInside"		},
	{21,	"desert",		2,	127,	"stateWreckedTruck"		},
	{22,	"opening",	2,	100, 	"stateGorgeVista"			},
	{23,	"caveopen",	2,	127,	"stateCaveOpen"				},
	{24,	"cavecut1",	2,	127,	"stateCaveOuter"			},
	{25,	"cavecut1",	1,	127,	"stateCaveMiddle"			},
	{26,	"cave",			2,	127,	"stateCaveInner"			},
	{27,	"corville",	2,	127,	"stateCorvilleFront"	},
	{28,	"mines",		2,	127,	"stateMineField"			},
	{29,	"bunyman3",	2,	127,	"stateBunnyStore"			},
	{30,	"stretch",	2,	127,	"stateStretchBen"			},
	{31,	"saveme",		2,	127,	"stateBenPleas"				},
	{32,	"",					4,	0,		"stateBenConvinces"		},
	{33,	"derby",		3,	127,	"stateDemoDerby"			},
	{34,	"fire",			3,	127,	"stateLightMyFire"		},
	{35,	"derby",		3,	127,	"stateDerbyChase"			},
	{36,	"carparts",	2,	127,	"stateVultureCarParts"},
	{37,	"cavecut1",	2,	127,	"stateVulturesInside"	},
	{38,	"mines",		2,	127,	"stateFactoryRear"		},
	{39,	"croffice",	2,	127,	"stateCorleyOffice"		},
	{40,	"melcut",		2,	127,	"stateCorleyHall"			},
	{41,	"",					4,	0,		"stateProjRoom"				},
	{42,	"",					4,	0,		"stateMMRoom"					},
	{43,	"bumper",		2,	127,	"stateBenOnBumper"		},
	{44,	"benump",		2,	127,	"stateBenOnBack"			},
	{45,	"plane",		2,	127,	"stateInCargoPlane"		},
	{46,	"saveme",		2,	127,	"statePlaneControls"	},
	{47,	"",					4,	0,		"stateCliffHanger1"		},
	{48,	"",					4,	0,		"stateCliffHanger2"		},
	{-1,	"",					0,	0,		""										}
};

static const imuse_ft_music_table _ftSeqMusicTable[] = {
	{0,		"",					2,	127,  "SEQ_NULL"						},
	{1,		"opening",	2,	127,	"seqLogo"							},
	{2,		"barbeat",	2,	127,	"seqOpenFlick"				},
	{3,		"barwarn",	2,	127,	"seqBartender"				},
	{4,		"benwakes", 2,	127,  "seqBenWakes"					},
	{5,		"barwarn",	2,	127,	"seqPhotoScram"				},
	{6,		"swatben",	2,	127,	"seqClimbChain"				},
	{7,		"dogattak",	2,	127,	"seqDogChase"					},
	{8,		"",					0,	0,   	"seqDogSquish"				},
	{9,		"",					0,	0,   	"seqDogHoist"					},
	{10,	"cops2",		2,	127,	"seqCopsArrive"				},
	{11,	"cops2",		2,	127,	"seqCopsLand"					},
	{12,	"cops2",		2,	127,	"seqCopsLeave"				},
	{13,	"",					0,	0,		"seqCopterFlyby"			},
	{14,	"bunymrch",	2,	127,	"seqCopterCrash"			},
	{15,	"",					0,	0,		"seqMoGetsParts"			},
	{16,	"",					0,	0,		"seqMoFixesBike"			},
	{17,	"",					0,	0,		"seqFirstGoodbye"			},
	{18,	"trucker",	2,	127,	"seqCopRoadblock"			},
	{19,	"cops2",		2,	127,	"seqDivertCops"				},
	{20,	"barwarn",	2,	127,	"seqMurder"						},
	{21,	"corldie",	2,	127,	"seqCorleyDies"				},
	{22,	"barwarn",	2,	127,	"seqTooLateAtMoes"		},
	{23,	"picture",	2,	127,	"seqPicture"					},
	{24,	"ripintro",	2,	127,	"seqNewsReel"					},
	{25,	"trucker",	2,	127,	"seqCopsInspect"			},
	{26,	"ripdead",	2,	127,	"seqHijack"						},
	{27,	"nesranch",	2,	127,	"seqNestolusAtRanch"	},
	{28,	"scolding", 2,	127,	"seqRipLimo"					},
	{29,	"desert",		2,	127,	"seqGorgeTurn"				},
	{30,	"cavecut1",	2,	127,	"seqStealRamp"				},
	{31,	"vaceamb",	2,	80,		"seqCavefishTalk"			},
	{32,	"castle",		2,	127,	"seqArriveCorville"		},
	{33,	"bunymrch",	2,	105,	"seqSingleBunny"			},
	{34,	"valkyrs",	2,	127,	"seqBunnyArmy"				},
	{35,	"melcut",		2,	127,	"seqArriveAtMines"		},
	{36,	"veltures",	2,	127,	"seqArriveAtVultures"	},
	{37,	"sorry",		2,	127,	"seqMakePlan"					},
	{38,	"makeplan",	2,	127,	"seqShowPlan"					},
	{39,	"castle",		2,	127,	"seqDerbyStart"				},
	{40,	"fire",			3,	127,	"seqLightBales"				},
	{41,	"saveme",		3,	127,	"seqNestolusBBQ"			},
	{42,	"cops2",		2,	127,	"seqCallSecurity"			},
	{43,	"sorry",		2,	127,	"seqFilmFail"					},
	{44,	"sorry",		2,	127,	"seqFilmBurn"					},
	{45,	"caveamb",	2,	127,	"seqRipSpeech"				},
	{46,	"expose",		2,	127,	"seqExposeRip"				},
	{47,	"ripscram",	2,	127,	"seqRipEscape"				},
	{48,	"",					0,	0,		"seqRareMoment"				},
	{49,	"valkyrs",	2,	127,	"seqFanBunnies"				},
	{50,	"ripdead",	2,	127,	"seqRipDead"					},
	{51,	"funeral",	2,	127,	"seqFuneral"					},
	{52,	"bornbad",	2,	127,	"seqCredits"					},
	{-1,	"",					0,	0,		""										}
};
#endif

static uint32 decode12BitsSample(byte *src, byte **dst, uint32 size, bool stereo) {
	uint32 loop_size = size / 3;
	uint32 s_size = loop_size * 4;
	if (stereo) {
		s_size *= 2;
	}
	byte *ptr = *dst = (byte *)malloc(s_size);

	uint32 tmp;
	while (loop_size--) {
		byte v1 = *src++;
		byte v2 = *src++;
		byte v3 = *src++;
		tmp = ((((v2 & 0x0f) << 8) | v1) << 4) - 0x8000;
		WRITE_BE_UINT16(ptr, tmp); ptr += 2;
		if (stereo) {
			WRITE_BE_UINT16(ptr, tmp); ptr += 2;
		}
		tmp = ((((v2 & 0xf0) << 4) | v3) << 4) - 0x8000;
		WRITE_BE_UINT16(ptr, tmp); ptr += 2;
		if (stereo) {
			WRITE_BE_UINT16(ptr, tmp); ptr += 2;
		}
	}
	return s_size;
}

void IMuseDigital::timer_handler(void *refCon) {
	IMuseDigital *imuseDigital = (IMuseDigital *)refCon;
	imuseDigital->callback();
}

IMuseDigital::IMuseDigital(ScummEngine *scumm)
	: _scumm(scumm) {
	_pause = false;
	
	_voiceVocData = NULL;
	_voiceVocSize = 0;
	_voiceVocRate = 0;

	_voiceBundleData = NULL;

	_nameBundleMusic = "";
	_musicBundleBufFinal = NULL;
	_musicBundleBufOutput = NULL;
	_musicDisk = 0;

	_bundle = new Bundle();

	_scumm->_timer->installTimerProc(timer_handler, 40000, this);
}

IMuseDigital::~IMuseDigital() {
	_scumm->_timer->removeTimerProc(timer_handler);

	for (int l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
		_scumm->_mixer->stopHandle(_channel[l].handle);
	}

	delete _bundle;
}

void IMuseDigital::callback() {
	int l = 0;

	if (_pause || !_scumm)
		return;

	for (l = 0; l < MAX_DIGITAL_CHANNELS;l ++) {
		if (_channel[l].used) {
			if (_channel[l].toBeRemoved) {
				debug(5, "IMuseDigital::callback(): stoped sound: %d", _channel[l].idSound);
				_scumm->_mixer->endStream(_channel[l].handle);
				free(_channel[l].data);
				_channel[l].used = false;
				continue;
			}

			if (_channel[l].volFadeUsed == true) {
				if (_channel[l].volFadeStep < 0) {
					if (_channel[l].vol > _channel[l].volFadeDest) {
						_channel[l].vol += _channel[l].volFadeStep;
						if (_channel[l].vol < _channel[l].volFadeDest) {
							_channel[l].vol = _channel[l].volFadeDest;
							_channel[l].volFadeUsed = false;
						}
						if (_channel[l].vol == 0) {
							_channel[l].toBeRemoved = true;
						}
					}
				} else if (_channel[l].volFadeStep > 0) {
					if (_channel[l].vol < _channel[l].volFadeDest) {
						_channel[l].vol += _channel[l].volFadeStep;
						if (_channel[l].vol > _channel[l].volFadeDest) {
							_channel[l].vol = _channel[l].volFadeDest;
							_channel[l].volFadeUsed = false;
						}
					}
				}
				debug(5, "Fade: sound(%d), Vol(%d)", _channel[l].idSound, _channel[l].vol / 1000);
			}

			int32 mixer_size = _channel[l].mixerSize;

			if (_channel[l].offset + mixer_size > _channel[l].size) {
				mixer_size = _channel[l].size - _channel[l].offset;
				_channel[l].toBeRemoved = true;
			}

			int pan = (_channel[l].pan != 64) ? 2 * _channel[l].pan - 127 : 0;
			if (_scumm->_mixer->isReady()) {
				if (!_channel[l].handle.isActive())
					_scumm->_mixer->newStream(&_channel[l].handle, _channel[l].freq,
											_channel[l].mixerFlags, 100000);
				_scumm->_mixer->setChannelVolume(_channel[l].handle, _channel[l].vol / 1000);
				_scumm->_mixer->setChannelPan(_channel[l].handle, pan);
				_scumm->_mixer->appendStream(_channel[l].handle, _channel[l].data + _channel[l].offset, mixer_size);
			}
			_channel[l].offset += mixer_size;
		}
	}
}

void IMuseDigital::setVocVoice(byte *src, int32 size, int rate) {
	_voiceVocData = src;
	_voiceVocSize = size;
	_voiceVocRate = rate;
}

void IMuseDigital::setBundleVoice(byte *src) {
	_voiceBundleData = src;
}

void IMuseDigital::startSound(int sound) {
	debug(5, "IMuseDigital::startSound(%d)", sound);
	int l, r;

	for (l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
		if (!_channel[l].used && !_channel[l].handle.isActive()) {
			byte *ptr, *s_ptr;
			if ((sound == kTalkSoundID) && (_voiceBundleData)) {
				s_ptr = ptr = _voiceBundleData;
			} else if ((sound == kTalkSoundID) && (_voiceVocData)) {
				//
				s_ptr = ptr = 0;
			} else if (sound != kTalkSoundID) {
				ptr = _scumm->getResourceAddress(rtSound, sound);
				s_ptr = ptr;
				if (ptr == NULL) {
					warning("IMuseDigital::startSound(%d) NULL resource pointer", sound);
					return;
				}
			} else {
				error("IMuseDigital::startSound() unknown condition");
			}
			_channel[l].pan = 64;
			_channel[l].vol = 127 * 1000;
			_channel[l].volFadeDest = 0;
			_channel[l].volFadeStep = 0;
			_channel[l].volFadeDelay = 0;
			_channel[l].volFadeUsed = false;

			_channel[l].numJumps = 0;
			_channel[l].numRegions = 0;
			_channel[l].numMarkers = 0;

			_channel[l].offset = 0;
			_channel[l].idSound = sound;

			uint32 tag;
			int32 size = 0;
			int t;

			if ((sound == kTalkSoundID) && (_voiceVocData)) {
				_channel[l].mixerSize = _voiceVocRate * 2;
				_channel[l].freq = _voiceVocRate;
				_channel[l].size = _voiceVocSize * 2;
				_channel[l].bits = 8;
				_channel[l].channels = 2;
				_channel[l].mixerFlags = SoundMixer::FLAG_STEREO | SoundMixer::FLAG_REVERSE_STEREO | SoundMixer::FLAG_UNSIGNED;
				_channel[l].data = (byte *)malloc(_channel[l].size);

				for (t = 0; t < _channel[l].size / 2; t++) {
					*(_channel[l].data + t * 2 + 0) = *(_voiceVocData + t);
					*(_channel[l].data + t * 2 + 1) = *(_voiceVocData + t);
				}

				_voiceVocData = NULL;
			} else if (READ_UINT32(ptr) == MKID('Crea')) {
				byte *t_ptr= readCreativeVoc(ptr, size, _channel[l].freq);
				_channel[l].mixerSize = _channel[l].freq * 2;
				_channel[l].size = size * 2;
				_channel[l].bits = 8;
				_channel[l].channels = 2;
				_channel[l].mixerFlags = SoundMixer::FLAG_STEREO | SoundMixer::FLAG_REVERSE_STEREO | SoundMixer::FLAG_UNSIGNED;
				_channel[l].data = (byte *)malloc(_channel[l].size);

				for (t = 0; t < _channel[l].size / 2; t++) {
					*(_channel[l].data + t * 2 + 0) = *(t_ptr + t);
					*(_channel[l].data + t * 2 + 1) = *(t_ptr + t);
				}

				free(t_ptr);
			} else if (READ_UINT32(ptr) == MKID('iMUS')) {
				ptr += 16;
				for (;;) {
					tag = READ_BE_UINT32(ptr); ptr += 4;
					switch(tag) {
					case MKID_BE('FRMT'):
						ptr += 12;
						_channel[l].bits = READ_BE_UINT32(ptr); ptr += 4;
						_channel[l].freq = READ_BE_UINT32(ptr); ptr += 4;
						_channel[l].channels = READ_BE_UINT32(ptr); ptr += 4;
					break;
					case MKID_BE('TEXT'):
						size = READ_BE_UINT32(ptr); ptr += 4;
						if (_channel[l].numMarkers >= MAX_IMUSE_MARKERS) {
							warning("IMuseDigital::startSound() Not enough space for Marker");
							ptr += size;
							break;
						}
						strcpy(_channel[l].marker[_channel[l].numMarkers].name, (char *)ptr + 4);
						_channel[l].numMarkers++;
						ptr += size;
						break;
					case MKID_BE('REGN'):
						size = READ_BE_UINT32(ptr); ptr += 4;
						if (_channel[l].numRegions >= MAX_IMUSE_REGIONS) {
							warning("IMuseDigital::startSound() Not enough space for Region");
							ptr += 8;
							break;
						}
						_channel[l].region[_channel[l].numRegions].start = READ_BE_UINT32(ptr); ptr += 4;
						_channel[l].region[_channel[l].numRegions].length = READ_BE_UINT32(ptr); ptr += 4;
						_channel[l].numRegions++;
						break;
					case MKID_BE('STOP'):
						ptr += 4;
						_channel[l].offsetStop = READ_BE_UINT32(ptr); ptr += 4;
						break;
					case MKID_BE('JUMP'):
						size = READ_BE_UINT32(ptr); ptr += 4;
						if (_channel[l].numJumps >= MAX_IMUSE_JUMPS) {
							warning("IMuseDigital::startSound() Not enough space for Jump");
							ptr += size;
							break;
						}
						_channel[l].jump[_channel[l].numJumps].start = READ_BE_UINT32(ptr); ptr += 4;
						_channel[l].jump[_channel[l].numJumps].dest = READ_BE_UINT32(ptr); ptr += 4;
						_channel[l].jump[_channel[l].numJumps].hookId = READ_BE_UINT32(ptr); ptr += 4;
						_channel[l].jump[_channel[l].numJumps].fadeDelay = READ_BE_UINT32(ptr); ptr += 4;
						_channel[l].numJumps++;
						break;
					case MKID_BE('SYNC'):
						size = READ_BE_UINT32(ptr); ptr += size + 4;
						break;
					case MKID_BE('DATA'):
						size = READ_BE_UINT32(ptr); ptr += 4;
						break;
					default:
						error("IMuseDigital::startSound(%d) Unknown sfx header '%s'", sound, tag2str(tag));
					}
					if (tag == MKID_BE('DATA'))
						break;
				}

				if ((sound == kTalkSoundID) && (_voiceBundleData)) {
					if (_scumm->_actorToPrintStrFor != 0xFF && _scumm->_actorToPrintStrFor != 0) {
						Actor *a = _scumm->derefActor(_scumm->_actorToPrintStrFor, "playBundleSound");
						_channel[l].freq = (_channel[l].freq * a->talkFrequency) / 256;
						_channel[l].pan = a->talkPan;
					}
				}

				uint32 header_size = ptr - s_ptr;

				_channel[l].offsetStop -= header_size;
				if (_channel[l].bits == 12) {
					_channel[l].offsetStop = (_channel[l].offsetStop / 3) * 4;
				}
				for (r = 0; r < _channel[l].numRegions; r++) {
					_channel[l].region[r].start -= header_size;
					if (_channel[l].bits == 12) {
						_channel[l].region[r].start = (_channel[l].region[r].start / 3) * 4;
						_channel[l].region[r].length = (_channel[l].region[r].length / 3) * 4;
					}
				}
				if (_channel[l].numJumps > 0) {
					for (r = 0; r < _channel[l].numJumps; r++) {
						_channel[l].jump[r].start -= header_size;
						_channel[l].jump[r].dest -= header_size;
						if (_channel[l].bits == 12) {
							_channel[l].jump[r].start = (_channel[l].jump[r].start / 3) * 4;
							_channel[l].jump[r].dest = (_channel[l].jump[r].dest / 3) * 4;
						}
					}
				}

				_channel[l].mixerFlags = SoundMixer::FLAG_STEREO | SoundMixer::FLAG_REVERSE_STEREO;
				_channel[l].mixerSize = _channel[l].freq * 2;

				if (_channel[l].bits == 12) {
					_channel[l].mixerSize *= 2;
					_channel[l].mixerFlags |= SoundMixer::FLAG_16BITS;
					_channel[l].size = decode12BitsSample(ptr, &_channel[l].data, size, (_channel[l].channels == 2) ? false : true);
				} else if (_channel[l].bits == 16) {
					_channel[l].mixerSize *= 2;
					_channel[l].mixerFlags |= SoundMixer::FLAG_16BITS;
					
					// FIXME: For some weird reasons, sometimes we get an odd size, even though
					// the data is supposed to be in 16 bit format... that makes no sense...
					size &= ~1;
					
					if (_channel[l].channels == 1) {
						size *= 2;
						_channel[l].channels = 2;
						_channel[l].data = (byte *)malloc(size);
						for (t = 0; t < size / 4; t++) {
							*(_channel[l].data + t * 4 + 0) = *(ptr + t * 2 + 0);
							*(_channel[l].data + t * 4 + 1) = *(ptr + t * 2 + 1);
							*(_channel[l].data + t * 4 + 2) = *(ptr + t * 2 + 0);
							*(_channel[l].data + t * 4 + 3) = *(ptr + t * 2 + 1);
						}
					}
					_channel[l].size = size;
				} else if (_channel[l].bits == 8) {
					_channel[l].mixerFlags |= SoundMixer::FLAG_UNSIGNED;
					if (_channel[l].channels == 1) {
						size *= 2;
						_channel[l].channels = 2;
						_channel[l].data = (byte *)malloc(size);
						for (t = 0; t < size / 2; t++) {
							*(_channel[l].data + t * 2 + 0) = *(ptr + t);
							*(_channel[l].data + t * 2 + 1) = *(ptr + t);
						}
					} else {
						_channel[l].data = (byte *)malloc(size);
						memcpy(_channel[l].data, ptr, size);
					}
					_channel[l].size = size;
				} else
					error("Can't handle %d bit samples in iMuseDigital", _channel[l].bits);
			}
			_channel[l].mixerSize /= 25;
			_channel[l].toBeRemoved = false;
			_channel[l].used = true;
			break;
		}
	}
}

void IMuseDigital::stopSound(int sound) {
	debug(5, "IMuseDigital::stopSound(%d)", sound);
	for (int l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
		if ((_channel[l].idSound == sound) && _channel[l].used) {
			_channel[l].toBeRemoved = true;
		}
	}
}

void IMuseDigital::stopAllSounds() {
	debug(5, "IMuseDigital::stopAllSounds");
	for (int l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
		if (_channel[l].used) {
			_channel[l].toBeRemoved = true;
		}
	}
}

void IMuseDigital::pause(bool p) {
	for (int l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
		if (_channel[l].used) {
			_scumm->_mixer->pauseHandle(_channel[l].handle, p);
		}
	}
	_pause = p;
	pauseBundleMusic(p);
}

void IMuseDigital::parseScriptCmds(int a, int b, int c, int d, int e, int f, int g, int h) {
	int cmd = a;
	int sample = b;
	int sub_cmd = c;
	int chan = -1;
	int l, r;

	if (!cmd)
		return;

	switch (cmd) {
	case 10: // ImuseStopAllSounds
		debug(5, "ImuseStopAllSounds()");
		stopAllSounds();
		break;
	case 12: // ImuseSetParam
		switch (sub_cmd) {
		case 0x500: // set priority - could be ignored
			break;
		case 0x600: // set volume
			debug(5, "ImuseSetParam (0x600), sample(%d), volume(%d)", sub_cmd, sample, d);
			for (l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
				if ((_channel[l].idSound == sample) && _channel[l].used) {
					_channel[l].vol = d * 1000;
//					if (_channel[l].volFadeUsed)
//						_channel[l].volFadeStep = (_channel[l].volFadeDest - _channel[l].vol) * 60 * 40 / (1000 * _channel[chan].volFadeDelay);
				}
			}
			if (l == -1) {
				debug(5, "ImuseSetParam (0x600), sample(%d) not exist in channels", sub_cmd, sample);
				return;
			}
			break;
		case 0x700: // set pan
			debug(5, "ImuseSetParam (0x700), sample(%d), pan(%d)", sample, d);
			for (l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
				if ((_channel[l].idSound == sample) && _channel[l].used) {
					_channel[l].pan = d;
				}
			}
			if (l == -1) {
				debug(5, "ImuseSetParam (0x700), sample(%d) not exist in channels", sample);
				return;
			}
			break;
		default:
			warning("IMuseDigital::doCommand SetParam DEFAULT command %d", sub_cmd);
			break;
		}
		break;
	case 14: // ImuseFadeParam
		switch (sub_cmd) {
		case 0x600: // set new volume with fading
			debug(5, "ImuseFadeParam - fade sample(%d), to volume(%d) with 60hz ticks(%d)", sample, d, e);
			if ((_scumm->_gameId == GID_DIG) && (_scumm->_features & GF_DEMO)) {
				stopSound(sample);
				return;
			}
			for (l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
				if ((_channel[l].idSound == sample) && _channel[l].used) {
					_channel[l].volFadeDelay = e;
					_channel[l].volFadeDest = d * 1000;
					_channel[l].volFadeStep = (_channel[l].volFadeDest - _channel[l].vol) * 60 * 40 / (1000 * e);
					_channel[l].volFadeUsed = true;
					debug(5, "ImuseFadeParam: vol %d, volDest %d, step %d", _channel[l].vol, d * 1000, _channel[l].volFadeStep);
				}
			}
			if (chan == -1) {
				debug(5, "ImuseFadeParam (0x600), sample %d not exist in channels", sample);
				return;
			}
			break;
		default:
			warning("IMuseDigital::doCommand FadeParam DEFAULT sub command %d", sub_cmd);
			break;
		}
		break;
	case 0x1000: // ImuseSetState
		debug(5, "ImuseSetState (%d)", b);
		if ((_scumm->_gameId == GID_DIG) && (_scumm->_features & GF_DEMO)) {
			if (b == 1)
				startSound(1);
			else {
				if (getSoundStatus(2) == 0)
					startSound(2);
			}
		} else if (_scumm->_gameId == GID_DIG) {
			if (b == 1000) {		// STATE_NULL
				stopBundleMusic();
				return;
			}
			for (l = 0; _digStateMusicMap[l].room != -1; l++) {
				if (_digStateMusicMap[l].room == b) {
					int music = _digStateMusicMap[l].table_index;
					debug(5, "Play imuse music: %s, %s, %s", _digStateMusicTable[music].name, _digStateMusicTable[music].title, _digStateMusicTable[music].filename);
					if ((_digStateMusicTable[music].filename[0] != 0) && 
						(strcmp(_digStateMusicTable[_digStateMusicTable[music].unk3].filename, _nameBundleMusic) != 0) ) {
						playBundleMusic(_digStateMusicTable[music].filename);
					}
					break;
				}
			}
		} else if ((_scumm->_gameId == GID_CMI) && (_scumm->_features & GF_DEMO)) {
			if (b == 2) {
				playBundleMusic("in1.imx");
			} else if (b == 4) {
				playBundleMusic("in2.imx");
			} else if (b == 8) {
				playBundleMusic("out1.imx");
			} else if (b == 9) {
				playBundleMusic("out2.imx");
			} else if (b == 16) {
				playBundleMusic("gun.imx");
			} else {
				warning("imuse digital: set state unknown for cmi demo: %d, room: %d", b, this->_scumm->_currentRoom);
			}
		} else if (_scumm->_gameId == GID_CMI) {
			if (b == 1000) {		// STATE_NULL
				stopBundleMusic();
				return;
			}
			for (l = 0; _comiStateMusicTable[l].id != -1; l++) {
				if ((_comiStateMusicTable[l].id == b)) {
					debug(5, "Play imuse music: %s, %s, %s", _comiStateMusicTable[l].name, _comiStateMusicTable[l].title, _comiStateMusicTable[l].filename);
					if (_comiStateMusicTable[l].filename[0] != 0) {
						playBundleMusic(_comiStateMusicTable[l].filename);
					}
					break;
				}
			}
		} else if (_scumm->_gameId == GID_FT) {
			for (l = 0;; l++) {
				if (_ftStateMusicTable[l].index == -1) {
					return;
				}
				if (_ftStateMusicTable[l].index == b) {
					debug(5, "Play imuse music: %s, %s", _ftStateMusicTable[l].name, _ftStateMusicTable[l].audioname);
					if (_ftStateMusicTable[l].audioname[0] != 0) {
						for (r = 0; r < _scumm->_numAudioNames; r++) {
							if (strcmp(_ftStateMusicTable[l].audioname, &_scumm->_audioNames[r * 9]) == 0) {
								startSound(r);
								parseScriptCmds(12, r, 1536, _ftStateMusicTable[l].volume, 0, 0, 0, 0);
							}
						}
					}
				}
			}
		}
		break;
	case 0x1001: // ImuseSetSequence
		debug(5, "ImuseSetSequence (%d)", b);
		if (_scumm->_gameId == GID_DIG) {
			for (l = 0; _digSeqMusicTable[l].room != -1; l++) {
				if (_digSeqMusicTable[l].room == b) {
					debug(5, "Play imuse music: %s, %s, %s", _digSeqMusicTable[l].name, _digSeqMusicTable[l].title, _digSeqMusicTable[l].filename);
					if (_digSeqMusicTable[l].filename[0] != 0) {
						playBundleMusic(_digSeqMusicTable[l].filename);
					}
					break;
				}
			}
		} else if (_scumm->_gameId == GID_CMI) {
			for (l = 0; _comiSeqMusicTable[l].id != -1; l++) {
				if (_comiSeqMusicTable[l].id == b) {
					debug(5, "Play imuse music: %s, %s, %s", _comiSeqMusicTable[l].name, _comiSeqMusicTable[l].title, _comiSeqMusicTable[l].filename);
					if (_comiSeqMusicTable[l].filename[0] != 0) {
						playBundleMusic(_comiSeqMusicTable[l].filename);
					}
					break;
				}
			}
		} else if (_scumm->_gameId == GID_FT) {
			for (l = 0; _ftSeqMusicTable[l].index != -1; l++) {
				if (_ftSeqMusicTable[l].index == b) {
					debug(5, "Play imuse music: %s, %s", _ftSeqMusicTable[l].name, _ftSeqMusicTable[l].audioname);
					if (_ftSeqMusicTable[l].audioname[0] != 0) {
						for (r = 0; r < _scumm->_numAudioNames; r++) {
							if (strcmp(_ftSeqMusicTable[l].audioname, &_scumm->_audioNames[r * 9]) == 0) {
//								startSound(r);
//								parseScriptCmds(12, r, 1536, _ftSeqMusicTable[l].volume, 0, 0, 0, 0);
							}
						}
					}
				}
			}
		}
		break;
	case 0x1002: // ImuseSetCuePoint
		debug(5, "ImuseSetCuePoint (%d)", b);
		break;
	case 0x1003: // ImuseSetAttribute
		debug(5, "ImuseSetAttribute (%d, %d)", b, c);
		break;
	case 0x2000: // ImuseSetMasterSFXVolume
		debug(5, "ImuseSetMasterSFXVolume (%d)", b);
		break;
	case 0x2001: // ImuseSetMasterVoiceVolume
		debug(5, "ImuseSetMasterVoiceVolume (%d)", b);
		break;
	case 0x2002: // ImuseSetMasterMusicVolume
		debug(5, "ImuseSetMasterMusicVolume (%d)", b);
		break;
	default:
		warning("IMuseDigital::doCommand DEFAULT command %d", cmd);
	}
}

int IMuseDigital::getSoundStatus(int sound) const {
	debug(5, "IMuseDigital::getSoundStatus(%d)", sound);
	for (int l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
		if ((_channel[l].idSound == sound) && _channel[l].used) {
			return 1;
		}
	}

	return 0;
}

void IMuseDigital::music_handler(void *refCon) {
	((IMuseDigital *)refCon)->bundleMusicHandler();
}

void IMuseDigital::playBundleMusic(const char *song) {
	if (!_scumm->_mixer->isReady()) {
		return;
	}

	if (_nameBundleMusic[0] == 0) {
		if (_scumm->_gameId == GID_CMI) {
			_outputMixerSize = (22050 * 2 * 2);

			char bunfile[20];
			if (_scumm->_features & GF_DEMO) {
				if (_bundle->openMusicFile("music.bun", _scumm->getGameDataPath()) == false) {
					_outputMixerSize = 0;
					return;
				}
			} else {

				sprintf(bunfile, "musdisk%d.bun", _scumm->VAR(_scumm->VAR_CURRENTDISK));
				if (_musicDisk != _scumm->VAR(_scumm->VAR_CURRENTDISK)) 
					_bundle->closeMusicFile();

				if (_bundle->openMusicFile(bunfile, _scumm->getGameDataPath()) == false) {
					if (_bundle->openMusicFile("music.bun", _scumm->getGameDataPath()) == false) {
						_outputMixerSize = 0;
						return;
					}
				}

				_musicDisk = (byte)_scumm->VAR(_scumm->VAR_CURRENTDISK);
			}
		} else {
			_outputMixerSize = ((22050 * 2 * 2) / 4) * 3;

			if (_bundle->openMusicFile("digmusic.bun", _scumm->getGameDataPath()) == false)
				return;
		}
		_musicBundleBufFinal = (byte *)malloc(_outputMixerSize);
		_musicBundleBufOutput = (byte *)malloc(((_outputMixerSize / 0x2000) + 1) * _outputMixerSize);
		_currentSampleBundleMusic = 0;
		_offsetSampleBundleMusic = 0;
		_offsetBufBundleMusic = 0;
		_bundleMusicPosition = 0;
		_bundleSongPosInMs = 0;
		_pauseBundleMusic = false;
		_musicBundleToBeChanged = false;
		_numberSamplesBundleMusic = _bundle->getNumberOfMusicSamplesByName(song);
		_nameBundleMusic = song;
		_scumm->_timer->installTimerProc(&music_handler, 1000000, this);
	} else if (strcmp(_nameBundleMusic, song) != 0) {
		_newNameBundleMusic = song;
		_musicBundleToBeChanged = true;
	}
}

void IMuseDigital::pauseBundleMusic(bool state) {
	_pauseBundleMusic = state;
}

void IMuseDigital::stopBundleMusic() {
	// First stop the music timer
	_scumm->_timer->removeTimerProc(&music_handler);
	_nameBundleMusic = "";
	_scumm->_mixer->stopHandle(_bundleMusicTrack);
	if (_musicBundleBufFinal) {
		free(_musicBundleBufFinal);
		_musicBundleBufFinal = NULL;
	}
	if (_musicBundleBufOutput) {
		free(_musicBundleBufOutput);
		_musicBundleBufOutput = NULL;
	}
}

void IMuseDigital::bundleMusicHandler() {
	byte *ptr;
	int32 l, num = _numberSamplesBundleMusic, length, k;
	int32 rate = 22050;
	int32 tag, size = -1, header_size = 0;

	if (_pauseBundleMusic)
		return;

	if (_musicBundleToBeChanged) {
		_nameBundleMusic = _newNameBundleMusic;
		_numberSamplesBundleMusic = _bundle->getNumberOfMusicSamplesByName(_nameBundleMusic);
		_currentSampleBundleMusic = 0;
		_offsetSampleBundleMusic = 0;
		_offsetBufBundleMusic = 0;
		_musicBundleToBeChanged = false;
		_bundleMusicPosition = 0;
		_bundleSongPosInMs = 0;
	}

	ptr = _musicBundleBufOutput;

	for (k = 0, l = _currentSampleBundleMusic; l < num; k++) {
		length = _bundle->decompressMusicSampleByName(_nameBundleMusic, l, (_musicBundleBufOutput + ((k * 0x2000) + _offsetBufBundleMusic)));
		_offsetSampleBundleMusic += length;

		if (l == 0) {
			tag = READ_BE_UINT32(ptr); ptr += 4;
			if (tag != MKID_BE('iMUS')) {
				error("Decompressing bundle song failed (unknown tag '%s')", tag2str(tag));
			}

			ptr += 12;
			while (tag != MKID_BE('DATA')) {
				tag = READ_BE_UINT32(ptr);  ptr += 4;
				switch(tag) {
				case MKID_BE('FRMT'):
					ptr += 12;
					_bundleMusicSampleBits = READ_BE_UINT32(ptr); ptr += 4;
					rate = READ_BE_UINT32(ptr); ptr += 4;
					_bundleSampleChannels = READ_BE_UINT32(ptr); ptr += 4;
					break;
				case MKID_BE('TEXT'):
				case MKID_BE('REGN'):
				case MKID_BE('STOP'):
				case MKID_BE('JUMP'):
					size = READ_BE_UINT32(ptr); ptr += size + 4;
					break;
				case MKID_BE('SYNC'):
					size = READ_BE_UINT32(ptr);
					printf("bundleMusicHandler: Lip sync data:\n");
					Common::hexdump(ptr, size + 4);
					ptr += size + 4;
					break;
				case MKID_BE('DATA'):
					size = READ_BE_UINT32(ptr); ptr += 4;
					break;

				default:
					error("Unknown sound header '%s'", tag2str(tag));
				}
			}
			if (size < 0) {
				error("Decompressing sound failed (missing size field)");
			}
			header_size = (ptr - _musicBundleBufOutput);
		}

		l++;
		_currentSampleBundleMusic = l;

		if (_offsetSampleBundleMusic >= _outputMixerSize + header_size) {
			memcpy(_musicBundleBufFinal, (_musicBundleBufOutput + header_size), _outputMixerSize);
			_offsetBufBundleMusic = _offsetSampleBundleMusic - _outputMixerSize - header_size;
			memcpy(_musicBundleBufOutput, (_musicBundleBufOutput + (_outputMixerSize + header_size)), _offsetBufBundleMusic);
			_offsetSampleBundleMusic = _offsetBufBundleMusic;
			break;
		}
	}

	if (_currentSampleBundleMusic == num) {
		_currentSampleBundleMusic = 0;
		_offsetSampleBundleMusic = 0;
		_offsetBufBundleMusic = 0;
		_bundleMusicPosition = 0;
		_bundleSongPosInMs = 0;
	}

	ptr = _musicBundleBufFinal;

	byte *buffer = NULL;
	uint32 final_size;
	if (_bundleMusicSampleBits == 12) {
		final_size = decode12BitsSample(ptr, &buffer, _outputMixerSize, false);
	} else if (_bundleMusicSampleBits == 16) {
		buffer = (byte *)malloc(_outputMixerSize);
		final_size = _outputMixerSize;
		memcpy(buffer, ptr, _outputMixerSize);
	} else {
		warning("IMuseDigital::bundleMusicHandler TODO: more newStream options...");
		return;
	}

	// Compute the position in the song in milliseconds (in brief: _outputMixerSize
	// gives the number of bytes used for one second, so if we compute the value of
	// (_bundleMusicPosition / _outputMixerSize), that is an offset in seconds;
	// multiplying that by 1000 gives milliseconds).
	_bundleSongPosInMs = 1000 * _bundleMusicPosition / _outputMixerSize;
	_bundleMusicPosition += final_size;
	if (!_bundleMusicTrack.isActive())
		_scumm->_mixer->newStream(&_bundleMusicTrack, rate, SoundMixer::FLAG_16BITS | SoundMixer::FLAG_STEREO, 300000);
	_scumm->_mixer->appendStream(_bundleMusicTrack, buffer, final_size);
	free(buffer);
}

void IMuseDigital::playBundleSound(const char *sound) {
	byte *ptr = 0;
	bool result;

	if (!_scumm->_mixer->isReady())
		return;	

	if (_scumm->_gameId == GID_CMI) {
		if (_scumm->_features & GF_DEMO) {
			result = _bundle->openVoiceFile("voice.bun", _scumm->getGameDataPath());
		} else {
			char voxfile[20];
			sprintf(voxfile, "voxdisk%d.bun", _scumm->VAR(_scumm->VAR_CURRENTDISK));
			if (_voiceDisk != _scumm->VAR(_scumm->VAR_CURRENTDISK))
				_bundle->closeVoiceFile();

			result = _bundle->openVoiceFile(voxfile, _scumm->getGameDataPath());

			if (result == false)
				result = _bundle->openVoiceFile("voice.bun", _scumm->getGameDataPath());
			_voiceDisk = (byte)_scumm->VAR(_scumm->VAR_CURRENTDISK);
		}
	} else if (_scumm->_gameId == GID_DIG)
		result = _bundle->openVoiceFile("digvoice.bun", _scumm->getGameDataPath());
	else
		error("Don't know which bundle file to load");

	if (!result)
		return;

	if (_scumm->_gameId == GID_CMI) {
		char name[20];
		strcpy(name, sound);
		if (!(_scumm->_features & GF_DEMO)) // CMI demo does not have .IMX for voice but does for music...
			strcat(name, ".IMX");
		_bundle->decompressVoiceSampleByName(name, &ptr);
	} else {
		_bundle->decompressVoiceSampleByName(sound, &ptr);
	}

	if (ptr) {
		stopSound(kTalkSoundID);
		setBundleVoice(ptr);
		startSound(kTalkSoundID);
		free(ptr);
	}
}

} // End of namespace Scumm

#ifdef __PALM_OS__
#include "scumm_globals.h"

_GINIT(IMuseDigital)
_GSETPTR(Scumm::_digStateMusicMap,		GBVARS_DIGSTATEMUSICMAP_INDEX,		Scumm::imuse_music_map		, GBVARS_SCUMM)
_GSETPTR(Scumm::_digStateMusicTable,	GBVARS_DIGSTATEMUSICTABLE_INDEX,	Scumm::imuse_music_table	, GBVARS_SCUMM)
_GSETPTR(Scumm::_comiStateMusicTable,	GBVARS_COMISTATEMUSICTABLE_INDEX,	Scumm::imuse_music_table	, GBVARS_SCUMM)
_GSETPTR(Scumm::_comiSeqMusicTable,		GBVARS_COMISEQMUSICTABLE_INDEX,		Scumm::imuse_music_table	, GBVARS_SCUMM)
_GSETPTR(Scumm::_digSeqMusicTable,		GBVARS_DIGSEQMUSICTABLE_INDEX,		Scumm::imuse_music_table	, GBVARS_SCUMM)
_GSETPTR(Scumm::_ftStateMusicTable,		GBVARS_FTSTATEMUSICTABLE_INDEX,		Scumm::imuse_ft_music_table	, GBVARS_SCUMM)
_GSETPTR(Scumm::_ftSeqMusicTable,		GBVARS_FTSEQMUSICTABLE_INDEX,		Scumm::imuse_ft_music_table	, GBVARS_SCUMM)
_GEND

_GRELEASE(IMuseDigital)
_GRELEASEPTR(GBVARS_DIGSTATEMUSICMAP_INDEX		, GBVARS_SCUMM)
_GRELEASEPTR(GBVARS_DIGSTATEMUSICTABLE_INDEX	, GBVARS_SCUMM)
_GRELEASEPTR(GBVARS_COMISTATEMUSICTABLE_INDEX	, GBVARS_SCUMM)
_GRELEASEPTR(GBVARS_COMISEQMUSICTABLE_INDEX		, GBVARS_SCUMM)
_GRELEASEPTR(GBVARS_DIGSEQMUSICTABLE_INDEX		, GBVARS_SCUMM)
_GRELEASEPTR(GBVARS_FTSTATEMUSICTABLE_INDEX		, GBVARS_SCUMM)
_GRELEASEPTR(GBVARS_FTSEQMUSICTABLE_INDEX		, GBVARS_SCUMM)
_GEND

#endif
