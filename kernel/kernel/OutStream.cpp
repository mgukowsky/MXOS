#include <OutStream.hpp>
#include <TextModeIO.hpp>

using namespace KERN;

const char * OutStream::_HEX_PREFIX = "0x";

OutStream& OutStream::operator<<(const char *cstr) {
	DRIVERS::TextModeIO::print_string(cstr);
	return *this;
}