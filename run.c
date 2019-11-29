#include <stdlib.h>
#include <string.h>
#include "argument.h"
#include "boolean.h"
#include "command.h"
#include "help.h"
#include "metadata.h"
#include "stats.h"
#include "utils.h"

BOOL lai_run(lai_argument_t *arg, char *out)
{
    if (is_command_equal(lai_argument_command(arg), COMMAND_VERSION)) {
        help_version();
        return TRUE;
    }
    else if (is_command_equal(lai_argument_command(arg), COMMAND_LICENSE)) {
        help_license();
        return TRUE;
    }
    else if (is_command_equal(lai_argument_command(arg), COMMAND_TOO_FEW)) {
        PUTERR("No input file");
        return FALSE;
    }
    else if (is_command_equal(lai_argument_command(arg), COMMAND_LOAD)) {
        lai_stats_t *stats = NULL;
        FILE *fp = NULL;
        char *line = NULL;

        stats = lai_stats_new();
        if (!stats) {
            PUTERR("Failed to load stats");
            goto ERROR_LOAD;
        }
        
        /* Use stat() to check that file at path is available. */

        /* Check this statement later. */
        /* detect_target_language() may detect target language by
           opening target source. Hence, we put the statement before
           fopen() statement. */
        char *lang = detect_target_language(lai_argument_path(arg));

        fp = fopen(lai_argument_path(arg), "r");
        if (!fp) {
            PUTERR("Failed to open file at %s", lai_argument_path(arg));
            goto ERROR_LOAD;
        }

#if DEBUG
        if (is_string_equal(lang, "")) {
            PUTERR("Unsupported language");
        }
        else {
            PUTS("Target language: %s", lang);   
        }
#endif

        size_t line_size = 150;  /* Sensible line size */
        line = (char *) malloc(line_size * sizeof(char));
        if (!line) {
            PUTERR("Failed to allocate line object");
            goto ERROR_LOAD;
        }

        while (fgets(line, line_size, fp)) {
            if (line_size == strlen(line)) {
                if ('\n' != line[line_size-1]) {
                    line_size <<= 1;
                    if (!realloc(line, line_size)) {
                        PUTERR("Failed to realloc line object");
                        goto ERROR_LOAD;
                    }
                }
                else goto LOAD_LINE;
            }
            else {
            LOAD_LINE:
                if (strlen(line) > lai_stats_width(stats)) {
                    lai_stats_set_width(stats, strlen(line));
                }

                lai_stats_set_height(stats, lai_stats_height(stats) + 1);
            }
        }

#if DEBUG
        PUTS("Source width: %lu", lai_stats_width(stats));
        PUTS("Source height: %lu", lai_stats_height(stats));
#endif

        /* The format of line number:
             *start*    1 *end*
           ^^ --> indent after original source code
              ^^^^^^ --> start word of comment
                    ^ --> one space
                     ^^^ --> indent for line number
                        ^ --> line number
                         ^ --> one space (optional)
                          ^^^^^ --> end word of comment (optional)
        */
        /* Implement two hash tables to store the start word and the end word
           of source code of specific language. */

        /* Free system resources. */
        free(line);
        fclose(fp);
        lai_stats_delete((void *) stats);

        return TRUE;

ERROR_LOAD:
        if (line)
            free(line);

        if (fp)
            fclose(fp);

        if (stats)
            lai_stats_delete((void *) stats);

        return FALSE;
    }
    else if (is_command_equal(lai_argument_command(arg), COMMAND_TOO_MANY)) {
        PUTERR("%s only accepts single file", LAI_PROGRAM);
        return FALSE;
    }
    else {
        PUTERR("Unknown option");
        return FALSE;
    }
}
