
#ifndef __QMDL_BASE_H__
#define __QMDL_BASE_H__


#include "qmdl_module.h"


// - 空模块
qmdl_module(QNULL, QModule)
qmdl_class
public:
	QNULL(){};
qmdl_end

// - 主模块
qmdl_module(QBaseMain, QModule)
qmdl_class
public:
	QBaseMain(){ main = this; };

	virtual QINT PostEvent(QMDL module, QSTR name, QINT code, QPNT params[], QINT count){ return QNO_OK; };
	virtual QINT SendEvent(QMDL module, QSTR name, QINT code, QPNT params[], QINT count){ return QNO_OK; };
	virtual QINT ShowModule(QMDL module){ return QNO_OK; };
	virtual QINT HideModule(QMDL module){ return QNO_OK; };
	virtual QINT PushModule(QMDL module){ return QNO_OK; };
	virtual QINT PopModule(QMDL module){ return QNO_OK; };

virtual QHDL GetDC(QMDL module) { return NULL; };
virtual QINT ReleaseDC(QHDL hdc) { return QNO_OK; }
virtual QINT DrawLine(QHDL hdc, QINT x) { return QNO_OK; }

	virtual QINT LoadString(QSTR file, QSTR name, QSTR value, QINT size){ return 0; };
	virtual QINT GetScreenRect(QRECT *rect){ return QNO_OK; };

qmdl_end


QINT qmdlPost(QMDL env, QSTR name, QINT code, QPNT params[], QINT count);
QINT qmdlSend(QMDL env, QSTR name, QINT code, QPNT params[], QINT count);

QINT qmdlChange(QMDL env, QMDL module);
QINT qmdlPush(QMDL env, QMDL module);
QINT qmdlPop(QMDL env);

QINT qmdlLoadString(QMDL env, QSTR file, QSTR name, QSTR value, QINT size);
QINT qmdlGetScreenRect(QMDL env, QRECT *rect);


// - 视图模块

enum
{
	QStateVisual                 = 0x00000001,		// 可视状态
	
	QLayoutNone                  = 0x00000000,		// 空
	QLayoutAttrMask              = 0x000000FF,		// 属性掩码
	QLayoutWidth                 = 0x00000001,		// 宽度
	QLayoutHeight                = 0x00000002,		// 高度
	QLayoutLeft                  = 0x00000003,		// 左
	QLayoutTop                   = 0x00000004,		// 上
	QLayoutRight                 = 0x00000005,		// 右
	QLayoutBottom                = 0x00000006,		// 下
	QLayoutVCenter               = 0x00000007,		// 垂直中点
	QLayoutHCenter               = 0x00000008,		// 水平中点
	QLayoutCenter                = 0x00000009,		// 居中
	QLayoutAppend                = 0x0000000A,		// 后接
	QLayoutDiv                   = 0x0000000B,		// 分割
	
	QLayoutFlagMask              = 0x0000FF00,		// 标志掩码
	QLayoutFlagMove              = 0x00000008,		// 标志偏移，内部实际存储时进行移位
	QLayoutEqual                 = 0x00000100,		// 相等标志，默认
	QLayoutLess                  = 0x00000200,		// 小于标志
	QLayoutMore                  = 0x00000400,		// 大于标志
	QLayoutMPercent              = 0x00000800,		// 乘积百分比标志
	QLayoutRPercent              = 0x00001000,		// 余数百分比标志
	QLayoutFinished              = 0x00008000,		// 位置子项完成标志
};

#define QLINK_BL_RELATION		(QLINK_DBL)			// 基础排版关联链表标志

struct QBaseLayout
{
	QUCHR vflag[12];								// 排版标志
	QRECT rcposition;								// 版面位置
	QRECT rcmargin;									// 版面间隙
	QHDL hposition;									// 位置规则（自身）
	QHDL hchildren;									// 排版规则（子树）
	QHDL hrelation;									// 依赖关系（因控件先后顺序引起的位置计算先后依赖）
};

QINT QBaseViewInitWidth(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QBaseViewInitHeight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QBaseViewInitLeft(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QBaseViewInitTop(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QBaseViewInitRight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QBaseViewInitBottom(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QBaseViewInitPosition(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);
QINT QBaseViewInitLayout(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);

qmdl_module(QBaseView, QModule)

	qmdl_attribute_head(NULL, NULL)
		qmdl_attribute("width",			QBaseViewInitWidth			)
		qmdl_attribute("height",		QBaseViewInitHeight			)
		qmdl_attribute("left",			QBaseViewInitLeft			)
		qmdl_attribute("top",			QBaseViewInitTop			)
		qmdl_attribute("right",			QBaseViewInitRight			)
		qmdl_attribute("bottom",		QBaseViewInitBottom			)
		qmdl_attribute("position",		QBaseViewInitPosition		)
		qmdl_attribute("layout",		QBaseViewInitLayout			)
	qmdl_attribute_tail

qmdl_class
public:
	QBaseView();
	~QBaseView();

	virtual QINT SetState(QINT state, QINT flag) { return QNO_TRUE; }
	virtual QINT GetState(QINT state) { return QNO_TRUE; }
	virtual QINT GetWidth() { return 0; }
	virtual QINT GetHeight() { return 0; }
	virtual void SetPosition(QRECT *rect) { return; }
	virtual void GetPosition(QRECT *rect) { return; }

	QINT SetNewline(QINT flag);
	QINT GetNewline();

	QINT HasLayoutPosition();
	void SetLayoutPosition(QRECT *rect);
	void SetLayoutMargin(QRECT *rect);
	void GetLayoutPosition(QRECT *rect);
	void GetLayoutMargin(QRECT *rect);
	void MovePosition(QINT x, QINT y);

	void AddPositionRule(QINT srcattr, QMDL dstmdl, QINT dstattr, QFLT multiplier, QINT remainder);
	void AddLayoutRule(QINT srcattr, QMDL dstmdl, QINT dstattr, QFLT multiplier, QINT remainder);
	void LayoutPosition();
	void UpdatePosition();
	void InvalidatePosition();

public:
	struct QBaseLayout mstlayout;
};


// - 数字

qmdl_module(QBasePointer, QModule)
qmdl_class
qmdl_end

qmdl_module(QBaseCallback, QModule)
qmdl_class
qmdl_end

qmdl_module(QBaseNumber, QModule)
qmdl_class
qmdl_end

qmdl_module(QBaseString, QModule)
qmdl_class
qmdl_end

qmdl_module(QBaseData, QModule)
qmdl_class
qmdl_end

qmdl_module(QBaseLink, QModule)
qmdl_class
qmdl_end

qmdl_module(QBaseTree, QModule)
qmdl_class
qmdl_end

qmdl_module(QBaseNetwork, QModule)
qmdl_class
qmdl_end


#endif
