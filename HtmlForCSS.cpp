#include "HtmlForCSS.h"

#include <QDomDocument>
#include <QFile>

CHtmlForCSS::CHtmlForCSS()
{
    InitData();
}

CHtmlForCSS::~CHtmlForCSS()
{

}

bool CHtmlForCSS::CreateFile(const QString vFilePath, const QString vFileName, const QString vFileSuffix)
{
    if(vFilePath.isEmpty())
        return false;

    if(vFileName.isEmpty())
        return false;

    if(m_HtmlDataList.isEmpty())
        return false;

    QDomDocument tDomDocument;

    //add root
    QDomElement tRootDomElement = tDomDocument.createElement("html");
    tDomDocument.appendChild(tRootDomElement);

    //add head to root
    QDomElement tHeadDomElement = tDomDocument.createElement("head");
    tRootDomElement.appendChild(tHeadDomElement);

    //add style to head
    if(!m_CSSAttributeHash.isEmpty())
    {
        QDomElement tStyleDomElement = tDomDocument.createElement("style");
        tHeadDomElement.appendChild(tStyleDomElement);

        for(auto tCSSAttributeIndex: m_CSSAttributeHash.keys())
        {
            const sCSSAttribute& tCSSAttribute = m_CSSAttributeHash.value(tCSSAttributeIndex);

            QString tStyleClassName = tCSSAttribute.StyleClassName;
            if(!tCSSAttribute.StyleParentClassName.isEmpty())
                tStyleClassName = QString("%1.%2").arg(tCSSAttribute.StyleParentClassName, tStyleClassName);

            QString tStyleClass = QString(".%1{%2}").arg(tStyleClassName, tCSSAttribute.StyleAttributeList.join(' '));
            QDomText tStyleClassDomText = tDomDocument.createTextNode(tStyleClass);
            tStyleDomElement.appendChild(tStyleClassDomText);
        }
    }

    //add body to root
    QDomElement tBodyDomElement = tDomDocument.createElement("body");
    tRootDomElement.appendChild(tBodyDomElement);

    //add html data to body
    for(auto tHtmlData: m_HtmlDataList)
    {
        QDomDocument tDomDocument2Read;
        if(tDomDocument2Read.setContent(tHtmlData))
        {
            QDomElement tDomElement = tDomDocument2Read.documentElement();
            tBodyDomElement.appendChild(tDomElement);
        }
    }

    QString tFilePath = vFilePath + vFileName + vFileSuffix;
    QFile tFile(tFilePath);
    if(!tFile.open(QIODevice::WriteOnly | QIODevice::Append))
        return false;

    QString tFileData = tDomDocument.toString();
    tFile.write(tFileData.toUtf8());
    tFile.close();//关闭文件

    return true;
}

void CHtmlForCSS::ClearDataCache()
{
    m_HtmlDataList.clear();
}

void CHtmlForCSS::ClearTableDataCache()
{
    m_TableHeadHtmlDataHash.clear();
    m_TableDataHtmlDataHash.clear();
}

void CHtmlForCSS::ClearCSSAttributeDataCache()
{
    m_CSSAttributeHash.clear();

    AddDefaultStyleClass();
}

void CHtmlForCSS::AddHtmlData4Wrap()
{
    QString tTagName = "br";
    QString tUserData = "";
    QString tStyleClassName = "";
    QStringList tAttributeList;
    QStringList tStyleAttributeList;

    QString tHtmlData = Convert2HtmlData(tTagName, tUserData, tStyleClassName, tAttributeList, tStyleAttributeList);
    m_HtmlDataList.push_back(tHtmlData);
}

void CHtmlForCSS::AddHtmlData4Paging()
{
    QString tTagName = "div";
    QString tUserData = " ";
    QString tStyleClassName = "";
    QStringList tAttributeList;
    QStringList tStyleAttributeList{
        "display:block;",
        "clear:both;",
        "page-break-after:always;",
    };

    QString tHtmlData = Convert2HtmlData(tTagName, tUserData, tStyleClassName, tAttributeList, tStyleAttributeList);
    m_HtmlDataList.push_back(tHtmlData);
}

void CHtmlForCSS::AddHtmlData4Paragraph(const QString vUserData, uint32_t vCSSAttributeIndex)
{
    QString tTagName = "p";
    QString tUserData = vUserData;
    QString tStyleClassName = GetStyleClassName(vCSSAttributeIndex);
    QStringList tAttributeList;
    QStringList tStyleAttributeList;

    if(m_CSSAttributeHash.contains(vCSSAttributeIndex))
    {
        const sCSSAttribute& tCSSAttribute = m_CSSAttributeHash.value(vCSSAttributeIndex);
        tAttributeList.append(tCSSAttribute.AttributeList);
    }

    QString tHtmlData = Convert2HtmlData(tTagName, tUserData, tStyleClassName, tAttributeList, tStyleAttributeList);
    m_HtmlDataList.push_back(tHtmlData);
}

uint32_t CHtmlForCSS::AddHtmlData4TableHead(const QString vUserData, uint32_t vCSSAttributeIndex)
{
    QString tTagName = "th";
    QString tUserData = vUserData;
    QString tStyleClassName = GetStyleClassName(vCSSAttributeIndex);
    QStringList tAttributeList;
    QStringList tStyleAttributeList;

    if(m_CSSAttributeHash.contains(vCSSAttributeIndex))
    {
        const sCSSAttribute& tCSSAttribute = m_CSSAttributeHash.value(vCSSAttributeIndex);
        tAttributeList.append(tCSSAttribute.AttributeList);
    }

    QString tHtmlData = Convert2HtmlData(tTagName, tUserData, tStyleClassName, tAttributeList, tStyleAttributeList);

    uint32_t tDataIndex = GetSequenceID();
    m_TableHeadHtmlDataHash.insert(tDataIndex, tHtmlData);

    return tDataIndex;
}

uint32_t CHtmlForCSS::AddHtmlData4TableDataList(const QStringList vUserDataList, uint32_t vCSSAttributeIndex)
{
    QString tTagName = "td";
    QString tStyleClassName = GetStyleClassName(vCSSAttributeIndex);
    QStringList tAttributeList;
    QStringList tStyleAttributeList;

    if(m_CSSAttributeHash.contains(vCSSAttributeIndex))
    {
        const sCSSAttribute& tCSSAttribute = m_CSSAttributeHash.value(vCSSAttributeIndex);
        tAttributeList.append(tCSSAttribute.AttributeList);
    }

    QStringList tHtmlDataList;
    for(auto tUserData: vUserDataList)
    {
        QString tHtmlData = Convert2HtmlData(tTagName, tUserData, tStyleClassName, tAttributeList, tStyleAttributeList);
        tHtmlDataList.push_back(tHtmlData);
    }

    uint32_t tDataIndex = GetSequenceID();
    m_TableDataHtmlDataHash.insert(tDataIndex, tHtmlDataList);

    return tDataIndex;
}

void CHtmlForCSS::AddHtmlData4Table(const QList<uint32_t> vTableHeadIndexList, const QList<uint32_t> vTableDataIndexList, uint32_t vCSSAttributeIndex, bool vIsFreezeHead, bool vIsAutoClear)
{
    if(vIsFreezeHead && vTableHeadIndexList.isEmpty())
        return;

    QDomDocument tDomDocument;
    QDomElement tRootDomElement = tDomDocument.createElement("table");
    tDomDocument.appendChild(tRootDomElement);

    //set style class
    QString tStyleClassName = GetStyleClassName(vCSSAttributeIndex);
    if(!tStyleClassName.isEmpty())
    {
//        tStyleClassName = QString("%1 %2").arg(m_DefaultStyleClassName, tStyleClassName);
        tRootDomElement.setAttribute("class", tStyleClassName);
    }

    //set attribute list
    if(m_CSSAttributeHash.contains(vCSSAttributeIndex))
    {
        const sCSSAttribute& tCSSAttribute = m_CSSAttributeHash.value(vCSSAttributeIndex);
        for(auto tAttributeSTR: tCSSAttribute.AttributeList)
        {
            //Attribute STR: key="value"
            QStringList tSplitList = tAttributeSTR.split('=');
            if(tSplitList.count() < 2)
                continue;

            //get key
            QString tAttributeKey = tSplitList.first();

            //get value
            int tAttributeValuePos = tAttributeKey.length() + 2;
            int tAttributeValueLength = tAttributeSTR.length() - tAttributeValuePos - 1;
            QString tAttributeValue = tAttributeSTR.mid(tAttributeValuePos, tAttributeValueLength);

            //set key value
            tRootDomElement.setAttribute(tAttributeKey, tAttributeValue);
        }
    }

    //set table head
    if(!vTableHeadIndexList.isEmpty())
    {
        QDomElement tTRDomElement = tDomDocument.createElement("tr");
        if(vIsFreezeHead)
        {
            QDomElement tTheadDomElement = tDomDocument.createElement("thead");
            tRootDomElement.appendChild(tTheadDomElement);

            tTheadDomElement.appendChild(tTRDomElement);
        }
        else
        {
            tRootDomElement.appendChild(tTRDomElement);
        }

        for(auto tTableHeadIndex: vTableHeadIndexList)
        {
            QString tTableHeadHtmlData = m_TableHeadHtmlDataHash.value(tTableHeadIndex);

            QDomDocument tDomDocument2Read;
            if(tDomDocument2Read.setContent(tTableHeadHtmlData))
            {
                QDomElement tTHDomElement = tDomDocument2Read.documentElement();
                tTRDomElement.appendChild(tTHDomElement);
            }
        }
    }

    //set table data
    int tColumnNum = vTableHeadIndexList.count();
    if(tColumnNum <= 0)
    {
        uint32_t tTableDataIndex = vTableDataIndexList.first();
        const QStringList& tTableDataHtmlDataList = m_TableDataHtmlDataHash.value(tTableDataIndex);
        tColumnNum = tTableDataHtmlDataList.count();
    }

    for(auto tTableDataIndex: vTableDataIndexList)
    {
        const QStringList& tTableDataHtmlDataList = m_TableDataHtmlDataHash.value(tTableDataIndex);

        int tHtmlDataCount = tColumnNum;
        if(tTableDataHtmlDataList.count() < tColumnNum)
            tHtmlDataCount = tTableDataHtmlDataList.count();

        QDomElement tTRDomElement = tDomDocument.createElement("tr");
        if(vIsFreezeHead)
        {
            QDomElement tTbodyDomElement = tDomDocument.createElement("tbody");
            tRootDomElement.appendChild(tTbodyDomElement);

            tTbodyDomElement.appendChild(tTRDomElement);
        }
        else
        {
            tRootDomElement.appendChild(tTRDomElement);
        }

        for(int i = 0; i < tHtmlDataCount; i++)
        {
            QString tTableDataHtmlData = tTableDataHtmlDataList.at(i);

            QDomDocument tDomDocument2Read;
            if(tDomDocument2Read.setContent(tTableDataHtmlData))
            {
                QDomElement tTDDomElement = tDomDocument2Read.documentElement();
                tTRDomElement.appendChild(tTDDomElement);
            }
        }
    }

    QString tHtmlData = tDomDocument.toString();
    m_HtmlDataList.push_back(tHtmlData);

    //组装完成，清空表格数据缓存
    if(vIsAutoClear)
        ClearTableDataCache();
}

void CHtmlForCSS::AddHtmlData4TableVertical(const QList<uint32_t> vTableHeadIndexList, const QList<uint32_t> vTableDataIndexList, uint32_t vCSSAttributeIndex, bool vIsAutoClear)
{
    if(vTableHeadIndexList.isEmpty())
        return;

    QDomDocument tDomDocument;
    QDomElement tRootDomElement = tDomDocument.createElement("table");
    tDomDocument.appendChild(tRootDomElement);

    //set style class
    QString tStyleClassName = GetStyleClassName(vCSSAttributeIndex);
    if(!tStyleClassName.isEmpty())
    {
//        tStyleClassName = QString("%1 %2").arg(m_DefaultStyleClassName, tStyleClassName);
        tRootDomElement.setAttribute("class", tStyleClassName);
    }

    //set attribute list
    if(m_CSSAttributeHash.contains(vCSSAttributeIndex))
    {
        const sCSSAttribute& tCSSAttribute = m_CSSAttributeHash.value(vCSSAttributeIndex);
        for(auto tAttributeSTR: tCSSAttribute.AttributeList)
        {
            //Attribute STR: key="value"
            QStringList tSplitList = tAttributeSTR.split('=');
            if(tSplitList.count() < 2)
                continue;

            //get key
            QString tAttributeKey = tSplitList.first();

            //get value
            int tAttributeValuePos = tAttributeKey.length() + 2;
            int tAttributeValueLength = tAttributeSTR.length() - tAttributeValuePos - 1;
            QString tAttributeValue = tAttributeSTR.mid(tAttributeValuePos, tAttributeValueLength);

            //set key value
            tRootDomElement.setAttribute(tAttributeKey, tAttributeValue);
        }
    }

    //set vertical table
    for(int i = 0; i < vTableHeadIndexList.count(); i++)
    {
        QDomElement tTRDomElement = tDomDocument.createElement("tr");
        tRootDomElement.appendChild(tTRDomElement);

        uint32_t tTableHeadIndex = vTableHeadIndexList.at(i);
        QString tTableHeadHtmlData = m_TableHeadHtmlDataHash.value(tTableHeadIndex);
        if(!tTableHeadHtmlData.isEmpty())
        {
            QDomDocument tDomDocument2Read;
            if(tDomDocument2Read.setContent(tTableHeadHtmlData))
            {
                QDomElement tTHDomElement = tDomDocument2Read.documentElement();
                tTRDomElement.appendChild(tTHDomElement);
            }
        }

        if(i > vTableDataIndexList.count())
            continue;

        uint32_t tTableDataIndex = vTableDataIndexList.at(i);
        const QStringList& tTableDataHtmlDataList = m_TableDataHtmlDataHash.value(tTableDataIndex);
        for(auto tTableDataHtmlData: tTableDataHtmlDataList)
        {
            QDomDocument tDomDocument2Read;
            if(tDomDocument2Read.setContent(tTableDataHtmlData))
            {
                QDomElement tTDDomElement = tDomDocument2Read.documentElement();
                tTRDomElement.appendChild(tTDDomElement);
            }
        }
    }

    QString tHtmlData = tDomDocument.toString();
    m_HtmlDataList.push_back(tHtmlData);

    //组装完成，清空表格数据缓存
    if(vIsAutoClear)
        ClearTableDataCache();
}

QString CHtmlForCSS::GetStyleClassName(uint32_t vCSSAttributeIndex)
{
    if(vCSSAttributeIndex == 0)
        return "";

    if(!m_CSSAttributeHash.contains(vCSSAttributeIndex))
        return "";

    const sCSSAttribute& tCSSAttribute = m_CSSAttributeHash.value(vCSSAttributeIndex);
    return tCSSAttribute.StyleClassName;
}

uint32_t CHtmlForCSS::SetCSSAttribute(const nsHtmlForCSS::CTableAttribute vTableAttribute, QString vStyleParentClassName)
{
    //CSS属性数据结构
    sCSSAttribute tCSSAttribute;

    //边框属性
    tCSSAttribute.append(GetCSSAttributes(vTableAttribute.m_BorderAttribute));

    //水平对齐
    if(vTableAttribute.m_IsAlignCenter)
    {
        QString tAttributeKey = m_AttributeTypeHash.value(nsHtmlForCSS::e_Attribute_Align);
        QString tAttributeValue = Enum2STR(nsHtmlForCSS::e_Align_Center);
        QString tAttribute = GetAttribute(tAttributeKey, tAttributeValue);
        tCSSAttribute.AttributeList.push_back(tAttribute);

//        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_Margin);
//        QString tStyleValue = "auto";
//        QString tStyleAttribute = QString("%1:%2;").arg(tStyleKey, tStyleValue);
//        tStyleAttributeList.push_back(tStyleAttribute);
    }

    {
        //表格宽度比例
        QString tAttributeKey = m_AttributeTypeHash.value(nsHtmlForCSS::e_Attribute_Width);
        QString tAttributeValue = QString("%1%").arg(vTableAttribute.m_WidthRange);
        QString tAttribute = GetAttribute(tAttributeKey, tAttributeValue);
        tCSSAttribute.AttributeList.push_back(tAttribute);

//        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_Width);
//        QString tStyleValue = QString("%1%").arg(vTableAttribute.m_WidthRange);
//        QString tStyleAttribute = QString("%1:%2;").arg(tStyleKey, tStyleValue);
//        tStyleAttributeList.push_back(tStyleAttribute);
    }

    //边框合并
    if(vTableAttribute.m_IsCollapseCellBorder)
    {
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_BorderCollapse);
        QString tStyleValue = Enum2STR(nsHtmlForCSS::e_BorderCollapse_Collapse);
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    {
        //边框间距
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_BorderSpacing);
        QString tStyleValue = QString("%1%2").arg(QString::number(vTableAttribute.m_BorderSpacing), Enum2STR(vTableAttribute.m_BorderSpacingLengthType));
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    uint32_t tCSSAttributeIndex = GetSequenceID();
    tCSSAttribute.StyleClassName = QString("table_%1").arg(tCSSAttributeIndex);
    tCSSAttribute.StyleParentClassName = vStyleParentClassName;
    m_CSSAttributeHash.insert(tCSSAttributeIndex, tCSSAttribute);

    return tCSSAttributeIndex;
}

uint32_t CHtmlForCSS::SetCSSAttribute(const nsHtmlForCSS::CTableHeadAttribute vTableHeadAttribute, QString vStyleParentClassName)
{
    //CSS属性数据结构
    sCSSAttribute tCSSAttribute;

    //字体属性
    tCSSAttribute.append(GetCSSAttributes(vTableHeadAttribute.m_FontAttribute));

    //文本属性
    tCSSAttribute.append(GetCSSAttributes(vTableHeadAttribute.m_TextAttribute));

    //背景颜色
    if(vTableHeadAttribute.m_IsUseBgColor)
    {
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_BackgroundColor);
        QString tStyleValue = Enum2STR(vTableHeadAttribute.m_BgColorType);
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    {
        //内边距
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_Padding);
        QString tStyleValue = QString("%1%2").arg(QString::number(vTableHeadAttribute.m_PaddingSize), Enum2STR(vTableHeadAttribute.m_PaddingLengthType));
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    //列宽比例
    if(vTableHeadAttribute.m_IsUseWidthRange)
    {
        QString tAttributeKey = m_AttributeTypeHash.value(nsHtmlForCSS::e_Attribute_Width);
        QString tAttributeValue = QString("%1%").arg(vTableHeadAttribute.m_WidthRange);
        QString tAttribute = GetAttribute(tAttributeKey, tAttributeValue);
        tCSSAttribute.AttributeList.push_back(tAttribute);

//        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_Width);
//        QString tStyleValue = QString("%1%").arg(vTableHeadAttribute.m_WidthRange);
//        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
//        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    uint32_t tCSSAttributeIndex = GetSequenceID();
    tCSSAttribute.StyleClassName = QString("th_%1").arg(tCSSAttributeIndex);
    tCSSAttribute.StyleParentClassName = vStyleParentClassName;
    m_CSSAttributeHash.insert(tCSSAttributeIndex, tCSSAttribute);

    return tCSSAttributeIndex;
}

uint32_t CHtmlForCSS::SetCSSAttribute(const nsHtmlForCSS::CTableDataAttribute vTableDataAttribute, QString vStyleParentClassName)
{
    //CSS属性数据结构
    sCSSAttribute tCSSAttribute;

    //字体属性
    tCSSAttribute.append(GetCSSAttributes(vTableDataAttribute.m_FontAttribute));

    //文本属性
    tCSSAttribute.append(GetCSSAttributes(vTableDataAttribute.m_TextAttribute));

    //背景颜色
    if(vTableDataAttribute.m_IsUseBgColor)
    {
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_BackgroundColor);
        QString tStyleValue = Enum2STR(vTableDataAttribute.m_BgColorType);
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    {
        //内边距
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_Padding);
        QString tStyleValue = QString("%1%2").arg(QString::number(vTableDataAttribute.m_PaddingSize), Enum2STR(vTableDataAttribute.m_PaddingLengthType));
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    uint32_t tCSSAttributeIndex = GetSequenceID();
    tCSSAttribute.StyleClassName = QString("td_%1").arg(tCSSAttributeIndex);
    tCSSAttribute.StyleParentClassName = vStyleParentClassName;
    m_CSSAttributeHash.insert(tCSSAttributeIndex, tCSSAttribute);

    return tCSSAttributeIndex;
}

uint32_t CHtmlForCSS::SetCSSAttribute(const nsHtmlForCSS::CParagraphAttribute vParagraphAttribute, QString vStyleParentClassName)
{
    //CSS属性数据结构
    sCSSAttribute tCSSAttribute;

    //字体属性
    tCSSAttribute.append(GetCSSAttributes(vParagraphAttribute.m_FontAttribute));

    //文本属性
    tCSSAttribute.append(GetCSSAttributes(vParagraphAttribute.m_TextAttribute));

    //背景颜色
    if(vParagraphAttribute.m_IsUseBgColor)
    {
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_BackgroundColor);
        QString tStyleValue = Enum2STR(vParagraphAttribute.m_BgColorType);
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    //文本缩进
    if(vParagraphAttribute.m_TextIndent > 0)
    {
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_TextIndent);
        QString tStyleValue = QString("%1%2").arg(QString::number(vParagraphAttribute.m_TextIndent), Enum2STR(vParagraphAttribute.m_LengthType));
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    uint32_t tCSSAttributeIndex = GetSequenceID();
    tCSSAttribute.StyleClassName = QString("p_%1").arg(tCSSAttributeIndex);
    tCSSAttribute.StyleParentClassName = vStyleParentClassName;
    m_CSSAttributeHash.insert(tCSSAttributeIndex, tCSSAttribute);

    return tCSSAttributeIndex;
}

QString CHtmlForCSS::Convert2HtmlData(const QString vTagName, const QString vUserData, const QString vStyleClassName, const QStringList vAttributeList, const QStringList vStyleAttributeList)
{
    //标签名不可为空
    if(vTagName.isEmpty())
        return "";

    QDomDocument tDomDocument;
    QDomElement tRootDomElement = tDomDocument.createElement(vTagName);
    tDomDocument.appendChild(tRootDomElement);

    //set style class
    if(!vStyleClassName.isEmpty())
    {
//        QString tStyleClassName = QString("%1 %2").arg(m_DefaultStyleClassName, vStyleClassName);
        tRootDomElement.setAttribute("class", vStyleClassName);
    }

    //set attribute list
    for(auto tAttributeSTR: vAttributeList)
    {
        //Attribute STR: key="value"
        QStringList tSplitList = tAttributeSTR.split('=');
        if(tSplitList.count() < 2)
            continue;

        //get key
        QString tAttributeKey = tSplitList.first();

        //get value
        int tAttributeValuePos = tAttributeKey.length() + 2;
        int tAttributeValueLength = tAttributeSTR.length() - tAttributeValuePos - 1;
        QString tAttributeValue = tAttributeSTR.mid(tAttributeValuePos, tAttributeValueLength);

        //set key value
        tRootDomElement.setAttribute(tAttributeKey, tAttributeValue);
    }

    //set style attribute list
    if(!vStyleAttributeList.isEmpty())
    {
        QString tStyleAttributeSTR = vStyleAttributeList.join(' ');
        tRootDomElement.setAttribute("style", tStyleAttributeSTR);
    }

    //add user data
    if(!vUserData.isEmpty())
    {
        QStringList tSplitList = vUserData.split("\n");
        for(int i = 0; i < tSplitList.count(); i++)
        {
            if(i != 0)
            {
                //第一个不用换行
                QDomElement tWrapDomElement = tDomDocument.createElement("br");
                tRootDomElement.appendChild(tWrapDomElement);
            }

            QDomElement tSpanDomElement = tDomDocument.createElement("span");
            tRootDomElement.appendChild(tSpanDomElement);

            QString tDataSTR = tSplitList.at(i);
            QDomText tDomText = tDomDocument.createTextNode(tDataSTR);
            tSpanDomElement.appendChild(tDomText);
        }
    }

    //输出html数据
    QString tHtmlData = tDomDocument.toString();
    return tHtmlData;
}

void CHtmlForCSS::InitData()
{
    m_AttributeTypeHash = QHash<nsHtmlForCSS::eAttributeType, QString>{
        {nsHtmlForCSS::e_Attribute_Align,           "align"},
        {nsHtmlForCSS::e_Attribute_Valign,          "valign"},
        {nsHtmlForCSS::e_Attribute_Width,           "width"},
        {nsHtmlForCSS::e_Attribute_Border,          "border"},
        {nsHtmlForCSS::e_Attribute_Cellspacing,     "cellspacing"},
        {nsHtmlForCSS::e_Attribute_Cellpadding,     "cellpadding"},
    };

    m_StyleAttributeTypeHash = QHash<nsHtmlForCSS::eStyleAttributeType, QString>{
        {nsHtmlForCSS::e_StyleAttribute_Height,             "height"},
        {nsHtmlForCSS::e_StyleAttribute_Width,              "width"},
        {nsHtmlForCSS::e_StyleAttribute_MaxWidth,           "max-width"},
        {nsHtmlForCSS::e_StyleAttribute_Margin,             "margin"},
        {nsHtmlForCSS::e_StyleAttribute_Padding,            "padding"},
        {nsHtmlForCSS::e_StyleAttribute_BackgroundColor,    "background-color"},
        {nsHtmlForCSS::e_StyleAttribute_BorderStyle,        "border-style"},
        {nsHtmlForCSS::e_StyleAttribute_BorderWidth,        "border-width"},
        {nsHtmlForCSS::e_StyleAttribute_BorderColor,        "border-color"},
        {nsHtmlForCSS::e_StyleAttribute_BorderCollapse,     "border-collapse"},
        {nsHtmlForCSS::e_StyleAttribute_BorderSpacing,      "border-spacing"},
        {nsHtmlForCSS::e_StyleAttribute_TextColor,          "color"},
        {nsHtmlForCSS::e_StyleAttribute_TextAlign,          "text-align"},
        {nsHtmlForCSS::e_StyleAttribute_TextValign,         "vertical-align"},
        {nsHtmlForCSS::e_StyleAttribute_TextIndent,         "text-indent"},
        {nsHtmlForCSS::e_StyleAttribute_TextWhiteSpace,     "white-space"},
        {nsHtmlForCSS::e_StyleAttribute_FontSize,           "font-size"},
        {nsHtmlForCSS::e_StyleAttribute_FontStyle,          "font-style"},
        {nsHtmlForCSS::e_StyleAttribute_FontWeight,         "font-weight"},
        {nsHtmlForCSS::e_StyleAttribute_FontFamily,         "font-family"},
    };

    m_DefaultStyleClassName = "clear";
    m_DefaultStyleAttributeList = QStringList{
            "margin:0;",
            "padding:0;",
    };

    AddDefaultStyleClass();
}

void CHtmlForCSS::AddDefaultStyleClass()
{
    uint32_t tCSSAttributeIndex = 0; //固定0，GetSequenceID()禁止使用0
    sCSSAttribute tCSSAttribute;
    tCSSAttribute.StyleClassName = m_DefaultStyleClassName;
    tCSSAttribute.StyleAttributeList.append(m_DefaultStyleAttributeList);

    m_CSSAttributeHash.insert(tCSSAttributeIndex, tCSSAttribute);
}

uint16_t CHtmlForCSS::GetSequenceID()
{
    //永远拿不到0，0用于内部默认处理
    if(m_SequenceID >= 0xffff)
        m_SequenceID = 0;

    return ++m_SequenceID;
}

QString CHtmlForCSS::GetAttribute(const QString vAttributeKey, const QString vAttributeValue)
{
    QString tAttribute = QString("%1=\"%2\"").arg(vAttributeKey, vAttributeValue);
    return tAttribute;
}

QString CHtmlForCSS::GetStyleAttribute(const QString vStyleAttributeKey, const QString vStyleAttributeValue)
{
    QString tStyleAttribute = QString("%1:%2;").arg(vStyleAttributeKey, vStyleAttributeValue);
    return tStyleAttribute;
}

CHtmlForCSS::sCSSAttribute CHtmlForCSS::GetCSSAttributes(nsHtmlForCSS::CFontAttribute vAttributeData)
{
    //CSS属性数据结构
    sCSSAttribute tCSSAttribute;

    {
        //字体大小
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_FontSize);
        QString tStyleValue = QString("%1%2").arg(QString::number(vAttributeData.m_FontSize), Enum2STR(vAttributeData.m_LengthType));
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    {
        //字体样式
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_FontStyle);
        QString tStyleValue = Enum2STR(vAttributeData.m_FontStyleType);
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    {
        //字体宽度
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_FontWeight);
        QString tStyleValue = Enum2STR(vAttributeData.m_FontWeightType);
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    {
        //字体系列
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_FontFamily);
        QString tStyleValue = Enum2STR(vAttributeData.m_FontFamilyType);
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    return tCSSAttribute;
}

CHtmlForCSS::sCSSAttribute CHtmlForCSS::GetCSSAttributes(nsHtmlForCSS::CTextAttribute vAttributeData)
{
    //CSS属性数据结构
    sCSSAttribute tCSSAttribute;

    {
        //文本颜色
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_TextColor);
        QString tStyleValue = Enum2STR(vAttributeData.m_ColorType);
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    {
        //文本水平对齐
//        QString tAttributeKey = m_AttributeTypeHash.value(nsHtmlForCSS::e_Attribute_Align);
//        QString tAttributeValue = Enum2STR(vAttributeData.m_AlignType);
//        QString tAttribute = GetAttribute(tAttributeKey, tAttributeValue);
//        tCSSAttribute.AttributeList.push_back(tAttribute);

        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_TextAlign);
        QString tStyleValue = Enum2STR(vAttributeData.m_AlignType);
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    {
        //文本垂直对齐
//        QString tAttributeKey = m_AttributeTypeHash.value(nsHtmlForCSS::e_Attribute_Valign);
//        QString tAttributeValue = Enum2STR(vAttributeData.m_ValignType);
//        QString tAttribute = GetAttribute(tAttributeKey, tAttributeValue);
//        tCSSAttribute.AttributeList.push_back(tAttribute);

        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_TextValign);
        QString tStyleValue = Enum2STR(vAttributeData.m_ValignType);
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    //2024.01.23 cjh：当前没有使用需求
//    {
//        //文本空白符属性
//        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_TextWhiteSpace);
//        QString tStyleValue = Enum2STR(vAttributeData.m_WhiteSpaceType);
//        QString tStyleAttribute = QString("%1:%2;").arg(tStyleKey, tStyleValue);
//        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
//    }

    return tCSSAttribute;
}

CHtmlForCSS::sCSSAttribute CHtmlForCSS::GetCSSAttributes(nsHtmlForCSS::CBorderAttribute vAttributeData)
{
    //CSS属性数据结构
    sCSSAttribute tCSSAttribute;

    {
        //边框宽度
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_BorderWidth);
        QString tStyleValue = "";
        if(vAttributeData.m_IsUseBorderWidthType)
            tStyleValue = Enum2STR(vAttributeData.m_BorderWidthType);
        else
            tStyleValue = QString("%1%2").arg(QString::number(vAttributeData.m_BorderSize), Enum2STR(vAttributeData.m_LengthType));

        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    {
        //边框颜色
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_BorderColor);
        QString tStyleValue = Enum2STR(vAttributeData.m_ColorType);
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    {
        //边框样式
        QString tStyleKey = m_StyleAttributeTypeHash.value(nsHtmlForCSS::e_StyleAttribute_BorderStyle);
        QString tStyleValue = Enum2STR(vAttributeData.m_BorderStyleType);
        QString tStyleAttribute = GetStyleAttribute(tStyleKey, tStyleValue);
        tCSSAttribute.StyleAttributeList.push_back(tStyleAttribute);
    }

    return tCSSAttribute;
}

QString CHtmlForCSS::Enum2STR(nsHtmlForCSS::eAlignType vEnumType)
{
    QHash<int, QString> tEnumTypeHash{
        {nsHtmlForCSS::e_Align_Left,    "left"},
        {nsHtmlForCSS::e_Align_Right,   "right"},
        {nsHtmlForCSS::e_Align_Center,  "center"},
        {nsHtmlForCSS::e_Align_Justify, "justify"},
    };

    QString tEnumSTR = tEnumTypeHash.value(vEnumType, "");
    return tEnumSTR;
}

QString CHtmlForCSS::Enum2STR(nsHtmlForCSS::eValignType vEnumType)
{
    QHash<int, QString> tEnumTypeHash{
        {nsHtmlForCSS::e_Valign_Top,        "top"},
        {nsHtmlForCSS::e_Valign_Middle,     "middle"},
        {nsHtmlForCSS::e_Valign_Bottom,     "bottom"},
        {nsHtmlForCSS::e_Valign_Baseline,   "baseline"},
    };

    QString tEnumSTR = tEnumTypeHash.value(vEnumType, "");
    return tEnumSTR;
}

QString CHtmlForCSS::Enum2STR(nsHtmlForCSS::eColorType vEnumType)
{
    QHash<int, QString> tEnumTypeHash{
        {nsHtmlForCSS::e_Color_Black,   "black"},
        {nsHtmlForCSS::e_Color_Blue,    "blue"},
        {nsHtmlForCSS::e_Color_Green,   "green"},
        {nsHtmlForCSS::e_Color_Red,     "red"},
        {nsHtmlForCSS::e_Color_White,   "white"},
        {nsHtmlForCSS::e_Color_Yellow,  "yellow"},
    };

    QString tEnumSTR = tEnumTypeHash.value(vEnumType, "");
    return tEnumSTR;
}

QString CHtmlForCSS::Enum2STR(nsHtmlForCSS::eWhiteSpaceType vEnumType)
{
    QHash<int, QString> tEnumTypeHash{
        {nsHtmlForCSS::e_WhiteSpace_Normal,     "normal"},
        {nsHtmlForCSS::e_WhiteSpace_Pre,        "pre"},
        {nsHtmlForCSS::e_WhiteSpace_Nowrap,     "nowrap"},
        {nsHtmlForCSS::e_WhiteSpace_PreWrap,    "pre-wrap"},
        {nsHtmlForCSS::e_WhiteSpace_PreLine,    "pre-line"},
        {nsHtmlForCSS::e_WhiteSpace_Inherit,    "inherit"},
    };

    QString tEnumSTR = tEnumTypeHash.value(vEnumType, "");
    return tEnumSTR;
}

QString CHtmlForCSS::Enum2STR(nsHtmlForCSS::eLengthType vEnumType)
{
    QHash<int, QString> tEnumTypeHash{
        {nsHtmlForCSS::e_Length_PX, "px"},
        {nsHtmlForCSS::e_Length_PT, "pt"},
        {nsHtmlForCSS::e_Length_EM, "em"},
    };

    QString tEnumSTR = tEnumTypeHash.value(vEnumType, "");
    return tEnumSTR;
}

QString CHtmlForCSS::Enum2STR(nsHtmlForCSS::eFontStyleType vEnumType)
{
    QHash<int, QString> tEnumTypeHash{
        {nsHtmlForCSS::e_FontStyle_Normal, "normal"},
        {nsHtmlForCSS::e_FontStyle_Italic, "italic"},
    };

    QString tEnumSTR = tEnumTypeHash.value(vEnumType, "");
    return tEnumSTR;
}

QString CHtmlForCSS::Enum2STR(nsHtmlForCSS::eFontWeightType vEnumType)
{
    QHash<int, QString> tEnumTypeHash{
        {nsHtmlForCSS::e_FontWeight_Normal, "normal"},
        {nsHtmlForCSS::e_FontWeight_Bold,   "bold"},
    };

    QString tEnumSTR = tEnumTypeHash.value(vEnumType, "");
    return tEnumSTR;
}

QString CHtmlForCSS::Enum2STR(nsHtmlForCSS::eFontFamilyType vEnumType)
{
    QHash<int, QString> tEnumTypeHash{
        {nsHtmlForCSS::e_FontFamily_SongTi,         "宋体"},
        {nsHtmlForCSS::e_FontFamily_Black,          "black"},
        {nsHtmlForCSS::e_FontFamily_Arial,          "Arial"},
        {nsHtmlForCSS::e_FontFamily_TimesNewRoman,  "Times New Roman"},
    };

    QString tEnumSTR = tEnumTypeHash.value(vEnumType, "");
    return tEnumSTR;
}

QString CHtmlForCSS::Enum2STR(nsHtmlForCSS::eBorderStyleType vEnumType)
{
    QHash<int, QString> tEnumTypeHash{
        {nsHtmlForCSS::e_BorderStyle_Dotted,    "dotted"},
        {nsHtmlForCSS::e_BorderStyle_Dashed,    "dashed"},
        {nsHtmlForCSS::e_BorderStyle_Solid,     "solid"},
        {nsHtmlForCSS::e_BorderStyle_Double,    "double"},
        {nsHtmlForCSS::e_BorderStyle_Groove,    "groove"},
        {nsHtmlForCSS::e_BorderStyle_Ridge,     "ridge"},
        {nsHtmlForCSS::e_BorderStyle_Inset,     "inset"},
        {nsHtmlForCSS::e_BorderStyle_Outset,    "outset"},
        {nsHtmlForCSS::e_BorderStyle_None,      "none"},
        {nsHtmlForCSS::e_BorderStyle_Hidden,    "hidden"},
    };

    QString tEnumSTR = tEnumTypeHash.value(vEnumType, "");
    return tEnumSTR;
}

QString CHtmlForCSS::Enum2STR(nsHtmlForCSS::eBorderWidthType vEnumType)
{
    QHash<int, QString> tEnumTypeHash{
        {nsHtmlForCSS::e_BorderWidth_Thin,      "thin"},
        {nsHtmlForCSS::e_BorderWidth_Medium,    "medium"},
        {nsHtmlForCSS::e_BorderWidth_Thick,     "thick"},
    };

    QString tEnumSTR = tEnumTypeHash.value(vEnumType, "");
    return tEnumSTR;
}

QString CHtmlForCSS::Enum2STR(nsHtmlForCSS::eBorderCollapseType vEnumType)
{
    QHash<int, QString> tEnumTypeHash{
        {nsHtmlForCSS::e_BorderCollapse_Separate,   "separate"},
        {nsHtmlForCSS::e_BorderCollapse_Collapse,   "collapse"},
        {nsHtmlForCSS::e_BorderCollapse_Inherit,    "inherit"},
    };

    QString tEnumSTR = tEnumTypeHash.value(vEnumType, "");
    return tEnumSTR;
}
