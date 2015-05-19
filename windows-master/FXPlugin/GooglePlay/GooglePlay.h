#ifndef __FXPlugin_GooglePlay_H__
#define __FXPlugin_GooglePlay_H__

#include "FXPlugin/PluginIAP.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

namespace FX{ namespace Plugin{
class GooglePlay : public PluginIAP
{
private:
	static GooglePlay* _instanceGooglePlay;
public:
	static GooglePlay* getInstance();
public:
	GooglePlay();
	~GooglePlay();

	void payForProduct(ProductInfoMap info);

    void setPayListener(PayResultListener* listener);

	PayResultListener* getPayResultListener(){return m_pPayResultListener;};


	void onPayResult(PayResultCode ret, const char* msg);

private:

	PayResultListener*		m_pPayResultListener;
	ProductInfoMap m_productInfo;
};

}}
#endif //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif //__FXPlugin_GooglePlay_H__