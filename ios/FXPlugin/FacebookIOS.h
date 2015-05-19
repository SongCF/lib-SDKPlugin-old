
#pragma once

#include <vector>
#include <string>
#include <map>
//#import <FacebookSDK/FacebookSDK.h>


class FacebookIOS
{
private:
    FacebookIOS(){}
    static FacebookIOS* _instanceFacebookIOS;
public:
    static FacebookIOS* getInstance();
    
    void login();
    void logout();
    bool isLogin();
    std::string getUserID();
    std::map<std::string,std::string> getUserInfor();
    std::string getAccessToken();
    
    void inviteFriends(const char* inviteMsg);
    
    void share(std::map<std::string,std::string>* shareInfo);
    
    
    void didFinishLaunchingWithOptions(void *application, void *launchOptions);
    void applicationDidBecomeActive(void *application);
    bool applicationOpenURLSourceApplicationAnnotation(void *application, void *url, void *sourceApplication, void* annotation);
    
    
protected:
    void cacheUserInfor();
    void loadUserInfor();
    void initUserInfoAndCallback();
    void onSessionStateChanged(void* session, int state, void* error);
    
    
protected:
    std::map<std::string,std::string> _userInfor;
    
    bool m_loginAlreadyCallback;
};

