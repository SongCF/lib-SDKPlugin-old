
#pragma once

#include <map>

namespace FX{


enum class ShareResultCode
{
	eSuccess = 0,
	eCancle,
	eError,
};




class ShareResultListener
{
public:
    virtual void onShareResult(ShareResultCode ret, const char* msg) = 0;
};




class PluginShare
{
public:

    /**
    @breif set the result listener
    @param pListener The callback object for share result
    @wraning Must invoke this interface before share
    */
    virtual void setShareListener(ShareResultListener* pListener) = 0;

	/**
    @brief share information
    @param info The info of share, contains key:
			link : 分享链接 （部分sdk在link存在时，才有下面几个字段，如：Facebook）
			name : 标题
			caption : 副标题
			description : 描述
			pictureURL : 分享的网络图片
			pictureImg : 本地图片，如果本地图片为找到就会使用 url图片代替
    @warning For different plugin, the parameter should have other keys to share.
             Look at the manual of plugins.
    */
    virtual void share(std::map<std::string, std::string>* info) = 0;

};


}