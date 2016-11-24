
#include "qmdl_util.h"


QPNT qmalloc(QHDL mem, QINT size)
{
	return malloc(size);
}

void qmfree(QPNT data)
{
	free(data);
}
