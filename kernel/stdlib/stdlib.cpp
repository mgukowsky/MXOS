#include <stdlib.h>

__STDLIB_NOMANGLE void atexit() {}

__STDLIB_NOMANGLE void* operator new(size_t count) throw() {
	__STDLIB_UNUSED_PARAMETER(count);
	return nullptr;
}

__STDLIB_NOMANGLE void* operator new[](size_t count) throw() {
	__STDLIB_UNUSED_PARAMETER(count);
	return nullptr;
}

__STDLIB_NOMANGLE void operator delete(void* ptr) {
	__STDLIB_UNUSED_PARAMETER(ptr);
}

__STDLIB_NOMANGLE void operator delete[](void* ptr) {
	__STDLIB_UNUSED_PARAMETER(ptr);
}
