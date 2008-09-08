#include "innocent/inter.h"

#include "common/endian.h"
#include "common/util.h"

#include "innocent/innocent.h"
#include "innocent/logic.h"
#include "innocent/program.h"
#include "innocent/resources.h"

#include "innocent/opcode_handlers.cpp"

namespace Innocent {

enum Debug {
	kOpcodeDetails = 3
};

enum {
	kOpcodeMax = 0xfd
};

template <int opcode>
void Interpreter::opcodeHandler(ValueVector args){
	error("unhandled opcode %d [=0x%02x]", opcode, opcode);
}


template<int N>
void Interpreter::init_opcodes() {
	_handlers[N] = &Innocent::Interpreter::opcodeHandler<N>;
	init_opcodes<N-1>();
}

template<>
void Interpreter::init_opcodes<-1>() {}

// class Uint16Argument : public Argument {
// public:
// 	Uint16Argument(byte *ptr) : Argument(ptr) {}
// 	operator uint16() const { return READ_LE_UINT16(_ptr); }
// 	Argument operator=(uint16 value) { WRITE_LE_UINT16(_ptr, value); return *this; }
// 	operator byte() const { return READ_LE_UINT16(_ptr); }
// 	Argument operator=(byte value) { WRITE_LE_UINT16(_ptr, value); return *this; }
// };
// 
// class ByteArgument : public Argument {
// public:
// 	ByteArgument(byte *ptr) : Argument(ptr) {}
// 	operator byte() const { return *_ptr; }
// 	Argument operator=(byte b) { *_ptr = b; return *this; }
// };

Interpreter::Interpreter(Logic *l, byte *base, const char *n) :
		_logic(l),
		_engine(l->engine()),
		_resources(_engine->resources()),
		_base(base),
		_roomLoop(0)
		{
	init_opcodes<255>();
	strncpy(_name, n, 100);
	init();
}

void Interpreter::setRoomLoop(byte *code) {
	_roomLoop = code;
}

void Interpreter::tick() {
	if (_roomLoop)
		run(_roomLoop - _base, kCodeRoomLoop);
}

/* mode:
0 - initialization,
1 - room handler,
8 - dataset init
*/

void Interpreter::init() {
	_graphics = _engine->graphics();
}

Status Interpreter::run(uint16 offset, OpcodeMode mode) {
	_mode = mode;
	return run(offset);
}

Status Interpreter::run(uint16 offset) {
	byte *code = _base + offset;
	_failedCondition = 0;
	_return = false;

	while (!_return) {
		byte opcode = *code;
		if (opcode > kOpcodeMax) {
			return kInvalidOpcode;
		}

		uint8 nargs = _argumentsCounts[opcode];

		OpcodeHandler handler = _handlers[opcode];

		ValueVector args;

		for (uint i = 0; i < nargs; i++)
			args.push_back(getArgument(code));

		if (nargs == 0)
			code += 2;

		if (opcode == 0x2c || opcode == 0x2d || opcode == 1 || !_failedCondition)
			(this->*handler)(args);
		else {
			debugC(3, kDebugLevelScript, "opcode 0x%02x skipped", opcode);
			if (opcode != 0 && opcode < 0x26)
				_failedCondition++;
		}
	}

	return kReturned;
}

enum ArgumentTypes {
	kArgumentImmediate = 1,
	kArgumentMainWord = 2,
	kArgumentMainByte = 3,
	kArgumentString = 7,
	kArgumentCode = 9
};

template<>
Constant *Interpreter::readArgument<Constant>(byte *&code) {
	uint16 value = READ_LE_UINT16(code);
	code += 2;
	debugC(4, kDebugLevelScript, "read constant value %d as argument", value);
	return new Constant(value);
}

class GlobalByteVariable : public ByteVariable {
public:
	GlobalByteVariable(uint16 index, Resources *res) : ByteVariable(res->getGlobalByteVariable(index)) {
		snprintf(_inspect, 27, "global byte variable %d", index);
	}
	virtual const char *operator+() const { return _inspect; }
private:
	char _inspect[27];
};

class GlobalWordVariable : public WordVariable {
public:
	GlobalWordVariable(uint16 index, Resources *res) : WordVariable(res->getGlobalWordVariable(index)) {
		snprintf(_inspect, 27, "global word variable %d", index);
	}
	virtual const char *operator+() const { return _inspect; }
private:
	char _inspect[27];
};

template<>
GlobalByteVariable *Interpreter::readArgument<GlobalByteVariable>(byte *&code) {
	uint16 index = READ_LE_UINT16(code);
	code += 2;
	debugC(4, kDebugLevelScript, "read global byte variable %d as argument", index);
	return new GlobalByteVariable(index, _resources);
}

template<>
GlobalWordVariable *Interpreter::readArgument<GlobalWordVariable>(byte *&code) {
	uint16 index = READ_LE_UINT16(code) / 2;
	code += 2;
	debugC(4, kDebugLevelScript, "read global word variable %d as argument", index);
	return new GlobalWordVariable(index, _resources);
}

template<>
CodePointer *Interpreter::readArgument<CodePointer>(byte *&code) {
	uint16 offset = READ_LE_UINT16(code);
	code += 2;
	debugC(4, kDebugLevelScript, "read local code offset 0x%04x as argument", offset);
	return new CodePointer(offset, this);
}

// Argument *Interpreter::readLocalArg(byte *&code) {
// 	uint16 offset = READ_LE_UINT16(code);
// 	code += 2;
// 	Argument *arg = new Uint16Argument(_base + offset);
// 	return arg;
// }
// 
// Interpreter::StringArgument::StringArgument(byte *code, Resources *res) : Argument(code) {
// 	byte ch;
// 	byte *str = _translateBuf;
// 	uint16 offset, value;
// 	while ((ch = *(code++))) {
// 		switch (ch) {
// 		case kStringGlobalWord:
// 			offset = READ_LE_UINT16(code);
// 			code += 2;
// 			value = READ_LE_UINT16(res->getGlobalWordVariable(offset/2));
// 			str += snprintf(reinterpret_cast<char *>(str), _translateBuf - str, "%d", value);
// 			break;
// 		case kStringSetColour:
// 			*(str++) = ch;
// 			*(str++) = *(code++);
// 			break;
// 		case kStringCountSpacesIf0:
// 		case kStringCountSpacesIf1:
// 			error("unhandled string special 0x%02x", ch);
// 			break;
// 		case kStringCountSpacesTerminate:
// 			break;
// 		default:
// 			*(str++) = ch;
// 		}
// 	}
// 	*str = 0;
// }
// 
// Argument *Interpreter::readStringArg(byte *&code) {
// 	Argument *arg = new StringArgument(code, _resources);
// 
// 	// skip the string
// 	byte ch;
// 
// 
// 	bool displayed = false;
// 
// 	do {
// 		ch = *(code++);
// 		// try string args len
// 		switch (ch) {
// 		case 9:
// 		case 7:
// 			code ++;
// 		case 6:
// 		case 10:
// 		case 11:
// 			code ++;
// 		case 14:
// 		case 3:
// 			code += 2;
// 			displayed = false;
// 		}
// 
// 		if (ch == 5) {
// 			while (*(code++) != 0);
// 			code += 2;
// 			displayed = false;
// 			continue;
// 		}
// 
// 		if (!displayed) {
// 			displayed = true;
// 		}
// 	} while (ch != 0);
// 
// 	return arg;
// }

Value *Interpreter::getArgument(byte *&code) {
	uint8 argument_type = code[1];
	code += 2;

	switch (argument_type) {
		case kArgumentImmediate:
			return readArgument<Constant>(code);
		case kArgumentMainWord:
			return readArgument<GlobalWordVariable>(code);
		case kArgumentMainByte:
			return readArgument<GlobalByteVariable>(code);
/*		case kArgumentString:
			return readStringArg(code);*/
		case kArgumentCode:
			return readArgument<CodePointer>(code);
		default:
			error("don't know how to handle argument type 0x%02x", argument_type);
	}
}

const uint8 Interpreter::_argumentsCounts[] = {
	#include "opcodes_nargs.data"
};

void Interpreter::failedCondition() {
	_failedCondition++;
}

void Interpreter::endIf() {
	if (_failedCondition) _failedCondition--;
}

void Interpreter::goBack() {
	_return = true;
}

} // End of namespace Innocent
