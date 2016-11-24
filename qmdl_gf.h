
#ifndef __QMDL_GF_H__
#define __QMDL_GF_H__

#include "qmdl_config.h"

#ifdef QMDL_VER_GF

//#include "Thirdparty_Component/Common/include/utilnetwork.h"
#include "Thirdparty_Component/Common/include/utildata.h"
#include "Thirdparty_Component/Common/include/utilcore.h"
#include "Thirdparty_Component/Common/include/stdtx.h"
#include "Thirdparty_Component/Common/include/common.h"
#include "Graphic/Include/xGraphic_Type.h"

#include "GF/Include/UtilGF.h"
#include "GF/Include/IGFMessage.h"
#include "GF/Include/UtilMsgEvt.h"
#include "GF/Include/IGFDrawCtrl.h"
#include "GF/Include/UtilGF.h"
#include "GF/Include/IGFFrame.h"
#include "GF/Include/IGFButton.h"
#include "GF/Include/IGFEdit.h"

#include "IMVCEventHandler.h"

#include "qmdl_module.h"
#include "qmdl_base.h"


// - QUIView

class QUIView;
class IQUIGFViewEvent;

class IQUIGFView : public IMVCModule
{
public:
	virtual HRESULT Initialize(IGFFrame *pParent) = 0;

	virtual void AddEventHandler(IQUIGFViewEvent *pEventHandler) = 0;
	virtual void RemoveEventHandler(IQUIGFViewEvent *pEventHandler) = 0;
};

class IQUIGFViewEvent : public IMVCEventBase
{
public:
};

class QUIGFView: public CMVCEventHandlerProducer< IQUIGFView, IQUIGFViewEvent >
{
public:
	virtual HRESULT Initialize(IGFFrame *pParent);

	QUIGFView();
	~QUIGFView();

	HRESULT OnMessage(IGFFrame *pTargetFrame, tagBaseArg *pArg, BOOL* pbWinMsgHandled, LRESULT *pRet, BOOL *pbProceedMsgFlow, BOOL *pbCallNextMsgListener);
	void OnGFEvent(int nEvent, IGFEventSupporter* pSupporter, ITXData* pData);

public:
	QUIView *mpview;

	CGFEventListenerHelper<QUIGFView> mpEventListener;
	CGFMessageListenerPtr<QUIGFView> mpMessageListener;

	CComPtr<IGFFrame> mpparent;
};

QINT QUIViewInitHidden(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitEnable(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitAlpha(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitOpaque(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitTransparent(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitMargin(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitPadding(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitTip(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

QINT QUIViewInitWidth(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitHeight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitLeft(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitTop(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitRight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitBottom(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitVCenter(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitHCenter(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitCenter(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIViewInitPosition(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

QINT QUIViewOnEvent(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUIView, QModule)
	qmdl_attribute_head(NULL)
		qmdl_attribute("hidden",			QUIViewInitHidden		)
		qmdl_attribute("enable",			QUIViewInitEnable		)
		qmdl_attribute("alpha",			QUIViewInitAlpha		)
		qmdl_attribute("opaque",			QUIViewInitOpaque		)
		qmdl_attribute("transparent",		QUIViewInitTransparent	)
		qmdl_attribute("margin",			QUIViewInitMargin		)
		qmdl_attribute("tip",				QUIViewInitTip			)

		qmdl_attribute("width",			QUIViewInitWidth		)
		qmdl_attribute("height",			QUIViewInitHeight		)
		qmdl_attribute("left",			QUIViewInitLeft			)
		qmdl_attribute("top",				QUIViewInitTop			)
		qmdl_attribute("right",			QUIViewInitRight		)
		qmdl_attribute("bottom",			QUIViewInitBottom		)
		qmdl_attribute("vcenter",			QUIViewInitVCenter		)
		qmdl_attribute("hcenter",			QUIViewInitHCenter		)
		qmdl_attribute("center",			QUIViewInitCenter		)
		qmdl_attribute("position",		QUIViewInitPosition		)
	qmdl_attribute_tail
qmdl_class
public:
	QUIView();
	~QUIView();

	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);
	virtual QINT MakeModuleInit(QMDL env, QMDL parent, QXML mxml, QSTR url);
	virtual QINT MakeModuleEnd(QMDL env, QXML mxml, QSTR url);

	IGFFrame *HasViewWithName(QMDL env, QMDL parent, QXML mxml, QSTR url, QSTR name, QINT size);

public:
	CComPtr<IGFFrame> mpframe;
	QUIGFView mcgfview;

private:
qmdl_end

// - QUIWindow

typedef enum qui_window_style
{
	QUIWindowStyleGray				= 0x00000001,
	QUIWindowStyleColorize			= 0x00000002,
	QUIWindowStyleCache				= 0x00000004,
	QUIWindowStyleAutoRotate			= 0x00000008,
} QUIWindowStyle;

qstr_enum_extern(QUIWindowStyleMap);

QINT QUIWindowInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIWindowInitTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUIWindow, QUIView)
	qmdl_attribute_head(NULL)
		qmdl_attribute("style",			QUIWindowInitStyle					)
		qmdl_attribute("title",			QUIWindowInitTitle					)
	qmdl_attribute_tail
qmdl_class
public:
	QUIWindow();
	~QUIWindow();
	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);

private:
qmdl_end


// - QUIXtml

QINT QUIXtmlInitPath(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUIXtml, QUIView)
	qmdl_attribute_head(NULL)
		qmdl_attribute("path",			QUIXtmlInitPath		)
	qmdl_attribute_tail
qmdl_class
public:
	QUIXtml();
	~QUIXtml();
	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);

private:
qmdl_end


// - QUILabel

typedef enum qui_label_style
{
	QUILabelStyleLeft				= DT_LEFT,
	QUILabelStyleHCenter			= DT_CENTER,
	QUILabelStyleRight				= DT_RIGHT,
	QUILabelStyleTop				= DT_TOP,
	QUILabelStyleVCenter			= DT_VCENTER,
	QUILabelStyleBottom				= DT_BOTTOM,
	QUILabelStyleCenter				= DT_CENTER,
	
	QUILabelStyleSingleLine			= DT_SINGLELINE,
	QUILabelStyleWordBreak			= DT_WORDBREAK,
	QUILabelStyleEllipsis			= 0x10000000,
	QUILabelStyleColorize			= 0x20000000,
} QUILabelStyle;

qstr_enum_extern(QUILabelStyleMap);

QINT QUILabelInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitText(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitFontName(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitFontWeight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitFontWidth(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitFontHeight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitMaxRow(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitLineSpace(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUILabelInitIndent(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUILabel, QUIView)
	qmdl_attribute_head(NULL)
		qmdl_attribute("style",			QUILabelInitStyle					)
		qmdl_attribute("format",			QUILabelInitStyle					)
		qmdl_attribute("text",			QUILabelInitText					)
		qmdl_attribute_head("font")
			qmdl_attribute("name",		QUILabelInitFontName				)
			qmdl_attribute("weight",		QUILabelInitFontWeight				)
			qmdl_attribute("width",		QUILabelInitFontWidth				)
			qmdl_attribute("height",		QUILabelInitFontHeight				)
		qmdl_attribute_tail
		qmdl_attribute("MaxRow",			QUILabelInitMaxRow					)
		qmdl_attribute("LineSpace",		QUILabelInitLineSpace				)
		qmdl_attribute("indent",			QUILabelInitIndent					)
	qmdl_attribute_tail
qmdl_class
public:
	QUILabel();
	~QUILabel();
	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);

private:
qmdl_end


// - QUIImage

typedef enum qui_image_style
{
	QUIImageStyleGray				= 0x00000001,
	QUIImageStyleColorize			= 0x00000002,
	QUIImageStyleCache				= 0x00000004,
	QUIImageStyleAutoRotate			= 0x00000008,
} QUIImageStyle;

qstr_enum_extern(QUIImageStyleMap);

QINT QUIImageInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIImageInitFile(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUIImage, QUIView)
	qmdl_attribute_head(NULL)
		qmdl_attribute("style",			QUIImageInitStyle					)
		qmdl_attribute("file",			QUIImageInitFile					)
	qmdl_attribute_tail
qmdl_class
public:
	QUIImage();
	~QUIImage();
	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);

private:
qmdl_end


// - QUIButton

QINT QUIButtonInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitFontName(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitFontWeight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitFontWidth(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitFontHeight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitNormalTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitNormalColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitNormalImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitNormalBackground(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitHighlightColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitHighlightImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitHighlightBackground(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitPushedImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitPushedColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QUIButtonInitPushedBackground(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUIButton, QUIView)
	qmdl_attribute_head(NULL)
		qmdl_attribute("style",			QUIButtonInitStyle						)
		qmdl_attribute("title",			QUIButtonInitNormalTitle				)
		qmdl_attribute("color",			QUIButtonInitNormalColor				)
		qmdl_attribute("image",			QUIButtonInitNormalImage				)
		qmdl_attribute("background",		QUIButtonInitNormalBackground			)
		qmdl_attribute_head("font")
			qmdl_attribute("name",		QUIButtonInitFontName					)
			qmdl_attribute("weight",		QUIButtonInitFontWeight					)
			qmdl_attribute("width",		QUIButtonInitFontWidth					)
			qmdl_attribute("height",		QUIButtonInitFontHeight					)
		qmdl_attribute_tail
		qmdl_attribute_head("normal")
			qmdl_attribute("title",		QUIButtonInitNormalTitle				)
			qmdl_attribute("image",		QUIButtonInitNormalImage				)
			qmdl_attribute("color",		QUIButtonInitNormalColor				)
			qmdl_attribute("background",	QUIButtonInitNormalBackground			)
		qmdl_attribute_tail
		qmdl_attribute_head("Highlight")
			qmdl_attribute("image",		QUIButtonInitHighlightImage				)
			qmdl_attribute("color",		QUIButtonInitHighlightColor				)
			qmdl_attribute("background",	QUIButtonInitHighlightBackground		)
		qmdl_attribute_tail
		qmdl_attribute_head("pushed")
			qmdl_attribute("image",		QUIButtonInitPushedImage				)
			qmdl_attribute("color",		QUIButtonInitPushedColor				)
			qmdl_attribute("background",	QUIButtonInitPushedBackground			)
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

#define ES_LEFT             0x0000L
#define ES_CENTER           0x0001L
#define ES_RIGHT            0x0002L
#define ES_MULTILINE        0x0004L
#define ES_UPPERCASE        0x0008L
#define ES_LOWERCASE        0x0010L
#define ES_PASSWORD         0x0020L
#define ES_AUTOVSCROLL      0x0040L
#define ES_AUTOHSCROLL      0x0080L
#define ES_NOHIDESEL        0x0100L
#define ES_OEMCONVERT       0x0400L
#define ES_READONLY         0x0800L
#define ES_WANTRETURN       0x1000L

typedef enum qui_edit_style
{
	QUIEditStyleLeft				= ES_LEFT,
	QUIEditStyleCenter				= ES_CENTER,
	QUIEditStyleRight				= ES_RIGHT,
	QUIEditStyleMultiLine			= ES_MULTILINE,
	QUIEditStyleUpperCase			= ES_UPPERCASE,
	QUIEditStyleLowerCase			= ES_LOWERCASE,
	QUIEditStylePassword			= ES_PASSWORD,
	QUIEditStyleAutoVScroll			= ES_AUTOVSCROLL,
	QUIEditStyleAutoHScroll			= ES_AUTOHSCROLL,
	QUIEditStyleNoHideSel			= ES_NOHIDESEL,
	QUIEditStyleOEMConvert			= ES_OEMCONVERT,
	QUIEditStyleReadOnly			= ES_READONLY,
	QUIEditStyleWantReturn			= ES_WANTRETURN,
	QUIEditStyleNumber				= ES_NUMBER,
} QUIEditStyle;

qstr_enum_extern(QUIEditStyleMap);

QINT QUIEditInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QUIEdit, QUIView)
	qmdl_attribute_head(NULL)
		qmdl_attribute("style",			QUIEditInitStyle						)
	qmdl_attribute_tail
qmdl_class
public:
	QUIEdit();
	~QUIEdit();
	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);

private:
qmdl_end


qmdl_include_begin(QUILib)
	qmdl_include("view",						QPATH_LIB,		QUIView			)
	qmdl_include("xtml",						QPATH_LIB,		QUIXtml			)

    qmdl_include("label",					QPATH_LIB,		QUILabel		)
	qmdl_include("image",					QPATH_LIB,		QUIImage		)
	qmdl_include("button",					QPATH_LIB,		QUIButton		)
	qmdl_include("edit",						QPATH_LIB,		QUIEdit			)
qmdl_include_end


// - Common

CString quiStr2Path(QSTR str, QINT size);


#endif //QMDL_VER_GF

#endif //__QMDL_GF_H__
