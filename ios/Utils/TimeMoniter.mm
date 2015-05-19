//
//  TimeMoniter.m
//  LostStar
//
//  Created by Camellia_Mac on 15/4/23.
//
//

#import <Foundation/Foundation.h>
#import "TimeMoniter.h"

#include <functional>
#include <vector>
#include <utility>

static std::vector<std::pair<NSTimer*, std::function<void()>>> s_timeline;


@implementation TimeMoniter


//获取单例对象
+(TimeMoniter*)getInstance
{
    static TimeMoniter* s_timeMoniterInstance = nil;
    if (s_timeMoniterInstance == nil)
    {
        s_timeMoniterInstance = [TimeMoniter alloc];
    }
    return s_timeMoniterInstance;
}

-(void)scheduleOnce:(std::function<void()>)callback withTime:(float)time
{
    
    
    NSTimer *paintingTimer = [NSTimer scheduledTimerWithTimeInterval:time
                                                                       target:self
                                                                     selector:@selector(onOnceCallback:)  userInfo:nil
                                                             repeats:NO];
    s_timeline.push_back(std::make_pair(paintingTimer, callback));
}
// 定时器执行的方法
- (void)onOnceCallback:(NSTimer *)paramTimer{
    
    NSLog(@"触发回调方法onOnceCallback");
//    [paramTimer invalidate];
//    paramTimer = nil;
    
    for (int i=0,count=s_timeline.size(); i<count; ++i)
    {
        if (s_timeline[i].first == paramTimer)
        {
            NSLog(@"找到指定定时器，并开始执行回调");
            s_timeline[i].second();
            s_timeline.erase(s_timeline.begin()+i);
            return;
        }
    }
}

@end