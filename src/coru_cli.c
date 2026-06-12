#include <stdlib.h>
#include "boolean.h"
#include "coru.h"
#include "coru_argument.h"
#include "coru_command.h"
#include "coru_help.h"
#include "coru_metadata.h"
#include "coru_stats.h"
#include "print.h"

static BOOL coru_run(int argc, char **argv);

int main(int argc, char *argv[])
{
    if (!coru_run(argc, argv)) return 1;

    return 0;
}

static BOOL coru_run_load(coru_argument_t * arg);

static BOOL coru_run(int argc, char **argv)
{
    coru_argument_t arg;

    if (coru_argument_parse(&arg, argc, argv))
        goto ERROR_CORU;

    CORU_COMMAND cmd = coru_argument_command(&arg);

    if (is_coru_command_equal(cmd, CORU_COMMAND_VERSION)) {
        coru_help_version();
    }
    else if (is_coru_command_equal(cmd, CORU_COMMAND_LICENSE)) {
        coru_help_license();
    }
    else if (is_coru_command_equal(cmd, CORU_COMMAND_HELP)) {
        coru_help_help(stdout);
    }
    else if (is_coru_command_equal(cmd, CORU_COMMAND_TOO_FEW)) {
        PUTERR("No input file");
        goto ERROR_CORU;
    }
    else if (is_coru_command_equal(cmd, CORU_COMMAND_LOAD)) {
        if (!coru_run_load(&arg)) {
            PUTERR("Failed to load target file");
            goto ERROR_CORU;
        }
    }
    else if (is_coru_command_equal(cmd, CORU_COMMAND_TOO_MANY)) {
        PUTERR("%s only accepts single file", CORU_PROGRAM);
        goto ERROR_CORU;
    }
    else {
        PUTERR("Unknown option");
        coru_help_help(stderr);
        goto ERROR_CORU;
    }

    return TRUE;

ERROR_CORU:
    return FALSE;
}

static BOOL coru_run_load(coru_argument_t * arg)
{
    FILE *fp = NULL;
    coru_doc_t *doc = NULL;

#if _MSC_VER
    if (0 != fopen_s(&fp, coru_argument_path(arg), "r"))
        goto ERROR_LOAD;
#else
    fp = fopen(coru_argument_path(arg), "r");
    if (!fp)
        goto ERROR_LOAD;
#endif

    fclose(fp);
    fp = NULL;

    language_t lang = LANGUAGE_UNKNOWN;

    if (LANGUAGE_UNKNOWN != coru_argument_language(arg)) {
        lang =  coru_argument_language(arg);
    }
    else {
        /* detect_target_language() may detect target language by
           opening target source. Hence, we put the statement before
           fopen() statement. */
        lang = detect_target_language(coru_argument_path(arg));
    }

    if (is_language_equal(LANGUAGE_UNKNOWN, lang))
        goto ERROR_LOAD;

#if DEBUG
    if (is_language_equal(lang, LANGUAGE_UNKNOWN)) {
        PUTERR("Unsupported language");
    } else {
        PUTS("Target language: %s", language_to_string(lang));
    }
#endif

#if _MSC_VER
    if (0 != fopen_s(&fp, coru_argument_path(arg), "r")) {
    #if DEBUG
        PUTERR("Failed to open file at %s", coru_argument_path(arg));
    #endif
        goto ERROR_LOAD;
    }
#else
    fp = fopen(coru_argument_path(arg), "r");
    if (!fp) {
    #if DEBUG
        PUTERR("Failed to open file at %s", coru_argument_path(arg));
    #endif
        goto ERROR_LOAD;
    }
#endif

#if DEBUG
    PUTS("Source width: %lu", coru_stats_width(stats));
    PUTS("Source height: %lu", coru_stats_height(stats));
    /* Add stats for non-comment lines. */
#endif

    BOOL is_all = coru_argument_is_all(arg);

    if (is_all) {
        doc = coru_doc_load_all_fs(fp, lang);
        if (!doc) {
            goto ERROR_LOAD;
        }
    }
    else {
        doc = coru_doc_load_non_empty_fs(fp, lang);
        if (!doc) {
            goto ERROR_LOAD;
        }
    }

    PRINT("%s", coru_doc_string(doc));

    fclose(fp);
    coru_doc_delete(doc);

    return TRUE;

ERROR_LOAD:
    if (fp)
        fclose(fp);

    if (doc)
        coru_doc_delete(doc);

    return FALSE;
}
