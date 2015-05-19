//
//  IAPHelper.mm
//
//  Created by Jiangjie on 13-5-27.
//
//

#import "AppStoreIOS.h"
#include "AppStore.h"
#include "VersionControl.h"

@implementation AppStoreIOS

@synthesize _SKProducts;

//单例对象
static AppStoreIOS* _instanceAppStoreIOS = nil;


-(void)dealloc
{
    if (_instanceAppStoreIOS != nil)
    {
        [self._SKProducts removeAllObjects];
        [self._SKProducts release];
        [[SKPaymentQueue defaultQueue] removeTransactionObserver:_instanceAppStoreIOS];
        _instanceAppStoreIOS = nil;
    }
    [super dealloc];
}

+(AppStoreIOS*)getInstance
{
    if (_instanceAppStoreIOS == nil)
    {
        //如果没有,则生成
        _instanceAppStoreIOS = [[self alloc] init];
       _instanceAppStoreIOS.self._SKProducts = [[NSMutableArray alloc] initWithCapacity:0];
        _instanceAppStoreIOS->_bInit = YES;
        
        //初始化监听
        [[SKPaymentQueue defaultQueue] addTransactionObserver:_instanceAppStoreIOS];
        
        [_instanceAppStoreIOS initSKProducts];
    }
    return _instanceAppStoreIOS;
}

-(void)initSKProducts
{
    NSArray *productArray = nil;
    
#if Version_Free != 0
    productArray = [[NSArray alloc] initWithObjects:
                    @"buy.all.solution.free",
                    @"buy.one.supertool.free",
                    @"buy.ten.supertool.free",
                    nil];
#else
    productArray = [[NSArray alloc] initWithObjects:
                             @"buy.all.solution",
                             @"buy.one.supertool",
                             @"buy.ten.supertool",
                             nil];
#endif 
    NSSet *nsset = [NSSet setWithArray:productArray];
    
    SKProductsRequest* productRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:nsset];
    
    productRequest.delegate = _instanceAppStoreIOS;
    [productRequest start];
    [productArray release];
    [productRequest release];
    NSLog(@"初始化商品信息....");
}

//根据产品ID来请求数据
-(void)requestProductData:(NSString*)productID
{
    NSArray *productArray = [[NSArray alloc] initWithObjects:productID, nil];
    NSSet *nsset = [NSSet setWithArray:productArray];
    
    SKProductsRequest* productRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:nsset];
    
    productRequest.delegate = self;
    [productRequest start];
    [productArray release];
    [productRequest release];
    NSLog(@"请求商品信息....");
}

//接收产品请求返回的数据
-(void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    NSLog(@"得到商品信息....");
    NSArray* products = response.products;
    
    SKProduct* product = nil;
    if (products.count <= 0)
    {
        NSLog(@"没有商品信息.....");
        if (!_bInit)
        {
            FX::Plugin::AppStore::getInstance()->onPayResult(FX::PayResultCode::ePayFail, "");
        }
        _bInit = NO;
        return;
    }
    
    //缓存购买信息
    for (int i = 0; i < products.count; ++i)
    {
        product = [products objectAtIndex:i];
        NSLog(@"商品名: %@, 价格: %f, ID: %@",[product localizedTitle],
              [[product price] doubleValue], [product productIdentifier]);
        if ([self isInSKProducts:product.productIdentifier])
        {
            continue;
        }
        [_instanceAppStoreIOS->_SKProducts addObject:product];
    }
    
    NSLog(@"缓存数量 is %lu", (unsigned long)self._SKProducts.count);
    if (!_bInit && products.count == 1)
    {
        product = [products objectAtIndex:0];
        SKPayment *payment = [SKPayment paymentWithProduct:product];
        [[SKPaymentQueue defaultQueue] addPayment:payment];
    }
    
    _bInit = NO;
}

-(SKProduct*)isInSKProducts:(NSString*)productID
{
    SKProduct* product = nil;
    for (int i = 0; i <self._SKProducts.count; ++i)
    {
        product = [_instanceAppStoreIOS->_SKProducts objectAtIndex:i];
        if ([product.productIdentifier isEqualToString:productID])
        {
            return product;
        }
    }
    return nil;
}

-(void)requestDidFinish:(SKRequest *)request
{
    NSLog(@"----------反馈信息结束--------------");
    _bInit = NO;
}

//弹出错误信息
- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    NSLog(@"-------弹出错误信息----------");
    NSLog(@"%@", [error localizedDescription]);
    
    if (_bInit)
    {
        return;
    }
    _bInit = NO;
    FX::Plugin::AppStore::getInstance()->onPayResult(FX::PayResultCode::ePayFail, "");
//    UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Alert",NULL) message:[error localizedDescription]
//                                                       delegate:nil cancelButtonTitle:NSLocalizedString(@"Close",nil) otherButtonTitles:nil];
//    [alerView show];
//    [alerView release];
}

-(void)buyProduct:(NSString *)productID
{
    _bInit = NO;
    NSLog(@"开始购买....");
    //如果允许程序内购买
    if ([SKPaymentQueue canMakePayments])
    {
        //如果没有找到缓存的，则请求数据，然后再发起交易
        //如果有，那就取缓存的直接发起交易
        SKProduct* product =  [self isInSKProducts:productID];
        if (nil == product)
        {
            [self requestProductData:productID];
        }
        else
        {
            SKPayment *payment = [SKPayment paymentWithProduct:product];
            [[SKPaymentQueue defaultQueue] addPayment:payment];
        }
    }
    else
    {
        NSLog(@"不允许内购买");
    }
}

-(void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction* transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
            {
                [self completeTransaction:transaction];
            }
                break;
                
            case SKPaymentTransactionStateFailed:
            {
                [self failedTransaction:transaction];
            }
                break;
                
            case SKPaymentTransactionStateRestored:
            {
                [self restoreTransaction:transaction];
            }
                break;
                
            case SKPaymentTransactionStatePurchasing:
            {
                NSLog(@"交易正在进行中....");
            }
                break;
                
            default:
                break;
        }
    }
}
-(void)failedTransaction:(SKPaymentTransaction *)transaction
{
    NSLog(@"failed....");
    NSLog(@"-------弹出失败信息----------");
    
    NSLog(@"%@", [transaction.error localizedDescription]);
    
    //移除交易队列
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    if (_bInit)
    {
        return;
    }
    _bInit = NO;
//    UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Alert",NULL) message:[transaction.error localizedDescription]
//                                                       delegate:nil cancelButtonTitle:NSLocalizedString(@"Close",nil) otherButtonTitles:nil];
//    [alerView show];
//    [alerView release];
    
    FX::Plugin::AppStore::getInstance()->onPayResult(FX::PayResultCode::ePayFail, "");
}
-(void)completeTransaction:(SKPaymentTransaction *)transaction
{
    NSLog(@"complete....");
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    
    FX::Plugin::AppStore::getInstance()->onPayResult(FX::PayResultCode::ePaySuccess, "");
}
-(void)restoreTransaction:(SKPaymentTransaction *)transaction
{
    NSLog(@"restore....");
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    
    FX::Plugin::AppStore::getInstance()->onPayResult(FX::PayResultCode::ePaySuccess, "");
}


@end