package org.FXPlugin;

import java.util.HashMap;

import mm.purchasesdk.OnPurchaseListener;
import mm.purchasesdk.Purchase;
import mm.purchasesdk.PurchaseSkin;
import mm.purchasesdk.core.PurchaseCode;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class MMPay {
	
	private static MMPay _instanceMMPay = null;
    private MMPay(){}
    public static MMPay getInstance(){
    	if (_instanceMMPay == null){
    		_instanceMMPay = new MMPay();
    	}
    	return _instanceMMPay;
    }
    
    public void onCreate(Bundle savedInstanceState) {
    	init(FXPluginManager._activity);
	}
    public void onResume() {
	}
	public void onPause() {
	}
	public void onDestroy(){	
	}
	
	private final int kPaySuccess = 0;
	private final int kPayFail = 1;
	private final int kPayCancel = 2;

	//  计费应用信息(demo 测试)
	private static final String APPID = "300008790861";
	private static final String APPKEY = "3E7D340532BF7184FD71A1313FE63009";
	
	private int mProductNum = 1;
	
	//  计费点信息
	//add	
	private static Purchase purchase;
	private static IAPListener mListener;
	private static Activity mActivity = null;
	
	public void init(Activity activity) {
		mActivity = activity;
		IAPHandler iapHandler = new IAPHandler(mActivity);
		
		/**
		 * IAP组件初始化.包括下面3步。
		/**
		 * step1.实例化PurchaseListener。实例化传入的参数与您实现PurchaseListener接口的对象有关。
		 * 例如，此Demo代码中使用IAPListener继承PurchaseListener，其构造函数需要Context实例。
		 */
		mListener = new IAPListener(mActivity, iapHandler);
		
		/**
		 * step2.获取Purchase实例。
		 */
		purchase = Purchase.getInstance();
		/**
		 * step3.向Purhase传入应用信息。APPID，APPKEY。 需要传入参数APPID，APPKEY。 APPID，见开发者文档
		 * APPKEY，见开发者文档
		 */
		try {
			purchase.setAppInfo(APPID, APPKEY);
		} catch (Exception e1) {
			e1.printStackTrace();
		}
		/**
		 * step4. IAP组件初始化开始， 参数PurchaseListener，初始化函数需传入step1时实例化的
		 * PurchaseListener。
		 */
		try {
			purchase.init(mActivity, mListener);

		} catch (Exception e) {
			e.printStackTrace();
			return;
		}
	}
	
	public static void buy(final String productId) {		
		mActivity.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				purchase.order(mActivity, productId, mListener);
			}
		});
	}
	
	
	public class IAPListener implements OnPurchaseListener {
		private final String TAG = "IAPListener";
		private Activity context;
		private IAPHandler iapHandler;

		public IAPListener(Activity context, IAPHandler iapHandler) {
			this.context = context;
			this.iapHandler = iapHandler;
		}

		@Override
		public void onInitFinish(int code) {
			Log.d(TAG, "Init finish, status code = " + code);
			Log.d(TAG, "初始化结果：" + Purchase.getReason(code));
		}

		@Override
		public void onBillingFinish(int code, HashMap arg1) {
			Log.d(TAG, "billing finish, status code = " + code);
			String result = "订购结果：订购成功";
			// 此次订购的orderID
			String orderID = null;
			// 商品的paycode
			String paycode = null;
			// 商品的有效期(仅租赁类型商品有效)
			String leftday = null;
			// 商品的交易 ID，用户可以根据这个交易ID，查询商品是否已经交易
			String tradeID = null;
			
			String ordertype = null;
			if (code == PurchaseCode.ORDER_OK || (code == PurchaseCode.AUTH_OK) ||(code == PurchaseCode.WEAK_ORDER_OK)) {
				/**
				 * 商品购买成功或者已经购买。 此时会返回商品的paycode，orderID,以及剩余时间(租赁类型商品)
				 */
				if (arg1 != null) {
					leftday = (String) arg1.get(OnPurchaseListener.LEFTDAY);
					if (leftday != null && leftday.trim().length() != 0) {
						result = result + ",剩余时间 ： " + leftday;
					}
					orderID = (String) arg1.get(OnPurchaseListener.ORDERID);
					if (orderID != null && orderID.trim().length() != 0) {
						result = result + ",OrderID ： " + orderID;
					}
					paycode = (String) arg1.get(OnPurchaseListener.PAYCODE);
					if (paycode != null && paycode.trim().length() != 0) {
						result = result + ",Paycode:" + paycode;
					}
					tradeID = (String) arg1.get(OnPurchaseListener.TRADEID);
					if (tradeID != null && tradeID.trim().length() != 0) {
						result = result + ",tradeID:" + tradeID;
					}
					ordertype = (String) arg1.get(OnPurchaseListener.ORDERTYPE);
					if (tradeID != null && tradeID.trim().length() != 0) {
						result = result + ",ORDERTYPE:" + ordertype;
					}
				}
				final String retMsg = result;
				mActivity.runOnUiThread(new Runnable() {
					
					@Override
					public void run() {
						// TODO Auto-generated method stub
						onPayResult(kPaySuccess, retMsg);
					}
				});
			} else {
				/**
				 * 表示订购失败。
				 */
				result = "订购结果：" + Purchase.getReason(code);
				final String retMsg = result;
				mActivity.runOnUiThread(new Runnable() {
					
					@Override
					public void run() {
						// TODO Auto-generated method stub
						onPayResult(kPayFail, retMsg);
					}
				});				
			}
			System.out.println(result);
		}

		@Override
		public void onQueryFinish(int code, HashMap arg1) {
			Log.d(TAG, "license finish, status code = " + code);
			String result = "查询成功,该商品已购买";
			// 此次订购的orderID
			String orderID = null;
			// 商品的paycode
			String paycode = null;
			// 商品的有效期(仅租赁类型商品有效)
			String leftday = null;
			if (code != PurchaseCode.QUERY_OK) {
				/**
				 * 查询不到商品购买的相关信息
				 */
				result = "查询结果：" + Purchase.getReason(code);
			} else {
				/**
				 * 查询到商品的相关信息。
				 * 此时你可以获得商品的paycode，orderid，以及商品的有效期leftday（仅租赁类型商品可以返回）
				 */
				leftday = (String) arg1.get(OnPurchaseListener.LEFTDAY);
				if (leftday != null && leftday.trim().length() != 0) {
					result = result + ",剩余时间 ： " + leftday;
				}
				orderID = (String) arg1.get(OnPurchaseListener.ORDERID);
				if (orderID != null && orderID.trim().length() != 0) {
					result = result + ",OrderID ： " + orderID;
				}
				paycode = (String) arg1.get(OnPurchaseListener.PAYCODE);
				if (paycode != null && paycode.trim().length() != 0) {
					result = result + ",Paycode:" + paycode;
				}
			}
			System.out.println(result);
		}

		

		@Override
		public void onUnsubscribeFinish(int code) {
			// TODO Auto-generated method stub
			String result = "退订结果：" + Purchase.getReason(code);
			System.out.println(result);
		}

		@Override
		public void onAfterApply() {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void onAfterDownload() {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void onBeforeApply() {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void onBeforeDownload() {
			// TODO Auto-generated method stub
			
		}
	}
	
	
	public class IAPHandler extends Handler {

		private Activity context;

		public IAPHandler(Context context) {
			this.context = (Activity)context;
		}
		
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
		}
	}
	
	public native void onPayResult(int retCode, String msg);
}
