#include "FXPluginManager.h"
#include "cocos2d.h"

#include "Facebook/Facebook.h"
#include "AppStore/AppStore.h"

#include "DataEye/DataEye.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "FXPlugin/DataEye/DataEyeLib/Android/include/DCJniHelper.h"
#endif
#include "TalkingData/TalkingData.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "GooglePlay/GooglePlay.h"
#include "FXPlugin/TalkingData/TalkingDataGameAnalytics/android/TDGAJniHelper.h"
#endif

using namespace cocos2d;
using namespace FX;
using namespace FX::Plugin;


PluginManager* PluginManager::_instancePluginManager = NULL;


PluginManager::PluginManager()
	: _user(NULL)
	, _share(NULL)
	, _iap(NULL)
{
}

PluginManager* PluginManager::getInstance()
{
	if (! _instancePluginManager){
		_instancePluginManager = new PluginManager();
	}
	return _instancePluginManager;
}

void PluginManager::loadPlugin()
{
	_user = Facebook::getInstance();
	_share = Facebook::getInstance();

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	_iap = AppStore::getInstance();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	_iap = GooglePlay::getInstance();
#endif

	//analytics
	{
		_analyticsMap.clear();
		_analyticsMap.insert(std::make_pair(PLUGIN_ANALYTICS_TalkingData, TalkingData::getInstance()));
		_analyticsMap.insert(std::make_pair(PLUGIN_ANALYTICS_DataEye, DataEye::getInstance()));
	}
}


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void PluginManager::setJavaVM(JavaVM *vm)
{
	//talking data
	TDGAJniHelper::setJavaVM(static_cast<JavaVM*>(vm));
	//data eye
	DCJniHelper::setJVM(static_cast<JavaVM*>(vm));
}
#endif




PluginUser* PluginManager::getPluginUser()
{
	return _user;
}

PluginShare* PluginManager::getPluginShare()
{
	return _share;
}

PluginIAP* PluginManager::getPluginIAP()
{
	return _iap;
}

PluginAnalytics* PluginManager::getPluginAnalytics(const char* pluginName)
{
	std::map<std::string, PluginAnalytics *>::iterator iter = _analyticsMap.find(pluginName);
	if (iter != _analyticsMap.end()) return iter->second;
	else return nullptr;
}