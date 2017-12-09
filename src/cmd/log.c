#include "log.h"

#include <stdio.h>


#ifdef DEBUG
void log_debug
(
 const char * const s
)
{
	fprintf(stderr, "debug: %s\n", s);
}
#endif

void warn
(
 const char * const s
)
{
	fprintf(stderr, "warning: %s\n", s);
}

void err
(
 const char * const s
)
{
	fprintf(stderr, "error: %s\n", s);
}
