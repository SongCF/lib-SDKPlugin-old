
#import "IOSWrapper.h"

void IOSWrapper::openUrl(const char *Url)
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:Url]]];
}