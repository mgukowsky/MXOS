#pragma once

#include <stdlib.h>
#include <stdint.h>

#define KERNEL_NOMANGLE	extern "C" __cdecl

//Explicitly turn off copy/assignment/move constructors
#define KERNEL_DISABLE_ALTERNATE_CTORS(CLASS_T) \
	CLASS_T(const CLASS_T&) = delete; \
	CLASS_T& operator=(const CLASS_T&) = delete; \
	CLASS_T(CLASS_T&&) = delete; \
	CLASS_T& operator=(const CLASS_T&&) = delete

namespace KERN {
	typedef 	int8_t 			s8;
	typedef 	int16_t 		s16;
	typedef 	int32_t 		s32;
	typedef 	int64_t 		s64;

	typedef 	uint8_t 		u8;
	typedef 	uint16_t		u16;
	typedef 	uint32_t		u32;
	typedef 	uint64_t		u64;
}