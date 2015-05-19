#ifndef __FXPlugin_MMPay_H__
#define __FXPlugin_MMPay_H__

#include "FXPlugin/PluginIAP.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

namespace FX{ namespace Plugin{
	class MMPay : public PluginIAP
	{
	private:
		static MMPay* _instanceMMPay;
	public:
		static MMPay* getInstance();
	public:
		MMPay();
		~MMPay();

		void payForProduct(ProductInfoMap info);

		void setPayListener(PayResultListener* listener);

		PayResultListener* getPayResultListener(){return m_pPayResultListener;};


		void onPayResult(PayResultCode ret, const char* msg);

	private:

		PayResultListener*		m_pPayResultListener;
		ProductInfoMap			m_productInfo;
	};

}}
#endif //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif //__FXPlugin_MMPay_H__