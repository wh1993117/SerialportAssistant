/*******************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: SerialportPanel.h
Version: V0.0.0/LittleBoy/2017-09-22
Description: SerialportPanel类的声明
Others: 注意 DISABLE_COMBOBOX_AND_REFRESH_BUTTON_WHEN_SERIALPORT_IS_OPEN 的使用
History:
    01. LittleBoy/2017-09-22~2017-09-27
        创建本文档，完善本文档。

*******************************************************************************/

#ifndef __SERIALPOET_H
#define __SERIALPORT_H

#include <QSerialPort>
#include <QGroupBox>
#include <QComboBox>
#include <QTextBrowser>
#include <QPushButton>


/*
 * 该宏如果被定义，在打开串口时，本控件的所有预选框和刷新按钮将被设定为禁止编辑状态，该宏可以通过以下条件
 * 编译来控制它是否被定义。
 * 为什么要定义这个宏？因为目前为止，我不知道那种方式比较好，两种方式都预留，以后更改起来方便些。
 * 上述的两种方式指的是：
 *  1. 打开串口时，本控件所有预选框能编辑，用于刷新串口设备的按钮能点击。
 *  2. 打开串口时，本控件所有预选框不能编辑，用于刷新串口设备的按钮不能点击。
 *
 * 说一些与这个工程代码无关的：到中山这边工作差不多半个月了，压力好大，一周6天班，天天要加班，好累。。。
 * 2017-09-27
 */
#if 1
#define DISABLE_COMBOBOX_AND_REFRESH_BUTTON_WHEN_SERIALPORT_IS_OPEN
#endif



/*
 * 类的前置声明与定义
 */
class SerialportPanel;
class SerialportPanel : public QGroupBox
{
    Q_OBJECT
public:
    SerialportPanel(QSerialPort *serialport, QWidget *parent = Q_NULLPTR);
    ~ SerialportPanel();
    QSerialPort *GetSerialport();       //获取本控件的串口类指针
private:
    QSerialPort *m_pSerialport = NULL;  //貌似老旧的编译器不支持类数据成员的类内初始化，如果报错，不初始化就行了...
    QComboBox *m_pSerialportComboBox;   //串口设备预选框
    QTextBrowser *m_pTextBrowser;       //操作信息提示/显示框
    QComboBox *m_pBaudRateComboBox;     //波特率预选框
    QPushButton *m_pRefreshPBt;         //刷新设备
    QPushButton *m_pOpenSerialportPBt;  //打开关闭按钮
    QComboBox *m_pDataBitsComboBox;     //数据位预选框
    QComboBox *m_pStopBitsComboBox;     //停止位预选框
    QComboBox *m_pParityComboBox;       //奇偶校验位预选框
private slots:
    void RefreshDevice();               //刷新串口设备
    void OpenCloseSerialport();         //打开或关闭串口
signals:
    void SerialportHadOpened();         //打开串口时，发射该信号
    void SerialportHadClosed();         //关闭串口时，发射该信号
};


#endif
