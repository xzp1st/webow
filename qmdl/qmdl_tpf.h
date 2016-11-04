
#ifndef __QMDL_IOS_H__
#define __QMDL_IOS_H__

#include "qmdl_config.h"

#ifdef QMDL_VER_TPF

#include <stdio.h>
#include <Windows.h>
#include <string>

#include "qmdl_module.h"
#include "qmdl_base.h"

//
// 定义这个宏之后，默认调用的类工厂是TenFact.dll
//
#ifndef TENIO_SPEC_FACT
#	define	TENIO_SPEC_FACT
#endif	// TENIO_SPEC_FACT

#include "tenio_t.h"
#include "tenioapp.h"				// Tenio::Initialize
#include "UILib\uilib_i.h"			// IUILib
#include "teniopointer_t.h"		    // TeionPtr
#include "componentid_tpf.h"
#include "uilib\ScriptEventDefine.h"
#include "UIProperty\propertyid.h"
#include "UIProperty\tpf_propertyid.h"

using namespace Tenio;

typedef struct tagQMainPropInfo
{
    DWORD dwprop;
    DWORD dwtype;

    tagQMainPropInfo()
    {
        dwprop = 0;
        dwtype = 0;
    }

    tagQMainPropInfo(DWORD prop)
    {
        dwprop = prop;
        dwtype = EVT_STRING;
    }

    tagQMainPropInfo(DWORD prop, DWORD type)
    {
        dwprop = prop;
        dwtype = type;
    }

    tagQMainPropInfo(tagQMainPropInfo &info)
    {
        dwprop = info.dwprop;
        dwtype = info.dwtype;
    }
} QMainPropInfo;

typedef struct tagQMainTypeInfo
{
    DWORD dwtype;
    std::map<string, QMainPropInfo> mapprops;

    tagQMainTypeInfo(DWORD type)
    {
        dwtype = type;
    }
} QMainTypeInfo;

// -- 系统标准控件

// - QUIView

class QUIView;

class QTPFView:
    public IUILibEvent,
    public TTenio<QTPFView>
{
public:
    virtual ~QTPFView();
    virtual	void OnControlMessage(DWORD id, IVariableParam* param);
    virtual int GetEventVersion();

public:
    QUIView *mpview;
public:
    IUILib *mpuilib;
    DWORD mdwcontrol;
    IControl *mpcontrol;
    QMainTypeInfo *mpviewtype;
    QMainTypeInfo *mpcommontype;
    TTenioPtr<IVariableParam> mpparam;
};

QINT QUIViewInitWaitcb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitBase(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitHidden(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitActive(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitAttrcb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

QINT QUIViewSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUIView, QBaseView)
    qmdl_attributeb("attrcb",			    QUIViewInitAttrcb			)
    qmdl_attributeb("waitcb",			    QUIViewInitWaitcb			)
	qmdl_attribute_head(NULL, QUIViewInitBase)
		qmdl_attributeb("hidden",			QUIViewInitHidden			)
		qmdl_attributeb("active",			QUIViewInitActive			)
	qmdl_attribute_tail

	qmdl_self_cb(QUIViewSelfCb)
qmdl_class
public:
	QUIView();
	~QUIView();

	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);
	virtual QINT MakeModuleInit(QMDL env, QMDL parent, QXML mxml, QSTR url);
	virtual QINT MakeModuleEnd(QMDL env, QXML mxml, QSTR url);

	virtual QINT SetState(QINT state, QINT flag);
	virtual QINT GetState(QINT state);
	virtual void SetPosition(QRECT *rect);
	virtual void GetPosition(QRECT *rect);

    QUIView *GetRootView();
    QUIView *GetParentView();
	QINT GetAttrByName(QMDL module, QXML mxml, QSTR url, QPNT name, QPNT value, QINT size);
	QHDL FindViewByTag(QMDL env, QMDL parent, QMDL *module, QXML mxml, QSTR url, QINT *tag);

public:
    QTPFView mcview;
qmdl_end


// - QUIWindow

QINT QUIWindowSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUIWindow, QUIView)
	qmdl_self_cb(QUIWindowSelfCb)
qmdl_class
public:
	QUIWindow();
	~QUIWindow();
	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);

	QINT PushWindow();
	QINT PopWindow();
	QINT PaintWindow();

private:
qmdl_end


// - QUITpf

QINT QUITpfInitClass(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUITpfInitNibName(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUITpfInitPath(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUITpf, QUIView)
	qmdl_attribute_head(NULL, NULL)
		qmdl_attribute("class",			QUITpfInitClass			)
		qmdl_attribute("nibname",		QUITpfInitNibName		)
		qmdl_attribute("path",			QUITpfInitPath			)
	qmdl_attribute_tail
qmdl_class
public:
	QUITpf();
	~QUITpf();
	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);

private:
qmdl_end


// - QUILabel

typedef enum qui_label_style
{
	QUILabelStyleLeft				= 1 << 0,
	QUILabelStyleHCenter			= 1 << 1,
	QUILabelStyleRight				= 1 << 2,
	QUILabelStyleTop				= 1 << 3,
	QUILabelStyleVCenter			= 1 << 4,
	QUILabelStyleBottom				= 1 << 5,
	QUILabelStyleCenter				= (QUILabelStyleHCenter|QUILabelStyleVCenter),
	
	QUILabelStyleWordWrap			= 1 << 6,
	QUILabelStyleCharWrap			= 1 << 7,
	QUILabelStyleClip				= 1 << 8,
	QUILabelStyleHeadTrunc			= 1 << 9,
	QUILabelStyleTailTrunc			= 1 << 10,
	QUILabelStyleMiddleTrunc		= 1 << 11,
	
	QUILabelStyleFitFont			= 1 << 12,
} QUILabelStyle;

qstr_enum_extern(QUILabelStyleMap);

QINT QUILabelInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitText(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitFont(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitHighlightColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitShadowColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitShadowOffset(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitLines(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUILabel, QUIView)
	qmdl_attribute_head(NULL, NULL)
		qmdl_attribute("style",			QUILabelInitStyle					)
		qmdl_attribute("text",			QUILabelInitText					)
		qmdl_attribute("font",			QUILabelInitFont					)
		qmdl_attribute_head("highlight", NULL)
			qmdl_attribute("color",		QUILabelInitHighlightColor			)
		qmdl_attribute_tail
		qmdl_attribute_head("shadow", NULL)
			qmdl_attribute("color",		QUILabelInitShadowColor				)
			qmdl_attribute("offset",	QUILabelInitShadowOffset			)
		qmdl_attribute_tail
		qmdl_attribute("lines",			QUILabelInitLines					)
	qmdl_attribute_tail
qmdl_class
public:
	QUILabel();
	~QUILabel();
	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);

private:
qmdl_end


// - QUIButton

QINT QUIButtonInitBase(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitFont(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitNormalTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitNormalColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitNormalImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitNormalBackgroundImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitHighlightTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitHighlightImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitHighlightBackgroundImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitDisableTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitDisableImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitDisableBackgroundImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitSelectedTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitSelectedImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitSelectedBackgroundImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUIButton, QUIView)
	qmdl_attribute_head(NULL, QUIButtonInitBase)
		qmdl_attribute("style",			NULL									)
		qmdl_attribute("font",			QUIButtonInitFont						)
		qmdl_attribute("title",			QUIButtonInitNormalTitle				)
		qmdl_attribute("color",			QUIButtonInitNormalColor				)
		qmdl_attribute_head("normal", NULL)
			qmdl_attribute("title",		QUIButtonInitNormalTitle				)
			qmdl_attribute("color",		QUIButtonInitNormalColor				)
			qmdl_attribute("image",		QUIButtonInitNormalImage				)
			qmdl_attribute("BackgroundImage",QUIButtonInitNormalBackgroundImage	)
		qmdl_attribute_tail
		qmdl_attribute_head("highlight", NULL)
			qmdl_attribute("title",		QUIButtonInitHighlightTitle				)
			qmdl_attribute("image",		QUIButtonInitHighlightImage				)
			qmdl_attribute("BackgroundImage",QUIButtonInitHighlightBackgroundImage)
		qmdl_attribute_tail
		qmdl_attribute_head("disable", NULL)
			qmdl_attribute("title",		QUIButtonInitDisableTitle				)
			qmdl_attribute("image",		QUIButtonInitDisableImage				)
			qmdl_attribute("BackgroundImage",QUIButtonInitDisableBackgroundImage	)
		qmdl_attribute_tail
		qmdl_attribute_head("selected", NULL)
			qmdl_attribute("title",		QUIButtonInitSelectedTitle				)
			qmdl_attribute("image",		QUIButtonInitSelectedImage				)
			qmdl_attribute("BackgroundImage",QUIButtonInitSelectedBackgroundImage)
		qmdl_attribute_tail
	qmdl_attribute_tail
qmdl_class
public:
	QUIButton();
	~QUIButton();
	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);

private:
qmdl_end


// - QUIEdit

qmdl_module(QUIEdit, QUIView)
	qmdl_attribute_head(NULL, NULL)
	qmdl_attribute_tail
qmdl_class
public:
	QUIEdit();
	~QUIEdit();
	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);

private:
qmdl_end


// - QUIList

qmdl_module(QUIList, QUIView)
	qmdl_attribute_head(NULL, NULL)
	qmdl_attribute_tail
qmdl_class
public:
	QUIList();
	~QUIList();
	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);

private:
qmdl_end


// - QUISlider

qmdl_module(QUISlider, QUIView)
	qmdl_attribute_head(NULL, NULL)
	qmdl_attribute_tail
qmdl_class
public:
	QUISlider();
	~QUISlider();
	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);

private:
qmdl_end


QINT QMainInitApplication(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QMain, QBaseMain)

	qmdl_include("QMain",						"/lib/",		QMain			)

	qmdl_include_head("uilib")
		qmdl_include("view",					QPATH_LIB,		QUIView			)
		qmdl_include("window",					QPATH_LIB,		QUIWindow		)
		qmdl_include("tpf",						QPATH_LIB,		QUITpf			)

		qmdl_include("label",					QPATH_LIB,		QUILabel		)
		qmdl_include("image",					QPATH_LIB,		QUILabel		)
		qmdl_include("picker",					QPATH_LIB,		QUILabel		)
		qmdl_include("progress",				QPATH_LIB,		QUILabel		)
		qmdl_include("UIActivityIndicatorView",	QPATH_LIB,		QUILabel		)

		qmdl_include("button",					QPATH_LIB,		QUIButton		)
		qmdl_include("edit",					QPATH_LIB,		QUIEdit			)
		qmdl_include("slider",					QPATH_LIB,		QUISlider		)
		qmdl_include("UISwitch",				QPATH_LIB,		QUILabel		)
		qmdl_include("UIPageControl",			QPATH_LIB,		QUILabel		)
		qmdl_include("UIDatePicker",			QPATH_LIB,		QUILabel		)
		qmdl_include("UISegmentedControl",		QPATH_LIB,		QUILabel		)

		qmdl_include("UIScrollView",			QPATH_LIB,		QUILabel		)
		qmdl_include("list",					QPATH_LIB,		QUIList			)
		qmdl_include("UICollectionView",		QPATH_LIB,		QUILabel		)
		qmdl_include("UITextView",				QPATH_LIB,		QUILabel		)

		qmdl_include("UITabBar",				QPATH_LIB,		QUILabel		)
		qmdl_include("UIToolBar",				QPATH_LIB,		QUILabel		)
		qmdl_include("UINavigationBar",			QPATH_LIB,		QUILabel		)

		qmdl_include("UIActionSheet",			QPATH_LIB,		QUILabel		)
		qmdl_include("UIAlertView",				QPATH_LIB,		QUILabel		)
	qmdl_include_tail

	qmdl_attribute_head(NULL, NULL)
		qmdl_attribute("application",		QMainInitApplication			)
	qmdl_attribute_tail

qmdl_class
public:
	QMain();
	~QMain();
	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);

	virtual QINT PostEvent(QMDL module, QSTR name, QINT code, QPNT params[], QINT count);
	virtual QINT SendEvent(QMDL module, QSTR name, QINT code, QPNT params[], QINT count);
	virtual QINT ShowModule(QMDL module);
	virtual QINT HideModule(QMDL module);
	virtual QINT PushModule(QMDL module);
	virtual QINT PopModule(QMDL module);

	virtual QINT LoadString(QSTR file, QSTR name, QSTR value, QINT size);
	virtual QINT GetScreenRect(QRECT *rect);

public:
    HWND mhwnd;
    WNDPROC mpwndproc;

    std::map<string, QMainTypeInfo*> mmaptypes;

private:
    void InitTypes();
    void ExitTypes();
    void InitWndproc();
    void ExitWndproc();

	// 加入部份控件的公用属性
	void AddGridProperties(QMainTypeInfo *info);
	void AddTextProperties(QMainTypeInfo *info);
	void AddMultiLineTextProperties(QMainTypeInfo *info);
	void AddRangeProperties(QMainTypeInfo *info);
	void AddStateBgProperties(QMainTypeInfo *info);
	void AddStateImageProperties(QMainTypeInfo *info);
	// 初始化所有控件的公用属性
	void InitCommonPropertyPropertyMap();
	void InitCommonEventPropertyMap();
	// 初始化每个控件的私有属性，在这里可以加入部分控件的公用属性
	void InitBeelinePropertyMap();
	void InitBehaviorDesignerPropertyMap();
	void InitButtonPropertyMap();
	void InitTinyButtonPropertyMap();
	void InitCardCtrlPropertyMap();
	void InitCheckBoxPropertyMap();
	void InitComboBoxPropertyMap();
	void InitContainerPropertyMap();
	void InitDialogPropertyMap();
	void InitDigitPicBoxPropertyMap();
	void InitEditPropertyMap();
	void InitExcluderPropertyMap();
	void InitHeaderPropertyMap();
	void InitWin32ImageListPropertyMap();
	void InitTPFImageListPropertyMap();
	void InitLabelPropertyMap();
	void InitLevelPropertyMap();
	void InitListBoxPropertyMap();
	void InitListViewPropertyMap();
	void InitMenuPropertyMap();
	void InitMultiColorCtrlPropertyMap();
	void InitMultiImageVerminPropertyMap();
	void InitMultiImageCtrlPropertyMap();
	void InitOverlapContainerPropertyMap();
	void InitRichEditPropertyMap();
	void InitRichEditViewPropertyMap();
	void InitScrollBarPropertyMap();
	void InitScrollViewPropertyMap();
	void InitSignaledSwitchPropertyMap();
	void InitSliderPropertyMap();
	void InitStaticPropertyMap();
	void InitTablePropertyMap();
	void InitTimeKeeperPropertyMap();
	void InitTitleBarPropertyMap();
	void InitToolTipPropertyMap();
	void InitWin32TreePropertyMap();
	void InitTPFTreePropertyMap();
	void InitWebBrowserPropertyMap();
	void InitTurnTablePropertyMap();
	void InitPosterPropertyMap();
	void InitQQMJPropertyMap();
	void InitUserObjectPropertyMap();
	void InitSplitterPropertyMap();
	void InitIconboxPropertyMap();
	void InitIconGridPropertyMap();
	void InitRichEdit2PropertyMap();
	void InitRichTextBoxPropertyMap();
	void InitWordCardCtrlPropertyMap();
	void InitToolTipBinderPropertyMap();
	void InitIconbufferPropertyMap();
    void InitRadarPropertyMap();
	void InitSoftKeyBoardPropertyMap();
	void InitMarqueePropertyMap();
	void InitBehaviorDesigner2PropertyMap();
	void InitAccordionPropertyMap();
	void InitCustomControlsPropertyMap();

qmdl_end


// -- 公共函数


#endif //QMDL_VER_IOS

#endif //__QMDL_IOS_H__
