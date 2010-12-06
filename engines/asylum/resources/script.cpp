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

#include "asylum/resources/script.h"

#include "asylum/resources/actor.h"
#include "asylum/resources/encounters.h"
#include "asylum/resources/object.h"
#include "asylum/resources/worldstats.h"

#include "asylum/system/config.h"
#include "asylum/system/cursor.h"
#include "asylum/system/graphics.h"
#include "asylum/system/screen.h"
#include "asylum/system/speech.h"

#include "asylum/views/menu.h"
#include "asylum/views/scene.h"

#include "asylum/asylum.h"
#include "asylum/staticres.h"

#include "common/rational.h"

namespace Asylum {

//////////////////////////////////////////////////////////////////////////
// ActionArea
//////////////////////////////////////////////////////////////////////////
void ActionArea::load(Common::SeekableReadStream *stream) {
	stream->read(&name, 52);
	id             = stream->readSint32LE();
	field01        = stream->readSint32LE();
	field02        = stream->readSint32LE();
	field_40       = stream->readSint32LE();
	field_44       = stream->readSint32LE();
	flags          = stream->readSint32LE();
	scriptIndex    = stream->readSint32LE();
	scriptIndex2   = stream->readSint32LE();
	actionType     = stream->readSint32LE();

	for (int32 i = 0; i < 10; i++)
		flagNums[i] = stream->readSint32LE();

	field_7C          = stream->readSint32LE();
	polygonIndex           = stream->readSint32LE();
	field_84          = stream->readSint32LE();
	field_88          = stream->readSint32LE();
	soundResourceId   = (ResourceId)stream->readSint32LE();
	field_90          = stream->readSint32LE();
	paletteResourceId = (ResourceId)stream->readSint32LE();

	for (int32 i = 0; i < 5; i++)
		array[i] = stream->readSint32LE();

	volume = stream->readSint32LE();
}

//////////////////////////////////////////////////////////////////////////
// ScriptManager
//////////////////////////////////////////////////////////////////////////
ScriptManager::ScriptManager(AsylumEngine *engine) : _vm(engine) {
	// Build list of opcodes
	ADD_OPCODE(Return);
	ADD_OPCODE(SetGameFlag);
	ADD_OPCODE(ClearGameFlag);
	ADD_OPCODE(ToggleGameFlag);
	ADD_OPCODE(JumpIfGameFlag);
	ADD_OPCODE(HideCursor);
	ADD_OPCODE(ShowCursor);
	ADD_OPCODE(PlayAnimation);
	ADD_OPCODE(MoveScenePosition);
	ADD_OPCODE(HideActor);
	ADD_OPCODE(ShowActor);
	ADD_OPCODE(SetActorPosition);
	ADD_OPCODE(SetSceneMotionStatus);
	ADD_OPCODE(DisableActor);
	ADD_OPCODE(EnableActor);
	ADD_OPCODE(EnableObjects);
	ADD_OPCODE(Return);
	ADD_OPCODE(RemoveObject);
	ADD_OPCODE(JumpActorSpeech);
	ADD_OPCODE(JumpAndSetDirection);
	ADD_OPCODE(JumpIfActorCoordinates);
	ADD_OPCODE(Nop);
	ADD_OPCODE(ResetAnimation);
	ADD_OPCODE(DisableObject);
	ADD_OPCODE(JumpIfSoundPlayingAndPlaySound);
	ADD_OPCODE(JumpIfActionFind);
	ADD_OPCODE(SetActionFind);
	ADD_OPCODE(ClearActionFind);
	ADD_OPCODE(JumpIfActionGrab);
	ADD_OPCODE(SetActionGrab);
	ADD_OPCODE(ClearActionGrab);
	ADD_OPCODE(JumpIfActionTalk);
	ADD_OPCODE(SetActionTalk);
	ADD_OPCODE(ClearActionTalk);
	ADD_OPCODE(_unk22);
	ADD_OPCODE(_unk23);
	ADD_OPCODE(_unk24);
	ADD_OPCODE(RunEncounter);
	ADD_OPCODE(JumpIfAction16);
	ADD_OPCODE(SetAction16);
	ADD_OPCODE(ClearAction16);
	ADD_OPCODE(SetActorField638);
	ADD_OPCODE(JumpIfActorField638);
	ADD_OPCODE(ChangeScene);
	ADD_OPCODE(_unk2C_ActorSub);
	ADD_OPCODE(PlayMovie);
	ADD_OPCODE(StopAllObjectsSounds);
	ADD_OPCODE(StopProcessing);
	ADD_OPCODE(ResumeProcessing);
	ADD_OPCODE(ResetSceneRect);
	ADD_OPCODE(ChangeMusicById);
	ADD_OPCODE(StopMusic);
	ADD_OPCODE(_unk34_Status);
	ADD_OPCODE(SetVolume);
	ADD_OPCODE(Jump);
	ADD_OPCODE(RunBlowUpPuzzle);
	ADD_OPCODE(JumpIfAction8);
	ADD_OPCODE(SetAction8);
	ADD_OPCODE(ClearAction8);
	ADD_OPCODE(_unk3B_PALETTE_MOD);
	ADD_OPCODE(IncrementParam2);
	ADD_OPCODE(WaitUntilFramePlayed);
	ADD_OPCODE(UpdateWideScreen);
	ADD_OPCODE(JumpIfActor);
	ADD_OPCODE(PlaySpeechScene);
	ADD_OPCODE(PlaySpeech);
	ADD_OPCODE(PlaySpeechScene2);
	ADD_OPCODE(MoveScenePositionFromActor);
	ADD_OPCODE(PaletteFade);
	ADD_OPCODE(StartPaletteFadeThread);
	ADD_OPCODE(_unk46);
	ADD_OPCODE(ActorFaceObject);
	ADD_OPCODE(_unk48_MATTE_01);
	ADD_OPCODE(_unk49_MATTE_90);
	ADD_OPCODE(JumpIfSoundPlaying);
	ADD_OPCODE(ChangePlayerActorIndex);
	ADD_OPCODE(ChangeActorStatus);
	ADD_OPCODE(StopSound);
	ADD_OPCODE(JumpRandom);
	ADD_OPCODE(ClearScreen);
	ADD_OPCODE(Quit);
	ADD_OPCODE(JumpObjectFrame);
	ADD_OPCODE(DeleteGraphics);
	ADD_OPCODE(DeleteGraphics);
	ADD_OPCODE(_unk54_SET_ACTIONLIST_6EC);
	ADD_OPCODE(_unk55);
	ADD_OPCODE(_unk56);
	ADD_OPCODE(SetResourcePalette);
	ADD_OPCODE(SetObjectFrameIdxFlaged);
	ADD_OPCODE(_unk59);
	ADD_OPCODE(_unk5A);
	ADD_OPCODE(_unk5B);
	ADD_OPCODE(QueueScript);
	ADD_OPCODE(_unk5D);
	ADD_OPCODE(ClearActorFields);
	ADD_OPCODE(SetObjectLastFrameIdx);
	ADD_OPCODE(_unk60_SET_OR_CLR_ACTIONAREA_FLAG);
	ADD_OPCODE(_unk61);
	ADD_OPCODE(ShowOptionsScreen);
	ADD_OPCODE(_unk63);

	reset();
}

ScriptManager::~ScriptManager() {
	for (int i = 0; i < (int)_opcodes.size(); i++)
		delete _opcodes[i];

	_scripts.clear();
	_queue.clear();

	_currentScript = NULL;

	// Zero-out passed pointers
	_vm = NULL;
}

void ScriptManager::load(Common::SeekableReadStream *stream) {
	stream->readSint32LE();  // size
	int32 numEntries = stream->readSint32LE();

	for (int32 a = 0; a < numEntries; a++) {
		Script script;
		memset(&script, 0, sizeof(Script));

		for (int32 c = 0; c < MAX_ACTION_COMMANDS; c++) {
			ScriptEntry command;
			memset(&command, 0, sizeof(ScriptEntry));

			command.numLines = stream->readSint32LE();
			command.opcode   = stream->readSint32LE();
			command.param1   = stream->readSint32LE();
			command.param2   = stream->readSint32LE();
			command.param3   = stream->readSint32LE();
			command.param4   = stream->readSint32LE();
			command.param5   = stream->readSint32LE();
			command.param6   = stream->readSint32LE();
			command.param7   = stream->readSint32LE();
			command.param8   = stream->readSint32LE();
			command.param9   = stream->readSint32LE();

			script.commands[c] = command;
		}

		script.field_1BAC = stream->readSint32LE();
		script.field_1BB0 = stream->readSint32LE();
		script.counter    = stream->readSint32LE();

		_scripts.push_back(script);
	}
}

void ScriptManager::reset() {
	// Reset script queue
	resetQueue();

	// Remove all scripts
	_scripts.clear();

	_skipProcessing    = false;
	_currentLine       = 0;
	_currentLoops      = 0;
	_currentScript     = NULL;
	_delayedSceneIndex = kResourcePackInvalid;
	_delayedVideoIndex = -1;
	_done = false;
	_exit = false;
	_lineIncrement = 0;
	_waitCycle = false;
}

void ScriptManager::resetQueue() {
	_queue.clear();
}

void ScriptManager::queueScript(int scriptIndex, ActorIndex actorIndex) {
	// When the skipProcessing flag is set, do not queue any more scripts
	if (_skipProcessing)
		return;

	ScriptQueueEntry entry;
	entry.scriptIndex = scriptIndex;
	entry.actorIndex  = actorIndex;

	// If there's currently no script for the processor to run,
	// assign it directly and skip the stack push. If however the
	// current script is assigned, push the script to the stack
	if (_currentScript)
		_queue.push(entry);
	else {
		_currentQueueEntry = entry;
		_currentScript     = &_scripts[entry.scriptIndex];
	}
}

bool ScriptManager::isInQueue(int32 scriptIndex) {
	for (int32 i = 0; i < _queue.size(); i++) {
		if (_queue[i].scriptIndex == scriptIndex)
			return true;
	}

	return false;
}

bool ScriptManager::process() {
	_done          = false;
	_exit          = false;
	_waitCycle     = false;
	_lineIncrement = 1;

	_vm->setGameFlag(kGameFlagScriptProcessing);

	if (_currentScript) {
		while (!_done && !_waitCycle) {
			_lineIncrement = 0; //Reset line increment value

			ScriptEntry *cmd = &_currentScript->commands[_currentLine];

			int32 opcode = cmd->opcode;

			debugC(kDebugLevelScripts, "[0x%02X] %s (%d, %d, %d, %d, %d, %d, %d, %d, %d)",
			       opcode, _opcodes[opcode]->name,
			       cmd->param1, cmd->param2, cmd->param3, cmd->param4, cmd->param5,
			       cmd->param6, cmd->param7, cmd->param8, cmd->param9);

			// Execute opcode
			(*_opcodes[opcode]->func)(cmd);

			if (_exit)
				return true;

			if (!_lineIncrement)
				_currentLine ++;
		}

		if (_done) {
			_currentLine  = 0;

			if (!_queue.empty()) {
				_currentQueueEntry = _queue.pop();
				_currentScript     = &_scripts[_currentQueueEntry.scriptIndex];
			} else {
				_currentScript = NULL;
			}
		}
	}

	_vm->clearGameFlag(kGameFlagScriptProcessing);

	return false;
}

//////////////////////////////////////////////////////////////////////////
// Opcode Functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Opcode 0x00
IMPLEMENT_OPCODE(Return)
	_done          = true;
	_lineIncrement = 0;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x01
IMPLEMENT_OPCODE(SetGameFlag)
	int32 flagNum = cmd->param1;

	if (flagNum >= 0)
		_vm->setGameFlag((GameFlag)flagNum);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x02
IMPLEMENT_OPCODE(ClearGameFlag)
	int32 flagNum = cmd->param1;

	if (flagNum >= 0)
		_vm->clearGameFlag((GameFlag)flagNum);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x03
IMPLEMENT_OPCODE(ToggleGameFlag)
	int32 flagNum = cmd->param1;

	if (flagNum >= 0)
		_vm->toggleGameFlag((GameFlag)flagNum);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x04
IMPLEMENT_OPCODE(JumpIfGameFlag)
	if (cmd->param1 < 0)
		return;

	bool doJump = (cmd->param2) ? _vm->isGameFlagSet((GameFlag)cmd->param1) : _vm->isGameFlagNotSet((GameFlag)cmd->param1);
	if (!doJump)
		return;

	setNextLine(cmd->param3);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x05
IMPLEMENT_OPCODE(HideCursor)
	getCursor()->hide();
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x06
IMPLEMENT_OPCODE(ShowCursor)
	getCursor()->show();

	getSharedData()->setFlag(kFlag1, false);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x07
IMPLEMENT_OPCODE(PlayAnimation)
	Object *object = getWorld()->getObjectById((ObjectId)cmd->param1);

	if (cmd->param2 == 2) {
		if (object->checkFlags())
			_lineIncrement = 1;
		else
			cmd->param2 = 1;

		return;
	}

	// Update flags
	if (cmd->param4) {
		object->flags &= ~kObjectFlag10E38;
        object->flags |= kObjectFlag20;
	} else if (cmd->param3) {
		object->flags &= ~kObjectFlag10E38;
		object->flags |= kObjectFlag10000;
	} else if (object->flags & kObjectFlag10000) {
		object->flags |= kObjectFlag8;
		object->flags &= ~kObjectFlag10000;
	} else if (!(object->flags & kObjectFlag10E38)) {
		object->flags |= kObjectFlag8;
	}

	object->setNextFrame(object->flags);

	if (object->getField688() == 1) {
		if (object->flags & kObjectFlag4) {
			getSharedData()->getPoint()->x = object->x;
			getSharedData()->getPoint()->y = object->y;
		} else {
			Common::Rect frameRect = GraphicResource::getFrameRect(_vm, object->getResourceId(), object->getFrameIndex());

			getSharedData()->getPoint()->x = frameRect.left + Common::Rational(frameRect.width(), 2).toInt()  + object->x;
			getSharedData()->getPoint()->y = frameRect.top  + Common::Rational(frameRect.height(), 2).toInt() + object->y;
		}
	}

	if (cmd->param2) {
		cmd->param2 = 2;
		_lineIncrement = 1;
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x08
IMPLEMENT_OPCODE(MoveScenePosition)
	if (cmd->param3 < 1) {
		getWorld()->xLeft = cmd->param1;
		getWorld()->yTop  = cmd->param2;
		getWorld()->motionStatus = 3;

	} else if (!cmd->param4) {
		getWorld()->motionStatus = 5;

		getScene()->updateSceneCoordinates(cmd->param1,
			                           cmd->param2,
		                               cmd->param3);

	} else if (cmd->param5) {
		if (getWorld()->motionStatus == 2)
			_lineIncrement = 1;
		else
			cmd->param5 = 0;
	} else {
		cmd->param5 = 1;
		getWorld()->motionStatus = 2;

		getScene()->updateSceneCoordinates(cmd->param1,
		                               cmd->param2,
		                               cmd->param3,
		                               true);

		_lineIncrement = 1;
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x09
IMPLEMENT_OPCODE(HideActor)
	Actor *actor = getScene()->getActor(cmd->param1);

	actor->hide();
	actor->updateDirection();
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x0A
IMPLEMENT_OPCODE(ShowActor)
	Actor *actor = getScene()->getActor(cmd->param1);

	actor->show();
	actor->updateDirection();
	actor->setLastScreenUpdate(_vm->getTick());
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x0B
IMPLEMENT_OPCODE(SetActorPosition)
	Actor *actor = getScene()->getActor(cmd->param1);

	actor->setPosition(cmd->param2, cmd->param3, (ActorDirection)cmd->param4, (uint32)cmd->param5);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x0C
IMPLEMENT_OPCODE(SetSceneMotionStatus)
	getWorld()->motionStatus = cmd->param1;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x0D
IMPLEMENT_OPCODE(DisableActor)
	Actor *actor = getScene()->getActor(cmd->param1);

	actor->updateStatus(kActorStatusDisabled);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x0E
IMPLEMENT_OPCODE(EnableActor)
	Actor *actor = getScene()->getActor(cmd->param1);

	if (actor->getStatus() == kActorStatusDisabled)
		actor->enable();
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x0F
IMPLEMENT_OPCODE(EnableObjects)
	Object *object = getWorld()->getObjectById((ObjectId)cmd->param1);

	if (!_currentScript->counter && getWorld()->chapter != 13)
		_vm->sound()->playSound(cmd->param3 ? MAKE_RESOURCE(kResourcePackSound, 6) : MAKE_RESOURCE(kResourcePackSound, 1));

	if (_currentScript->counter >= (3 * cmd->param2 - 1)) {
		_currentScript->counter = 0;
		object->setField67C(0);
		enableObject(cmd, kObjectEnableType2);
	} else {
		++_currentScript->counter;
		if (cmd->param3) {
			object->setField67C(3 - _currentScript->counter / cmd->param2);
			enableObject(cmd, kObjectEnableType1);
		} else {
			object->setField67C(_currentScript->counter / cmd->param2 + 1);
			enableObject(cmd, kObjectEnableType0);
		}

		_lineIncrement = 1;
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x10 : Identical to opcode 0x00

//////////////////////////////////////////////////////////////////////////
// Opcode 0x11
IMPLEMENT_OPCODE(RemoveObject)
	if (!cmd->param1)
		return;

	Object *object = getWorld()->getObjectById((ObjectId)cmd->param1);

	object->disableAndRemoveFromQueue();
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x12
IMPLEMENT_OPCODE(JumpActorSpeech)
	Actor *actor = getScene()->getActor(cmd->param1);

	if (actor->process(cmd->param2, cmd->param3))
		return;

	_currentLine = cmd->param4;

	if (cmd->param5)
		getSpeech()->playIndexed(1);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x13
IMPLEMENT_OPCODE(JumpAndSetDirection)
	Actor *actor = getScene()->getActor(cmd->param1);

	if (actor->getStatus() != kActorStatus2 && actor->getStatus() != kActorStatus13) {
		if (cmd->param5 != 2) {

			if (cmd->param2 == -1 || cmd->param3 == -1) {
				actor->updateFromDirection((ActorDirection)cmd->param4);
			} else if ((actor->getPoint1()->x + actor->getPoint2()->x) == cmd->param2 && (actor->getPoint1()->y + actor->getPoint2()->y) == cmd->param3) {
				actor->updateFromDirection((ActorDirection)cmd->param4);
			} else {
				actor->processStatus(cmd->param2, cmd->param3, (bool)cmd->param4);

				if (cmd->param5 == 1) {
					cmd->param5 = 2;
					_lineIncrement = 1;
				}
			}
		} else {
			cmd->param5 = 1;
			_lineIncrement = 0;

			if ((actor->getPoint1()->x + actor->getPoint2()->x) == cmd->param2 && (actor->getPoint1()->y + actor->getPoint2()->y) == cmd->param3)
				actor->updateFromDirection((ActorDirection)cmd->param4);
		}
	} else {
		if (cmd->param5 == 2)
			_lineIncrement = 1;
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x14
IMPLEMENT_OPCODE(JumpIfActorCoordinates)
	Actor *actor = getScene()->getActor(cmd->param1);

	if ((actor->getPoint1()->x + actor->getPoint2()->x) != cmd->param2 || (actor->getPoint1()->y + actor->getPoint2()->y) != cmd->param3)
		_lineIncrement = cmd->param4;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x15
IMPLEMENT_OPCODE(Nop)
	// Nothing to do
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x16
IMPLEMENT_OPCODE(ResetAnimation)
	Object *object = getWorld()->getObjectById((ObjectId)cmd->param1);

	if (object->flags & kObjectFlag10000)
		object->setFrameIndex(object->getFrameCount() - 1);
	else
		object->setFrameIndex(0);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x17
IMPLEMENT_OPCODE(DisableObject)
	Object *object = getWorld()->getObjectById((ObjectId)cmd->param1);

	object->disable();
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x18
IMPLEMENT_OPCODE(JumpIfSoundPlayingAndPlaySound)
	ResourceId resource = (ResourceId)cmd->param1;

	if (cmd->param2 == 2) {
		if (getSound()->isPlaying(resource))
			_lineIncrement = 1;
		else
			cmd->param2 = 1;
	} else if (!_vm->sound()->isPlaying(resource)) {
		int32 vol = getSound()->getAdjustedVolume(abs(Config.sfxVolume));
		getSound()->playSound(resource, (bool)cmd->param4, -((abs(cmd->param3) + vol) * (abs(cmd->param3) + vol)), 0);

		if (cmd->param2 == 1) {
			cmd->param2 = 2;
			_lineIncrement= 1;
		}
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x19
IMPLEMENT_OPCODE(JumpIfActionFind)
	jumpIfActionFlag(cmd, kActionTypeFind);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x1A
IMPLEMENT_OPCODE(SetActionFind)
	setActionFlag(cmd, kActionTypeFind);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x1B
IMPLEMENT_OPCODE(ClearActionFind)
	clearActionFlag(cmd, kActionTypeFind);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x1C
IMPLEMENT_OPCODE(JumpIfActionGrab)
	jumpIfActionFlag(cmd, kActionTypeGrab);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x1D
IMPLEMENT_OPCODE(SetActionGrab)
	setActionFlag(cmd, kActionTypeGrab);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x1E
IMPLEMENT_OPCODE(ClearActionGrab)
	clearActionFlag(cmd, kActionTypeGrab);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x1F
IMPLEMENT_OPCODE(JumpIfActionTalk)
	jumpIfActionFlag(cmd, kActionTypeTalk);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x20
IMPLEMENT_OPCODE(SetActionTalk)
	setActionFlag(cmd, kActionTypeTalk);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x21
IMPLEMENT_OPCODE(ClearActionTalk)
	clearActionFlag(cmd, kActionTypeTalk);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x22
IMPLEMENT_OPCODE(_unk22)
	Actor *actor = getScene()->getActor(cmd->param3 ? cmd->param3 : _currentQueueEntry.actorIndex);

	actor->process_41BC00(cmd->param1, cmd->param2);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x23
IMPLEMENT_OPCODE(_unk23)
	Actor *actor = getScene()->getActor(cmd->param3 ? cmd->param3 : _currentQueueEntry.actorIndex);

	actor->process_41BCC0(cmd->param1, cmd->param2);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x24
IMPLEMENT_OPCODE(_unk24)
	Actor *actor = getScene()->getActor(cmd->param4 ? cmd->param4 : _currentQueueEntry.actorIndex);

	actor->process_41BDB0(cmd->param1, (bool)cmd->param3);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x25
IMPLEMENT_OPCODE(RunEncounter)
	Encounter *encounter = _vm->encounter();

	encounter->disablePlayerOnExit(cmd->param5);

	if (cmd->param6) {
		if (encounter->isRunning())
			_lineIncrement = 1;
		else
			cmd->param6 = 0;
	} else {
		encounter->run(cmd->param1, (ObjectId)cmd->param2, (ObjectId)cmd->param3, cmd->param4);

		cmd->param6 = 2;
		_lineIncrement = 1;
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x26
IMPLEMENT_OPCODE(JumpIfAction16)
	jumpIfActionFlag(cmd, kActionType16);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x27
IMPLEMENT_OPCODE(SetAction16)
	setActionFlag(cmd, kActionType16);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x28
IMPLEMENT_OPCODE(ClearAction16)
	clearActionFlag(cmd, kActionType16);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x29
IMPLEMENT_OPCODE(SetActorField638)
	Actor *actor = getScene()->getActor(cmd->param1);

	actor->setField638(cmd->param2);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x2A
IMPLEMENT_OPCODE(JumpIfActorField638)
	Actor *actor = getScene()->getActor(cmd->param1);

	if (actor->getField638())
		_currentLine = cmd->param3;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x2B
IMPLEMENT_OPCODE(ChangeScene)
	getScene()->getActor(0)->updateStatus(kActorStatusDisabled);
	resetQueue();

	// Fade screen to black
	getScreen()->paletteFade(0, 75, 8);
	getScreen()->clear();

	// Stop all sounds & music
	getSound()->stopAll();
	getSound()->stopMusic();

	// Change the scene number
	_delayedSceneIndex = (ResourcePackId)(cmd->param1 + 4);

	_exit = true;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x2C
IMPLEMENT_OPCODE(_unk2C_ActorSub)
	Actor *player = getScene()->getActor();
	Actor *actor = getScene()->getActor(_currentQueueEntry.actorIndex);
	Common::Point playerPoint((int16)(player->getPoint1()->x + player->getPoint2()->x), (int16)(player->getPoint1()->y + player->getPoint2()->y));
	ActorDirection direction = (cmd->param2 == 8) ? player->getDirection() : (ActorDirection)cmd->param2;

	ActorDirection newDirection = (ActorDirection)((player->getDirection() + 4) % 8);

	if (cmd->param2 == 8)
		cmd->param2 = player->getDirection();

	if (cmd->param3 == 2) {
		switch (actor->getStatus()) {
		default:
			_lineIncrement = 1;
			return;

		case kActorStatus7:
			actor->enable();
			break;

		case kActorStatus16:
			// We want to continue processing and not go into the default case
			break;

		case kActorStatus20:
			actor->updateStatus(kActorStatus14);
		}

		cmd->param3 = 0;

		if (cmd->param1 == 2) {
			Common::Point point(playerPoint);

			if (player->process_408B20(&point, newDirection, 3, false)) {

				point.x += (int16)(3 * deltaPointsArray[newDirection].x);
				point.y += (int16)(3 * deltaPointsArray[newDirection].y);

				player->setPosition(point.x, point.y, actor->getDirection(), 0);
			}
		}

	} else if (cmd->param1 != 2 || player->process_408B20(&playerPoint, newDirection, 3, false)) {
		ResourceId id = kResourceNone;
		if (direction >= 5)
			id = actor->getResourcesId((uint32)(5 * cmd->param1 - (direction - 38)));
		else
			id = actor->getResourcesId((uint32)(5 * cmd->param1 + direction + 30));

		actor->setResourceId(id);
		actor->setFrameCount(GraphicResource::getFrameCount(_vm, id));
		actor->setFrameIndex(0);
		actor->setDirection(direction);
		actor->updateStatus(actor->getStatus() <= kActorStatus11 ? kActorStatus3 : kActorStatus19);

		cmd->param3 = 2;
		_lineIncrement = 1;
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x2D
IMPLEMENT_OPCODE(PlayMovie)
	if (getSharedData()->getMatteBarHeight() < 170) {
		_lineIncrement = 1;

		if (!getSharedData()->getMatteBarHeight()) {
			getCursor()->hide();
			getScreen()->makeGreyPalette();
			getSharedData()->setMatteVar1(1);
			getSharedData()->setMatteBarHeight(1);
			getSharedData()->setMatteVar2(0);
			getSharedData()->setMattePlaySound(cmd->param3 == 0);
			getSharedData()->setMatteInitialized(cmd->param2 == 0);
			_delayedVideoIndex = cmd->param1;
		}

		return;
	}

	bool check = false;
	ActionArea *area = getWorld()->actions[getScene()->getActor()->getActionIndex3()];
	if (area->paletteResourceId) {
		getScreen()->setPalette(area->paletteResourceId);
		getScreen()->setGammaLevel(area->paletteResourceId, 0);
	} else {
		getScreen()->setPalette(getWorld()->currentPaletteId);
		getScreen()->setGammaLevel(getWorld()->currentPaletteId, 0);
	}

	getSharedData()->setMatteBarHeight(0);
	_lineIncrement = 0;

	if (!getSharedData()->getMattePlaySound() && _currentScript->commands[0].numLines != 0) {
		bool found = true;
		int index = 0;


		while (_currentScript->commands[index].opcode != 0x2B) { // ChangeScene
			++index;

			if (index >= _currentScript->commands[0].numLines) {
				found = false;
				break;
			}
		}

		if (found)
			check = true;
	}

	if (!check && getSharedData()->getMatteVar2() == 0 && getWorld()->musicCurrentResourceIndex != kMusicStopped)
		_vm->sound()->playMusic(MAKE_RESOURCE(kResourcePackMusic, getWorld()->musicCurrentResourceIndex));

	getCursor()->show();
	getSharedData()->setMatteVar2(0);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x2E
IMPLEMENT_OPCODE(StopAllObjectsSounds)
	Object *object = getWorld()->getObjectById((ObjectId)cmd->param1);

	object->stopAllSounds();
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x2F
IMPLEMENT_OPCODE(StopProcessing)
	_skipProcessing = true;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x30
IMPLEMENT_OPCODE(ResumeProcessing)
	_skipProcessing = false;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x31
IMPLEMENT_OPCODE(ResetSceneRect)
	getWorld()->sceneRectIdx = (uint8)LOBYTE(cmd->param1);
	getScreen()->paletteFade(0, 25, 10);
	getSharedData()->setFlag(kFlagSceneRectChanged, true);

	getWorld()->xLeft = getWorld()->sceneRects[getWorld()->sceneRectIdx].left;
	getWorld()->yTop  = getWorld()->sceneRects[getWorld()->sceneRectIdx].top;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x32
IMPLEMENT_OPCODE(ChangeMusicById)
	_vm->sound()->changeMusic((ResourceId)cmd->param1, cmd->param2 ? 2 : 1);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x33
IMPLEMENT_OPCODE(StopMusic)
	_vm->sound()->changeMusic(kMusicStopped, 0);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x34
IMPLEMENT_OPCODE(_unk34_Status)
	if (cmd->param1 >= 2) {
		cmd->param1 = 0;
	} else {
		cmd->param1++;
		_lineIncrement = 1;
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x35
IMPLEMENT_OPCODE(SetVolume)
	AmbientSoundItem item = getWorld()->ambientSounds[cmd->param1];
	int var = cmd->param2 + item.delta;

	int32 volume = -((Config.sfxVolume + var) * (Config.ambientVolume + var));

	if (volume < 0) {
		if (volume < -10000)
			volume = -10000;
	} else {
		volume = 0;
	}

	_vm->sound()->setVolume(item.resourceId, volume);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x36
IMPLEMENT_OPCODE(Jump)
	_currentLine = cmd->param1 - 1;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x37
IMPLEMENT_OPCODE(RunBlowUpPuzzle)
	getScreen()->clear();
	getScreen()->clearGraphicsInQueue();

	_vm->switchEventHandler(_vm->getPuzzle((uint32)cmd->param1));

	_currentLine++;

	_exit = true;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x38
IMPLEMENT_OPCODE(JumpIfAction8)
	jumpIfActionFlag(cmd, kActionType8);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x39
IMPLEMENT_OPCODE(SetAction8)
	setActionFlag(cmd, kActionType8);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x3A
IMPLEMENT_OPCODE(ClearAction8)
	clearActionFlag(cmd, kActionType8);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x3B
IMPLEMENT_OPCODE(_unk3B_PALETTE_MOD)
	if (!cmd->param2) {
		getScreen()->makeGreyPalette();
		cmd->param2 = 1;
	}

	if (cmd->param1 >= 22) {
		getScreen()->clear();

		cmd->param1 = 0;
		cmd->param2 = 0;
		_currentLine++;

		getScreen()->clearGraphicsInQueue();

		_exit = true;
		return;
	}

	getScreen()->updatePalette(cmd->param1);

	_lineIncrement = 1;
	++cmd->param1;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x3C
IMPLEMENT_OPCODE(IncrementParam2)
	if (cmd->param1) {
		if (cmd->param2 >= cmd->param1) {
			cmd->param2 = 0;
		} else {
			++cmd->param2;
			_lineIncrement = 1;
		}
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x3D
IMPLEMENT_OPCODE(WaitUntilFramePlayed)
	Object *object = getWorld()->getObjectById((ObjectId)cmd->param1);

	int32 frameNum = cmd->param2;
	if (frameNum == -1)
		frameNum = object->getFrameCount() - 1;

	if ((int32)object->getFrameIndex() != frameNum) {
		_lineIncrement = 1;
		_waitCycle     = true;
	}
END_OPCODE


//////////////////////////////////////////////////////////////////////////
// Opcode 0x3E
IMPLEMENT_OPCODE(UpdateWideScreen)
	int barSize = cmd->param1;

	if (barSize >= 22) {
		cmd->param1 = 0;
		_lineIncrement = 0;

		getSharedData()->setMatteBarHeight(0);
	} else {
		getScreen()->drawWideScreenBars((int16)(4 * barSize));

		_lineIncrement = 1;
		++cmd->param1;
	}
END_OPCODE


//////////////////////////////////////////////////////////////////////////
// Opcode 0x3F
IMPLEMENT_OPCODE(JumpIfActor)
	ActorIndex index = (cmd->param1 == kActorInvalid) ? getScene()->getPlayerIndex() : cmd->param1;

	if (_currentQueueEntry.actorIndex != index)
		_currentLine = cmd->param2 - 1;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x40
IMPLEMENT_OPCODE(PlaySpeechScene)
	if (cmd->param1 < 0 || cmd->param1 >= 5)
		return;

	if (cmd->param4 != 2) {
		cmd->param5 = (int32)getSpeech()->playIndexed((ResourceId)cmd->param1);

		if (cmd->param2) {
			_vm->setGameFlag(kGameFlagScriptProcessing);
			cmd->param4 = 2;
			if (cmd->param6) {
				getSharedData()->setFlag(kFlag1, true);
				getSharedData()->setFlag(kFlag2, true);
			}
			_lineIncrement = 1;
		}

		if (cmd->param3 && !cmd->param6)
			_vm->setGameFlag(kGameFlag219);

		return;
	}

	if (_vm->sound()->isPlaying((ResourceId)cmd->param5)) {
		_lineIncrement = 1;
		return;
	}

	_vm->clearGameFlag(kGameFlagScriptProcessing);
	cmd->param4 = 0;

	if (cmd->param3) {
		if (cmd->param6) {
			getSharedData()->setFlag(kFlag1, false);
			getSharedData()->setFlag(kFlag2, false);

			return;
		} else {
			_vm->clearGameFlag(kGameFlag219);
		}
	}

	if (!cmd->param6) {
		cmd->param6 = 1;
	} else {
		getSharedData()->setFlag(kFlag1, false);
		getSharedData()->setFlag(kFlag2, false);
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x41
IMPLEMENT_OPCODE(PlaySpeech)
	if (cmd->param1 < 0)
		return;

	if (cmd->param4 != 2) {
		cmd->param5 = (int32)getSpeech()->playPlayer((ResourceId)cmd->param1);

		if (cmd->param2) {
			_vm->setGameFlag(kGameFlagScriptProcessing);
			cmd->param4 = 2;
			if (cmd->param6) {
				getSharedData()->setFlag(kFlag1, true);
				getSharedData()->setFlag(kFlag2, true);
			}
			_lineIncrement = 1;
		}

		if (cmd->param3 && !cmd->param6)
			_vm->setGameFlag(kGameFlag219);

		return;
	}

	if (_vm->sound()->isPlaying((ResourceId)cmd->param5)) {
		_lineIncrement = 1;
		return;
	}

	_vm->clearGameFlag(kGameFlagScriptProcessing);
	cmd->param4 = 0;

	if (cmd->param3) {
		if (cmd->param6) {
			getSharedData()->setFlag(kFlag1, false);
			getSharedData()->setFlag(kFlag2, false);

			return;
		} else {
			_vm->clearGameFlag(kGameFlag219);
		}
	}

	if (!cmd->param6) {
		cmd->param6 = 1;
	} else {
		getSharedData()->setFlag(kFlag1, false);
		getSharedData()->setFlag(kFlag2, false);
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x42
IMPLEMENT_OPCODE(PlaySpeechScene2)
	if (cmd->param1 < 0 || cmd->param1 >= 20 || cmd->param2 < 0)
		return;

	if (cmd->param5 == 2) {
		if (getSound()->isPlaying((ResourceId)cmd->param6)) {
			_lineIncrement = 1;
			return;
		}

		_vm->clearGameFlag(kGameFlagScriptProcessing);

		cmd->param5 = 0;

		if (!cmd->param4) {
			if (!cmd->param7) {
				cmd->param7 = 1;
				return;
			}
		} if (!cmd->param7) {
			_vm->clearGameFlag(kGameFlag219);
			cmd->param7 = 1;
			return;
		}

		getSharedData()->setFlag(kFlag1, false);
		getSharedData()->setFlag(kFlag2, false);
		return;
	}

	cmd->param6 = (int32)getSpeech()->playScene(cmd->param1, cmd->param2);

	if (cmd->param3) {
		_vm->setGameFlag(kGameFlagScriptProcessing);

		cmd->param5 = 2;

		if (cmd->param7) {
			getSharedData()->setFlag(kFlag1, false);
			getSharedData()->setFlag(kFlag2, false);
		}

		_lineIncrement = 1;
	}

	if (cmd->param4 && !cmd->param7)
		_vm->setGameFlag(kGameFlag219);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x43
IMPLEMENT_OPCODE(MoveScenePositionFromActor)
	Actor *actor = getScene()->getActor(cmd->param1);

	if (!cmd->param3) {
		getWorld()->motionStatus = 5;

		getScene()->updateSceneCoordinates(actor->getPoint1()->x + Common::Rational(actor->getPoint2()->x, 2).toInt() - 320,
			                           actor->getPoint1()->y + Common::Rational(actor->getPoint2()->y, 2).toInt() - 240,
		                               cmd->param2);
	} else if (cmd->param6) {
		if (getWorld()->motionStatus == 2) {
			_lineIncrement = 1;
		} else {
			cmd->param6 = 0;
			getWorld()->coordinates[0] = -1;
		}
	} else {
		cmd->param6 = 1;
		getWorld()->motionStatus = 2;

		if (getScene()->updateSceneCoordinates(actor->getPoint1()->x + Common::Rational(actor->getPoint2()->x, 2).toInt() - 320,
										   actor->getPoint1()->y + Common::Rational(actor->getPoint2()->y, 2).toInt() - 240,
										   cmd->param2,
										   true,
										   &cmd->param6))
			_lineIncrement = 0;
		else
			_lineIncrement = 1;
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x44
IMPLEMENT_OPCODE(PaletteFade)
	getScreen()->paletteFade(0, cmd->param1, cmd->param2);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x45
IMPLEMENT_OPCODE(StartPaletteFadeThread)
	getScreen()->startPaletteFade(getWorld()->currentPaletteId, cmd->param1, cmd->param2);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x46
IMPLEMENT_OPCODE(_unk46)
	if (cmd->param6) {
		if (getSound()->isPlaying(getSpeech()->getSoundResourceId())) {
			_lineIncrement = 1;
		} else {
			cmd->param6 = 0;
			if (cmd->param5) {
				getScene()->getActor(cmd->param5)->enable();
			} else if (cmd->param4 != cmd->param3 && cmd->param4) {
				getWorld()->getObjectById((ObjectId)cmd->param3)->disable();

				Object *object = getWorld()->getObjectById((ObjectId)cmd->param4);
				object->setNextFrame(object->flags);
			}

			_vm->clearGameFlag(kGameFlagScriptProcessing);

			getSpeech()->resetResourceIds();
		}
	} else {
		_vm->setGameFlag(kGameFlagScriptProcessing);
		getSpeech()->play(MAKE_RESOURCE(kResourcePackSpeech, 515 + cmd->param1), MAKE_RESOURCE(kResourcePackShared, 1290 + cmd->param1));

		if (cmd->param2) {
			getScene()->getActor(cmd->param5)->updateStatus(kActorStatus8);
			cmd->param6 = 1;
			_lineIncrement = 1;
		} else {
			if (cmd->param4 != cmd->param3) {
				if (cmd->param4)
					getWorld()->getObjectById((ObjectId)cmd->param4)->disable();

				if (cmd->param3)
					getWorld()->getObjectById((ObjectId)cmd->param3)->setNextFrame(getWorld()->getObjectById((ObjectId)cmd->param4)->flags);
			}

			cmd->param6 = 1;
			_lineIncrement = 1;
		}
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x47
IMPLEMENT_OPCODE(ActorFaceObject)
	getScene()->getActor(cmd->param1)->faceTarget((ObjectId)cmd->param2, (DirectionFrom)cmd->param3);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x48
IMPLEMENT_OPCODE(_unk48_MATTE_01)
	getSharedData()->setMatteVar1(0);
	getSharedData()->setMatteInitialized(true);

	if (getSharedData()->getMatteBarHeight() >= 170) {
		getSharedData()->setMatteBarHeight(0);
		_lineIncrement = 0;
		getCursor()->show();
	} else {
		_lineIncrement = 1;

		if (!getSharedData()->getMatteBarHeight()) {
			getCursor()->hide();
			getSharedData()->setMatteBarHeight(1);
		}
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x49
IMPLEMENT_OPCODE(_unk49_MATTE_90)
	getSharedData()->setMatteVar1(0);
	getSharedData()->setMatteInitialized(true);
	getSharedData()->setMattePlaySound(true);

	if (getSharedData()->getMatteBarHeight() >= 170) {
		getSharedData()->setMatteBarHeight(0);
		_lineIncrement = 0;
		getCursor()->show();
	} else {
		_lineIncrement = 1;

		if (!getSharedData()->getMatteBarHeight()) {
			getCursor()->hide();
			getSharedData()->setMatteBarHeight(90);
		}
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x4A
IMPLEMENT_OPCODE(JumpIfSoundPlaying)
	if (cmd->param3 == 1) {
		if (_vm->sound()->isPlaying((ResourceId)cmd->param1)) {
			_currentLine = cmd->param2;
		}
	} else if (!_vm->sound()->isPlaying((ResourceId)cmd->param1)) {
		_currentLine = cmd->param2;
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x4B
IMPLEMENT_OPCODE(ChangePlayerActorIndex)
	getScene()->changePlayer(cmd->param1);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x4C
IMPLEMENT_OPCODE(ChangeActorStatus)
	Actor *actor = getScene()->getActor(cmd->param1);

	if (cmd->param2) {
		if (actor->getStatus() < kActorStatus11)
			actor->setStatus(kActorStatus14);
	} else {
		actor->setStatus(kActorStatusEnabled);
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x4D
IMPLEMENT_OPCODE(StopSound)
	if (_vm->sound()->isPlaying((ResourceId)cmd->param1))
		_vm->sound()->stop((ResourceId)cmd->param1);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x4E
IMPLEMENT_OPCODE(JumpRandom)
	if (_vm->getRandom((uint32)cmd->param1) < (uint32)cmd->param2)
		return;

	setNextLine(cmd->param3);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x4F
IMPLEMENT_OPCODE(ClearScreen)
	getSharedData()->setFlag(kFlagSkipDrawScene, (bool)cmd->param1);

	if (cmd->param1)
		getScreen()->clear();
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x50
IMPLEMENT_OPCODE(Quit)
	getScreen()->clear();
	Engine::quitGame();

	// We need to exit the interpreter loop so we get back to the event loop
	// and get the quit message
	_exit = true;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x51
IMPLEMENT_OPCODE(JumpObjectFrame)
	Object *object = getWorld()->getObjectById((ObjectId)cmd->param1);

	if (cmd->param2 == -1)
		cmd->param2 = object->getFrameCount() - 1;

	if (cmd->param3) {
		if(object->getFrameIndex() == (uint32)cmd->param2)
			return;
	} else if (cmd->param4) {
		if (object->getFrameIndex() < (uint32)cmd->param2)
			return;
	} else if (cmd->param5) {
		if (object->getFrameIndex() > (uint32)cmd->param2)
			return;
	} else if (cmd->param6) {
		if (object->getFrameIndex() <= (uint32)cmd->param2)
			return;
	} else if (cmd->param7) {
		if (object->getFrameIndex() >= (uint32)cmd->param2)
			return;
	} else if (!cmd->param8 || object->getFrameIndex() != (uint32)cmd->param2) {
		return;
	}

	setNextLine(cmd->param9);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x52
IMPLEMENT_OPCODE(DeleteGraphics)
	for (uint i = 0; i < 55; i++)
		getScreen()->deleteGraphicFromQueue(getScene()->getActor((ActorIndex)(cmd->param1))->getResourcesId((uint32)cmd->param1));
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x53
IMPLEMENT_OPCODE(SetActorField944)
	Actor *actor = getScene()->getActor(cmd->param1);

	actor->setField944(cmd->param2);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x54
IMPLEMENT_OPCODE(_unk54_SET_ACTIONLIST_6EC)
	if (cmd->param2)
		_currentScript->field_1BB0 = _vm->getRandom((uint32)cmd->param1);
	else
		_currentScript->field_1BB0 = cmd->param1;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x55
IMPLEMENT_OPCODE(_unk55)

	if (cmd->param2) {
		if (_currentScript->field_1BB0 == cmd->param1)
			return;
	} else {

		if (cmd->param3) {
			if (_currentScript->field_1BB0 < cmd->param1)
				return;
		} else if (cmd->param4) {
			if (_currentScript->field_1BB0 > cmd->param1)
				return;
		} else if (!cmd->param5) {
			if (cmd->param6) {
				if (_currentScript->field_1BB0 >= cmd->param1)
					return;
			} else {
				if (!cmd->param7 || _currentScript->field_1BB0 != cmd->param1)
					return;
			}
		} else {
			if (_currentScript->field_1BB0 <= cmd->param1)
				return;
		}
	}

	setNextLine(cmd->param8);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x56
IMPLEMENT_OPCODE(_unk56)
	Actor *actor = getScene()->getActor(cmd->param2 == 2 ? kActorInvalid : cmd->param1);

	if (actor->getStatus() == kActorStatus2 || actor->getStatus() == kActorStatus13) {
		if (cmd->param2 == 2)
			_lineIncrement = 1;

		return;
	}

	if (cmd->param2 == 2) {
		cmd->param2 = 1;
		_lineIncrement = 0;

		if ((actor->getPoint1()->x + actor->getPoint2()->x == cmd->param6) && (actor->getPoint1()->y + actor->getPoint2()->y == cmd->param7)) {
			getScene()->getActor()->faceTarget((ObjectId)cmd->param1, kDirectionFromActor);
			actor->updateFromDirection((ActorDirection)((actor->getDirection() + 4) & 7));
		} else {
			_currentLine = cmd->param3;
		}
	} else {
		int32 x = 0;
		int32 y = 0; // FIXME: is is set somewhere else?

		if (actor->process_4069B0(&x, &cmd->param4) == 1) {
			getScene()->getActor()->processStatus(x, y, (bool)cmd->param4);
			cmd->param6 = x;
			cmd->param7 = y;

			if (cmd->param2 == 1) {
				cmd->param2 = 2;
				_lineIncrement = 1;
			}
		} else {
			if (cmd->param4)
				getSpeech()->playIndexed(1);

			_currentLine = cmd->param3;
		}
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x57
IMPLEMENT_OPCODE(SetResourcePalette)
	getWorld()->currentPaletteId = getWorld()->graphicResourceIds[cmd->param1];
	getScreen()->setPalette(getWorld()->currentPaletteId);
	getScreen()->setGammaLevel(getWorld()->currentPaletteId, 0);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x58
IMPLEMENT_OPCODE(SetObjectFrameIdxFlaged)
	Object *object = getWorld()->getObjectById((ObjectId)cmd->param1);

	if (cmd->param3)
		object->flags = 1 | object->flags;
	else
		object->flags &= ~kObjectFlagEnabled;

	object->setFrameIndex(cmd->param2);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x59
IMPLEMENT_OPCODE(_unk59)
	Object *object = getWorld()->getObjectById((ObjectId)cmd->param1);

	if (cmd->param2) {
		object->flags |= kObjectFlag40000;
	} else {
		object->flags &= ~kObjectFlag10E38;
	}

	if (cmd->param3 && (object->flags & kObjectFlag10E38))
		_lineIncrement = 1;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x5A
IMPLEMENT_OPCODE(_unk5A)
	getScene()->getActor(cmd->param1)->setActionIndex2(cmd->param2);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x5B
IMPLEMENT_OPCODE(_unk5B)
	if (cmd->param2 >= 0 && cmd->param2 <= 3) {
		if (cmd->param1) {
			Object *object = getWorld()->getObjectById((ObjectId)cmd->param1);

			object->setField67C(cmd->param2);

			if (object->getField67C())
				object->setField67C(object->getField67C() + 3);
		} else {
			getScene()->getActor(cmd->param3)->setField96C(cmd->param2);
		}
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x5C
IMPLEMENT_OPCODE(QueueScript)
	queueScript(getWorld()->getActionAreaById(cmd->param1)->scriptIndex, cmd->param2);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x5D
IMPLEMENT_OPCODE(_unk5D)
	Actor *actor = getScene()->getActor(cmd->param1);

	actor->process_401830(cmd->param2, cmd->param3, cmd->param4, cmd->param5, cmd->param6, cmd->param7, cmd->param8, cmd->param9);
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x5E
IMPLEMENT_OPCODE(ClearActorFields)
	Actor *actor = getScene()->getActor(cmd->param1);

	// Clear fields starting from field_970
	actor->clearFields();
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x5F
IMPLEMENT_OPCODE(SetObjectLastFrameIdx)
	Object *object = getWorld()->getObjectById((ObjectId)cmd->param1);

	if (object->getFrameIndex() == object->getFrameCount() - 1) {
		_lineIncrement = 0;
		object->flags &= ~kObjectFlag10E38;
	} else {
		_lineIncrement = 1;
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x60
IMPLEMENT_OPCODE(_unk60_SET_OR_CLR_ACTIONAREA_FLAG)
	ActionArea *area = getWorld()->getActionAreaById(cmd->param1);

	if (cmd->param2)
		area->flags |= 1;
	else
		area->flags &= ~1;
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x61
IMPLEMENT_OPCODE(_unk61)
	if (cmd->param2) {
		if (getWorld()->nextPlayer == kActorInvalid) {
			_lineIncrement = 0;
			cmd->param2 = 0;
		} else {
			_lineIncrement = 1;
		}
	} else {
		Actor::updatePlayerChapter9(_vm, cmd->param1);
		cmd->param2 = 1;
		_lineIncrement = 1;
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x62
IMPLEMENT_OPCODE(ShowOptionsScreen)
	_vm->menu()->show();
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode 0x63
IMPLEMENT_OPCODE(_unk63)
	if (cmd->param1) {
		getSharedData()->setFlag(kFlag1, true);
		getSharedData()->setFlag(kFlag2, true);
	}

	if (_vm->sound()->isPlaying(getSpeech()->getSoundResourceId())) {
		_lineIncrement = 1;
		return;
	} else if (!cmd->param1) {
		cmd->param1 = 1;
	}
END_OPCODE

//////////////////////////////////////////////////////////////////////////
// Opcode Helper functions
//////////////////////////////////////////////////////////////////////////

void ScriptManager::enableObject(ScriptEntry *cmd, ObjectEnableType type) {
	int32 field67C = 0;

	// Setup field67C
	switch (type) {
	default:
		break;

	case kObjectEnableType0:
		field67C = 4 + _currentScript->counter / cmd->param2;
		break;

	case kObjectEnableType1:
		field67C = 6 - _currentScript->counter / cmd->param2;
		break;

	case kObjectEnableType2:
		field67C = 0;
		break;
	}

	// Update first set of objects
	for (int i = 0; i < 7; i++) {
		Object *object = getWorld()->getObjectById((ObjectId)cmd->param4);
		if (object != NULL)
			object->setField67C(field67C);

		++cmd->param4;
	}

	// Update per-chapter objects
	switch (getWorld()->chapter) {
	default:
		break;

	case kChapter3:
		if (cmd->param1 != 810)
			break;

		getWorld()->getObjectById(kObjectTableRecordRoom)->setField67C(field67C);
		getWorld()->getObjectById(kObjectOrangeRecord)->setField67C(field67C);
		break;

	case kChapter4:
		if (cmd->param1 != 1232)
			break;

		getWorld()->getObjectById(kObjectInfernoStatusQuo)->setField67C(field67C);
		getWorld()->getObjectById(kObjectJugglerWithPin)->setField67C(field67C);
		getWorld()->getObjectById(kObjectJuggler)->setField67C(field67C);
		getWorld()->getObjectById(kObjectClownStatusQuo)->setField67C(field67C);
		getWorld()->getObjectById(kObjectTrixieStatusQuo)->setField67C(field67C);
		getWorld()->getObjectById(kObjectSimonStatusQuo)->setField67C(field67C);
		getWorld()->getObjectById(kObjectBigTopBarrel)->setField67C(field67C);
		getWorld()->getObjectById(kObjectStandBehindJuggler)->setField67C(field67C);
		getWorld()->getObjectById(kObjectStrongmanLeft)->setField67C(field67C);
		getWorld()->getObjectById(kObjectStrongmanRight)->setField67C(field67C);
		break;

	case kChapter6:
		if (cmd->param1 == 1998) {
			getWorld()->getObjectById(kObjectGlow4)->setField67C(field67C);
			getWorld()->getObjectById(kObjectBugOnTable)->setField67C(field67C);
			getWorld()->getObjectById(kObjectInsidePipeCyberPod)->setField67C(field67C);
			getWorld()->getObjectById(kObjectDiscardedBugPincer)->setField67C(field67C);
			getWorld()->getObjectById(kObjectLitLimbScanner)->setField67C(field67C);
			getWorld()->getObjectById(kObjectCyberTable)->setField67C(field67C);
		}

		if (cmd->param1 == 2003) {
			getWorld()->getObjectById(kObjectNPC066StatusQuo)->setField67C(field67C);
			getWorld()->getObjectById(kObject2507)->setField67C(field67C);
			getWorld()->getObjectById(kObjectBrokenPipe)->setField67C(field67C);
			getWorld()->getObjectById(kObjectEmberPopsOut)->setField67C(field67C);
			getWorld()->getObjectById(kObjectBugCarriesEmber)->setField67C(field67C);
			getWorld()->getObjectById(kObjectFurnaceHole)->setField67C(field67C);
			getWorld()->getObjectById(kObjectTopOfFurnace)->setField67C(field67C);
			getWorld()->getObjectById(kObjectElderBugLimb)->setField67C(field67C);
		}
		break;

	case kChapter7:
		if (cmd->param1 != 1273)
			break;

		getWorld()->getObjectById(kObjectHeadOnTable)->setField67C(field67C);
		getWorld()->getObjectById(kObjectOfficeWallNew)->setField67C(field67C);
		getWorld()->getObjectById(kObjectChalice)->setField67C(field67C);
		break;

	case kChapter8:
		if (cmd->param1 != 1795)
			break;

		getWorld()->getObjectById(kObjectHook1B)->setField67C(field67C);
		getWorld()->getObjectById(kObjectHook2B)->setField67C(field67C);
		getWorld()->getObjectById(kObjectHook3B)->setField67C(field67C);
		getWorld()->getObjectById(kObjectHook4B)->setField67C(field67C);
		getWorld()->getObjectById(kObjectHook5B)->setField67C(field67C);
		getWorld()->getObjectById(kObjectHook6B)->setField67C(field67C);
		getWorld()->getObjectById(kObjectHook0Down)->setField67C(field67C);
		getWorld()->getObjectById(kObjectHook2Down)->setField67C(field67C);
		getWorld()->getObjectById(kObjectHook3Down)->setField67C(field67C);
		getWorld()->getObjectById(kObjectHook4Down)->setField67C(field67C);
		getWorld()->getObjectById(kObjectHook5Down)->setField67C(field67C);
		getWorld()->getObjectById(kObjectHook6Down)->setField67C(field67C);
		getWorld()->getObjectById(kObject2230)->setField67C(field67C);
		break;
	}
}

void ScriptManager::setActionFlag(ScriptEntry *cmd, ActionType flag) {
	switch (cmd->param2) {
	default:
		getWorld()->getObjectById((ObjectId)cmd->param1)->actionType |= flag;
		break;

	case 1:
		getWorld()->getActionAreaById(cmd->param1)->actionType |= flag;
		break;

	case 2:
		getWorld()->actors[cmd->param1]->actionType |= flag;
		break;
	}
}

void ScriptManager::clearActionFlag(ScriptEntry *cmd, ActionType flag) {
	switch (cmd->param2) {
	default:
		getWorld()->getObjectById((ObjectId)cmd->param1)->actionType &= ~flag;
		break;

	case 1:
		getWorld()->getActionAreaById(cmd->param1)->actionType &= ~flag;
		break;

	case 2:
		getWorld()->actors[cmd->param1]->actionType &= ~flag;
		break;
	}
}

void ScriptManager::jumpIfActionFlag(ScriptEntry *cmd, ActionType flag) {
	bool done = false;

	switch (cmd->param3) {
	default:
		done = (getWorld()->actors[cmd->param1]->actionType & flag) == 0;
		break;

	case 0:
		done = (getWorld()->getObjectById((ObjectId)cmd->param1)->actionType & flag) == 0;
		break;

	case 1:
		done = (getWorld()->getActionAreaById(cmd->param1)->actionType & flag) == 0;
		break;
	}

	if (!done)
		return;

	setNextLine(cmd->param2);
}

void ScriptManager::setNextLine(int32 line) {
	if (!_currentScript)
		error("[ActionList::setNextLine] No current script set!");

	int32 opcode = _currentScript->commands[line].opcode;
	if (opcode == 0x10 || opcode == 0) { // Return
		_currentLine = line;
	} else {
		_done = true;
	}
}

} // end of namespace Asylum
