/*******************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: DataCountArea.h
Version: V0.0.0/LittleBoy/2017-09-30
Description: DataCountArea类的声明，该类用于显示收发数据的帧数、字节数
Others: not yet
Qt Version: Qt 5.6.3 MinGW
History:
    01. LittleBoy/2017-09-30~2017-10-02
        创建本文档，完善本文档。

*******************************************************************************/

#ifndef __DATA_COUNT_AREA_H
#define __DATA_COUNT_AREA_H

#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QByteArray>


#define UPDATE_SEND_DATA_FLAG 0      // 更新发送数据计数显示标志
#define UPDATE_RECEIVE_DATA_FLAG 1   // 更新接收数据计数显示标志

#define RESET_RECEIVE_DATA_COUNT 1   // 重置接收数据发送
#define RESET_SEND_DATA_COUNT 0      // 重置发送数据计数
#define RESET_ALL_DATA_COUNT 2       // 重置接收和发送数据计数


class DataCountArea;
class DataCountArea : public QGroupBox
{
    Q_OBJECT
public:
    DataCountArea(QWidget *parent = Q_NULLPTR);
    ~DataCountArea();
    void UpdateDataCount(QByteArray &dataArray, int flag); // 更新数据计数显示函数
    void UpdateDataCount(QByteArray *dataArray, int flag); // 更新数据计数显示函数
    void SendDdataAddOne();                                // 更新数据计数显示函数

private:
    bool bPauseCountFlag;                                  // 暂停计数标志，1暂停

    QPushButton *m_pPauseCountPBt = NULL;                  // 暂停计数
    QLabel *m_pSendDataFrameCountLab = NULL;               // 累计发送帧数显示标签
    QLabel *m_pSendByteCountLab = NULL;                    // 累计发送字节显示标签
    QLabel *m_pReceiveDataFrameCountLab = NULL;            // 累计接收帧数显示标签
    QLabel *m_pReceiveByteCountLab = NULL;                 // 累计接收字节显示标签    

    int m_nSendDataFrameCount = 0;                         // 发送数据帧计数整形变量
    int m_nSendByteCount = 0;                              // 发送字节数计数整形变量
    int m_nReceiveDataFrameCount = 0;                      // 接收数据帧计数整形变量
    int m_nReceiveByteCount = 0;                           // 接收字节数计数整形变量

    QPushButton *m_pResetSendCountPBt = NULL;              // 重置发送数据计数按钮
                                                           // 包括数据帧计数和字节计数

    QPushButton *m_pResetReceiveCountPBt = NULL;           // 重置接收数据计数按钮
                                                           // 包括数据帧计数和字节计数

    QPushButton *m_pResetCountPBt = NULL;                  // 重置发送和接收数据计数

                                                           // 以上所说重置就是将相应的
                                                           // 值设置为0
private slots:
    void ResetDataCount(int flag);                       // 重置数据计数函数
    void PauseCount();                                    // 暂停计数函数

    void ResetReceiveCountPBtClicked();                // “重置收数据”按钮点击槽函数
    void ResetSendCountPBtClicked();                   // “重置发数据”按钮点击槽函数
    void ResetCountPBtClicked();                        // “重置计数”按钮点击槽函数

signals:
    void ResetCountPBtIsClicked(int flag);              // 重置计数信号
};

#endif
