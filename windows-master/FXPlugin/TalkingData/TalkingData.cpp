#include "TalkingData.h"
#include "TalkingDataGameAnalytics/include/TalkingData.h"

using namespace FX;
using namespace FX::Plugin;

TalkingData::TalkingData()
	: m_tdAccount(nullptr)
{
}

TalkingData* TalkingData::getInstance()
{
	static TalkingData* _instanceTalkingData = nullptr;
	if (!_instanceTalkingData){
		_instanceTalkingData = new TalkingData();
	}
	return _instanceTalkingData;
}

////////////////////////////////////////////////////////////////////////////////////////////
//1. custom event   --- 自定义事件
////////////////////////////////////////////////////////////////////////////////////////////
void TalkingData::logError(const char* errorId, const char* message)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	EventParamMap infoMap;
	infoMap.insert(EventParamPair("ErrorInfor", message));
	TDCCTalkingDataGA::onEvent(errorId, &infoMap);
#endif
}
void TalkingData::logEvent(const char* eventId, LogEventMap* paramMap)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	TDCCTalkingDataGA::onEvent(eventId, paramMap);
#endif
}
/*
void TalkingData::logTimedEventBegin(const char* eventId)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	std::string temp = eventId;
	temp += "Begin";

	char buf[48];
	time_t tt = time(NULL);//这句返回的只是一个时间戳
	tm* t= localtime(&tt);
	sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d", 
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec);
	EventParamMap infoMap;
	infoMap.insert(EventParamPair("Time", buf));
	TDCCTalkingDataGA::onEvent(temp.c_str(), &infoMap);
#endif
}
void TalkingData::logTimedEventEnd(const char* eventId)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	std::string temp = eventId;
	temp += "End";

	char buf[48];
	time_t tt = time(NULL);//这句返回的只是一个时间戳
	tm* t= localtime(&tt);
	sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d", 
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec);
	EventParamMap infoMap;
	infoMap.insert(EventParamPair("Time", buf));
	TDCCTalkingDataGA::onEvent(temp.c_str(), &infoMap);
#endif
}*/


////////////////////////////////////////////////////////////////////////////////////////////
//2. Account   --- 账户 统计
////////////////////////////////////////////////////////////////////////////////////////////
/**
@brief 设置账户id，在使用账户统计功能时必须先设置id，accountId需保证全局唯一
@param 如果期望以设备为玩家单元进行统计，请在setAccount时传入getDeviceId()，SDK将帮助自动定义设备ID作为Account
*/
void TalkingData::setAccount(const char* accountId)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	m_tdAccount = TDCCAccount::setAccount(accountId);
#endif
}
const char* TalkingData::getDeviceId()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return TDCCTalkingDataGA::getDeviceId();
#endif
	return "";
}

//设置帐户类型
void TalkingData::setAccountType(AccountType accountType)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	if (m_tdAccount)
	{

		TDCCAccount::TDCCAccountType type;
		switch (accountType)
		{
		case FX::PluginAnalytics::eAccountAnonymous:
			type = TDCCAccount::kAccountAnonymous;
			break;
		case FX::PluginAnalytics::eAccountRegistered:
			type = TDCCAccount::kAccountRegistered;
			break;
		case FX::PluginAnalytics::eAccountSianWeibo:
			type = TDCCAccount::kAccountSianWeibo;
			break;
		case FX::PluginAnalytics::eAccountQQ:
			type = TDCCAccount::kAccountQQ;
			break;
		case FX::PluginAnalytics::eAccountTencentWeibo:
			type = TDCCAccount::kAccountTencentWeibo;
			break;
		case FX::PluginAnalytics::eAccountND91:
			type = TDCCAccount::kAccountND91;
			break;
		case FX::PluginAnalytics::eAccountFacebook:
			type = TDCCAccount::kAccountType1;
			break;
		case FX::PluginAnalytics::eAccountType1:
			type = TDCCAccount::kAccountType2;
			break;
		case FX::PluginAnalytics::eAccountType2:
			type = TDCCAccount::kAccountType3;
			break;
		case FX::PluginAnalytics::eAccountType3:
			type = TDCCAccount::kAccountType4;
			break;
		case FX::PluginAnalytics::eAccountType4:
			type = TDCCAccount::kAccountType5;
			break;
		case FX::PluginAnalytics::eAccountType5:
			type = TDCCAccount::kAccountType6;
			break;
		default:
			type = TDCCAccount::kAccountType10;
			break;
		}
		m_tdAccount->setAccountType(type);
	}
#endif
}
//设置帐户的显性名
void TalkingData::setAccountName(const char* accountName)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	if (m_tdAccount) m_tdAccount->setAccountName(accountName);
#endif
}
//设置级别
void TalkingData::setLevel(int level)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	if (m_tdAccount) m_tdAccount->setLevel(level);
#endif
}
//设置性别
void TalkingData::setGender(AcountGender gender)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	if (m_tdAccount) 
	{
		TDCCAccount::TDCCGender tdgender;
		switch (gender)
		{
		case FX::PluginAnalytics::eGenderMale:
			tdgender = TDCCAccount::kGenderMale;
			break;
		case FX::PluginAnalytics::eGenderFemale:
			tdgender = TDCCAccount::kGenderFemale;
			break;
		default:
			tdgender = TDCCAccount::kGenderUnknown;
			break;
		}
		m_tdAccount->setGender(tdgender);
	}
#endif
}
//设置年龄  0-120
void TalkingData::setAge(int age)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	if (m_tdAccount) m_tdAccount->setAge(age);
#endif
}
//设置区服
void TalkingData::setGameServer(const char* gameServer)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	if (m_tdAccount) m_tdAccount->setGameServer(gameServer);
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
void TalkingData::onChargeRequest(const char* orderId, const char* itemName, double currencyAmount, 
								  const char* currencyType, double virtualCurrencyAmount, const char* paymentType)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	TDCCVirtualCurrency::onChargeRequest(orderId, itemName, currencyAmount, currencyType, virtualCurrencyAmount, paymentType);
#endif
}
//充值成功
void TalkingData::onChargeSuccess(const char* orderId)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	TDCCVirtualCurrency::onChargeSuccess(orderId);
#endif
}
//赠予虚拟币
/*
*pref:在任务奖励、登录奖励、成就奖励等环节免费发放给玩家虚拟币,
只获得过赠予虚拟币的玩家不会被记为付费玩家。赠予的虚拟币会计入到所有的虚拟币产出中
*/
void TalkingData::onReward(double virtualCurrencyAmount, const char* reason)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	TDCCVirtualCurrency::onReward(virtualCurrencyAmount, reason);
#endif
}


////////////////////////////////////////////////////////////////////////////////////////////
//4. Item   --- 跟踪 游戏内有价值的虚拟币的消费点
////////////////////////////////////////////////////////////////////////////////////////////
//记录付费点 (在任意消费点发生时尽快调用)
void TalkingData::onPurchase(const char* itemName, int itemNumber, double priceInVirtualCurrency)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	TDCCItem::onPurchase(itemName, itemNumber, priceInVirtualCurrency);
#endif
}
//消耗物品或服务等 (在某个道具/服务被用掉（消失）时尽快调用)
void TalkingData::onUse(const char* itemName, int itemNumber, const char* reason)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	TDCCItem::onUse(itemName, itemNumber);
#endif
}


////////////////////////////////////////////////////////////////////////////////////////////
//5. Mission   --- 跟踪 任务/关卡/副本
////////////////////////////////////////////////////////////////////////////////////////////
/*
*同一个missionId如果在未结束前，重复进行了onBegin调用，则重新开始计时，上一次的调用被丢弃。
*/
//接受或进入
void TalkingData::onBegin(const char* missionId)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	TDCCMission::onBegin(missionId);
#endif
}
//完成
void TalkingData::onCompleted(const char* missionId)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	TDCCMission::onCompleted(missionId);
#endif
}
//失败
void TalkingData::onFailed(const char* missionId, const char* cause)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	TDCCMission::onFailed(missionId, cause);
#endif
}