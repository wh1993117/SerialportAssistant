HEADERS += \
    SerialportPanel/SerialportPanel.h \
    DataCountArea/DataCountArea.h \
    InfoArea/InfoArea.h \
    SendArea/SendArea.h \
    SendArea/HelpDialog.h \
    VersionInfo/Info.h \
    VersionInfo/Version.h \
    VersionInfo/Info.h \
    Integration/Integration.h \
    Main/Global.h \
    Integration/Icon.h

SOURCES += \
    SerialportPanel/SerialportPanel.cpp \
    Main/Main.cpp \
    DataCountArea/DataCountArea.cpp \
    InfoArea/InfoArea.cpp \
    InfoArea/InfoArea_ShowData.cpp \
    SendArea/SendArea.cpp \
    SendArea/HelpDialog.cpp \
    Integration/Integration.cpp \
    Main/Global.cpp

QT      += \
    widgets\
    serialport

RESOURCES += \
    Images/icon.qrc \
    Text/text.qrc

DISTFILES += \
    Developer/DevelopLog.log

RC_FILE += \
    Script/icon.rc
