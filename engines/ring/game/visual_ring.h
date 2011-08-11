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

#ifndef RING_VISUALELEMENTRING_H
#define RING_VISUALELEMENTRING_H

#include "ring/graphics/visual.h"

namespace Ring {

class VisualElementRing : public Visual {
public:
	VisualElementRing(Id id);
	virtual ~VisualElementRing();

	virtual void alloc();
	virtual void dealloc();

	virtual bool handleLeftButtonUp(const Common::Point &point) { return false; }
	virtual bool handleUpdate(const Common::Point &point) { return false; }

	void init(uint32 a1, uint32 a2, uint32 left, uint32 top, uint32 offsetY, uint32 height, uint32 progressMultiplier, uint32 progressColor);
	virtual void draw();

private:
	uint32 _field_D;
	uint32 _field_11;
	uint32 _left;
	uint32 _top;
	uint32 _offsetY;
	uint32 _height;
	uint32 _progressMultiplier;
	uint32 _progressColor;  ///< The progress color in RGB form (0x00bbggrr)
	bool   _initialized;
	float  _progress1;
	float  _progress2;
	float  _progress3;
	float  _progress4;

	void setupProgress(Id progressId, uint32 textIndex, float *progress);
};

} // End of namespace Ring

#endif // RING_VISUALELEMENTRING_H
