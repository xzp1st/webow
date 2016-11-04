
#ifndef __QMDL_MODULE_INTERNAL_H__
#define __QMDL_MODULE_INTERNAL_H__

#include "qmdl_config.h"


#define QLINK_ATTR					(QLINK_HDR|QLINK_DBL)

struct qmdl_attr_item
{
    QINT nflag;
    QSTR pname;
    QPNT ptype;
    QPNT pval;
    QXML pxmlval;
    QPCB pcbval;
    QHDL hsublink;
};


#endif //__QMDL_MODULE_INTERNAL_H__
