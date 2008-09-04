#include "common/scummsys.h"
#include "common/system.h"
#include "common/endian.h"
#include "common/util.h"

#include "innocent/logic.h"

#include "common/file.h"

using namespace Common;

namespace Innocent {

Logic::Logic() {}

Logic::~Logic() {}

void Logic::load() {
	const char filename[] = "iuc_main.dat";
	debug(1, "Logic::open() Loading from %s", filename);
	Common::File s;
	if (!s.open(filename))
		error("Logic::open() Could not open %s", filename);


	uint16 length = s.readUint16LE();

	byte *data = new byte[length];

	s.seek(0);
	s.read(data, length);

	descramble(data + 2, length - 2);
	_mainData.reset(new MemoryReadStream(data, length, true));
	
	if (gDebugLevel >= 5) {
		Common::DumpFile fd;
		fd.open("maindat.dump");
		fd.write(data, length);
		fd.close();
		debug(5, "Logic::load() dumped descrambled file to  maindat.dump");
	}

	_descriptor.reset(s.readStream(kDescriptorSize));
	s.close();
}

void Logic::descramble(byte *data, uint16 length) {
	for (; length; length--)
		(*(data++)) ^= 0x6f;
}

uint16 Logic::graphicsCount() const {
	static uint16 count = 0;
	if (count) return count;

	_descriptor->seek(kDescriptorGraphicsCount);
	return (count = _descriptor->readUint16LE());
}

} // End of namespace Innocent
