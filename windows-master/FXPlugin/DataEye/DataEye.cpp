
#include "FXPlugin/DataEye/DataEye.h"

#include "cocos2d.h"
using namespace cocos2d;

//data eye lib
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "FXPlugin/DataEye/DataEyeLib/Android/include/DCAgent.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "FXPlugin/DataEye/DataEyeLib/IOS/DCAgent.h"
#endif



using namespace FX;
using namespace FX::Plugin;

DataEye::DataEye()
{
}

DataEye* DataEye::getInstance()
{
	static DataEye* _instanceDataEye = nullptr;
	if (!_instanceDataEye){
		_instanceDataEye = new DataEye();
	}
	return _instanceDataEye;
}

////////////////////////////////////////////////////////////////////////////////////////////
//1. custom event   --- 自定义事件
////////////////////////////////////////////////////////////////////////////////////////////
void DataEye::logError(const char* errorId, const char* message)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	LogEventMap infoMap;
	infoMap.insert(std::make_pair("ErrorInfor", message));
	DCEvent::onEvent(errorId, &infoMap);
#endif
}
void DataEye::logEvent(const char* eventId, LogEventMap* paramMap)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	DCEvent::onEvent(eventId, paramMap);
#endif
}


////////////////////////////////////////////////////////////////////////////////////////////
//2. Account   --- 账户 统计
////////////////////////////////////////////////////////////////////////////////////////////
/**
@brief 设置账户id，在使用账户统计功能时必须先设置id，accountId需保证全局唯一
@param 如果期望以设备为玩家单元进行统计，请在setAccount时传入getDeviceId()，SDK将帮助自动定义设备ID作为Account
*/
void DataEye::setAccount(const char* accountId)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	DCAccount::login(accountId);
#endif
}
const char* DataEye::getDeviceId()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return DCAgent::getUID();
#endif
	return "";
}

//设置帐户类型
void DataEye::setAccountType(AccountType accountType)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	DCAccountType type;
	switch (accountType)
	{
	case FX::PluginAnalytics::eAccountAnonymous:
		type = DCAccountType::DC_Anonymous;
		break;
	case FX::PluginAnalytics::eAccountRegistered:
		type = DCAccountType::DC_Registered;
		break;
	case FX::PluginAnalytics::eAccountSianWeibo:
		type = DCAccountType::DC_SinaWeibo;
		break;
	case FX::PluginAnalytics::eAccountQQ:
		type = DCAccountType::DC_QQ;
		break;
	case FX::PluginAnalytics::eAccountTencentWeibo:
		type = DCAccountType::DC_QQWeibo;
		break;
	case FX::PluginAnalytics::eAccountND91:
		type = DCAccountType::DC_ND91;
		break;
	case FX::PluginAnalytics::eAccountFacebook:
		type = DCAccountType::DC_Type1;
		break;
	case FX::PluginAnalytics::eAccountType1:
		type = DCAccountType::DC_Type2;
		break;
	case FX::PluginAnalytics::eAccountType2:
		type = DCAccountType::DC_Type3;
		break;
	case FX::PluginAnalytics::eAccountType3:
		type = DCAccountType::DC_Type4;
		break;
	case FX::PluginAnalytics::eAccountType4:
		type = DCAccountType::DC_Type5;
		break;
	case FX::PluginAnalytics::eAccountType5:
		type = DCAccountType::DC_Type6;
		break;
	default:
		type = DCAccountType::DC_Type10;
		break;
	}
	DCAccount::setAccountType(type);
#endif
}

//设置帐户的显性名
void DataEye::setAccountName(const char* accountName)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	CCLOG("Warning: %s ---> DataEye has not this method", __FUNCTION__);
#endif
}
//设置级别
void DataEye::setLevel(int level)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	DCAccount::setLevel(level);
#endif
}
//设置性别
void DataEye::setGender(AcountGender gender)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

	DCGender dcGender;
	switch (gender)
	{
	case FX::PluginAnalytics::eGenderMale:
		dcGender = DCGender::DC_MALE;
		break;
	case FX::PluginAnalytics::eGenderFemale:
		dcGender = DCGender::DC_FEMALE;
		break;
	default:
		dcGender = DCGender::DC_UNKNOWN;
		break;
	}
	DCAccount::setGender(dcGender);
#endif
}
//设置年龄  0-120
void DataEye::setAge(int age)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	DCAccount::setAge(age);
#endif
}
//设置区服
void DataEye::setGameServer(const char* gameServer)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	DCAccount::setGameServer(gameServer);
#endif
}


////////////////////////////////////////////////////////////////////////////////////////////
//3. VirtualCurrency   --- 跟踪 充值/赠送 获得虚拟币情况
////////////////////////////////////////////////////////////////////////////////////////////
/*
*充值过程分两个跟踪阶段：1、发出有效的充值请求；2确认某次充值请求已完成充值。
*您可在玩家发起充值请求时（例如玩家选择了某个充值包，进入支付流程那一
*    刻）调用onChargeRequest，并传入该笔交易的唯一订单ID和详细信息；在
*    确认玩家支付成功时调用onChargeSuccess，并告知完成的是哪个订单ID。
*/
//充值请求
void DataEye::onChargeRequest(const char* orderId, const char* itemName, double currencyAmount, 
								  const char* currencyType, double virtualCurrencyAmount, const char* paymentType)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	m_paymentInfo.insert(std::make_pair(orderId,
		std::make_tuple(currencyAmount, currencyType, paymentType, itemName)));
#endif
}
//充值成功
void DataEye::onChargeSuccess(const char* orderId)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	DataeyePaymentInfo::iterator iter = m_paymentInfo.find(orderId);
	if (iter != m_paymentInfo.end())
	{
		double currencyAmount = 0.0;
		std::string currencyType;
		std::string paymentType;
		std::string itemName;
		std::tie(currencyAmount,currencyType,paymentType,itemName) = iter->second;
		DCVirtualCurrency::paymentSuccessInLevel(orderId, currencyAmount, currencyType.c_str(), paymentType.c_str(), itemName.c_str());
	}
	else
	{
		CCLOG("Error: %s ---> not found this orderId[%s]", __FUNCTION__, orderId);
	}
#endif
}
//赠予虚拟币
/*
*pref:在任务奖励、登录奖励、成就奖励等环节免费发放给玩家虚拟币,
只获得过赠予虚拟币的玩家不会被记为付费玩家。赠予的虚拟币会计入到所有的虚拟币产出中
*/
void DataEye::onReward(double virtualCurrencyAmount, const char* reason)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	DCCoin::gain(reason, "Unkown", virtualCurrencyAmount, 0);
#endif
}


////////////////////////////////////////////////////////////////////////////////////////////
//4. Item   --- 跟踪 游戏内有价值的虚拟币的消费点
////////////////////////////////////////////////////////////////////////////////////////////
//记录付费点 (在任意消费点发生时尽快调用)
void DataEye::onPurchase(const char* itemName, int itemNumber, double priceInVirtualCurrency)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	DCItem::buy(itemName, "", itemNumber, priceInVirtualCurrency, "Unknown", "");
#endif
}
//消耗物品或服务等 (在某个道具/服务被用掉（消失）时尽快调用)
void DataEye::onUse(const char* itemName, int itemNumber, const char* reason)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	DCItem::consume(itemName, "Unknown", itemNumber, reason);
#endif
}


////////////////////////////////////////////////////////////////////////////////////////////
//5. Mission   --- 跟踪 任务/关卡/副本
////////////////////////////////////////////////////////////////////////////////////////////
/*
*同一个missionId如果在未结束前，重复进行了onBegin调用，则重新开始计时，上一次的调用被丢弃。
*/
//接受或进入
void DataEye::onBegin(const char* missionId)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	DCTask::begin(missionId, DCTaskType::DC_Other);
#endif
}
//完成
void DataEye::onCompleted(const char* missionId)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	DCTask::complete(missionId);
#endif
}
//失败
void DataEye::onFailed(const char* missionId, const char* cause)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	DCTask::fail(missionId, cause);
#endif
}