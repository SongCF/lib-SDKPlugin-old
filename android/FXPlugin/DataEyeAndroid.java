package org.FXPlugin;

import com.dataeye.DCAgent;
import com.dataeye.DCReportMode;

import android.os.Bundle;

/*
 * (数据上报模式)
 * 通过调用 DCAgent.setReportMode()来设置,DataEye 的数据上报模式
 *   DC_DEFAULT  :   数 据 在 SDK 初 始 化 成 功 后 按 固 定 周 期 上 报 .  设置为
DCReportMode.DC_DEFAULT 上报模式的游戏,无需调用 DCAccount.login 方法,相关
用户将使用 DataEyeSDK 生成的 Uid 作为用户编号。
  DC_AFTER_LOGIN :  数据在调用 DCAccount.login(const char*  accountId)方法之
后,DataeyeSDK 才会按固定周期上报. 用户将使用 DCAccount.login 方法中设置的
AccountID 作为用户编号。 后台所有用户数据都与设置的 AccountID 关联。
注意事项：
1.  如果不调用此接口,则默认采用 DC_DEFAULT 模式上报数据
2.  DCAgent.setReprotMode() 接口 需要在入口 Activity 的 OnCreate 方法中调用. 

 */




public class DataEyeAndroid {

	/*//////////////////////////////////////////////////////////////////////
     * singleton
     */
    private static DataEyeAndroid _instanceDataEyeAndroid = null;
    private DataEyeAndroid(){}
    public static DataEyeAndroid getInstance(){
    	if (_instanceDataEyeAndroid == null){
    		_instanceDataEyeAndroid = new DataEyeAndroid();
    	}
    	return _instanceDataEyeAndroid;
    }
    public static boolean isEnable(){
    	return _instanceDataEyeAndroid == null ? false : true;
    }
    
    

    public void onCreate(Bundle savedInstanceState) {
    	DCAgent.setDebugMode(true);
    	DCAgent.setReportMode(DCReportMode.DC_DEFAULT);//DCReportMode.DC_AFTER_LOGIN
    	DCAgent.initConfig(FXPluginManager._activity, "B302EBC1848676982DB670A33A4751D4", "000001");
	}
    public void onResume() {
    	DCAgent.onResume(FXPluginManager._activity);
	}
	public void onPause() {
		DCAgent.onPause(FXPluginManager._activity);
	}
	public void onDestroy() {
		DCAgent.onKillProcessOrExit();
	}
}
