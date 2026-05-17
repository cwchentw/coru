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
#define  LANGUAGE_PASCAL    3
#define  LANGUAGE_FORTRAN   4
#define  LANGUAGE_CL        5
#define  LANGUAGE_JAVA      6
#define  LANGUAGE_CSHARP    7
#define  LANGUAGE_PERL      8
#define  LANGUAGE_PYTHON    9
#define  LANGUAGE_RUBY     10
#define  LANGUAGE_PHP      11
#define  LANGUAGE_JS       12
#define  LANGUAGE_GO       13
#define  LANGUAGE_RUST     14
#define  LANGUAGE_OBJC     15
#define  LANGUAGE_OBJCPP   16
#define  LANGUAGE_SWIFT    17
#define  LANGUAGE_CSH      18
#define  LANGUAGE_SH       19
#define  LANGUAGE_PS       20
#define  LANGUAGE_CMAKE    21
#define  LANGUAGE_MAKE     22

#ifdef __cplusplus
extern "C" {
#endif

typedef struct coru_doc_t coru_doc_t;

coru_doc_t * coru_doc_load_all_fs(FILE *stream, language_t lang);
coru_doc_t * coru_doc_load_non_empty_fs(FILE *stream, language_t lang);
const char * coru_doc_string(const coru_doc_t *self);
void coru_doc_delete(coru_doc_t *self);

#ifdef __cplusplus
}
#endif

#endif  /* CORU_H */
