#include <stdio.h>
#include "platform.h"
#include "uncoru_help.h"
#include "uncoru_metadata.h"
#include "print_function.h"

void uncoru_help_version(void)
{
    PUTS("%s", UNCORU_VERSION);
}

void uncoru_help_license(void)
{
    PUTS("%s", UNCORU_LICENSE);
}

void uncoru_help_help(FILE *stream)
{
    fprintf(stream, "Usage: %s [option] path/to/source%s", UNCORU_PROGRAM, END_OF_LINE);
    fprintf(stream, "%s", END_OF_LINE);
    fprintf(stream, "Options:%s", END_OF_LINE);
    fprintf(stream, "  -v\t--version\tShow version info and exit%s", END_OF_LINE);
    fprintf(stream, "  --license\t\tShow license info and exit%s", END_OF_LINE);
    fprintf(stream, "  -h\t--help\t\tShow help info and exit%s", END_OF_LINE);
}
