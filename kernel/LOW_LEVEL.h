#pragma once

#include "defs.h"

namespace KERN {
namespace LOW_LEVEL {

inline void outb(const u16 port, const u8 byte){
	//From the osdev wiki
	//'a' constraint forces use of the *a* registers, same idea for 'd'
	//'N' allows values less than one byte to be assembled as immediate 
	//values, thus freeing up edx for other uses
	asm volatile ( "outb %0, %1" : : "a"(byte), "Nd"(port) );
}

} //namespace LOW_LEVEL
} //namespace KERN