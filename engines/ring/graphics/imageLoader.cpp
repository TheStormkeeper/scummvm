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

#include "ring/graphics/imageLoader.h"

#include "ring/base/stream.h"

#include "ring/graphics/image.h"
#include "ring/graphics/movie.h"

#include "ring/helpers.h"

#include "common/archive.h"

#include "graphics/imagedec.h"
#include "graphics/surface.h"

namespace Ring {

#pragma region BMP

bool ImageLoaderBMP::load(Image *image, ArchiveType, Zone, LoadFrom) {
	if (!image)
		error("[ImageLoaderBMP::load] Invalid image pointer!");

	// Get image stream
	Common::SeekableReadStream *stream = SearchMan.createReadStreamForMember(image->getName());
	if (!stream) {
		warning("[ImageLoaderBMP::load] Cannot load image (%s)", image->getName().c_str());
		return false;
	}

	// Get image surface
	Graphics::Surface *surface = Graphics::ImageDecoder::loadFile(*stream, g_system->getScreenFormat());
	if (!surface) {
		warning("[ImageLoaderBMP::load] Cannot decode image (%s)", image->getName().c_str());
		delete stream;
		return false;
	}

	image->setSurface(surface);

	delete stream;

	return true;
}

#pragma endregion

#pragma region BMA

ImageLoaderBMA::~ImageLoaderBMA() {
	deinit();
}

bool ImageLoaderBMA::load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom) {
	if (!image)
		error("[ImageLoaderBMA::load] Invalid image pointer!");

	_stream = NULL;
	_filename = image->getName();

	if (!init(type, zone, loadFrom)){
		warning("[ImageLoaderBMA::load] Error opening image file (%s)", _filename.c_str());
		goto cleanup;
	}

	// Read header
	if (!readHeader()) {
		warning("[ImageLoaderBMA::load] Error reading header (%s)", _filename.c_str());
		goto cleanup;
	}

	// Read image data
	if (!readImage(image)) {
		warning("[ImageLoaderBMA::load] Error reading image (%s)", _filename.c_str());
		goto cleanup;
	}

	deinit();

	return true;

cleanup:
	deinit();
	return false;
}

bool ImageLoaderBMA::init(ArchiveType type, Zone zone, LoadFrom loadFrom) {
	_stream = new CompressedStream();

	// Initialize stream
	switch (type) {
	default:
		warning("[ImageLoaderBMA::init] Invalid archive type (%d)!", type);
		break;

	case kArchiveFile:
		return _stream->init(_filename, 1, 0);

	case kArchiveArt:
		return _stream->initArt(_filename, zone, loadFrom);
	}

	return false;
}

void ImageLoaderBMA::deinit() {
	SAFE_DELETE(_stream);
}

bool ImageLoaderBMA::readHeader() {
	if (!_stream)
		error("[ImageLoaderBMA::readHeader] Stream not initialized");

	Common::SeekableReadStream *data = _stream->getCompressedStream();
	if (!data) {
		warning("[ImageLoaderBMA::readHeader] Cannot get compressed stream (%s)", _filename.c_str());
		return false;
	}

	// Read Seq size
	_seqSize = data->readUint32LE();

	// Read header
	_header.coreWidth = data->readUint16LE();
	_header.coreHeight = data->readUint16LE();
	_header.seqWidth = data->readUint32LE();
	_header.seqHeight = data->readUint32LE();
	_header.field_C = data->readUint32LE();
	_header.field_10 = data->readUint16LE();

	// Read core size
	data->seek(_seqSize + 76);
	_coreSize = data->readUint32LE();

	// Compute block size
	_blockSize = 0;
	uint32 val = _header.coreWidth;
	if (val) {
		do {
			val >>= 1;
			++_blockSize;
		} while (val);
	}

	return true;
}

bool ImageLoaderBMA::readImage(Image *image) {
	if (!_stream)
		error("[ImageLoaderBMA::readImage] Stream not initialized");

	Common::MemoryReadStream *imageData = _stream->decompressIndexed(_blockSize,
	                                                                 _seqSize,  (2 * _header.seqWidth * _header.seqHeight) / _header.coreHeight,
	                                                                 _coreSize, 2 * _header.coreWidth * _header.coreHeight,
	                                                                 _header.seqWidth * _header.seqHeight * 16,
							                                         2 * _header.seqWidth * _header.seqHeight - 6,
							                                         _header.field_C, _header.field_10);

	// Create surface to hold the data
	Graphics::Surface *surface = new Graphics::Surface();
	surface->create((uint16)_header.seqWidth, (uint16)_header.seqHeight, 2); // FIXME: Always 16bpp BMPs?

	// Read from compressed stream
	imageData->read(surface->pixels, _header.seqWidth * _header.seqHeight * 2);
	delete imageData;

	image->setSurface(surface);

	return true;
}

#pragma endregion

#pragma region TGC

ImageLoaderTGC::~ImageLoaderTGC() {
	deinit();
}

bool ImageLoaderTGC::load(Image *image, ArchiveType type, Zone zone, LoadFrom loadFrom) {
	if (!image)
		error("[ImageLoaderTGC::load] Invalid image pointer!");

	_stream = NULL;
	_filename = image->getName();

	Common::SeekableReadStream *decompressedData = init(type, zone, loadFrom);
	if (!decompressedData){
		warning("[ImageLoaderTGC::load] Error opening image file (%s)", _filename.c_str());
		goto cleanup;
	}

	// Read header
	if (!readHeader(decompressedData)) {
		warning("[ImageLoaderTGC::load] Error reading header (%s)", _filename.c_str());
		goto cleanup;
	}

	// Read image data
	if (!readImage(decompressedData, image)) {
		warning("[ImageLoaderTGC::load] Error reading image (%s)", _filename.c_str());
		goto cleanup;
	}

	deinit();

	return true;

cleanup:
	deinit();
	return false;
}

Common::SeekableReadStream *ImageLoaderTGC::init(ArchiveType type, Zone zone, LoadFrom loadFrom) {
	_stream = new CompressedStream();

	// Initialize stream
	switch (type) {
	default:
		warning("[ImageLoaderTGC::init] Invalid archive type (%d)!", type);
		return NULL;

	case kArchiveFile:
		if (!_stream->init(_filename, 1, 0))
			return NULL;
		break;

	case kArchiveArt:
		if (!_stream->initArt(_filename, zone, loadFrom))
			return NULL;
		break;
	}

	// TGC are compressed as chunks of up to 64kb
	Common::SeekableReadStream *stream = _stream->getCompressedStream();
	uint32 chunks = stream->readUint32LE();
	uint32 size = stream->readUint32LE();

	return _stream->decompressChuncks(chunks, size);
}

void ImageLoaderTGC::deinit() {
	SAFE_DELETE(_stream);
}

#pragma endregion

#pragma region TGA

bool ImageLoaderTGA::load(Image *image, ArchiveType, Zone, LoadFrom) {
	if (!image)
		error("[ImageLoaderTGA::load] Invalid image pointer!");

	_filename = image->getName();

	// Open a stream
	Common::SeekableReadStream *stream = SearchMan.createReadStreamForMember(_filename);
	if (!stream) {
		warning("[ImageLoaderTGA::load] Error opening image file (%s)", _filename.c_str());
		goto cleanup;
	}

	// Read header
	if (!readHeader(stream)) {
		warning("[ImageLoaderTGA::load] Error reading header (%s)", _filename.c_str());
		goto cleanup;
	}

	// Read image data
	if (!readImage(stream, image)) {
		warning("[ImageLoaderTGA::load] Error reading image (%s)", _filename.c_str());
		goto cleanup;
	}

	delete stream;
	return true;

cleanup:
	delete stream;
	return false;
}

bool ImageLoaderTGA::readHeader(Common::SeekableReadStream *stream) {
	if (!stream) {
		warning("[ImageLoaderTGA::readHeader] Invalid stream (%s)", _filename.c_str());
		return false;
	}

memset(&_header, 0, sizeof(_header));

	_header.identsize       = stream->readByte();
	_header.colourmaptype   = stream->readByte();
	_header.imagetype       = stream->readByte();

	_header.colourmapstart  = stream->readUint16LE();
	_header.colourmaplength = stream->readUint16LE();
	_header.colourmapbits   = stream->readByte();

	_header.xstart          = stream->readUint16LE();
	_header.ystart          = stream->readUint16LE();
	_header.width           = stream->readUint16LE();
	_header.height          = stream->readUint16LE();
	_header.bits            = stream->readByte();
	_header.descriptor      = stream->readByte();

	// Check that we support the image format
	if (_header.imagetype != kImageTypeRGB) {
		warning("[ImageLoaderTGA::readHeader] Only RGB TGA files are supported (%s)", _filename.c_str());
		return false;
	}

	// Check bits per pixel
	if (_header.bits <= 16) {
		warning("[ImageLoaderTGA::readHeader] Only 24 and 32bpp TGA files are supported (%s)", _filename.c_str());
		return false;
	}

	// Skip id string
	stream->skip(_header.identsize);

	return true;
}

bool ImageLoaderTGA::readImage(Common::SeekableReadStream *stream, Image *image) {
	// Check that we are not called with an unsupported format
	if (_header.imagetype != kImageTypeRGB || _header.bits <= 16)
		error("[ImageLoaderTGA::readImage] Unsupported image type (%s)!", _filename.c_str());

	Graphics::Surface *surface = new Graphics::Surface();
	surface->create(_header.width, _header.height, _header.bits / 8);

	uint32 size = _header.width * _header.height;

	switch (_header.bits) {
	default:
		warning("[ImageLoaderTGA::readImage] Unsupported pixel depth (%s)", _filename.c_str());
		surface->free();
		delete surface;
		return false;

	case 24:
		stream->read(surface->pixels, size * 3);
		break;

	case 32:
		stream->read(surface->pixels, size * 4);
		break;
	}

	image->setSurface(surface);

	return true;
}

#pragma endregion

#pragma region CNM

ImageLoaderCIN::~ImageLoaderCIN() {
	deinit();
}

bool ImageLoaderCIN::load(Image *image, ArchiveType, Zone, LoadFrom) {
	if (!image)
		error("[ImageLoaderCNM::load] Invalid image pointer!");

	byte format = 0;
	_filename = image->getName();

	if (!init(_filename)){
		warning("[ImageLoaderCIN::load] Error initializing image reader (%s)", _filename.c_str());
		goto cleanup;
	}

	// Check cinematic format
	format = _cinematic->readByte();
	if (_cinematic->eos() || _cinematic->err() || format != CINEMATIC_FORMAT) {
		warning("[ImageLoaderCIN::load] Wrong cinematic format for %s (was: %d, valid: %d)", _filename.c_str(), format, CINEMATIC_FORMAT);
		goto cleanup;
	}

	// Read image data
	if (!readImage(image)) {
		warning("[ImageLoaderCIN::load] Error reading image (%s)", _filename.c_str());
		goto cleanup;
	}

	deinit();

	return true;

cleanup:
	deinit();
	return false;
}

bool ImageLoaderCIN::init(Common::String filename) {
	_cinematic = new Cinematic();

	if (!_cinematic->init(filename)) {
		warning("[ImageLoaderCIN::init] Error initializing cinematic (%s)", _filename.c_str());
		return false;
	}

	if (!readHeader()) {
		warning("[ImageLoaderCIN::init] Error reading header (%s)", _filename.c_str());
		return false;
	}

	return true;
}

void ImageLoaderCIN::deinit() {
	SAFE_DELETE(_cinematic);
}

bool ImageLoaderCIN::readHeader() {
	if (!_cinematic)
		error("[ImageLoaderCIN::readHeader] Cinematic not initialized properly");

	memset(&_header, 0, sizeof(_header));

	// Read header (size: 0x40)
	_header.field_0    = _cinematic->readUint32LE();
	_header.field_4    = _cinematic->readUint32LE();
	_header.field_8    = _cinematic->readByte();
	_header.field_9    = _cinematic->readByte();
	_header.field_A    = _cinematic->readByte();
	_header.field_B    = _cinematic->readByte();
	_header.field_C    = _cinematic->readUint16LE();
	_header.chunkCount = _cinematic->readUint32LE();
	_header.field_12   = _cinematic->readUint16LE();
	_header.field_16   = _cinematic->readByte();
	_header.width      = _cinematic->readUint32LE();
	_header.height     = _cinematic->readUint32LE();
	_header.field_1F   = _cinematic->readByte();
	_header.field_20   = _cinematic->readUint32LE();
	_header.field_24   = _cinematic->readUint32LE();
	_header.field_28   = _cinematic->readUint32LE();
	_header.field_2C   = _cinematic->readUint32LE();
	_header.field_30   = _cinematic->readUint32LE();
	_header.field_34   = _cinematic->readUint32LE();
	_header.field_38   = _cinematic->readUint32LE();
	_header.field_3C   = _cinematic->readUint32LE();

	// Update width and height
	_width = _header.width;
	_height = _header.height;

	return true;
}

bool ImageLoaderCIN::readImage(Image *image) {
	if (!_cinematic)
		error("[ImageLoaderCIN::readImage] Cinematic not initialized properly");

	if (!image || !image->isInitialized())
		error("[ImageLoaderCNM::readImage] Invalid image pointer or image not initialized!");

	// Create surface to hold the data
	Graphics::Surface *surface = new Graphics::Surface();
	surface->create((uint16)_width, (uint16)_height, 2);

	// Update image data
	_field_1088 = _width + 3;
	_field_1084 = _field_1088 * 3;

	if (!_cinematic->sControl((byte *)surface->pixels))
		error("[ImageLoaderCIN::readImage] Cannot read image");

	delete surface;

	return true;
}

#pragma endregion


} // End of namespace Ring
