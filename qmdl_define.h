
#ifndef __QMDL_DEFINE_H__
#define __QMDL_DEFINE_H__

#include "qmdl_config.h"

// -- 公用类型
typedef bool			QBOOL;
typedef char			QBYTE;
typedef unsigned short	QWORD;
typedef unsigned long	QDWORD;
typedef char			QCHR;
typedef unsigned char	QUCHR;
typedef long			QINT;
typedef unsigned long	QUINT;
typedef float			QFLT;
typedef unsigned char	*QSTR;
typedef void			*QPNT;
typedef void			*QHDL;

typedef unsigned int	QCLR;

typedef struct QPointTag
{
	QINT x;
	QINT y;
	
	QPointTag()
	: x(0)
	, y(0)
	{
	}
	
	QPointTag(QINT xpos, QINT ypos)
	: x(xpos)
	, y(ypos)
	{
	}
} QPOINT;

typedef struct QRectTag
{
	QINT left;
	QINT top;
	QINT right;
	QINT bottom;
	
	QRectTag()
	: left(0)
	, top(0)
	, right(0)
	, bottom(0)
	{
	}
	
	QRectTag(QINT lpos, QINT tpos, QINT rpos, QINT bpos)
	: left(lpos)
	, top(tpos)
	, right(rpos)
	, bottom(bpos)
	{
	}
} QRECT;

#define QRECTW(rect)	((rect).right-(rect).left)
#define QRECTH(rect)	((rect).bottom-(rect).top)

typedef QINT (*QPCB)(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);				// 通用函数类型


// -- 句柄标识
#define QLCN_MASK							0x000F				// 标识掩码
#define QLCN_NULL							0x0000				// 空标识
#define QLCN_NONE							QLCN_NULL			// 空标识
#define QLCN_GHDL							0x0001				// 泛指句柄，用于scan时返回指向句柄而非最终取值
#define QLCN_HGMM							0x0002				// 内存句柄
#define QLCN_HGBS							0x0003				// 接口句柄
#define QLCN_HXML							0x0004				// 数组句柄
#define QLCN_HGOB							0x0005				// 对象句柄
#define QLCN_HGNV							0x0006				// 名值句柄
#define QLCN_HGTM							0x0007				// 计时句柄
#define QLCN_HPRM							0x0008				// 参数表句柄
#define QLCN_HSTR							0x0009				// 字符串句柄
#define QLCN_HENM							0x000A				// 枚举句柄
#define QLCN_USER							0x000F				// 用户标识


// -- 返回编码
#define QNO_OK								(0)					// 成功
#define QNO_FAIL							(-1)				// 失败
#define QNO_ERROR							(-1)				// 错误

#define QNO_TRUE							(1)					// 真
#define QNO_FALSE							(0)					// 假


// -- 事件编码
#define QCD_NULL							0x0000				// 空编码
#define QCD_NONE							QCD_NULL			// 空编码
#define QCD_CALL							0x0000				// 调用编码
#define QCD_KNOW							0x0001				// 推理编码
#define QCD_SET								0x0002				// 预备编码
#define QCD_GET								0x0003				// 完成编码
#define QCD_GHDL							0x0004				// 句柄构造，创建完成继承对象、子对象后触发
#define QCD_MAKE							0x0005				// 句柄创建，分配空间后按继承树的后序遍历触发
#define QCD_INIT							0x0006				// 句柄初始化，HGRS - 使用object资源结点创建后触发
#define QCD_EXIT							0x0007				// 句柄退出
#define QCD_FREE							0x0008				// 句柄释放

#define QCD_PREPARE							0x0101				// 预备编码
#define QCD_FINISH							0x0102				// 完成编码
#define QCD_PUSH							0x0103				// 压栈编码
#define QCD_POP								0x0104				// 退栈编码
#define QCD_PAINT							0x0105				// 刷新编码

#define QCD_TOUCHDOWN						0x0201				// 按下编码
#define QCD_TOUCHUP							0x0202				// 弹起编码
#define QCD_TOUCHMOVE						0x0203				// 移动编码
#define QCD_CLICKING						0x0211				// 点击进行
#define QCD_CLICKED							0x0212				// 点击完成
#define QCD_CLICK							QCD_CLICKED			// 默认点击
#define QCD_DBLCLK							0x0214				// 双击编码
#define QCD_TAP								0x0221				// 单击或多击编码
#define QCD_PAN								0x0222				// 拖动编码
#define QCD_PINCH							0x0223				// 缩放编码
#define QCD_SWIPE							0x0224				// 擦碰编码
#define QCD_ROTATION						0x0225				// 旋转编码
#define QCD_LONGPRESS						0x0226				// 长按编码

#define QCD_CHANGING						0x0301				// 正在改变
#define QCD_CHANGED							0x0302				// 已经改变
#define QCD_CHANGE							QCD_CHANGED			// 默认改变
#define QCD_SELECTING						0x0303				// 正在选择
#define QCD_SELECTED						0x0304				// 已经选择
#define QCD_SELECT							QCD_SELECTED		// 默认选择
#define QCD_CLEARING						0x0305				// 正在清除
#define QCD_CLEARED							0x0306				// 已经清除
#define QCD_CLEAR							QCD_CLEARED			// 默认清除
#define QCD_DOING							0x0307				// 动作进行
#define QCD_DONE							0x0308				// 动作完成
#define QCD_DO								QCD_DONE			// 默认动作

#define QCD_FAIL							0x0FFF				// 失败编码
#define QCD_USER							0x1000				// 用户编码(>=0x1000)


// -- 遍历编码
#define QSCN_FLAG							(0x8000)			// 遍历标志
#define QSCN_END							(QSCN_FLAG|0x0000)	// 遍历结束，不做路径回退，立即退出
#define QSCN_OK								(QSCN_FLAG|0x0001)	// 遍历正常
#define QSCN_VERT							(QSCN_FLAG|0x0002)	// 纵向遍历，绕过当前结点为根的子树
#define QSCN_HORZ							(QSCN_FLAG|0x0003)	// 横向遍历，只遍历当前结点为根的子树
#define QSCN_FAIL							(QSCN_FLAG|0x0004)	// 非真遍历，从当前路径退回
#define QSCN_ERR							(QSCN_FLAG|0x0008)	// 遍历出错


// -- 定位操作
#define QSEEK_SET							0x0000				// 设置偏移
#define QSEEK_CUR							0x0001				// 读取偏移
#define QSEEK_END							0x0002				// 设置末尾，返回偏移


// -- 消息定义
typedef enum qsystem
{
	QSystemMask               = 0xFF000000,
	QSystemModule             = 0x01000000,
	QSystemWin32              = 0x02000000,
	QSystemAndroid            = 0x03000000,
	QSystemIOS                = 0x04000000,
	QSystemGF                 = 0x05000000,
} QSystem;

typedef enum qflag
{
	QFlagMask                 = 0x0000FFFF,
} QFlag;

typedef enum qevent
{
	QEventNone                = 0,

	QEventPrepare             = (QSystemModule|0x00000001),
	QEventFinish              = (QSystemModule|0x00000002),

	QEventMake                = (QSystemModule|0x00001001),
	QEventPush                = (QSystemModule|0x00001002),
	QEventPaint               = (QSystemModule|0x00001003),
	QEventRenew               = (QSystemModule|0x00001004),
	QEventSnap                = (QSystemModule|0x00001005),
	QEventPop                 = (QSystemModule|0x00001006),
	QEventFree                = (QSystemModule|0x00001007),

	QEventLaunch              = (QSystemIOS|0x00002001),
	QEventInactive            = (QSystemIOS|0x00002002),
	QEventBackground          = (QSystemIOS|0x00002003),
	QEventForeground          = (QSystemIOS|0x00002004),
	QEventActive              = (QSystemIOS|0x00002005),
	QEventTerminate           = (QSystemIOS|0x00002006),
	QEventMemory              = (QSystemIOS|0x00002007),
	
	QEventTouchDown           = (QSystemIOS|0x00003000),      // on all touch downs
	QEventTouchDownRepeat     = (QSystemIOS|0x00003001),      // on multiple touchdowns (tap count > 1)
	QEventTouchDragInside     = (QSystemIOS|0x00003002),
	QEventTouchDragOutside    = (QSystemIOS|0x00003003),
	QEventTouchDragEnter      = (QSystemIOS|0x00003004),
	QEventTouchDragExit       = (QSystemIOS|0x00003005),
	QEventTouchUpInside       = (QSystemIOS|0x00003006),
	QEventTouchUpOutside      = (QSystemIOS|0x00003007),
	QEventTouchCancel         = (QSystemIOS|0x00003008),
	
	QEventValueChanged        = (QSystemIOS|0x0000400C),     // sliders, etc.
	
	QEventEditingDidBegin     = (QSystemIOS|0x00005010),     // UITextField
	QEventEditingChanged      = (QSystemIOS|0x00005011),
	QEventEditingDidEnd       = (QSystemIOS|0x00005012),
	QEventEditingDidEndOnExit = (QSystemIOS|0x00005013),     // 'return key' ending editing
	
	QEventTouch               = (QSystemIOS|0x0000600A),
	QEventTap                 = (QSystemIOS|0x0000600B),
	QEventPinch               = (QSystemIOS|0x0000600D),
	QEventRotation            = (QSystemIOS|0x0000600E),
	QEventSwipe               = (QSystemIOS|0x0000600F),
	QEventPan                 = (QSystemIOS|0x00006014),
	QEventLongPress           = (QSystemIOS|0x00006015),
	QEventMotion              = (QSystemIOS|0x00006016),
	QEventRemoteControl       = (QSystemIOS|0x00006017),
	
	QEventTouchMask           = 0x000001FF,  // for touch events
	QEventEditMask            = 0x000F0000,  // for UITextField
	QEventAppMask             = 0x0F000000,  // range available for application use
	QEventSysMask             = 0xF0000000,  // range reserved for internal framework use
	QEventAllMask             = 0xFFFFFFFF,
	
	QEventAllTouchEvents      = 0x000007FF,  // for touch events
	QEventAllEditingEvents    = 0x000F0000,  // for UITextField
	QEventApplicationReserved = 0x0F000000,  // range available for application use
	QEventSystemReserved      = 0xF0000000,  // range reserved for internal framework use
	QEventAllEvents           = 0xFFFFFFFF
} QEvent;


#endif
