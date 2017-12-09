#include "../../config/debug.h"
#include <stdio.h>

#define diagnosis(s) \
	fputs(s "\n", stderr)

#ifdef DEBUG
	void log_debug
	(
	 const char * const
	);
#else
	#define log_debug(...)
#endif

void warn
(
 const char * const
);

void err
(
 const char * const
);
