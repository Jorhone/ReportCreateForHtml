#ifndef CREPORTCREATEPDFDEMO_H
#define CREPORTCREATEPDFDEMO_H

#include "ReportDemoData.h"
#include "ReportCreatePDF.h"

class CReportCreatePDFDemo : public CReportDemoData
{
public:
    CReportCreatePDFDemo();

    /**
     * @brief create        生成报告数据
     * @param vVisible      [in]流程是否可见
     * @param vInfoListPTR  [in]错误信息列表指针，非空则使用，默认：nullptr
     * @return
     */
    virtual bool create(bool vVisible, QStringList* vInfoListPTR = nullptr);

    /**
     * @brief save          保存报告数据到文件
     * @param vFilePath     [in]报告文件存储目录，绝对路径，eg：D://test/
     * @return
     */
    virtual bool save(QString vFilePath, QString& vFileName);

private:
    CReportCreatePDF m_ReportCreatePDF;
};

#endif // CREPORTCREATEPDFDEMO_H
