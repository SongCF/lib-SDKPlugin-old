#include "AppStore.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import "AppStoreIOS.h"

using namespace FX;
using namespace FX::Plugin;

AppStore* AppStore::_instanceAppStore = NULL;

AppStore* AppStore::getInstance()
{
	if (!_instanceAppStore)
	{
		_instanceAppStore = new AppStore();
        _instanceAppStore->m_pPayResultListener = NULL;
	}
	return _instanceAppStore;
}

AppStore::AppStore()
{
}

AppStore::~AppStore()
{
}

void AppStore::payForProduct(FX::ProductInfoMap info)
{
	m_productInfo = info;
    NSString *id = [NSString stringWithUTF8String:info["Product_Id"].c_str()];// info["Product_Id"]
    [[AppStoreIOS getInstance] buyProduct:id];
}

void AppStore::setPayListener(PayResultListener* listener)
{
    m_pPayResultListener = listener;
}

void AppStore::onPayResult(FX::PayResultCode ret, const char* msg)
{
    if (m_pPayResultListener)
    {
        m_pPayResultListener->onPayResult(ret, msg, m_productInfo);
    }
}
#endif