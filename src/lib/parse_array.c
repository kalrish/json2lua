#include "parse_value.h"

const unsigned char * parse_array
(
 const unsigned char * cur,
 const unsigned char * const end,
 FILE * const output
)
{
	if ( fputc('[', output) == EOF )
	{
		return NULL;
	}
	
	parse_value(cur, end, output);
	
	cur = consume_whitespace(cur, end);
	
	if ( cur != end )
	{
		if ( *cur == ']' )
		{
			if ( fputc(']', output) == EOF )
			{
				return NULL;
			}
			
			return ++cur;
		}
		else
		{
			return NULL;
		}
	}
}
