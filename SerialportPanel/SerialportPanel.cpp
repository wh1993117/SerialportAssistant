/*******************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: SerialportPanel.cpp
Version: V0.0.0/LittleBoy/2017-09-22
Description: SerialportPanel类的具体实现(该类是一个串口操作面板，包括串口的选择，串口波特率、
             数据位、奇偶校验位、停止位等的设置，还包含串口的打开、关闭等功能)。
Others: not yet
Function List:
    01. SerialportPanel() 该类的构造函数
    02. ~SerialportPanel() 该类的析构函数（仪式上用的，没有功能）
    03. RefreshDevice() 刷新串口设备
    04. OpenCloseSerialport() 打开/关闭串口
    05. GetSerialport() 获取本控件的串口类指针
History:
    02. LittleBoy/2017/12/11
        修改该类的初始化方式，该类串口指针成员需由一个外部指针来初始化
    01. LittleBoy/2017/09/22~2017-09-27
        Create the file.
*******************************************************************************/

#include <QSerialPortInfo>
#include <QLabel>
#include <QIcon>
#include <QTextBrowser>
#include <QGridLayout>
#include <QGroupBox>
#include <QList>
#include <QStyle>
#include <QApplication>
#include <QTime>

#include "SerialportPanel.h"

/*******************************************************************************
Function: SerialportPanel()
Description: SerialportPanel类的具体实现
Calls:
Called By: SerialportPanel被创建时自动执行该函数
Input: parent 该类的父窗口
Output: null
Return: null
Others: not yet
History：
    03. LittleBoy/2017-11-13 设置默认波特率位9600
    02. LittleBoy/2017-11-12 修改控件布局，使得控件布局更加合理
    01. LittleBoy/2017-09-22~2017-09-27 创建并实现函数
*******************************************************************************/
SerialportPanel::SerialportPanel(QSerialPort *serialport, QWidget *parent):QGroupBox(parent)
{
    m_pSerialport = serialport;
    m_pTextBrowser = new QTextBrowser;

    /*
     * 获取系统已存在的串口，并添加到串口预选框中
     * 如果没有可用串口，则显示"未检测到可用设备"
     * 否则，将可用设备添加至串口预选框中
     */
    QLabel *pSerialportLabel = new QLabel(tr("串行口"));
    m_pSerialportComboBox = new QComboBox;
    QList<QSerialPortInfo> serialPortInfosTemp = QSerialPortInfo::availablePorts();

    if(serialPortInfosTemp.isEmpty())
    {
        m_pSerialportComboBox->clear();

        /*
         * 以下两行代码获取Qt内置图标（warning图标,黄色三角形加一个黑色感叹号）
         */
        QStyle *pStyle = QApplication::style();
        QIcon iconTemp = pStyle->standardIcon(QStyle::SP_MessageBoxWarning);

        m_pSerialportComboBox->addItem(iconTemp, tr("未检测到可用设备"));
    }
    else
    {
        m_pSerialportComboBox->clear();
        QString strTemp;
        strTemp.clear();
        for(int index = 0; index < serialPortInfosTemp.count(); index++)
        {
            strTemp = serialPortInfosTemp.at(index).portName();     //串口名称，如COM1
            strTemp += " - ";
            strTemp += serialPortInfosTemp.at(index).description(); //串口描述
            m_pSerialportComboBox->addItem(strTemp);
        }
    }


    /*
     * 波特率预选框初始化
     */
    QLabel *pBaudRateLabel = new QLabel(tr("波特率"));
    m_pBaudRateComboBox = new QComboBox;
    QList<qint32> baudRateTemp = QSerialPortInfo::standardBaudRates();
    for(int index = 0; index < baudRateTemp.count(); index++)
    {
        m_pBaudRateComboBox->addItem(QString::number(baudRateTemp.at(index)));
    }
    m_pBaudRateComboBox->setCurrentText("9600");

    /*
     * 刷新设备按钮，软件运行后，如果有新的可用串口设备，软件不会自动添加至串口预选框，需人工点击该
     * 按钮来刷新串口预选框中的串口设备
     */
    m_pRefreshPBt = new QPushButton(tr("刷新设备"));
    connect(m_pRefreshPBt, SIGNAL(clicked(bool)),
            this, SLOT(RefreshDevice()));

    /*
     * 打开串口按钮，打开串口的同时，该类还会发射一个SerialportIsOpen()的信号
     * 打开串口后，该按钮文本显示为"关闭按钮"，关闭串口后，该按钮文本显示为"打开串口"
     */
    m_pOpenSerialportPBt = new QPushButton(tr("打开串口"));
    connect(m_pOpenSerialportPBt, SIGNAL(clicked(bool)),
            this, SLOT(OpenCloseSerialport()));

    /*
     *数据位、停止位、奇偶校验位
     */
    QLabel *pDataBitsLabel = new QLabel(tr("数据位"));
    m_pDataBitsComboBox = new QComboBox;
    m_pDataBitsComboBox->clear();
    m_pDataBitsComboBox->addItem("5");
    m_pDataBitsComboBox->addItem("6");
    m_pDataBitsComboBox->addItem("7");
    m_pDataBitsComboBox->addItem("8");
    m_pDataBitsComboBox->setCurrentText("8");

    QLabel *pStopBitsLabel = new QLabel(tr("停止位"));
    m_pStopBitsComboBox = new QComboBox;
    m_pStopBitsComboBox->clear();
    m_pStopBitsComboBox->addItem("1");
    m_pStopBitsComboBox->addItem("1.5");
    m_pStopBitsComboBox->addItem("2");

    QLabel *pParityLabel = new QLabel(tr("%1奇偶校验位").arg(' '));
    m_pParityComboBox = new QComboBox;
    m_pParityComboBox->clear();
    m_pParityComboBox->addItem(tr("None"));
    m_pParityComboBox->addItem("Even");
    m_pParityComboBox->addItem("Odd");
    m_pParityComboBox->addItem("Space");
    m_pParityComboBox->addItem("Mark");

    /*
     * 往栅格布局管理器上添加部件
     */
    QGridLayout *pGridLayout = new QGridLayout;
    pGridLayout->addWidget(m_pOpenSerialportPBt, 0, 0, 1, 1);
    pGridLayout->addWidget(m_pRefreshPBt, 1, 0, 1, 1);
    pGridLayout->addWidget(pSerialportLabel, 0, 1, 1, 1);
    pGridLayout->addWidget(m_pSerialportComboBox, 0, 2, 1, 7);
    pGridLayout->addWidget(pDataBitsLabel, 1, 1, 1, 1);
    pGridLayout->addWidget(m_pDataBitsComboBox, 1, 2, 1, 1);
    pGridLayout->addWidget(pStopBitsLabel, 1, 3, 1, 1);
    pGridLayout->addWidget(m_pStopBitsComboBox, 1,4, 1, 1);
    pGridLayout->addWidget(pBaudRateLabel, 1, 5, 1, 1);
    pGridLayout->addWidget(m_pBaudRateComboBox, 1, 6, 1, 1);
    pGridLayout->addWidget(pParityLabel, 1, 7, 1, 1);
    pGridLayout->addWidget(m_pParityComboBox, 1, 8, 1, 1);
    pGridLayout->addWidget(m_pTextBrowser, 0, 9, 2, 1);

    /*
     * 将栅格布局管理器添加至串口操作面板组控件上
     */
    this->setTitle(tr("串口操作面板"));
    this->setLayout(pGridLayout);
}

/*******************************************************************************
Function: ~SerialportPanel()
Description: SerialportPanel类的析构函数
Calls: null
Called By: 当SerialportPanel被销毁时
Input: null
Output: null
Return: null
Others: 该函数实际中好像没什么用，经测试，delete该类时，并没有执行该函数
History：
    01. LittleBoy/2017-09-22 创建并实现函数
*******************************************************************************/
SerialportPanel::~SerialportPanel()
{
    //No Operation!
}

/*******************************************************************************
Function: RefreshDevice()
Description: 刷新现有可用串口设备，并重新添加到串口预选框中
Calls: null
Called By: “刷新设备”按钮被点击时执行该函数
Input: null
Output: null
Return: null
Others: not yet
History：
    01. LittleBoy/2017-09-27 创建并实现函数
*******************************************************************************/
void SerialportPanel::RefreshDevice()
{
    /*
     * 清空预选框
     */
    m_pSerialportComboBox->clear();

    QList<QSerialPortInfo> serialPortInfosTemp =  QSerialPortInfo::availablePorts();
    if(serialPortInfosTemp.isEmpty())
    {
        m_pSerialportComboBox->clear();

        /*
         * 以下两行代码获取Qt内置图标（warning图标,黄色三角形加一个黑色感叹号）
         */
        QStyle *pStyle = QApplication::style();
        QIcon iconTemp = pStyle->standardIcon(QStyle::SP_MessageBoxWarning);

        m_pSerialportComboBox->addItem(iconTemp, tr("未检测到可用设备"));
    }
    else{
        m_pSerialportComboBox->clear();
        QString strTemp;
        strTemp.clear();
        for(int index = 0; index < serialPortInfosTemp.count(); index++)
        {
            strTemp = serialPortInfosTemp.at(index).portName();     //串口名称，如COM1
            strTemp += " - ";
            strTemp += serialPortInfosTemp.at(index).description(); //串口描述
            m_pSerialportComboBox->addItem(strTemp);
        }
    }

    /*
     * 在信息显示框输出操作信息
     */
    QString infoString = QTime::currentTime().toString(QString("hh:mm:ss"));
    infoString += tr("<font color=blue> 刷新串口设备完成</font>");
    m_pTextBrowser->clear();
    m_pTextBrowser->setText(infoString);
}


/*******************************************************************************
Function: OpenCloseSerialport()
Description: 打开/关闭按钮
Calls: null
Called By: “打开串口”按钮被点击时执行该函数
Input: null
Output: null
Return: null
Others: not yet
History：
    01. LittleBoy/2017-09-27 创建并实现函数
*******************************************************************************/
void SerialportPanel::OpenCloseSerialport()
{
    bool isOK;       //该变量用于各种逻辑判断
    QString strTemp; //各种临时字符串

    /*
     * 静态bool变量，作为串口打开标志位，true表示串口正处于打开状态，false表示串口正处于关闭状
     * 态。该变量初始化为false，表示一开始串口是关闭的
     */
    static bool bIsOpened = false;

    /*
     * 如果串口处于打开状态，则关闭串口，并更改按钮显示文字，同时输出提示信息。
     */
    if (bIsOpened)
    {
        m_pSerialport->close();
        /*
         * 在信息显示框输出操作信息
         */
        QString infoString = QTime::currentTime().toString(QString("hh:mm:ss"));
        infoString += tr("<font color=blue> 串口已关闭！</font>");
        m_pTextBrowser->clear();
        m_pTextBrowser->setText(infoString);
        m_pOpenSerialportPBt->setText(tr("打开串口"));//切换文本

        bIsOpened = !bIsOpened;//更新串口状态

#ifdef DISABLE_COMBOBOX_AND_REFRESH_BUTTON_WHEN_SERIALPORT_IS_OPEN
        //预选框使能
        m_pSerialportComboBox->setEnabled(true);
        m_pBaudRateComboBox->setEnabled(true);
        m_pRefreshPBt->setEnabled(true);
        m_pDataBitsComboBox->setEnabled(true);
        m_pStopBitsComboBox->setEnabled(true);
        m_pParityComboBox->setEnabled(true);
#endif

        //发射串口关闭信号
        emit SerialportHadClosed();

        return;
    }


    /*
     * 如果串口处于关闭状态，则打开串口，并设置串口名称、波特率、数据位、停止位、奇偶校验位、输
     * 出相关提示信息等
     */
    else
    {
        //串口名称
        m_pSerialport->setPortName(m_pSerialportComboBox->currentText().split(' ').at(0));
        //波特率
        m_pSerialport->setBaudRate(m_pBaudRateComboBox->currentText().toInt(&isOK, 10));
        if (!isOK)//波特率设置失败，在信息显示框输出操作信息
        {
            QString infoString = QTime::currentTime().toString(QString("hh:mm:ss"));
            infoString += tr("<font color=red> %1</font>").arg(m_pSerialport->errorString());
            m_pTextBrowser->clear();
            m_pTextBrowser->setText(infoString);
            return;
        }
        //数据位
        strTemp = m_pDataBitsComboBox->currentText();
        if (strTemp == "5")
        {
            m_pSerialport->setDataBits(QSerialPort::Data5);
        }
        else if (strTemp == "6")
        {
            m_pSerialport->setDataBits(QSerialPort::Data6);
        }
        else if (strTemp == "7")
        {
            m_pSerialport->setDataBits(QSerialPort::Data7);
        }
        else if (strTemp == "8")
        {
            m_pSerialport->setDataBits(QSerialPort::Data8);
        }
        else
        {
            m_pSerialport->setDataBits(QSerialPort::UnknownDataBits);
            m_pSerialport->setStopBits(QSerialPort::UnknownStopBits);
            QString infoString = QTime::currentTime().toString(QString("hh:mm:ss"));
            infoString += tr("<font color=red> 未知数据位</font>");
            m_pTextBrowser->clear();
            m_pTextBrowser->setText(infoString);
            return;
        }
        //停止位
        strTemp = m_pStopBitsComboBox->currentText();
        if (strTemp == "1")
        {
            m_pSerialport->setStopBits(QSerialPort::OneStop);
        }
        else if (strTemp == "1.5")
        {
            m_pSerialport->setStopBits(QSerialPort::OneAndHalfStop);
        }
        else if (strTemp == "2")
        {
            m_pSerialport->setStopBits(QSerialPort::TwoStop);
        }
        else
        {
            m_pSerialport->setStopBits(QSerialPort::UnknownStopBits);
            QString infoString = QTime::currentTime().toString(QString("hh:mm:ss"));
            infoString += tr("<font color=red> 未知停止位</font>");
            m_pTextBrowser->clear();
            m_pTextBrowser->setText(infoString);
            return;
        }

        //奇偶校验位
        strTemp = m_pParityComboBox->currentText();
        if (strTemp == "None")
        {
            m_pSerialport->setParity(QSerialPort::NoParity);
        }
        else if (strTemp == "Even")
        {
            m_pSerialport->setParity(QSerialPort::EvenParity);
        }
        else if (strTemp == "Odd")
        {
            m_pSerialport->setParity(QSerialPort::OddParity);
        }
        else if (strTemp == "Space")
        {
            m_pSerialport->setParity(QSerialPort::EvenParity);
        }
        else if (strTemp == "Mark")
        {
            m_pSerialport->setParity(QSerialPort::OddParity);
        }
        else
        {
            m_pSerialport->setParity(QSerialPort::UnknownParity);
            m_pSerialport->setStopBits(QSerialPort::UnknownStopBits);
            QString infoString = QTime::currentTime().toString(QString("hh:mm:ss"));
            infoString += tr("<font color=red> 未知校验位</font>");
            m_pTextBrowser->clear();
            m_pTextBrowser->setText(infoString);
            return;
        }

        /*
         * 打开串口
         */
        isOK = m_pSerialport->open(QIODevice::ReadWrite);
        if (!isOK)//串口打开失败，在信息显示框输出操作信息
        {
            QString infoString = QTime::currentTime().toString(QString("hh:mm:ss"));
            infoString += tr("<font color=red> %1</font>").arg(m_pSerialport->errorString());
            m_pTextBrowser->clear();
            m_pTextBrowser->setText(infoString);
            return;
        }
        else//串口打开成功，在信息显示框输出操作信息
        {
            QString infoString = QTime::currentTime().toString(QString("hh:mm:ss"));
            infoString += tr("<font color=blue> 串口已打开\n</font>");
            m_pTextBrowser->clear();
            m_pTextBrowser->setText(infoString);

            infoString = tr("<font color=blue> 端口号：</font>%1").arg(m_pSerialport->portName());
            m_pTextBrowser->append(infoString);
            infoString = tr("<font color=blue> 波特率：</font>%1").arg(m_pSerialport->baudRate());
            m_pTextBrowser->append(infoString);
            infoString = tr("<font color=blue> 数据位：</font>%1").arg(m_pSerialport->dataBits());
            m_pTextBrowser->append(infoString);
            infoString = tr("<font color=blue> 停止位：</font>%1").arg(m_pSerialport->stopBits());
            m_pTextBrowser->append(infoString);
            infoString = tr("<font color=blue> 奇偶位：</font>%1").arg(m_pSerialport->parity());
            m_pTextBrowser->append(infoString);

            m_pOpenSerialportPBt->setText(tr("关闭串口"));

            bIsOpened = !bIsOpened;//更新串口状态

#ifdef DISABLE_COMBOBOX_AND_REFRESH_BUTTON_WHEN_SERIALPORT_IS_OPEN
            //禁用预选框
            m_pSerialportComboBox->setEnabled(false);
            m_pBaudRateComboBox->setEnabled(false);
            m_pRefreshPBt->setEnabled(false);
            m_pDataBitsComboBox->setEnabled(false);
            m_pStopBitsComboBox->setEnabled(false);
            m_pParityComboBox->setEnabled(false);
#endif

            //发射串口打开信号
            emit SerialportHadOpened();
        }
    }
}

/*******************************************************************************
Function: QSerialPort()
Description: 返回该控件的串口地址，使用该控件（SerialportPanel类）时，用此函数获得一个串口类
             指针，该指针指向的串口可以通过本控件(SerialportPanel)来进行图形化操作。
Calls: null
Called By: unknown
Input: null
Output: null
Return: null
Others: not yet
History：
    01. LittleBoy/2017-09-27 创建并实现函数
*******************************************************************************/
QSerialPort *SerialportPanel::GetSerialport()
{
    return m_pSerialport;
}
