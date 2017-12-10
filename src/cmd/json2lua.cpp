#include "config.hpp"

#include "json2lua.hpp"

#include <cstdlib>
#include "log.hpp"
#include "cbc.hpp"
#include "../lib/process_value.hpp"
#include "../lib/process_string.hpp"
#include "../lib/unexpected_token.hpp"


int json2lua_main
(
 std::FILE * const input,
 std::FILE * const output
)
noexcept
{
	cbc cbo;
	cbo.input = input;
	cbo.output = output;
	
	try {
		const char return_str[] = { u8'r', u8'e', u8't', u8'u', u8'r', u8'n', u8' ' };
		
		cbo.write(return_str, sizeof(return_str));
		
		json2lua::pointer_pair<char32_t> ends{nullptr, nullptr};
		
		json2lua::process_value(ends, cbo);
		
		return EXIT_SUCCESS;
	}
	catch ( json2lua::unexpected_token e )
	{
		err("unexpected token");
	}
	catch ( json2lua::invalid_escape_sequence e )
	{
		err("invalid escape sequence");
	}
	catch ( ... )
	{
	}
	
	return EXIT_FAILURE;
}

