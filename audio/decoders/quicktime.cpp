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

#include "common/debug.h"
#include "common/util.h"
#include "common/memstream.h"
#include "common/stream.h"

#include "audio/audiostream.h"
#include "audio/decoders/quicktime.h"

// Codecs
#include "audio/decoders/adpcm.h"
#include "audio/decoders/qdm2.h"
#include "audio/decoders/raw.h"

namespace Audio {

QuickTimeAudioDecoder::QuickTimeAudioDecoder() : Common::QuickTimeParser() {
	_audStream = 0;
}

QuickTimeAudioDecoder::~QuickTimeAudioDecoder() {
}

bool QuickTimeAudioDecoder::loadFile(const Common::String &filename) {
	if (!Common::QuickTimeParser::loadFile(filename))
		return false;

	init();
	return true;
}

bool QuickTimeAudioDecoder::loadStream(Common::SeekableReadStream *stream) {
	if (!Common::QuickTimeParser::loadStream(stream))
		return false;

	init();
	return true;
}

void QuickTimeAudioDecoder::init() {
	Common::QuickTimeParser::init();

	_audioStreamIndex = -1;

	// Find an audio stream
	for (uint32 i = 0; i < _numStreams; i++)
		if (_streams[i]->codec_type == CODEC_TYPE_AUDIO && _audioStreamIndex < 0)
			_audioStreamIndex = i;

	// Initialize audio, if present
	if (_audioStreamIndex >= 0) {
		AudioSampleDesc *entry = (AudioSampleDesc *)_streams[_audioStreamIndex]->sampleDescs[0];

		if (checkAudioCodecSupport(entry->codecTag)) {
			_audStream = makeQueuingAudioStream(entry->sampleRate, entry->channels == 2);
			_curAudioChunk = 0;

			// Make sure the bits per sample transfers to the sample size
			if (entry->codecTag == MKID_BE('raw ') || entry->codecTag == MKID_BE('twos'))
				_streams[_audioStreamIndex]->sample_size = (entry->bitsPerSample / 8) * entry->channels;
		}
	}
}

Common::QuickTimeParser::SampleDesc *QuickTimeAudioDecoder::readSampleDesc(MOVStreamContext *st, uint32 format) {
	if (st->codec_type == CODEC_TYPE_AUDIO) {
		debug(0, "Audio Codec FourCC: \'%s\'", tag2str(format));

		AudioSampleDesc *entry = new AudioSampleDesc();
		entry->codecTag = format;

		uint16 stsdVersion = _fd->readUint16BE();
		_fd->readUint16BE(); // revision level
		_fd->readUint32BE(); // vendor

		entry->channels = _fd->readUint16BE();			 // channel count
		entry->bitsPerSample = _fd->readUint16BE();	  // sample size

		_fd->readUint16BE(); // compression id = 0
		_fd->readUint16BE(); // packet size = 0

		entry->sampleRate = (_fd->readUint32BE() >> 16);

		debug(0, "stsd version =%d", stsdVersion);
		if (stsdVersion == 0) {
			// Not used, except in special cases. See below.
			entry->samplesPerFrame = entry->bytesPerFrame = 0;
		} else if (stsdVersion == 1) {
			// Read QT version 1 fields. In version 0 these dont exist.
			entry->samplesPerFrame = _fd->readUint32BE();
			debug(0, "stsd samples_per_frame =%d",entry->samplesPerFrame);
			_fd->readUint32BE(); // bytes per packet
			entry->bytesPerFrame = _fd->readUint32BE();
			debug(0, "stsd bytes_per_frame =%d", entry->bytesPerFrame);
			_fd->readUint32BE(); // bytes per sample
		} else {
			warning("Unsupported QuickTime STSD audio version %d", stsdVersion);
			delete entry;
			return 0;
		}

		// Version 0 videos (such as the Riven ones) don't have this set,
		// but we need it later on. Add it in here.
		if (format == MKID_BE('ima4')) {
			entry->samplesPerFrame = 64;
			entry->bytesPerFrame = 34 * entry->channels;
		}

		if (entry->sampleRate == 0 && st->time_scale > 1)
			entry->sampleRate = st->time_scale;

		return entry;
	}

	return 0;
}

bool QuickTimeAudioDecoder::checkAudioCodecSupport(uint32 tag) {
	// Check if the codec is a supported codec
	if (tag == MKID_BE('twos') || tag == MKID_BE('raw ') || tag == MKID_BE('ima4'))
		return true;

#ifdef AUDIO_QDM2_H
	if (tag == MKID_BE('QDM2'))
		return true;
#endif

	if (tag == MKID_BE('mp4a'))
		warning("No MPEG-4 audio (AAC) support");
	else
		warning("Audio Codec Not Supported: \'%s\'", tag2str(tag));

	return false;
}

AudioStream *QuickTimeAudioDecoder::createAudioStream(Common::SeekableReadStream *stream) {
	if (!stream || _audioStreamIndex < 0)
		return NULL;

	AudioSampleDesc *entry = (AudioSampleDesc *)_streams[_audioStreamIndex]->sampleDescs[0];

	if (entry->codecTag == MKID_BE('twos') || entry->codecTag == MKID_BE('raw ')) {
		// Fortunately, most of the audio used in Myst videos is raw...
		uint16 flags = 0;
		if (entry->codecTag == MKID_BE('raw '))
			flags |= FLAG_UNSIGNED;
		if (entry->channels == 2)
			flags |= FLAG_STEREO;
		if (entry->bitsPerSample == 16)
			flags |= FLAG_16BITS;
		uint32 dataSize = stream->size();
		byte *data = (byte *)malloc(dataSize);
		stream->read(data, dataSize);
		delete stream;
		return makeRawStream(data, dataSize, entry->sampleRate, flags);
	} else if (entry->codecTag == MKID_BE('ima4')) {
		// Riven uses this codec (as do some Myst ME videos)
		return makeADPCMStream(stream, DisposeAfterUse::YES, stream->size(), kADPCMApple, entry->sampleRate, entry->channels, 34);
#ifdef AUDIO_QDM2_H
	} else if (entry->codecTag == MKID_BE('QDM2')) {
		// Myst ME uses this codec for many videos
		return makeQDM2Stream(stream, _streams[_audioStreamIndex]->extradata);
#endif
	}

	error("Unsupported audio codec");

	return NULL;
}

uint32 QuickTimeAudioDecoder::getAudioChunkSampleCount(uint chunk) {
	if (_audioStreamIndex < 0)
		return 0;

	uint32 sampleCount = 0;

	for (uint32 j = 0; j < _streams[_audioStreamIndex]->sample_to_chunk_sz; j++)
		if (chunk >= _streams[_audioStreamIndex]->sample_to_chunk[j].first)
			sampleCount = _streams[_audioStreamIndex]->sample_to_chunk[j].count;

	return sampleCount;
}

void QuickTimeAudioDecoder::queueNextAudioChunk() {
	AudioSampleDesc *entry = (AudioSampleDesc *)_streams[_audioStreamIndex]->sampleDescs[0];
	Common::MemoryWriteStreamDynamic *wStream = new Common::MemoryWriteStreamDynamic();

	_fd->seek(_streams[_audioStreamIndex]->chunk_offsets[_curAudioChunk]);

	// First, we have to get the sample count
	uint32 sampleCount = getAudioChunkSampleCount(_curAudioChunk);
	assert(sampleCount);

	// Then calculate the right sizes
	while (sampleCount > 0) {
		uint32 samples = 0, size = 0;

		if (entry->samplesPerFrame >= 160) {
			samples = entry->samplesPerFrame;
			size = entry->bytesPerFrame;
		} else if (entry->samplesPerFrame > 1) {
			samples = MIN<uint32>((1024 / entry->samplesPerFrame) * entry->samplesPerFrame, sampleCount);
			size = (samples / entry->samplesPerFrame) * entry->bytesPerFrame;
		} else {
			samples = MIN<uint32>(1024, sampleCount);
			size = samples * _streams[_audioStreamIndex]->sample_size;
		}

		// Now, we read in the data for this data and output it
		byte *data = (byte *)malloc(size);
		_fd->read(data, size);
		wStream->write(data, size);
		free(data);
		sampleCount -= samples;
	}

	// Now queue the buffer
	_audStream->queueAudioStream(createAudioStream(new Common::MemoryReadStream(wStream->getData(), wStream->size(), DisposeAfterUse::YES)));
	delete wStream;

	_curAudioChunk++;
}

QuickTimeAudioDecoder::AudioSampleDesc::AudioSampleDesc() : Common::QuickTimeParser::SampleDesc() {
	channels = 0;
	sampleRate = 0;
	samplesPerFrame = 0;
	bytesPerFrame = 0;
}

/**
 * A wrapper around QuickTimeAudioDecoder that implements the RewindableAudioStream API
 */
class QuickTimeAudioStream : public RewindableAudioStream, public QuickTimeAudioDecoder {
public:
	QuickTimeAudioStream() {}

	~QuickTimeAudioStream() {
		delete _audStream;
	}

	bool loadFile(const Common::String &filename) {
		return QuickTimeAudioDecoder::loadFile(filename) && _audioStreamIndex >= 0 && _audStream;
	}

	// AudioStream API
	int readBuffer(int16 *buffer, const int numSamples) {
		int samples = 0;

		while (samples < numSamples && !endOfData()) {
			if (_audStream->numQueuedStreams() == 0)
				queueNextAudioChunk();

			samples += _audStream->readBuffer(buffer + samples, numSamples - samples);
		}

		return samples;
	}

	bool isStereo() const { return _audStream->isStereo(); }
	int getRate() const { return _audStream->getRate(); }
	bool endOfData() const { return _curAudioChunk >= _streams[_audioStreamIndex]->chunk_count && _audStream->endOfData(); }

	// RewindableAudioStream API
	bool rewind() {
		// Reset our parent stream
		_curAudioChunk = 0;
		delete _audStream;

		AudioSampleDesc *entry = (AudioSampleDesc *)_streams[_audioStreamIndex]->sampleDescs[0];
		_audStream = makeQueuingAudioStream(entry->sampleRate, entry->channels == 2);
		return true;
	}
};

RewindableAudioStream *makeQuickTimeStream(const Common::String &filename) {
	QuickTimeAudioStream *audioStream = new QuickTimeAudioStream();

	if (!audioStream->loadFile(filename)) {
		delete audioStream;
		return 0;
	}

	return audioStream;
}

} // End of namespace Audio
