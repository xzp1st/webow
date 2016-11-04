
#include "qmdl_util.h"


const QPNT qbool[1] = { NULL };
const QPNT qint[1] = { NULL };
const QPNT qpnt[1] = { NULL };
const QPNT qhdl[1] = { NULL };
const QPNT qstr[1] = { NULL };
const QPNT qpcb[1] = { NULL };

const QPNT qchkhdl[1] = { NULL };

const QPNT qnull[3] = { (QPNT )qnull, (QPNT )QLCN_NULL };

QINT qncb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QNO_OK;
}
