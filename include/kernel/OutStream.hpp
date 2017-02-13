#pragma once

#include <defs.hpp>

namespace KERN {

class __IOManip {
friend class OutStream;
protected:
	__IOManip() = default;
	virtual ~__IOManip() = default;

	enum class ID {
		ENDL
	};

	virtual ID get_id() = 0;
};

class __Endl final : public __IOManip {
protected:
	ID get_id() { return ID::ENDL; }
};

static __Endl endl;

class OutStream {
public:
	OutStream() = default;
	~OutStream() = default;

	template<typename T>
	OutStream& operator<<(const T val) {
		_print_number_as_hex<T>(val);
		return *this;
	}

	OutStream& operator<<(const char *cstr);

private:
	static const char * _HEX_PREFIX;
	static constexpr u16 _INTERNAL_BUFFER_SIZE = 0x1000;

	template<typename T>
	void _print_number_as_hex(const T num){
		OutStream& stream = *this;
		char *outBuffPtr = _outBuff;

		stream << _HEX_PREFIX;

		const int DIGIT_LIMIT = sizeof(T)*2; //2 hex chars per byte

		for(int digitIdx = 0; digitIdx < DIGIT_LIMIT; ++digitIdx){
			T numBuff = num;
			int shiftAmt = (DIGIT_LIMIT - digitIdx - 1) << 2;
			char current_char = (numBuff >> shiftAmt) & 0x0F;
			if(current_char < 0x0A){
				current_char += '0';
			} else {
				current_char +=  ('A' - 0x0A);
			}
			*outBuffPtr = current_char;
			++outBuffPtr;
		}

		*outBuffPtr = '\0';

		stream << reinterpret_cast<const char*>(_outBuff);
	}

	char _outBuff[_INTERNAL_BUFFER_SIZE];
};

static OutStream kout;

} //namespace KERN