#include "MMPay.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "platform\android\jni\JniHelper.h"

USING_NS_CC;
using namespace FX;
using namespace FX::Plugin;

#define JNI_CLASS "org/FXPlugin/MMPay"

MMPay* MMPay::_instanceMMPay = NULL;

MMPay* MMPay::getInstance()
{
	if (!_instanceMMPay)
	{
		_instanceMMPay = new MMPay();
		_instanceMMPay->m_pPayResultListener = NULL;
	}
	return _instanceMMPay;
}

MMPay::MMPay()
	: m_pPayResultListener(NULL)
{
}

MMPay::~MMPay()
{
}

void MMPay::payForProduct(FX::ProductInfoMap info)
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

void MMPay::setPayListener(PayResultListener* listener)
{
	m_pPayResultListener = listener;
}

void MMPay::onPayResult(FX::PayResultCode ret, const char* msg)
{
	if (m_pPayResultListener)
	{
		m_pPayResultListener->onPayResult(ret, msg, m_productInfo);
	}
}

// java to c++
#ifdef __cplusplus
extern "C" {
#endif
	// login callback
	JNIEXPORT void JNICALL Java_org_FXPlugin_MMPay_onPayResult
		(JNIEnv *env, jobject thiz, jint retCode, jstring msg)
	{
		int code = retCode;
		const char* chars = env->GetStringUTFChars(msg, NULL);
		std::string retMsg(chars);
		env->ReleaseStringUTFChars(msg, chars);
		CCLog("%s ---> ret code = %d, msg = %s", __FUNCTION__, code, retMsg.c_str());
		if (MMPay::getInstance()->getPayResultListener())
		{
			MMPay::getInstance()->onPayResult(PayResultCode(code), retMsg.c_str());
		}
		
	}
#ifdef __cplusplus
}
#endif


#endif  //#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
