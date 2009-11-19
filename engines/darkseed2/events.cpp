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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include "common/events.h"

#include "engines/darkseed2/events.h"
#include "engines/darkseed2/graphics.h"
#include "engines/darkseed2/talk.h"

namespace DarkSeed2 {

Events::Events(DarkSeed2Engine &vm) : _vm(&vm) {
}

Events::~Events() {
}

void Events::mainLoop() {
	while (!_vm->shouldQuit()) {
		handleInput();

		g_system->delayMillis(10);

		_vm->_talkMan->updateStatus();

		_vm->_graphics->retrace();
		g_system->updateScreen();
	}
}

void Events::handleInput() {
	Common::Event event;

	while (g_system->getEventManager()->pollEvent(event)) {
		switch (event.type) {
		case Common::EVENT_LBUTTONDOWN:
			{
				TalkLine talkLine(*_vm->_resources, "DNA006");
				_vm->_talkMan->talk(talkLine);
			}
			break;
		default:
			break;

		}
	}
}

} // End of namespace DarkSeed2
