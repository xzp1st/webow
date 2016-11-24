
#ifndef __QMDL_MODULE_H__
#define __QMDL_MODULE_H__


#include "qmdl_config.h"
#include "qmdl_define.h"
#include "qmdl_util.h"


class IQModule;

typedef IQModule *QMDL;

typedef QMDL (*QPMK)(QMDL env, QMDL parent, QSTR path);
typedef QINT (IQModule::*QPFN)(QMDL hdl, QSTR name, QINT code, QPNT params[], QINT count);				// 模块函数类型

typedef QINT (*qmdl_event_scan_cb)(QMDL observed, QMDL observer,
								   QSTR name, QINT code, QPFN event_fn, QPCB event_cb, QPNT params[], QINT count);


// -- IQModule/QModule

#define QFLAG_CLAXX								0x00000001		// 模块类结点
#define QFLAG_MODULE							0x00000002		// 模块对象结点
#define QFLAG_PEEK								0x00000004		// 消息窃取，消息路径中断
#define QFLAG_ROUTED							0x00000008		// 被消息路由的模块

#define QWAIT_PRIORITY_MASK						0x0000FFFF		// 等待事件优先级掩码
#define QWAIT_NOREPLACE							0x00010000		// 等待事件不可覆盖标志
#define QWAIT_SINGLE							0x00020000		// 等待事件单次触发标志


class IQModule
{
public:
	virtual ~IQModule(){};

	virtual QINT GetFlag() = 0;
	virtual void SetFlag(QINT flag) = 0;
	virtual QSTR GetName() = 0;
	virtual void SetName(QSTR name) = 0;
	virtual QSTR GetPath() = 0;
	virtual void SetPath(QSTR path) = 0;
	virtual QPMK GetMakeCb() = 0;
	virtual void SetMakeCb(QPMK new_cb) = 0;
	virtual QINT ReferModule(QINT count) = 0;
	virtual QINT UnreferModule(QINT count) = 0;
	virtual QPNT GetSelfData() = 0;
	virtual void SetSelfData(QPNT selfdata) = 0;
	virtual QPNT GetUserData() = 0;
	virtual void SetUserData(QPNT userdata) = 0;
	
	virtual QPCB GetSelfCb() = 0;
	virtual void SetSelfCb(QPCB cb) = 0;
	virtual QPCB GetUserCb() = 0;
	virtual void SetUserCb(QPCB cb) = 0;
	virtual QINT SelfCb(QSTR name, QINT code, QPNT params[], QINT count) = 0;
	virtual QINT UserCb(QSTR name, QINT code, QPNT params[], QINT count) = 0;
	virtual QINT ModuleCb(QSTR name, QINT code, QPNT params[], QINT count) = 0;
	virtual QINT RouteCb(QMDL parent, QSTR name, QINT code, QPNT params[], QINT count) = 0;
	virtual QINT IsRouted();
	virtual QINT IsPeek();

	virtual QMDL MainModule() = 0;
	virtual QMDL RootModule();
	virtual QMDL ParentModule() = 0;
	virtual QMDL PrevModule() = 0;
	virtual QMDL NextModule() = 0;
	virtual QMDL HeadModule() = 0;
	virtual QMDL TailModule() = 0;
	virtual QMDL RegisterModule(QSTR path, QPMK new_cb) = 0;
	virtual QMDL RegisterModule(QSTR path, QMDL module) = 0;
	virtual void UnregisterModule(QSTR path, QMDL module) = 0;

	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url) = 0;
	virtual QINT MakeModuleInit(QMDL env, QMDL parent, QXML mxml, QSTR url) = 0;
	virtual QINT MakeModuleEnd(QMDL env, QXML mxml, QSTR url) = 0;
	
	virtual void ScanEventx(qmdl_event_scan_cb scan_cb, QPNT params[], QINT count) = 0;
	virtual void ScanEvent(qmdl_event_scan_cb scan_cb, QINT count, ...) = 0;
    virtual void WaitEvent(QMDL module, QSTR name, QINT code, QINT flag, QPFN event_fn, QPCB event_cb) = 0;
    virtual void DispatchEventx(QSTR name, QINT code, QPNT params[], QINT count) = 0;
    virtual void DispatchEvent(QSTR name, QINT code, QINT count, ...) = 0;
	virtual void RouteEventx(QMDL parent, QSTR name, QINT code, QPNT params[], QINT count);
	virtual void RouteEvent(QMDL parent, QSTR name, QINT code, QINT count, ...);
	
	virtual void SetMethod(QSTR name, QINT code, QPFN method_fn, QPCB method_cb) = 0;
	virtual void GetMethod(QSTR name, QINT code, QPFN *method_fn, QPCB *method_cb) = 0;
	virtual QINT CallMethodx(QSTR name, QINT code, QPNT params[], QINT count) = 0;
	virtual QINT CallMethod(QSTR name, QINT code, QINT count, ...) = 0;
	
	virtual QINT SetAttr(QMDL env, QSTR name, QPNT type, QPNT value, QINT len) = 0;
	virtual QINT GetAttr(QMDL env, QSTR name, QPNT *type, QPNT *value, QINT *len) = 0;
	virtual void InitAttrs() = 0;

	virtual QINT DiffCount(QMDL module, QINT index) { return 0; };
	virtual QMDL DiffModule(QMDL module, QINT index) { return NULL; };

	virtual QINT GetPath(QMDL parent, QPNT path, QINT size);
	virtual QMDL FindModule(QSTR path, QINT pathlen, QSTR name, QINT namelen);
	virtual QINT ScanModulex(QMDL module, QPCB prev_cb, QPCB post_cb, QPNT params[], QINT count);
	virtual QINT ScanModule(QMDL module, QPCB prev_cb, QPCB post_cb, QINT count, ...);
};

class QModule: public IQModule
{
public:
	static void InitModule(QMDL env, QMDL parent, QMDL module, QSTR path){};
	
public:
    QModule();
	QModule(QSTR name, QINT size);
	virtual ~QModule();

	virtual QINT GetFlag();
	virtual void SetFlag(QINT flag);
	virtual QSTR GetName();
	virtual void SetName(QSTR name);
	virtual QSTR GetPath();
	virtual void SetPath(QSTR path);
	virtual QPMK GetMakeCb();
	virtual void SetMakeCb(QPMK new_cb);
	virtual QINT ReferModule(QINT count);
	virtual QINT UnreferModule(QINT count);
	virtual QPNT GetUserData();
	virtual void SetUserData(QPNT userdata);
	virtual QPNT GetSelfData();
	virtual void SetSelfData(QPNT selfdata);
	
	virtual QPCB GetSelfCb();
	virtual void SetSelfCb(QPCB cb);
	virtual QPCB GetUserCb();
	virtual void SetUserCb(QPCB cb);
	virtual QINT SelfCb(QSTR name, QINT code, QPNT params[], QINT count);
	virtual QINT UserCb(QSTR name, QINT code, QPNT params[], QINT count);
	virtual QINT ModuleCb(QSTR name, QINT code, QPNT params[], QINT count);
	virtual QINT RouteCb(QMDL parent, QSTR name, QINT code, QPNT params[], QINT count);

	virtual QMDL MainModule();
	virtual QMDL ParentModule();
	virtual QMDL PrevModule();
	virtual QMDL NextModule();
	virtual QMDL HeadModule();
	virtual QMDL TailModule();
	virtual QMDL RegisterModule(QSTR path, QPMK new_cb);
	virtual QMDL RegisterModule(QSTR path, QMDL module);
	virtual void UnregisterModule(QSTR path, QMDL module);

	virtual QINT MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url);
	virtual QINT MakeModuleInit(QMDL env, QMDL parent, QXML mxml, QSTR url);
	virtual QINT MakeModuleEnd(QMDL env, QXML mxml, QSTR url);

	virtual void ScanEventx(qmdl_event_scan_cb scan_cb, QPNT params[], QINT count);
	virtual void ScanEvent(qmdl_event_scan_cb scan_cb, QINT count, ...);
    virtual void WaitEvent(QMDL module, QSTR name, QINT code, QINT flag, QPFN event_fn, QPCB event_cb);
    virtual void DispatchEventx(QSTR name, QINT code, QPNT params[], QINT count);
    virtual void DispatchEvent(QSTR name, QINT code, QINT count, ...);
    
	virtual void SetMethod(QSTR name, QINT code, QPFN method_fn, QPCB method_cb);
	virtual void GetMethod(QSTR name, QINT code, QPFN *method_fn, QPCB *method_cb);
	virtual QINT CallMethodx(QSTR name, QINT code, QPNT params[], QINT count);
	virtual QINT CallMethod(QSTR name, QINT code, QINT count, ...);
	
	virtual QINT SetAttr(QMDL env, QSTR name, QPNT type, QPNT value, QINT len);
	virtual QINT GetAttr(QMDL env, QSTR name, QPNT *type, QPNT *value, QINT *len);
	virtual void InitAttrs();

public:
	QMDL AddParentModule(QMDL child, QMDL parent);
	QMDL AddChildModule(QMDL parent, QMDL child);
	QMDL AddNextModule(QMDL module, QMDL base, QMDL next);
	QMDL AddPrevModule(QMDL module, QMDL base, QMDL prev);
	QMDL RemoveModule(QMDL child);
	
public:
	QMDL main;
	
// -- 内部成员
public:
    QHDL SetObserver(QHDL observer);
    void RemObserver(QHDL observer);
    QHDL SetObserved(QHDL observer);
    void RemObserved(QHDL observer);
    void ClrObsrLink();

	void SetAttrXml(QINT copy, QXML mxml);
	QHDL GetAttrItem(QSTR name, QINT flag);
	QHDL SetAttrItem(QSTR name, QINT flag);
	void ClrAttrLink();
	
	QINT SetInitItem(QPNT name, QPNT type, QPNT value, QINT len);
	QINT GetInitItem(QPNT name, QPNT *type, QPNT *value, QINT *len);
	void ClrInitLink();
	
public:
	QINT mnFlag;													// 模块标志
	QSTR mpName;													// 模块名称
	QSTR mpPath;													// 模块路径
	QINT mnRefer;													// 模块引用计数
	QPMK mpcbMake;													// 模块构造函数
	QPNT mpUserData;												// 用户数据
	QPCB mpcbSelf;													// 模块私有函数
	QPCB mpcbUser;													// 用户回调函数
	QHDL mhAttrLink;												// 模块属性链
	QHDL mhMthdLink;												// 模块方法链
	QHDL mhObsrLink;												// 观察者信息链
	QMDL mpParentModule;											// 父模块
	QMDL mpPrevModule;												// 上一个孩子模块
	QMDL mpNextModule;												// 下一个孩子模块
	QMDL mpHeadModule;												// 第一个孩子模块
	QMDL mpTailModule;												// 最后一个孩子模块
	
public:
	QINT mnInitDepth;												// 初始化继承深度
	QHDL mhInitLink;												// 初始化取值缓存
};


QINT qmdlMain(int argc, char * argv[], QPMK main);					// 应用程序入口，具体平台具体实现

#define qmdlExtern(env, path, cls) ((env)?(env)->RegisterModule((path), &(cls)::MakeModule):NULL)	// 声明注册模块
QMDL qmdlMakei(QMDL env, QMDL parent, QPMK cls, QMDL module, QXML mxml, QSTR url);					// 构建模块，不广播Make消息
QMDL qmdlMake(QMDL env, QMDL parent, QPMK cls, QMDL module, QXML mxml, QSTR url);					// 构建模块，广播Make消息
QINT qmdlInit(QMDL env, QMDL module, QXML mxml, QSTR url);											// 初始化模块，mxml为空，url创建；mxml非空，url为参数输入

// 取值拷贝：$(name) - url参数取值；${name} - 国际化变量取值；$[name] - 调用者变量取值；
QINT qmdlCopy(QMDL module, QMDL caller, QPNT params[], QINT count, QPNT url, QPNT dst, QINT dstsize, QPNT src, QINT *srcsize);


// -- 模块定义宏

#define QMDL_MASK									(0x000F0000)	// QMDL标志掩码

#define QMDL_INCLUDE								(0x00010000)	// 包含结点标志
#define QMDL_OBJECT									(0x00020000)	// 对象结点标志
#define QMDL_CLASS									(0x00030000)	// 类型结点标志
#define QMDL_CSS									(0x00040000)	// 样式结点标志
#define QMDL_ATTR									(0x00050000)	// 无显示类型属性结点标志，默认为字符串类型
#define QMDL_NODE									(0x00060000)	// 普通结点标志
#define QMDL_PARAM									(0x00070000)	// 参数结点标志，暂由QXML_PRM替代
#define QMDL_METHOD									(0x00080000)	// 方法结点标志
#define QMDL_SELFCB									(0x00090000)	// 用户回调标志
#define QMDL_USERCB									(0x000A0000)	// 私有回调标志
#define QMDL_WAITCB									(0x000B0000)	// 监听结点标志
#define QMDL_EVENT									(0x000C0000)	// 事件结点标志
#define QMDL_ATTRIBUTE								(0x000D0000)	// 属性定义标志
#define QMDL_DATA									(0x000E0000)	// 数据结点标志
#define QMDL_VIEW									(0x000F0000)	// 视图结点标志


// -- 模块宏

// 通用项
#define qmdl_item(flag, tag, id, valn, valp, valcb) qxml_item(flag, tag, id, valn, valp, valcb)

// 结点宏
#define qmdl_mxml_begin(name)						qxml_begin(name)
#define qmdl_node_head(tag, ID, path)					qmdl_item( QXML_HEAD|QMDL_NODE,		tag,		ID, 0, path, NULL),
#define qmdl_node_tail									qmdl_item( QXML_TAIL|QMDL_NODE,		NULL,		NULL, 0, NULL, NULL),
#define qmdl_node(tag, ID, path)						qmdl_item( QXML_NODE|QMDL_NODE,		tag,		ID, 0, path, NULL),
#define qmdl_node_cb(tag, ID, cb)						qmdl_item( QXML_NODE|QMDL_NODE,		tag,		ID, 0, NULL, cb),
#define qmdl_mxml_end								qxml_end

// 对象宏
#define qmdl_object_begin(name, claxx, path)		const struct qxml_item_info name[] =\
													{\
														qxml_item( QXML_BEGIN|QXML_HEAD|QMDL_OBJECT,	"object",	#name, 0, NULL, claxx),
#define qmdl_object_end									qxml_item( QXML_END|QXML_TAIL|QMDL_OBJECT,		NULL, NULL, 0, NULL, NULL )\
													};
#define qmdl_object_head(ID, claxx, path)			qmdl_item( QXML_HEAD|QMDL_OBJECT,		"object",	ID, 0, path, claxx),
#define qmdl_object_tail							qmdl_item( QXML_TAIL|QMDL_OBJECT,		"object",	NULL, 0, NULL, NULL),
#define qmdl_object(ID, claxx, path)				qmdl_item( QXML_NODE|QMDL_OBJECT,		"object",	ID, 0, path, claxx),

// 模块声明引用宏
#define qmdl_extern(name)							class name;\
													extern static name::MakeModule
// 模块类型引用宏
#define qmdl_claxx(name)							name::MakeModule
// 模块名称引用宏
#define qmdl_name(name)								#name

// 模块宏
#define qmdl_module(name, base)						class name: public base\
													{\
													public:\
														static QMDL MakeModule(QMDL env, QMDL parent, QSTR path)\
														{\
															QModule *pmodule = (QModule *)(new name());\
															if(pmodule == NULL)\
															{\
																return NULL;\
															}\
															if(pmodule->main == NULL)\
															{\
																pmodule->main = env?env->MainModule():NULL;\
															}\
															pmodule->mnInitDepth = 0;\
															name::InitModule((QMDL)env, parent, (QMDL)pmodule, (QSTR)path);\
															pmodule->mpcbMake = (QPMK)&name::MakeModule;\
															pmodule->mnFlag |= QFLAG_MODULE;\
															return pmodule;\
														}\
														static void InitModule(QMDL env, QMDL parent, QMDL module, QSTR path)\
														{\
															QMDL penv = env;\
															QModule *pmodule = (QModule *)module;\
															QSTR ppath = path;\
															void (*pcbinit)(QMDL env, QMDL parent, QMDL module, QSTR path);\
															pcbinit = &base::InitModule;\
															static const struct qxml_item_info vdefine[] =\
															{\
																qxml_item( QXML_BEGIN|QXML_HEAD|QMDL_OBJECT,	"object", #name, 0, NULL, NULL),
#define qmdl_class												qxml_item( QXML_END|QXML_TAIL|QMDL_OBJECT,		NULL, NULL, 0, NULL, NULL )\
															};\
															if(pmodule->mnInitDepth == 0) pmodule->SetPath(ppath);\
															pmodule->mnInitDepth++;\
															pcbinit(penv, parent, pmodule, ppath);\
															pmodule->mnInitDepth--;\
															qmdlMakei(penv, parent, NULL, pmodule, (QXML)vdefine, ppath);\
														}
#define qmdl_end									};
														
// 样式宏
#define qmdl_css(css, url)							qmdl_item( QXML_ATTR|QMDL_CSS,			url,		"css", 0, css, NULL),

// 属性定义宏
#define qmdl_attribute_headx(ID, cb, type)			qmdl_item( QXML_HEAD|QMDL_ATTRIBUTE,	type,	ID, 0, NULL, cb),
#define qmdl_attribute_head(ID, cb)					qmdl_item( QXML_HEAD|QMDL_ATTRIBUTE,	qstr,	ID, 0, NULL, cb),
#define qmdl_attribute(ID, cb)						qmdl_item( QXML_NODE|QMDL_ATTRIBUTE,	NULL,	ID, 0, NULL, cb),
#define qmdl_attributex(ID, cb, type)				qmdl_item( QXML_NODE|QMDL_ATTRIBUTE,	type,	ID, 0, NULL, cb),
#define qmdl_attributeb(ID, cb)						qmdl_item( QXML_NODE|QMDL_ATTRIBUTE,	qbool,	ID, 0, NULL, cb),
#define qmdl_attributen(ID, cb)						qmdl_item( QXML_NODE|QMDL_ATTRIBUTE,	qint,	ID, 0, NULL, cb),
#define qmdl_attributep(ID, cb)						qmdl_item( QXML_NODE|QMDL_ATTRIBUTE,	qpnt,	ID, 0, NULL, cb),
#define qmdl_attributes(ID, cb)						qmdl_item( QXML_NODE|QMDL_ATTRIBUTE,	qstr,	ID, 0, NULL, cb),
#define qmdl_attributecb(ID, cb)					qmdl_item( QXML_NODE|QMDL_ATTRIBUTE,	qpcb,	ID, 0, NULL, cb),
#define qmdl_attribute_tail							qmdl_item( QXML_TAIL|QMDL_ATTRIBUTE,	NULL,	NULL, 0, NULL, NULL),
														
// 属性赋值宏
#define qmdl_attr(ID, val)							qmdl_item( QXML_ATTR|QMDL_ATTR,			NULL,		ID, 0, val, NULL),
#define qmdl_attrx(ID, type, val)					qmdl_item( QXML_ATTR|QMDL_ATTR,			type,		ID, 0, NULL, val),
#define qmdl_attrb(ID, val)							qmdl_item( QXML_ATTR|QMDL_ATTR,			qint,		ID, 0, val, NULL),
#define qmdl_attrn(ID, val)							qmdl_item( QXML_ATTR|QMDL_ATTR,			qint,		ID, 0, val, NULL),
#define qmdl_attrp(ID, val)							qmdl_item( QXML_ATTR|QMDL_ATTR,			qpnt,		ID, 0, val, NULL),
#define qmdl_attrs(ID, val)							qmdl_item( QXML_ATTR|QMDL_ATTR,			qstr,		ID, 0, val, NULL),
#define qmdl_attrcb(ID, val)						qmdl_item( QXML_ATTR|QMDL_ATTR,			qpcb,		ID, 0, NULL, val),

// 参数宏
#define qmdl_param_begin(name)						qprm_begin(name)
#define qmdl_param_head(name)							qprm_head(name)
#define qmdl_param(name, type, cb, value)					qprm_param(name, type, cb, value)
#define qmdl_paramn(name, cb, value)						qprm_paramn(name, cb, value)
#define qmdl_paramp(name, cb, value)						qprm_paramp(name, cb, value)
#define qmdl_paramcb(name, cb, value)						qprm_paramcb(name, cb, value)
#define qmdl_param_tail									qprm_tail
#define qmdl_param_end								qprm_end
														
// 成员函数宏
#define qmdl_method_cb(name, cb)					qmdl_item( QXML_NODE|QMDL_METHOD,		"method",	name, 0, NULL, cb),
// 私有回调宏
#define qmdl_self_cb(cb)							qmdl_item( QXML_NODE|QMDL_SELFCB,		"selfcb",	NULL, 0, NULL, cb),
// 用户回调宏
#define qmdl_user_cb(cb)							qmdl_item( QXML_NODE|QMDL_USERCB,		"usercb",	NULL, 0, NULL, cb),
// 监听定义宏
#define qmdl_wait_cb(module, name, code, cb)		qmdl_item( QXML_NODE|QMDL_WAITCB,		"waitcb",	module, code, name, cb),
// 事件定义宏
#define qmdl_event(name, cb)						qmdl_item( QXML_NODE|QMDL_EVENT,		"event",	name, 0, NULL, cb),
// 声明定义宏
#define qmdl_include_begin(name)					const struct qmdl_xml_item name[] =\
													{
#define qmdl_include(ID, path, module)					qmdl_item( QXML_NODE|QMDL_INCLUDE,	"include",	ID, 0, path, &module::MakeModule),
#define qmdl_include_head(name)							qmdl_item( QXML_HEAD|QMDL_INCLUDE,	"include",	name, 0, NULL, NULL),
#define qmdl_include_tail								qmdl_item( QXML_TAIL|QMDL_INCLUDE,	NULL,		NULL, 0, NULL, NULL),
#define qmdl_include_end								qmdl_item( QXML_END|QXML_TAIL,		NULL,		NULL, 0, NULL, NULL)\
													};
														
// 数据定义宏
#define qmdl_data(name, cb)							qmdl_item( QXML_NODE|QMDL_DATA,			"data",	name, 0, NULL, cb),
// 视图定义宏
#define qmdl_include_begin(name)					const struct qmdl_xml_item name[] =\
													{
#define qmdl_view_head(name, cb, type)					qmdl_item( QXML_HEAD|QMDL_VIEW,		type,	name, 0, NULL, cb),
#define qmdl_view(name, cb, type)						qmdl_item( QXML_NODE|QMDL_VIEW,		type,	name, 0, NULL, cb),
#define qmdl_view_tail									qmdl_item( QXML_TAIL|QMDL_VIEW,		NULL,	NULL, 0, NULL, NULL),
#define qmdl_view_end									qmdl_item( QXML_END|QXML_TAIL,		NULL,	NULL, 0, NULL, NULL)\
													};

// 公共类型
qmdl_module(QRawPointer, QModule)
qmdl_class
qmdl_end

qmdl_module(QRawCallback, QModule)
qmdl_class
qmdl_end

qmdl_module(QRawNumber, QModule)
qmdl_class
qmdl_end

qmdl_module(QRawString, QModule)
qmdl_class
qmdl_end

qmdl_module(QRawData, QModule)
qmdl_class
qmdl_end

qmdl_module(QRawLink, QModule)
qmdl_class
qmdl_end

qmdl_module(QRawTree, QModule)
qmdl_class
qmdl_end

														
#endif
