
#ifndef __QMDL_UTIL_H__
#define __QMDL_UTIL_H__

#include "qmdl_define.h"


// -- 基本类型

// 基本结构体
typedef struct qmdl_header
{
	QHDL hcheck;												// 句柄校验，取值为对象指针值
	QINT nflag;													// 标志，最低4bits由QLCN_MASK所表示
} QHDR;

extern const QPNT qbool[];										// 布尔类型
extern const QPNT qint[];										// 整形类型
extern const QPNT qpnt[];										// 指针类型
extern const QPNT qhdl[];										// 句柄类型
extern const QPNT qstr[];										// 字符串类型
extern const QPNT qpcb[];										// 回调函数类型

extern const QPNT qnull[];										// 空句柄，某些场合下作为结束符
extern const QPNT qchkhdl[];									// 公共句柄


QINT qncb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count);// 空回调，作占位用

#define qmmake(data, flag)\
	if((data) != NULL)\
	{\
		((struct qmdl_header *)(data))->hcheck = (QHDL )(data);\
		((struct qmdl_header *)(data))->nflag = (QINT )(flag);\
	}

// 判断是否MDL句柄，返回原值。
#define qmcheck(data, flag)\
	(	((data)!=NULL)\
		?\
		(	(	((struct qmdl_header *)(data))->hcheck==(QHDL)qchkhdl ||\
				((struct qmdl_header *)(data))->hcheck==(QHDL)(data)\
			)\
			?\
			(	(	(QINT)((flag)&QLCN_MASK)==QLCN_NULL ||\
					(QINT)((flag)&QLCN_MASK)==QLCN_GHDL\
				)\
				?\
				(QHDL)(data)\
				:\
				(	(	(QINT)((flag)&QLCN_MASK)==(((struct qmdl_header *)(data))->nflag&QLCN_MASK)\
					)\
					?\
					(QHDL)(data)\
					:\
					NULL\
				)\
			)\
			:\
			(	(	(((struct qmdl_header *)(data))-1)->hcheck==(QHDL)qchkhdl ||\
					(((struct qmdl_header *)(data))-1)->hcheck==(QHDL)(data)\
				)\
				?\
				(	(	(QINT)((flag)&QLCN_MASK)==QLCN_NULL ||\
						(QINT)((flag)&QLCN_MASK)==QLCN_GHDL\
					)\
					?\
					(QHDL)(data)\
					:\
					(	(	(QINT)((flag)&QLCN_MASK)==((((struct qmdl_header *)(data))-1)->nflag&QLCN_MASK)\
						)\
						?\
						(QHDL)(data)\
						:\
						NULL\
					)\
				)\
				:\
				NULL\
			)\
		)\
		:\
		NULL\
	)

// 获取MDL句柄头。
#define qmheader(data, flag)\
	(	((data)!=NULL)\
		?\
		(	(	((struct qmdl_header *)(data))->hcheck==(QHDL)qchkhdl ||\
				((struct qmdl_header *)(data))->hcheck==(QHDL)(data)\
			)\
			?\
			(	(	(QINT)((flag)&QLCN_MASK)==QLCN_NULL ||\
					(QINT)((flag)&QLCN_MASK)==QLCN_GHDL\
				)\
				?\
				(QHDL)(data)\
				:\
				(	(	(QINT)((flag)&QLCN_MASK)==(((struct qmdl_header *)(data))->nflag&QLCN_MASK)\
					)\
					?\
					(QHDL)(data)\
					:\
					NULL\
				)\
			)\
			:\
			(	(	(((struct qmdl_header *)(data))-1)->hcheck==(QHDL)qchkhdl ||\
					(((struct qmdl_header *)(data))-1)->hcheck==(QHDL)(data)\
				)\
				?\
				(	(	(QINT)((flag)&QLCN_MASK)==QLCN_NULL ||\
						(QINT)((flag)&QLCN_MASK)==QLCN_GHDL\
					)\
					?\
					(QHDL)(((struct qmdl_header *)(data))-1)\
					:\
					(	(	(QINT)((flag)&QLCN_MASK)==((((struct qmdl_header *)(data))-1)->nflag&QLCN_MASK)\
						)\
						?\
						(QHDL)(((struct qmdl_header *)(data))-1)\
						:\
						NULL\
					)\
				)\
				:\
				NULL\
			)\
		)\
		:\
		NULL\
	)

// 获取所在内存句柄
#define qmhost(data)\
	(	(	(data)!=NULL\
		)\
		?\
		(	(	((struct qmdl_header *)(data))->hcheck==(QHDL)qchkhdl ||\
				((struct qmdl_header *)(data))->hcheck==(QHDL)(data)\
			)\
			?\
			(QHDL)(*((QHDL*)(data)-1))\
			:\
			(	(	(((struct qmdl_header *)(data))-1)->hcheck==(QHDL)qchkhdl ||\
					(((struct qmdl_header *)(data))-1)->hcheck==(QHDL)(data)\
				)\
				?\
				(QHDL)(*((QHDL*)((QSTR)(data)-sizeof(struct qmdl_header)-sizeof(QHDL))))\
				:\
				(QHDL)(*((QHDL*)(data)-1))\
			)\
		)\
		:\
		NULL\
	)

// -- 内存管理

QPNT qmalloc(QHDL mem, QINT size);
void qmfree(QPNT data);


// -- 简单链表

#define QLINK_NONE							0x0000				// 空属性
#define QLINK_MEM							0x0001				// 带内存句柄
#define QLINK_HDR							0x0002				// 带头信息
#define QLINK_RNG							0x0004				// 环形链表
#define QLINK_DBL							0x0008				// 双向链表

struct qmdl_link_item;

struct qmdl_link_item
{
	struct qmdl_link_item *pnext;								// 链表下一结点
	struct qmdl_link_item *pprev;								// 链表上一结点
	QHDL mem;													// 链表所在内存池
};

struct qmdl_link_data
{
	struct qmdl_link_item *phead;								// 链表头结点
	struct qmdl_link_item *ptail;								// 链表尾结点
};

// 读取链表头结点
#define qlinkHead(flag, link)\
	( ((link)==NULL) ? NULL :\
	( ((flag)&QLINK_HDR) ? ((QHDL )((struct qmdl_link_data *)(link))->phead) : (link)\
	)\
	)
// 读取链表下一结点
#define qlinkNext(flag, item)\
	( ((item)==NULL) ? NULL :\
	( (QHDL )(((struct qmdl_link_item *)(item))->pnext)\
	)\
	)
// 读取链表上一结点
#define qlinkPrev(flag, item)\
	( ((item)==NULL) ? NULL :\
	( ((flag)&QLINK_DBL) ? ((QHDL )(((struct qmdl_link_item *)(item))->pprev)) : NULL\
	)\
	)
// 读取链表结点数据
#define qlinkData(flag, item)\
	( ((item)==NULL) ? NULL : (QHDL )\
	(	(QCHR *)item +\
	sizeof(struct qmdl_link_item *) +\
	( ((flag)&QLINK_DBL) ? sizeof(struct qmdl_link_item *) : 0 ) +\
	( ((flag)&QLINK_MEM) ? sizeof(QHDL) : 0 )\
	)\
	)

QHDL qlinkTail(QINT flag, QHDL link);							// 读取链表尾结点
QPNT qlinkItem(QINT flag, QHDL data);							// 读取数据所在的链表结点
QHDL qlinkLink(QINT flag, QHDL item);							// 读取结点所在的链表指针

QHDL qlinkMake(QINT flag, QHDL mem, QHDL data, QINT size);		// 创建链表
QHDL qlinkInsertNext(QINT flag, QHDL link, QHDL base, QHDL item);// 往后插入结点
QHDL qlinkInsertPrev(QINT flag, QHDL link, QHDL base, QHDL item);// 往前插入结点
QHDL qlinkRemove(QINT flag, QHDL link, QHDL item);				// 删除结点，不做释放
QHDL qlinkFreeLinkx(QINT flag, QHDL link, QPCB free_cb, QPNT params[], QINT count);		// 释放链表
QHDL qlinkFreeLink(QINT flag, QHDL link, QPCB free_cb, QINT count, ...);
QINT qlinkFreeItemx(QINT flag, QHDL item, QPCB free_cb, QPNT params[], QINT count);		// 释放结点
QHDL qlinkFreeItem(QINT flag, QHDL item, QPCB free_cb, QINT count, ...);
QHDL qlinkClearLinkx(QINT flag, QHDL link, QPCB free_cb, QPNT params[], QINT count);	// 清除链表
QHDL qlinkClearLink(QINT flag, QHDL link, QPCB free_cb, QINT count, ...);
QINT qlinkClearItemx(QINT flag, QHDL item, QPCB free_cb, QPNT params[], QINT count);	// 清除结点
QINT qlinkClearItem(QINT flag, QHDL item, QPCB free_cb, QINT count, ...);

QINT qlinkScanx(QINT flag, QHDL link, QPCB scan_cb, QPNT params[], QINT count);
QINT qlinkScan(QINT flag, QHDL link, QPCB scan_cb, QINT count, ...);


// -- QXML

// flag
#define QXML_TREE_MASK								0x0FF0								// XML树结构BIT掩码
#define QXML_TYPE_MASK								0xF000								// XML结点类型BIT掩码
#define QXML_MASK									(QXML_TREE_MASK|QXML_TYPE_MASK)		// XML相关掩码

#define QXML_ALLOC									0x0010		// 内存分配标识，表示该结点需要自行释放
#define QXML_BEGIN									0x0020		// XML定义开始
#define QXML_HEAD									0x0040		// XML结点起始
#define QXML_ATTR									0x0080		// XML属性结点
#define QXML_TAIL									0x0100		// XML结点结束
#define QXML_END									0x0200		// XML定义结束
#define QXML_NODE									(QXML_HEAD|QXML_TAIL)				// 资源结点

#define QXML_PARAM									0x1000		// 参数类型
#define QXML_VALUE									0x2000		// 取值类型


struct qxml_base;
typedef struct qxml_base QMXML, *QXML;

typedef QXML (*QXCB)(QXML mxml);

struct qxml_base
{
	QHDL hcheck;												// 校验标志
	QINT nflag;													// 结点标志
	QXCB pcbprev;												// 上一结点函数
	QXCB pcbnext;												// 下一结点函数
	QINT nsize;													// 结点长度
	QSTR pid;													// 结点ID
};

struct qxml_item_info
{
	QHDL hcheck;												// 校验标志
	QINT nflag;													// 结点标志
	QXCB pcbprev;												// 上一结点函数
	QXCB pcbnext;												// 下一结点函数
	QINT nsize;													// 结点长度
	QSTR pid;													// 结点ID
	
	QPNT ptag;													// 结点标签
	QINT nval;													// 结点取值（整形）
	QPNT pval;													// 结点取值（指针）
	QPCB pcbval;												// 结点取值（处理函数）
};

// 公共结点定义
#define qxml_item(flag, tag, ID, valn, valp, valcb)\
														{	(QHDL)qchkhdl, (QINT)(flag)|QLCN_HXML,\
															NULL, NULL, (QINT)sizeof(struct qxml_item_info),\
															(QSTR)(ID),\
															(QPNT)(tag), (QINT)(valn), (QPNT)(valp), (QPCB)(valcb)\
														}
// XML树结构定义
#define qxml_begin(name)								const struct qxml_item_info name[] =\
														{\
															qxml_item( QXML_BEGIN|QXML_HEAD,	#name, #name, 0, NULL, NULL ),
#define qxml_node(tag, id, value)							qxml_item( QXML_NODE,				tag, id, 0, value, NULL ),
#define qxml_node_size(tag, id, value, size)				qxml_item( QXML_NODE,				tag, id, size, value, NULL ),
#define qxml_node_cb(tag, id, cb)							qxml_item( QXML_NODE,				tag, id, 0, NULL, cb ),
#define qxml_head(tag, id)									qxml_item( QXML_HEAD,				tag, id, 0, NULL, NULL ),
#define qxml_attr(name, value)								qxml_item( QXML_ATTR,				NULL, name, 0, value, NULL ),
#define qxml_attrn(name, value)								qxml_item( QXML_ATTR,				qint, name, 0, value, NULL ),
#define qxml_attrp(name, value)								qxml_item( QXML_ATTR,				qpnt, name, 0, value, NULL ),
#define qxml_attrcb(name, cb)								qxml_item( QXML_ATTR,				qpcb, name, 0, NULL, cb ),
#define qxml_attr_size(name, value, size)					qxml_item( QXML_ATTR,				NULL, name, size, value, NULL ),
#define qxml_tail											qxml_item( QXML_TAIL,				NULL, NULL, 0, NULL, NULL ),
#define qxml_end											qxml_item( QXML_END|QXML_TAIL,		NULL, NULL, 0, NULL, NULL )\
														};
// 通用结点定义
#define qxml_param(name, format, value)					qxml_item( QXML_NODE|QXML_PARAM,		format, name, 0, value, NULL ),
#define qxml_param_size(name, format, value, size)		qxml_item( QXML_NODE|QXML_PARAM,		format, name, size, value, NULL ),
#define qxml_param_cb(name, format, cb)					qxml_item( QXML_NODE|QXML_PARAM,		format, name, 0, NULL, cb ),

// 资源声明
#define qxml_extern(name)								extern const struct qxml_item_info name[]


QHDL qxmlMake(QHDL mem);
QINT qxmlFree(QHDL mxml);
QHDL qxmlDupx(QHDL mem, QHDL mxml, QPCB dup_cb, QPNT params[], QINT count);
QHDL qxmlDup(QHDL mem, QHDL mxml, QPCB dup_cb, QINT count, ...);

QHDL qxmlGetRoot(QHDL mxml);									// 读取根资源结点
QHDL qxmlGetParent(QHDL mxml);									// 读取父资源结点
QHDL qxmlGetPrev(QHDL mxml);									// 读取上一资源结点
QHDL qxmlGetNext(QHDL mxml);									// 读取下一资源结点
QHDL qxmlGetHead(QHDL mxml);									// 读取第一个子资源结点
QHDL qxmlGetTail(QHDL mxml);									// 读取最后一个子资源结点

QHDL qxmlGetAttr(QHDL mxml, QINT flag, QPNT name, QINT index);	// 读取资源结点的属性值，flag - 非树标志限制；name - 表示目标属性名称；index - 表示第index个属性
QINT qxmlCntAttr(QHDL mxml, QINT flag);							// 读取资源结点的属性个数
QHDL qxmlAttrGetHead(QHDL mxml, QINT flag);						// 读取头属性
QHDL qxmlAttrGetTail(QHDL mxml, QINT flag);						// 读取尾属性
QHDL qxmlAttrGetNext(QHDL mxml, QINT flag);						// 读取下一属性
QHDL qxmlAttrGetPrev(QHDL mxml, QINT flag);						// 读取上一属性

QINT qxmlCntChild(QHDL mxml, QINT flag);						// 读取资源结点的子节点个数
QHDL qxmlGetChild(QHDL mxml, QINT flag, QINT index);			// 读取资源结点的子节点，index - 表示第index个子结点
QHDL qxmlGetSibling(QHDL mxml, QINT flag, QINT index);			// 读取资源结点的兄弟结点，index - 表示第index个兄弟结点
QINT qxmlScanx(QHDL mxml, QPCB prev_cb, QPCB post_cb, QPNT params[], QINT count);	// 资源结点遍历（参数数组）
QINT qxmlScan(QHDL mxml, QPCB prev_cb, QPCB post_cb, QINT count, ...);				// 资源结点遍历（不定参数）
QINT qxmlGetPath(QHDL src, QHDL dst, QPNT path, QINT size);		// 读取资源所在路径
QHDL qxmlFind(QHDL mxml, QPNT path);							// 查找资源结点，path - 查找路径，格式：id1[.id2.id3....]

QINT qxmlGetFlag(QHDL mxml);									// 读取资源标志
QPNT qxmlGetTag(QHDL mxml);										// 读取资源标签指针
QPNT qxmlGetId(QHDL mxml);										// 读取资源ID指针
QINT qxmlGetSize(QHDL mxml);									// 读取资源大小
QPNT qxmlGetValp(QHDL mxml);									// 读取资源指针取值
QINT qxmlGetValn(QHDL mxml);									// 读取资源整形取值
QPCB qxmlGetValcb(QHDL mxml);									// 读取资源取值指针（处理函数）


// -- 参数表

#define QPRM_MASK_COUNT							0xFFFF0000		// 个数掩码
#define QPRM_MASK_FLAG							0x0000FFF0		// 标志掩码
#define QPRM_ALLOC								0x00000010		// 内存分配标志，不可修改
#define QPRM_DEFINE								0x00000020		// 参数定义标志
#define QPRM_FREECB								0x00000040		// 释放回调标志

#define qprm_begin(name)						const struct qxml_item_info name[] =\
												{\
													qxml_item( QXML_BEGIN|QXML_HEAD|QXML_PARAM,	#name, #name, 0, NULL, NULL ),
#define qprm_head(name)								qxml_item( QXML_HEAD|QXML_PARAM, "param", name, 0, NULL, NULL ),
#define qprm_param(name, type, cb, val)					qxml_item( QXML_NODE|QXML_PARAM, type, name, 0, val, cb )
#define qprm_paramn(name, cb, val)						qxml_item( QXML_NODE|QXML_PARAM, qint, name, 0, val, cb )
#define qprm_paramp(name, cb, val)						qxml_item( QXML_NODE|QXML_PARAM, qpnt, name, 0, val, cb )
#define qprm_paramcb(name, cb, val)						qxml_item( QXML_NODE|QXML_PARAM, qpcb, name, 0, val, cb )
#define qprm_tail									qxml_item( QXML_TAIL|QXML_PARAM, NULL, NULL, 0, NULL, NULL ),
#define qprm_end								qxml_end

// 读取参数表个数
#define qprmcnt(list)	( qmcheck(list, QLCN_HPRM) ? (((((struct qmdl_header *)(list))-1)->nflag)>>16) : (-1) )

// 获取定义
#define qprmdfn(list)\
	(	qmcheck(list, QLCN_HPRM)\
		?\
		(	(	(((struct qmdl_header *)(list))-1)->nflag & QPRM_DEFINE\
			)\
			?\
			*((QHDL*)((QSTR)(list)-sizeof(struct qmdl_header)-sizeof(QHDL)))\
			:\
			NULL\
		)\
		:\
		NULL\
	)

/*****************************************************************************************
 *
 * -- 函    数： qprmmakev(); qprmmakex(); qprmmake(); qprmfree();
 * -- 功    能： 通过不定参数marker生成参数表；通过参数数组params生成参数表；释放参数表；
 * -- 参    数：
 *    hdl        -   内存指针，用于传入内存池句柄
 *    list       -   参数表句柄
 *    buff       -   缓冲区指针
 *    size       -   缓冲区大小
 *    define     -   参数表定义，可用qnull占位
 *    free_cb    -   参数释放函数，可用qnull或qncb占位
 *    marker     -   不定参数指针
 *    params     -   参数数组指针
 *    count      -   参数个数
 *
 * -- 注    意： define和free_cb如果为NULL参数表不为其预留空间，如需预留可用qnull进行占位。
 *
 *****************************************************************************************/
QHDL qprmmakev(QHDL hdl, QPNT buff, QINT size, QHDL define, QPCB free_cb, va_list marker, QINT *count);
QHDL qprmmakex(QHDL hdl, QPNT buff, QINT size, QHDL define, QPCB free_cb, QPNT params[], QINT *count);
QHDL qprmmake(QHDL hdl, QPNT buff, QINT size, QHDL define, QPCB free_cb, QINT *count, ...);
QINT qprmfree(QHDL list);

QPNT *qprmlist(QHDL list, QINT *cnt);							// 读取参数表指针
QINT qprmflag(QHDL list);										// 读取参数表标志
QINT qprmSetFlag(QHDL list, QINT flag);							// 设置参数表标志
QINT qprmGetFlag(QHDL list);									// 获取参数表标志
QHDL qprmSetDefine(QHDL list, QHDL define);						// 设置参数表定义
QHDL qprmGetDefine(QHDL list);									// 读取参数表定义
QPCB qprmSetFreeCb(QHDL list, QPCB free_cb);					// 设置参数表释放函数
QPCB qprmGetFreeCb(QHDL list);									// 读取参数表释放函数

QPNT qprmconvert(QHDL dsttype, QPNT buff, QINT *size, QHDL srctype, QPNT srcval, QINT srcsize);
													
QHDL qprmSetValue(QHDL list, QPNT name, QINT *index, QHDL *define, QPNT value);		// 设置参数表项取值，失败 - 返回空，成功 - 返回表项所在参数表、序号、定义
QHDL qprmGetValue(QHDL list, QPNT name, QINT *index, QHDL *define, QPNT *value);	// 获取参数表项取值，失败 - 返回空，成功 - 返回表项所在参数表、序号、定义、取值
													
	
// -- 字符串

#define QSTR_CMP_NONE							0x00000000		// 空标志
#define QSTR_CMP_ICASE							0x00000001		// 不区分大小写
#define QSTR_CMP_PATTERN						0x00000002		// 模式匹配

// 字符串参数遍历函数
typedef QINT (*qstr_prm_cb)(QPNT str, QINT chr, QINT index, QPNT param, QINT size, QPNT params[], QINT count);

QINT qstrlen(QPNT str);											// '\0'结尾的字符串长度
QINT qstrcpy(QPNT str1, QINT size1, QPNT str2, QINT size2);
QPNT qstrdup(QPNT mem, QPNT str, ...);							// 字符串复制生成
QINT qstrint(QINT system, QPNT str, QINT *len);					// 各种进制的字符串转整数
QINT qstrcmp(QINT flag, QPNT str1, QPNT str2, QINT size);		// 字符串比较，flag：ICASE - 不区分大小写；PATTERN - 模式匹配
QPNT qstrchr(QINT flag, QPNT str, QINT size, QINT chr);			// 字符串字符左起查找
QPNT qstrrchr(QINT flag, QPNT str, QINT size, QINT chr);		// 字符串字符右起查找
QPNT qstrstr(QINT flag, QPNT str, QINT size, QPNT mode);		// 字符串字符串查找
QINT qstrstrip(QPNT dst, QINT dstsize, QPNT src, QINT srcsize);	// 字符串萃取，截去头尾的' '、'\t'、'\r'、'\n'
QINT qstrword(QPNT str, QUCHR chr, QINT index, QPNT word, QINT size);					// 读取字符串词语
QINT qstrprm(QPNT str, QUCHR chr, QINT index, QPNT param, QINT size);					// 读取字符串参数
QINT qstrprmcbx(QPNT str, QUCHR chr, qstr_prm_cb prm_cb, QPNT params[], QINT count);	// 遍历字符串参数，输入为数组
QINT qstrprmcb(QPNT str, QUCHR chr, qstr_prm_cb prm_cb, QINT count, ...);				// 遍历字符串参数，输入为不定参数
QPNT qstrnext(QPNT str, QINT *size);													// 读取下一字符

#define QSTR_MATCH_NONE							0x00000000		// 空内容
#define QSTR_MATCH_ICS							0x00000001		// 大小写不区分
													
#define QSTR_MATCH_DEC							0x00000002		// 十进制数字
#define QSTR_MATCH_HEX							0x00000004		// 十六进制数字
#define QSTR_MATCH_AWD							0x00000010		// ASCII词语，[a~z][A~Z][0~9][_]
#define QSTR_MATCH_UWD							0x00000020		// UNICODE词语
#define QSTR_MATCH_SYM							0x00000040		// 符号

#define QSTR_MATCH_SSTR							0x00000100		// 单引号字符串，'...'
#define QSTR_MATCH_DSTR							0x00000200		// 双引号字符串，"..."
#define QSTR_MATCH_BLK							0x00000400		// 串块，(name)、[name]、{name}、<name>
#define QSTR_MATCH_VAL							0x00000800		// 取值，$name、$(name)、$[name]、${name}、$<name>
#define QSTR_MATCH_OBJ							0x00001000		// 对象，@name、@(name)、@[name]、@{name}、@<name>
#define QSTR_MATCH_URL							0x00002000		// 资源，/a/b/.../c.d....?e=f&g=h&...

#define QSTR_MATCH_NUM							(QSTR_MATCH_DEC|QSTR_MATCH_HEX)			// 数字
#define QSTR_MATCH_WORD							(QSTR_MATCH_AWD|QSTR_MATCH_UWD)			// 词语
#define QSTR_MATCH_ASC							(QSTR_MATCH_NUM|QSTR_MATCH_SYM|QSTR_MATCH_AWD)// ASC类
#define QSTR_MATCH_UNI							(QSTR_MATCH_ASC|QSTR_MATCH_UWD)			// UNICODE类
#define QSTR_MATCH_STR							(QSTR_MATCH_SSTR|QSTR_MATCH_DSTR)		// 字符串
#define QSTR_MATCH_VAR							(QSTR_MATCH_VAL|QSTR_MATCH_OBJ)			// 变量
#define QSTR_MATCH_ALL							(QSTR_MATCH_UNI|QSTR_MATCH_STR|QSTR_MATCH_VAR|QSTR_MATCH_URL)

QINT qstrmatch(QSTR str, QINT size, QINT *flag, QSTR *read, QINT *len);					// 字符串匹配，[字母_][数字][符号]，以' '、'\t'、'\r'、'\n'为分隔


/***********************************************************************************************
 *
 * -- 字符串格式化
 *
 * - qstr_item:  <%>+[nfillflag]+[nfillchar]+[nfillsize]+[.]+[nparam]+<nflag>
 * - nfillflag: 取值为：'+'|'-'，字符串类型'+'表示往后填充，'-'表示往前填充；数字类型则反之；
 * - nfillchar: 取值为：'0'|'\t'|'\r'|'\n'|' '|"\\XX"
 * - nfillsize: 填充宽度，超过时不截断；
 * - nparam:    字符串类型时表示最大的串长度，超过则截断；数字类型时表示进制；
 * - nflag:     'c'|'C': 字符
 *              'd'|'D': 十进制
 *              'x'|'X': 十六进制
 *              'i'|'I': 数字，默认为十进制
 *              'p'|'P': 指针地址字符串化
 *              'v'|'V': 值拷贝，'v'表示小端拷贝，'V'表示大端拷贝
 *              's'|'S': 字符串拷贝
 *              'm'|'M': 内存区拷贝
 *              '@'|'@': 对象字串化
 *
 ************************************************************************************************/

#define QSTR_ITEM_BEGIN								(1)			// 格式化开始
#define QSTR_ITEM_BUFF								(2)			// 获取缓冲区，make时提供编码缓冲，scan时提供解码缓冲，通过buff和size传出
#define QSTR_ITEM_MAKE								(3)			// 编码字符串，默认编码时用qstrmakei；自定义编码时用qstrwrite，需要主动更新pparams、ncount；
#define QSTR_ITEM_SCAN								(4)			// 解码字符串，默认解码时用qstrscani；自定义解码时用qstrread，需要主动更新pparams、ncount；
#define QSTR_ITEM_END								(5)			// 格式化结束

#define QSTR_MAKE_BEGIN								QSTR_ITEM_BEGIN
#define QSTR_MAKE_BUFF								QSTR_ITEM_BUFF
#define QSTR_MAKE									QSTR_ITEM_MAKE
#define QSTR_MAKE_END								QSTR_ITEM_END

#define QSTR_SCAN_BEGIN								QSTR_ITEM_BEGIN
#define QSTR_SCAN_BUFF								QSTR_ITEM_BUFF
#define QSTR_SCAN									QSTR_ITEM_SCAN
#define QSTR_SCAN_END								QSTR_ITEM_END


struct qstr_item;
typedef struct qstr_item qstr_item_t;
struct qstr_info;
typedef struct qstr_info qstr_info_t;

// 字符串格式化函数
typedef QINT (*qstr_item_cb)(qstr_info_t *info, QINT code, QPNT *buff, QINT *size);

struct qstr_item
{
	QINT nflag;													// 格式化项标志
	QINT nparam;												// 格式化项参数
	QINT nfillflag;												// 格式化项填充标志
	QINT nfillchar;												// 格式化项填充字符
	QINT nfillsize;												// 格式化项填充长度
	QPNT pfmt;													// 格式化项指针
	QINT nfmtsize;												// 格式化项长度
	QPNT pname;													// 格式化项名称
	QINT nnamesize;												// 格式化项名称长度
};

struct qstr_info
{
	QPNT pstr;													// 输入字符串指针
	QINT nstrsize;												// 输入字符串长度
	qstr_item_cb pcbitem;										// 输入字符串项处理函数
	
	QPNT pbuff;													// 格式化缓冲指针
	QINT nbuffsize;												// 格式化缓冲长度
	
	qstr_item_t stitem;											// 当前格式化项信息
	va_list *pargs;												// 当前不定参数指针
	QPNT *pparams;												// 当前数组指针
	QINT ncount;												// 当前数组长度
};

QINT qstritem(QPNT fmt, qstr_item_t *item);															// 格式化项解析

QINT qstrwrite(qstr_info_t *info, QPNT buff, QINT len);												// 字符串写入，用于格式化项处理函数中
QINT qstrmakei(qstr_info_t *info);																	// 格式化项生成字符串的默认操作
QINT qstrmakex(QPNT str, QINT size, QPNT fmt, QPNT params[], QINT count);							// 字符串生成，输入为数组
QINT qstrmakev(QPNT str, QINT size, QPNT fmt, va_list args);										// 字符串生成，输入为不定参数指针
QINT qstrmake(QPNT str, QINT size, QPNT fmt, ...);													// 字符串生成，输入为不定参数
QINT qstrmakecbx(QPNT str, QINT size, qstr_item_cb make_cb, QPNT fmt, QPNT params[], QINT count);	// 字符串生成，输入带回调函数与数组
QINT qstrmakecbv(QPNT str, QINT size, qstr_item_cb make_cb, QPNT fmt, va_list args);				// 字符串生成，输入带回调函数与不定参数指针
QINT qstrmakecb(QPNT str, QINT size, qstr_item_cb make_cb, QPNT fmt, ...);							// 字符串生成，输入带回调函数与不定参数

QINT qstrread(qstr_info_t *info, QPNT buff, QINT len);												// 字符串读取，用于格式化项处理函数中
QINT qstrscani(qstr_info_t *info);																	// 格式化项获取字符串的默认操作
QINT qstrscanx(QPNT str, QINT size, QPNT fmt, QPNT params[], QINT count);							// 字符串获取，输入为数组
QINT qstrscanv(QPNT str, QINT size, QPNT fmt, va_list args);										// 字符串获取，输入为不定参数指针
QINT qstrscan(QPNT str, QINT size, QPNT fmt, ...);													// 字符串获取，输入为不定参数
QINT qstrscancbx(QPNT str, QINT size, qstr_item_cb scan_cb, QPNT fmt, QPNT params[], QINT count);	// 字符串获取，输入带回调函数与数组
QINT qstrscancbv(QPNT str, QINT size, qstr_item_cb scan_cb, QPNT fmt, va_list args);				// 字符串获取，输入带回调函数与不定参数指针
QINT qstrscancb(QPNT str, QINT size, qstr_item_cb scan_cb, QPNT fmt, ...);							// 字符串获取，输入带回调函数与不定参数
													
													
// -- 缓冲区

QHDL qbufmakev(QHDL hdl, QPNT buff, QINT size, QPCB free_cb, va_list marker, QINT *count);
QHDL qbufmakex(QHDL hdl, QPNT buff, QINT size, QPCB free_cb, QPNT params[], QINT *count);
QHDL qbufmake(QHDL hdl, QPNT buff, QINT size, QPCB free_cb, QINT *count, ...);
QINT qbuffree(QHDL buf);


// -- 位值
													
struct qstrenum_item
{
	QPNT pstr;													// 字符串
	QINT nvalue;												// 位值
};

struct qstrenum_data
{
	QHDL hcheck;												// 字符串
	QINT nflag;													// 位值
	struct qstrenum_item *pitems;
};

typedef const struct qstrenum_data *QBITS;
													
// 字符串位表声明
#define qstr_enum_extern(name)			extern const struct qstrenum_data *name
// 字符串位表定义
#define qstr_enum_begin(name)			extern const struct qstrenum_item name##list[];\
										const struct qstrenum_data name##data = { (QHDL)qchkhdl, (QINT)QLCN_HENM, (struct qstrenum_item *)name##list };\
										const struct qstrenum_data *name = &name##data;\
										const struct qstrenum_item name##list[] =\
										{
#define qstr_enum_item(name, value)			{ (QPNT)name, (QINT )value },
#define qstr_enum_end						{ NULL, 0 }\
										};

QPNT qstr4enum(QBITS bits, QINT value);							// 位值转字符串
QINT qstr2enum(QBITS bits, QINT flag, QPNT str, QINT *count);	// 字符串转位值


// -- URL

/*******************************************************************
 *
 * name = %index：读取第index个变量，同时返回该变量的名称，
 * 注：此时name不能为常量字符串。
 *
 *******************************************************************/
QINT qurlIsPath(QPNT url);
QINT qurlAccess(QPNT url, QINT mode);
QINT qurlMergeFilePath(QPNT defpath, QPNT filepath, QPNT path, QINT size);
QINT qurlExpandPath(QPNT path, QPNT url, QINT size);
QINT qurlGetProtocol(QPNT url, QPNT protocol, QINT size);
QINT qurlGetPath(QPNT url, QPNT path, QINT size);
QINT qurlGetFile(QPNT url, QPNT file, QINT size);
QINT qurlGetDir(QPNT url, QPNT dir, QINT size);
QINT qurlGetName(QPNT url, QPNT name, QINT size);
QINT qurlGetDomainName(QPNT url, QPNT name, QINT size);
QINT qurlGetExt(QPNT url, QPNT ext, QINT size);
QINT qurlGetParam(QPNT url, QPNT param, QINT size);
QINT qurlGetVar(QPNT url, QPNT name, QPNT value, QINT size);
QINT qurlSetVar(QPNT url, QPNT name, QPNT value, QINT size);
QINT qurlDelVar(QPNT url, QPNT name);
QINT qurlCpyVar(QPNT dsturl, QPNT dstname, QPNT srcurl, QPNT srcname);
QINT qurlCmp(QINT flag, QPNT url1, QPNT url2);
QINT qurlStrip(QPNT dst, QINT dstsize, QPNT src, QINT srcsize);
QINT qurlCopy(QPNT url, QPNT dst, QINT dstsize, QPNT src, QINT *srcsize);

typedef QINT (*qurl_scan_cb)(QPNT url, QPNT name, QPNT value, QINT size, QPNT params[], QINT count);
QINT qurlScanx(QPNT url, qurl_scan_cb scan_cb, QPNT params[], QINT count);
QINT qurlScan(QPNT url, qurl_scan_cb scan_cb, QINT count, ...);


// -- hash算法

typedef QINT (*qhash_value_cb)(QHDL hash, QPNT key, QPNT params[], QINT count);
typedef QINT (*qhash_equal_cb)(QHDL hash, QPNT key1, QPNT key2, QPNT params[], QINT count);

// 整体操作
QHDL qhashMake(QHDL mem);
QINT qhashFree(QHDL hash);
QINT qhashInitx(QHDL hash, qhash_value_cb value_cb, qhash_equal_cb equal_cb, QPCB free_cb, QPNT params[], QINT count);
QHDL qhashInit(QHDL hash, qhash_value_cb value_cb, qhash_equal_cb equal_cb, QPCB free_cb, QINT count, ...);
QINT qhashGetSize(QHDL hash);
qhash_value_cb qhashGetValueCb(QHDL hash);
qhash_equal_cb qhashGetEqualCb(QHDL hash);
qhash_value_cb qhashSetValueCb(QHDL hash, qhash_value_cb value_cb);
qhash_equal_cb qhashSetEqualCb(QHDL hash, qhash_equal_cb equal_cb);
QPNT qhashSetParamsx(QHDL hash, QPCB free_cb, QPNT params[], QINT count);
QPNT qhashSetParams(QHDL hash, QPCB free_cb, QINT count, ...);
QPNT qhashGetParams(QHDL hash);

// 结点操作
QPNT qhashNodeGetKey(QHDL node);
QPNT qhashNodeGetValue(QHDL node);
QPNT qhashNodeSetValue(QHDL node, QPNT value);
QPCB qhashNodeGetFreeCb(QHDL node);
QPCB qhashNodeSetFreeCb(QHDL node, QPCB free_cb);

// hash操作
QHDL qhashGetValue(QHDL hash, QPNT key);
QHDL qhashGetNode(QHDL hash, QPNT key);
QHDL qhashAddNode(QHDL hash, QPCB free_cb, QPNT key, QPNT value);
QINT qhashDelNode(QHDL hash, QPNT key);
QINT qhashScan(QHDL hash, QPCB scan_cb, QPNT in, QPNT out);

// 直接hash
QINT qhash_direct_value_cb(QHDL hash, QPNT key, QPNT params[], QINT count);
QINT qhash_direct_equal_cb(QHDL hash, QPNT key1, QPNT key2, QPNT params[], QINT count);
// 整数hash
QINT qhash_int_value_cb(QHDL hash, QPNT key, QPNT params[], QINT count);
QINT qhash_int_equal_cb(QHDL hash, QPNT key1, QPNT key2, QPNT params[], QINT count);
// 经典字符串hash
QINT qhash_commonstr_value_cb(QHDL hash, QPNT key, QPNT params[], QINT count);
QINT qhash_commonstr_equal_cb(QHDL hash, QPNT key1, QPNT key2, QPNT params[], QINT count);
// php字符串hash
QINT qhash_phpstr_value_cb(QHDL hash, QPNT key, QPNT params[], QINT count);
QINT qhash_phpstr_equal_cb(QHDL hash, QPNT key1, QPNT key2, QPNT params[], QINT count);
// 计算字符串hash
QINT qhash_calcstr_value_cb(QHDL hash, QPNT key, QPNT params[], QINT count);
QINT qhash_calcstr_equal_cb(QHDL hash, QPNT key1, QPNT key2, QPNT params[], QINT count);
QINT qhash_calcstr2_value_cb(QHDL hash, QPNT key, QPNT params[], QINT count);
QINT qhash_calcstr2_equal_cb(QHDL hash, QPNT key1, QPNT key2, QPNT params[], QINT count);
// Blizzard取值hash
QINT qhash_mpqstr_value_cb(QHDL hash, QPNT key, QPNT params[], QINT count);
QINT qhash_mpqstr_equal_cb(QHDL hash, QPNT key1, QPNT key2, QPNT params[], QINT count);


// -- 通用颜色

#define qclrMakeRGBA(r,g,b,a)		( ( ((r)&0xFF) | ((QWORD)((g)&0xFF)<<8) | ((QDWORD)((b)&0xFF)<<16) | ((QDWORD)((a)&0xFF)<<24) ) )
#define qclrGetRColor(rgba)			( ( (QBYTE)(rgba) ) & 0xFF )
#define qclrGetGColor(rgba)			( ( (QBYTE)((rgba)>>8) ) & 0xFF )
#define qclrGetBColor(rgba)			( ( (QBYTE)((rgba)>>16) ) & 0xFF )
#define qclrGetAColor(rgba)			( ( (QBYTE)((rgba)>>24) ) & 0xFF )

#define QCLR_BLACK					qclrMakeRGBA(0,   0,   0, 255)
#define QCLR_GRAY0					qclrMakeRGBA(0,   0,   0, 255)
#define QCLR_GRAY1					qclrMakeRGBA(17,  17,  17, 255)
#define QCLR_GRAY2					qclrMakeRGBA(34,  34,  34, 255)
#define QCLR_GRAY3					qclrMakeRGBA(51,  51,  51, 255)
#define QCLR_GRAY4					qclrMakeRGBA(68,  68,  68, 255)
#define QCLR_GRAY5					qclrMakeRGBA(85,  85,  85, 255)
#define QCLR_GRAY6					qclrMakeRGBA(102, 102, 102, 255)
#define QCLR_GRAY7					qclrMakeRGBA(119, 119, 119, 255)
#define QCLR_GRAY8					qclrMakeRGBA(136, 136, 136, 255)
#define QCLR_GRAY9					qclrMakeRGBA(153, 153, 153, 255)
#define QCLR_GRAY10					qclrMakeRGBA(170, 170, 170, 255)
#define QCLR_GRAY11					qclrMakeRGBA(187, 187, 187, 255)
#define QCLR_GRAY12					qclrMakeRGBA(204, 204, 204, 255)
#define QCLR_GRAY13					qclrMakeRGBA(221, 221, 221, 255)
#define QCLR_GRAY14					qclrMakeRGBA(238, 238, 238, 255)
#define QCLR_GRAY15					qclrMakeRGBA(255, 255, 255, 255)
#define QCLR_WHITE					qclrMakeRGBA(255, 255, 255, 255)

#define QCLR_BLUE					qclrMakeRGBA(0  , 0  , 128, 255)
#define QCLR_GREEN					qclrMakeRGBA(0  , 128, 0  , 255)
#define QCLR_CYAN					qclrMakeRGBA(0  , 128, 128, 255)
#define QCLR_RED					qclrMakeRGBA(128, 0  , 0  , 255)
#define QCLR_MAGENTA				qclrMakeRGBA(128, 0  , 128, 255)
#define QCLR_BROWN					qclrMakeRGBA(128, 128, 0  , 255)
#define QCLR_LTGRAY					qclrMakeRGBA(192, 192, 192, 255)
#define QCLR_GRAY					qclrMakeRGBA(128, 128, 128, 255)
#define QCLR_LTBLUE					qclrMakeRGBA(0  , 0  , 255, 255)
#define QCLR_LTGREEN				qclrMakeRGBA(0  , 255, 0  , 255)
#define QCLR_LTCYAN					qclrMakeRGBA(0  , 255, 255, 255)
#define QCLR_LTRED					qclrMakeRGBA(255, 0  , 0  , 255)
#define QCLR_LTMAGENTA				qclrMakeRGBA(255, 0  , 255, 255)
#define QCLR_YELLOW					qclrMakeRGBA(255, 255, 0  , 255)

qstr_enum_extern(QUIColor);
													
QCLR quiStr2Color(QSTR color, QINT size);


#endif
