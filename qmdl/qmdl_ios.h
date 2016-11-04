
#ifndef __QMDL_IOS_H__
#define __QMDL_IOS_H__

#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

#include "qmdl_module.h"
#include "qmdl_base.h"


// -- 通用枚举定义

typedef enum qui_state_code
{
	QUIStateNormal       = 0,
	QUIStateHighlight    = 1 << 0,                  // used when UIControl is Highlight is set
	QUIStateDisable      = 1 << 1,
	QUIStateSelected     = 1 << 2,                  // flag usable by app (see below)
	QUIStateApplication  = 0x00FF0000,              // additional flags available for application use
	QUIStateReserved     = 0xFF000000               // flags reserved for internal framework use
} QUIState;


// -- 系统标准控件

// - QUIView

QINT QUIViewInitHidden(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitChildrenHidden(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitActive(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitBackgroundColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

QINT QUIViewSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUIView, QBaseView)
	qmdl_attribute_head(NULL, NULL)
		qmdl_attributeb("hidden",			QUIViewInitHidden			)
		qmdl_attributeb("children-hidden",	QUIViewInitChildrenHidden	)
		qmdl_attributeb("active",			QUIViewInitActive			)

		qmdl_attribute_head("background", NULL)
			qmdl_attribute("color",			QUIViewInitBackgroundColor	)
		qmdl_attribute_tail
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

	QINT GetAttrByName(QMDL module, QXML mxml, QSTR url, QPNT name, QPNT value, QINT size);
	id FindViewByTag(QMDL env, QMDL parent, QMDL *module, QXML mxml, QSTR url, QINT *tag);

public:
	id midview;
	id midevent;
	id midcontroller;
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

public:
	id midapplication;
	id middelegate;

private:
qmdl_end


// - QUIXib

QINT QUIXibInitClass(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIXibInitNibName(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIXibInitPath(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUIXib, QUIView)
	qmdl_attribute_head(NULL, NULL)
		qmdl_attribute("class",			QUIXibInitClass			)
		qmdl_attribute("nibname",		QUIXibInitNibName		)
		qmdl_attribute("path",			QUIXibInitPath			)
	qmdl_attribute_tail
qmdl_class
public:
	QUIXib();
	~QUIXib();
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
QINT QMainInitDelegate(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QMain, QBaseMain)

	qmdl_include("QMain",						"/lib/",		QMain			)

	qmdl_include_head("uilib")
		qmdl_include("view",					QPATH_LIB,		QUIView			)
		qmdl_include("window",					QPATH_LIB,		QUIWindow		)
		qmdl_include("xib",						QPATH_LIB,		QUIXib			)

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
		qmdl_attribute("app",				QMainInitApplication			)
		qmdl_attribute("application",		QMainInitApplication			)
		qmdl_attribute("delegate",			QMainInitDelegate				)
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
	id midapplication;
	id middelegate;

qmdl_end


// -- 公共函数

UIColor *quiStr2UIColor(QSTR color);
QINT quiMakeUIView(QMDL env, QMDL parent, QMDL module, QXML mxml, QSTR url);


#endif //QMDL_VER_IOS

#endif //__QMDL_IOS_H__
