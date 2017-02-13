#pragma once

#include <LOW_LEVEL.hpp>

namespace KERN {
namespace DRIVERS {

class TextModeIO {
public:

	//Input MUST be a null-terminated string
	static void print_string(const char *msg);
	
	static void clear_screen();
	static void scroll_screen();

private:
	struct Glyph {
		u8 codePoint;
		u8 attrs;

		inline void set_foreground_color(u8 color){
			color &= 0x0F;
			attrs &= 0xF0;
			attrs |= color;
		}

		inline void set_background_color(u8 color){
			color &= 0x70;
			attrs &= 0x8F;
			attrs |= color;
		}

		inline void set_blink(u8 blink){
			blink &= 0x80;
			attrs &= 0x7F;
			attrs |= blink;
		}
	};

	struct Cursor {
	public:
		u8 row;
		u8 col;
		u16 position; //VGA controller requires we add row and col together

		void set(u8 newCol, u8 newRow);
		void adjust(const u8 deltaCol, const u8 deltaRow);
		void adjust(const u32 deltaChars);

		void newline();

	private:
		void _move_cursor();
	};

	//Static class has no constructors/destructor
	TextModeIO() = delete;
	~TextModeIO() = delete;
	KERNEL_DISABLE_ALTERNATE_CTORS(TextModeIO);

	inline static void _draw_glyph(
		const char codePoint, const u8 attrs = 0, const bool useAttrs = false){

		_pCurrentGlyph->codePoint = codePoint;
		if(useAttrs){
			_pCurrentGlyph->set_foreground_color(attrs);
			_pCurrentGlyph->set_background_color(attrs);
			_pCurrentGlyph->set_blink(attrs);
		}

		++_pCurrentGlyph;
	}


	static Glyph *_pCurrentGlyph;
	static Cursor _cursor;

	//Dimensions of the text mode screen
	static constexpr u8  _NUM_COLUMNS = 80;
	static constexpr u8  _NUM_ROWS = 25;
	static constexpr u16 _VIDEO_SIZE = _NUM_COLUMNS * _NUM_ROWS;

	static constexpr u8  _TAB_SIZE = 8;

	static Glyph * const _pVideoMemory;
	//The end pointer is one past the end of video memory,
	//so we make it pointer-to-const to signal we should never
	//actually write to it
	static const Glyph * const _pVideoMemoryEnd; 
};

} //namespace DRIVERS
} //namespace KERN