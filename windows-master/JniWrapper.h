
#pragma once
#include "cocos2d.h"
using namespace cocos2d;
//jni  callback
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "platform\android\jni\JniHelper.h"


//c++ to java
class JniWrapper
{
	//static
};



// java to c++
#ifdef __cplusplus
extern "C" {
#endif
	//JNIEXPORT jint JNICALL Java_org_FXUtils_JniWrapper_getCurSettingLanguageTypeNative(JNIEnv *env, jobject thiz);
#ifdef __cplusplus
}
#endif



#endif  //#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
