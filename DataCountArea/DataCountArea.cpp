/*******************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: DataCountArea.cpp
Version: V0.0.0/LittleBoy/2017-10-01
Description:  DataCountArea类的声明，该类用于显示收发数据的帧数、字节数
Others: 今天国庆节，公司放假五天，没回家，无聊，在敲代码，看直播！
Function List:
    01. DataCountArea()   构造函数
    02. ~DataCountArea()  析构函数
    03. UpdateDataCount() 更新数据计数，该函数有两个重载；注意flag参数
    04. UpdateDataCount()
    05. ResetDataCount()  重置计数函数，注意该函数的flag参数
    06. ResetReceiveCountPBtClicked() “重置收数据”按钮槽函数
    07. ResetSendCountPBtClicked() “重置发数据”按钮槽函数
    08. ResetCountPBtClicked() “重置计数”按钮槽函数
    09. SendDdataAddOne()   更新数据显示

History:
    01. LittleBoy/2017/09/30~2017-10-02
        Create the file.
*******************************************************************************/

#include "DataCountArea.h"

#include <QVBoxLayout>

/*
 * 构造函数
 */
DataCountArea::DataCountArea(QWidget *parent) : QGroupBox(parent)
{
    // 暂停计数标志初始化为false
    bPauseCountFlag = false;

    /*
     * 数值初始化，该数值在类内已经初始化为0，此处初始化可以省略
     */
    m_nReceiveDataFrameCount = 0;
    m_nReceiveByteCount = 0;
    m_nSendDataFrameCount = 0;
    m_nSendByteCount = 0;

    m_pPauseCountPBt = new QPushButton(tr("暂停计数"));
    connect(m_pPauseCountPBt, SIGNAL(clicked(bool)),
            this, SLOT(PauseCount()));

    /*
     * 标签初始化
     */
    m_pReceiveDataFrameCountLab = new QLabel(tr("收帧数：<font color=blue>%1</font>")
                                             .arg(m_nReceiveDataFrameCount));
    m_pReceiveByteCountLab = new QLabel(tr("收字节：<font color=blue>%1</font>")
                                        .arg(m_nReceiveByteCount));
    m_pSendDataFrameCountLab = new QLabel(tr("发帧数：<font color=blue>%1</font>")
                                          .arg(m_nSendDataFrameCount));
    m_pSendByteCountLab = new QLabel(tr("发字节：<font color=blue>%1</font>")
                                     .arg(m_nSendByteCount));

    /*
     * 按钮初始化
     */
    m_pResetReceiveCountPBt = new QPushButton(tr("重置收计数"));
    m_pResetReceiveCountPBt->setToolTip(tr("重置收数据计数"));
    connect(m_pResetReceiveCountPBt, SIGNAL(clicked(bool)),
            this, SLOT(ResetReceiveCountPBtClicked()));

    m_pResetSendCountPBt = new QPushButton(tr("重置发计数"));
    m_pResetSendCountPBt->setToolTip(tr("重置发数据计数"));
    connect(m_pResetSendCountPBt, SIGNAL(clicked(bool)),
            this, SLOT(ResetSendCountPBtClicked()));

    m_pResetCountPBt = new QPushButton(tr("重置计数"));
    m_pResetCountPBt->setToolTip(tr("重置收数据计数和发数据计数"));
    connect(m_pResetCountPBt, SIGNAL(clicked(bool)),
            this, SLOT(ResetCountPBtClicked()));


    /*
     * 布局管理器
     */
    QVBoxLayout *pThisLayout = new QVBoxLayout;
    pThisLayout->addWidget(m_pPauseCountPBt);
    pThisLayout->addWidget(m_pReceiveDataFrameCountLab);
    pThisLayout->addWidget(m_pReceiveByteCountLab);
    pThisLayout->addWidget(m_pSendDataFrameCountLab);
    pThisLayout->addWidget(m_pSendByteCountLab);
    pThisLayout->addWidget(m_pResetReceiveCountPBt);
    pThisLayout->addWidget(m_pResetSendCountPBt);
    pThisLayout->addWidget(m_pResetCountPBt);

    this->setLayout(pThisLayout);
    this->setTitle(tr("数据统计区"));

    /*
     * "重置收计数"、"重置发计数"、"重置计数"这三个按钮被点击时，在其槽函数中会发射
     * ResetCountPBtIsClicked(int) 信号；参数分别为：
     * RESET_RECEIVE_DATA_COUNT
     * RESET_SEND_DATA_COUNT
     * RESET_ALL_DATA_COUNT
     * ResetCountPBtIsClicked(int) 信号与ResetDataCount(int)关联。
     *
     */
    connect(this, SIGNAL(ResetCountPBtIsClicked(int)),
            this, SLOT(ResetDataCount(int)));
}


/*
 * 析构函数， 没有功能，仪式上定义
 */
DataCountArea::~DataCountArea()
{
    // NO Function
}


/*******************************************************************************
Function: UpdateDataCount()
Description: 更新本控件上数据计数的显示
Calls:
Called By: 串口发送或接受完一帧数据时，可以调用该函数
Input: dataArray发送或接受的数据；
       flag = SEND_FLAG表示数据是发送的数据
       flag = RECEIVE_FLAG表示数据是接收的数据
Output: 没有（本函数不改变输入参数）
Return: void
Others: not yet
History：
    02. LittleBoy/2017-11-23
        修复接收数据计数不显示的问题
    01. LittleBoy/2017-09-22~2017-10-02
        创建并实现函数
*******************************************************************************/
void DataCountArea::UpdateDataCount(QByteArray &dataArray, int flag)
{
    /*
     * 如果暂停计数，则不跟新计数数据
     */
    if (bPauseCountFlag)
    {
        return;
    }
    switch(flag)
    {
    case UPDATE_RECEIVE_DATA_FLAG:
        m_nReceiveDataFrameCount += 1;
        m_pReceiveDataFrameCountLab->setText(tr("收帧数：<font color=blue>%1</font>")
                                             .arg(m_nReceiveDataFrameCount));

        m_nReceiveByteCount += dataArray.count();
        m_pReceiveByteCountLab->setText(tr("收字节：<font color=blue>%1</font>")
                                        .arg(m_nReceiveByteCount));
        break;
    case UPDATE_SEND_DATA_FLAG:
        m_nSendDataFrameCount += 1;
        m_pSendDataFrameCountLab->setText(tr("发帧数：<font color=blue>%1</font>")
                                          .arg(m_nSendDataFrameCount));

        m_nSendByteCount += dataArray.count();
        m_pSendByteCountLab->setText(tr("发字节：<font color=blue>%1</font>")
                                     .arg(m_nSendByteCount));
        break;
    default:
        break;
    }
}


/*******************************************************************************
Function: UpdateDataCount()
Description: 更新本控件上数据计数的显示
Calls:
Called By: 串口发送或接受完一帧数据时，可以调用该函数
Input: dataArray发送或接受的数据的指针；
       flag = UPDATE_SEND_DATA_FLAG表示数据是发送的数据
       flag = UPDATE_RECEIVE_DATA_FLAG表示数据是接收的数据
Output: 没有（本函数不改变输入参数）
Return: void
Others: not yet
History：
    02. LittleBoy/2017-11-23
        修复接收数据计数不显示的问题
    01. LittleBoy/2017-09-22~2017-10-01
        创建并实现函数
*******************************************************************************/
void DataCountArea::UpdateDataCount(QByteArray *dataArray, int flag)
{
    /*
     * 如果暂停计数，则不跟新计数数据
     */
    if (bPauseCountFlag)
    {
        return;
    }

    switch(flag)
    {
    case UPDATE_RECEIVE_DATA_FLAG:
        m_nReceiveDataFrameCount += 1;
        m_pReceiveDataFrameCountLab->setText(tr("收帧数：<font color=blue>%1</font>")
                                             .arg(m_nReceiveDataFrameCount));

        m_nReceiveByteCount += dataArray->count();
        m_pReceiveByteCountLab->setText(tr("收字节：<font color=blue>%1</font>")
                                        .arg(m_nReceiveByteCount));
        break;
    case UPDATE_SEND_DATA_FLAG:
        m_nSendDataFrameCount += 1;
        m_pSendDataFrameCountLab->setText(tr("发帧数：<font color=blue>%1</font>")
                                          .arg(m_nSendDataFrameCount));

        m_nSendByteCount += dataArray->count();
        m_pSendByteCountLab->setText(tr("发字节：<font color=blue>%1</font>")
                                     .arg(m_nSendByteCount));
        break;
    default:
        break;
    }
}


/*******************************************************************************
Function: ResetDataCount()
Description: 重置计数
Calls:
Called By: 串口发送或接受完一帧数据时，可以调用该函数
Input: flag = RESET_SEND_DATA_COUNT 重置发送数据
       flag = RESET_RECEIVE_DATA_COUNT 重置接收数据
       flag = RESET_ALL_DATA_COUNT 重置发送和接收数据
Output: 没有（本函数不改变输入参数）
Return: void
Others: not yet
History：
    02. LittleBoy/2017-11-23
        修复部分数据显示的颜色问题
    01. LittleBoy/2017-10-01
        创建并实现函数
*******************************************************************************/
void DataCountArea::ResetDataCount(int flag)
{
    switch(flag)
    {
    case RESET_RECEIVE_DATA_COUNT://重置收数据计数
        m_nReceiveDataFrameCount = 0;
        m_nReceiveByteCount = 0;
        m_pReceiveDataFrameCountLab->setText(tr("收帧数：<font color=blue>%1</font>")
                                             .arg(m_nReceiveDataFrameCount));
        m_pReceiveByteCountLab->setText(tr("收字节：<font color=blue>%1</font>")
                                        .arg(m_nReceiveByteCount));
        break;

    case RESET_SEND_DATA_COUNT://重置发数据计数
        m_nSendDataFrameCount = 0;
        m_nSendByteCount = 0;
        m_pSendDataFrameCountLab->setText(tr("发帧数：<font color=blue>%1</font>")
                                          .arg(m_nSendDataFrameCount));
        m_pSendByteCountLab->setText(tr("发字节：<font color=blue>%1</font>").arg(m_nSendByteCount));
        break;

    case RESET_ALL_DATA_COUNT://重置收发数据计数
        m_nReceiveDataFrameCount = 0;
        m_nReceiveByteCount = 0;
        m_pReceiveDataFrameCountLab->setText(tr("收帧数：<font color=blue>%1</font>")
                                             .arg(m_nReceiveDataFrameCount));
        m_pReceiveByteCountLab->setText(tr("收字节：<font color=blue>%1</font>").arg(m_nReceiveByteCount));

        m_nSendDataFrameCount = 0;
        m_nSendByteCount = 0;
        m_pSendDataFrameCountLab->setText(tr("发帧数：<font color=blue>%1</font>")
                                          .arg(m_nSendDataFrameCount));
        m_pSendByteCountLab->setText(tr("发字节：<font color=blue>%1</font>")
                                     .arg(m_nSendByteCount));
        break;

    default:
        break;
    }
}

/*******************************************************************************
Function: PauseCount()
Description: 暂停/继续计数
Calls:
Called By: （"暂停计数"被点击时执行该函数）
Input: null
Output: null
Return: void
Others: not yet
History：
    01. LittleBoy/2017-10-01 创建并实现函数
*******************************************************************************/
void DataCountArea::PauseCount()
{
    bPauseCountFlag = !bPauseCountFlag;

    if (bPauseCountFlag)
    {
        m_pPauseCountPBt->setText(tr("继续计数"));
    }
    else
    {
        m_pPauseCountPBt->setText(tr("暂停计数"));
    }
}

// “重置收数据”按钮点击槽函数 2017-10-02
void DataCountArea::ResetReceiveCountPBtClicked()
{
    emit ResetCountPBtIsClicked(RESET_RECEIVE_DATA_COUNT);
}

// “重置发数据”按钮点击槽函数 2017-10-02
void DataCountArea::ResetSendCountPBtClicked()
{
    emit ResetCountPBtIsClicked(RESET_SEND_DATA_COUNT);
}

// “重置计数”按钮点击槽函数 2017-10-02
void DataCountArea::ResetCountPBtClicked()
{
    emit ResetCountPBtIsClicked(RESET_ALL_DATA_COUNT);
}

/*******************************************************************************
Function: SendDdataAddOne()
Description: 更新数据显示
Calls:
Called By: （外部调用）
Input: null
Output: null
Return: void
Others: not yet
History：
    01. LittleBoy/2017-12-15 创建并实现函数
*******************************************************************************/
void DataCountArea::SendDdataAddOne()
{
    m_nSendDataFrameCount += 1;
    m_pSendDataFrameCountLab->setText(tr("发帧数：<font color=blue>%1</font>")
                                      .arg(m_nSendDataFrameCount));

    m_nSendByteCount += 1;
    m_pSendByteCountLab->setText(tr("发字节：<font color=blue>%1</font>")
                                 .arg(m_nSendByteCount));
}
