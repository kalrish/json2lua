#include "config.hpp"

#include <cstdlib>
#include <cstdio>
#include <string_view>
#include "log.hpp"
#include "json2lua.hpp"


namespace
{
	void help()
	noexcept
	{
		std::puts("json2lua INPUT OUTPUT");
	}
	
	void version()
	noexcept
	{
		std::puts("json2lua 0.1");
	}
}

int main
(
 int argc,
 char * * argv
)
{
	using namespace std::literals::string_view_literals;
	
	bool parsing_arguments = true;
	bool input_set = false;
	bool output_set = false;
	std::FILE * input = stdin;
	std::FILE * output = stdout;
	bool close_input = false;
	bool close_output = false;
	int argi = 1;
	
	for ( char * * argp = argv+1 ; argp != argv+argc ; ++argp, ++argi )
	{
		char * arg = *argp;
		
		if ( parsing_arguments )
		{
			switch ( *arg )
			{
				case '-':
					++arg;
					switch ( *arg )
					{
						case 0: // stdin/stdout specifier
							if ( !input_set )
								input_set = true;
							else if ( !output_set )
								output_set = true;
							else
							{
								err("too many arguments");
								return EXIT_FAILURE;
							}
							
							break;
						case '-': // long option
							++arg;
							
							if ( *arg == 0 )
							{
								parsing_arguments = false;
							}
							else
							{
								if ( "version"sv.compare(arg) == 0 )
								{
									if ( argi == 1 )
									{
										version();
										return EXIT_SUCCESS;
									}
									else
									{
										err("--version only makes sense as first argument");
										return EXIT_FAILURE;
									}
								}
								else if ( "help"sv.compare(arg) == 0 )
								{
									if ( argi == 1 )
									{
										help();
										return EXIT_SUCCESS;
									}
									else
									{
										err("--help only makes sense as first argument");
										return EXIT_FAILURE;
									}
								}
							}
							
							break;
						default: // short option(s)
							err("short option specified");
							return EXIT_FAILURE;
					}
					break;
				default:
					goto open_file;
			}
		}
		else
		{
open_file:
			if ( !input_set )
			{
				input_set = true;
				input = std::fopen(arg, "rb");
				if ( input )
					close_input = true;
				else
					err("couldn't open input file");
			}
			else if ( !output_set )
			{
				output_set = true;
				output = std::fopen(arg, "wb");
				if ( output )
					close_output = true;
				else
					err("couldn't open output file");
			}
			else
			{
				err("too many arguments");
				return EXIT_FAILURE;
			}
		}
	}
	
	if ( input && output )
	{
		auto rc = json2lua_main(input, output);
		
		if ( close_input )
		{
			if ( std::fclose(input) == EOF )
				warn("couldn't close input file");
		}
		
		if ( close_output )
		{
			if ( std::fclose(output) == EOF )
				warn("couldn't close output file");
		}
		
		return rc;
	}
	else
		return EXIT_FAILURE;
}
