#include <LOW_LEVEL.hpp>
#include <TextModeIO.hpp>
#include <Keyboard.hpp>
#include <OutStream.hpp>

using namespace KERN;

//Don't mangle the name so that this symbol can be linked to the ASM
//This function never exits
KERNEL_NOMANGLE void kernel_entry(){

	DRIVERS::TextModeIO::clear_screen();
	kout << "Stage 3 began at address "
		 << reinterpret_cast<u32>(kernel_entry)
		 << "; welcome to the Kernel :D\n";

    if(DRIVERS::Keyboard::can_echo()){
    	kout << "PS/2 Keyboard is ready\n";
    }
    else {
    	kout << "PS/2 Keyboard is NOT ready...\n";
    }

	//We will never exit this loop
	while(true){

		for(int i = 0; i < 0xFFFFFFF; ++i){
			LOW_LEVEL::nop();
		}

		kout << "\n*\n*\n*\n"
			 << DRIVERS::Keyboard::get_char();

	}

}