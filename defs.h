#ifndef __BDIFF_DEFS_H
#define __BDIFF_DEFS_H

#include <stdint.h>

/* keys table */
#define quit 0
#define next 1
#define repl 2
#define addn 3
#define delt 4

/* args length (zero means first argument is the len) */
#define nextlen 1
#define repllen 0
#define addnlen 0

/* functions */
uint8_t * load_cmd   (uint8_t * cmd);
void      print_file (void);


#endif // __BDIFF_DEFS_H
