package org.FXPlugin;

import java.util.List;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import com.example.android.trivialdrivesample.util.IabHelper;
import com.example.android.trivialdrivesample.util.IabResult;
import com.example.android.trivialdrivesample.util.Inventory;
import com.example.android.trivialdrivesample.util.Purchase;

public class GooglePlay {
	
	// (arbitrary) request code for the purchase flow
    final static int RC_REQUEST = 2342341;
	private static final int iSUCCESS = 0;
	private static final int iCANCLE = 1;
	private static final int iERROR = 2;
	
	private static String m_CurPayProductID = "";
	
	private static GooglePlay _instanceGooglePlay = null;
	public static GooglePlay getInstance(){
    	if (_instanceGooglePlay == null){
    		_instanceGooglePlay = new GooglePlay();
    	}
    	return _instanceGooglePlay;
    }
	
	private static String TAG = "GooglePlay";
	private static IabHelper mHelper;
	
	String base64EncodedPublicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAlJ5EPA3IG4rim+"
			+ "n6mbyZddJTd4RP+DPys6WOaL/NGDnlbp5vmQ1EqyXpgBQnNEHPpZQy3Ois4IfCtUym5ILNDqdZ2HgU1mP"
			+ "gZ015G37u8mYdaY1xglmGaQkOla0PvQLguwE85+cIU7w5i3EPmG4hHf9mFD3aUNWa3949l32n6YS35hE8"
			+ "0psqfI/kG8ZjxtjwSBWbSb1+o/oBxryLHYa7UU4W9OgAqVAtUOx7TsIVQFFhWVoEgTtIrguSdNkYkygO"
			+ "Iv5uOx6idapiCDQ1ZkIDKfZheSfPENUYlAvSw/ICGLkgq/aq227evOSYdEV/KxfcI+0B0JRyX8GeJVDOx"
			+ "2nvxQIDAQAB";
	
	public void onCreate(Bundle savedInstanceState) {
		// compute your public key and store it in base64EncodedPublicKey
		   mHelper = new IabHelper(FXPluginManager._activity, base64EncodedPublicKey);
		   mHelper.enableDebugLogging(true);
		   
		   try 
		   {
			   mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
				   public void onIabSetupFinished(IabResult result) {
				      if (!result.isSuccess()) {
				         // Oh noes, there was a problem.
			         Log.d(TAG, "Problem setting up In-app Billing: " + result);
			         return;
			      }
				   // Have we been disposed of in the meantime? If so, quit.
	               if (mHelper == null) return;

	               // IAB is fully set up. Now, let's get an inventory of stuff we own.
	               Log.d(TAG, "Setup successful. Querying inventory.");
	               mHelper.queryInventoryAsync(mGotInventoryListener);
			   }
			});
		} catch (Exception e) {
			// TODO: handle exception
		}
	}
	
	// Listener that's called when we finish querying the items and subscriptions we own
	IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
	   public void onQueryInventoryFinished(IabResult result, Inventory inventory)   
	   {
		   Log.d(TAG, "Query inventory finished.");

           // Have we been disposed of in the meantime? If so, quit.
           if (mHelper == null) return;

           // Is it a failure?
           if (result.isFailure()) {
        	   Log.d(TAG, "Failed to query inventory: " + result.toString());
               return;
           }
           
         //每次查询的时候,如果查到有没消耗的商品,那么就消耗掉,不然会导致购买失败
         //(V3版本的不受管理的商品默认是只能购买一次的.所以必须在每次购买之后消耗掉,不然会导致第二次购买失败)
           if (inventory != null) {
        	   List<Purchase> list = inventory.getAllPurchases();
        	   for (int i = 0; i < list.size(); i++) {
        		   if (list.get(i) == null) {
					continue;
				}
				mHelper.consumeAsync(list.get(i), mConsumeFinishedListener);
			}
		}
           

           Log.d(TAG, "Query inventory was successful.");
           
          Log.d(TAG, "查询完成; 接下来可以操作UI线程了..");
	   }
	};
	
    /** Verifies the developer payload of a purchase. */
    boolean verifyDeveloperPayload(Purchase p) {
        String payload = p.getDeveloperPayload();
        Log.d(TAG,  payload);
        /*
         * TODO: verify that the developer payload of the purchase is correct. It will be
         * the same one that you sent when initiating the purchase.
         *
         * WARNING: Locally generating a random string when starting a purchase and
         * verifying it here might seem like a good approach, but this will fail in the
         * case where the user purchases an item on one device and then uses your app on
         * a different device, because on the other device you will not have access to the
         * random string you originally generated.
         *
         * So a good developer payload has these characteristics:
         *
         * 1. If two different users purchase an item, the payload is different between them,
         *    so that one user's purchase can't be replayed to another user.
         *
         * 2. The payload must be such that you can verify it even when the app wasn't the
         *    one who initiated the purchase flow (so that items purchased by the user on
         *    one device work on other devices owned by the user).
         *
         * Using your own server to store and verify developer payloads across app
         * installations is recommended.
         */

        return true;
    }
    
    public static void buy(String skugasString) {

        String payload = "";
        m_CurPayProductID = skugasString;
 //       m_CurPayProductID = "android.test.purchased";
        try {

            mHelper.launchPurchaseFlow(FXPluginManager._activity, m_CurPayProductID, RC_REQUEST,
                    mPurchaseFinishedListener, payload);
		} catch (Exception e) {
			// TODO: handle exception
			payCallBack(iERROR, "");
		}
	}
    
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.d(TAG, "onActivityResult(" + requestCode + "," + resultCode + "," + data);
        if (mHelper == null) return;

        // Pass on the activity result to the helper for handling
        if (!mHelper.handleActivityResult(requestCode, resultCode, data)) {
            // not handled, so handle it ourselves (here's where you'd
            // perform any handling of activity results not related to in-app
            // billing...
        	Log.d(TAG, "onActivityResult not handled by IABUtil.");
        }
        else {
            Log.d(TAG, "onActivityResult handled by IABUtil.");
        }
    }

	/* Callback for when a purchase isfinished
     * 购买成功处理
     */
	static IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
	   public void onIabPurchaseFinished(IabResult result, Purchase purchase) 
	   {
	      //购买成功后需要执行消耗操作
	      //(V3版本的不受管理的商品默认是只能购买一次的.所以必须在每次购买之后消耗掉,不然会导致第二次购买失败)
//		  if (purchase.getSku().equals(m_CurPayProductID)) 
	      if (purchase != null)
	      {
              mHelper.consumeAsync(purchase, mConsumeFinishedListener);
	      }
		   
	      if (result.isFailure()) {
	         Log.d(TAG, "Error purchasing: " + result);
	         payCallBack(iCANCLE, result.getMessage());
	         return;
	      }      
	      payCallBack(iSUCCESS, "");
	   }
	};
	
	/* Called when consumption is complete
     * 如果是消耗品的话 需要调用消耗方法
     */
	static IabHelper.OnConsumeFinishedListener mConsumeFinishedListener =
	   new IabHelper.OnConsumeFinishedListener() {
	   public void onConsumeFinished(Purchase purchase, IabResult result) {
		   Log.d(TAG, "消耗操作完成.产品为:" + purchase + ", 结果: " + result);
		   // if we were disposed of in the meantime, quit.
		   if (mHelper == null)return;
		   // 如果有多个消耗产品，应该在这里一一检查。这里只有一个消耗产品，所以不检查
		   
		   if (result.isSuccess()) {
			// 消耗成功后，填 写我们的逻辑。
		   }
		   else {
	         // handle error
		   }
	   	}
	};
	
	public void onDestroy() {
	   if (mHelper != null) mHelper.dispose();
	   mHelper = null;
	} 
	
	public static native void payCallBack(int errcode, String msgString);
}
