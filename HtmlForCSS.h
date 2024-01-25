#ifndef CHTMLFORCSS_H
#define CHTMLFORCSS_H

#include "HtmlForCSSDefine.h"

#include <QString>
#include <QList>
#include <QHash>

#if not defined(LIBRARY_UNEXPORT)
    #include <QtCore/qglobal.h>
    #if defined(HTMLFORCSS_LIBRARY)
    #  define HTMLFORCSS_EXPORT Q_DECL_EXPORT
    #else
    #  define HTMLFORCSS_EXPORT Q_DECL_IMPORT
    #endif
#else
    #  define HTMLFORCSS_EXPORT
#endif

class HTMLFORCSS_EXPORT CHtmlForCSS
{
public:
    CHtmlForCSS();
    virtual ~CHtmlForCSS();

    /**
     * @brief CreateFile            生成文件
     * @param vFilePath             [in]文件路径，需要携带路径拼接符，eg：D:/abc/
     * @param vFileName             [in]文件名，不包含后缀，eg：test
     * @param vFileSuffix           [in]文件后缀，eg：.txt
     * @return
     */
    virtual bool CreateFile(const QString vFilePath, const QString vFileName, const QString vFileSuffix);

    /**
     * @brief ClearDataCache        清空数据缓存
     */
    virtual void ClearDataCache(void);

    /**
     * @brief ClearTableDataCache   清空表格数据缓存
     */
    virtual void ClearTableDataCache(void);

    /**
     * @brief ClearStyleDataCache   清空样式属性类数据缓存
     */
    virtual void ClearCSSAttributeDataCache(void);

    /**
     * @brief AddHtmlData4Wrap          添加html数据：换行
     */
    virtual void AddHtmlData4Wrap(void);

    /**
     * @brief AddHtmlData4Paging        添加html数据：分页
     */
    virtual void AddHtmlData4Paging(void);

    /**
     * @brief AddHtmlData4Paragraph     添加html数据：段落
     * @param vUserData                 [in]用户数据
     * @param vCSSAttributeIndex        [in]CSS属性索引
     */
    void AddHtmlData4Paragraph(const QString vUserData, uint32_t vCSSAttributeIndex = 0);

    /**
     * @brief AddHtmlData4TableHead     添加html数据：表格表头数据，需要指定所属表格
     * @param vUserData                 [in]用户数据
     * @param vCSSAttributeIndex        [in]CSS属性索引
     * @return                          [out]html数据关联index
     */
    uint32_t AddHtmlData4TableHead(const QString vUserData, uint32_t vCSSAttributeIndex = 0);

    /**
     * @brief AddHtmlData4TableDataList 添加html数据：表格一行数据，需要指定所属表格
     * @param vUserDataList             [in]用户数据列表
     * @param vCSSAttributeIndex        [in]CSS属性索引
     * @return                          [out]html数据关联index
     */
    uint32_t AddHtmlData4TableDataList(const QStringList vUserDataList, uint32_t vCSSAttributeIndex = 0);

    /**
     * @brief AddHtmlData4Table         添加html数据：表格，表头水平，执行后清空所有表格相关数据索引
     * @param vTableHeadIndexList       [in]表格表头数据索引列表
     * @param vTableDataIndexList       [in]表格行数据索引列表
     * @param vCSSAttributeIndex        [in]CSS属性索引
     * @param vIsFreezeHead             [in]是否冻结表头
     * @param vIsAutoClear              [in]是否自动清除表格数据缓存
     */
    void AddHtmlData4Table(const QList<uint32_t> vTableHeadIndexList, const QList<uint32_t> vTableDataIndexList, \
                           uint32_t vCSSAttributeIndex = 0, bool vIsFreezeHead = true, bool vIsAutoClear = true);

    /**
     * @brief AddHtmlData4TableVertical 添加html数据：表格，表头垂直，执行后清空所有表格相关数据索引
     * @param vTableHeadIndexList       [in]表格表头数据索引列表
     * @param vTableDataIndexList       [in]表格行数据索引列表
     * @param vCSSAttributeIndex        [in]CSS属性索引
     * @param vIsAutoClear              [in]是否自动清除表格数据缓存
     */
    void AddHtmlData4TableVertical(const QList<uint32_t> vTableHeadIndexList, const QList<uint32_t> vTableDataIndexList, \
                                   uint32_t vCSSAttributeIndex = 0, bool vIsAutoClear = true);

    //添加目录
    //添加图片
    //单元格合并,eg: colspan="2",rowspan="2"
    //添加水印
    //todo

    /**
     * @brief GetStyleClassName         获取样式属性类名
     * @param vCSSAttributeIndex        [in]CSS属性索引
     * @return
     */
    QString GetStyleClassName(uint32_t vCSSAttributeIndex = 0);

    /**
     * @brief SetCSSAttribute           设置CSS属性
     * @param vTableAttribute           [in]表格属性
     * @param vStyleParentClassName     [in]样式属性父类名称
     * @return                          [out]CSS属性索引(!= 0)
     */
    uint32_t SetCSSAttribute(const nsHtmlForCSS::CTableAttribute vTableAttribute, QString vStyleParentClassName = "");

    /**
     * @brief SetCSSAttribute           设置CSS属性
     * @param vTableHeadAttribute       [in]表格表头属性
     * @param vStyleParentClassName     [in]样式属性父类名称
     * @return                          [out]CSS属性索引(!= 0)
     */
    uint32_t SetCSSAttribute(const nsHtmlForCSS::CTableHeadAttribute vTableHeadAttribute, QString vStyleParentClassName = "");

    /**
     * @brief SetCSSAttribute           设置CSS属性
     * @param vTableDataAttribute       [in]表格数据属性
     * @param vStyleParentClassName     [in]样式属性父类名称
     * @return                          [out]CSS属性索引(!= 0)
     */
    uint32_t SetCSSAttribute(const nsHtmlForCSS::CTableDataAttribute vTableDataAttribute, QString vStyleParentClassName = "");

    /**
     * @brief SetCSSAttribute           设置CSS属性
     * @param vParagraphAttribute       [in]段落属性
     * @param vStyleParentClassName     [in]样式属性父类名称
     * @return                          [out]CSS属性索引(!= 0)
     */
    uint32_t SetCSSAttribute(const nsHtmlForCSS::CParagraphAttribute vParagraphAttribute, QString vStyleParentClassName = "");

protected:
    struct sCSSAttribute
    {
        QStringList AttributeList;      //节点属性列表

        QString     StyleParentClassName;   //节点样式属性父类名
        QString     StyleClassName;         //节点样式属性类名
        QStringList StyleAttributeList;     //节点样式属性列表

        void append(const sCSSAttribute& vObject)
        {
            this->AttributeList.append(vObject.AttributeList);
            this->StyleAttributeList.append(vObject.StyleAttributeList);
        }
    };

protected:
    QStringList m_HtmlDataList;

    QHash<uint32_t, QString> m_TableHeadHtmlDataHash; //<index, data>
    QHash<uint32_t, QStringList> m_TableDataHtmlDataHash; //<index, data>

    QHash<uint32_t, sCSSAttribute> m_CSSAttributeHash; //<CSS属性索引，CSS属性>

protected:
    /**
     * @brief Convert2HtmlData      转换生成html数据
     * @param vTagName              [in]标签名
     * @param vUserData             [in]用户数据
     * @param vStyleClassName       [in]样式属性类名
     * @param vAttributeList        [in]节点属性列表
     * @param vStyleAttributeList   [in]节点样式属性列表
     * @return
     */
    QString Convert2HtmlData(const QString vTagName, const QString vUserData, \
                             const QString vStyleClassName, const QStringList vAttributeList, const QStringList vStyleAttributeList);

private:
    void InitData(void);
    void AddDefaultStyleClass(void);

    /**
     * @brief GetSequenceID 获取顺序ID(!= 0)
     * @return
     */
    uint16_t GetSequenceID(void);

    /**
     * @brief GetAttribute          获取节点属性
     * @param vAttributeKey         [in]节点属性键值
     * @param vAttributeValue       [in]节点属性数据
     * @return
     */
    QString GetAttribute(const QString vAttributeKey, const QString vAttributeValue);

    /**
     * @brief GetStyleAttribute     获取节点样式属性
     * @param vStyleAttributeKey    [in]节点样式属性键值
     * @param vStyleAttributeValue  [in]节点样式属性数据
     * @return
     */
    QString GetStyleAttribute(const QString vStyleAttributeKey, const QString vStyleAttributeValue);

    //attribute data convert to css attribute
    sCSSAttribute GetCSSAttributes(nsHtmlForCSS::CFontAttribute vAttributeData);
    sCSSAttribute GetCSSAttributes(nsHtmlForCSS::CTextAttribute vAttributeData);
    sCSSAttribute GetCSSAttributes(nsHtmlForCSS::CBorderAttribute vAttributeData);

    //enum convert to string
    QString Enum2STR(nsHtmlForCSS::eAlignType vEnumType);
    QString Enum2STR(nsHtmlForCSS::eValignType vEnumType);
    QString Enum2STR(nsHtmlForCSS::eColorType vEnumType);
    QString Enum2STR(nsHtmlForCSS::eWhiteSpaceType vEnumType);
    QString Enum2STR(nsHtmlForCSS::eLengthType vEnumType);
    QString Enum2STR(nsHtmlForCSS::eFontStyleType vEnumType);
    QString Enum2STR(nsHtmlForCSS::eFontWeightType vEnumType);
    QString Enum2STR(nsHtmlForCSS::eFontFamilyType vEnumType);
    QString Enum2STR(nsHtmlForCSS::eBorderStyleType vEnumType);
    QString Enum2STR(nsHtmlForCSS::eBorderWidthType vEnumType);
    QString Enum2STR(nsHtmlForCSS::eBorderCollapseType vEnumType);

private:
    volatile uint16_t m_SequenceID = 0;
    QHash<nsHtmlForCSS::eAttributeType, QString> m_AttributeTypeHash; //<type, key name>
    QHash<nsHtmlForCSS::eStyleAttributeType, QString> m_StyleAttributeTypeHash; //<type, key name>

    QString     m_DefaultStyleClassName;
    QStringList m_DefaultStyleAttributeList;
};

#endif // CHTMLFORCSS_H
