#include "config.hpp"

#include "log.hpp"

#include <cstdio>


#ifdef DEBUG
void log_debug
(
 const char * const s
)
noexcept
{
	std::fputs("debug: ", stderr);
	std::fputs(s, stderr);
	std::fputc('\n', stderr);
}
#else
void log_debug
(
 const char * const
)
noexcept
{ }
#endif

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
