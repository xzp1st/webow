
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

qstr_enum_extern(QFontEnum);

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
			qmdl_attributex("color",		QUIViewInitBackgroundColor,			QCLREnum)
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
	virtual void InvalidateDisplay(QRECT *rect);

	void InitUIEvent();

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

QINT QUIXibSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

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
	QLBLS_Left				= 1 << 0,
	QLBLS_HCenter			= 1 << 1,
	QLBLS_Right				= 1 << 2,
	QLBLS_Top				= 1 << 3,
	QLBLS_VCenter			= 1 << 4,
	QLBLS_Bottom			= 1 << 5,
	QLBLS_Center			= (QLBLS_HCenter|QLBLS_VCenter),
	
	QLBLS_WordWrap			= 1 << 6,
	QLBLS_CharWrap			= 1 << 7,
	QLBLS_Clip				= 1 << 8,
	QLBLS_HeadTrunc			= 1 << 9,
	QLBLS_TailTrunc			= 1 << 10,
	QLBLS_MiddleTrunc		= 1 << 11,
	
	QLBLS_FitFont			= 1 << 12,
} QUILabelStyle;

qstr_enum_extern(QLBLSEnum);

QINT QUILabelInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitText(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitFont(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitHighlightColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitShadowColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitShadowOffset(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitLines(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

QINT QUILabelSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUILabel, QUIView)
	qmdl_attribute_head(NULL, NULL)
		qmdl_attributex("style",		QUILabelInitStyle,			QLBLSEnum	)
		qmdl_attributes("text",			QUILabelInitText						)
		qmdl_attributex("font",			QUILabelInitFont,			QFontEnum	)
		qmdl_attribute_head("highlight", NULL)
			qmdl_attributex("color",	QUILabelInitHighlightColor,	QCLREnum	)
		qmdl_attribute_tail
		qmdl_attribute_head("shadow", NULL)
			qmdl_attributex("color",	QUILabelInitShadowColor,	QCLREnum	)
			qmdl_attributes("offset",	QUILabelInitShadowOffset				)
		qmdl_attribute_tail
		qmdl_attributen("lines",		QUILabelInitLines						)
	qmdl_attribute_tail

	qmdl_self_cb(QUILabelSelfCb)
qmdl_class
public:
	QINT mnstyle;

public:
	QUILabel();
	~QUILabel();

private:
qmdl_end


// - QUIImage

QINT QUIImageInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIImageInitText(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIImageInitFont(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIImageInitHighlightColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIImageInitShadowColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIImageInitShadowOffset(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIImageInitLines(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

QINT QUIImageSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUIImage, QUIView)
	qmdl_attribute_head(NULL, NULL)
		qmdl_attributex("style",		QUIImageInitStyle,			QLBLSEnum	)
		qmdl_attributes("text",			QUIImageInitText						)
		qmdl_attributex("font",			QUIImageInitFont,			QFontEnum	)
		qmdl_attributen("lines",		QUIImageInitLines						)
		qmdl_attribute_head("highlight", NULL)
			qmdl_attributex("color",	QUIImageInitHighlightColor,	QCLREnum	)
		qmdl_attribute_tail
		qmdl_attribute_head("shadow", NULL)
			qmdl_attributex("color",	QUIImageInitShadowColor,	QCLREnum	)
			qmdl_attributes("offset",	QUIImageInitShadowOffset				)
		qmdl_attribute_tail
	qmdl_attribute_tail

	qmdl_self_cb(QUIImageSelfCb)
qmdl_class
public:
	QINT mnstyle;

public:
	QUIImage();
	~QUIImage();

private:
qmdl_end


// - QUIButton

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

QINT QUIButtonSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUIButton, QUIView)
	qmdl_attribute_head(NULL, NULL)
		qmdl_attributen("style",			NULL									)
		qmdl_attributex("font",			QUIButtonInitFont,			QFontEnum	)
		qmdl_attributes("title",		QUIButtonInitNormalTitle				)
		qmdl_attributex("color",		QUIButtonInitNormalColor,	QCLREnum	)
		qmdl_attribute_head("normal", NULL)
			qmdl_attributes("title",	QUIButtonInitNormalTitle				)
			qmdl_attributex("color",	QUIButtonInitNormalColor,	QCLREnum	)
			qmdl_attributes("image",	QUIButtonInitNormalImage				)
			qmdl_attributes("BackgroundImage",QUIButtonInitNormalBackgroundImage)
		qmdl_attribute_tail
		qmdl_attribute_head("highlight", NULL)
			qmdl_attributes("title",		QUIButtonInitHighlightTitle			)
			qmdl_attributes("image",		QUIButtonInitHighlightImage			)
			qmdl_attributes("BackgroundImage",QUIButtonInitHighlightBackgroundImage)
		qmdl_attribute_tail
		qmdl_attribute_head("disable", NULL)
			qmdl_attributes("title",		QUIButtonInitDisableTitle			)
			qmdl_attributes("image",		QUIButtonInitDisableImage			)
			qmdl_attributes("BackgroundImage",QUIButtonInitDisableBackgroundImage)
		qmdl_attribute_tail
		qmdl_attribute_head("selected", NULL)
			qmdl_attributes("title",		QUIButtonInitSelectedTitle			)
			qmdl_attributes("image",		QUIButtonInitSelectedImage			)
			qmdl_attributes("BackgroundImage",QUIButtonInitSelectedBackgroundImage)
		qmdl_attribute_tail
	qmdl_attribute_tail

	qmdl_self_cb(QUIButtonSelfCb)
qmdl_class
public:
	QUIButton();
	~QUIButton();
	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);

private:
qmdl_end


// - QUIEdit

typedef enum qui_edit_style
{
	QEDTS_BorderMask				= 0x000000FF,
	QEDTS_None						= UITextBorderStyleNone,			// 默认边框
	QEDTS_Line						= UITextBorderStyleLine,			// 线条边框
	QEDTS_Bezel						= UITextBorderStyleBezel,
	QEDTS_RoundedRect				= UITextBorderStyleRoundedRect,		// 圆角边框
	QEDTS_Round						= QEDTS_RoundedRect,
	
	QEDTS_MultiLine					= 1 << 8,							// 密码显示
	QEDTS_Password					= 1 << 9,							// 密码显示
	QEDTS_ReadOnly					= 1 << 10,							// 只读
	QEDTS_ClearsOnBeginEditing		= 1 << 11,							// 重新编辑前清空旧有内容
	QEDTS_AutoClear					= QEDTS_ClearsOnBeginEditing,
	QEDTS_FitFont					= 1 << 12,							// 自动匹配字体大小，MinFontSize为最小字体大小
	
	QEDTS_WordCap					= 1 << 13,							// 单词首字母大写
	QEDTS_SentenceCap				= 1 << 14,							// 句子首字母大写
	QEDTS_CharCap					= 1 << 15,							// 所有字符大写
	
	QEDTS_DefCorrection				= 1 << 16,							// 默认纠错
	QEDTS_HasCorrection				= 1 << 17,							// 强制纠错
	QEDTS_NoCorrection				= 1 << 18,							// 不做纠错
	
	QEDTS_AlignMask					= 0xFF000000,
	QEDTS_AlignLeft					= 1 << 24,							// 靠左
	QEDTS_AlignHCenter				= 1 << 25,							// 水平居中
	QEDTS_AlignRight				= 1 << 26,							// 靠右
	QEDTS_AlignTop					= 1 << 27,							// 靠上
	QEDTS_AlignVCenter				= 1 << 28,							// 垂直居中
	QEDTS_AlignBottom				= 1 << 29,							// 靠下
	QEDTS_AlignFill					= 1 << 30,							// 垂直填满
	QEDTS_AlignCenter				= (QEDTS_AlignHCenter|QEDTS_AlignVCenter),// 居中
} QUIEditStyle;

typedef enum qui_edit_view_mode
{
	QEDTViewMode_Never				= UITextFieldViewModeNever,			// 从不出现
	QEDTViewMode_WhileEditing		= UITextFieldViewModeWhileEditing,	// 编辑中出现
	QEDTViewMode_UnlessEditing		= UITextFieldViewModeUnlessEditing,	// 非编辑中出现
	QEDTViewMode_Always				= UITextFieldViewModeAlways,		// 一直出现
	QEDTViewMode_Editing			= QEDTViewMode_WhileEditing,
	QEDTViewMode_Idle				= QEDTViewMode_UnlessEditing,
} QUIEditViewMode;

typedef enum qui_edit_keyboard
{
	QEDTKeyboard_TypeMask			= 0x000000FF,
	QEDTKeyboard_TypeMove			= 0x00000000,
	QEDTKeyboard_Default			= UIKeyboardTypeDefault,
	QEDTKeyboard_ASC				= UIKeyboardTypeASCIICapable,
	QEDTKeyboard_NumberPunctuation	= UIKeyboardTypeNumbersAndPunctuation,
	QEDTKeyboard_URL				= UIKeyboardTypeURL,
	QEDTKeyboard_Number				= UIKeyboardTypeNumberPad,
	QEDTKeyboard_Num				= UIKeyboardTypeNumberPad,
	QEDTKeyboard_Phone				= UIKeyboardTypePhonePad,
	QEDTKeyboard_NamePhone			= UIKeyboardTypeNamePhonePad,
	QEDTKeyboard_Email				= UIKeyboardTypeEmailAddress,
	QEDTKeyboard_Decimal			= UIKeyboardTypeDecimalPad,
	QEDTKeyboard_Twitter			= UIKeyboardTypeTwitter,
	QEDTKeyboard_Alphabet			= UIKeyboardTypeAlphabet,
	
	QEDTKeyboard_AppearanceMask		= 0x0000FF00,
	QEDTKeyboard_AppearanceMove		= 0x00000008,
	QEDTKeyboard_DefAppearance		= (UIKeyboardAppearanceDefault << 8),
	QEDTKeyboard_AlertAppearance	= (UIKeyboardAppearanceAlert << 8),
	QEDTKeyboard_DefaultAppearance	= QEDTKeyboard_DefAppearance,
	
	QEDTKeyboard_ReturnKeyMask		= 0x00FF0000,
	QEDTKeyboard_ReturnKeyMove		= 0x00000010,
	QEDTKeyboard_ReturnKeyDefault	= (UIReturnKeyDefault << 16),
	QEDTKeyboard_ReturnKeyGo		= (UIReturnKeyGo << 16),
	QEDTKeyboard_ReturnKeyGoogle	= (UIReturnKeyGoogle << 16),
	QEDTKeyboard_ReturnKeyJoin		= (UIReturnKeyJoin << 16),
	QEDTKeyboard_ReturnKeyNext		= (UIReturnKeyNext << 16),
	QEDTKeyboard_ReturnKeyRoute		= (UIReturnKeyRoute << 16),
	QEDTKeyboard_ReturnKeySearch	= (UIReturnKeySearch << 16),
	QEDTKeyboard_ReturnKeySend		= (UIReturnKeySend << 16),
	QEDTKeyboard_ReturnKeyYahoo		= (UIReturnKeyYahoo << 16),
	QEDTKeyboard_ReturnEmergencyCall= (UIReturnKeyEmergencyCall << 16),
} QUIEditKeyboard;

qstr_enum_extern(QEDTSEnum);
qstr_enum_extern(QEDTViewModeEnum);
qstr_enum_extern(QEDTKeyboardEnum);

QINT QUIEditInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitPassword(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitReadOnly(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitAutoCorrection(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitAutoClear(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitClearButtonMode(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitTextAlignment(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

QINT QUIEditInitFont(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitFitFont(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitMinFontSize(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

QINT QUIEditInitBackgroundColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitTextColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitBackgroundImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitDisabledImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitLeftViewMode(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitLeftView(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitRightViewMode(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitRightView(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

QINT QUIEditInitText(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitPlaceholder(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIEditInitKeyboard(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

QINT QUIEditSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUIEdit, QUIView)
	qmdl_attribute_head(NULL, NULL)
		qmdl_attributex("style",		QUIEditInitStyle,				QEDTSEnum			)
		qmdl_attributeb("password",		QUIEditInitPassword									)
		qmdl_attributeb("readonly",		QUIEditInitReadOnly									)
		qmdl_attributeb("AutoCorrection",QUIEditInitAutoCorrection							)
		qmdl_attributeb("Correction",	QUIEditInitAutoCorrection							)
		qmdl_attributeb("AutoClear",	QUIEditInitAutoClear								)
		qmdl_attributex("ClearButtonMode",QUIEditInitClearButtonMode,	QEDTViewModeEnum	)
		qmdl_attributex("ClearButton",	QUIEditInitClearButtonMode,		QEDTViewModeEnum	)
		qmdl_attributex("TextAlignment",QUIEditInitTextAlignment,		QEDTSEnum			)
		qmdl_attributex("align",		QUIEditInitTextAlignment,		QEDTSEnum			)

		qmdl_attributex("font",			QUIEditInitFont,				QFontEnum			)
		qmdl_attributeb("FitFont",		QUIEditInitFitFont									)
		qmdl_attributen("MinFontSize",	QUIEditInitMinFontSize								)

		qmdl_attributex("BackgroundColor",QUIEditInitBackgroundColor,	QCLREnum			)
		qmdl_attributex("TextColor",	QUIEditInitTextColor,			QCLREnum			)
		qmdl_attributes("BackgroundImage",QUIEditInitBackgroundImage						)
		qmdl_attributes("DisabledImage",QUIEditInitDisabledImage							)
		qmdl_attributex("LeftViewMode",	QUIEditInitLeftViewMode,		QEDTViewModeEnum	)
		qmdl_attributep("LeftView",		QUIEditInitLeftView									)
		qmdl_attributex("RightViewMode",QUIEditInitRightViewMode,		QEDTViewModeEnum	)
		qmdl_attributep("RightView",	QUIEditInitRightView								)

		qmdl_attributes("Text",			QUIEditInitText										)
		qmdl_attributes("Placeholder",	QUIEditInitPlaceholder								)
		qmdl_attributex("Keyboard",		QUIEditInitKeyboard,			QEDTKeyboardEnum	)
	qmdl_attribute_tail

	qmdl_self_cb(QUIEditSelfCb)
qmdl_class
public:
	QINT mnstyle;

public:
	QUIEdit();
	~QUIEdit();

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
		qmdl_include("image",					QPATH_LIB,		QUIImage		)
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
		qmdl_attribute("app",					QMainInitApplication			)
		qmdl_attribute("application",			QMainInitApplication			)
		qmdl_attribute("delegate",				QMainInitDelegate				)
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
UIColor *quiRGBA2UIColor(QINT color);
QINT quiUIColor2RGBA(UIColor *color);

NSString *quiUtf82NSString(QSTR str, QINT len);

QINT quiMakeUIView(QMDL env, QMDL parent, QMDL module, QXML mxml, QSTR url);


#endif //QMDL_VER_IOS

#endif //__QMDL_IOS_H__
