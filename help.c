#include <stdio.h>
#include "boolean.h"
#include "help.h"
#include "metadata.h"
#include "platform.h"
#include "utils.h"

void help_version(void)
{
    PUTS("%s", CORU_VERSION);
}

void help_license(void)
{
    PUTS("%s", CORU_LICENSE);
}

/*-------1---------2---------3---------4---------5---------6---------7---------8*/
/*345678901234567890123456789012345678901234567890123456789012345678901234567890*/
void help_help(FILE *stream)
{
    fprintf(stream, "Usage: %s [option] path/to/source%s", CORU_PROGRAM, END_OF_LINE);
    fprintf(stream, "%s", END_OF_LINE);
    fprintf(stream, "Options:%s", END_OF_LINE);
    fprintf(stream, "\t-v\t--version\tShow version info and exit%s", END_OF_LINE);
    fprintf(stream, "\t--license\t\tShow license info and exit%s", END_OF_LINE);
    fprintf(stream, "\t-h\t--help\t\tShow help info and exit%s", END_OF_LINE);
    fprintf(stream, "%s", END_OF_LINE);
    fprintf(stream, "Supported languages:%s", END_OF_LINE);
    fprintf(stream, "\tC and C++%s", END_OF_LINE);
    fprintf(stream, "\tJava%s", END_OF_LINE);
    fprintf(stream, "\tC# (C sharp)%s", END_OF_LINE);
    fprintf(stream, "\tPerl%s", END_OF_LINE);
    fprintf(stream, "\tPython%s", END_OF_LINE);
    fprintf(stream, "\tRuby%s", END_OF_LINE);
    fprintf(stream, "\tPHP%s", END_OF_LINE);
    fprintf(stream, "\tJavaScript%s", END_OF_LINE);
    fprintf(stream, "\tGolang%s", END_OF_LINE);
    fprintf(stream, "\tRust%s", END_OF_LINE);
    fprintf(stream, "\tObjective-C (and Objective++)%s", END_OF_LINE);
    fprintf(stream, "\tSwift%s", END_OF_LINE);
    fprintf(stream, "\tC Shell%s", END_OF_LINE);
    fprintf(stream, "\tBourne Shell%s", END_OF_LINE);
}
/*345678901234567890123456789012345678901234567890123456789012345678901234567890*/
/*-------1---------2---------3---------4---------5---------6---------7---------8*/
