#include <stdlib.h>
#include "log.h"
#include "json2lua.h"

static int open_files
(
	const char * const input_file_name,
	const char * const output_file_name
)
{
	FILE * const input = fopen(input_file_name, "rb");
	FILE * const output = fopen(output_file_name, "wb");
	
	if ( input )
	{
		if ( output )
		{
			int rv = json2lua(input, output);
			
			if ( fclose(input) == EOF )
			{
				warn("couldn't close input file");
			}
			
			if ( fclose(output) == EOF )
			{
				warn("couldn't close output file");
			}
			
			return rv;
		}
		else
		{
			err("couldn't open output file");
			
			if ( fclose(input) == EOF )
			{
				err("couldn't close input file");
			}
			
			return EXIT_FAILURE;
		}
	}
	else
	{
		err("couldn't open input file");
		
		if ( output )
		{
			if ( fclose(output) == EOF )
			{
				warn("couldn't close output file");
			}
		}
		else
		{
			err("couldn't open output file");
		}
		
		return EXIT_FAILURE;
	}
}

int main
(
	int argc,
	char * * argv
)
{
	switch ( argc )
	{
		case 1:
			err("missing input and output file names");
			return EXIT_FAILURE;
		case 2:
			err("missing output file name");
			return EXIT_FAILURE;
		case 3:
			return open_files(argv[1], argv[2]);
		default:
			err("too many arguments");
			return EXIT_FAILURE;
	}
}
