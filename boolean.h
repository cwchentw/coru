#ifndef BOOLEAN_H
#define BOOLEAN_H

#if __STDC_VERSION__ < 199901L
    #ifndef __cplusplus
        #ifndef BOOL
            typedef char BOOL;
            #define FALSE  0
            #define TRUE   1
        #endif
    #else
        typedef bool BOOL;
        #define FALSE  false
        #define TRUE   true
    #endif
#else
    #ifndef __cplusplus
        #include <stdbool.h>
    #else
        #include <cstdbool>
    #endif

    typedef bool   BOOL;
    #define FALSE  false
    #define TRUE   true
#endif  /* C89 or C99 above */

#endif  /* CORU_BOOLEAN_H */
