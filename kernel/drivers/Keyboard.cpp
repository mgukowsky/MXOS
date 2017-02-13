#include <Keyboard.hpp>

using namespace KERN::DRIVERS;

char Keyboard::get_char() {
	return LOW_LEVEL::inb(LOW_LEVEL::Port::KEYBOARD_DATA);
}

bool Keyboard::can_echo() {
	LOW_LEVEL::outb(LOW_LEVEL::Port::KEYBOARD_DATA, 
		static_cast<u8>(Commands::ECHO));

	//Try a bunch of times to see if the echo functionality is working
	for(int numAttempts = 0; numAttempts < 256; ++numAttempts){
		if(LOW_LEVEL::inb(LOW_LEVEL::Port::KEYBOARD_DATA) == 
			static_cast<u8>(Responses::ECHO)){

			return true;
		}
	}

	return false;
}