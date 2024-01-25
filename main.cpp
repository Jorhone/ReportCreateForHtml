#include <QApplication>
#include <QDebug>

#include "ReportCreatePDFDemo.h"
#include "ReportCreateWordDemo.h"

void ExecDemo_CreateReportPDF(QString vFilePath)
{
    CReportCreatePDFDemo tReportCreateDemo;

    QString tFileName;
    QStringList tErrorInfoList;
    bool tCreateResult = tReportCreateDemo.create(false, &tErrorInfoList);
    qDebug() << tCreateResult;

    if(tCreateResult)
    {
        bool tSaveResult = tReportCreateDemo.save(vFilePath, tFileName);
        qDebug() << tSaveResult;
    }
}

void ExecDemo_CreateReportWord(QString vFilePath)
{
    CReportCreateWordDemo tReportCreateDemo;

    QString tFileName;
    QStringList tErrorInfoList;
    bool tCreateResult = tReportCreateDemo.create(false, &tErrorInfoList);
    qDebug() << tCreateResult;

    if(tCreateResult)
    {
        bool tSaveResult = tReportCreateDemo.save(vFilePath, tFileName);
        qDebug() << tSaveResult;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString tFilePath = "./";
    ExecDemo_CreateReportPDF(tFilePath);
    ExecDemo_CreateReportWord(tFilePath);

    return a.exec();
}
