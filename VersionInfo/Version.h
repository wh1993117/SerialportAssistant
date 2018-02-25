/*******************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: Version.h
Version: V0.0.0/LittleBoy/2017-11-23
Description: 软件版本信息
Others: not yet
Qt Version: Qt 5.9.3 MinGW
History:
    01. LittleBoy/2017-11-23
        创建本文档

*******************************************************************************/
#ifndef __VERSION_H
#define __VERSION_H

#include <QDate>
#include <QTime>

/*
 * 以下两行分别获取编译时间和编译日期
 */
static const QTime g_BuildTime = QTime::fromString( __TIME__, "hh:mm:ss" );
static const QDate g_BuildDate = QLocale( QLocale::English ).toDate( QString( __DATE__ ).replace( "  ", " 0" ), "MMM dd yyyy");


#define VERSION "V1.0.0"
/*
 * 实现所有预设功能 2017/12/06
 */

#endif
