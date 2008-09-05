#include "innocent/logic.h"

#include "innocent/innocent.h"
#include "innocent/inter.h"
#include "innocent/resources.h"
#include "innocent/main_dat.h"

namespace Innocent {

Logic::Logic(Engine *e) :
		_engine(e),
		_interpreter(new Interpreter(this)),
		_status(kStatusOk),
		_main(_engine->_resources->_main.get()) // FIXME careful when restarting
	{}

void Logic::start() {
	byte *entry_point = _main->getEntryPoint();

	_interpreter->run(entry_point, 0);
}

byte *Logic::getGlobalByteVar(uint16 index) {
	return _main->getByteVar(index);
}

} // End of namespace Innocent
