package org.FXUtils;

import org.FXPlugin.FXPluginManager;
import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Intent;
import android.net.Uri;

public final class JniWrapper {
	
	public enum LanguageType
	{
	    kLanguageEnglish(0),
	    kLanguageChinese(1),
	    kLanguageFrench(2),
	    kLanguageItalian(3),
	    kLanguageGerman(4),
	    kLanguageSpanish(5),
	    kLanguageDutch(6),
	    kLanguageRussian(7),
	    kLanguageKorean(8),
	    kLanguageJapanese(9),
	    kLanguageHungarian(10),
	    kLanguagePortuguese(11),
	    kLanguageArabic(12);
	    
	    private int _value;
	    
	    private LanguageType(int value)
	    {
	    	_value = value;
	    }

	    public int value()
	    {
	    	return _value;
	    }
	}	

	public static int getCurSettingLanguageType()
	{
		return getCurSettingLanguageTypeNative();
	}
			
	public static native int getCurSettingLanguageTypeNative();
	
	public static void openUrl(final String urlText) 
	{
		// TODO Auto-generated method stub
		Intent intent = new Intent(Intent.ACTION_VIEW);
		intent.setData(Uri.parse(urlText));
		FXPluginManager._activity.startActivity(intent);
	}

}