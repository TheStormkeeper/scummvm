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
 */

#ifndef PRINCE_H
#define PRINCE_H

#include "common/random.h"
#include "common/system.h"
#include "common/debug.h"
#include "common/debug-channels.h"
#include "common/textconsole.h"
#include "common/rect.h"

#include "engines/engine.h"
#include "engines/util.h"

#include "graphics/surface.h"

#include "audio/mixer.h"

namespace Prince {

struct PrinceGameDescription;

class PrinceEngine;

class PrinceEngine : public Engine {
protected:
	Common::Error run();

public:
	PrinceEngine(OSystem *syst, const PrinceGameDescription *gameDesc);
	virtual ~PrinceEngine();

	virtual bool hasFeature(EngineFeature f) const;

	int getGameType() const;
	const char *getGameId() const;
	uint32 getFeatures() const;
	Common::Language getLanguage() const;

	const PrinceGameDescription *_gameDescription;
	
private:
	Common::RandomSource *_rnd;

	byte _palette[768];
	Graphics::Surface *_frontScreen;
	const Graphics::Surface *_roomBackground;

    void mainLoop();

	void loadBackground(Common::SeekableReadStream *stream);
	void setFullPalette();
	
	void drawSprite(Graphics::Surface *sprite, int32 x, int32 y, int32 mod);

};

} // End of namespace Prince

#endif
