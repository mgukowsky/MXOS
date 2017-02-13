#pragma once

#include <LOW_LEVEL.hpp>

namespace KERN {
namespace DRIVERS {

class Keyboard {
public:
	static char get_char();

	static bool can_echo();

private:
	//Static class has no constructors/destructor
	Keyboard() = delete;
	~Keyboard() = delete;
	KERNEL_DISABLE_ALTERNATE_CTORS(Keyboard);

	enum class Commands : u8 {
		ECHO = 0xEE
	};

	enum class Responses : u8 {
		ECHO = 0xEE
	};

};

} //namespace DRIVERS
} //namespace KERN