#ifndef CORU_HELP_H
#define CORU_HELP_H

#include <stdio.h>
#include "coru_metadata.h"
#include "print.h"

#define coru_help_version(x) PUTS("%s", CORU_VERSION)
#define coru_help_license(x) PUTS("%s", CORU_LICENSE)

void coru_help_help(FILE *stream);

#endif  /* CORU_HELP_H */
