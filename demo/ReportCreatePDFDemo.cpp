#include "ReportCreatePDFDemo.h"

#include <QMap>

CReportCreatePDFDemo::CReportCreatePDFDemo()
{

}

bool CReportCreatePDFDemo::create(bool vVisible, QStringList *vInfoListPTR)
{
    Q_UNUSED(vVisible);

    //清空错误信息
    m_ReportErrorList.clear();

    //初始化时间
    m_ReportDateTime = QDateTime::currentDateTime();

    //清空缓存数据
    m_ReportCreatePDF.ClearDataCache();
    m_ReportCreatePDF.ClearTableDataCache();
    m_ReportCreatePDF.ClearCSSAttributeDataCache();

    //组装报告
    AddDataAboutHome(&m_ReportCreatePDF);

    //输出错误信息列表
    if(vInfoListPTR != nullptr)
        vInfoListPTR->append(m_ReportErrorList);

    return true;
}

bool CReportCreatePDFDemo::save(QString vFilePath, QString &vFileName)
{
#ifdef QT_DEBUG
    m_ReportCreatePDF.SetNeedCheckDataEnable(true);
#endif

    QString tReportFilePath = vFilePath;
    QString tReportFileName = m_ReportDateTime.toString("yyyyMMddhhmmss");
    QString tReportFileSuffix = ".pdf";

    bool tCreateRet = m_ReportCreatePDF.CreateFile(tReportFilePath, tReportFileName, tReportFileSuffix);
    if(tCreateRet)
        vFileName = tReportFileName + tReportFileSuffix;

    return tCreateRet;
}
