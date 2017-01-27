#pragma once

#include "stdlib/stdint.h"

#define KERNEL_NOMANGLE	extern "C" __cdecl

//Explicitly turn off copy/assignment/move constructors
#define KERNEL_DISABLE_ALTERNATE_CTORS(CLASS_T) \
	CLASS_T(const CLASS_T&) = delete; \
	CLASS_T& operator=(const CLASS_T&) = delete; \
	CLASS_T(CLASS_T&&) = delete; \
	CLASS_T& operator=(const CLASS_T&&) = delete
