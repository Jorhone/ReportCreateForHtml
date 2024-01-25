#include "ReportCreatePDF.h"

#include <QDomDocument>
#include <QTextDocument>
#include <QPrinter>

CReportCreatePDF::CReportCreatePDF()
{

}

bool CReportCreatePDF::CreateFile(const QString vFilePath, const QString vFileName, const QString vFileSuffix)
{
    if(vFilePath.isEmpty())
        return false;

    if(vFileName.isEmpty())
        return false;

    if(m_HtmlDataList.isEmpty())
        return false;

//    <html>
//        <head>
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

    QPrinter tPrinter(QPrinter::HighResolution);
    tPrinter.setPaperSize(QPrinter::A4);
    tPrinter.setOrientation(QPrinter::Portrait);

    //设置输出格式为PDF
    QString tFilePath = vFilePath + vFileName + vFileSuffix;
    tPrinter.setOutputFormat(QPrinter::PdfFormat);
    tPrinter.setOutputFileName(tFilePath);

    QTextDocument tTextDocument;
    QString tFileData = tDomDocument.toString();
    tTextDocument.setHtml(tFileData);
    tTextDocument.print(&tPrinter);

    //检查style和body数据
    if(m_NeedCheckData)
        CHtmlForCSS::CreateFile(vFilePath, vFileName, ".html");

    return true;
}

void CReportCreatePDF::SetNeedCheckDataEnable(bool vEnable)
{
    m_NeedCheckData = vEnable;
}
