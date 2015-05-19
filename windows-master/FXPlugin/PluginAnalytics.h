#ifndef __FXPlugin_PluginAnalytics_H__
#define __FXPlugin_PluginAnalytics_H__


#include <map>
#include <string>

namespace FX{

//Tag
#define PLUGIN_ANALYTICS_TalkingData "PluginAnalyticsTalkingData"
#define PLUGIN_ANALYTICS_DataEye "PluginAnalyticsDataEye"



typedef std::map<std::string,std::string> LogEventMap;

class PluginAnalytics
{
public:
	enum AccountType
	{
		eAccountAnonymous = 0,		//匿名
		eAccountRegistered = 1,		//该游戏内注册

		//第三方账号（国内）
		eAccountSianWeibo = 11,		//新浪微博
		eAccountQQ = 12,			//QQ
		eAccountTencentWeibo = 13,  //腾讯微博
		eAccountND91 = 14,			//网龙91

		//第三方账号（国外)
		eAccountFacebook = 21,		//Facebook

		//预留了10种自定义的帐户类型
		eAccountType1 = 101,
		eAccountType2 = 102,
		eAccountType3 = 103,
		eAccountType4 = 104,
		eAccountType5 = 105,
// 		eAccountType6 = 106,
// 		eAccountType7 = 107,
// 		eAccountType8 = 108,
// 		eAccountType9 = 109,
// 		eAccountType10 = 110
	};
	enum AcountGender
	{
		eGenderUnknown = 0,
		eGenderMale = 1,
		eGenderFemale = 2
	};
public:
	////////////////////////////////////////////////////////////////////////////////////////////
	//1. custom event   --- 自定义事件
	////////////////////////////////////////////////////////////////////////////////////////////
	virtual void logError(const char* errorId, const char* message) = 0;
	virtual void logEvent(const char* eventId, LogEventMap* paramMap = NULL) = 0;


	////////////////////////////////////////////////////////////////////////////////////////////
	//2. Account   --- 账户 统计
	////////////////////////////////////////////////////////////////////////////////////////////
    /**
     @brief 设置账户id，在使用账户统计功能时必须先设置id，accountId需保证全局唯一
	 @param 如果期望以设备为玩家单元进行统计，请在setAccount时传入getDeviceId()，SDK将帮助自动定义设备ID作为Account
     */
	virtual void setAccount(const char* accountId) = 0;
	virtual const char* getDeviceId() = 0;
	//设置帐户类型
	virtual void setAccountType(AccountType accountType) = 0;
	//设置帐户的显性名
	virtual void setAccountName(const char* accountName) = 0;
	//设置级别
	virtual void setLevel(int level) = 0;
	//设置性别
	virtual void setGender(AcountGender gender) = 0;
	//设置年龄  0-120
	virtual void setAge(int age) = 0;
	//设置区服
	virtual void setGameServer(const char* gameServer) = 0;


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
	/*
	 * @parma 
	 1.orderId(唯一订单ID) 
	 2.itemName(内购物品名/ID)
	 3.currencyAmount(货币数量)
	 4.currencyType(货币类型)请使用国际标准组织ISO 4217中规范的3位字母代码标记货币类型。例：人民币CNY；美元USD；欧元EUR
	 5.virtualCurrencyAmount(参数3换算后的游戏内虚拟币数量)
	 6.paymentType(支付渠道)例如：“支付宝”“苹果官方”“XX支付SDK”
	*/
	virtual void onChargeRequest(const char* orderId, const char* itemName, double currencyAmount, 
		const char* currencyType, double virtualCurrencyAmount, const char* paymentType) = 0;
	//充值成功
	virtual void onChargeSuccess(const char* orderId) = 0;
	//赠予虚拟币
	/*
	 *pref:在任务奖励、登录奖励、成就奖励等环节免费发放给玩家虚拟币,
		只获得过赠予虚拟币的玩家不会被记为付费玩家。赠予的虚拟币会计入到所有的虚拟币产出中
	*/
	virtual void onReward(double virtualCurrencyAmount, const char* reason) = 0;


	////////////////////////////////////////////////////////////////////////////////////////////
	//4. Item   --- 跟踪 游戏内有价值的虚拟币的消费点
	////////////////////////////////////////////////////////////////////////////////////////////
	//记录付费点 (在任意消费点发生时尽快调用)
	virtual void onPurchase(const char* itemName, int itemNumber, double priceInVirtualCurrency) = 0;
	//消耗物品或服务等 (在某个道具/服务被用掉（消失）时尽快调用)
	virtual void onUse(const char* itemName, int itemNumber, const char* reason) = 0;


	////////////////////////////////////////////////////////////////////////////////////////////
	//5. Mission   --- 跟踪 任务/关卡/副本
	////////////////////////////////////////////////////////////////////////////////////////////
	/*
	 *同一个missionId如果在未结束前，重复进行了onBegin调用，则重新开始计时，上一次的调用被丢弃。
	*/
	//接受或进入
	virtual void onBegin(const char* missionId) = 0;
	//完成
	virtual void onCompleted(const char* missionId) = 0;
	//失败
	virtual void onFailed(const char* missionId, const char* cause) = 0;
};
}

#endif /* define(__FXPlugin_PluginAnalytics_H__) */