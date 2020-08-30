#include <stdlib.h>
#include "coru.h"
#include "coru_argument.h"
#include "coru_command.h"
#include "coru_help.h"
#include "coru_metadata.h"
#include "coru_stats.h"
#include "print.h"

BOOL coru_run(int argc, char **argv, char **out);

int main(int argc, char *argv[])
{
    char **out = (char **) malloc(sizeof(char *));
    if (!out)
        goto ERROR_CORU_CLI;

    *out = NULL;

    if (!coru_run(argc, argv, out))
        goto ERROR_CORU_CLI;

    if (*out)
        PRINT("%s", *out);

    free(*out);
    free(out);

    return 0;

ERROR_CORU_CLI:
    if (out && *out)
        free(*out);

    if (out)
        free(out);

    return 1;
}

BOOL coru_run_load(coru_argument_t * arg, char **out);

BOOL coru_run(int argc, char **argv, char **out)
{
    coru_argument_t *arg = coru_argument_parse(argc, argv);
    if (!arg)
        goto ERROR_CORU;

    CORU_COMMAND cmd = coru_argument_command(arg);

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
        if (!coru_run_load(arg, out)) {
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

    coru_argument_delete(arg);

    return TRUE;

ERROR_CORU:
    if (arg)
        coru_argument_delete(arg);

    return FALSE;
}

BOOL coru_run_load(coru_argument_t * arg, char **out)
{
    coru_stats_t *stats = NULL;
    FILE *fp = NULL;

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


    stats = coru_stats_load(fp);
    if (!stats)
        goto ERROR_LOAD;

    rewind(fp);

#if DEBUG
    PUTS("Source width: %lu", coru_stats_width(stats));
    PUTS("Source height: %lu", coru_stats_height(stats));
    /* Add stats for non-comment lines. */
#endif

    BOOL is_all = coru_argument_is_all(arg);

    if (is_all) {
        if (!coru_load_all_fs(fp, out, stats, lang)) {
            goto ERROR_LOAD;
        }
    }
    else {
        if (!coru_load_non_empty_s(fp, stats, lang, out)) {
            goto ERROR_LOAD;
        }
    }

    /* Free system resources. */
    fclose(fp);
    coru_stats_delete((void *) stats);

    return TRUE;

ERROR_LOAD:
    if (fp)
        fclose(fp);

    if (stats)
        coru_stats_delete((void *) stats);

    return FALSE;
}
