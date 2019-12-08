#ifndef CORU_PLATFORM_H
#define CORU_PLATFORM_H

#ifdef _WIN32
    #define END_OF_LINE "\r\n"
#elif __unix__
    #define END_OF_LINE "\n"
#elif __APPLE__
    #define END_OF_LINE "\n"
#else
    #error "Unsupported platform"
#endif

#ifdef _WIN32
    #define PATH_SEPARATOR "\\"
#elif __unix__
    #define PATH_SEPARATOR "/"
#elif __APPLE__
    #define PATH_SEPARATOR "/"
#else
    #error "Unsupported platform"
#endif

#endif  /* CORU_PLATFORM_H */
