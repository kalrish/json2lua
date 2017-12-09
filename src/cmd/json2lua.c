#include "json2lua.h"

#include <stdlib.h>
#include <stdio.h>
#include "../../config/buffer_size.h"
#include "log.h"
#include "../lib/parse_value.h"

static void decode_utf8
(
 const unsigned char * buffer,
 size_t read,
 uint_fast32_t * output
)
{
	while ( read > 0 )
	{
		*output++ = *buffer++;
		--read;
	}
}

struct reader_stuff
{
	uint_fast32_t buffer[buffer_size];
	FILE * input;
};

static struct pointer_pair reader
(
 void * const restrict argument
)
{
	struct reader_stuff * const stuff = (struct reader_stuff * const) argument;
	unsigned char buffer[buffer_size];
	struct pointer_pair rv;
	
	FILE * const input = stuff->input;
	uint_fast32_t * const code_points = stuff->buffer;
	
	log_debug("reader called");
	
	const size_t read = fread(buffer, 1, buffer_size, input);
	
	decode_utf8(buffer, read, code_points);
	
	if ( ferror(input) == 0 )
	{
		rv.beg = code_points;
		rv.end = rv.beg + read;
	}
	else
	{
		err("couldn't read input file");
	}
	
	return rv;
}

static int writer
(
 const unsigned char * const restrict buffer,
 size_t size,
 void * const restrict argument
)
{
	FILE * const output = (FILE * const) argument;
	
	log_debug("writer called");
	
	return fwrite(buffer, 1, size, output) == size;
}

int json2lua
(
 FILE * const input,
 FILE * const output
)
{
	struct pointer_pair ends;
	ends.beg = NULL;
	ends.end = NULL;
	struct reader_stuff stuff_in;
	stuff_in.input = input;
	
	if ( fputs("return", output) == EOF )
	{
		return EXIT_FAILURE;
	}
	
	ends = parse_value(ends, reader, &stuff_in, writer, output);
	
	return ends.beg != NULL ? EXIT_SUCCESS : EXIT_FAILURE;
}

