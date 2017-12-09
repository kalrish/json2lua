#include "parse_object.h"

#include <stdio.h>
#include "consume_whitespace.h"
#include "parse_string.h"
#include "parse_value.h"


struct pointer_pair parse_object
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
	static const unsigned char brace_open = '{';
	static const unsigned char brace_close = '}';
	static const unsigned char bracket_open = '[';
	static const unsigned char bracket_close_and_equal_sign[2] = "]=";
	
	writer(&brace_open, 1, writer_stuff);
	
again:
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
	
	++ends.beg;
	if ( ends.beg == ends.end )
	{
		ends = reader(reader_stuff);
		if ( ends.beg == NULL )
		{
			return ends;
		}
	}
	
	writer(&bracket_open, 1, writer_stuff);
	
	parse_string(ends, reader, reader_stuff, writer, writer_stuff);
	
	writer(bracket_close_and_equal_sign, sizeof(bracket_close_and_equal_sign), writer_stuff);
	
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
	
	++ends.beg;
	if ( ends.beg == ends.end )
	{
		ends = reader(reader_stuff);
		if ( ends.beg == NULL )
		{
			return ends;
		}
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
	fprintf(stderr, "chars are '%c', '%c', '%c', '%c', '%c'\n", *(ends.beg-3), *(ends.beg-2), *(ends.beg-1), *ends.beg, *(ends.beg+1));
	
	/*
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
	}*/
	parse_value(ends, reader, reader_stuff, writer, writer_stuff);
	
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
	
	if ( *ends.beg == ',' )
	{
		goto again;
	}
	
	writer(&brace_close, 1, writer_stuff);
	
	return ends;
}
