/*******************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: Global.cpp
Version: V0.0.0/LittleBoy/2017-12-11
Description:  初始化本软件所有全局函变量
Others: not yet
Functions:  01. GlobaParameterInit();   // 初始化所有全局函数
History:
    01. LittleBoy/2017-12-11
        创建本文档

*******************************************************************************/
#include <QSerialPort>

#include "global.h"

QSerialPort *g_SerialPort;  // 指向一个串口类，该类在main函数中初始化（调用GlobaParameterInit()函数）


/*******************************************************************************
Number：01
Function: GlobaParameterInit()
Description: 初始化所有全局函数
Calls:
Called By: main()
Return: void
Others: not yet
History：
    01. LittleBoy/2017-12-11 创建并部分实现函数
*******************************************************************************/
void GlobaParameterInit()
{
    g_SerialPort = new QSerialPort;
}
