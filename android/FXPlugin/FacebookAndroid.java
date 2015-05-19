package org.FXPlugin;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import org.json.JSONException;
import org.json.JSONObject;

import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.widget.Toast;

import com.camellia.starfall.star;
import com.facebook.AppEventsLogger;
import com.facebook.FacebookAuthorizationException;
import com.facebook.FacebookOperationCanceledException;
import com.facebook.FacebookException;
import com.facebook.Request;
import com.facebook.Request.GraphUserCallback;
import com.facebook.Response;
import com.facebook.Session;
import com.facebook.SessionState;
import com.facebook.UiLifecycleHelper;
import com.facebook.model.GraphUser;
import com.facebook.widget.FacebookDialog;
import com.facebook.widget.WebDialog;
import com.facebook.widget.FacebookDialog.ShareDialogBuilder;
import com.facebook.widget.WebDialog.OnCompleteListener;


/*
 * pref: facebook Android sdk， 版本号 3.22.0
 * 包含内容： 登录，获取好友列表，分享
 * 1. 使用时须在manifest的application中配置Facebook appId  和    Facebook的登录activity
 * 
        <activity android:name="com.facebook.LoginActivity"
                  android:theme="@android:style/Theme.Translucent.NoTitleBar"
                  android:label="@string/app_name" />
    	<meta-data android:name="com.facebook.sdk.ApplicationId" android:value="@string/facebook_app_id"/>
 *
 * 2. 须将FacebookSDK(3.22.0)作为library加入自己的项目     
 *      --------    右键项目属性  -> Android -> Add library -> select FacebookSDK
 *      
 */


public class FacebookAndroid
{

    /*//////////////////////////////////////////////////////////////////////
     * singleton
     */
    private static FacebookAndroid _instancePluginFacebook = null;
    private FacebookAndroid(){}
    public static FacebookAndroid getInstance(){
    	if (_instancePluginFacebook == null){
    		_instancePluginFacebook = new FacebookAndroid();
    	}
    	return _instancePluginFacebook;
    }
    public static boolean isEnable(){
    	return _instancePluginFacebook == null ? false : true;
    }
    
    
	private static final String TAG = "Facebook";
	private static final int iSUCCESS = 0;
	private static final int iCANCLE = 1;
	private static final int iERROR = 2;
	
	private static final String ID = "ID";
	private static final String NAME = "name";
	private static final String PICTURE = "picture";
	
	//public static String UserID = "";
	public static HashMap<String, String> UserInfoMap = new HashMap<String, String>();
	private static final List<String> PERMISSIONS = new ArrayList<String>() {
        {
            add("user_friends");
            add("public_profile");
        }
    };
    
    public UiLifecycleHelper uiHelper;
    public Session.StatusCallback loginCallback = new Session.StatusCallback() {
	    @Override
	    public void call(Session session, SessionState state, Exception exception) {
	        onSessionStateChange(session, state, exception);
	    }
	};

    private FacebookDialog.Callback dialogCallback = new FacebookDialog.Callback() {
        @Override
        public void onError(FacebookDialog.PendingCall pendingCall, Exception error, Bundle data) {
            Log.e("Activity", String.format("Error: %s", error.toString()));
            shareCallback(iERROR, error.toString());
        }

        @Override
        public void onComplete(FacebookDialog.PendingCall pendingCall, Bundle data) {
            Log.i("Activity", "Success!");
            String completionGesture = FacebookDialog.getNativeDialogCompletionGesture(data);
            if (completionGesture.equals("post")) {
            	shareCallback(iSUCCESS, "");
			}
            else {
                shareCallback(iCANCLE, "User clicked the Cancel button");
			}
        }
    };
    
    
    public void onCreate(Bundle savedInstanceState) {    	
	    uiHelper = new UiLifecycleHelper(FXPluginManager._activity, loginCallback);
	    uiHelper.onCreate(savedInstanceState);
	}
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		uiHelper.onActivityResult(requestCode, resultCode, data, dialogCallback);
	}
    public void onResume() {
//    	// For scenarios where the main activity is launched and user
//	    // session is not null, the session state change notification
//	    // may not be triggered. Trigger it if it's open/closed.
//	    Session session = Session.getActiveSession();
//	    if (session != null &&
//	           (session.isOpened() || session.isClosed()) ) {
//	        onSessionStateChange(session, session.getState(), null);
//	    }
	    uiHelper.onResume();
	    //facebook : Logs 'install' and 'app activate' App Events.
	    AppEventsLogger.activateApp(FXPluginManager._activity);
	}
	public void onPause() {
		uiHelper.onPause();
	    //facebook : Logs 'app deactivate' App Event.
	    AppEventsLogger.deactivateApp(FXPluginManager._activity);
	}
	public void onDestroy() {
		uiHelper.onDestroy();
	}
	public void onSaveInstanceState(Bundle outState) {
		uiHelper.onSaveInstanceState(outState);
	}
	
	
	private void onSessionStateChange(Session session, SessionState state, Exception exception) {
	    if (state.isOpened()) {
	        Log.i(TAG, "onSessionStateChange, Logged in...");
	    } else if (state.isClosed()) {
	        Log.i(TAG, "onSessionStateChange, Logged out...");
	    }
	    
	    if (exception == null){
	    	if (state.isOpened()){
	    		//如果登录了，那么等获取到userid后再回调
	    		//loginCallback(iSUCCESS, "");
		        Log.i(TAG, "logged, will init user info");
	    		initUserIdAndCallback();
	    	}
	    }else if (exception instanceof FacebookOperationCanceledException){
	        Log.i(TAG, "log fail, FacebookOperationCanceledException");
	    	loginCallback(iERROR, "login cancle");
		}else if (exception instanceof FacebookAuthorizationException){
	        Log.i(TAG, "log fail, FacebookAuthorizationException");
			loginCallback(iERROR, "login Authorization error");
		}
	}
	
	
	
	// access_token
	public static String getAccessToken() {
		if (Session.getActiveSession() != null)
			return Session.getActiveSession().getAccessToken();
		else 
			return null;
	}
	
	
	// login
	public static void login()
	{
		if (! FacebookAndroid.isEnable()) return;
		
		FXPluginManager._activity.runOnUiThread( new Runnable () {
			public void run () {
				if (Session.getActiveSession() == null || !Session.getActiveSession().isOpened()) {
					Session.openActiveSession(FXPluginManager._activity, true, PERMISSIONS, FacebookAndroid.getInstance().loginCallback);
				}
			}
		});
	}
	// logout
	public static void logout()
	{
		if (! FacebookAndroid.isEnable()) return;
		
		FXPluginManager._activity.runOnUiThread( new Runnable () {
			public void run () {
				if (Session.getActiveSession() != null) {
					Session.getActiveSession().closeAndClearTokenInformation();
				}
			}
		});
	}
	public static native void loginCallback(int retCode, String msg);
	//
	public static boolean isLogin()
	{
		// 1. facebook实例存在     2. 且当前会话存在     3. 且会话是打开状态     ---》 那么表示登录了
		if (FXPluginManager._pluginFacebook != null && Session.getActiveSession() != null && Session.getActiveSession().isOpened()){
			return true;
		}
		else {
			return false;
		}
	}
	public static String getUserID()
	{
		return getUserInfo()[0];
	}
	public static String[] getUserInfo()
	{
		String idString = UserInfoMap.get(ID);
		if (isLogin() && (idString == null || idString == "")){
			loadUserInfoByCache();
			idString = UserInfoMap.get(ID);
		}
		
		if (isLogin() && idString != ""){
			String[] vec = {UserInfoMap.get(ID), UserInfoMap.get(NAME), UserInfoMap.get(PICTURE)};
			return vec;
		}
		else {
			return new String[]{"", "", ""};
		}
	}
	private static void cacheUserInfo(GraphUser user)
	{
		String pathString = "/data/data/" + star.getContext().getPackageName()
				+ "/files/" + "Facebook_UserInfor";
		try {
			FileWriter fw = new FileWriter(pathString);			
			fw.write(UserInfoMap.get(ID));
			fw.write('\n');
			fw.write(UserInfoMap.get(NAME));
			fw.write('\n');
			fw.write(UserInfoMap.get(PICTURE));		
			fw.flush();
			fw.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	private static void loadUserInfoByCache()
	{
		String pathString = "/data/data/" + star.getContext().getPackageName()
				+ "/files/" + "Facebook_UserInfor";
		try {
			BufferedReader br = new BufferedReader(new FileReader(pathString));
			UserInfoMap.put(ID, br.readLine());//一次读入一行，直到读入null为文件结束  
			UserInfoMap.put(NAME, br.readLine());
			UserInfoMap.put(PICTURE, br.readLine());
			br.close();			
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}  
	}
	
	// get user id
	private boolean m_loginAlreadyCallback;
	public void initUserIdAndCallback()
	{
		if (isLogin()){
			m_loginAlreadyCallback = false;
			//超时处理
			Timer timeoutTimer = new Timer();          
			timeoutTimer.schedule(new TimerTask() {              
			            @Override  
			            public void run() {   
			            	if (m_loginAlreadyCallback) return;
			            	m_loginAlreadyCallback = true;
			            	loginCallback(iERROR, "get user info time out");
			            }   
			        }, 20000);
			
			Request request = Request.newMeRequest(Session.getActiveSession(), new GraphUserCallback(){
				@Override
				public void onCompleted(GraphUser user, Response response) {
					
					//超时处理
					if (m_loginAlreadyCallback) return;
					m_loginAlreadyCallback = true;
					
					if (user != null && response.getError() == null){
						UserInfoMap.put(ID, user.getId());
						UserInfoMap.put(NAME, user.getName());
						try {
							JSONObject jsonData = user.getInnerJSONObject();
							UserInfoMap.put(PICTURE, jsonData.getJSONObject("picture").getJSONObject("data").getString("url"));
						} catch (JSONException e) {
							e.printStackTrace();
						}
						cacheUserInfo(user);
						loginCallback(iSUCCESS, "");
					}else {
						loginCallback(iERROR, "get user infor failed");
					}
				}
			});
        	HashSet<String> fields = new HashSet<String>();
            fields.addAll(Arrays.asList("id", "name", "picture"));
            Bundle parameters = request.getParameters();
            parameters.putString("fields", TextUtils.join(",", fields));
            request.setParameters(parameters);
            request.executeAsync();
		}
	}
	
	// c++ use graph api(based http)
/*	// get friends
	public static void getFriendList()
	{
		if (PluginFacebook.getInstance() == null) return;
		
		FXPluginManager._activity.runOnUiThread( new Runnable () {
			public void run () {
				Session session = Session.getActiveSession();
		        boolean enableButtons = (session != null && session.isOpened());
		        if (enableButtons) {
		        	Request request = Request.newMyFriendsRequest(session, new Request.GraphUserListCallback() {				
						@Override
						public void onCompleted(List<GraphUser> users, Response response) {
							Log.d(TAG, "Request Completed!! " + response.toString() + "---- size:" + users.size());
					         if (users != null && response.getError() == null) {
					             HashMap friendsList = new HashMap();
					             for (GraphUser user : users) {
					                friendsList.put(user.getId(), user.getName());
					             }
					         }
						}
					});
		        	HashSet<String> fields = new HashSet<String>();
		            fields.addAll(Arrays.asList("id", "name", "picture"));
		            Bundle parameters = request.getParameters();
		            parameters.putString("fields", TextUtils.join(",", fields));
		            request.setParameters(parameters);
		            
		        	request.executeAsync();
				}
			}
		});
	}
	public static native void getFriendsCallback();
	*/
	
	//share
	public static void share(String[] key, String[] value)
	{
		if (! FacebookAndroid.isEnable()) return;
		
		assert(key.length == value.length);
		HashMap<String, String> infoHashMap = new HashMap<String, String>();
		for (int i = 0; i < key.length; i++) {
			infoHashMap.put(key[i], value[i]);
		}
		final HashMap<String, String> shareDataHashMap = infoHashMap;
		
		FXPluginManager._activity.runOnUiThread( new Runnable () {
			public void run () {
				if (FacebookDialog.canPresentShareDialog(FXPluginManager._activity.getApplicationContext(), 
                        FacebookDialog.ShareDialogFeature.SHARE_DIALOG)) {
					// Publish the post using the Share Dialog
					ShareDialogBuilder dialogBuilder = new FacebookDialog.ShareDialogBuilder(FXPluginManager._activity);
					if (shareDataHashMap.get("link") != null)
					{
						dialogBuilder.setLink(shareDataHashMap.get("link"));
						if (shareDataHashMap.get("name") != null)dialogBuilder.setName(shareDataHashMap.get("name"));
						if (shareDataHashMap.get("caption") != null)dialogBuilder.setCaption(shareDataHashMap.get("caption"));
						if (shareDataHashMap.get("description") != null)dialogBuilder.setDescription(shareDataHashMap.get("description"));
						if (shareDataHashMap.get("picture") != null)dialogBuilder.setPicture(shareDataHashMap.get("picture"));
					}
					FacebookDialog shareDialog = dialogBuilder.build();
					FacebookAndroid.getInstance().uiHelper.trackPendingDialogCall(shareDialog.present());

				} else {
					// Fallback. For example, publish the post using the Feed Dialog
					Bundle params = new Bundle();
					if (shareDataHashMap.get("link") != null)
					{
						params.putString("link", shareDataHashMap.get("link"));
						if (shareDataHashMap.get("name") != null) params.putString("name", shareDataHashMap.get("name"));
						if (shareDataHashMap.get("caption") != null) params.putString("caption", shareDataHashMap.get("caption"));
						if (shareDataHashMap.get("description") != null) params.putString("description", shareDataHashMap.get("description"));
						if (shareDataHashMap.get("picture") != null) params.putString("picture", shareDataHashMap.get("picture"));
					}

				    WebDialog feedDialog = (
				        new WebDialog.FeedDialogBuilder(FXPluginManager._activity,
				            Session.getActiveSession(),
				            params))
				        .setOnCompleteListener(new OnCompleteListener() {

				            @Override
				            public void onComplete(Bundle values,
				                FacebookException error) {
				                if (error == null) {
				                    // When the story is posted, echo the success
				                    // and the post Id.
				                    final String postId = values.getString("post_id");
				                    if (postId != null) {
				                        shareCallback(iSUCCESS, "");
				                    } else {
				                        // User clicked the Cancel button
				                        shareCallback(iCANCLE, "User clicked the Cancel button");
				                    }
				                } else if (error instanceof FacebookOperationCanceledException) {
				                    // User clicked the "x" button
			                        shareCallback(iCANCLE, "User clicked the 'x' button");
				                } else {
				                    // Generic, ex: network error
			                        shareCallback(iERROR, "Generic, ex: network error");
				                }
				            }
				        })
				        .build();
				    feedDialog.show();
				}
			}
		});
	}
	public static native void shareCallback(int retCode, String msg);
	
	
	//invite friends
	public static void inviteFriends(String inviteMsg)
	{
		//friends get notifications
		//https://developers.facebook.com/docs/android/send-requests#notifications
		
		final String msgString = inviteMsg;
		
		FXPluginManager._activity.runOnUiThread(new Runnable() {
			public void run() {
				Bundle params = new Bundle();
				params.putString("message", msgString);
				WebDialog requestsDialog = (new WebDialog.RequestsDialogBuilder(
						FXPluginManager._activity, Session.getActiveSession(),
						params)).setOnCompleteListener(
						new OnCompleteListener() {

							@Override
							public void onComplete(Bundle values,
									FacebookException error) {
								if (error != null) {
									if (error instanceof FacebookOperationCanceledException) {
				                        inviteFriendsCallback(iCANCLE, "Request cancelled");
									} else {
				                        inviteFriendsCallback(iERROR, "Network Error");
									}
								} else {
									final String requestId = values
											.getString("request");
									if (requestId != null) {
				                        inviteFriendsCallback(iSUCCESS, "");
									} else {
				                        inviteFriendsCallback(iCANCLE, "Request cancelled");
									}
								}
							}

						}).build();
				requestsDialog.show();
			}
		});
	}
	public static native void inviteFriendsCallback(int retCode, String msg);
    
	
	
	
	// not support
	// warning: not use facebook analytics
	public static void logEvent(String msgId, String[] paramsKey, String[] paramsValue)
	{
	    assert(paramsKey.length == paramsValue.length);
	    Bundle parameters = new Bundle();
		for (int i = 0; i < paramsKey.length; i++) {
			parameters.putString(paramsKey[i], paramsValue[i]);
		}
		final Bundle params = parameters;
		final String eventIdString = msgId;
		FXPluginManager._activity.runOnUiThread( new Runnable () {
			public void run () {
			    AppEventsLogger logger = FacebookAndroid.getInstance().uiHelper.getAppEventsLogger();
			    if (logger == null) return;
			    logger.logEvent(eventIdString, params);
			}
		});
	}
	
	
	
	
}