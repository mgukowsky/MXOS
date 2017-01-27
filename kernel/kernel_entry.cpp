#include "LOW_LEVEL.h"
#include "TextModeIO.h"
#include "OutStream.h"

using namespace KERN;

//Don't mangle the name so that this symbol can be linked to the ASM
//This function never exits
KERNEL_NOMANGLE void kernel_entry(){

	TextModeIO::clear_screen();
	kout << "Stage 3 began at address "
		 << (int)kernel_entry
		 << "; welcome to the Kernel :D\n";
	
	//We will never exit this loop
	while(true){

	}

}