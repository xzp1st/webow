
#include "qmdl_util.h"


QPNT qstr4enumi(QENUM vals, QINT value)
{
	struct qstrenum_item *pitem;
	
	if(vals == NULL)
	{
		return NULL;
	}
	pitem = (struct qstrenum_item *)vals->pitems;
	while(pitem)
	{
		if(pitem->pstr == NULL)
		{
			if(pitem->nvalue == 0)
			{
				break;
			}
		}
		else
		{
			if(pitem->nvalue == value)
			{
				break;
			}
		}
		pitem++;
	}
	
	return pitem->pstr;
}

QINT qstr2enumi(QENUM vals, QINT flag, QPNT str, QINT *count)
{
	QINT nlen, nsize, ncount, nvalue;
	QUCHR *pcur, *pnext, vvalue[QSTR_BUFF_SIZE];
	struct qstrenum_item *pitem;
	
	if(vals == NULL || str == NULL)
	{
		if(count != NULL)
		{
			*count = 0;
		}
		
		return 0;
	}
	ncount = 0;
	nvalue = 0;
	pcur = (QUCHR *)str;
	if(count != NULL)
	{
		nsize = *count;
	}
	else
	{
		nsize = 0;
	}
	while(pcur)
	{
		pnext = (QUCHR *)qstrchr(0, (QPNT )pcur, nsize, (QINT )'|');
		if(pnext == NULL)
		{
			nlen = qstrlen((QUCHR *)pcur);
			qstrstrip(vvalue, sizeof(vvalue), pcur, nlen);
		}
		else
		{
			nlen = pnext-pcur;
			qstrstrip(vvalue, sizeof(vvalue), pcur, nlen);
		}
		pitem = (struct qstrenum_item *)vals->pitems;
		while(pitem)
		{
			if(pitem->pstr == NULL)
			{
				if(pitem->nvalue == 0)
				{
					break;
				}
			}
			else
			{
				if(qstrcmp(flag, pitem->pstr, vvalue, 0))
				{
					ncount++;
					nvalue |= (QINT)pitem->nvalue;
					break;
				}
			}
			pitem++;
		}
		if(pnext == NULL)
		{
			break;
		}
		pcur = pnext+1;
		nsize -= nlen+1;
	}
	if(count != NULL)
	{
		*count = ncount;
	}
	
	return nvalue;
}

QPNT qstr4enum(QENUM vals, QINT value)
{
	qstrenum_cb pcb;
	struct qstrenum_item *pitem;
	
	if(vals == NULL)
	{
		return NULL;
	}
	pitem = (struct qstrenum_item *)vals->pitems;
	while(pitem)
	{
		if(pitem->pstr == NULL)
		{
			pcb = (qstrenum_cb)pitem->nvalue;
			if(pcb != NULL)
			{
				// 定制枚举取值转换。
				return (QPNT)pcb(vals, 0, NULL, NULL, value);
			}
			break;
		}
		if(pitem->nvalue == value)
		{
			break;
		}
		pitem++;
	}
	
	return pitem->pstr;
}

QINT qstr2enum(QENUM vals, QINT flag, QPNT str, QINT *count)
{
	qstrenum_cb pcb;
	QINT nlen, nsize, ncount, nvalue;
	QUCHR *pcur, *pnext, vvalue[QSTR_BUFF_SIZE];
	struct qstrenum_item *pitem;
	
	if(vals == NULL || str == NULL)
	{
		if(count != NULL)
		{
			*count = 0;
		}
		
		return 0;
	}
	ncount = 0;
	nvalue = 0;
	pcur = (QUCHR *)str;
	if(count != NULL)
	{
		nsize = *count;
	}
	else
	{
		nsize = 0;
	}
	while(pcur)
	{
		pnext = (QUCHR *)qstrchr(0, (QPNT )pcur, nsize, (QINT )'|');
		if(pnext == NULL)
		{
			nlen = qstrlen((QUCHR *)pcur);
			qstrstrip(vvalue, sizeof(vvalue), pcur, nlen);
		}
		else
		{
			nlen = pnext-pcur;
			qstrstrip(vvalue, sizeof(vvalue), pcur, nlen);
		}
		pitem = (struct qstrenum_item *)vals->pitems;
		while(pitem)
		{
			if(pitem->pstr == NULL)
			{
				pcb = (qstrenum_cb)pitem->nvalue;
				if(pcb != NULL)
				{
					// 定制枚举取值转换。
					return pcb(vals, flag, str, count, 0);
				}
				break;
			}
			if(qstrcmp(flag, pitem->pstr, vvalue, 0))
			{
				ncount++;
				nvalue |= (QINT )pitem->nvalue;
				break;
			}
			pitem++;
		}
		if(pnext == NULL)
		{
			break;
		}
		pcur = pnext+1;
		nsize -= nlen+1;
	}
	if(count != NULL)
	{
		*count = ncount;
	}
	
	return nvalue;
}
