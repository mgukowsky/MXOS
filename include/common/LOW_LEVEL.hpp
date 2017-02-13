#pragma once

#include <defs.hpp>

namespace KERN {
namespace LOW_LEVEL {

enum class Port : u16 {
	KEYBOARD_DATA = 0x60,
	KEYBOARD_CMD = 0x64,
	VGA_REG = 0x3D4,
	VGA_DATA
};

inline void outb(const Port port, const u8 byte){
	//From the osdev wiki
	//'a' constraint forces use of the *a* registers, same idea for 'd'
	//'N' allows values less than one byte to be assembled as immediate 
	//values, thus freeing up edx for other uses
	asm volatile ( "outb %0, %1" : : "a"(byte), "Nd"(port) );
}

inline u8 inb(const Port port){
	u8 retVal;
	asm volatile ( "inb %1, %0" : "=a"(retVal) : "Nd"(port) );
	return retVal;
}

inline void nop(){
	asm volatile ( "nop" );
}

inline void hlt(){
	asm volatile ( "hlt" );
}


} //namespace LOW_LEVEL
} //namespace KERN