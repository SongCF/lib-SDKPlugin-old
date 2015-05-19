//
//  IAPHelper.h
//
//  Created by Jiangjie on 13-5-27.
//
//

#ifndef __AppStoreIOS_H__
#define __AppStoreIOS_H__


#import <StoreKit/StoreKit.h>

@interface AppStoreIOS : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
    NSMutableArray* _SKProducts;
    
    BOOL  _bInit;           //主要用于初始化的时候缓存内购产品信息
}

@property (nonatomic, retain)NSMutableArray* _SKProducts;

//请求产品数据,该接口不必暴露在外面
//-(void)requestProductData:(NSString*)productID;

//购买产品
-(void)buyProduct:(NSString*)productID;

//获取单例对象
+(AppStoreIOS*)getInstance;

@end

#endif /* defined(__AppStoreIOS_H__) */
