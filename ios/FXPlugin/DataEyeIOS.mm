//
//  DataEyeIOS.m
//  LostStar
//
//  Created by scf on 15/4/20.
//
//

#include "DataEyeIOS.h"
#include "DCAgent.h"


@implementation DataEyeIOS


-(void)initData
{
    DCAgent::setReportMode(DCReportMode::DC_DEFAULT);
    //DCAgent::onStart("D8D847C8D27E06E5979E1D21861B6880", "000001");
    DCAgent::onStart("B302EBC1848676982DB670A33A4751D4", "000001");
}

//获取单例对象
+(DataEyeIOS*)getInstance
{
    static DataEyeIOS* s_dataeyeInstance = nil;
    if (s_dataeyeInstance == nil)
    {
        s_dataeyeInstance = [DataEyeIOS alloc];
        [s_dataeyeInstance initData];
    }
    return s_dataeyeInstance;
}

@end
