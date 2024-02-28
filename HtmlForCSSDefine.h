#ifndef HTMLFORCSSDEFINE_H
#define HTMLFORCSSDEFINE_H

#include <QString>

namespace nsHtmlForCSS {

/**
 * 数据类型定义
 */

typedef enum
{
    //通用属性
    e_Attribute_Align,
    e_Attribute_Valign,
    e_Attribute_Width,

    //边框属性
    e_Attribute_Border,

    //表格属性
    e_Attribute_Cellspacing,
    e_Attribute_Cellpadding,

}eAttributeType;

typedef enum
{
    //通用样式属性
    e_StyleAttribute_Height,                //高度
    e_StyleAttribute_Width,                 //宽度
    e_StyleAttribute_MaxWidth,              //最大宽度
    e_StyleAttribute_Margin,                //外边距
    e_StyleAttribute_Padding,               //内边距
    e_StyleAttribute_BackgroundColor,       //背景颜色

    //边框样式属性
    e_StyleAttribute_Border,                //边框属性总
    e_StyleAttribute_BorderStyle,           //边框样式
    e_StyleAttribute_BorderWidth,           //边框宽度
    e_StyleAttribute_BorderColor,           //边框颜色
    e_StyleAttribute_BorderCollapse,        //边框合并
    e_StyleAttribute_BorderSpacing,         //边框间距

    //文本样式属性
    e_StyleAttribute_TextColor,             //文本颜色
    e_StyleAttribute_TextAlign,             //文本水平对齐
    e_StyleAttribute_TextValign,            //文本垂直对齐
    e_StyleAttribute_TextIndent,            //文本缩进
    e_StyleAttribute_TextWhiteSpace,        //文本空白符属性

    //字体样式属性
    e_StyleAttribute_FontSize,              //字体大小
    e_StyleAttribute_FontStyle,             //字体样式
    e_StyleAttribute_FontWeight,            //字体宽度
    e_StyleAttribute_FontFamily,            //字体系列

}eStyleAttributeType;

//水平对齐
typedef enum
{
    e_Align_Left        = 1,    //左对齐
    e_Align_Right       = 2,    //右对齐
    e_Align_Center      = 3,    //居中对齐
    e_Align_Justify     = 4,    //两端对齐
}eAlignType;

//垂直对齐
typedef enum
{
    e_Valign_Top        = 1,    //上对齐
    e_Valign_Middle     = 2,    //居中对齐
    e_Valign_Bottom     = 3,    //下对齐
    e_Valign_Baseline   = 4,    //基线对齐
}eValignType;

//色彩板
typedef enum
{
    e_Color_Black       = 0x000000,     //黑
    e_Color_Blue        = 0x0000FF,     //蓝
    e_Color_Green       = 0x008000,     //绿
    e_Color_Red         = 0xFF0000,     //红
    e_Color_White       = 0xFFFFFF,     //白
    e_Color_Yellow      = 0xFFFF00,     //黄
}eColorType;

//空白符属性
typedef enum
{
    e_WhiteSpace_Normal     = 1,    //空白符会被忽略
    e_WhiteSpace_Pre        = 2,    //空白符会被保留
    e_WhiteSpace_Nowrap     = 3,    //文本不会换行，直到遇到<br>标签为止
    e_WhiteSpace_PreWrap    = 4,    //保留空白符序列，但是正常地进行换行
    e_WhiteSpace_PreLine    = 5,    //合并空白符序列，但是保留换行符
    e_WhiteSpace_Inherit    = 6,    //继承父元素属性
}eWhiteSpaceType;

//长度单位
typedef enum
{
    e_Length_PX     = 1,    //像素
    e_Length_PT     = 2,    //点
    e_Length_EM     = 3,    //相对于元素的字体大小
}eLengthType;

//字体样式
typedef enum
{
    e_FontStyle_Normal  = 1,    //常规
    e_FontStyle_Italic  = 2,    //斜体
}eFontStyleType;

//字体宽度
typedef enum
{
    e_FontWeight_Normal = 1,    //常规
    e_FontWeight_Bold   = 2,    //加粗
}eFontWeightType;

//字体系列
typedef enum
{
    e_FontFamily_SongTi,            //宋体
    e_FontFamily_Black,             //黑体
    e_FontFamily_Arial,             //Arial
    e_FontFamily_TimesNewRoman,     //Times New Roman
}eFontFamilyType;

//边框样式
typedef enum
{
    e_BorderStyle_Dotted    = 1,    //定义点线边框
    e_BorderStyle_Dashed    = 2,    //定义虚线边框
    e_BorderStyle_Solid     = 3,    //定义实线边框
    e_BorderStyle_Double    = 4,    //定义双边框
    e_BorderStyle_Groove    = 5,    //定义 3D 坡口边框。效果取决于 border-color 值
    e_BorderStyle_Ridge     = 6,    //定义 3D 脊线边框。效果取决于 border-color 值
    e_BorderStyle_Inset     = 7,    //定义 3D inset 边框。效果取决于 border-color 值
    e_BorderStyle_Outset    = 8,    //定义 3D outset 边框。效果取决于 border-color 值
    e_BorderStyle_None      = 9,    //定义无边框
    e_BorderStyle_Hidden    = 10,   //定义隐藏边框
}eBorderStyleType;

//边框宽度
typedef enum
{
    e_BorderWidth_Thin      = 1,    //细边框
    e_BorderWidth_Medium    = 2,    //中等边框
    e_BorderWidth_Thick     = 3,    //粗边框
}eBorderWidthType;

//边框合并
typedef enum
{
    e_BorderCollapse_Separate   = 1,    //边框分离
    e_BorderCollapse_Collapse   = 2,    //边框合并
    e_BorderCollapse_Inherit    = 3,    //继承父元素属性
}eBorderCollapseType;

//---------------------------------------------------------------------------

/**
 * 数据结构/类定义
 */

//字体属性
class CFontAttribute
{
public:
    float           m_FontSize          = 16;                   //字体大小
    eLengthType     m_LengthType        = e_Length_PX;          //字体大小单位
    eFontStyleType  m_FontStyleType     = e_FontStyle_Normal;   //字体样式
    eFontWeightType m_FontWeightType    = e_FontWeight_Normal;  //字体宽度
    eFontFamilyType m_FontFamilyType    = e_FontFamily_SongTi;  //字体系列
};

//文本属性
class CTextAttribute
{
public:
    eColorType      m_ColorType         = e_Color_Black;    //文本颜色
    eAlignType      m_AlignType         = e_Align_Left;     //文本水平对齐
    eValignType     m_ValignType        = e_Valign_Top;     //文本垂直对齐
    eWhiteSpaceType m_WhiteSpaceType    = e_WhiteSpace_Pre; //文本空白符属性，2024.01.23 cjh：当前没有使用需求
};

//边框属性
class CBorderAttribute
{
public:
    float               m_BorderSize            = 1;                    //边框宽度
    eLengthType         m_LengthType            = e_Length_PX;          //边框宽度单位
    bool                m_IsUseBorderWidthType  = false;                //启用时，忽略BorderSize
    eBorderWidthType    m_BorderWidthType       = e_BorderWidth_Thin;   //边框宽度类型

    eColorType          m_ColorType             = e_Color_Black;        //边框颜色
    eBorderStyleType    m_BorderStyleType       = e_BorderStyle_Solid;  //边框样式
};

//表格数据属性
class CTableDataAttribute
{
public:
    CBorderAttribute m_BorderAttribute;
    CFontAttribute  m_FontAttribute;
    CTextAttribute  m_TextAttribute;

    bool        m_IsUseBgColor  = false;            //是否使用单元格背景颜色
    eColorType  m_BgColorType   = e_Color_White;    //单元格背景颜色

    float       m_PaddingSize       = 6;            //单元格边沿到内容之间的间距
    eLengthType m_PaddingLengthType = e_Length_PX;  //单元格边沿到内容之间的间距单位
};

//表格表头数据属性
class CTableHeadAttribute : public CTableDataAttribute
{
public:
    bool        m_IsUseWidthRange   = false;        //是否使用列宽比例
    uint32_t    m_WidthRange        = 100;          //列宽比例，相对于表格宽度
};

//表格属性
class CTableAttribute
{
public:
    CBorderAttribute m_BorderAttribute;

    bool            m_IsAlignCenter         = true;     //表格水平居中
    uint32_t        m_WidthRange            = 100;      //表格宽度比例，相对于文档宽度
    bool            m_IsCollapseCellBorder  = true;     //单元格之间的边框是否折叠

    float           m_BorderSpacing             = 0;            //边框间距
    eLengthType     m_BorderSpacingLengthType   = e_Length_PX;  //边框间距单位
};

//段落属性
class CParagraphAttribute
{
public:
    CFontAttribute  m_FontAttribute;
    CTextAttribute  m_TextAttribute;

    bool        m_IsUseBgColor  = false;            //是否使用背景颜色
    eColorType  m_BgColorType   = e_Color_White;    //背景颜色

    float       m_TextIndent    = 0;                //文本缩进
    eLengthType m_LengthType    = e_Length_EM;      //文本缩进长度单位
};

} //namespace nsHtmlForCSS

#endif // HTMLFORCSSDEFINE_H
