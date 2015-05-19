package org.FXPlugin;


import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;



/*
 * pref: 所有插件的管理者
 * 1. 请在主activity oncreate的时候调用初始化函数init
 * 2. 在主activity的onResume、onPause等函数调用相应接口
 */


public class FXPluginManager
{
	public static Activity _activity = null;
	public static FacebookAndroid _pluginFacebook = FacebookAndroid.getInstance();
	public static TalkingDataAndroid _pluginTalkingData = TalkingDataAndroid.getInstance();
	public static DataEyeAndroid _pluginDataEyeAndroid = DataEyeAndroid.getInstance();
	public static GooglePlay	_pluginGooglePlay = GooglePlay.getInstance();
	
	//oncreate
	public static void init(Activity context, Bundle savedInstanceState)
	{
		_activity = context;
		
		if (_pluginFacebook != null) _pluginFacebook.onCreate(savedInstanceState);
		if (_pluginTalkingData != null) _pluginTalkingData.onCreate(savedInstanceState);
		if (_pluginDataEyeAndroid != null) _pluginDataEyeAndroid.onCreate(savedInstanceState);
		if (_pluginGooglePlay != null) { _pluginGooglePlay.onCreate(savedInstanceState);}
	}
	
	

	public static void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (_pluginGooglePlay != null && requestCode == _pluginGooglePlay.RC_REQUEST) {
			_pluginGooglePlay.onActivityResult(requestCode, resultCode, data);
		}
		else {
			if (_pluginFacebook != null) _pluginFacebook.onActivityResult(requestCode,resultCode,data);
		}
	}
	public static void onResume() {
		if (_pluginFacebook != null) _pluginFacebook.onResume();
		if (_pluginTalkingData != null) _pluginTalkingData.onResume();
		if (_pluginDataEyeAndroid != null) _pluginDataEyeAndroid.onResume();
	}
	public static void onPause() {
		if (_pluginFacebook != null) _pluginFacebook.onPause();
		if (_pluginTalkingData != null) _pluginTalkingData.onPause();
		if (_pluginDataEyeAndroid != null) _pluginDataEyeAndroid.onPause();
	}
	public static void onDestroy() {
		if (_pluginFacebook != null) _pluginFacebook.onDestroy();
		if (_pluginDataEyeAndroid != null) _pluginDataEyeAndroid.onDestroy();
		if (_pluginGooglePlay != null) _pluginGooglePlay.onDestroy();
	}
	public static void onSaveInstanceState(Bundle outState) {
		if (_pluginFacebook != null) _pluginFacebook.onSaveInstanceState(outState);
	}
	
}