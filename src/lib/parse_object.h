#include <stdlib.h>
#include "pointer_pair.h"

struct pointer_pair parse_object
(
 struct pointer_pair,
 struct pointer_pair (* const)
 (
  void * restrict
 ),
 void * const restrict,
 int (* const)
 (
  const unsigned char * restrict,
  size_t,
  void * restrict
 ),
 void * const restrict
);
