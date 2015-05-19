//
//  TalkingDataIOS.cpp
//  LostStarFree
//
//  Created by camelliacamellia on 15/4/15.
//
//

#import "TalkingDataIOS.h"
#import "TalkingDataGA.h"


@implementation TalkingDataIOS


-(void)initData
{
    //[TalkingDataGA onStart:@"C2FE5394A22E9FB3A924C657F808189F" withChannelId:@"000001"];
    [TalkingDataGA onStart:@"6BA3079B044EDB653CEED36AEA8FA08A" withChannelId:@"000001"];
}

//获取单例对象
+(TalkingDataIOS*)getInstance
{
    static TalkingDataIOS* s_talkingdataIOSInstance = nil;
    if (s_talkingdataIOSInstance == nil)
    {
        s_talkingdataIOSInstance = [TalkingDataIOS alloc];
        [s_talkingdataIOSInstance initData];
    }
    return s_talkingdataIOSInstance;
}

@end