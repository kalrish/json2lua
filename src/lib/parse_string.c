#include "parse_string.h"

#include <stdlib.h>


static void output_code_unit
(
 const unsigned char code_unit,
 int (* const writer)
 (
	const unsigned char * restrict,
	size_t,
	void * restrict
 ),
 void * const restrict writer_stuff
)
{
	unsigned char output[4] = "\\00";
	unsigned char * output_p = output + sizeof(output) - 1;
	div_t result;
	
	result.quot = code_unit;
	
	do {
		result = div(result.quot, 8);
		
		*output_p-- = result.rem + '0';
	} while ( result.quot > 0 );
	
	writer(output, sizeof(output), writer_stuff);
}

static void output_code_point_utf8
(
 const uint_fast32_t code_point,
 int (* const writer)
 (
	const unsigned char * restrict,
	size_t,
	void * restrict
 ),
 void * const restrict writer_stuff
)
{
	if ( code_point <= 127 )
	{
		output_code_unit(code_point, writer, writer_stuff);
	}
}

enum string_parsing_mode
{
	literal,
	escaped
};

struct pointer_pair parse_string
(
 struct pointer_pair ends,
 struct pointer_pair (* const reader)
 (
	void * restrict
 ),
 void * const restrict reader_stuff,
 int (* const writer)
 (
	const unsigned char * restrict,
	size_t,
	void * restrict
 ),
 void * const restrict writer_stuff
)
{
	static const unsigned char double_quote = '"';
	
	enum string_parsing_mode mode = literal;
	
	writer(&double_quote, 1, writer_stuff);
	
again:
	switch ( mode )
	{
		case literal:
			switch ( *ends.beg )
			{
				case '"':
					writer(&double_quote, 1, writer_stuff);
					//++ends.beg;
					return ends;
				case '\\':
					mode = escaped;
					break;
				default:
					output_code_point_utf8(*ends.beg, writer, writer_stuff);
			}
			break;
		case escaped:
			switch ( *ends.beg )
			{
				case '"':
					break;
				case '\\':
					break;
				case '/':
					break;
				case 'b':
					break;
				case 'f':
					break;
				case 'n':
					break;
				case 'r':
					break;
				case 't':
					break;
				case 'u':
					break;
			}
			mode = literal;
	}
	
	++ends.beg;
	if ( ends.beg == ends.end )
	{
		ends = reader(reader_stuff);
		if ( ends.beg == NULL )
		{
			return ends;
		}
	}
	
	goto again;
}
