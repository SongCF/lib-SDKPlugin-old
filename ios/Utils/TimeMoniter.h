//
//  TimeMoniter.h
//  LostStar
//
//  Created by scf on 15/4/23.
//
//

#import <UIKit/UIKit.h>

#include <functional>


@interface TimeMoniter : NSObject {
    
    
}

//获取单例对象
+(TimeMoniter*)getInstance;

-(void)scheduleOnce:(std::function<void()>)callback withTime:(float)time;


@end

