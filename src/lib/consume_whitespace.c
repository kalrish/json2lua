#include <stdlib.h>
#include "consume_whitespace.h"

struct pointer_pair consume_whitespace
(
 struct pointer_pair ends,
 struct pointer_pair (* reader)(void *),
 void * const reader_stuff
)
{
again:
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
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			++ends.beg;
			goto again;
		default:
			return ends;
	}
}
