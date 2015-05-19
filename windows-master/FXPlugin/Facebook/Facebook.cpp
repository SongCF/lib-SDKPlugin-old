
#include "Facebook.h"
#include "include_json.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "platform\android\jni\JniHelper.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "FacebookIOS.h"
#endif


using namespace FX;
using namespace FX::Plugin;


#define JNI_CLASS "org/FXPlugin/FacebookAndroid"
#define log CCLog


Facebook* Facebook::_instanceFacebook = nullptr;

Facebook::Facebook()
	: _loginListener(nullptr)
	, _shareListener(nullptr)
	, _inviteFriendsListener(nullptr)
{
}

Facebook* Facebook::getInstance()
{
	if (! _instanceFacebook) _instanceFacebook = new Facebook();
	return _instanceFacebook;
}



// 1. user
void Facebook::setLoginListener(UserLoginListener* listener)
{
	_loginListener = listener;
}
void Facebook::login()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JNI_CLASS, "login", "()V")) 
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	FacebookIOS::getInstance()->login();
#endif
}
void Facebook::logout()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JNI_CLASS, "logout", "()V")) 
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	FacebookIOS::getInstance()->logout();
#endif
}
bool Facebook::isLogined()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	jboolean ret = false;
	if (JniHelper::getStaticMethodInfo(t, JNI_CLASS, "isLogin", "()Z")) 
	{
		ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
	return ret;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return FacebookIOS::getInstance()->isLogin();
#endif
	return false;
}
std::string Facebook::getUserID()
{
	std::string ret;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JNI_CLASS, "getUserID", "()Ljava/lang/String;"))
	{
		jstring juser_id = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		jboolean jb_copy = false;
		if(juser_id) ret = std::string(t.env->GetStringUTFChars(juser_id, &jb_copy));
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	ret = FacebookIOS::getInstance()->getUserID();
#endif
	CCLog("%s ---> user id = %s", __FUNCTION__, ret.c_str());
	return ret;
}
UserInfo Facebook::getUserInfo()
{
	UserInfo ret;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JNI_CLASS, "getUserInfo", "()[Ljava/lang/String;"))
	{
		jobjectArray infoArr = (jobjectArray)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		jstring juser_id = (jstring)t.env->GetObjectArrayElement(infoArr, 0);
		jstring juser_name = (jstring)t.env->GetObjectArrayElement(infoArr, 1);
		jstring juser_pic = (jstring)t.env->GetObjectArrayElement(infoArr, 2);
		jboolean jb_copy = false;
		if (juser_id) ret.id = std::string(t.env->GetStringUTFChars(juser_id, &jb_copy));
		if (juser_name) ret.name = std::string(t.env->GetStringUTFChars(juser_name, &jb_copy));
		if (juser_pic) ret.picture = std::string(t.env->GetStringUTFChars(juser_pic, &jb_copy));

		t.env->DeleteLocalRef(t.classID);
	}
	CCLog("%s end", __FUNCTION__);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	std::map<std::string,std::string> infoMap = FacebookIOS::getInstance()->getUserInfor();
	ret.id = infoMap["ID"];
	ret.name = infoMap["name"];
	ret.picture = infoMap["picture"];
#endif
	CCLog("%s ---> user id = %s, name = %s, picture = %s", __FUNCTION__, ret.id.c_str(), ret.name.c_str(), ret.picture.c_str());
	return ret;
}
void Facebook::getAppFriendList(pfuncGetFriends callback)
{
	m_getAppFriendsCallback = callback;

	std::string url = "https://graph.facebook.com/me/friends?access_token=" + getAccessToken()
		+ "&fields=id,name,picture";


	CCHttpRequest* request = new CCHttpRequest();
	// required fields
	request->setUrl(url.c_str());
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setResponseCallback(this, httpresponse_selector(Facebook::calbackGetFriends));
	// optional fields                            
	request->setTag("GET App friends");
    
    CCHttpClient::getInstance()->setTimeoutForConnect(20);
    CCHttpClient::getInstance()->setTimeoutForRead(20);
	CCHttpClient::getInstance()->send(request);

	// don't forget to release it, pair to new
	request->release();
}
void Facebook::getAllFriendList(pfuncGetFriends callback)
{
	m_getAllFriendsCallback = callback;

	std::string url = "https://graph.facebook.com/me/taggable_friends?access_token=" + getAccessToken()
		+ "&fields=id,name,picture";


	CCHttpRequest* request = new CCHttpRequest();
	// required fields
	request->setUrl(url.c_str());
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setResponseCallback(this, httpresponse_selector(Facebook::calbackGetFriends));
	// optional fields                            
	request->setTag("GET All friends");
    
    CCHttpClient::getInstance()->setTimeoutForConnect(20);
    CCHttpClient::getInstance()->setTimeoutForRead(20);
	CCHttpClient::getInstance()->send(request);

	// don't forget to release it, pair to new
	request->release();
}
void Facebook::calbackGetFriends(CCHttpClient* client, CCHttpResponse* response)
{
	if (!response)
	{
		return;
	}

	// You can get original request type from: response->request->reqType
	std::string tag = response->getHttpRequest()->getTag();
	if (! tag.empty()) 
	{
		log("%s completed", tag.c_str());
	}

	int statusCode = response->getResponseCode();
	log("response code: %d, tag = %s", statusCode, tag.c_str());

	if (!response->isSucceed()) 
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		if ("GET All friends" == tag && m_getAllFriendsCallback != nullptr)
			m_getAllFriendsCallback(false,std::vector<UserInfo>());
		else if ("GET App friends" == tag && m_getAppFriendsCallback != nullptr)
			m_getAppFriendsCallback(false,std::vector<UserInfo>());
		return;
	}

	// dump data
	std::vector<char> *buffer = response->getResponseData();
	std::string jsonDoc(buffer->begin(), buffer->end());
	log("%s", jsonDoc.c_str());

	//parse json data
/*	{
		"data": [
		{
			"id": "1386146831694197",
				"name": "Richard Song",
				"picture": {
					"data": {
						"is_silhouette": false,
							"url": "https://fbcdn-profile-a.akamaihd.net/hprofile-ak-xfa1/v/t1.0-1/p50x50/10551094_1386928481616032_6900265282582756767_n.jpg?oh=0348537dd972bc5681a9fe6713fa3494&oe=556B4862&__gda__=1431737572_816ed28b49a4e61c61d4e0e169cb9c8d"
					}
			}
		}
		],
			"paging": {
				"next": "https://graph.facebook.com/v2.2/1388746124766128/friends?fields=id,name,picture&access_token=CAAE2vHMXCEYBAOGMgtkpv2fqZAUla7J55u8u8A0BtY6ZBzh7xytjsiHZAqAEPc6s72rAnDpWZAmBEZA5IJR9f3NLgFWw2yFlGkz0ZAYYVINTZAYz8X97szqvBTnVSUgQEewxqO5wUS9cxPseUh3zLqBpQKX5eDSfeINbMhuQQOo5ItH6GjmZC0pf4U51ZB9jnVl8QEM0q10GTw1wSc2AA3eLH&limit=25&offset=25&__after_id=enc_AexvH2t_R-XTIqNVm3TVWhvwRfDKKsQJgPV6oe8VCQv1yekyw9Bt_guKhNHOrOyVUr2ft4ZHraUmJ6KXrO7Bkldu"
		},
		"summary": {
			"total_count": 2
				}
	}*/
	Json::Reader reader;
	Json::Value root;
	//解析不成功就返回空;
	if (!reader.parse(jsonDoc, root))
	{
		log("get facebook friends--->parse json file error!");
		if ("GET All friends" == tag && m_getAllFriendsCallback != nullptr)
			m_getAllFriendsCallback(false,std::vector<UserInfo>());
		else if ("GET App friends" == tag && m_getAppFriendsCallback != nullptr)
			m_getAppFriendsCallback(false,std::vector<UserInfo>());
		return;
	}
	const Json::Value& jFriendList = root["data"];
	UserInfo temp;
	std::vector<UserInfo> vec;
	log("get facebook friends--->parse json file OK! count = %d", jFriendList.size());
	for (int i=0,fCount=jFriendList.size(); i<fCount; ++i)
	{
		const Json::Value& oneFriend = jFriendList[i];
		temp.id = oneFriend["id"].asString();
		temp.name = oneFriend["name"].asString();
		temp.picture = oneFriend["picture"]["data"]["url"].asString();
		vec.push_back(temp);
	}

	if ("GET All friends" == tag && m_getAllFriendsCallback != nullptr)
		m_getAllFriendsCallback(true,vec);
	else if ("GET App friends" == tag && m_getAppFriendsCallback != nullptr)
		m_getAppFriendsCallback(true,vec);
}

void Facebook::inviteFriends(InviteFriendsListener* listener, const char* message)
{
	_inviteFriendsListener = listener;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JNI_CLASS, "inviteFriends", "(Ljava/lang/String;)V"))
	{
		jstring temp_jstring = t.env->NewStringUTF(message==nullptr ? "" : message);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, temp_jstring);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	FacebookIOS::getInstance()->inviteFriends(message==nullptr ? "" : message);
#endif
}


// 2. share
void Facebook::setShareListener(ShareResultListener* pListener)
{
	_shareListener = pListener;
}
void Facebook::share(std::map<std::string, std::string>* info)
{
	if (!info || info->empty()) {
		CCAssert(false, "");
		return;
	}

	//将pictureURL 转为 picture
	std::map<std::string, std::string>::iterator it = info->find("pictureURL");
	if (it != info->end())
	{
		std::string temp = it->second;
		info->erase(it);
		info->insert(std::make_pair("picture", temp));
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JNI_CLASS, "share", "([Ljava/lang/String;[Ljava/lang/String;)V")) 
	{
		jobjectArray key, value;
		key = t.env->NewObjectArray(info->size(), t.env->FindClass("java/lang/String"), 0);
		value = t.env->NewObjectArray(info->size(), t.env->FindClass("java/lang/String"), 0);
		int i=0;
		for (std::map<std::string, std::string>::const_iterator it = info->begin(); it != info->end(); it++, i++)
		{
			//多数JNI函数都创建局部引用。例如JNI函数NewObject创建一个实例，并且返回一个指向该实例的局部引用。
			//局部引用只在创建它的native方法的动态上下文中有效，并且只在native方法的一次调用中有效。所有局部引用只在一个native方法的执行期间有效，在该方法返回时，它就被回收。
			jstring temp_jstring = t.env->NewStringUTF(it->first.c_str());
			t.env->SetObjectArrayElement(key, i, temp_jstring);
			temp_jstring = t.env->NewStringUTF(it->second.c_str());
			t.env->SetObjectArrayElement(value, i, temp_jstring);
		}
		t.env->CallStaticVoidMethod(t.classID, t.methodID, key, value);		
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	FacebookIOS::getInstance()->share(info);
#endif
}




std::string Facebook::getAccessToken()
{
	std::string ret;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JNI_CLASS, "getAccessToken", "()Ljava/lang/String;"))
	{
		jstring accessToken = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		jboolean jb_copy = false;
		ret = std::string(t.env->GetStringUTFChars(accessToken, &jb_copy));
		t.env->DeleteLocalRef(t.classID);
		CCLog("%s ---> access token = %s", __FUNCTION__, ret.c_str());
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	ret = FacebookIOS::getInstance()->getAccessToken();
#endif
	return ret;
}




















#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#ifdef __cplusplus
extern "C" {
#endif
// login callback
JNIEXPORT void JNICALL Java_org_FXPlugin_FacebookAndroid_loginCallback
	(JNIEnv *env, jobject thiz, jint retCode, jstring msg)
{
	jboolean jb_copy = false;
	int code = retCode;
	std::string strMsg = std::string(env->GetStringUTFChars(msg, &jb_copy));
	CCLog("%s ---> ret code = %d, msg = %s", __FUNCTION__, code, strMsg.c_str());
	if (Facebook::getInstance()->getLoginListener())
		Facebook::getInstance()->getLoginListener()->onLoginResult((LoginResultCode)code, strMsg.c_str());
}
// share callback
JNIEXPORT void JNICALL Java_org_FXPlugin_FacebookAndroid_shareCallback
	(JNIEnv *env, jobject thiz, jint retCode, jstring msg)
{
	jboolean jb_copy = false;
	int code = retCode;
	std::string strMsg = std::string(env->GetStringUTFChars(msg, &jb_copy));
	CCLog("%s ---> ret code = %d, msg = %s", __FUNCTION__, code, strMsg.c_str());
	if (Facebook::getInstance()->getShareListener())
		Facebook::getInstance()->getShareListener()->onShareResult((ShareResultCode)code, strMsg.c_str());
}
// invite callback
JNIEXPORT void JNICALL Java_org_FXPlugin_FacebookAndroid_inviteFriendsCallback
	(JNIEnv *env, jobject thiz, jint retCode, jstring msg)
{
	jboolean jb_copy = false;
	int code = retCode;
	std::string strMsg = std::string(env->GetStringUTFChars(msg, &jb_copy));
	CCLog("%s ---> ret code = %d, msg = %s", __FUNCTION__, code, strMsg.c_str());
	if (Facebook::getInstance()->getInviteFriendsListener())
		Facebook::getInstance()->getInviteFriendsListener()->onInviteFriendsResult((InviteFriendsResultCode)retCode, strMsg.c_str());
}
#ifdef __cplusplus
}
#endif

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
void ios_loginCallback(int retCode, const char* msg)
{
	CCLog("%s ---> ret code = %d, msg = %s", __FUNCTION__, retCode, msg);
	if (Facebook::getInstance()->getLoginListener())
		Facebook::getInstance()->getLoginListener()->onLoginResult((LoginResultCode)retCode, msg);
}
void ios_shareCallback(int retCode, const char* msg)
{
	CCLog("%s ---> ret code = %d, msg = %s", __FUNCTION__, retCode, msg);
	if (Facebook::getInstance()->getShareListener())
		Facebook::getInstance()->getShareListener()->onShareResult((ShareResultCode)retCode, msg);
}
void ios_inviteFriendsCallback(int retCode, const char* msg)
{
	CCLog("%s ---> ret code = %d, msg = %s", __FUNCTION__, retCode, msg);
	if (Facebook::getInstance()->getInviteFriendsListener())
		Facebook::getInstance()->getInviteFriendsListener()->onInviteFriendsResult((InviteFriendsResultCode)retCode, msg);
}

#endif