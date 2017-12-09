#include "parse_value.h"

#include "consume_whitespace.h"
#include "parse_array.h"
#include "parse_object.h"
#include "parse_string.h"


struct pointer_pair parse_value
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
	ends = consume_whitespace(ends, reader, reader_stuff);
	
	if ( ends.beg == NULL )
	{
		return ends;
	}
	
	if ( ends.beg == ends.end )
	{
		ends = reader(reader_stuff);
		if ( ends.beg == NULL )
		{
			return ends;
		}
	}
	
	switch ( *ends.beg )
	{
		case '{': // object
			++ends.beg;
			if ( ends.beg == ends.end )
			{
				ends = reader(reader_stuff);
				if ( ends.beg == NULL )
				{
					return ends;
				}
			}
			
			return parse_object(ends, reader, reader_stuff, writer, writer_stuff);
		case '[': // array
			//return ++cur != end ? parse_array(cur, end, output) : NULL;
		case '"': // string
			++ends.beg;
			if ( ends.beg == ends.end )
			{
				ends = reader(reader_stuff);
				if ( ends.beg == NULL )
				{
					return ends;
				}
			}
			
			return parse_string(ends, reader, reader_stuff, writer, writer_stuff);
		case 't': // true  - boolean
		case 'f': // false - boolean
			break;
		case 'n': // null
			break;
	}
	
	return ends;
}
