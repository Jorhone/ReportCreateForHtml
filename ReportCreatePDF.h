#ifndef CREPORTCREATEPDF_H
#define CREPORTCREATEPDF_H

#include "ReportCreateForHtmlExport.h"
#include "HtmlForCSS.h"

#include <QString>

// Create Report PDF With HtmlForCSS
class REPORTCREATEFORHTML_EXPORT CReportCreatePDF : public CHtmlForCSS
{
public:
    CReportCreatePDF();

    /**
     * @brief CreateFile            生成文件
     * @param vFilePath             [in]文件路径，需要携带路径拼接符，eg：D:/abc/
     * @param vFileName             [in]文件名，不包含后缀，eg：test
     * @param vFileSuffix           [in]文件后缀，eg：.txt
     * @return
     */
    virtual bool CreateFile(const QString vFilePath, const QString vFileName, const QString vFileSuffix);

    /**
     * @brief SetNeedCheckDataEnable    设置使能标识，是否需要检查数据
     * @param vEnable
     */
    void SetNeedCheckDataEnable(bool vEnable);

private:
    bool m_NeedCheckData = false;
};

#endif // CREPORTCREATEPDF_H
