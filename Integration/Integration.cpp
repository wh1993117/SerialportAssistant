/********************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: Integration.cpp
Version: V0.0.0/LittleBoy/2017-11-12
Description: 整合各个模块
Others: not yet
Function List:
    01. SerialportAssistant() 构造函数
    02. ~SerialportAssistant() 析构函数
    03. ReadData() 读取串口数据
    04. CreateMenu() 创建菜单
    05. AboutQt() 弹出“关于Qt”对话框
    06. AboutSoftware() 弹出“关于软件”对话框
    07. HelpManual() 打开帮助手册
    08. MyBlog() 打开博客链接
    09. GitBub() 打开GitHub链接
    10. UpdateDataCountArea() 更新数据统计面板数据
    11. WindowsInit() 初始化窗口，包括标题、图标、窗口大小等
    12. OpenFile() 打开数据文件，被打开文件中的文本将作为数据通过串口发送
    13. UpdateHistory() 查看软件发布修订记录
History:
    01. LittleBoy/2017-11-12
        Create the file.
********************************************************************************/

#include "Integration.h"
#include "../VersionInfo/Info.h"
#include "../VersionInfo/Version.h"
#include "../Main/Global.h"

#include <QGridLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFile>

/*
 * 01. 构造函数
 */
SerialportAssistant::SerialportAssistant(QWidget *parent):QMainWindow(parent)
{
    /*
     * 创建各独立模块
     */
    m_pDataCountArea = new DataCountArea;
    m_pDataCountArea->setMaximumWidth(120);
    m_pDataCountArea->setMaximumHeight(270);
    m_pInfoArea = new InfoArea;
    m_pSerialportPanel = new SerialportPanel(g_SerialPort);
    m_pSerialportPanel->setMaximumHeight(150);

    /*
     * 初始化串口，关联读就绪信号
     */
    m_pSerialport = m_pSerialportPanel->GetSerialport();    // 此处返回的指针指向（*g_SerialPort）
    connect(m_pSerialport, SIGNAL(readyRead()),
            this, SLOT(ReadData()));

    m_pSendArea = new SendArea(m_pSerialport);
    connect(m_pSendArea, SIGNAL(SendDatas(int,QByteArray)),
            this, SLOT(UpdateDataCountArea(int,QByteArray)));
    m_pSendArea->setMaximumHeight(150);
    m_pSendArea->show();

    /*
     * 栅格布局
     */
    QGridLayout *pGridLayout = new QGridLayout;
    pGridLayout->addWidget(m_pDataCountArea, 0, 0, 1, 1);
    pGridLayout->addWidget(m_pInfoArea, 0, 1, 2, 1);
    pGridLayout->addWidget(m_pSerialportPanel, 3, 0, 1, 2);
    pGridLayout->addWidget(m_pSendArea, 4, 0, 1, 2);
    QWidget *pCentralWidget = new QWidget;
    pCentralWidget->setLayout(pGridLayout);
    this->setCentralWidget(pCentralWidget);

    /*
     * 初始化窗口
     */
    WindowsInit();

    /*
     * 创建菜单
     */
    CreateMenu();
}

/*
 * 02. 析构函数
 */
SerialportAssistant::~SerialportAssistant()
{    
    // not yet
}

/*******************************************************************************
Number：03
Function: ReadData()
Description: 读串口数据
Calls:
Called By: 串口接收到数据时执行该函数
Return: void
Others: not yet
History：
    01. LittleBoy/2017-11-13 创建并部分实现函数
*******************************************************************************/
void SerialportAssistant::ReadData()
{
    QByteArray ByteArrayTemp;

    while(m_pSerialport->waitForReadyRead(80)); // 等待数据发送完成
    ByteArrayTemp = m_pSerialport->readAll();
    m_pInfoArea->ShowData(&ByteArrayTemp, RECEIVE_DATA);
    m_pDataCountArea->UpdateDataCount(ByteArrayTemp,UPDATE_RECEIVE_DATA_FLAG);
}

/*******************************************************************************
Number：04
Function: CreateMenu()
Description: 创建菜单
Calls:
Called By: 构造函数
Return: void
Others: not yet
History：
    01. LittleBoy/2017-12-04 创建函数并实现函数功能
*******************************************************************************/
void SerialportAssistant::CreateMenu()
{
    /*
     * 文件菜单，包含退出等动作
     */
    m_pFileMenu = new QMenu(tr("文件"));

    m_pOpenAction = new QAction(tr("打开"));
    m_pFileMenu->addAction(m_pOpenAction);
    connect(m_pOpenAction, SIGNAL(triggered(bool)),
            this, SLOT(OpenFile()));

    m_pExitAction = new QAction(tr("退出"));
    m_pFileMenu->addAction(m_pExitAction);
    connect(m_pExitAction, SIGNAL(triggered(bool)),
            this, SLOT(close()));    

    this->menuBar()->addMenu(m_pFileMenu);

    /*
     * 帮助菜单，包含关于Qt、关于软件等动作
     */
    m_pAboutQtAction = new QAction(tr("关于Qt"));
    connect(m_pAboutQtAction, SIGNAL(triggered(bool)),
            this, SLOT(AboutQt()));
    m_pAboutSoftwareAction = new QAction(tr("关于软件"));
    connect(m_pAboutSoftwareAction, SIGNAL(triggered(bool)),
            this, SLOT(AboutSoftware()));

    m_pHelpManual = new QAction(tr("帮助手册"));
    connect(m_pHelpManual, SIGNAL(triggered(bool)),
             this, SLOT(HelpManual()));

    m_pMyBlog = new QAction(tr("我的博客"));
    connect(m_pMyBlog, SIGNAL(triggered(bool)),
             this, SLOT(MyBlog()));

    m_pGitBubAction = new QAction(tr("GitHub"));
    connect(m_pGitBubAction, SIGNAL(triggered(bool)),
             this, SLOT(GitBub()));

    m_pUpdateHistory = new QAction(tr("更新记录"));
    connect(m_pUpdateHistory, SIGNAL(triggered(bool)),
            this, SLOT(UpdateHistory()));

    m_pHelpMenu = new QMenu(tr("帮助"));
    m_pHelpMenu->addAction(m_pAboutQtAction);   
    m_pHelpMenu->addAction(m_pAboutSoftwareAction);
    m_pHelpMenu->addAction(m_pHelpManual);
    m_pHelpMenu->addAction(m_pMyBlog);
    m_pHelpMenu->addAction(m_pGitBubAction);
    m_pHelpMenu->addAction(m_pUpdateHistory);
    this->menuBar()->addMenu(m_pHelpMenu);

}

/*******************************************************************************
Number：05
Function: AboutQt()
Description: “关于Qt”对话框
Calls:
Called By: “关于Qt”菜单选项被触发时，执行该函数
Return: void
Others: not yet
History：
    01. LittleBoy/2017-12-04
*******************************************************************************/
void SerialportAssistant::AboutQt()
{
    QMessageBox::aboutQt(this, tr("关于Qt"));
}


/*******************************************************************************
Number：06
Function: AboutSoftware()
Description: “关于软件”对话框
Calls:
Called By: “关于软件”菜单选项被触发时，执行该函数,该函数弹出一个与软件相关信息窗口
Return: void
Others: not yet
History：
    01. LittleBoy/2017-12-06
*******************************************************************************/
void SerialportAssistant::AboutSoftware()
{
    QMessageBox::information(this, tr("关于软件"), tr("软件作者：%1\n"
                                               "电子邮箱：%2\n"
                                               "博客地址：%3\n"
                                               "版本信息：%4\n"
                                               "编译时间：%5%6%7\n"
                                               "版权信息：%9\n"
                                               "\n"
                                               "GitHub：%8\n").arg(AUTHOR).arg(E_MAIL)
                       .arg(MYBLOG).arg(VERSION).arg(g_BuildDate.toString("yyyy-MM-dd"))
                       .arg(' ', 2).arg(g_BuildTime.toString("hh:mm:ss")).arg(GITHUB)
                       .arg(COPYRIGHT));
}

/*******************************************************************************
Number：07
Function: HelpManual()
Description: 帮助手册(预留功能)
Calls:
Called By: “帮助手册”菜单选项被触发时，执行该函数,该函数弹出本软件的帮助手册
Return: void
Others: not yet
History：
    01. LittleBoy/2017-12-06
*******************************************************************************/
void SerialportAssistant::HelpManual()
{

}

/*******************************************************************************
Number：08
Function: MyBlog()
Description: 打开博客链接
Calls:
Called By: “博客地址”菜单选项被触发时，执行该函数,该函数将转博客页面
Return: void
Others: not yet
History：
    01. LittleBoy/2017-12-06
*******************************************************************************/
void SerialportAssistant::MyBlog()
{
    QDesktopServices::openUrl(QUrl(MYBLOG));
}

/*******************************************************************************
Number：09
Function: GitBub()
Description: 打开GitHub链接
Calls:
Called By: “GitHub”菜单选项被触发时，执行该函数,该函数跳转至本软件源码GitHub页面
Return: void
Others: not yet
History：
    01. LittleBoy/2017-12-06
*******************************************************************************/
void SerialportAssistant::GitBub()
{
    QDesktopServices::openUrl(QUrl(GITHUB));
}

/*******************************************************************************
Number：10
Function: UpdateDataCountArea()
Description: 更新数据统计面板数据显示
Calls:
Called By: 该函数由串口发送数据时发射的信号触发
Return: void
Others: not yet
History：
    01. LittleBoy/2017-12-15
*******************************************************************************/
void SerialportAssistant::UpdateDataCountArea(int mode, QByteArray array)
{
    switch (mode)
    {
    case MODE_CHAR:
        m_pDataCountArea->SendDdataAddOne();
        break;
    case MODE_BYTE_ARRAY:
        m_pDataCountArea->UpdateDataCount(array, UPDATE_SEND_DATA_FLAG);
        break;
    default:
        break;
    }
}

/*******************************************************************************
Number：11
Function: WindowsInit()
Description: 窗口初始化，包括大小，标题、图标等
Calls:
Called By: 构造函数
Return: void
Others: not yet
History：
    01. LittleBoy/2017-12-18
        创建并实现函数
*******************************************************************************/
void SerialportAssistant::WindowsInit()
{
    setWindowTitle(tr("串口助手"));
    setWindowIcon(QIcon(WINDOWS_ICON));
}

/*******************************************************************************
Number：12
Function: OpenFile()
Description: 打开文件
Calls:
Called By:
Return: void
Others: not yet
History：
    02. LittleBoy/2017-12-25 新增AllFile(*.*) 过滤器
    01. LittleBoy/2017-12-25 创建并实现函数
*******************************************************************************/
void SerialportAssistant::OpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开数据文件"),".",
                                                    "TextFile(*.txt);;DataFile(*.data);;AllFile(*.*)");
    if (!fileName.isEmpty())
    {
        QFile dataFile(fileName);
        bool isOK = dataFile.open(QFile::ReadWrite);
        if (isOK)
        {
            m_pSendArea->SetText(QString(dataFile.readAll()));
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
Number：13
Function: UpdateHistory()
Description: 查看软件的发布修订记录
Calls:
Called By:
Return: void
Others: not yet
History：
    01. LittleBoy/2017-12-26
        创建并实现函数
*******************************************************************************/
void SerialportAssistant::UpdateHistory()
{
    QFile note(":/release_note.txt");
    bool isOK = note.open(QFile::ReadOnly);
    if (isOK)
    {
        QMessageBox::information(this, tr("发行信息"), QString(note.readAll()));
        note.close();
    }
    else
    {
        QMessageBox::warning(this, tr("错误警告"), note.errorString());
        note.close();
    }
}
