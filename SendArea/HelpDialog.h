/*******************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: HelpDialog.h
Version: V0.0.0/LittleBoy/2017-11-03
Description:  "说明"按钮点击对话框(帮助对话框)
Others: not yet
History:
    01. LittleBoy/2017-11-03
        创建本文档

*******************************************************************************/

#ifndef __HELP_DIALOG_H
#define __HELP_DIALOG_H

#include <QDialog>
#include <QWidget>

class HelpDialog;
class HelpDialog:public QDialog
{
    Q_OBJECT
public:
    HelpDialog(QWidget *parent = 0);
    ~HelpDialog();
};


#endif
