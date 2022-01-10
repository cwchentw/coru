#import <Foundation/Foundation.h>
#include <stdio.h>

int main(void)
{
    NSAutoreleasePool *pool = \
        [[NSAutoreleasePool alloc] init];
    if (!pool) {
        fprintf(stderr,
            "Failed to allocate auto release pool\n");
        fprintf(stderr,
            "Check available system memory\n");
        return 1;
    }

    NSString *greet = \
        [[[NSString alloc]
            initWithString: @"Hello World"]
                autorelease];
    printf("%s\n", [greet UTF8String]);

    [pool drain];

    return 0;
}
