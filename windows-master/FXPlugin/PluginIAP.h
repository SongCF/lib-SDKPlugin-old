#ifndef __FXPlugin_PluginIAP_H__
#define __FXPlugin_PluginIAP_H__


#include <map>
#include <string>
#include "cocos2d.h"

namespace FX{

enum class PayResultCode
{
	ePaySuccess = 0,
	ePayCancle,
	ePayFail,
	ePayNetworkError,
};

typedef std::map<std::string,std::string> ProductInfoMap;

class PayResultListener
{
public: virtual void onPayResult(PayResultCode ret, const char* msg, ProductInfoMap info) = 0;
};


class PluginIAP
{
public:
	
    /**
    @brief pay for product
    @param info The info of product, must contains key:
            productName         The name of product
            productPrice        The price of product(must can be parse to float)
            productDesc         The description of product
    @warning For different plugin, the parameter should have other keys to pay.
    */
    virtual void payForProduct(ProductInfoMap info) = 0;

    /**
     @brief set the result listener
     @param pListener The callback object for payment result
     */
    virtual void setPayListener(PayResultListener* listener) = 0 ;



// private:
// 	PayResultListener* m_pfnPayResultListener;
};

}

#endif /* define(__FXPlugin_PluginIAP_H__) */