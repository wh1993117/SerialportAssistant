/*******************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: SendArea.h
Version: V0.0.0/LittleBoy/2017-10-08
Description: 串口发送数据编辑控件
Others: not yet
History:
    01. LittleBoy/2017-10-08~2017-10-30
        创建本文档，完善本文档。

*******************************************************************************/

#ifndef __SEND_AREA_H
#define __SEND_AREA_H

/*
 * 发射信号参数
 * MODE_CHAR：只发送一个字节
 * MODE_BYTE_ARRAY：发送多个字节（以一个QByteArray发送）
 */
#define MODE_CHAR 0
#define MODE_BYTE_ARRAY 1

#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <Qlabel>
#include <QSerialPort>
#include <Qtimer>

#include "HelpDialog.h"

#define MINI_DELAY_TIME 1              // 循环发送时，每帧数据之间的时间间隔必须大于等于该宏所定义得值

class SendArea;
class SendArea : public QGroupBox
{
    Q_OBJECT
public:
    SendArea(QSerialPort *serialport, QWidget *parent = 0);
    ~SendArea();
    void SetText(QString str);                     // 设置输入框内容
private:
    bool m_bSendByFrame;                // "帧发送"为真，否则为假
    bool m_bNonRepeating;               // 不循环发送时为真，否则为假
    int m_index;                        // 字节发送时的数据数组下标

    QSerialPort *m_pSerialPort;         // 使用该控件，必须初始化该指针(构造函数中指向一个实际有效的串口类)

    QCheckBox *m_pASCIICBox;            // "ASCII字符" 表示本控件文本编辑框的数据位ASCII字符
    QCheckBox *m_pHexCBox;              // "16进制数" 表示本控件文本编辑框中的数据位16进制数
    QCheckBox *m_pSendFrameCBox;        // “帧　发送” 本控件的数据编辑框中的所有数据组成一个数据帧
    QCheckBox *m_pSendByteCBox;         // "字节发送" 本控件的数据编辑框中的每一个数据（每一个ASCII或
                                        // 每一个十六进制数）组成一帧数据，然后分别发送每帧数据
    QCheckBox *m_pSendCircularyCBox;    // "循环发送" 表示本控件文本编辑框中的所有数据将被循环发送
    QCheckBox *m_pSendDealyCBox;        // "延时发送" 表示每两帧数据发送之间的延时
    QLabel *m_pDelayTimeLab;            // "延时时间" 每两帧数据之间的延时时间（Label）
    QLineEdit *m_pDelayTimeLEdit;       // "延时时间" 输入框

    QTextEdit *m_pDataTEdit;            // 本控件的数据编辑框
    QPushButton *m_pSendPBt;            // "发送数据" 通过串口发送数据
    QPushButton *m_pClearPBt;           // “清空” 清空文本编辑框中的输入
    QPushButton *m_pUsagePbt;           // “说明” 本控件的使用说明

    HelpDialog *helpDl;                 // "说明"按钮点击时的弹窗

    QPushButton *m_pOpenFilePBt;        // "打开文件" 2017-11-12新增
    QTimer *m_pSendDataTimer;           // 该定时器用于循环发送数据
    QByteArray mDataArray;              // 将要通过串口发送的数据
    QByteArray *m_pDataArray;           // m_pDataArray 指向 mDataArray
private slots:
    void ASCIICBoxClicked();            // "ASCII字符" 复选框被点击时的槽函数
    void HexCBoxClicked();              // "16进制数" 复选框被点击时的槽函数
    void SendFrameCBoxClicked();        // “帧　发送” 复选框被点击时的槽函数
    void SendByteCBoxClicked();         // "字节发送" 复选框被点击时的槽函数
    void SetTextFormat();               // 数据输入框内容改变时触发该该函数
    void UsagePbtClicked();             // "使用说明"按钮点击时的槽函数（系统调用，打开说明文档）
    void SendPBtClicked();              // "发送数据"按钮被点击时的槽函数（通过串口发送数据）
    void OpenFilePBtClicked();          // "打开文件"按钮被点击时的槽函数（读入文本数据）   
    void DelayTimeLEditClicked();       // 监控输入框的内容，时间间隔必须大于等于10（以后会改善）
    void SendDataTimerOverTime();       // 定时器计数溢出中断函数
    void SendCircularyCBoxClicked();    // "循环发送"被点击时，执行该函数（循环被取消，如果定时器在工作，关闭定时器）
signals:
    void SendDatas(int mode, QByteArray data);    // 发送数据时，发射该信号，发送的数据是信号参数
};
#endif
