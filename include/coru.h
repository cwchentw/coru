#ifndef CORU_H
#define CORU_H

#ifdef _WIN32
    #include <windows.h>
#endif

#ifdef __cplusplus
    #include <cstdio>
#else
    #include <stdio.h>
#endif

/* Custom boolean type */
#ifndef _WIN32
#ifdef __cplusplus
    #ifndef _BOOL_IS_DEFINED
        typedef bool BOOL;
        #define FALSE  false
        #define TRUE   true
        #define _BOOL_IS_DEFINED
    #endif  /* BOOL */
#else
    #if __STDC_VERSION__ < 199901L
        #ifndef _BOOL_IS_DEFINED
            typedef unsigned char BOOL;
            #define FALSE  0
            #define TRUE   1
            #define _BOOL_IS_DEFINED
        #endif  /* BOOL */
    #else
        #ifndef _BOOL_IS_DEFINED
            #include <stdbool.h>
            typedef bool BOOL;
            #define FALSE  false
            #define TRUE   true
            #define _BOOL_IS_DEFINED
        #endif  /* BOOL */
    #endif  /* C89 */
#endif  /* __cplusplus */
#endif

/* Valid target language. */
#ifndef _LANGUAGE_TYPE_IS_DEFINED
    typedef unsigned char language_t;
    #define _LANGUAGE_TYPE_IS_DEFINED
#endif

#define  LANGUAGE_UNKNOWN   0
#define  LANGUAGE_C         1
#define  LANGUAGE_CPP       2
#define  LANGUAGE_FORTRAN   3
#define  LANGUAGE_JAVA      4
#define  LANGUAGE_CSHARP    5
#define  LANGUAGE_PERL      6
#define  LANGUAGE_PYTHON    7
#define  LANGUAGE_RUBY      8
#define  LANGUAGE_PHP       9
#define  LANGUAGE_JS       10
#define  LANGUAGE_GO       11
#define  LANGUAGE_RUST     12
#define  LANGUAGE_OBJC     13
#define  LANGUAGE_OBJCPP   14
#define  LANGUAGE_SWIFT    15
#define  LANGUAGE_CSH      16
#define  LANGUAGE_SH       17
#define  LANGUAGE_CMAKE    18
#define  LANGUAGE_MAKE     19

#ifdef __cplusplus
extern "C" {
#endif

typedef struct coru_stats_t coru_stats_t;

coru_stats_t * coru_stats_load(FILE *stream);
void coru_stats_delete(void *self);

BOOL coru_load_all(FILE *stream, coru_stats_t *stats, language_t lang, char **out);
BOOL coru_load_non_empty(FILE *stream, coru_stats_t *stats, language_t lang, char **out);

#ifdef __cplusplus
}
#endif

#endif  /* CORU_H */
