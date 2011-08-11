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
 */

#ifndef RING_AQUATOR_H
#define RING_AQUATOR_H

#include "ring/shared.h"

#include "common/rect.h"

namespace Ring {

class ImageHeaderEntry {
public:
	struct Header {
		float field_0;
		float field_4;
		float field_8;
		float field_C;
		float field_10;
		float field_14;
		float field_18;
		float field_1C;
		float field_20;
		float field_24;
		float field_28;
		uint32 field_2C;
		float field_30;

		Header();
		void update(const Header &header);
		void load(Common::SeekableReadStream *stream);
	};

	ImageHeaderEntry();
	~ImageHeaderEntry();

	void init(Common::SeekableReadStream *stream, bool hasAdditionnalData);
	void init(ImageHeaderEntry *entry);
	void update(ImageHeaderEntry *entry, bool updateCaller = true);

	// Buffer
	void prepareBuffer();
	void drawBuffer();
	void updateBuffer(Common::Point *point);
	void updateCoordinates(Common::Point *point);

	bool isInitialized() { return _buffer != NULL; }
	const Header &getHeader() { return _header; }
	void *getBuffer() { return _buffer; }

private:
	Header  _header;
	void   *_buffer;
	void   *_bufferData;
	bool    _hasAdditionnalData;

	void reset();
	void *allocBuffer(bool hasAdditionnalData) const;
	void initData();
};

class ImageHeader {
public:
	ImageHeader();
	~ImageHeader();

	void reset();

	void init(Common::SeekableReadStream *stream);
	void update(ImageHeaderEntry* entry);

	bool hasEntries() { return _entries.size() != 0; }
	uint32 getField4() { return _field_4; }
	ImageHeaderEntry *get(uint32 index);
	ImageHeaderEntry *getCurrent() { return _current; }

private:
	uint32 _field_4;
	Common::Array<ImageHeaderEntry *> _entries;
	ImageHeaderEntry *_current;
	int32 _field_4C;
};

class AquatorImageHeader {
public:
	AquatorImageHeader();
	~AquatorImageHeader();

	void setField0(bool state) { _field_0 = state; }
	void setField4(uint32 val) { _field_4 = val; }
	void setField8(uint32 val) { _field_8 = val; }
	void setChannel(uint32 val);

	bool getField0() { return _field_0; }
	uint32 getField4() { return _field_4; }
	uint32 getField8() { return _field_8; }
	ImageHeader *getHeader() { return _header; }
	uint32 getChannel() { return _channel; }

private:
	bool _field_0;
	uint32 _field_4;
	uint32 _field_8;
	ImageHeader *_header;
	uint32 _channel;
};

class AquatorStream {
public:
	AquatorStream(uint32 count, Common::String path);
	~AquatorStream();

	void alloc(bool isCompressed, const Graphics::PixelFormat &format, uint32 size);
	void dealloc();

	uint32 sub_410F50(uint32 index);
	void updateEntries(float timeOffset);
	void sub_411530(uint32 index, uint32 a2);
	void setChannel(uint32 index, uint32 channel);

	ImageHeaderEntry *getEntry() { return _entry; }
	uint32 getCount() { return _headers.size(); }
	uint32 getChannel(uint32 index);

	bool isInitialized() { return _entry->isInitialized(); }

private:
	Common::Array<AquatorImageHeader *> _headers;
	Common::String _path;
	ImageHeaderEntry *_entry;

	void initNode(Common::SeekableReadStream *stream, const Graphics::PixelFormat &format);
	void initChannel(Common::SeekableReadStream *stream, uint32 index);

	// Decompressed aquator
	void initStream(uint32 index);
};

} // End of namespace Ring

#endif // RING_AQUATOR_H
