
#import "FacebookIOS.h"
#import <FacebookSDK/FacebookSDK.h>
#import <FacebookSDK/FBRequestConnection.h>
#import "AppDelegate.h"

#include "TimeMoniter.h"

#define ID "ID"
#define NAME "name"
#define PICTURE "picture"


//native c++
extern void ios_loginCallback(int retCode, const char* msg);
extern void ios_shareCallback(int retCode, const char* msg);
extern void ios_inviteFriendsCallback(int retCode, const char* msg);


FacebookIOS* FacebookIOS::_instanceFacebookIOS = NULL;


FacebookIOS* FacebookIOS::getInstance()
{
    if (!_instanceFacebookIOS){
        _instanceFacebookIOS = new FacebookIOS();
    }
    return _instanceFacebookIOS;
}

void FacebookIOS::login()
{
//test
 /*   _userInfor[ID] = "1386146831694197";
    _userInfor[NAME] = "Camellia Camellia network";
    _userInfor[PICTURE] = "https://fbcdn-profile-a.akamaihd.net/hprofile-ak-xfa1/v/t1.0-1/p50x50/10551094_1386928481616032_6900265282582756767_n.jpg?oh=0348537dd972bc5681a9fe6713fa3494&oe=556B4862&__gda__=1431737572_816ed28b49a4e61c61d4e0e169cb9c8d";
    cacheUserInfor();
    loadUserInfor();*/
    
    
    
    // If the session state is any of the two "open" states when the button is clicked
    if (FBSession.activeSession.state == FBSessionStateOpen
        || FBSession.activeSession.state == FBSessionStateOpenTokenExtended) {
        
        // Close the session and remove the access token from the cache
        // The session state handler (in the app delegate) will be called automatically
        [FBSession.activeSession closeAndClearTokenInformation];
        
        // If the session state is not any of the two "open" states when the button is clicked
    } else {
        // Open a session showing the user the login UI
        // You must ALWAYS ask for public_profile permissions when opening a session
        [FBSession openActiveSessionWithReadPermissions:@[@"public_profile", @"user_friends"]
                                           allowLoginUI:YES
                                      completionHandler:
         ^(FBSession *session, FBSessionState state, NSError *error) {
             // Retrieve the app delegate
//             AppDelegate* appDelegate = [UIApplication sharedApplication].delegate;
//             // Call the app delegate's sessionStateChanged:state:error method to handle session state changes
//             [appDelegate sessionStateChanged:session state:state error:error];
             this->onSessionStateChanged(session, state, error);
         }];
    }
}
bool FacebookIOS::isLogin()
{
    bool ret = (FBSession.activeSession.state == FBSessionStateOpen);
    NSLog(@"%@", ret ? @"log in" : @"log out");
    return ret;
}
void FacebookIOS::logout()
{
	if (FBSession.activeSession) {
        [FBSession.activeSession closeAndClearTokenInformation];
    }
}


void FacebookIOS::inviteFriends(const char* inviteMsg)
{
    NSString * msg = [NSString stringWithUTF8String:inviteMsg];
    
    [FBWebDialogs presentRequestsDialogModallyWithSession:nil message:msg title:nil parameters:nil handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error) {
			if (error)
             {
				 ios_inviteFriendsCallback(2, [msg UTF8String]);
             }
             else
             {
                 if (result == FBWebDialogResultDialogNotCompleted)
                 {
					 ios_inviteFriendsCallback(1, "User cancelled invite friends.");
                 }
                 else if([[resultURL description] hasPrefix:@"fbconnect://success?request="]) 
                 {
                    // Facebook returns FBWebDialogResultDialogCompleted even user 
                    // presses "Cancel" button, so we differentiate it on the basis of
                    // url value, since it returns "Request" when we ACTUALLY
                    // completes Dialog
					 ios_inviteFriendsCallback(0, "");
                 }
                 else
                 {
					 ios_inviteFriendsCallback(1, "User cancelled invite friends.");
                 }
             }
    }];
}

void FacebookIOS::share(std::map<std::string,std::string>* shareInfo)
{
    NSString* link = @"";
    NSString* picture = @"";
    NSString* name = @"";
    NSString* caption = @"";
    NSString* linkDescription = @"";
    
    std::map<std::string,std::string>::iterator it;
    it = shareInfo->find("link");
    if(shareInfo->end() != it)
    {
        link = [NSString stringWithUTF8String:shareInfo->at("link").c_str()];
        it = shareInfo->find("name");
        if(shareInfo->end() != it) name = [NSString stringWithUTF8String:shareInfo->at("name").c_str()];
        it = shareInfo->find("caption");
        if(shareInfo->end() != it) caption = [NSString stringWithUTF8String:shareInfo->at("caption").c_str()];
        it = shareInfo->find("picture");
        if(shareInfo->end() != it) picture = [NSString stringWithUTF8String:shareInfo->at("picture").c_str()];
        it = shareInfo->find("description");
        if(shareInfo->end() != it) linkDescription = [NSString stringWithUTF8String:shareInfo->at("description").c_str()];
    }
    
    // Check if the Facebook app is installed and we can present the share dialog
    FBLinkShareParams *params = [[[FBLinkShareParams alloc] init] autorelease];
    
    params.link = [NSURL URLWithString:link];
    params.name = name;
    params.caption = caption;
    params.picture = [NSURL URLWithString:picture];
    //params.description = linkDescription;
    params.linkDescription = linkDescription;

    // If the Facebook app is installed and we can present the share dialog
    if ([FBDialogs canPresentShareDialogWithParams:params]) {
        // Present the share dialog
        [FBDialogs presentShareDialogWithLink:params.link
                                         name:params.name
                                      caption:params.caption
                                  description:params.linkDescription
                                      picture:params.picture
                                  clientState:nil
                                      handler:^(FBAppCall *call, NSDictionary *results, NSError *error) {
                                          if(error) {
                                              // An error occurred, we need to handle the error
                                              // See: https://developers.facebook.com/docs/ios/errors
                                              NSString *msg = [NSString stringWithFormat:@"Error publishing story: %@", error.description];
                                              NSLog(@"%@", msg);
                                              ios_shareCallback(2, [msg UTF8String]);
                                          } else {
                                              // Success
                                              NSLog(@"result %@", results);
                                              NSString* resultString = [results objectForKey:@"completionGesture"];
                                              if (resultString != nil && [resultString isEqual:@"post"]) {
                                                  ios_shareCallback(0, "");
                                              }
                                              else
                                              {
                                                  ios_shareCallback(1, "User cancelled");
                                              }
                                          }
                                      }];
    } else {
        // Present the feed dialog
		// Put together the dialog parameters
        
NSMutableDictionary *webparams = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                               name, @"name",
                               caption, @"caption",
                               linkDescription, @"description",
                               link, @"link",
                               picture, @"picture",
                               nil];

// Show the feed dialog
[FBWebDialogs presentFeedDialogModallyWithSession:nil
                                       parameters:webparams
                                          handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error) {
                                            if (error) {
                                              // An error occurred, we need to handle the error
                                              // See: https://developers.facebook.com/docs/ios/errors
                                                NSString *msg = [NSString stringWithFormat:@"Error publishing story: %@", error.description];
                                                NSLog(@"%@", msg);
                                                ios_shareCallback(2, [msg UTF8String]);
                                            } else {
                                              if (result == FBWebDialogResultDialogNotCompleted) {
                                                // User cancelled.
                                                  NSLog(@"User cancelled.");
                                                  ios_shareCallback(1, "User cancelled");
                                              } else {
                                                // Handle the publish feed callback
                                                  NSString *query = [resultURL query];
												  NSArray *pairs = [query componentsSeparatedByString:@"&"];
												  NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
												  for (NSString *pair in pairs) {
                                                      NSArray *kv = [pair componentsSeparatedByString:@"="];
                                                      NSString *val =
                                                      [kv[1] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
                                                      params[kv[0]] = val;
                                                }
                                                NSDictionary *urlParams = params;

                                                if (![urlParams valueForKey:@"post_id"]) {
                                                    // User cancelled.
                                                    NSLog(@"User cancelled.");
                                                    ios_shareCallback(1, "User cancelled");
                                                } else {
                                                    // User clicked the Share button
                                                    NSString *result = [NSString stringWithFormat: @"Posted story, id: %@", [urlParams valueForKey:@"post_id"]];
                                                    NSLog(@"result %@", result);
                                                    ios_shareCallback(0, "");
                                                }
                                              }
                                            }
                                          }];
    }
}
std::string FacebookIOS::getUserID()
{
    return getUserInfor()[ID];
}
std::map<std::string,std::string> FacebookIOS::getUserInfor()
{
    if (isLogin()){
        if (_userInfor[ID].size() <= 0){
            loadUserInfor();
        }
        return _userInfor;
    } else {
        _userInfor.clear();
        return _userInfor;
    }
}
std::string FacebookIOS::getAccessToken()
{
    if (isLogin()){
        return [FBSession.activeSession.accessTokenData.accessToken UTF8String];
    } else {
        _userInfor.clear();
        return "";
    }
}





void FacebookIOS::didFinishLaunchingWithOptions(void *application, void *launchOptions)
{
    // Whenever a person opens app, check for a cached session
    if (FBSession.activeSession.state == FBSessionStateCreatedTokenLoaded) {
        
        // If there's one, just open the session silently, without showing the user the login UI
        [FBSession openActiveSessionWithReadPermissions:@[@"public_profile", @"user_friends"]
                                           allowLoginUI:NO
                                      completionHandler:^(FBSession *sessison, FBSessionState state, NSError *error) {
                                          // Handler for session state changes
                                          // Call this method EACH time the session state changes,
                                          //  NOT just when the session open
                                          
                                          //[self sessionStateChanged:session state:state error:error];
                                          this->onSessionStateChanged(sessison, state, error);
                                      }];
    }
}
void FacebookIOS::applicationDidBecomeActive(void *application)
{
    // (analytics) log event to facebook
    [FBAppEvents activateApp];
    
    // handle cancle login
    [FBAppCall handleDidBecomeActive];
}
bool FacebookIOS::applicationOpenURLSourceApplicationAnnotation(void *application, void *url, void *sourceApplication, void* annotation)
{
    // Note this handler block should be the exact same as the handler passed to any open calls.
    [FBSession.activeSession setStateChangeHandler:
        ^(FBSession *session, FBSessionState state, NSError *error) {
//        // Retrieve the app delegate
//        AppDelegate* appDelegate = [UIApplication sharedApplication].delegate;
//        // Call the app delegate's sessionStateChanged:state:error method to handle session state changes
//        [appDelegate sessionStateChanged:session state:state error:error];
            this->onSessionStateChanged(session, state, error);
    }];
    
    //return [FBAppCall handleOpenURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication];
    BOOL urlWasHandled = [FBAppCall handleOpenURL:(NSURL *)url
                              sourceApplication:(NSString *)sourceApplication
                                fallbackHandler:^(FBAppCall *call) {
                                  NSLog(@"Unhandled deep link: %@", url);
                                  // Here goes the code to handle the links 
                                  // Use the links to show a relevant view of your app to the user
                                }];

	return urlWasHandled;
}


void FacebookIOS::onSessionStateChanged(void* psession, int pstate, void* perror)
{
    FBSession* session = (FBSession*)psession;
    FBSessionState state = (FBSessionState) pstate;
    NSError* error = (NSError*) perror;

    // If the session was opened successfully
    if (!error && state == FBSessionStateOpen){
        NSLog(@"Session opened");
        // Show the user the logged-in UI
        //[self userLoggedIn];
        //ios_loginCallback(0, "");
        initUserInfoAndCallback();
        return;
    }
    if (/*state == FBSessionStateClosed || */state == FBSessionStateClosedLoginFailed){
        // If the session is closed
        NSLog(@"Session closed");
        // Show the user the logged-out UI
        //[self userLoggedOut];
        ios_loginCallback(1, "log out");
        return;
    }
    
    // Handle errors
    if (error){
        NSLog(@"Error");
        NSString *alertText;
        NSString *alertTitle;
        // If the error requires people using an app to make an action outside of the app in order to recover
        if ([FBErrorUtility shouldNotifyUserForError:error] == YES){
            alertTitle = @"Something went wrong";
            alertText = [FBErrorUtility userMessageForError:error];
            //[self showMessage:alertText withTitle:alertTitle];
            ios_loginCallback(1, [alertText UTF8String]);
        } else {
            
            // If the user cancelled login, do nothing
            if ([FBErrorUtility errorCategoryForError:error] == FBErrorCategoryUserCancelled) {
                NSLog(@"User cancelled login");
                ios_loginCallback(1, "log in cancle");
                // Handle session closures that happen outside of the app
            } else if ([FBErrorUtility errorCategoryForError:error] == FBErrorCategoryAuthenticationReopenSession){
                alertTitle = @"Session Error";
                alertText = @"Your current session is no longer valid. Please log in again.";
                //[self showMessage:alertText withTitle:alertTitle];
                ios_loginCallback(1, [alertText UTF8String]);
                
                // Here we will handle all other errors with a generic error message.
                // We recommend you check our Handling Errors guide for more information
                // https://developers.facebook.com/docs/ios/errors/
            } else {
                //Get more error information from the error
                NSDictionary *errorInformation = [[[error.userInfo objectForKey:@"com.facebook.sdk:ParsedJSONResponseKey"] objectForKey:@"body"] objectForKey:@"error"];
                
                // Show the user an error message
                alertTitle = @"Something went wrong";
                alertText = [NSString stringWithFormat:@"Please retry. \n\n If the problem persists contact us and mention this error code: %@", [errorInformation objectForKey:@"message"]];
                //[self showMessage:alertText withTitle:alertTitle];
                ios_loginCallback(1, [alertText UTF8String]);
            }
        }
        // Clear this token
        [FBSession.activeSession closeAndClearTokenInformation];
        // Show the user the logged-out UI
        //[self userLoggedOut];
    }
}

void FacebookIOS::initUserInfoAndCallback()
{
    if (! isLogin()) {
        return;
    }
    m_loginAlreadyCallback = false;
    
    //启动定时
    std::function<void()> timeoutCallback = [&]()
    {
        NSLog(@"触发指定回调");
        if (m_loginAlreadyCallback) return;
        m_loginAlreadyCallback = true;
        
        _userInfor.clear();
        ios_loginCallback(1, "get user info time out");
    };
    [[TimeMoniter getInstance] scheduleOnce:timeoutCallback withTime:20.0f];
    
    FBRequest *request = [FBRequest requestForMe];
    [[request parameters] setValue:@"fields" forKey:@"uid,name"];
    [request startWithCompletionHandler:
     ^(FBRequestConnection *connection,
       NSDictionary<FBGraphUser> *user,
       NSError *error) {
         
         if (m_loginAlreadyCallback) return;  //超时
         //结束定时器
         m_loginAlreadyCallback = true;
         
         NSLog(@"获取到玩家facebook信息initUserInfoAndCallback");
         if (!error) {
             _userInfor.clear();
             _userInfor[ID] = [user.objectID UTF8String];
             _userInfor[NAME] = [user.name UTF8String];
             _userInfor[PICTURE] = [[NSString stringWithFormat:@"https://graph.facebook.com/%@/picture?type=small", user.objectID] UTF8String];
             NSLog(@"\n=====================================\n");
             NSLog(@"\nid=%s\nname=%s\npicture=%s", _userInfor[ID].c_str(), _userInfor[NAME].c_str(), _userInfor[PICTURE].c_str());
             NSLog(@"\n=====================================\n");
             cacheUserInfor();
             ios_loginCallback(0, "");
         }else{
             _userInfor.clear();
             ios_loginCallback(1, "get user infor failed");
         }
     }];
    
}

void FacebookIOS::cacheUserInfor()
{
    NSArray *paths=NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *fileName=[[paths objectAtIndex:0] stringByAppendingPathComponent:@"Facebook_UserInfor"];
    //查找文件，如果不存在，就创建一个文件
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:fileName]) {
        [fileManager createFileAtPath:fileName contents:nil attributes:nil];
    }
    
    NSDictionary* dictionary = [NSDictionary dictionaryWithObjectsAndKeys:[NSString stringWithUTF8String:_userInfor[ID].c_str()], @"ID",
                                [NSString stringWithUTF8String:_userInfor[NAME].c_str()], @"name",
                                [NSString stringWithUTF8String:_userInfor[PICTURE].c_str()], @"picture",
                                nil];
    //==生成Json对应的Data
    NSError *error=nil;
    NSData *JsonData=[NSJSONSerialization dataWithJSONObject:dictionary options:NSJSONWritingPrettyPrinted error:&error];
    if (error) {
        NSLog(@"Error %@",[error localizedDescription]);
        return;
    }
    //==写入文件
    NSLog(@"write facebook cache to file %@",[JsonData writeToFile:fileName atomically:YES] ? @"Succeed":@"Failed");
}
void FacebookIOS::loadUserInfor()
{
    if (_userInfor[ID].size() > 0)
        return;
    
    NSArray *paths=NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *fileName=[[paths objectAtIndex:0] stringByAppendingPathComponent:@"Facebook_UserInfor"];
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:fileName]) {
        return;
    }
    //==Json数据
    NSData *data=[NSData dataWithContentsOfFile:fileName];
    //==JsonObjec
    NSError * error = nil;
    id JsonObject=[NSJSONSerialization JSONObjectWithData:data
                                                  options:NSJSONReadingAllowFragments
                                                    error:&error];
    if (JsonObject != nil && error == nil) {
        if ([JsonObject isKindOfClass:[NSDictionary class]]) {
            NSDictionary *dictionary = (NSDictionary *)JsonObject;
            _userInfor[ID] = [[dictionary objectForKey:@"ID"] UTF8String];
            _userInfor[NAME] = [[dictionary objectForKey:@"name"] UTF8String];
            _userInfor[PICTURE] = [[dictionary objectForKey:@"picture"] UTF8String];
            printf("id:%s\nname:%s\npicture:%s\n", _userInfor[ID].c_str(), _userInfor[NAME].c_str(), _userInfor[PICTURE].c_str());
        }
        else{
            NSLog(@"JsonObject not NSDictionary Class");
        }
    }
}


