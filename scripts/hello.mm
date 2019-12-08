#import <Foundation/Foundation.h>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main(void)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    if (!pool) {
        cerr << "Failed to allocate auto release pool" << endl;
        cerr << "Check available system memory" << endl;
        return 1;
    }

    NSString *greet = [[[NSString alloc] initWithString: @"Hello World"] autorelease];
    string s([greet UTF8String]);

    cout << s << endl;

    [pool drain];

    return 0;
}
