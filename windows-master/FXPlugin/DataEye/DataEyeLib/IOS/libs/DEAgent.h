//
//  DCSdk.h
//  DCSdk
//
//  Created by xqwang on 13-7-8.
//  Copyright (c) 2013年 digitcube. All rights reserved.
//

//#if !(TARGET_IOS || TARGET_UNITY || TARGET_FLASH)

#ifndef __DataEye__DEAgenth__
#define __DataEye__DEAgenth__

#import <Foundation/Foundation.h>
#import "DCAccountType.h"
#import "DCGender.h"
#import "DCTaskType.h"
#import "DCReportMode.h"

//这里提供静态方法
@interface DEAgent : NSObject

/**
 *  @brief 设置数据上报模式，DEFAULT不需要用户调用login，直接以设备ID为账号上报数据，
 *         AFTER_LOGIN需要接入者调用login接口，否则不会上报数据，该接口必须在onStart接口之前调用，否则无效
 */
+(void)setReportMode:(DCReportMode)mode;

/**
 * @brief 设置数据上报间隔时间，该接口需要在onStart接口之前调用，否则无效。如果不调用该接口，SDK默认以1分钟的间隔上报数据
 *
 **/
+(void)setUploadInterval:(unsigned int)time;

/**
 *  @brief 程序启动时调用
 *
 *  一般在- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions内调用一次即可
 *  !!!一定要调用，否则后面无法上报,只能调用一次
 *  @param appId         在我们网站上创建游戏时分配的appId,用于标识这款游戏. 注意保密!
 *  @param channelId    渠道id , 方便统计分析你在各渠道的推广效果
 */
+ (void)onStart:(NSString *)appId withChannelId:(NSString*)channelId;

/**
 *  @brief 设置开启Log
 *
 *  (1)打印流程信息
 *  (2)错误提示
 *  默认关闭，开发调试时打开，请在正式发布前关闭。
 */
+(void)setDebugMode:(BOOL)mode;

/**
 * @brief 设置应用版本号，不调用该接口时SDK默认从配置文件中读取应用版本号
 *
 * @param version:应用版本号
 */
+(void)setVersion:(NSString*)version;

/**
 * @brief 游戏异常上报接口
 *
 * @param errorTitle:异常名
 * @param content:异常堆栈
 */
+(void)reportError:(NSString*)errorTitle content:(NSString*)content;

/**
 * @brief 立即上报数据
 **/
+(void)uploadNow;

/**
 * @brief 获取设备ID
 **/
+(NSString*)getUID;

@end

@interface DEAccount : NSObject
/**
 *	@brief	设置帐号
 *
 *  ！！！必须调用，上报系统信赖accountId进行数据分析
 *
 *	@param 	accountId   帐号id，必须分配且唯一。
 */
+ (void)login:(NSString *)accountId;

/**
 *  @brief 账号登陆，及设置账号所在区服
 *
 *  @param accountId  帐号id，必须分配且唯一。
 *
 *  @param gameServer 账号所在区服
 **/
+(void)login:(NSString *)accountId gameServer:(NSString*)gameServer;

/**
 *  @brief 退出时调用
 *
 */
+ (void)logout;

/**
 * @brief 获得当前登陆用户的账号ID
 **/
+(NSString*)getAccountId;

/**
 *  @brief 标记玩家属于哪一类玩家，即给玩家加玩家类型标记，如：任务党、成就党、聊天党等
 *
 */
//+(void)tag:(NSString*)tag;

/**
 *  @brief 将一种玩家标记从玩家身上去除
 *
 */
//+(void)unTag:(NSString*)tag;

+(void)addTag:(NSString*)tag subTag:(NSString*)subTag;

+(void)removeTag:(NSString*)tag subTag:(NSString*)subTag;

/**
 *	@brief	设置帐号类型
 *	@param 	accountType     账户类型   
 */
+ (void)setAccountType:(DCAccountType)accountType;

/**
 *	@brief	设置帐号等级
 *
 *  !!!用户升级后务必调用
 *	@param 	level       升级之后的等级
 */
+ (void)setLevel:(int)level;

/**
 *	@brief	设置性别
 *	@param 	gender      性别 
 */
+ (void)setGender:(DCGender)gender;

/**
 *	@brief	设置年龄
 *	@param 	age     年龄
 */
+ (void)setAge:(int)age;

/**
 *	@brief	设置区服
 *	@param  gameServer      区服
 */
+ (void)setGameServer:(NSString *)gameServer;

@end

@interface DEVirtualCurrency : NSObject

/**
 *	@brief	虚拟币充值请求，用于只能发起充值请求，无法获取充值完成详情的情形
 *
 *	@param 	currencyAmount          现金金额
 *	@param 	currencyType            币种       需要使用ISO4217中规范的3字母代码，如美元USD、人民币CNY等
 *	@param 	paymentType             支付类型    
 *  @param levelId                  付费成功时当前所在关卡ID
 */
+(void)paymentSuccess:(NSString*)orderId
       currencyAmount:(double)currencyAmount
         currencyType:(NSString *)currencyType
          paymentType:(NSString *)paymentType;

+(void)paymentSuccessInLevel:(NSString*)orderId
       currencyAmount:(double)currencyAmount
         currencyType:(NSString *)currencyType
          paymentType:(NSString *)paymentType
              levelId:(NSString*)levelId;
@end

@interface DEItem : NSObject

/**
 *  @brief 道具购买
 *  @param itemId    道具id或者名称，需要保证该值在游戏中的唯一性，非空，最大32个字符
 *  @param itemType   道具类型，最大32个字符
 *  @param itemCount  购买的道具数量
 *  @param virtualCurrency 购买的道具虚拟价值，非空，最大32字符
 *  @param currencyType 计算道具虚拟价值所用的货币类型，非空，最大32字符
 *  @param consumePoint 付费点，用于有目的性的购买某个道具，比如为通过某一关卡，在关卡内购买道具，该参数可以为空
 *  @param levelId 当前所在关卡ID，可以为空
 */
+(void)buy:(NSString*)itemId
      type:(NSString*)itemType
 itemCount:(int)itemCount
virtualCurrency:(long long)virtualCurrency
currencyType:(NSString*)currencyType
consumePoint:(NSString*)consumePoint;

+(void)buyInLevel:(NSString*)itemId
             type:(NSString*)itemType
        itemCount:(int)itemCount
  virtualCurrency:(long long)virtualCurrency
     currencyType:(NSString*)currencyType
     consumePoint:(NSString*)consumePoint
          levelId:(NSString*)levelId;

/**
 *  @brief 因某种原因消耗道具
 *  @param itemId    道具id或者名称，需要保证该值在游戏中的唯一性，非空，最大32个字符
 *  @param itemType   道具类型
 *  @param itemCount  消耗的道具数量
 *  @param reason     消耗道具的原因，非空
 *  @param levelId    当前所在关卡ID，可以为空
 */
+(void)consume:(NSString*)itemId
          type:(NSString*)itemType
     itemCount:(int)itemCount
        reason:(NSString*)reason;

+(void)consumeInLevel:(NSString*)itemId
                 type:(NSString*)itemType
            itemCount:(int)itemCount
               reason:(NSString*)reason
              levelId:(NSString*)levelId;

/**
 *  @brief 因某种原因获得道具
 *  @param itemId    道具id或者名称，需要保证该值在游戏中的唯一性，非空，最大32个字符
 *  @param itemType   道具类型
 *  @param itemCount  获得的道具数量
 *  @param reason     获得道具的原因
 *  @param levelId    当前所在关卡ID，可以为空
 */
+(void)get:(NSString*)itemId
      type:(NSString*)itemType
 itemCount:(int)itemCount
    reason:(NSString*)reason;

+(void)getInLevel:(NSString*)itemId
             type:(NSString*)itemType
        itemCount:(int)itemCount
           reason:(NSString*)reason
          levelId:(NSString*)levelId;

@end

@interface DETask : NSObject

+(DCTaskType)getType:(int)type;

/**
 *  @brief 开始任务
 *  @param taskId    任务id或者名称，需要保证该值在游戏中的唯一性，非空，最大32个字符
 *  @param taskType   任务类型
 *  
 */
+(void)begin:(NSString*)taskId taskType:(DCTaskType)type;

/**
 *  @brief 任务完成
 *  @param taskId    任务id或者名称，需要保证该值在游戏中的唯一性，非空，最大32个字符
 *
 */
+(void)complete:(NSString*)taskId;

/**
 *  @brief 任务失败
 *  @param taskId    任务id或者名称，需要保证该值在游戏中的唯一性，非空，最大32个字符
 *  @param taskType   任务失败原因
 *
 */
+(void)fail:(NSString*)taskId reason:(NSString*)reason;

@end

@interface DEEvent : NSObject

+(void)onEventBeforeLogin:(NSString*)eventId dictionary:(NSDictionary*)dic duration:(long long)duration;

/**
 *  @brief 自定义事件
 *  @param eventId    自定义事件ID， 最大32个字符
 *  @param duration   事件发生时长
 *  @param map        事件关注的属性值对
 */
 + (void)onEventCount:(NSString *)eventId count:(int)count;
 + (void)onEvent : (NSString*)eventId;
 + (void)onEvent : (NSString*)eventId label:(NSString*)label;
 + (void)onEvent : (NSString*)eventId dictionary:(NSDictionary*)dic;

 + (void)onEventDuration : (NSString*)eventId duration:(long long)duration;
 + (void)onEventDuration : (NSString*)eventId label:(NSString*)label duration:(long long)duration;
 + (void)onEventDuration : (NSString*)eventId dictionary:(NSDictionary*)dic duration:(long long)duration;
 
/**
 *  @brief 过程性事件开始接口，用于准确统计该事件发生的时长,注意该事件发生的最小时间为1分钟，否则时间统计不准，或者为0
 *  @eventId 自定义事件ID
 *
 */
 + (void)onEventBegin : (NSString*)eventId;
 + (void)onEventBegin : (NSString*)eventId dictionary:(NSDictionary*)dic;
 + (void)onEventEnd : (NSString*)eventId;
 + (void)onEventEnd : (NSString *)eventId dictionary:(NSDictionary*)dic;

 + (void)onEventBegin : (NSString*)eventId dictionary:(NSDictionary*)dic flag:(NSString*)flag;
 + (void)onEventEnd : (NSString*)eventId flag:(NSString*)flag;
 + (void)onEventEnd : (NSString*)eventId dictionary:(NSDictionary*)dic flag:(NSString*)flag;
 
@end

@interface DECoin : NSObject

/**
 *  @brief 设置玩家金币总量
 *  @param coinType 虚拟币类型
 *
 **/
+(void)setCoinNum:(long long)totalCoin coinType:(NSString*)coinType;

/**
 *  @brief 玩家失去虚拟币
 *  @param reason 失去虚拟币原因，后台以该参数作为统计ID
 *  @param lostCoin 失去虚拟币数量
 *  @param leftCoin 剩余虚拟币总量
 *  @param coinType 虚拟币类型
 *  @param levelId 当前所在关卡ID，可以为空
 **/
+(void)lost:(NSString*)reason coinType:(NSString*)coinType lostCoin:(long long)lost leftCoin:(long long)left;
+(void)lostInLevel:(NSString*)reason coinType:(NSString*)coinType lostCoin:(long long)lost leftCoin:(long long)left levelId:(NSString*)levelId;

/**
 *  @brief 玩家获得虚拟币
 *  @param reason 获得虚拟币原因，后台以该参数作为统计ID
 *  @param gainCoin 获得虚拟币数量
 *  @param leftCoin 剩余虚拟币总量
 *  @param coinType 虚拟币类型
 *  @param levelId 当前所在关卡ID，可以为空
 **/
+(void)gain:(NSString*)reason coinType:(NSString*)coinType gainCoin:(long long)gain leftCoin:(long long)left;
+(void)gainInLevel:(NSString*)reason coinType:(NSString*)coinType gainCoin:(long long)gain leftCoin:(long long)left levelId:(NSString*)levelId;
@end

#define DATAEYE_CONFIG_PARAMS_UPDATE_COMPLETE    @"DataeyeConfigParamsComplete"
typedef void (*CALLBACK)(const char* event);

@interface DEConfigParams : NSObject

-(void)update;

+(void)update;

-(void)setCallBack:(CALLBACK)func;

+(int)getParameterInt:(NSString*)key defaults:(int)value;

+(long long)getParameterLong:(NSString*)key defaults:(long long)value;

+(BOOL)getParameterBool:(NSString*)key defaults:(BOOL)value;

+(NSString*)getParameterString:(NSString*)key defaults:(NSString*)value;
@end

@interface DELevels : NSObject

+(void)begin:(NSString*)levelId;

+(void)complete:(NSString*)levelId;

+(void)fail:(NSString*)levelId failPoint:(NSString*)failPoint;

@end

@interface DECardsGame : NSObject

/**
 *  @brief 玩家房间完成一局游戏后调用
 *  @brief lostOrGainCoin    获得或者丢失的虚拟币数量（不能为零），胜利时为正值，输掉一局传入负值
 *  @param taxCoin  完成一局游戏时系统需要回收的虚拟币数量（税收）
 *  @param leftCoin   玩家剩余的虚拟币总量
 */
+(void)play:(NSString*)roomId label:(NSString*)label coinType:(NSString*)coinType lostOrGainCoin:(long long)coinNum taxCoin:(long long)tax leftCoin:(long long)left;

/**
 *  @brief 玩家房间内丢失虚拟币时调用（完成一局游戏调用play接口后不必再调用该接口）
 *  @brief reason    虚拟币丢失原因
 *  @param lostCoin  丢失的虚拟币数量，传入正值即可
 *  @param leftCoin   剩余的虚拟币数量
 */
+(void)lost:(NSString*)roomId reason:(NSString*)reason coinType:(NSString*)coinType lostCoin:(long long)lost leftCoin:(long long)left;

/**
 *  @brief 玩家房间内获得虚拟币时调用（完成一局游戏调用play接口后不必再调用该接口）
 *  @param reason    虚拟币获得原因
 *  @param gainCoin   赢得的虚拟币数量，传入正值即可
 *  @param leftCoin  剩余的虚拟币数量
 */
+(void)gain:(NSString*)roomId reason:(NSString*)reason coinType:(NSString*)coinType gainCoin:(long long)gain leftCoin:(long long)left;

@end

#endif

//#endif /*!(TARGET_IOS || TARGET_UNITY || TARGET_FLASH)*/
