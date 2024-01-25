#ifndef CREPORTDEMODATA_H
#define CREPORTDEMODATA_H

#include <QString>
#include <QDateTime>

class CHtmlForCSS;

class CReportDemoData
{
public:
    virtual ~CReportDemoData(){};

protected:
    /**
     * @brief AddDataAboutHome      添加首页数据
     */
    void AddDataAboutHome(CHtmlForCSS* vHtmlForCSS);

protected:
    QStringList     m_ReportErrorList;  //报告错误信息列表
    QDateTime       m_ReportDateTime;   //报告时间信息

private:
    void AddTitleData2Home(CHtmlForCSS* vHtmlForCSS);
    void AddReportTypeData2Home(CHtmlForCSS* vHtmlForCSS);
    void AddReportNumberData2Home(CHtmlForCSS* vHtmlForCSS);
    void AddBaseInfoData2Home(CHtmlForCSS* vHtmlForCSS);
};

#endif // CREPORTDEMODATA_H
