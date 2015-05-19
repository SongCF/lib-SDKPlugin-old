package org.FXPlugin;

import android.os.Bundle;
import com.tendcloud.tenddata.TalkingDataGA;

public class TalkingDataAndroid {

	/*//////////////////////////////////////////////////////////////////////
     * singleton
     */
    private static TalkingDataAndroid _instanceTalkingData = null;
    private TalkingDataAndroid(){}
    public static TalkingDataAndroid getInstance(){
    	if (_instanceTalkingData == null){
    		_instanceTalkingData = new TalkingDataAndroid();
    	}
    	return _instanceTalkingData;
    }
    public static boolean isEnable(){
    	return _instanceTalkingData == null ? false : true;
    }
    
    

    public void onCreate(Bundle savedInstanceState) {
    	TalkingDataGA.init(FXPluginManager._activity, "1DFE37804B7D5F91172B54E56E5FBAAA", "000001");
	}
    public void onResume() {
    	TalkingDataGA.onResume(FXPluginManager._activity);
	}
	public void onPause() {
		TalkingDataGA.onPause(FXPluginManager._activity);
	}
}
