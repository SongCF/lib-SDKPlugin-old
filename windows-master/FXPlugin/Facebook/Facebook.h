#ifndef __FXPlugin_Facebook_H__
#define __FXPlugin_Facebook_H__

#include "FXPlugin/PluginUser.h"
#include "FXPlugin/PluginShare.h"
#include "FXPlugin/PluginIAP.h"
#include "FXPlugin/PluginAnalytics.h"
#include "FXPlugin/PluginAds.h"


#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "platform\android\jni\JniHelper.h"
#endif


namespace FX{ namespace Plugin{

class Facebook : public CCObject, public PluginUser, public PluginShare
{
private:
	static Facebook* _instanceFacebook;
	Facebook();
public:
	static Facebook* getInstance();

	// 1. user
public:
    virtual void setLoginListener(UserLoginListener* listener);
	UserLoginListener* getLoginListener(){return _loginListener;}
	virtual  void login();
	virtual  void logout();
    virtual bool isLogined();
	virtual std::string getUserID();
	virtual UserInfo getUserInfo();

	virtual void getAppFriendList(pfuncGetFriends callback);
	virtual void getAllFriendList(pfuncGetFriends callback);
	void calbackGetFriends(CCHttpClient* client, CCHttpResponse* response); // for ver cocos2dx-2.x

	InviteFriendsListener* getInviteFriendsListener(){return _inviteFriendsListener;}
	virtual void inviteFriends(InviteFriendsListener* listener, const char* message);
protected:
	std::string getAccessToken();
protected:
	pfuncGetFriends m_getAppFriendsCallback;
	pfuncGetFriends m_getAllFriendsCallback;



	// 2. share
public:
	virtual void setShareListener(ShareResultListener* pListener);
	ShareResultListener* getShareListener(){return _shareListener;}
    virtual void share(std::map<std::string, std::string>* info);




protected:
	UserLoginListener* _loginListener;
	ShareResultListener* _shareListener;
	InviteFriendsListener* _inviteFriendsListener;
};




}}


#endif /* define(__FXPlugin_Facebook_H__) */