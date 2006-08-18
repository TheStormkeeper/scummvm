/* ScummVM - Scumm Interpreter
 * Copyright (C) 2004-2006 The ScummVM project
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

#include "common/stdafx.h"
#include "common/system.h"

#include "kom/screen.h"
#include "kom/kom.h"
#include "kom/actor.h"

namespace Kom {

Screen::Screen(KomEngine *vm, OSystem *system)
	: _system(system), _vm(vm) {

	_screenBuf = new uint8[SCREEN_W * SCREEN_H];
	memset(_screenBuf, 0, SCREEN_W * SCREEN_H);
}

Screen::~Screen() {
	delete[] _screenBuf;
}

bool Screen::init() {
	_system->beginGFXTransaction();
		_vm->initCommonGFX(false);
		_system->initSize(320, 200);
	_system->endGFXTransaction();

	return true;
}

void Screen::update() {
	_vm->actorMan()->displayAll();
	_system->copyRectToScreen(_screenBuf, SCREEN_W, 0, 0, SCREEN_W, SCREEN_H);
	_system->updateScreen();
}

} // End of namespace Kom
