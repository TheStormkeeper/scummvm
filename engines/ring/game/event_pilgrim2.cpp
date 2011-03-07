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

#include "ring/game/event_pilgrim2.h"

#include "ring/base/application.h"

//using namespace Pilgrim2Game;

namespace Ring {

EventHandlerPilgrim2::EventHandlerPilgrim2(Application *application) : _app(application) {
}

EventHandlerPilgrim2::~EventHandlerPilgrim2() {
	// Zero-out passed pointers
	_app = NULL;
}

#pragma region Event handling

void EventHandlerPilgrim2::onMouseLeftButtonUp(Common::Event &evt, bool isControlPressed) {
	error("[EventHandlerPilgrim2::onMouseLeftButtonUp] Not implemented");
}

void EventHandlerPilgrim2::onMouseLeftButtonDown(Common::Event &evt) {
	error("[EventHandlerPilgrim2::onMouseLeftButtonDown] Not implemented");
}

void EventHandlerPilgrim2::onMouseRightButtonUp(Common::Event &evt) {
	error("[EventHandlerPilgrim2::onMouseRightButtonUp] Not implemented");
}

void EventHandlerPilgrim2::onKeyDown(Common::Event &evt) {
	error("[EventHandlerPilgrim2::onKeyDown] Not implemented");
}

#pragma endregion

#pragma region Zone Setup

void EventHandlerPilgrim2::onSetup(Zone zone, SetupType type) {
	error("[EventHandlerPilgrim2::onSetup] Not implemented");
}

#pragma endregion


#pragma region Timer

void EventHandlerPilgrim2::onTimer(TimerId timerId) {
	error("[EventHandlerPilgrim2::onTimer] Not implemented");
}

#pragma endregion

#pragma region Sound

void EventHandlerPilgrim2::onSound(Id id, SoundType type, uint32 a3) {
	error("[EventHandlerPilgrim2::onSound] Not implemented");
}

#pragma endregion

} // End of namespace Ring
