#include <TextModeIO.hpp>

using namespace KERN;
using namespace KERN::DRIVERS;
using KERN::LOW_LEVEL::outb;

TextModeIO::Glyph * TextModeIO::_pCurrentGlyph;
TextModeIO::Cursor TextModeIO::_cursor;

TextModeIO::Glyph * const TextModeIO::_pVideoMemory = 
	reinterpret_cast<TextModeIO::Glyph*>(0xB8000); 
const TextModeIO::Glyph * const TextModeIO::_pVideoMemoryEnd = 
	reinterpret_cast<TextModeIO::Glyph*>(0xB8000) +TextModeIO::_VIDEO_SIZE;

//Input MUST be a null-terminated string
void TextModeIO::print_string(const char *msg){
	u32 numCharsDrawn = 0;
	while(*msg != '\0'){
		switch(*msg) {
			case '\n':
				_cursor.newline();
				numCharsDrawn = 0;
				break;

			case '\t':
				_pCurrentGlyph += _TAB_SIZE;
				numCharsDrawn += _TAB_SIZE;
				break;

			default:
				_draw_glyph(*msg);
				++numCharsDrawn;
				break;
		}
		++msg;
	}
	_cursor.adjust(numCharsDrawn);
}

void TextModeIO::clear_screen(){
	_pCurrentGlyph = _pVideoMemory;

	while(_pCurrentGlyph < _pVideoMemoryEnd){
		_draw_glyph(' ', 0x02, true);
	}

	//Reset the cursor
	_pCurrentGlyph = _pVideoMemory;
	_cursor.set(0, 0);
}

void TextModeIO::scroll_screen(){
	Glyph *pGlyph = _pVideoMemory;

	while(pGlyph < _pVideoMemoryEnd){
		pGlyph->codePoint = (pGlyph + _NUM_COLUMNS)->codePoint;
		pGlyph->attrs = 0x02;
		++pGlyph;
	}

	//Clear out the bottom row
	pGlyph -= _NUM_COLUMNS;
	for(; pGlyph < _pVideoMemoryEnd; ++pGlyph){
		pGlyph->codePoint = ' ';
		pGlyph->attrs = 0x02;
	}
}

void TextModeIO::Cursor::set(u8 newCol, u8 newRow){
	col = newCol % _NUM_COLUMNS;
	row = newRow % _NUM_ROWS;
	const u16 newPos = (row * _NUM_COLUMNS) + col;
	position = (row * _NUM_COLUMNS) + col;
	_move_cursor();
	_pCurrentGlyph = _pVideoMemory + newPos;
}

void TextModeIO::Cursor::adjust(const u8 deltaCol, const u8 deltaRow){
	set(col + deltaCol, row + deltaRow);
}

void TextModeIO::Cursor::adjust(const u32 deltaChars){
	adjust(deltaChars % _NUM_COLUMNS, deltaChars / _NUM_COLUMNS);
}

void TextModeIO::Cursor::newline(){
	if(row == _NUM_ROWS-1){
		TextModeIO::scroll_screen();
		set(0, _NUM_ROWS-1);
	}
	else {
		set(0, row+1);
	}
}


void TextModeIO::Cursor::_move_cursor(){
	//Adjust the VGA controller, then send the appropriate position byte
	outb(LOW_LEVEL::Port::VGA_REG, 0x0F);
	outb(LOW_LEVEL::Port::VGA_DATA, static_cast<u8>(position & 0xFF));
	outb(LOW_LEVEL::Port::VGA_REG, 0x0E);
	outb(LOW_LEVEL::Port::VGA_DATA, static_cast<u8>((position >> 8) & 0xFF));
}