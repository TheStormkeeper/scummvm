/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#ifndef KOM_DATABASE_H
#define KOM_DATABASE_H

#include "common/str.h"
#include "common/list.h"
#include "common/file.h"
#include "engines/engine.h"

#include "kom/actor.h"

#if defined(__GNUC__)
	#define KOM_GCC_SCANF(x,y) __attribute__((format(scanf, x, y)))
#else
	#define KOM_GCC_SCANF(x,y)
#endif

namespace Kom {

class KomEngine;

struct EventLink {
	int data1;
	int data2;
};

struct Object {
	Object() : data1(0), type(0), data2(0), proc(0),
	           data4(0), isCarryable(0), isContainer(0), isVisible(0), isSprite(0), isUseImmediate(0),
	           data9(0), isUsable(0), price(0), data11(0), spellCost(0), data12(0),
	           data13(0), ownerType(0), ownerId(0), box(0), data16(0), data17(0),
	           data18(0) {}

	char name[7];
	int data1;
	char desc[50];
	int type;
	int data2;
	int proc;
	int data4;
	int isCarryable;
	int isContainer;
	int isVisible;
	int isSprite;
	int isUseImmediate;
	int data9;
	int isUsable;
	int price;
	int data11;
	int spellCost;
	int data12;
	int data13;
	int ownerType;
	int ownerId;
	int box;
	int data16;
	int data17;
	int data18;
	Common::List<int> contents;
};

struct Character {
	Character() : mode(0), modeCount(0), isBusy(false), isAlive(true), isVisible(true),
		spellMode(0), gold(0) {}
	char name[7];
	int xtend;
	int data2;
	char desc[50];
	int proc;
	int locationId;
	int box;
	int data5;
	int data6;
	int data7;
	int data8;
	int data9;
	int hitPoints;
	int hitPointsMax;
	uint16 mode;
	uint16 modeCount;
	bool isBusy;
	bool isAlive;
	bool isVisible;
	uint8 spellMode;
	int strength;
	int defense;
	int damageMin;
	int damageMax;
	int data14; // spell speed - unused
	int data15; // spell time - unused
	int data16;
	int spellPoints;
	int spellPointsMax;
	int16 destLoc;
	int16 destBox;
	int gold;
	Common::List<int> inventory;
	Common::List<int> weapons;
	Common::List<int> spells;
};

struct Location {
	char name[7];
	int xtend;
	int allowedTime;
	char desc[50];
	Common::List<EventLink> events;
	Common::List<int> objects;
	Common::List<int> characters;
};

struct OpCode {
	int opcode;
	char arg1[30];
	int arg2;
	int arg3;
	int arg4;
	int arg5;
	int arg6;
	OpCode() : arg2(0), arg3(0), arg4(0), arg5(0), arg6(0) { arg1[0] = '\0'; }; 
};

struct Command {
	int cmd;
	uint16 value;
	Common::List<OpCode> opcodes;
	Command() : value(0) {};
};

struct Process {
	char name[30];
	Common::List<Command> commands;
};

struct Exit {
	int16 exit;
	int16 exitLoc;
	int16 exitBox;
};

struct Box {
	Box() : enabled(false) {}
	bool enabled;
	int16 x1, y1, x2, y2, priority;
	int32 z1, z2;
	uint8 attrib;
	int8 joins[6];
};

struct LocRoute {
	Exit exits[6];
	Box boxes[32];
};

struct CharScope {
	CharScope() : actorId(-1), screenH(0), screenDH(0), offset0c(0), offset10(0),
		offset14(262144), offset1c(0), offset20(262144), offset28(0), ratioX(262144),
		ratioY(262144), relativeSpeed(1024), direction(0), lastDirection(2),
		scopeInUse(-1), scopeWanted(8), priority(0), fightPartner(-1),
		spriteSceneState(0), spriteTimer(0), somethingX(0), somethingY(0) {}
	Scope scopes[18];
	int16 actorId;
	int16 screenX;
	int16 screenY;
	int16 screenH;
	int16 screenDH;
	int32 offset0c;
	int32 offset10;
	int32 offset14;
	int32 offset1c;
	int32 offset20;
	int32 offset28;
	int32 ratioX;
	int32 ratioY;
	int16 priority;
	int32 lastLocation;
	int32 lastBox;
	int32 gotoBox;
	int16 gotoX;
	int16 gotoY;
	int16 gotoLoc;
	uint16 walkSpeed;
	uint16 relativeSpeed;
	uint16 animSpeed;
	uint16 direction;
	uint16 lastDirection;
	bool stopped;
	uint16 timeout;
	int32 start3;
	int32 start3Prev;
	int32 start3PrevPrev;
	int32 start4;
	int32 start4Prev;
	int32 start4PrevPrev;
	int32 somethingX;
	int32 somethingY;
	int32 start5;
	int32 start5Prev;
	int32 start5PrevPrev;
	uint32 offset78;
	int16 fightPartner;
	uint8 spriteSceneState;
	uint16 spriteTimer;
	int16 scopeInUse;
	uint16 scopeWanted;
};

class Database {
public:
	Database(KomEngine *vm, OSystem *system);
	~Database();

	void init(Common::String databasePrefix);

	static void stripUndies(char *s);
	int CDECL readLineScanf(Common::File &f, const char *format, ...) KOM_GCC_SCANF(3, 4);

	int8 loc2loc(int from, int to) { return (int8)(_routes[(int8)(_routes[0]) * to + from + 1]); }
	int8 box2box(int loc, int fromBox, int toBox);

	Object *object(int obj) const { return &(_objects[obj]); }

	Box *getBox(int locId, int boxId) const { return &(_locRoutes[locId].boxes[boxId]); }
	Exit *getExits(int locId) const { return _locRoutes[locId].exits; }

	int8 whatBox(int locId, int x, int y);
	int8 whatBoxLinked(int locId, int8 boxId, int x, int y);
	int16 getMidX(int loc, int box) { return _locRoutes[loc].boxes[box].x1 +
		(_locRoutes[loc].boxes[box].x2 - _locRoutes[loc].boxes[box].x1) / 2; }
	int16 getMidY(int loc, int box) { return _locRoutes[loc].boxes[box].y1 +
		(_locRoutes[loc].boxes[box].y2 - _locRoutes[loc].boxes[box].y1) / 2; }
	int16 getMidOverlapX(int loc, int box1, int box2);
	int16 getMidOverlapY(int loc, int box1, int box2);
	int16 getPriority(int loc, int box) { return _locRoutes[loc].boxes[box].priority; }
	int16 getZValue(int loc, int box, int32 y);
	uint16 getExitBox(int currLoc, int nextLoc);
	bool isInLine(int loc, int box, int x, int y);

	void setCharPos(int charId, int loc, int box);
	bool giveObject(int charId, int obj, bool something);

	Process *getProc(uint16 procIndex) const { return procIndex < _procsNum ? &(_processes[procIndex]) : NULL; }
	Character *getChar(uint16 charIndex) const { return charIndex < _charactersNum ? &(_characters[charIndex]) : NULL; }
	Object *getObj(uint16 objIndex) const { return objIndex < _objectsNum ? &(_objects[objIndex]) : NULL; }
	Location *getLoc(uint16 locIndex) const { return locIndex < _locationsNum ? &(_locations[locIndex]) : NULL; }
	CharScope *getCharScope(uint16 scpIndex) const { return scpIndex < _charactersNum ? &(_charScopes[scpIndex]) : NULL; }

	int charactersNum() { return _charactersNum; }

	int16 getVar(uint16 index) { assert(index < _varSize); return _variables[index]; }
	void setVar(uint16 index, int16 value) { assert(index < _varSize); _variables[index] = value; }

private:
	void loadConvIndex();
	void initLocations();
	void initCharacters();
	void initObjects();
	void initEvents();
	void initObjectLocs();
	void initCharacterLocs();
	void initProcs();
	void initRoutes();
	void initScopes();

	OSystem *_system;
	KomEngine *_vm;

	Common::String _databasePrefix;

	Location *_locations;
	int _locationsNum;

	Character *_characters;
	int _charactersNum;

	Object *_objects;
	int _objectsNum;

	Process *_processes;
	int _procsNum;

	CharScope *_charScopes;

	int _varSize;
	int16 *_variables;

	int _convIndexSize;
	byte *_convIndex;

	int _mapSize;
	byte *_map;

	int _routesSize;
	byte *_routes;

	static const int _locRoutesSize;
	LocRoute *_locRoutes;
};
} // End of namespace Kom

#endif
