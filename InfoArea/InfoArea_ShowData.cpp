/*******************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: InfoArea_ShowData.cpp
Version: V0.0.0/LittleBoy/2017-10-02
Description:  信息显示区
Others: not yet
Qt Version: Qt 5.6.3 MinGW492_32
Function List:
    01. ShowData() 显示数据


History:
    02. LittleBoy/2017-11-13
        修复时间显示勾选问题.
    01. LittleBoy/2017-10-05~2017-10-30
        Create the file.
*******************************************************************************/

#include "InfoArea.h"
#include <QDate>
#include <QTime>

/*******************************************************************************
Function: ShowData()
Description: 更新本控件上数据计数的显示
Calls:
Called By: 串口发送或接受完一帧数据时，可以调用该函数
Input: dataArray发送或接受的数据；
       flag = RECEIVE_DATA表示数据是发送的数据
       flag = SEND_DATA表示数据是接收的数据
Output: 没有（本函数不改变输入参数）
Return: void
Others: not yet
History：
    03. LittleBoy/2017-11-23 文本提示“收到数据”“发送数据”用蓝色显示
    02. LittleBoy/2017-11-13 修复时间显示勾选问题
    01. LittleBoy/2017-10-05 创建并实现函数
*******************************************************************************/
void InfoArea::ShowData(QByteArray *array, int flag)
{
    /*
     * 如果暂停显示信息，直接返回
     */
    if (m_bPauseFlag)
    {
        return;
    }

    // 用于保存信息
    QString infoStrTemp;
    infoStrTemp.clear();

    // 如果“显示日期”复选框被勾选，输出的信息包含日期信息
    if (m_pShowDateCBox->isChecked())
    {
        infoStrTemp += QDate::currentDate().toString(QString("yyyy.MM.dd"));
        infoStrTemp += " ";
    }

    // 如果“显示时间”复选框被勾选，输出的信息包含时间信息
    if (m_pShowTimeCBox->isChecked())
    {
        infoStrTemp += QTime::currentTime().toString(QString("hh:mm:ss"));
        infoStrTemp += " ";
    }

    /*
     * 以下两个条件用于判断添加的提示语是“收到数据”还是“发送数据”
     */
    if (flag == RECEIVE_DATA)
    {
        infoStrTemp += tr("<font color=blue>收到数据：</font>");
    }
    if (flag == SEND_DATA)
    {
         infoStrTemp += tr("<font color=blue>发送数据：</font>");
    }

    // 用于索引
    int index;

    if (m_pHexCBox->isChecked())// 显示数据用16进制表示
    {
        for(index = 0; index < array->count(); index++)
        {
            infoStrTemp += QString::number(int(array->at(index)), 16);
            infoStrTemp += " ";
        }
    }
    else if (m_pDecCBox->isChecked())// 显示数据用10进制表示
    {
        for(index = 0; index < array->count(); index++)
        {
            infoStrTemp += QString::number(int(array->at(index)), 10);
            infoStrTemp += " ";
        }
    }
    else if (m_pOctCBox->isChecked())// 显示数据用8进制表示
    {
        for(index = 0; index < array->count(); index++)
        {
            infoStrTemp += QString::number(int(array->at(index)), 8);
            infoStrTemp += " ";
        }
    }
    else if (m_pBinCBox->isChecked())// 显示数据用2进制表示
    {
        for(index = 0; index < array->count(); index++)
        {
            infoStrTemp += QString::number(int(array->at(index)), 2);
            infoStrTemp += " ";
        }
    }
    else if (m_pASCIICBox->isChecked()) // 显示数据用ASCII字符表示
    {
        for(index = 0; index < array->count(); index++)
        {
            infoStrTemp += char(array->at(index));
            //infoStrTemp += " ";   // 每个字符之间不需要空格
        }
    }
    /* 默认用16进制表示，原则上（由于本控件的设计关系），以上情况肯定会有一个是成立的，
     * 所以，本else代码段不会被执。
     */
    else
    {
        for(index = 0; index < array->count(); index++)
        {
            infoStrTemp += QString::number(int(array->at(index)), 16);
            infoStrTemp += " ";
        }
    }

    // 输出显示信息
    m_pInfoShowArea->append(infoStrTemp);

    // 显示计数信息
    if (m_pCountByteCBox->isChecked())
    {
        switch(flag)
        {
        case RECEIVE_DATA:
            infoStrTemp = tr("本次共接收%1个字节。").arg(array->count());
            break;
        case SEND_DATA:
            infoStrTemp = tr("本次共发送%1个字节。").arg(array->count());
            break;
        default:
            infoStrTemp = tr("<font color=red>未知错误，无法统计收发字节数！</font>");
            break;
        }

        // 输出显示信息
        m_pInfoShowArea->append(infoStrTemp);
    }
}
