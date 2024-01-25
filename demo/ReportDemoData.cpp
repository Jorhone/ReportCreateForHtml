#include "ReportDemoData.h"
#include "HtmlForCSS.h"

void CReportDemoData::AddDataAboutHome(CHtmlForCSS *vHtmlForCSS)
{
    if(vHtmlForCSS == nullptr)
        return;

    vHtmlForCSS->AddHtmlData4Wrap();
    vHtmlForCSS->AddHtmlData4Wrap();

    //添加报告标题
    AddTitleData2Home(vHtmlForCSS);

    //添加报告类型
    AddReportTypeData2Home(vHtmlForCSS);

    //添加报告编号
    AddReportNumberData2Home(vHtmlForCSS);

    vHtmlForCSS->AddHtmlData4Wrap();
    vHtmlForCSS->AddHtmlData4Wrap();

    //添加报告基础信息
    AddBaseInfoData2Home(vHtmlForCSS);

    vHtmlForCSS->AddHtmlData4Paging();
}

void CReportDemoData::AddTitleData2Home(CHtmlForCSS *vHtmlForCSS)
{
    if(vHtmlForCSS == nullptr)
        return;

    QString tReportTitle = "报告模块_内部测试";
    QString tReportTitleSplit = "_";
    if(!tReportTitleSplit.isEmpty())
    {
        QStringList tTitleDataList = tReportTitle.split(tReportTitleSplit);
        tReportTitle.clear();

        for(auto tTitleData: tTitleDataList)
        {
            if(tTitleData.isEmpty())
                continue;

            if(!tReportTitle.isEmpty())
                tReportTitle += "\n";

            tReportTitle += tTitleData;
        }
    }

    nsHtmlForCSS::CParagraphAttribute tParagraphAttribute;
    tParagraphAttribute.m_FontAttribute.m_FontSize = 36;
    tParagraphAttribute.m_FontAttribute.m_LengthType = nsHtmlForCSS::e_Length_PT;
    tParagraphAttribute.m_FontAttribute.m_FontWeightType = nsHtmlForCSS::e_FontWeight_Bold;
    tParagraphAttribute.m_TextAttribute.m_AlignType = nsHtmlForCSS::e_Align_Center;

    uint32_t tCSSAttributeIndex = vHtmlForCSS->SetCSSAttribute(tParagraphAttribute);
    vHtmlForCSS->AddHtmlData4Paragraph(tReportTitle, tCSSAttributeIndex);
}

void CReportDemoData::AddReportTypeData2Home(CHtmlForCSS *vHtmlForCSS)
{
    if(vHtmlForCSS == nullptr)
        return;

    QString tReportTypeSTR = "调  试  报  告";

    nsHtmlForCSS::CParagraphAttribute tParagraphAttribute;
    tParagraphAttribute.m_FontAttribute.m_FontSize = 40;
    tParagraphAttribute.m_FontAttribute.m_LengthType = nsHtmlForCSS::e_Length_PT;
    tParagraphAttribute.m_FontAttribute.m_FontWeightType = nsHtmlForCSS::e_FontWeight_Bold;
    tParagraphAttribute.m_TextAttribute.m_AlignType = nsHtmlForCSS::e_Align_Center;

    uint32_t tCSSAttributeIndex = vHtmlForCSS->SetCSSAttribute(tParagraphAttribute);
    vHtmlForCSS->AddHtmlData4Paragraph(tReportTypeSTR, tCSSAttributeIndex);
}

void CReportDemoData::AddReportNumberData2Home(CHtmlForCSS *vHtmlForCSS)
{
    if(vHtmlForCSS == nullptr)
        return;

    QString tReportNumberSTR = QString("编号：%1").arg(m_ReportDateTime.currentMSecsSinceEpoch());

    nsHtmlForCSS::CParagraphAttribute tParagraphAttribute;
    tParagraphAttribute.m_FontAttribute.m_FontSize = 10;
    tParagraphAttribute.m_FontAttribute.m_LengthType = nsHtmlForCSS::e_Length_PT;
    tParagraphAttribute.m_FontAttribute.m_FontWeightType = nsHtmlForCSS::e_FontWeight_Bold;
    tParagraphAttribute.m_TextAttribute.m_AlignType = nsHtmlForCSS::e_Align_Center;

    uint32_t tCSSAttributeIndex = vHtmlForCSS->SetCSSAttribute(tParagraphAttribute);
    vHtmlForCSS->AddHtmlData4Paragraph(tReportNumberSTR, tCSSAttributeIndex);
}

void CReportDemoData::AddBaseInfoData2Home(CHtmlForCSS *vHtmlForCSS)
{
    if(vHtmlForCSS == nullptr)
        return;

    //表格属性
    nsHtmlForCSS::CTableAttribute tTableAttribute;
    tTableAttribute.m_WidthRange = 80;
    uint32_t tTableCSSAttributeIndex = vHtmlForCSS->SetCSSAttribute(tTableAttribute);

    //表格表头属性
    nsHtmlForCSS::CTableHeadAttribute tTableHeadAttribute;
    tTableHeadAttribute.m_IsUseWidthRange = true;
    tTableHeadAttribute.m_WidthRange = 30;
    tTableHeadAttribute.m_FontAttribute.m_FontSize = 10;
    tTableHeadAttribute.m_FontAttribute.m_LengthType = nsHtmlForCSS::e_Length_PT;
    tTableHeadAttribute.m_FontAttribute.m_FontWeightType = nsHtmlForCSS::e_FontWeight_Bold;
    tTableHeadAttribute.m_TextAttribute.m_AlignType = nsHtmlForCSS::e_Align_Center;
    tTableHeadAttribute.m_TextAttribute.m_ValignType = nsHtmlForCSS::e_Valign_Middle;
    uint32_t tTableHeadCSSAttributeIndex = vHtmlForCSS->SetCSSAttribute(tTableHeadAttribute);

    //表格数据属性
    nsHtmlForCSS::CTableDataAttribute tTableDataAttribute;
    tTableDataAttribute.m_FontAttribute.m_FontSize = 10;
    tTableDataAttribute.m_FontAttribute.m_LengthType = nsHtmlForCSS::e_Length_PT;
    tTableDataAttribute.m_TextAttribute.m_AlignType = nsHtmlForCSS::e_Align_Center;
    tTableDataAttribute.m_TextAttribute.m_ValignType = nsHtmlForCSS::e_Valign_Middle;
    uint32_t tTableDataCSSAttributeIndex = vHtmlForCSS->SetCSSAttribute(tTableDataAttribute);

    //表格表头数据索引列表
    QList<uint32_t> tTableHeadIndexList;
    //表格行数据索引列表
    QList<uint32_t> tTableDataIndexList;

    QList<QPair<QString, QString>> tTableHeadAndDataList{
        {"名称", "测试终端"},
        {"生产单位", "仿真工坊0x0001"},
        {"调试类别", "模拟测试"},
        {"编写日期", m_ReportDateTime.toString("yyyy年MM月dd日")},
    };

    for(auto tTableHeadAndDataPair: tTableHeadAndDataList)
    {
        QString tTableHeadSTR = tTableHeadAndDataPair.first;
        uint32_t tTableHeadIndex = vHtmlForCSS->AddHtmlData4TableHead(tTableHeadSTR, tTableHeadCSSAttributeIndex);
        tTableHeadIndexList.append(tTableHeadIndex);

        QStringList tTableDataList;
        tTableDataList.append(tTableHeadAndDataPair.second);
        uint32_t tTableDataIndex = vHtmlForCSS->AddHtmlData4TableDataList(tTableDataList, tTableDataCSSAttributeIndex);
        tTableDataIndexList.push_back(tTableDataIndex);
    }

    vHtmlForCSS->AddHtmlData4TableVertical(tTableHeadIndexList, tTableDataIndexList, tTableCSSAttributeIndex);
}
