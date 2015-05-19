#ifndef __FXPlugin_TalkingData_H__
#define __FXPlugin_TalkingData_H__


#include "FXPlugin/PluginAnalytics.h"


#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;

#include <map>
#include <string>

//#include "TalkingDataGameAnalytics/include/TalkingData.h"
class TDCCAccount;

namespace FX{ namespace Plugin{

class TalkingData : public PluginAnalytics
{
private:
	TalkingData();
public:
	static TalkingData* getInstance();

public:
	////////////////////////////////////////////////////////////////////////////////////////////
	//1. custom event   --- 自定义事件
	////////////////////////////////////////////////////////////////////////////////////////////
	virtual void logError(const char* errorId, const char* message);
	virtual void logEvent(const char* eventId, LogEventMap* paramMap = NULL);
/*	virtual void logTimedEventBegin(const char* eventId);
	virtual void logTimedEventEnd(const char* eventId);*/


	////////////////////////////////////////////////////////////////////////////////////////////
	//2. Account   --- 账户 统计
	////////////////////////////////////////////////////////////////////////////////////////////
    /**
     @brief 设置账户id，在使用账户统计功能时必须先设置id，accountId需保证全局唯一
	 @param 如果期望以设备为玩家单元进行统计，请在setAccount时传入getDeviceId()，SDK将帮助自动定义设备ID作为Account
     */
	virtual void setAccount(const char* accountId);
	virtual const char* getDeviceId();
	//设置帐户类型
	virtual void setAccountType(AccountType accountType);
	//设置帐户的显性名
	virtual void setAccountName(const char* accountName);
	//设置级别
	virtual void setLevel(int level);
	//设置性别
	virtual void setGender(AcountGender gender);
	//设置年龄  0-120
	virtual void setAge(int age);
	//设置区服
	virtual void setGameServer(const char* gameServer);


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
	 2.iapId(内购物品名/ID)
	 3.currencyAmount(货币数量)
	 4.currencyType(货币类型)请使用国际标准组织ISO 4217中规范的3位字母代码标记货币类型。例：人民币CNY；美元USD；欧元EUR
	 5.virtualCurrencyAmount(参数3换算后的游戏内虚拟币数量)
	 6.paymentType(支付渠道)例如：“支付宝”“苹果官方”“XX支付SDK”
	*/
	virtual void onChargeRequest(const char* orderId, const char* itemName, double currencyAmount, 
		const char* currencyType, double virtualCurrencyAmount, const char* paymentType);
	//充值成功
	virtual void onChargeSuccess(const char* orderId);
	//赠予虚拟币
	/*
	 *pref:在任务奖励、登录奖励、成就奖励等环节免费发放给玩家虚拟币,
		只获得过赠予虚拟币的玩家不会被记为付费玩家。赠予的虚拟币会计入到所有的虚拟币产出中
	*/
	virtual void onReward(double virtualCurrencyAmount, const char* reason);


	////////////////////////////////////////////////////////////////////////////////////////////
	//4. Item   --- 跟踪 游戏内有价值的虚拟币的消费点
	////////////////////////////////////////////////////////////////////////////////////////////
	//记录付费点 (在任意消费点发生时尽快调用)
	virtual void onPurchase(const char* itemName, int itemNumber, double priceInVirtualCurrency);
	//消耗物品或服务等 (在某个道具/服务被用掉（消失）时尽快调用)  
	// talkingdata 没有reason字段
	virtual void onUse(const char* itemName, int itemNumber, const char* reason);


	////////////////////////////////////////////////////////////////////////////////////////////
	//5. Mission   --- 跟踪 任务/关卡/副本
	////////////////////////////////////////////////////////////////////////////////////////////
	/*
	 *同一个missionId如果在未结束前，重复进行了onBegin调用，则重新开始计时，上一次的调用被丢弃。
	*/
	//接受或进入
	virtual void onBegin(const char* missionId);
	//完成
	virtual void onCompleted(const char* missionId);
	//失败
	virtual void onFailed(const char* missionId, const char* cause);



private:
	TDCCAccount * m_tdAccount;

};

}}

#endif /* define(__FXPlugin_TalkingData_H__) */