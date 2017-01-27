#include "OutStream.h"
#include "TextModeIO.h"

using namespace KERN;

const char * OutStream::_HEX_PREFIX = "0x";

OutStream& OutStream::operator<<(const char *cstr) {
	TextModeIO::print_string(cstr);
	return *this;
}

OutStream& OutStream::operator<<(const int num) {
	_print_number_as_hex(num);
	return *this;
}