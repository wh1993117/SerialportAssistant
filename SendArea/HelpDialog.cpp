/*******************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: HelpDialog.cpp
Version: V0.0.0/LittleBoy/2017-10-08
Description: "说明"按钮点击对话框(帮助对话框)
Others: 2017-11-03 今天和瘦浪单挑输了，我用盖伦，他王子
Function List:
    01. HelpDialog() 构造函数
    02. ~HelpDialog()析构函数
History:
    01. LittleBoy/2017-10-08~2017-10-
        Create the file.
*******************************************************************************/
#include "HelpDialog.h"

#include <QFile>
#include <QTextBrowser>
#include <QGridLayout>
#include <QMessageBox>
/*
 * 本控件的构造函数
 */
HelpDialog::HelpDialog(QWidget *parent):QDialog(parent)
{
    /*
     * 初始化布局等
     */
    QTextBrowser *showInstruction = new QTextBrowser;
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(showInstruction, 0, 1);
    this->setLayout(layout);

    /*
     * 设置文本
     */
    QFile manual(":/manual.txt");
    bool isOK = manual.open(QFile::ReadOnly);   // 文件是资源文件，打开方式为“读写”时将出错
    if (isOK)
    {
        showInstruction->setText(QString(manual.readAll()));
        manual.close();
    }
    else
    {
        QMessageBox::warning(this, tr("打开文件错误提示"), manual.errorString());
        manual.close();
    }
}

/*
 * 本控件的析构函数
 */
HelpDialog::~HelpDialog()
{

}
