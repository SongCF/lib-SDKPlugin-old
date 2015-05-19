/*
 * 插件管理类（统一管理各种插件）
 */


#pragma once

class IOSWrapper
{
private:
    ~IOSWrapper(){}
    IOSWrapper(){}
public:
    
    static void openUrl(const char* Url);
};
