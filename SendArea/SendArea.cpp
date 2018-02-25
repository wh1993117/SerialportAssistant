/*******************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: SendArea.cpp
Version: V0.0.0/LittleBoy/2017-10-08
Description: 数据发送区域，本控件提供一个直观的用于编辑串口发送数据的图形界面
Others: not yet
Function List:
    01. SendArea() 本控件的构造函数
    02. ~SendArea() 本控件的析构函数
    03. ASCIICBoxClicked() "ASCII字符" 复选框被点击时的槽函数
    04. HexCBoxClicked() "16进制数" 复选框被点击时的槽函数
    05. SendFrameCBoxClicked() “帧　发送” 复选框被点击时的槽函数
    06. SendByteCBoxClicked() "字节发送" 复选框被点击时的槽函数
    07. SetTextFormat() 设置本控件数据输入框的文本格式，输入框内容发生变化时触发（槽函数）
    08. SendPBtClicked() 通过串口发送数据
    09. OpenFilePBtClicked() 读入文本数据 2017-11-12 新增
    10. DelayTimeLEditClicked() 监控输入框的内容，用于控制循环发送时，每帧数据之间的间间隔必须大于等于
                                MINI_DELAY_TIME，该宏现定义的值为1。 2017/12/11 新增
    11. SendDataTimerOverTime()  定时器计数溢出中断函数吗，用于发送数据
    12. SendCircularyCBoxClicked() 循环发送被取消，定时器停止
    13. SetText() 设置输入框文本内容
History:
    01. LittleBoy/2017-10-08~2017-10-30
        Create the file.
*******************************************************************************/

#include "SendArea.h"
#include <QGridLayout>
#include <QRegExp>
#include <QValidator>
#include <QMessageBox>
#include <QObject>
#include <QByteArray>
#include <QStringList>
#include <QFileDialog>
#include <QFile>

/*
 * 本控件的构造函数
 */
SendArea::SendArea(QSerialPort *serialport, QWidget *parent):QGroupBox(parent)
{
    m_bSendByFrame = true;      // 默认"帧发送"方式
    m_bNonRepeating = false;
    m_index = 0;
    m_pDataArray = &mDataArray;
    m_pSerialPort = serialport;

    m_pASCIICBox = new QCheckBox(tr("ASCII字符"));
    m_pASCIICBox->setChecked(true);
    connect(m_pASCIICBox, SIGNAL(clicked(bool)),
            this, SLOT(ASCIICBoxClicked()));

    m_pHexCBox = new QCheckBox(tr("十六进制"));
    connect(m_pHexCBox, SIGNAL(clicked(bool)),
            this, SLOT(HexCBoxClicked()));

    m_pSendFrameCBox = new QCheckBox(tr("帧　发送"));
    m_pSendFrameCBox->setChecked(true);
    connect(m_pSendFrameCBox, SIGNAL(clicked(bool)),
            this, SLOT(SendFrameCBoxClicked()));

    m_pSendByteCBox = new QCheckBox(tr("字节发送"));
    connect(m_pSendByteCBox, SIGNAL(clicked(bool)),
            this, SLOT(SendByteCBoxClicked()));

    m_pSendCircularyCBox = new QCheckBox(tr("循环发送"));
    m_pSendCircularyCBox->setChecked(true);
    connect(m_pSendCircularyCBox, SIGNAL(clicked(bool)),
            this, SLOT(SendCircularyCBoxClicked()));

    m_pSendDealyCBox = new QCheckBox(tr("延时发送"));
    m_pSendDealyCBox->setChecked(true);

    m_pDelayTimeLab = new QLabel(tr("延时时间(ms)"));
    m_pDelayTimeLEdit = new QLineEdit;
    m_pDelayTimeLEdit->setMaximumWidth(70);
    m_pDelayTimeLEdit->setMaxLength(8);
    m_pDelayTimeLEdit->setText(tr("%1").arg(MINI_DELAY_TIME));
    m_pDelayTimeLEdit->setAlignment(Qt::AlignRight);
    // 以下三行限制延时时间输入框的输入格式（只能输入数字0-9）
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, m_pDelayTimeLEdit);
    m_pDelayTimeLEdit->setValidator(validator);
    connect(m_pDelayTimeLEdit, SIGNAL(textChanged(QString)),
            this, SLOT(DelayTimeLEditClicked()));

    m_pDataTEdit = new QTextEdit;
    m_pDataTEdit->setText("10");
    connect(m_pDataTEdit, SIGNAL(textChanged()),
            this, SLOT(SetTextFormat()));

    m_pSendPBt = new QPushButton(tr("发送数据"));
    connect(m_pSendPBt, SIGNAL(clicked(bool)),
            this, SLOT(SendPBtClicked()));

    m_pClearPBt = new QPushButton(tr("清空"));
    m_pClearPBt->setMaximumWidth(45);
    connect(m_pClearPBt, SIGNAL(clicked(bool)),
            m_pDataTEdit, SLOT(clear()));

    m_pUsagePbt = new QPushButton(tr("说明"));
    m_pUsagePbt->setMaximumWidth(45);
    connect(m_pUsagePbt, SIGNAL(clicked(bool)),
            this, SLOT(UsagePbtClicked()));

    m_pOpenFilePBt = new QPushButton(tr("打开\n文件"));
    m_pOpenFilePBt->setMinimumHeight(72);
    connect(m_pOpenFilePBt, SIGNAL(clicked(bool)),
            this, SLOT(OpenFilePBtClicked()));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(m_pSendPBt, 0, 0, 1, 2);
    layout->addWidget(m_pClearPBt, 1, 0, 1, 1);
    layout->addWidget(m_pUsagePbt, 1, 1, 1, 1);
    layout->addWidget(m_pASCIICBox, 0, 2, 1, 1);
    layout->addWidget(m_pHexCBox, 1, 2, 1, 1);
    layout->addWidget(m_pSendFrameCBox, 0, 3, 1, 1);
    layout->addWidget(m_pSendByteCBox, 1, 3, 1, 1);
    layout->addWidget(m_pSendCircularyCBox, 0, 4, 1, 1);
    layout->addWidget(m_pSendDealyCBox, 1, 4, 1, 1);
    layout->addWidget(m_pDelayTimeLab, 0, 5, 1, 1);
    layout->addWidget(m_pDelayTimeLEdit, 1, 5, 1, 1);
    layout->addWidget(m_pOpenFilePBt, 0, 6, 2, 1);
    layout->addWidget(m_pDataTEdit, 0, 7, 2, 1);


    this->setTitle(tr("数据发送区"));
    this->setLayout(layout);

    /*
     * 帮助窗口
     */
    helpDl = new HelpDialog;
    helpDl->setModal(true);
    helpDl->hide();

    /*
     * 定时器
     */
    m_pSendDataTimer = new QTimer;
    connect(m_pSendDataTimer, SIGNAL(timeout()),
            this, SLOT(SendDataTimerOverTime()));
}

/*
 * 本控件的析构函数
 */
SendArea::~SendArea()
{
    //
}

/*******************************************************************************
Number: 02
Function: ASCIICBoxClicked()
Description: "ASCII字符" 复选框被点击时的槽函数，如果"ASCII字符"被勾选，则保持勾
             选状态，否则设置为勾选状态，并且取消"16进制数"复选框的勾选状态
Calls:
Called By:
Input: null
Output: null
Return: void
Others: not yet
History：
    01. LittleBoy/2017-10-08~2017-10-30 创建并实现函数
*******************************************************************************/
void SendArea::ASCIICBoxClicked()
{
    if (m_pASCIICBox->isChecked())
    {
        m_pHexCBox->setChecked(false);
        m_pDataTEdit->clear();
    }
    else
    {
        m_pASCIICBox->setChecked(true);
        m_pHexCBox->setChecked(false);
    }
}

/*******************************************************************************
Number: 03
Function: HexCBoxClicked()
Description: "16进制数" 复选框被点击时的槽函数，如果"16进制数"被勾选，则保持勾选状态，否则
             设置为勾选状态，并且取消"ASCII字符"复选框的勾选状态
Calls:
Called By:
Input: null
Output: null
Return: void
Others: not yet
History：
    01. LittleBoy/2017-10-08~2017-10-30 创建并实现函数
*******************************************************************************/
void SendArea::HexCBoxClicked()
{
    if (m_pHexCBox->isChecked())
    {
        m_pASCIICBox->setChecked(false);
        m_pDataTEdit->clear();
    }
    else
    {
        m_pHexCBox->setChecked(true);
        m_pASCIICBox->setChecked(false);
    }
}

/*******************************************************************************
Number: 04
Function: HexCBoxClicked()
Description: “帧　发送” 复选框被点击时的槽函数，如果“帧　发送”被勾选，则保持勾选状态，否则
             设置为勾选状态，并且取消"字节发送"复选框的勾选状态
Calls:
Called By:
Input: null
Output: null
Return: void
Others: not yet
History：
    02. LittleBoy 2017/12/11
        新增 m_bSendByFrame 变量的赋值
    01. LittleBoy/2017-10-08 创建并实现函数
*******************************************************************************/
void SendArea::SendFrameCBoxClicked()
{
    if (m_pSendFrameCBox->isChecked())
    {
        m_pSendByteCBox->setChecked(false);
    }
    else
    {
        m_bSendByFrame = true;
        m_pSendFrameCBox->setChecked(true);
        m_pSendByteCBox->setChecked(false);
    }
}

/*******************************************************************************
Number: 05
Function: SendByteCBoxClicked()
Description: "字节发送" 复选框被点击时的槽函数，如果"字节发送"被勾选，则保持勾选状态，否则
             设置为勾选状态，并且取消“帧　发送”复选框的勾选状态
Calls:
Called By:
Input: null
Output: null
Return: void
Others: not yet
History：
    02. LittleBoy 2017/12/11
        新增 m_bSendByFrame 变量的赋值
    01. LittleBoy/2017-10-08 创建并实现函数
*******************************************************************************/
void SendArea::SendByteCBoxClicked()
{
    if (m_pSendByteCBox->isChecked())
    {
        m_pSendFrameCBox->setChecked(false);
    }
    else
    {
        m_bSendByFrame = false;
        m_pSendByteCBox->setChecked(true);
        m_pSendFrameCBox->setChecked(false);
    }
}

/*******************************************************************************
Number: 06
Function: SetTextFormat()
Description: 数据输入框内容发生变化时，执行本槽函数，本槽函数将会限制输入内容（文本格式）；数据
             输入框有两种数据格式，文本（ASCII字符）和16进制数据格式.
Calls:
Called By: (数据输入框的内容发生变化时，该函数被触发)
Parameter: null
Return: void
Others: not yet
History：
    01. LittleBoy/2017-10-30 创建并实现函数
*******************************************************************************/
#include <QDebug>
void SendArea::SetTextFormat()
{
    QString tempStr;
    QString temp;
    temp.clear();

    /*
     * 必须取消信号与槽的连接，否则在在设置输入框内容时会进入死循环
     */
    disconnect(m_pDataTEdit, SIGNAL(textChanged()),
               this, SLOT(SetTextFormat()));

    /*
     * ascii可见字符格式
     */
    if (m_pASCIICBox->isChecked())
    {
        tempStr = m_pDataTEdit->toPlainText();
        tempStr.remove(QRegExp("[^0-9a-zA-z~`!@#$%^&*()_+-=[]{}\\|:;\"',./<>?\n]"));
        m_pDataTEdit->clear();
        m_pDataTEdit->append(tempStr);
    }

    /*
     * 16进制数据格式（仅限数字输入、a-f或A-f的输入）
     */
    else
    {
        tempStr = m_pDataTEdit->toPlainText();
        tempStr.remove(QRegExp("[^0-9a-fA-F]"));
        for (int index = 0; index < tempStr.count(); index++)
        {
            if (!(index%2) && index != 0)
            {

                temp += ' ';    // 每两个字符之间插入一个空格
            }
            temp += tempStr[index];
        }
        m_pDataTEdit->clear();
        m_pDataTEdit->append(temp);
    }

    /*
     * 重新关联信号和槽
     */
    connect(m_pDataTEdit, SIGNAL(textChanged()),
            this, SLOT(SetTextFormat()));
}

/*******************************************************************************
Number: 07
Function: UsagePbtClicked()
Description: "使用说明"按钮点击时的槽函数（系统调用，打开说明文档）
Calls:
Called By: (数据输入框的内容发生变化时，该函数被触发)
Parameter: null
Return: void
Others: not yet
History：
    01. LittleBoy/2017-11-03 创建并实现函数
*******************************************************************************/
void SendArea::UsagePbtClicked()
{
    helpDl->show();
}

/*******************************************************************************
Number: 08
Function: SendPBtClicked()
Description: "发送数据"按钮被点击时的槽函数（通过串口发送数据）
Calls:
Called By: ("发送数据"按钮被点击时，该函数被触发)
Parameter: null
Return: void
Others: not yet
History：
    01. LittleBoy/2017-11-03 创建并实现函数
*******************************************************************************/
void SendArea::SendPBtClicked()
{  
    /*
     * 如果串口未打开，弹窗提示并返回
     */
    if (!m_pSerialPort->isOpen())
    {
        QMessageBox::warning(this, tr("Please open the serialport"),
                             tr("<font face=楷体 size=22 color=red>请打开串口后重试！</font>"));
        return;
    }

    /*
     * 获取数据输入框的内容，如果为空，输出提示信息；否则执行发送程序
     */
    QString data = m_pDataTEdit->toPlainText();
    if (data.isEmpty())
    {
        /*
         * 提示语
         */
        m_pDataTEdit->setPlaceholderText(tr("发送数据不能为空，请在此处输入你想要发送的数据..."));
        return;
    }

    /*
     * 提取数据转换为16进制数
     */
    if (m_pHexCBox->isChecked())
    {
        QStringList strList = data.split(' ');     // 字符串分割
        QString strTemp;
        for (int index = 0; index < strList.count(); index++)
        {
            strTemp = strList.at(index);
            mDataArray[index] = strTemp.toInt(NULL, 16);    // 字符串转16进制数
            qDebug() << mDataArray[index];
        }
    }

    //发送处理
    {
        if (m_pASCIICBox->isChecked() && m_pSendFrameCBox->isChecked()
            && !m_pSendCircularyCBox->isChecked())//字符-帧-不循环（延时无效）
        {
            mDataArray = data.toLatin1();
            m_pSerialPort->write(mDataArray);               // 串口发送数据
            emit SendDatas(MODE_BYTE_ARRAY, mDataArray);    // 发射信号
        }
        else if (m_pASCIICBox->isChecked() && m_pSendFrameCBox->isChecked()
                 && m_pSendCircularyCBox->isChecked()
                 && !m_pSendDealyCBox->isChecked())//字符-帧-循环-不延时(默认延时参数)
        {
            m_bSendByFrame = true;
            mDataArray = data.toLatin1();
            m_pSendDataTimer->start(MINI_DELAY_TIME);
        }
        else if (m_pASCIICBox->isChecked() && m_pSendFrameCBox->isChecked()
                 && m_pSendCircularyCBox->isChecked()
                 && m_pSendDealyCBox->isChecked())//字符-帧-循环-延时（没帧数据之间有一段延时）
        {
            m_bSendByFrame = true;
            mDataArray = data.toLatin1();
            m_pSendDataTimer->start(m_pDelayTimeLEdit->text().toInt());
        }

        /***********************************************************/

        if (m_pASCIICBox->isChecked() && m_pSendByteCBox->isChecked()
            && !m_pSendCircularyCBox->isChecked())//字符-字节-不循环（延时有效）
        {
            m_index = 0;
            m_bSendByFrame = false;
            m_bNonRepeating = true;
            m_pSendDataTimer->start(m_pDelayTimeLEdit->text().toInt());
        }
        else if (m_pASCIICBox->isChecked() && m_pSendByteCBox->isChecked()
                 && m_pSendCircularyCBox->isChecked()
                 && !m_pSendDealyCBox->isChecked())//字符-字节-循环-不延时(默认延时参数)
        {
            m_index = 0;
            m_bSendByFrame = false;
            mDataArray = data.toLatin1();           
            m_pSendDataTimer->start(MINI_DELAY_TIME);
        }
        else if (m_pASCIICBox->isChecked() && m_pSendByteCBox->isChecked()
                 && m_pSendCircularyCBox->isChecked()
                 && m_pSendDealyCBox->isChecked())//字符-字节-循环-延时（每帧数据之间有一段延时）
        {
            m_index = 0;
            m_bSendByFrame = false;
            mDataArray = data.toLatin1();            
            m_pSendDataTimer->start(m_pDelayTimeLEdit->text().toInt());
        }

        /***********************************************************/

        else if (m_pHexCBox->isChecked() && m_pSendFrameCBox->isChecked()
                 && !m_pSendCircularyCBox->isChecked())//16进制-帧-不循环（延时无效）
        {            
            m_pSerialPort->write(mDataArray);                   // 串口发送数据
            emit SendDatas(MODE_BYTE_ARRAY, mDataArray);    // 发射信号
        }
        else if (m_pHexCBox->isChecked() && m_pSendFrameCBox->isChecked()
                 && m_pSendCircularyCBox->isChecked()
                 && !m_pSendDealyCBox->isChecked())//16进制-帧-循环-不延时(默认延时参数)
        {
            m_bSendByFrame = true;
            m_pSendDataTimer->start(MINI_DELAY_TIME);
        }
        else if (m_pHexCBox->isChecked() && m_pSendFrameCBox->isChecked()
                 && m_pSendCircularyCBox->isChecked()
                 && m_pSendDealyCBox->isChecked())//16进制-帧-循环-延时
        {
            m_bSendByFrame = true;
            m_pSendDataTimer->start(m_pDelayTimeLEdit->text().toInt());
        }        

        /***********************************************************/
        else if (m_pHexCBox->isChecked() && m_pSendByteCBox->isChecked()
                 && !m_pSendCircularyCBox->isChecked())//16进制-字节-不循环（延时有效）
        {
            m_index = 0;
            m_bSendByFrame = false;
            m_bNonRepeating = true;
            m_pSendDataTimer->start(m_pDelayTimeLEdit->text().toInt());
        }
        else if (m_pHexCBox->isChecked() && m_pSendByteCBox->isChecked()
                 && m_pSendCircularyCBox->isChecked()
                 && !m_pSendDealyCBox->isChecked())//16进制-字节-循环-不延时(默认延时参数)
        {
            m_index = 0;
            m_bSendByFrame = false;
            m_pSendDataTimer->start(MINI_DELAY_TIME);
        }
        else if (m_pHexCBox->isChecked() && m_pSendByteCBox->isChecked()
                 && m_pSendCircularyCBox->isChecked()
                 && m_pSendDealyCBox->isChecked())//16进制-字节-循环-延时
        {
            m_index = 0;
            m_bSendByFrame = false;
            m_pSendDataTimer->start(m_pDelayTimeLEdit->text().toInt());
        }
        /***********************************************************/
        else
        {
            // not yet
        }
    }
}

/*******************************************************************************
Number: 09
Function: OpenFilePBtClicked()
Description: "打开文件"按钮被点击时的槽函数（读入文本数据）
Calls:
Called By: ("打开文件"按钮被点击时，该函数被触发)
Parameter: null
Return: void
Others: not yet
History：
    03. LittleBoy/2017-12-26 增加 AllFile(*.*) 过滤器
    02. LittleBoy/2017-12-25 初步实现函数预定功能
    01. LittleBoy/2017-11-12 创建函数（未实现功能）
*******************************************************************************/
void SendArea::OpenFilePBtClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开数据文件"),".",
                                                    "TextFile(*.txt);;DataFile(*.data);;AllFile(*.*)");
    if (!fileName.isEmpty())
    {
        QFile dataFile(fileName);
        bool isOK = dataFile.open(QFile::ReadWrite);
        if (isOK)
        {
            m_pDataTEdit->setText(QString(dataFile.readAll()));
        }
        else
        {
            QMessageBox::warning(this, tr("打开文件错误"), dataFile.errorString());
        }
        dataFile.close();
    }
    else
    {
        return;
    }
}

/*******************************************************************************
Number: 10
Function: DelayTimeLEditClicked()
Description: 监控输入框的内容，用于控制循环发送时，每帧数据之间的间间隔必须大于等于
             MINI_DELAY_TIME，该宏现定义的值为1（循环发送的方法有待改善）
Calls:
Called By: (延时时间编辑框内容发生改变时，该函数被调用)
Parameter: null
Return: void
Others: 该函数中有取消信号关联、重新关联信号这两步操作，其目的就是避免该函数被无限递归调用从而造成
        程序崩溃。
History：
    01. LittleBoy/2017-12-11 创建函数
*******************************************************************************/
void SendArea::DelayTimeLEditClicked()
{
    int64_t delayTime;

    /*
     * 取消信号关联
     */
    disconnect(m_pDelayTimeLEdit, SIGNAL(textChanged(QString)),
            this, SLOT(DelayTimeLEditClicked()));

    delayTime = m_pDelayTimeLEdit->text().toInt();
    if (delayTime < MINI_DELAY_TIME)
    {
        m_pDelayTimeLEdit->setText(tr("%1").arg(MINI_DELAY_TIME));
    }

    /*
     * 重新关联信号
     */
    connect(m_pDelayTimeLEdit, SIGNAL(textChanged(QString)),
            this, SLOT(DelayTimeLEditClicked()));
}

/*******************************************************************************
Number: 11
Function: SendDataTimerOverTime()
Description: 定时器计数溢出中断函数
Calls:
Called By: (定时器计数溢出时)
Parameter: null
Return: void
Others: not yet
History：
    01. LittleBoy/2017-12-11 创建函数
*******************************************************************************/
void SendArea::SendDataTimerOverTime()
{
    /*
     * 如果串口关闭，定时器停止
     */
    if (!m_pSerialPort->isOpen())
    {
        m_pSendDataTimer->stop();
        return;
    }

    if (m_bSendByFrame == true)                     // 帧发送
    {
        m_pSerialPort->write(mDataArray);           // 串口发送数据
        emit SendDatas(MODE_BYTE_ARRAY, mDataArray);    // 发射信号
    }
    else                                            // 字节发送
    {
        if (m_index == mDataArray.count())
        {
            if (m_bNonRepeating)
            {
                m_index = 0;
                m_bNonRepeating = false;            // 复位标志
                m_pSendDataTimer->stop();
                return;
            }
            else
            {
                m_index = 0;
            }
        }

        m_pSerialPort->write(&(mDataArray.data()[m_index]), 1); // 串口发送数据(每次发送一个字节)
        emit SendDatas(MODE_CHAR, mDataArray);    // 发射信号
        m_index++;
    }
}

/*******************************************************************************
Number: 12
Function: SendCircularyCBoxClicked()
Description: 循环发送被取消，定时器停止
Calls:
Called By: “循环发送”被点击时，执行该函数
Parameter: null
Return: void
Others: not yet
History：
    01. LittleBoy/2017-12-11 创建函数
*******************************************************************************/
void SendArea::SendCircularyCBoxClicked()
{
    if (!m_pSendCircularyCBox->isChecked())
    {
        m_pSendDataTimer->stop();
    }
}

/*******************************************************************************
Number: 13
Function: SetText()
Description: 设置输入框文本内容
Calls:
Called By:
Parameter: null
Return: void
Others: not yet
History：
    01. LittleBoy/2017-12-25 创建函数
*******************************************************************************/
void SendArea::SetText(QString str)
{
    m_pDataTEdit->setText(str);
}
