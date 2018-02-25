/*******************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: InfoArea.h
Version: V0.0.0/LittleBoy/2017-10-02
Description: 信息显示区，用于串口收发数据的显示
Others: not yet
Qt Version: Qt 5.6.3 MinGW492_32
History:
    01. LittleBoy/2017-10-02~2017-10-
        创建本文档，完善本文档。

*******************************************************************************/
#ifndef __INFO_AREA_H
#define __INFO_AREA_H

#include <QTextBrowser>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QByteArray>

/*
 * 以下两个宏用于区别显示的数据是接收的数据还是发送的数据，显示数据时，用“收到数据”、“发送数据”
 * 来区别显示
 */
#define RECEIVE_DATA 0
#define SEND_DATA 1

class InfoArea;
class InfoArea:public QGroupBox
{
    Q_OBJECT
public:
    InfoArea(QWidget *parent =  Q_NULLPTR);
    ~InfoArea();
    void ShowData(QByteArray *array, int flag); // 显示收发数据
                                                // flag = RECEIVE_DATA 收数据
                                                // flag = SEND_DATA  发数据
private:
    bool m_bPauseFlag;              // 暂停显示标志

    QTextBrowser *m_pInfoShowArea;  // 信息显示区
    QPushButton *m_pPausePBt;       // “暂停显示”按钮
    QPushButton *m_pClearInfoPBt;   // “清空信息”按钮
    QCheckBox *m_pShowDateCBox;     // “显示日期”复选框
    QCheckBox *m_pShowTimeCBox;     // “显示时间”复选框
    QCheckBox *m_pCountByteCBox;    // “统计字节数”复选框

    /*
     * 以下复选框为互斥关系，只能有一个被选中，控件初始化时，
     * “十六进制”复选框被选中
     */
    QCheckBox *m_pHexCBox;          // “十六进制”复选框
    QCheckBox *m_pDecCBox;          // “十进制”复选框
    QCheckBox *m_pOctCBox;          // “八进制”复选框
    QCheckBox *m_pBinCBox;          // “二进制”复选框
    QCheckBox *m_pASCIICBox;        // "ASCII"显示格式复选框 20171113新增

private slots:
    void PausePBtClicked();         // “暂停显示”按钮点击槽函数
    void ClearInfoPBtClicked();     // “清空信息”按钮点击槽函数

    void HexCBoxClicked();          // “十六进制”复选框点击槽函数
    void DecCBoxClicked();          // “十进制”复选框点击槽函数
    void OctCBoxClicked();          // “八进制”复选框点击槽函数
    void BinCBoxClicked();          // “二进制”复选框点击槽函数
    void ASCIICBoxClicked();        // "ASCII"复选框点击槽函数
};


#endif
