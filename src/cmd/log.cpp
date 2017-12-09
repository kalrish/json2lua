#include "config.hpp"

#include "log.hpp"

#include "config/debug_logs.hpp"
#include <cstdio>


void log_debug
(
 const char * const s
)
noexcept
{
	if constexpr ( include_debug_logs )
	{
		std::fputs("debug: ", stderr);
		std::fputs(s, stderr);
		std::fputc('\n', stderr);
	}
}

void warn
(
 const char * const s
)
noexcept
{
	std::fputs("warning: ", stderr);
	std::fputs(s, stderr);
	std::fputc('\n', stderr);
}

void err
(
 const char * const s
)
noexcept
{
	std::fputs("error: ", stderr);
	std::fputs(s, stderr);
	std::fputc('\n', stderr);
}
