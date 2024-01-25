#ifndef CREPORTCREATEWORD_H
#define CREPORTCREATEWORD_H

#include "ReportCreateForHtmlExport.h"
#include "HtmlForCSS.h"

#include <QString>

// Create Report Word With HtmlForCSS
class REPORTCREATEFORHTML_EXPORT CReportCreateWord : public CHtmlForCSS
{
public:
    CReportCreateWord();

    /**
     * @brief CreateFile            生成文件
     * @param vFilePath             [in]文件路径，需要携带路径拼接符，eg：D:/abc/
     * @param vFileName             [in]文件名，不包含后缀，eg：test
     * @param vFileSuffix           [in]文件后缀，eg：.txt
     * @return
     */
    virtual bool CreateFile(const QString vFilePath, const QString vFileName, const QString vFileSuffix);

    /**
     * @brief AddHtmlData4Paging    添加html数据：分页
     */
    virtual void AddHtmlData4Paging(void);

private:
    QString GetWordHeadData(void);
    QString GetWordDocumentData(void);
    QString GetCompatibilityData(void);
    QString GetMathPrData(void);
};

#endif // CREPORTCREATEWORD_H
