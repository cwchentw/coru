#include <stdlib.h>
#include "language.h"
#include "print.h"
#include "uncoru.h"
#include "uncoru_argument.h"
#include "uncoru_command.h"
#include "uncoru_help.h"
#include "uncoru_metadata.h"
#include "uncoru_stats.h"

static BOOL uncoru_run(int argc, char **argv);

int main(int argc, char *argv[])
{
    if (!uncoru_run(argc, argv)) return 1;

    return 0;
}

static BOOL uncoru_run_load(uncoru_argument_t *arg);

static BOOL uncoru_run(int argc, char **argv)
{
    uncoru_argument_t *arg = uncoru_argument_parse(argc, argv);
    if (!arg)
        goto ERROR_UNCORU;

    UNCORU_COMMAND cmd = uncoru_argument_command(arg);

    if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_VERSION)) {
        uncoru_help_version();
    }
    else if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_LICENSE)) {
        uncoru_help_license();
    }
    else if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_HELP)) {
        uncoru_help_help(stdout);
    }
    else if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_TOO_FEW)) {
        PUTERR("No input file");
        goto ERROR_UNCORU;
    }
    else if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_LOAD)) {
        if (!uncoru_run_load(arg)) {
            PUTERR("Failed to load target file");
            goto ERROR_UNCORU;
        }
    }
    else if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_TOO_MANY)) {
        PUTERR("%s only accepts single file", UNCORU_PROGRAM);
        goto ERROR_UNCORU;
    }
    else {
        PUTERR("Unknown option");
        uncoru_help_help(stderr);
        goto ERROR_UNCORU;
    }

    uncoru_argument_delete(arg);

    return TRUE;

ERROR_UNCORU:
    if (arg)
        uncoru_argument_delete(arg);

    return FALSE;
}

static BOOL uncoru_run_load(uncoru_argument_t *arg)
{
    FILE *fp = NULL;
    uncoru_doc_t *doc = NULL;
    language_t lang;

    if (LANGUAGE_UNKNOWN != uncoru_argument_language(arg)) {
        lang =  uncoru_argument_language(arg);
    }
    else {
        /* detect_target_language() may detect target language by
           opening target source. Hence, we put the statement before
           fopen() statement. */
        lang = detect_target_language(uncoru_argument_path(arg));
    }

    if (is_language_equal(LANGUAGE_UNKNOWN, lang))
        goto ERROR_LOAD;

#if _MSC_VER
    if (0 != fopen_s(&fp, uncoru_argument_path(arg), "r"))
        goto ERROR_LOAD;
#else
    fp = fopen(uncoru_argument_path(arg), "r");
    if (!fp)
        goto ERROR_LOAD;
#endif

    doc = uncoru_doc_load_fs(fp, lang);
    if (!doc)
        goto ERROR_LOAD;

    PRINT("%s", uncoru_doc_string(doc));

    fclose(fp);
    uncoru_doc_delete(doc);

    return TRUE;

ERROR_LOAD:
    if (fp)
        fclose(fp);

    if (doc)
        uncoru_doc_delete(doc);

    return FALSE;
}
