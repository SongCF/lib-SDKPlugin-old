#ifndef __DATAEYE_REPORTMODE_H__
#define __DATAEYE_REPORTMODE_H__

typedef enum _ReportMode
{
    DC_DEFAULT = 1,      //默认模式上传，即无需登录定时上传
    DC_AFTER_LOGIN = 2   //数据登录后上传
}DCReportMode;

#endif
