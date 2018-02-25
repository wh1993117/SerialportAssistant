/*******************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: Global.h
Version: V0.0.0/LittleBoy/2017-12-08
Description:  声明本软件所有全局变量
Others: not yet
History:
    01. LittleBoy/2017-12-08
        创建本文档

*******************************************************************************/
#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <QSerialPort>

extern QSerialPort *g_SerialPort;   // 该全局变量在Global.cpp中被定义


void GlobaParameterInit();          // 该函数将初始化本软件项目的所有全局函数
#endif // GLOBAL_H
