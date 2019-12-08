#include "boolean.h"
#include "help.h"
#include "metadata.h"
#include "utils.h"

void help_version(void)
{
    PUTS("%s", CORU_VERSION);
}

void help_license(void)
{
    PUTS("%s", CORU_LICENSE);
}
