#include "ReportCreateWordDemo.h"

CReportCreateWordDemo::CReportCreateWordDemo()
{

}

bool CReportCreateWordDemo::create(bool vVisible, QStringList *vInfoListPTR)
{
    Q_UNUSED(vVisible);

    //清空错误信息
    m_ReportErrorList.clear();

    //初始化时间和测试项列表
    m_ReportDateTime = QDateTime::currentDateTime();

    //清空缓存数据
    m_ReportCreateWord.ClearDataCache();
    m_ReportCreateWord.ClearTableDataCache();
    m_ReportCreateWord.ClearCSSAttributeDataCache();

    //组装报告
    AddDataAboutHome(&m_ReportCreateWord);

    //输出错误信息列表
    if(vInfoListPTR != nullptr)
        vInfoListPTR->append(m_ReportErrorList);

    return true;
}

bool CReportCreateWordDemo::save(QString vFilePath, QString &vFileName)
{
    QString tReportFilePath = vFilePath;
    QString tReportFileName = m_ReportDateTime.toString("yyyyMMddhhmmss");
    QString tReportFileSuffix = ".doc";

    bool tCreateRet = m_ReportCreateWord.CreateFile(tReportFilePath, tReportFileName, tReportFileSuffix);
    if(tCreateRet)
        vFileName = tReportFileName + tReportFileSuffix;

    return tCreateRet;
}
