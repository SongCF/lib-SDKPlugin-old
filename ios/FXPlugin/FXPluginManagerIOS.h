/*
 * 插件管理类（统一管理各种插件）
 */


#pragma once


#import <Foundation/Foundation.h>
#import "FacebookIOS.h"
#import "AppStoreIOS.h"
#import "TalkingDataIOS.h"
#import "DataEyeIOS.h"

class FXPluginManagerIOS
{
private:
    static FXPluginManagerIOS* _instancePluginManager;
    FXPluginManagerIOS(){}
public:
    static FXPluginManagerIOS* getInstance();
    void loadPlugin();
    
    
    void didFinishLaunchingWithOptions(UIApplication *application, NSDictionary *launchOptions);
    void applicationDidBecomeActive(UIApplication *application);
    bool applicationOpenURLSourceApplicationAnnotation(UIApplication *application, NSURL *url, NSString *sourceApplication, id annotation);
    
public:
    FacebookIOS *_facebook;
    AppStoreIOS *_appstore;
    TalkingDataIOS *_talkingdata;
    DataEyeIOS* _dataeye;
};
