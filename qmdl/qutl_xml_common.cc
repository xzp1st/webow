
#include "qmdl_config.h"
#include "qmdl_module.h"


QPNT qxmlGetTag(QHDL res)
{
	QXML pitem;
	
	pitem = (QXML)res;
	if(pitem == NULL)
	{
		return NULL;
	}
	
	return ((struct qxml_item_info *)pitem)->ptag;
}

QPNT qxmlGetId(QHDL res)
{
	QXML pitem;
	
	pitem = (QXML)res;
	if(pitem == NULL)
	{
		return NULL;
	}
	
	return ((struct qxml_item_info *)pitem)->pid;
}

QINT qxmlGetSize(QHDL res)
{
	QXML pitem;
	
	pitem = (QXML)res;
	if(pitem == NULL)
	{
		return NULL;
	}
	
	return ((struct qxml_item_info *)pitem)->nsize;
	
}

QPNT qxmlGetValp(QHDL res)
{
	QXML pitem;
	
	pitem = (QXML)res;
	if(pitem == NULL)
	{
		return NULL;
	}
	
	return ((struct qxml_item_info *)pitem)->pval;
}

QINT qxmlGetValn(QHDL res)
{
	QXML pitem;
	
	pitem = (QXML)res;
	if(pitem == NULL)
	{
		return -1;
	}
	
	return ((struct qxml_item_info *)pitem)->nval;
}

QPCB qxmlGetValcb(QHDL res)
{
	QXML pitem;
	
	pitem = (QXML)res;
	if(pitem == NULL)
	{
		return NULL;
	}
	
	return ((struct qxml_item_info *)pitem)->pcbval;
}
