#pragma once

#define __STDLIB_NOMANGLE	extern "C"
#define __STDLIB_UNUSED_PARAMETER(P) static_cast<void>(P)

typedef unsigned int size_t;

//Globals needed by CPP runtime
__STDLIB_NOMANGLE void atexit();
__STDLIB_NOMANGLE void* operator new(size_t count) throw();
__STDLIB_NOMANGLE void* operator new[](size_t count) throw();
__STDLIB_NOMANGLE void operator delete(void* ptr);
__STDLIB_NOMANGLE void operator delete[](void* ptr);