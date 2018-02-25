/********************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: Main.cpp
Version: V0.0.0/LittleBoy/2017-09-22
Description: 项目程序入口
Others: not yet
Function List:
    01. main()  程序入口
History:
    01. LittleBoy/2017-09-22
        Create the file.
********************************************************************************/


#include <QApplication>

#include "../Integration/Integration.h"
#include "Global.h"

int main(int argc, char *argv[])
{
    QApplication *app = new QApplication(argc, argv);

    GlobaParameterInit();

    SerialportAssistant *assistant = new SerialportAssistant;
    assistant->show();

    return app->exec();
}
