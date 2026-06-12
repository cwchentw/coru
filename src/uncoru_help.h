#ifndef UNCORU_HELP_H
#define UNCORU_HELP_H

#include <stdio.h>
#include "uncoru_metadata.h"
#include "print.h"

#define uncoru_help_version(x) PUTS("%s", UNCORU_VERSION)
#define uncoru_help_license(x) PUTS("%s", UNCORU_LICENSE)

void uncoru_help_help(FILE *stream);

#endif  /* UNCORU_HELP_H */
