#ifndef UNCORU_H
#define UNCORU_H

/* Custom boolean type */
#ifdef __cplusplus
    typedef bool BOOL;
    #define FALSE  false
    #define TRUE   true
#else
    #if __STDC_VERSION__ < 199901L
        #ifndef BOOL
            typedef char BOOL;
            #define FALSE  0
            #define TRUE   1
        #endif
    #else
        #include <stdbool.h>
        typedef bool BOOL;
        #define FALSE  false
        #define TRUE   true
    #endif
#endif

BOOL uncoru_run(int argc, char **argv, char **out);

#endif  /* UNCORU_H */
