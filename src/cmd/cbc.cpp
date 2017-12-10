#include "config.hpp"

#include "cbc.hpp"

#include "log.hpp"
#include "../enc/utf8.hpp"


json2lua::pointer_pair<char32_t>
cbc::read
( )
{
	unsigned char buffer[buffer_size];
	
	const auto input = this->input;
	const auto code_points = this->buffer;
	
	auto read = std::fread(buffer, 1, sizeof(buffer)-3, input);
	
	if ( std::ferror(input) == 0 )
	{
		if ( read > 0 )
		{
			const auto last_byte = buffer[read-1];
			
			std::size_t additional = 0;
			
			if ( (last_byte & 0b11000000u) == 0b11000000u )
				additional = 1;
			else if ( (last_byte & 0b11100000u) == 0b11100000u )
				additional = 2;
			else if ( (last_byte & 0b11110000u) == 0b11110000u )
				additional = 3;
			
			if ( additional > 0 )
			{
				log_debug("must read more");
				
				const auto second_read = std::fread(buffer+read, 1, additional, input);
				if ( std::ferror(input) == 0 )
				{
					if ( second_read == additional )
					{
						read += second_read;
					}
					else
					{
						err("incomplete UTF-8 sequence");
						
						throw 4;
					}
				}
				else
				{
					err("couldn't read input file");
					
					throw 2;
				}
			}
			else if ( (last_byte & 0b10000000u) == 0b10000000u )
			{
				/* it's a continuation byte; read until the next non-continuation character */
				auto buffer_p = buffer+read;
				
read_another_character:
				const auto c = std::fgetc(input);
				if ( c != EOF )
				{
					*buffer_p = c;
					++read;
					
					if ( (c & 0b10000000u) == 0b10000000u )
					{
						++buffer_p;
						goto read_another_character;
					}
				}
				else
				{
					if ( std::feof(input) == 0 )
					{
						err("couldn't read input file");
					}
					else
					{
						err("incomplete UTF-8 sequence");
					}
					
					throw 2;
				}
			}
			
			return {
				code_points,
				decode_utf8(buffer, read, code_points)
			};
		}
		else
		{
			if ( std::feof(input) == 0 )
			{
				err("couldn't read anything");
			}
			else
			{
				err("incomplete JSON");
			}
			
			throw 3;
		}
	}
	else
	{
		err("couldn't read input file");
		
		throw 2;
	}
}

void
cbc::write
(
 const char c
)
{
	if ( std::fputc(c, this->output) == EOF )
	{
		err("couldn't write to the output file");
		
		throw 1;
	}
}

void
cbc::write
(
 const char * const buffer,
 std::size_t size
)
{
	if ( std::fwrite(buffer, 1, size, this->output) != size )
	{
		err("couldn't write to the output file");
		
		throw 1;
	}
}

json2lua::pointer_pair<char>
cbc::encode_string_code_point
(
 const char32_t code_point
)
{
	return {
		this->string_code_units,
		this->string_code_units + encode_utf8(code_point, this->string_code_units)
	};
}
