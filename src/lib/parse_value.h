#include <stddef.h>
#include "pointer_pair.h"

struct json2lua_pointer_pair_unicode json2lua_parse_value
(
 struct json2lua_pointer_pair_unicode,
 struct json2lua_pointer_pair_unicode (* const)
 (
  void * restrict
 ),
 void * const,
 int (* const)
 (
  const unsigned char * restrict,
  size_t,
  void * restrict
 ),
 void * const
);
