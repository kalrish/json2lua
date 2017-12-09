#include <cstdio>

#include "config/buffer_size.hpp"
#include "../lib/pointer_pair.hpp"


class cbc final
{
	private:
		char32_t buffer[buffer_size];
		unsigned char string_code_units[4];
		
	public:
		std::FILE * input;
		std::FILE * output;
		
		json2lua::pointer_pair<char32_t>
			read
			( );
		
		void
			write
			(
			 const unsigned char
			);
		
		void
			write
			(
			 const unsigned char * const,
			 const std::size_t
			);
		
		json2lua::pointer_pair<unsigned char>
			encode_string_code_point
			(
			 const char32_t
			);
};
