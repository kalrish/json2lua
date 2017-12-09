#include "config.hpp"

#include "utf8.hpp"

#include <cstdint>


namespace {
	/*
	 * UTF-8 decoder adapted from http://bjoern.hoehrmann.de/utf-8/decoder/dfa/
	 * 
	 * Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>
	*/
	
	constexpr auto UTF8_ACCEPT = 0;
	constexpr auto UTF8_REJECT [[maybe_unused]] = 12;
	
	const uint8_t utf8d[] = {
		// The first part of the table maps bytes to character classes that
		// to reduce the size of the transition table and create bitmasks.
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,
		
		// The second part is a transition table that maps a combination
		// of a state of the automaton and a character class to a state.
		0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
		12,0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
		12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
		12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
		12,36,12,12,12,12,12,12,12,12,12,12
	};
	
	std::uint_fast32_t decode_step
	(
	 std::uint_fast32_t state,
	 char32_t * const codep,
	 std::uint_fast32_t byte
	)
	noexcept
	{
		std::uint_fast32_t type = utf8d[byte];
		
		*codep = (state != UTF8_ACCEPT) ? (byte & 0x3Fu) | (*codep << 6) : (0xFF >> type) & (byte);
		
		return utf8d[256 + state + type];
	}
}

const char32_t * decode_utf8
(
 const unsigned char * buffer,
 std::size_t read,
 char32_t * output
)
noexcept
{
	std::uint_fast32_t state = 0;
	
	while ( read > 0 )
	{
		state = decode_step(state, output, *buffer);
		
		if ( !state )
			++output;
		
		++buffer;
		--read;
	}
	
	return output;
}

std::size_t encode_utf8
(
 const char32_t code_point,
 unsigned char p[4]
)
noexcept
{
	std::size_t n;
	
	if ( code_point <= 127 )
	{
		*p = code_point;
		
		n = 1;
	}
	else
	{
		if ( code_point <= 0x7FF )
		{
			*p++ = (code_point >> 6) | 0b11000000u;
			
			n = 2;
		}
		else
		{
			if ( code_point <= 0xFFFF )
			{
				*p++ = (code_point >> 12) | 0b11100000u;
				
				n = 3;
			}
			else
			{
				*p++ = (code_point >> 18) | 0b11110000u;
				*p++ = ((code_point >> 12) & 0b111111u) | 0b10000000u;
				
				n = 4;
			}
			
			*p++ = ((code_point >> 6) & 0b111111u) | 0b10000000u;
		}
		
		*p = (code_point & 0b111111u) | 0b10000000u;
	}
	
	return n;
}
