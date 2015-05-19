#ifndef __FXPlugin_AppStore_H__
#define __FXPlugin_AppStore_H__

#include "FXPlugin/PluginIAP.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

namespace FX{ namespace Plugin{
class AppStore : public PluginIAP
{
private:
	static AppStore* _instanceAppStore;
public:
	static AppStore* getInstance();
public:
	AppStore();
	~AppStore();

	void payForProduct(ProductInfoMap info);

    void setPayListener(PayResultListener* listener);

	PayResultListener* getPayResultListener(){return m_pPayResultListener;};


	void onPayResult(PayResultCode ret, const char* msg);

private:

	PayResultListener*		m_pPayResultListener;
	ProductInfoMap m_productInfo;
};

}}
#endif
#endif //__FXPlugin_AppStore_H__