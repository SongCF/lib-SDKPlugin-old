
#import "FXPluginManagerIOS.H"


FXPluginManagerIOS* FXPluginManagerIOS::_instancePluginManager = nullptr;


FXPluginManagerIOS* FXPluginManagerIOS::getInstance()
{
    if (!_instancePluginManager){
        _instancePluginManager = new FXPluginManagerIOS();
        _instancePluginManager->loadPlugin();
    }
    return _instancePluginManager;
}

void FXPluginManagerIOS::loadPlugin()
{
    _facebook = FacebookIOS::getInstance();
    _appstore = [AppStoreIOS getInstance];
    _talkingdata = [TalkingDataIOS getInstance];
    _dataeye = [DataEyeIOS getInstance];
}

void FXPluginManagerIOS::didFinishLaunchingWithOptions(UIApplication *application, NSDictionary *launchOptions)
{
    _facebook->didFinishLaunchingWithOptions(application, launchOptions);
}
void FXPluginManagerIOS::applicationDidBecomeActive(UIApplication *application)
{
    _facebook->applicationDidBecomeActive(application);
}
bool FXPluginManagerIOS::applicationOpenURLSourceApplicationAnnotation(UIApplication *application, NSURL *url, NSString *sourceApplication, id annotation)
{
    return _facebook->applicationOpenURLSourceApplicationAnnotation(application, url, sourceApplication, annotation);
}