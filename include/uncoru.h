#ifndef UNCORU_H
#define UNCORU_H

#ifdef __cplusplus
    #include <cstdio>
#else
    #include <stdio.h>
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

typedef struct uncoru_doc_t uncoru_doc_t;

uncoru_doc_t * uncoru_doc_load_fs(FILE *stream, language_t lang);
const char * uncoru_doc_string(uncoru_doc_t *doc);
void uncoru_doc_delete(uncoru_doc_t *doc);

#ifdef __cplusplus
}  /* exetern "c" */
#endif

#endif  /* UNCORU_H */
