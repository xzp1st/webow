
#ifndef __QMDL_CONFIG_H__
#define __QMDL_CONFIG_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


// -- 平台定义
//#define QMDL_VER_WIN32
//#define QMDL_VER_GF
#define QMDL_VER_IOS
//#define QMDL_VER_ANDROID


// -- 缓冲配置
#define QPRM_BUFF_SIZE						128					// 参数表缓冲大小
#define QSTR_BUFF_SIZE						1024				// 字符串缓冲大小
#define QSCN_BUFF_SIZE						128					// 遍历缓冲大小
#define QSCN_STACK_CNT						32					// 遍历缓冲层级数


// -- 位置配置
#define QPATH_STK							"/stk/"
#define QPATH_LIB							"/lib/"


#endif
