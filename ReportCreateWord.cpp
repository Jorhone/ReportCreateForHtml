#include "ReportCreateWord.h"

#include <QMap>
#include <QDomDocument>
#include <QFile>

CReportCreateWord::CReportCreateWord()
{

}

bool CReportCreateWord::CreateFile(const QString vFilePath, const QString vFileName, const QString vFileSuffix)
{
    if(vFilePath.isEmpty())
        return false;

    if(vFileName.isEmpty())
        return false;

    if(m_HtmlDataList.isEmpty())
        return false;

//    <html ...>
//        <head>
//        <!-- [if]><![endif]-->
//            ......
//        </head>
//        <body>
//            ......
//        </body>
//    </html>

    QDomDocument tDomDocument;
//    QDomProcessingInstruction tDomProcessingInstruction = tDomDocument.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
//    tDomDocument.appendChild(tDomProcessingInstruction);

    //add root
    QDomElement tRootDomElement = tDomDocument.createElement("html");
    tDomDocument.appendChild(tRootDomElement);

    //add attribute to root
    QMap<QString, QString> tWordHtmlAttributeMap{
        {"xmlns:o",     "urn:schemas-microsoft-com:office:office"},
        {"xmlns:w",     "urn:schemas-microsoft-com:office:word"},
        {"xmlns",       "http://www.w3.org/TR/REC-html40"},
    };

    for(auto tWordHtmlAttribute: tWordHtmlAttributeMap.keys())
    {
        tRootDomElement.setAttribute(tWordHtmlAttribute, tWordHtmlAttributeMap.value(tWordHtmlAttribute));
    }

    //add head to root
    QDomElement tHeadDomElement = tDomDocument.createElement("head");
    tRootDomElement.appendChild(tHeadDomElement);

    //add comment to head
    {
        QString tCommentData = GetWordHeadData();
        QDomComment tDomComment = tDomDocument.createComment(tCommentData);
        tHeadDomElement.appendChild(tDomComment);

//        QDomDocument tDomDocument2Read;
//        if(tDomDocument2Read.setContent(tCommentData))
//        {
//            QDomElement tDomElement = tDomDocument2Read.documentElement();
//            tHeadDomElement.appendChild(tDomElement);
//        }
    }

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

void CReportCreateWord::AddHtmlData4Paging()
{
    QString tTagName = "br";
    QString tUserData = "";
    QString tStyleClassName = "";
    QStringList tAttributeList;
    QStringList tStyleAttributeList{
        "display:block;",
        "clear:both;",
        "page-break-before:always;",
    };

    QString tHtmlData = Convert2HtmlData(tTagName, tUserData, tStyleClassName, tAttributeList, tStyleAttributeList);
    m_HtmlDataList.push_back(tHtmlData);
}

QString CReportCreateWord::GetWordHeadData()
{
//    <!--[if gte mso 9]>
//    <xml>
//        <w:WordDocument>
//            <w:View>Print</w:View>
//            <w:TrackMoves>false</w:TrackMoves>
//            <w:TrackFormatting/>
//            <w:ValidateAgainstSchemas/>
//            <w:SaveIfXMLInvalid>false</w:SaveIfXMLInvalid>
//            <w:IgnoreMixedContent>false</w:IgnoreMixedContent>
//            <w:AlwaysShowPlaceholderText>false</w:AlwaysShowPlaceholderText>
//            <w:DoNotPromoteQF/>
//            <w:LidThemeOther>EN-US</w:LidThemeOther>
//            <w:LidThemeAsian>ZH-CN</w:LidThemeAsian>
//            <w:LidThemeComplexScript>X-NONE</w:LidThemeComplexScript>
//            <w:Compatibility>
//                <w:BreakWrappedTables/>
//                <w:SnapToGridInCell/>
//                <w:WrapTextWithPunct/>
//                <w:UseAsianBreakRules/>
//                <w:DontGrowAutofit/>
//                <w:SplitPgBreakAndParaMark/>
//                <w:DontVertAlignCellWithSp/>
//                <w:DontBreakConstrainedForcedTables/>
//                <w:DontVertAlignInTxbx/>
//                <w:Word11KerningPairs/>
//                <w:CachedColBalance/>
//                <w:UseFELayout/>
//            </w:Compatibility>
//            <w:BrowserLevel>MicrosoftInternetExplorer4</w:BrowserLevel>
//            <m:mathPr>
//                <m:mathFont m:val='Cambria Math'/>
//                <m:brkBin m:val='before'/>
//                <m:brkBinSub m:val='--'/>
//                <m:smallFrac m:val='off'/>
//                <m:dispDef/>
//                <m:lMargin m:val='0'/>
//                <m:rMargin m:val='0'/>
//                <m:defJc m:val='centerGroup'/>
//                <m:wrapIndent m:val='1440'/>
//                <m:intLim m:val='subSup'/>
//                <m:naryLim m:val='undOvr'/>
//            </m:mathPr>
//        </w:WordDocument>
//    </xml>
//    <![endif]-->

    QDomDocument tDomDocument;
    QDomElement tRootDomElement = tDomDocument.createElement("xml");
    tDomDocument.appendChild(tRootDomElement);

    //w:WordDocument
    {
        QString tWordDocumentData = GetWordDocumentData();

        QDomDocument tDomDocument2Read;
        if(tDomDocument2Read.setContent(tWordDocumentData))
        {
            QDomElement tDomElement = tDomDocument2Read.documentElement();
            tRootDomElement.appendChild(tDomElement);
        }
    }

    QString tXmlData = tDomDocument.toString();
    QString tCommentData = QString("[if gte mso 9]>%1<![endif]").arg(tXmlData);
    return tCommentData;

//    QDomDocument tDomDocument2Return;
//    QDomComment tDomComment = tDomDocument2Return.createComment(tCommentData);
//    tDomDocument2Return.appendChild(tDomComment);

//    //输出html数据
//    QString tHtmlData = tDomDocument2Return.toString();
//    return tHtmlData;
}

QString CReportCreateWord::GetWordDocumentData()
{
    QDomDocument tDomDocument;

    QDomElement tWordDocumentDomElement = tDomDocument.createElement("w:WordDocument");
    tDomDocument.appendChild(tWordDocumentDomElement);

    QDomElement tViewDomElement = tDomDocument.createElement("w:View");
    QDomText tViewDomText = tDomDocument.createTextNode("Print");
    tViewDomElement.appendChild(tViewDomText);
    tWordDocumentDomElement.appendChild(tViewDomElement);

    QDomElement tTrackMovesDomElement = tDomDocument.createElement("w:TrackMoves");
    QDomText tTrackMovesDomText = tDomDocument.createTextNode("false");
    tTrackMovesDomElement.appendChild(tTrackMovesDomText);
    tWordDocumentDomElement.appendChild(tTrackMovesDomElement);

    QDomElement tTrackFormattingDomElement = tDomDocument.createElement("w:TrackFormatting");
    tWordDocumentDomElement.appendChild(tTrackFormattingDomElement);

    QDomElement tValidateAgainstSchemasDomElement = tDomDocument.createElement("w:ValidateAgainstSchemas");
    tWordDocumentDomElement.appendChild(tValidateAgainstSchemasDomElement);

    QDomElement tSaveIfXMLInvalidDomElement = tDomDocument.createElement("w:SaveIfXMLInvalid");
    QDomText tSaveIfXMLInvalidDomText = tDomDocument.createTextNode("false");
    tSaveIfXMLInvalidDomElement.appendChild(tSaveIfXMLInvalidDomText);
    tWordDocumentDomElement.appendChild(tSaveIfXMLInvalidDomElement);

    QDomElement tIgnoreMixedContentDomElement = tDomDocument.createElement("w:IgnoreMixedContent");
    QDomText tIgnoreMixedContentDomText = tDomDocument.createTextNode("false");
    tIgnoreMixedContentDomElement.appendChild(tIgnoreMixedContentDomText);
    tWordDocumentDomElement.appendChild(tIgnoreMixedContentDomElement);

    QDomElement tAlwaysShowPlaceholderTextDomElement = tDomDocument.createElement("w:AlwaysShowPlaceholderText");
    QDomText tAlwaysShowPlaceholderTextDomText = tDomDocument.createTextNode("false");
    tAlwaysShowPlaceholderTextDomElement.appendChild(tAlwaysShowPlaceholderTextDomText);
    tWordDocumentDomElement.appendChild(tAlwaysShowPlaceholderTextDomElement);

    QDomElement tDoNotPromoteQFDomElement = tDomDocument.createElement("w:DoNotPromoteQF");
    tWordDocumentDomElement.appendChild(tDoNotPromoteQFDomElement);

    QDomElement tLidThemeOtherDomElement = tDomDocument.createElement("w:LidThemeOther");
    QDomText tLidThemeOtherDomText = tDomDocument.createTextNode("EN-US");
    tLidThemeOtherDomElement.appendChild(tLidThemeOtherDomText);
    tWordDocumentDomElement.appendChild(tLidThemeOtherDomElement);

    QDomElement tLidThemeAsianDomElement = tDomDocument.createElement("w:LidThemeAsian");
    QDomText tLidThemeAsianDomText = tDomDocument.createTextNode("ZH-CN");
    tLidThemeAsianDomElement.appendChild(tLidThemeAsianDomText);
    tWordDocumentDomElement.appendChild(tLidThemeAsianDomElement);

    QDomElement tLidThemeComplexScriptDomElement = tDomDocument.createElement("w:LidThemeComplexScript");
    QDomText tLidThemeComplexScriptDomText = tDomDocument.createTextNode("X-NONE");
    tLidThemeComplexScriptDomElement.appendChild(tLidThemeComplexScriptDomText);
    tWordDocumentDomElement.appendChild(tLidThemeComplexScriptDomElement);

    //w:Compatibility
    {
        QString tCompatibilityData = GetCompatibilityData();

        QDomDocument tDomDocument2Read;
        if(tDomDocument2Read.setContent(tCompatibilityData))
        {
            QDomElement tDomElement = tDomDocument2Read.documentElement();
            tWordDocumentDomElement.appendChild(tDomElement);
        }
    }

    QDomElement tBrowserLevelDomElement = tDomDocument.createElement("w:BrowserLevel");
    QDomText tBrowserLevelDomText = tDomDocument.createTextNode("MicrosoftInternetExplorer4");
    tBrowserLevelDomElement.appendChild(tBrowserLevelDomText);
    tWordDocumentDomElement.appendChild(tBrowserLevelDomElement);

    //m:mathPr
    {
        QString tMathPrData = GetMathPrData();

        QDomDocument tDomDocument2Read;
        if(tDomDocument2Read.setContent(tMathPrData))
        {
            QDomElement tDomElement = tDomDocument2Read.documentElement();
            tWordDocumentDomElement.appendChild(tDomElement);
        }
    }

    //输出html数据
    QString tHtmlData = tDomDocument.toString();
    return tHtmlData;
}

QString CReportCreateWord::GetCompatibilityData()
{
    QDomDocument tDomDocument;

    QDomElement tCompatibilityDomElement = tDomDocument.createElement("w:Compatibility");
    tDomDocument.appendChild(tCompatibilityDomElement);

    QDomElement tBreakWrappedTablesDomElement = tDomDocument.createElement("w:BreakWrappedTables");
    tCompatibilityDomElement.appendChild(tBreakWrappedTablesDomElement);

    QDomElement tSnapToGridInCellDomElement = tDomDocument.createElement("w:SnapToGridInCell");
    tCompatibilityDomElement.appendChild(tSnapToGridInCellDomElement);

    QDomElement tWrapTextWithPunctDomElement = tDomDocument.createElement("w:WrapTextWithPunct");
    tCompatibilityDomElement.appendChild(tWrapTextWithPunctDomElement);

    QDomElement tUseAsianBreakRulesDomElement = tDomDocument.createElement("w:UseAsianBreakRules");
    tCompatibilityDomElement.appendChild(tUseAsianBreakRulesDomElement);

    QDomElement tDontGrowAutofitDomElement = tDomDocument.createElement("w:DontGrowAutofit");
    tCompatibilityDomElement.appendChild(tDontGrowAutofitDomElement);

    QDomElement tSplitPgBreakAndParaMarkDomElement = tDomDocument.createElement("w:SplitPgBreakAndParaMark");
    tCompatibilityDomElement.appendChild(tSplitPgBreakAndParaMarkDomElement);

    QDomElement tDontVertAlignCellWithSpDomElement = tDomDocument.createElement("w:DontVertAlignCellWithSp");
    tCompatibilityDomElement.appendChild(tDontVertAlignCellWithSpDomElement);

    QDomElement tDontBreakConstrainedForcedTablesDomElement = tDomDocument.createElement("w:DontBreakConstrainedForcedTables");
    tCompatibilityDomElement.appendChild(tDontBreakConstrainedForcedTablesDomElement);

    QDomElement tDontVertAlignInTxbxDomElement = tDomDocument.createElement("w:DontVertAlignInTxbx");
    tCompatibilityDomElement.appendChild(tDontVertAlignInTxbxDomElement);

    QDomElement tWord11KerningPairsDomElement = tDomDocument.createElement("w:Word11KerningPairs");
    tCompatibilityDomElement.appendChild(tWord11KerningPairsDomElement);

    QDomElement tCachedColBalanceDomElement = tDomDocument.createElement("w:CachedColBalance");
    tCompatibilityDomElement.appendChild(tCachedColBalanceDomElement);

    QDomElement tUseFELayoutDomElement = tDomDocument.createElement("w:UseFELayout");
    tCompatibilityDomElement.appendChild(tUseFELayoutDomElement);

    //输出html数据
    QString tHtmlData = tDomDocument.toString();
    return tHtmlData;
}

QString CReportCreateWord::GetMathPrData()
{
    QDomDocument tDomDocument;

    QDomElement tMathPrDomElement = tDomDocument.createElement("m:mathPr");
    tDomDocument.appendChild(tMathPrDomElement);

    QDomElement tMathFontDomElement = tDomDocument.createElement("m:mathFont");
    tMathFontDomElement.setAttribute("m:val", "Cambria Math");
    tMathPrDomElement.appendChild(tMathFontDomElement);

    QDomElement tBrkBinDomElement = tDomDocument.createElement("m:brkBin");
    tBrkBinDomElement.setAttribute("m:val", "before");
    tMathPrDomElement.appendChild(tBrkBinDomElement);

    QDomElement tBrkBinSubDomElement = tDomDocument.createElement("m:brkBinSub");
    tBrkBinSubDomElement.setAttribute("m:val", "--");
    tMathPrDomElement.appendChild(tBrkBinSubDomElement);

    QDomElement tSmallFracDomElement = tDomDocument.createElement("m:smallFrac");
    tSmallFracDomElement.setAttribute("m:val", "off");
    tMathPrDomElement.appendChild(tSmallFracDomElement);

    QDomElement tDispDefDomElement = tDomDocument.createElement("m:dispDef");
    tMathPrDomElement.appendChild(tDispDefDomElement);

    QDomElement tLMarginDomElement = tDomDocument.createElement("m:lMargin");
    tLMarginDomElement.setAttribute("m:val", "0");
    tMathPrDomElement.appendChild(tLMarginDomElement);

    QDomElement tRMarginDomElement = tDomDocument.createElement("m:rMargin");
    tRMarginDomElement.setAttribute("m:val", "0");
    tMathPrDomElement.appendChild(tRMarginDomElement);

    QDomElement tDefJcDomElement = tDomDocument.createElement("m:defJc");
    tDefJcDomElement.setAttribute("m:val", "centerGroup");
    tMathPrDomElement.appendChild(tDefJcDomElement);

    QDomElement tWrapIndentDomElement = tDomDocument.createElement("m:wrapIndent");
    tWrapIndentDomElement.setAttribute("m:val", "1440");
    tMathPrDomElement.appendChild(tWrapIndentDomElement);

    QDomElement tIntLimDomElement = tDomDocument.createElement("m:intLim");
    tIntLimDomElement.setAttribute("m:val", "subSup");
    tMathPrDomElement.appendChild(tIntLimDomElement);

    QDomElement tNaryLimDomElement = tDomDocument.createElement("m:naryLim");
    tNaryLimDomElement.setAttribute("m:val", "undOvr");
    tMathPrDomElement.appendChild(tNaryLimDomElement);

    //输出html数据
    QString tHtmlData = tDomDocument.toString();
    return tHtmlData;
}
