/*******************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: Integration.h
Version: V0.0.0/LittleBoy/2017-09-22
Description: 整合各个模块
Others: not yet
History:
    01. LittleBoy/2017-11-12
        创建本文档

*******************************************************************************/
#ifndef __INTEGRATION_H
#define __INTEGRATION_H

#include "../DataCountArea/DataCountArea.h"
#include "../InfoArea/InfoArea.h"
#include "../SendArea/SendArea.h"
#include "../SerialportPanel/SerialportPanel.h"
#include "Icon.h"

#include <QMainWindow>
#include <QMenu>
#include <QAction>

class SerialportAssistant;
class SerialportAssistant:public QMainWindow
{
    Q_OBJECT
public:
    SerialportAssistant(QWidget *parent = 0);   // 该类的构造函数
    ~SerialportAssistant();                  // 该类的析构函数

    QSerialPort *m_pSerialport;                 // 串口
    DataCountArea *m_pDataCountArea;            // 串口数据计数模块
    InfoArea *m_pInfoArea;                      // 信息显示模块
    SendArea *m_pSendArea;                      // 发送控件模块
    SerialportPanel *m_pSerialportPanel;        // 串口操作面板模块
private:
    /*
     * “文件”菜单
     */
    QMenu *m_pFileMenu;                         // “文件”菜单
    QAction *m_pExitAction;                     // "退出"动作
    QAction *m_pOpenAction;                     // "打开"菜单选项

    /*
     * “帮助”菜单
     */
    QMenu *m_pHelpMenu;                         // "帮助"菜单
    QAction *m_pAboutQtAction;                  // "关于Qt"菜单项
    QAction *m_pAboutSoftwareAction;            // "关于软件"菜单项
    QAction *m_pHelpManual;                     // "帮助手册"菜单项
    QAction *m_pGitBubAction;                   // "GitHub链接"菜单项
    QAction *m_pMyBlog;                         // "我的博客"菜单项
    QAction *m_pUpdateHistory;                  // "更新记录"菜单项

    void WindowsInit();                         // 初始化窗口
    void CreateMenu();
private slots:
    void OpenFile();
    void ReadData();                            // 读串口数据
    void AboutQt();                             // 关于Qt，信息说明
    void AboutSoftware();                       // 软件信息
    void HelpManual();                          // 帮助手册
    void MyBlog();                              // 打开博客链接
    void GitBub();                              // 打开GitHub链接
    void UpdateHistory();                       // 查看软件发行更新记录

    void UpdateDataCountArea(int mode, QByteArray array); // 更新数据统计面板数据

};

#endif
