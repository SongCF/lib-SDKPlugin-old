
#pragma once

#include "FXPlugin/PluginUser.h"
#include "FXPlugin/PluginShare.h"
#include "FXPlugin/PluginIAP.h"
#include "FXPlugin/PluginAnalytics.h"
#include "FXPlugin/PluginAds.h"

#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#endif
#include <map>
#include <string>

namespace FX {

class PluginManager
{
public:
	static PluginManager* getInstance();
	void loadPlugin();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	void setJavaVM(JavaVM *vm);
#endif


	PluginUser* getPluginUser();
	PluginShare* getPluginShare();
	PluginIAP* getPluginIAP();
	PluginAnalytics* getPluginAnalytics(const char* pluginName);

private:
	PluginManager();
	static PluginManager* _instancePluginManager;

private:
	PluginUser * _user;
	PluginShare * _share;
	PluginIAP * _iap;
	std::map<std::string, PluginAnalytics *> _analyticsMap;
};
}