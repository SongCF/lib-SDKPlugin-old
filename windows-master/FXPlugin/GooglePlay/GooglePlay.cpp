#include "GooglePlay.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "platform\android\jni\JniHelper.h"

USING_NS_CC;
using namespace FX;
using namespace FX::Plugin;

#define JNI_CLASS "org/FXPlugin/GooglePlay"

GooglePlay* GooglePlay::_instanceGooglePlay = NULL;

GooglePlay* GooglePlay::getInstance()
{
	if (!_instanceGooglePlay)
	{
		_instanceGooglePlay = new GooglePlay();
        _instanceGooglePlay->m_pPayResultListener = NULL;
	}
	return _instanceGooglePlay;
}

GooglePlay::GooglePlay()
	: m_pPayResultListener(NULL)
{
}

GooglePlay::~GooglePlay()
{
}

void GooglePlay::payForProduct(FX::ProductInfoMap info)
{
	m_productInfo = info;
    
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, JNI_CLASS, "buy", "(Ljava/lang/String;)V")) 
	{
		jstring jId = t.env->NewStringUTF(info["Product_Id"].c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, jId);
		t.env->DeleteLocalRef(t.classID);
	}
}

void GooglePlay::setPayListener(PayResultListener* listener)
{
    m_pPayResultListener = listener;
}

void GooglePlay::onPayResult(FX::PayResultCode ret, const char* msg)
{
    if (m_pPayResultListener)
    {
        m_pPayResultListener->onPayResult(ret, msg, m_productInfo);
    }
}

#ifdef __cplusplus
extern "C" {
#endif
	// login callback
	JNIEXPORT void JNICALL Java_org_FXPlugin_GooglePlay_payCallBack
		(JNIEnv *env, jobject thiz, jint retCode, jstring msg)
	{
		int code = retCode;
		const char* chars = env->GetStringUTFChars(msg, NULL);
		std::string ret(chars);
		env->ReleaseStringUTFChars(msg, chars);
		CCLog("%s ---> ret code = %d, msg = %s", __FUNCTION__, code, ret.c_str());
		if (GooglePlay::getInstance()->getPayResultListener())
			GooglePlay::getInstance()->onPayResult((FX::PayResultCode)code, ret.c_str());
	}
#ifdef __cplusplus
}
#endif


#endif //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID