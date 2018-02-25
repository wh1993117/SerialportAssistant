/*******************************************************************************
Encoding: Encoding With UTF-8
Copyright (C), 1993-2017,LittleBoy.
File name: InfoArea.cpp
Version: V0.0.0/LittleBoy/2017-10-02
Description:  信息显示区
Others: not yet
Qt Version: Qt 5.6.3 MinGW492_32
Function List:
    01. InfoArea()
    02. ~InfoArea()
    03. ShowData() 显示数据（函数很长，已在独立文档中实现）
    04. PausePBtClicked()
    05. ClearInfoPBtClicked()
    06. HexCBoxClicked()
    07. DecCBoxClicked()
    08. OctCBoxClicked()
    09. BinCBoxClicked()

History:
    01. LittleBoy/2017-10-02~2017-10-05
        Create the file.
*******************************************************************************/

#include "InfoArea.h"
#include <QGridLayout>

/*
 * 构造函数
 * 历史版本：
 * 02. LittleBoy/2017-11-23
 *     “显示时间”、“显示日期”复选框默认不勾选（原来默认勾选）
 *     默认显示方式改为“ASCII”模式，（原来默认为16进制模式）
 * 01. LittleBoy/2017-10-05
 *     创建函数，实现函数功能
 */
InfoArea::InfoArea(QWidget *parent):QGroupBox(parent)
{
    // 数据成员初始化
    m_bPauseFlag = false;

    m_pInfoShowArea = new QTextBrowser;
    m_pPausePBt = new QPushButton(tr("暂停显示"));
    connect(m_pPausePBt, SIGNAL(clicked(bool)),
            this, SLOT(PausePBtClicked()));

    m_pClearInfoPBt = new QPushButton(tr("清空显示"));
    connect(m_pClearInfoPBt, SIGNAL(clicked(bool)),
            this, SLOT(ClearInfoPBtClicked()));

    m_pShowDateCBox = new QCheckBox(tr("显示日期"));
    m_pShowDateCBox->setChecked(false);

    m_pShowTimeCBox = new QCheckBox(tr("显示时间"));
    m_pShowTimeCBox->setChecked(false);

    m_pCountByteCBox = new QCheckBox(tr("统计字节数"));
    m_pCountByteCBox->setChecked(false);

    m_pHexCBox = new QCheckBox(tr("十六进制"));
    m_pHexCBox->setChecked(false);
    connect(m_pHexCBox, SIGNAL(clicked(bool)),
            this, SLOT(HexCBoxClicked()));

    m_pDecCBox = new QCheckBox(tr("十进制"));
    m_pDecCBox->setChecked(false);
    connect(m_pDecCBox, SIGNAL(clicked(bool)),
            this, SLOT(DecCBoxClicked()));

    m_pOctCBox = new QCheckBox(tr("八进制"));
    m_pOctCBox->setChecked(false);
    connect(m_pOctCBox, SIGNAL(clicked(bool)),
            this, SLOT(OctCBoxClicked()));

    m_pBinCBox = new QCheckBox(tr("二进制"));
    m_pBinCBox->setChecked(false);
    connect(m_pBinCBox, SIGNAL(clicked(bool)),
            this, SLOT(BinCBoxClicked()));

    m_pASCIICBox = new QCheckBox(tr("ASCII"));
    m_pASCIICBox->setChecked(true);
    connect(m_pASCIICBox, SIGNAL(clicked(bool)),
            this, SLOT(ASCIICBoxClicked()));

    // 布局
    QGridLayout *pGridLayout = new QGridLayout;    
    pGridLayout->addWidget(m_pInfoShowArea, 0, 0, 1, 9);
    pGridLayout->addWidget(m_pShowDateCBox, 1, 0, 1, 1);
    pGridLayout->addWidget(m_pShowTimeCBox, 1, 1, 1, 1);
    pGridLayout->addWidget(m_pHexCBox, 1, 2, 1, 1);
    pGridLayout->addWidget(m_pDecCBox, 1, 3, 1, 1);
    pGridLayout->addWidget(m_pOctCBox, 1, 4, 1, 1);
    pGridLayout->addWidget(m_pBinCBox, 1, 5, 1, 1);
    pGridLayout->addWidget(m_pASCIICBox, 1, 6, 1, 1);
    pGridLayout->addWidget(m_pPausePBt, 1, 7, 1, 1);
    pGridLayout->addWidget(m_pClearInfoPBt, 1, 8, 1, 1);
    pGridLayout->addWidget(m_pCountByteCBox, 2, 0, 1, 1);

    this->setTitle(tr("信息显示区"));
    this->setLayout(pGridLayout);
}

/*
 * 析构函数
 * 历史版本：
 * 01. LittleBoy/2017-10-05 创建函数，实现函数功能
 */
InfoArea::~InfoArea()
{

}

/*
 * 该函数已在“InfoArea_ShowData.cpp”中
 */
//void InfoArea::ShowData(QByteArray *array, int flag)
//{
    //
//}

/*
 * 暂停显示 按钮点击槽函数
 * 历史版本：
 * 01. LittleBoy/2017-10-05 创建函数，实现函数功能
 */
void InfoArea::PausePBtClicked()
{
    m_bPauseFlag = !m_bPauseFlag;

    if (m_bPauseFlag)
    {
        m_pPausePBt->setText(tr("继续显示"));
    }
    else
    {
        m_pPausePBt->setText(tr("暂停显示"));
    }
}

/*
 * 清空显示 按钮点击槽函数
 * 历史版本：
 * 01. LittleBoy/2017-10-05 创建函数，实现函数功能
 */
void InfoArea::ClearInfoPBtClicked()
{
    m_pInfoShowArea->clear();
}

/*
 * 十六进制 复选框点击槽函数
 * 历史版本：
 * 01. LittleBoy/2017-10-05 创建函数，实现函数功能
 */
void InfoArea::HexCBoxClicked()
{
    if(m_pHexCBox->isChecked())
    {
        m_pDecCBox->setChecked(false);
        m_pOctCBox->setChecked(false);
        m_pBinCBox->setChecked(false);
        m_pASCIICBox->setChecked(false);
    }
    else
    {
        m_pHexCBox->setChecked(true);
    }
}

/*
 * 十进制 复选框点击槽函数
 * 历史版本：
 * 01. LittleBoy/2017-10-05 创建函数，实现函数功能
 */
void InfoArea::DecCBoxClicked()
{
    if(m_pDecCBox->isChecked())
    {
        m_pHexCBox->setChecked(false);
        m_pOctCBox->setChecked(false);
        m_pBinCBox->setChecked(false);
        m_pASCIICBox->setChecked(false);
    }
    else
    {
        m_pDecCBox->setChecked(true);
    }
}

/*
 * 八进制 复选框点击槽函数
 * 历史版本：
 * 01. LittleBoy/2017-10-05 创建函数，实现函数功能
 */
void InfoArea::OctCBoxClicked()
{
    if(m_pOctCBox->isChecked())
    {
        m_pHexCBox->setChecked(false);
        m_pDecCBox->setChecked(false);
        m_pBinCBox->setChecked(false);
        m_pASCIICBox->setChecked(false);
    }
    else
    {
        m_pOctCBox->setChecked(true);
    }
}

/*
 * 二进制 复选框点击槽函数
 * 历史版本：
 * 01. LittleBoy/2017-10-05 创建函数，实现函数功能
 */
void InfoArea::BinCBoxClicked()
{
    if(m_pBinCBox->isChecked())
    {
        m_pHexCBox->setChecked(false);
        m_pDecCBox->setChecked(false);
        m_pOctCBox->setChecked(false);
        m_pASCIICBox->setChecked(false);
    }
    else
    {
        m_pBinCBox->setChecked(true);
    }
}

/*
 * 二进制 复选框点击槽函数
 * 历史版本：
 * 01. LittleBoy/2017-10-05 创建函数，实现函数功能
 */
void InfoArea::ASCIICBoxClicked()
{
    if (m_pASCIICBox->isChecked())
    {
        m_pHexCBox->setChecked(false);
        m_pDecCBox->setChecked(false);
        m_pOctCBox->setChecked(false);
        m_pBinCBox->setChecked(false);
    }
    else
    {
        m_pASCIICBox->setChecked(true);
    }
}
