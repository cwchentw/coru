#include <stdio.h>
#include "boolean.h"
#include "coru_help.h"
#include "coru_metadata.h"
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
    fprintf(stream, "    -v\t--version\tShow version info and exit%s", END_OF_LINE);
    fprintf(stream, "    --license\t\tShow license info and exit%s", END_OF_LINE);
    fprintf(stream, "    -h\t--help\t\tShow help info and exit%s", END_OF_LINE);
    fprintf(stream, "%s", END_OF_LINE);
    fprintf(stream, "    -c\t\t\tTreat target source as C%s", END_OF_LINE);
    fprintf(stream, "    --as-c%s", END_OF_LINE);
    fprintf(stream, "    -cpp  -cxx  -c++\tTreat target source as C++%s", END_OF_LINE);
    fprintf(stream, "    --as-cpp%s", END_OF_LINE);
    fprintf(stream, "    --as-cxx%s", END_OF_LINE);
    fprintf(stream, "    --as-c++%s", END_OF_LINE);
    fprintf(stream, "    -java\t\tTreat target source as Java%s", END_OF_LINE);
    fprintf(stream, "    --as-java%s", END_OF_LINE);
    fprintf(stream, "    -c#  -csharp\tTreat target source as C# (C sharp)%s", END_OF_LINE);
    fprintf(stream, "    --as-c#%s", END_OF_LINE);
    fprintf(stream, "    --as-csharp%s", END_OF_LINE);
    fprintf(stream, "%s", END_OF_LINE);
    fprintf(stream, "Supported languages:%s", END_OF_LINE);
    fprintf(stream, "    C and C++%s", END_OF_LINE);
    fprintf(stream, "    Java%s", END_OF_LINE);
    fprintf(stream, "    C# (C sharp)%s", END_OF_LINE);
    fprintf(stream, "    Perl%s", END_OF_LINE);
    fprintf(stream, "    Python%s", END_OF_LINE);
    fprintf(stream, "    Ruby%s", END_OF_LINE);
    fprintf(stream, "    PHP%s", END_OF_LINE);
    fprintf(stream, "    JavaScript%s", END_OF_LINE);
    fprintf(stream, "    Golang%s", END_OF_LINE);
    fprintf(stream, "    Rust%s", END_OF_LINE);
    fprintf(stream, "    Objective-C (and Objective++)%s", END_OF_LINE);
    fprintf(stream, "    Swift%s", END_OF_LINE);
    fprintf(stream, "    C Shell%s", END_OF_LINE);
    fprintf(stream, "    Bourne Shell%s", END_OF_LINE);
}
/*345678901234567890123456789012345678901234567890123456789012345678901234567890*/
/*-------1---------2---------3---------4---------5---------6---------7---------8*/
