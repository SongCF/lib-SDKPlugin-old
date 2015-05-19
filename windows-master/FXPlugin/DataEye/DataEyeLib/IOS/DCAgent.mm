//
//  DCAgent.mm
//  coco2d-sdk
//
//  Created by xqwang on 13-11-4.
//
//

#import "DCAgent.h"
#import "DEAgent.h"

void DCAgent::onStart(const char* appId, const char* channelId)
{
    if(appId == NULL || channelId == NULL)
    {
        return;
    }
    NSString* objcAppId = [NSString stringWithCString:appId encoding:NSUTF8StringEncoding];
    NSString* objcChannelId = [NSString stringWithCString:channelId encoding:NSUTF8StringEncoding];
    [DEAgent onStart:objcAppId withChannelId:objcChannelId];
}

void DCAgent::setDebugMode(bool mode)
{
    [DEAgent setDebugMode:mode];
}

void DCAgent::setReportMode(DCReportMode mode)
{
    [DEAgent setReportMode:mode];
}

void DCAgent::setUploadInterval(unsigned int time)
{
    [DEAgent setUploadInterval:time];
}

void DCAgent::setVersion(const char* version)
{
	if(version == NULL)
	{
		return;
	}
    NSString* objcVersion = [NSString stringWithCString:version encoding:NSUTF8StringEncoding];
    [DEAgent setVersion:objcVersion];
}

void DCAgent::reportError(const char* title, const char* content)
{
	if(title == NULL || content == NULL)
	{
		return;
	}
    NSString* objcTitle = [NSString stringWithCString:title encoding:NSUTF8StringEncoding];
    NSString* objcContent = [NSString stringWithCString:content encoding:NSUTF8StringEncoding];
    [DEAgent reportError:objcTitle content:objcContent];
}

void DCAgent::uploadNow()
{
    [DEAgent uploadNow];
}

const char* DCAgent::getUID()
{
    NSString* objcUID = [DEAgent getUID];
    if (objcUID == nil) {
        return NULL;
    }
    return [objcUID UTF8String];
}

void DCAccount::login(const char* accountId)
{
    if (accountId == NULL) 
	{
        return;
    }
    NSString* objcAccountId = [NSString stringWithCString:accountId encoding:NSUTF8StringEncoding];
    [DEAccount login:objcAccountId];
}

void DCAccount::login(const char* accountId, const char* gameServer)
{
    if (accountId == NULL || gameServer == NULL) 
	{
        return;
    }
    NSString* objcAccountId = [NSString stringWithCString:accountId encoding:NSUTF8StringEncoding];
    NSString* objcGameServer = [NSString stringWithCString:gameServer encoding:NSUTF8StringEncoding];
    [DEAccount login:objcAccountId gameServer:objcGameServer];
}

void DCAccount::logout()
{
    [DEAccount logout];
}

const char* DCAccount::getAccountId()
{
    NSString* objcAccountId = [DEAccount getAccountId];
    if (objcAccountId == nil) {
        return NULL;
    }
    return [objcAccountId UTF8String];
}

void DCAccount::setAccountType(DCAccountType accountType)
{
    [DEAccount setAccountType:accountType];
}

void DCAccount::setLevel(int level)
{
    [DEAccount setLevel:level];
}

void DCAccount::setGender(DCGender gender)
{
    [DEAccount setGender:gender];
}

void DCAccount::setAge(int age)
{
    [DEAccount setAge:age];
}

void DCAccount::setGameServer(const char* gameServer)
{
    if (gameServer == NULL) 
	{
        return;
    }
    NSString* objcGameServer = [NSString stringWithCString:gameServer encoding:NSUTF8StringEncoding];
    [DEAccount setGameServer:objcGameServer];
}

void DCAccount::addTag(const char* tag, const char* subTag)
{
    if (tag == NULL || subTag == NULL) 
	{
        return;
    }
    NSString* objcTag = [NSString stringWithCString:tag encoding:NSUTF8StringEncoding];
    NSString* objcSubTag = [NSString stringWithCString:subTag encoding:NSUTF8StringEncoding];
    [DEAccount addTag:objcTag subTag:objcSubTag];
}

void DCAccount::removeTag(const char* tag, const char* subTag)
{
    if (tag == NULL || subTag == NULL) 
	{
        return;
    }
    NSString* objcTag = [NSString stringWithCString:tag encoding:NSUTF8StringEncoding];
    NSString* objcSubTag = [NSString stringWithCString:subTag encoding:NSUTF8StringEncoding];
    [DEAccount removeTag:objcTag subTag:objcSubTag];
}

void DCVirtualCurrency::paymentSuccess(const char* orderId, double currencyAmount, const char* currencyType, const char* paymentType)
{
    if (orderId == NULL || currencyType == NULL || paymentType == NULL) 
	{
        return;
    }
	NSString* objcOrderId = [NSString stringWithCString:orderId encoding:NSUTF8StringEncoding];
    NSString* objcCurrencyType = [NSString stringWithCString:currencyType encoding:NSUTF8StringEncoding];
    NSString* objcPaymentType = [NSString stringWithCString:paymentType encoding:NSUTF8StringEncoding];
	
	[DEVirtualCurrency paymentSuccess:objcOrderId currencyAmount:currencyAmount currencyType:objcCurrencyType paymentType:objcPaymentType];
}

void DCVirtualCurrency::paymentSuccessInLevel(const char* orderId, double currencyAmount, const char* currencyType, const char* paymentType, const char* levelId)
{
    if (orderId == NULL || currencyType == NULL || paymentType == NULL || levelId == NULL) 
	{
        return;
    }
    NSString* objcOrderId = [NSString stringWithCString:orderId encoding:NSUTF8StringEncoding];
    NSString* objcCurrencyType = [NSString stringWithCString:currencyType encoding:NSUTF8StringEncoding];
    NSString* objcPaymentType = [NSString stringWithCString:paymentType encoding:NSUTF8StringEncoding];
    NSString* objcLevelId = [NSString stringWithCString:levelId encoding:NSUTF8StringEncoding];
    [DEVirtualCurrency paymentSuccessInLevel:objcOrderId currencyAmount:currencyAmount currencyType:objcCurrencyType paymentType:objcPaymentType levelId:objcLevelId];
}

void DCItem::buy(const char* itemId, const char* itemType, int itemCount, long long virtualCurrency, const char* currencyType, const char* consumePoint)
{
    if (itemId == NULL || itemType == NULL || currencyType == NULL || consumePoint == NULL) 
	{
        return;
    }
    NSString* objcItemId = [NSString stringWithCString:itemId encoding:NSUTF8StringEncoding];
    NSString* objcItemType = [NSString stringWithCString:itemType encoding:NSUTF8StringEncoding];
    NSString* objcCurrencyType = [NSString stringWithCString:currencyType encoding:NSUTF8StringEncoding];
    NSString* objcConsumePoint = [NSString stringWithCString:consumePoint encoding:NSUTF8StringEncoding];
	
    [DEItem buy:objcItemId type:objcItemType itemCount:itemCount virtualCurrency:virtualCurrency currencyType:objcCurrencyType consumePoint:objcConsumePoint];
}

void DCItem::buyInLevel(const char* itemId, const char* itemType, int itemCount, long long virtualCurrency, const char* currencyType, const char* consumePoint, const char* levelId)
{
    if (itemId == NULL || itemType == NULL || currencyType == NULL || consumePoint == NULL || levelId == NULL) 
	{
        return;
    }
    NSString* objcItemId = [NSString stringWithCString:itemId encoding:NSUTF8StringEncoding];
    NSString* objcItemType = [NSString stringWithCString:itemType encoding:NSUTF8StringEncoding];
    NSString* objcCurrencyType = [NSString stringWithCString:currencyType encoding:NSUTF8StringEncoding];
    NSString* objcConsumePoint = [NSString stringWithCString:consumePoint encoding:NSUTF8StringEncoding];
    NSString* objcLevelId = [NSString stringWithCString:levelId encoding:NSUTF8StringEncoding];
    
    [DEItem buyInLevel:objcItemId type:objcItemType itemCount:itemCount virtualCurrency:virtualCurrency currencyType:objcCurrencyType consumePoint:objcConsumePoint levelId:objcLevelId];
}

void DCItem::get(const char *itemId, const char *itemType, int itemCount, const char *reason)
{
    if (itemId == NULL || itemType == NULL || reason == NULL) 
	{
        return;
    }
    NSString* objcItemId = [NSString stringWithCString:itemId encoding:NSUTF8StringEncoding];
    NSString* objcItemType = [NSString stringWithCString:itemType encoding:NSUTF8StringEncoding];
    NSString* objcReason = [NSString stringWithCString:reason encoding:NSUTF8StringEncoding];
    
    [DEItem get:objcItemId type:objcItemType itemCount:itemCount reason:objcReason];
}

void DCItem::getInLevel(const char *itemId, const char *itemType, int itemCount, const char *reason, const char* levelId)
{
    if (itemId == NULL || itemType == NULL || reason == NULL || levelId == NULL) 
	{
        return;
    }
    NSString* objcItemId = [NSString stringWithCString:itemId encoding:NSUTF8StringEncoding];
    NSString* objcItemType = [NSString stringWithCString:itemType encoding:NSUTF8StringEncoding];
    NSString* objcReason = [NSString stringWithCString:reason encoding:NSUTF8StringEncoding];
    NSString* objcLevelId = [NSString stringWithCString:levelId encoding:NSUTF8StringEncoding];
    
    [DEItem getInLevel:objcItemId type:objcItemType itemCount:itemCount reason:objcReason levelId:objcLevelId];
}

void DCItem::consume(const char *itemId, const char *itemType, int itemCount, const char *reason)
{
    if (itemId == NULL || itemType == NULL || reason == NULL) 
	{
        return;
    }
    NSString* objcItemId = [NSString stringWithCString:itemId encoding:NSUTF8StringEncoding];
    NSString* objcItemType = [NSString stringWithCString:itemType encoding:NSUTF8StringEncoding];
    NSString* objcReason = [NSString stringWithCString:reason encoding:NSUTF8StringEncoding];
    
    [DEItem consume:objcItemId type:objcItemType itemCount:itemCount reason:objcReason];
}

void DCItem::consumeInLevel(const char *itemId, const char *itemType, int itemCount, const char *reason, const char* levelId)
{
    if (itemId == NULL || itemType == NULL || reason == NULL || levelId == NULL) 
	{
        return;
    }
    NSString* objcItemId = [NSString stringWithCString:itemId encoding:NSUTF8StringEncoding];
    NSString* objcItemType = [NSString stringWithCString:itemType encoding:NSUTF8StringEncoding];
    NSString* objcReason = [NSString stringWithCString:reason encoding:NSUTF8StringEncoding];
    NSString* objcLevelId = [NSString stringWithCString:levelId encoding:NSUTF8StringEncoding];
    
    [DEItem consumeInLevel:objcItemId type:objcItemType itemCount:itemCount reason:objcReason levelId:objcLevelId];
}

void DCTask::begin(const char *taskId, DCTaskType type)
{
    if (taskId == NULL) 
	{
        return;
    }
    NSString* objcTaskId = [NSString stringWithCString:taskId encoding:NSUTF8StringEncoding];
    
    [DETask begin:objcTaskId taskType:type];
}

void DCTask::complete(const char *taskId)
{
    if (taskId == NULL) 
	{
        return;
    }
    NSString* objcTaskId = [NSString stringWithCString:taskId encoding:NSUTF8StringEncoding];
    
    [DETask complete:objcTaskId];
}

void DCTask::fail(const char *taskId, const char *reason)
{
    if (taskId == NULL || reason == NULL) 
	{
        return;
    }
    NSString* objcTaskId = [NSString stringWithCString:taskId encoding:NSUTF8StringEncoding];
    NSString* objcReason = [NSString stringWithCString:reason encoding:NSUTF8StringEncoding];
    
    [DETask fail:objcTaskId reason:objcReason];
}

void DCEvent::onEventBeforeLogin(const char* eventId, map<string, string>* map, long long duration)
{
    if (eventId == NULL) 
	{
        return;
    }
    NSString* objcEventId = [NSString stringWithCString:eventId encoding:NSUTF8StringEncoding];
    
    NSMutableDictionary* dictionary = nil;
    if (map != NULL && (map->size() != 0)) 
	{
        dictionary = [[NSMutableDictionary alloc] initWithCapacity:map->size()];
        std::map<std::string, std::string>::iterator it;
        for (it = map->begin(); it != map->end(); ++it) 
		{
            NSString* key = [NSString stringWithCString:it->first.c_str() encoding:NSUTF8StringEncoding];
            NSString* value = [NSString stringWithCString:it->second.c_str() encoding:NSUTF8StringEncoding];
            [dictionary setObject:value forKey:key];
        }
    }
    
    [DEEvent onEventBeforeLogin:objcEventId dictionary:dictionary duration:duration];
}

void DCEvent::onEvent(const char *eventId)
{
    if (eventId == NULL) 
	{
        return;
    }
    NSString* objcEventId = [NSString stringWithCString:eventId encoding:NSUTF8StringEncoding];
    [DEEvent onEvent:objcEventId];
}

void DCEvent::onEvent(const char* eventId, const char* label)
{
    if (eventId == NULL || label == NULL) 
	{
        return;
    }
    NSString* objcEventId = [NSString stringWithCString:eventId encoding:NSUTF8StringEncoding];
    NSString* objcLabel = [NSString stringWithCString:label encoding:NSUTF8StringEncoding];
    [DEEvent onEvent:objcEventId label:objcLabel];
}

void DCEvent::onEvent(const char *eventId, std::map<std::string, std::string>* map)
{
    if (eventId == NULL) 
	{
        return;
    }
    NSString* objcEventId = [NSString stringWithCString:eventId encoding:NSUTF8StringEncoding];
    
    NSMutableDictionary* dictionary = nil;
    if (map != NULL && (map->size() != 0)) 
	{
        dictionary = [[NSMutableDictionary alloc] initWithCapacity:5];
        std::map<std::string, std::string>::iterator it;
        for (it = map->begin(); it != map->end(); ++it) 
		{
            NSString* key = [NSString stringWithCString:it->first.c_str() encoding:NSUTF8StringEncoding];
            NSString* value = [NSString stringWithCString:it->second.c_str() encoding:NSUTF8StringEncoding];
            [dictionary setObject:value forKey:key];
        }
    }
    
    [DEEvent onEvent:objcEventId dictionary:dictionary];
}

void DCEvent::onEventCount(const char *eventId, int count)
{
    if (eventId == NULL) 
	{
        return;
    }
    NSString* objcEventId = [NSString stringWithCString:eventId encoding:NSUTF8StringEncoding];
    [DEEvent onEventCount:objcEventId count:count];
}

void DCEvent::onEventDuration(const char *eventId, long long duration)
{
    if (eventId == NULL) 
	{
        return;
    }
    NSString* objcEventId = [NSString stringWithCString:eventId encoding:NSUTF8StringEncoding];
    [DEEvent onEventDuration:objcEventId duration:duration];
}

void DCEvent::onEventDuration(const char *eventId, const char *label, long long duration)
{
    if (eventId == NULL || label == NULL) 
	{
        return;
    }
    NSString* objcEventId = [NSString stringWithCString:eventId encoding:NSUTF8StringEncoding];
    NSString* objcLabel = [NSString stringWithCString:label encoding:NSUTF8StringEncoding];
    
    [DEEvent onEventDuration:objcEventId label:objcLabel duration:duration];
}

void DCEvent::onEventDuration(const char *eventId, std::map<std::string, std::string> *map, long long duration)
{
    if (eventId == NULL) 
	{
        return;
    }
    NSString* objcEventId = [NSString stringWithCString:eventId encoding:NSUTF8StringEncoding];
    
    NSMutableDictionary* dictionary = nil;
    if (map != NULL && (map->size() != 0)) {
        dictionary = [[NSMutableDictionary alloc] initWithCapacity:5];
        std::map<std::string, std::string>::iterator it;
        for (it = map->begin(); it != map->end(); ++it) 
		{
            NSString* key = [NSString stringWithCString:it->first.c_str() encoding:NSUTF8StringEncoding] ;
            NSString* value = [NSString stringWithCString:it->second.c_str() encoding:NSUTF8StringEncoding];
            [dictionary setObject:value forKey:key];
        }
    }
    
    [DEEvent onEventDuration:objcEventId dictionary:dictionary duration:duration];
}

void DCEvent::onEventBegin(const char *eventId)
{
    if (eventId == NULL) 
	{
        return;
    }
    NSString* objcEventId = [NSString stringWithCString:eventId encoding:NSUTF8StringEncoding];
    
    [DEEvent onEventBegin:objcEventId];
}

void DCEvent::onEventBegin(const char* eventId, std::map<std::string, std::string>* map)
{
    if (eventId == NULL) 
	{
        return;
    }
    NSString* objcEventId = [NSString stringWithCString:eventId encoding:NSUTF8StringEncoding];
    
    NSMutableDictionary* dictionary = nil;
    if (map != NULL && (map->size() != 0)) 
	{
        dictionary = [[NSMutableDictionary alloc] initWithCapacity:5];
        std::map<std::string, std::string>::iterator it;
        for (it = map->begin(); it != map->end(); ++it) 
		{
            NSString* key = [NSString stringWithCString:it->first.c_str() encoding:NSUTF8StringEncoding];
            NSString* value = [NSString stringWithCString:it->second.c_str() encoding:NSUTF8StringEncoding];
            [dictionary setObject:value forKey:key];
        }
    }
    
    [DEEvent onEventBegin:objcEventId dictionary:dictionary];
}

void DCEvent::onEventEnd(const char *eventId)
{
    if (eventId == NULL) 
	{
        return;
    }
    NSString* objcEventId = [NSString stringWithCString:eventId encoding:NSUTF8StringEncoding];
    
    [DEEvent onEventEnd:objcEventId];
}

void DCEvent::onEventBegin(const char *eventId, std::map<std::string, std::string> *map, const char *flag)
{
    if (eventId == NULL || flag == NULL) 
	{
        return;
    }
    NSString* objcEventId = [NSString stringWithCString:eventId encoding:NSUTF8StringEncoding];
    NSString* objFlag = [NSString stringWithCString:flag encoding:NSUTF8StringEncoding];
    
    NSMutableDictionary* dictionary = nil;
    if (map != NULL && (map->size() != 0)) 
	{
        dictionary = [[NSMutableDictionary alloc] initWithCapacity:5];
        std::map<std::string, std::string>::iterator it;
        for (it = map->begin(); it != map->end(); ++it) 
		{
            NSString* key = [NSString stringWithCString:it->first.c_str() encoding:NSUTF8StringEncoding];
            NSString* value = [NSString stringWithCString:it->second.c_str() encoding:NSUTF8StringEncoding];
            [dictionary setObject:value forKey:key];
        }
    }
    
    [DEEvent onEventBegin:objcEventId dictionary:dictionary flag:objFlag];
}

void DCEvent::onEventEnd(const char *eventId, const char *flag)
{
    if (eventId == NULL || flag == NULL) 
	{
        return;
    }
    NSString* objcEventId = [NSString stringWithCString:eventId encoding:NSUTF8StringEncoding];
    NSString* objFlag = [NSString stringWithCString:flag encoding:NSUTF8StringEncoding];
    
    [DEEvent onEventEnd:objcEventId flag:objFlag];
}

void DCCoin::setCoinNum(long long total, const char* coinType)
{
    if (coinType == NULL) 
	{
        return;
    }
    NSString* objcCoinType = [NSString stringWithCString:coinType encoding:NSUTF8StringEncoding];
    [DECoin setCoinNum:total coinType:objcCoinType];
}

void DCCoin::lost(const char* reason, const char* coinType, long long lost, long long left)
{
    if (reason == NULL || coinType == NULL)
	{
        return;
    }
    NSString* objcReason = [NSString stringWithCString:reason encoding:NSUTF8StringEncoding];
    NSString* objcCoinType = [NSString stringWithCString:coinType encoding:NSUTF8StringEncoding];
    [DECoin lost:objcReason coinType:objcCoinType lostCoin:lost leftCoin:left];
}

void DCCoin::lostInLevel(const char* reason, const char* coinType, long long lost, long long left, const char* levelId)
{
    if (reason == NULL || coinType == NULL || levelId == NULL) 
	{
        return;
    }
    NSString* objcReason = [NSString stringWithCString:reason encoding:NSUTF8StringEncoding];
    NSString* objcCoinType = [NSString stringWithCString:coinType encoding:NSUTF8StringEncoding];
    NSString* objcLevelId = [NSString stringWithCString:levelId encoding:NSUTF8StringEncoding];
    [DECoin lostInLevel:objcReason coinType:objcCoinType lostCoin:lost leftCoin:left levelId:objcLevelId];
}

void DCCoin::gain(const char* reason, const char* coinType, long long gain, long long left)
{
    if (reason == NULL || coinType == NULL) 
	{
        return;
    }
    NSString* objcReason = [NSString stringWithCString:reason encoding:NSUTF8StringEncoding];
    NSString* objcCoinType = [NSString stringWithCString:coinType encoding:NSUTF8StringEncoding];
    [DECoin gain:objcReason coinType:objcCoinType gainCoin:gain leftCoin:left];
}

void DCCoin::gainInLevel(const char* reason, const char* coinType, long long gain, long long left, const char* levelId)
{
    if (reason == NULL || coinType == NULL) 
	{
        return;
    }
    NSString* objcReason = [NSString stringWithCString:reason encoding:NSUTF8StringEncoding];
    NSString* objcCoinType = [NSString stringWithCString:coinType encoding:NSUTF8StringEncoding];
    NSString* objcLevelId = [NSString stringWithCString:levelId encoding:NSUTF8StringEncoding];
    [DECoin gainInLevel:objcReason coinType:objcCoinType gainCoin:gain leftCoin:left levelId:objcLevelId];
}

void DCConfigParams::update()
{
    DEConfigParams* config = [[DEConfigParams alloc] init];
    [config setCallBack:DCConfig_UpdateSuccess];
    [config update];
}

static void DCConfig_UpdateSuccess(const char* event)
{
    if(strcmp(event, DCCONFIGPARAMS_UPDATE_COMPLETE) == 0)
    {
        //NotificationCenter::getInstance()->postNotification(DCCONFIGPARAMS_UPDATE_COMPLETE);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(DCCONFIGPARAMS_UPDATE_COMPLETE);
    }
}

const char* DCConfigParams::getParameterString(const char *key, const char *defaultValue)
{
    if (key == NULL || defaultValue == NULL) 
	{
        return defaultValue;
    }
    NSString* objcKey = [NSString stringWithCString:key encoding:NSUTF8StringEncoding];
    NSString* objcValue = [NSString stringWithCString:defaultValue encoding:NSUTF8StringEncoding];
    NSString* objcResult = [DEConfigParams getParameterString:objcKey defaults:objcValue];
    return [objcResult UTF8String];
}

int DCConfigParams::getParameterInt(const char *key, int defaultValue)
{
    if (key == NULL) 
	{
        return defaultValue;
    }
    NSString* objcKey = [NSString stringWithCString:key encoding:NSUTF8StringEncoding];
    return [DEConfigParams getParameterInt:objcKey defaults:defaultValue];
}

long long DCConfigParams::getParameterLong(const char *key, long long defaultValue)
{
    if (key == NULL) 
	{
        return defaultValue;
    }
    NSString* objcKey = [NSString stringWithCString:key encoding:NSUTF8StringEncoding];
    return [DEConfigParams getParameterLong:objcKey defaults:defaultValue];
}

bool DCConfigParams::getParameterBool(const char *key, bool defaultValue)
{
    if (key == NULL) 
	{
        return defaultValue;
    }
    NSString* objcKey = [NSString stringWithCString:key encoding:NSUTF8StringEncoding];
    return [DEConfigParams getParameterBool:objcKey defaults:defaultValue];
}

void DCLevels::begin(const char* levelId)
{
    if (levelId == NULL) 
	{
        return;
    }
    NSString* objcLevelId = [NSString stringWithCString:levelId encoding:NSUTF8StringEncoding];
    
    [DELevels begin:objcLevelId];
}

void DCLevels::complete(const char *levelId)
{
    if (levelId == NULL) 
	{
        return;
    }
    NSString* objcLevelId = [NSString stringWithCString:levelId encoding:NSUTF8StringEncoding];
    
    [DELevels complete:objcLevelId];
}

void DCLevels::fail(const char *levelId, const char *failPoint)
{
    if (levelId == NULL || failPoint == NULL) 
	{
        return;
    }
    NSString* objcLevelId = [NSString stringWithCString:levelId encoding:NSUTF8StringEncoding];
    NSString* objcFailPoint = [NSString stringWithCString:failPoint encoding:NSUTF8StringEncoding];
    
    [DELevels fail:objcLevelId failPoint:objcFailPoint];
}

void DCCardsGame::play(const char* roomId, const char* roomType, const char* coinType, long long lostOrGainCoin, long long taxCoin, long long leftCoin)
{
    if (roomId == NULL || roomType == NULL || coinType == NULL) 
	{
        return;
    }
    NSString* objcRoomId = [NSString stringWithCString:roomId encoding:NSUTF8StringEncoding];
    NSString* objcLabel = [NSString stringWithCString:roomType encoding:NSUTF8StringEncoding];
    NSString* objcCoinType = [NSString stringWithCString:coinType encoding:NSUTF8StringEncoding];
	
    [DECardsGame play:objcRoomId label:objcLabel coinType:objcCoinType lostOrGainCoin:lostOrGainCoin taxCoin:taxCoin leftCoin:leftCoin];
}

void DCCardsGame::lost(const char* roomId, const char* roomType, const char* coinType, long long lostCoin, long long leftCoin)
{
    if (roomId == NULL || roomType == NULL || coinType == NULL) 
	{
        return;
    }
    NSString* objcRoomId = [NSString stringWithCString:roomId encoding:NSUTF8StringEncoding];
    NSString* objcReason = [NSString stringWithCString:roomType encoding:NSUTF8StringEncoding];
	NSString* objcCoinType = [NSString stringWithCString:coinType encoding:NSUTF8StringEncoding];
    
    [DECardsGame lost:objcRoomId reason:objcReason coinType:objcCoinType lostCoin:lostCoin leftCoin:leftCoin];
}

void DCCardsGame::gain(const char* roomId, const char* roomType, const char* coinType, long long gainCoin, long long leftCoin)
{
    if (roomId == NULL || roomType == NULL || coinType == NULL) 
	{
        return;
    }
    NSString* objcRoomId = [NSString stringWithCString:roomId encoding:NSUTF8StringEncoding];
    NSString* objcReason = [NSString stringWithCString:roomType encoding:NSUTF8StringEncoding];
	NSString* objcCoinType = [NSString stringWithCString:coinType encoding:NSUTF8StringEncoding];
    
    [DECardsGame gain:objcRoomId reason:objcReason coinType:objcCoinType gainCoin:gainCoin leftCoin:leftCoin];
}



