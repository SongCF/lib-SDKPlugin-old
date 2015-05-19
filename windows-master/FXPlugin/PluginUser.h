#ifndef __FXPlugin_PluginUser_H__
#define __FXPlugin_PluginUser_H__


#include <string>
#include <vector>
#include <functional>


namespace FX{


enum class LoginResultCode
{
	eSuccess = 0,
	eCancle,
};
enum class InviteFriendsResultCode
{
	eSuccess = 0,
	eCancle,
};



class UserLoginListener
{
public:
	virtual void onLoginResult(LoginResultCode ret, const char* msg) = 0;
};
class InviteFriendsListener
{
public:
	virtual void onInviteFriendsResult(InviteFriendsResultCode ret, const char* msg) = 0;
};



struct UserInfo
{
	std::string id;
	std::string name;
	std::string picture; //url
};
typedef std::function<void (bool, std::vector<UserInfo>)> pfuncGetFriends;

class PluginUser
{
public:
	
    /**
     @brief set the result listener
     @param pListener The callback object for user result
     */
    virtual void setLoginListener(UserLoginListener* listener) = 0 ;

	/**
     @brief User login
     */
    virtual  void login() = 0 ;
	/**
     @brief User logout
     */
    virtual  void logout() = 0 ;

	/**
     @brief Check whether the user logined or not
     @return If user logined, return value is true;
             else return value is false.
     */
    virtual bool isLogined() = 0 ;

    /**
     @brief Get session ID
     @return If user logined, return value is userID;
             else return value is empty string.
     */
    virtual std::string getUserID() = 0 ;
	virtual UserInfo getUserInfo() = 0 ;
	
    /**
     @brief Get friends
     */
	virtual void getAppFriendList(pfuncGetFriends callback) = 0 ;
	virtual void getAllFriendList(pfuncGetFriends callback) = 0 ;

	/*
	* invate firends to app                                                            
	*/
	virtual void inviteFriends(InviteFriendsListener* listener, const char* message) = 0;
};





}


#endif /* define(__FXPlugin_PluginUser_H__) */