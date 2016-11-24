
#include "qmdl_config.h"
#include "qmdl_module.h"


#define qxml_match_flag(inflag, itemflag)\
	( ((inflag)==0) ? 1 :\
	( ( ((inflag)|(QXML_TREE_MASK|QLCN_MASK))==((itemflag)|(QXML_TREE_MASK|QLCN_MASK)) ) ? 1 : 0\
	)\
	)

static QHDL qxml_locate_begin(QHDL res)
{
	QINT nflag, ncount;
	QXML pitem;

	pitem = (QXML)res;
	if(pitem == NULL)
	{
		return NULL;
	}
	nflag = pitem->nflag;
	if(nflag & QXML_HEAD)
	{
		return (QPNT )pitem;
	}
	if(nflag & QXML_BEGIN)
	{
		return NULL;
	}
	if(nflag & QXML_TAIL)
	{
		if(pitem->pcbprev != NULL)
		{
			pitem = pitem->pcbprev(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem-pitem->nsize);
		}
		else
		{
			pitem--;
		}
	}
	ncount = 1;
	while(1)
	{
		nflag = pitem->nflag;
		if(nflag & QXML_TAIL)
		{
			ncount++;
		}
		if(nflag & QXML_HEAD)
		{
			ncount--;
			if(ncount == 0)
			{
				return (QPNT )pitem;
			}
		}
		if(nflag & QXML_BEGIN)
		{
			break;
		}
		if(pitem->pcbprev != NULL)
		{
			pitem = pitem->pcbprev(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem-pitem->nsize);
		}
		else
		{
			pitem--;
		}
	}

	return NULL;
}

static QHDL qxml_locate_end(QHDL res)
{
	QINT nflag, ncount;
	QXML pitem;

	pitem = (QXML)res;
	if(pitem == NULL)
	{
		return NULL;
	}
	nflag = pitem->nflag;
	if(nflag & QXML_TAIL)
	{
		return (QPNT )pitem;
	}
	if(nflag & QXML_END)
	{
		return NULL;
	}
	if(nflag & QXML_HEAD)
	{
		if(pitem->pcbnext != NULL)
		{
			pitem = pitem->pcbnext(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem+pitem->nsize);
		}
		else
		{
			pitem++;
		}
	}
	ncount = 1;
	while(1)
	{
		nflag = pitem->nflag;
		if(nflag & QXML_HEAD)
		{
			ncount++;
		}
		if(nflag & QXML_TAIL)
		{
			ncount--;
			if(ncount == 0)
			{
				return (QPNT )pitem;
			}
		}
		if(nflag & QXML_END)
		{
			break;
		}
		if(pitem->pcbnext != NULL)
		{
			pitem = pitem->pcbnext(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem+pitem->nsize);
		}
		else
		{
			pitem++;
		}
	}

	return NULL;
}

QINT qxmlGetFlag(QHDL res)
{
	QXML pitem;

	pitem = (QXML)res;
	if(pitem == NULL)
	{
		return 0;
	}

	return pitem->nflag;
}

QHDL qxmlGetRoot(QHDL res)
{
	QXML pitem;

	pitem = (QXML)res;
	if(pitem == NULL)
	{
		return NULL;
	}
	while(1)
	{
		if(pitem->nflag & QXML_BEGIN)
		{
			break;
		}
		if(pitem->pcbprev != NULL)
		{
			pitem = pitem->pcbprev(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem-pitem->nsize);
		}
		else
		{
			pitem--;
		}
	}

	return (QHDL )pitem;
}

QHDL qxmlGetParent(QHDL res)
{
	QINT nflag, ncount;
	QXML pitem;

	if(res == NULL)
	{
		return NULL;
	}
	pitem = (QXML)qxml_locate_begin(res);
	if(pitem == NULL)
	{
		return NULL;
	}
	nflag = pitem->nflag;
	if(nflag & QXML_BEGIN)
	{
		return NULL;
	}
	if(pitem->pcbprev != NULL)
	{
		pitem = pitem->pcbprev(pitem);
	}
	else if(pitem->nsize > 0)
	{
		pitem = (QXML)((QCHR *)pitem-pitem->nsize);
	}
	else
	{
		pitem--;
	}
	ncount = 1;
	while(1)
	{
		nflag = pitem->nflag;
		if(nflag & QXML_TAIL)
		{
			ncount++;
		}
		if(nflag & QXML_HEAD)
		{
			ncount--;
			if(ncount == 0)
			{
				return (QHDL )pitem;
			}
		}
		if(nflag & QXML_BEGIN)
		{
			break;
		}
		if(pitem->pcbprev != NULL)
		{
			pitem = pitem->pcbprev(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem-pitem->nsize);
		}
		else
		{
			pitem--;
		}
	}

	return NULL;
}

QHDL qxmlGetPrev(QHDL res)
{
	QINT nflag;
	QXML pitem;

	if(res == NULL)
	{
		return NULL;
	}
	pitem = (QXML)qxml_locate_begin(res);
	if(pitem == NULL)
	{
		return NULL;
	}
	nflag = pitem->nflag;
	if(nflag & QXML_BEGIN)
	{
		return NULL;
	}
	if(pitem->pcbprev != NULL)
	{
		pitem = pitem->pcbprev(pitem);
	}
	else if(pitem->nsize > 0)
	{
		pitem = (QXML)((QCHR *)pitem-pitem->nsize);
	}
	else
	{
		pitem--;
	}
	while(1)
	{
		nflag = pitem->nflag;
		if(nflag & QXML_TAIL)
		{
			return qxml_locate_begin((QPNT )pitem);
		}
		if(nflag & QXML_HEAD)
		{
			break;
		}
		if(nflag & QXML_BEGIN)
		{
			break;
		}
		if(pitem->pcbprev != NULL)
		{
			pitem = pitem->pcbprev(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem-pitem->nsize);
		}
		else
		{
			pitem--;
		}
	}

	return NULL;
}

QHDL qxmlGetNext(QHDL res)
{
	QINT nflag;
	QXML pitem;

	if(res == NULL)
	{
		return NULL;
	}
	pitem = (QXML)qxml_locate_end(res);
	if(pitem == NULL)
	{
		return NULL;
	}
	nflag = pitem->nflag;
	if(nflag & QXML_END)
	{
		return NULL;
	}
	if(pitem->pcbnext != NULL)
	{
		pitem = pitem->pcbnext(pitem);
	}
	else if(pitem->nsize > 0)
	{
		pitem = (QXML)((QCHR *)pitem+pitem->nsize);
	}
	else
	{
		pitem++;
	}
	while(1)
	{
		nflag = pitem->nflag;
		if(nflag & QXML_HEAD)
		{
			return (QHDL )pitem;
		}
		if(nflag & QXML_TAIL)
		{
			break;
		}
		if(nflag & QXML_END)
		{
			break;
		}
		if(pitem->pcbnext != NULL)
		{
			pitem = pitem->pcbnext(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem+pitem->nsize);
		}
		else
		{
			pitem++;
		}
	}

	return NULL;
}

QHDL qxmlGetHead(QHDL res)
{
	QINT nflag;
	QXML pitem;

	if(res == NULL)
	{
		return NULL;
	}
	pitem = (QXML)qxml_locate_begin(res);
	if(pitem == NULL)
	{
		return NULL;
	}
	nflag = pitem->nflag;
	if( (nflag & QXML_HEAD) && (nflag & QXML_TAIL) )
	{
		return NULL;
	}
	if(nflag & QXML_END)
	{
		return NULL;
	}
	if(pitem->pcbnext != NULL)
	{
		pitem = pitem->pcbnext(pitem);
	}
	else if(pitem->nsize > 0)
	{
		pitem = (QXML)((QCHR *)pitem+pitem->nsize);
	}
	else
	{
		pitem++;
	}
	while(1)
	{
		nflag = pitem->nflag;
		if(nflag & QXML_HEAD)
		{
			return (QHDL )pitem;
		}
		if(nflag & QXML_TAIL)
		{
			break;
		}
		if(nflag & QXML_END)
		{
			break;
		}
		if(pitem->pcbnext != NULL)
		{
			pitem = pitem->pcbnext(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem+pitem->nsize);
		}
		else
		{
			pitem++;
		}
	}

	return NULL;
}

QHDL qxmlGetTail(QHDL res)
{
	QINT nflag;
	QXML pitem;

	if(res == NULL)
	{
		return NULL;
	}
	pitem = (QXML)qxml_locate_end(res);
	if(pitem == NULL)
	{
		return NULL;
	}
	nflag = pitem->nflag;
	if( (nflag & QXML_HEAD) && (nflag & QXML_TAIL) )
	{
		return NULL;
	}
	if(nflag & QXML_BEGIN)
	{
		return NULL;
	}
	if(pitem->pcbprev != NULL)
	{
		pitem = pitem->pcbprev(pitem);
	}
	else if(pitem->nsize > 0)
	{
		pitem = (QXML)((QCHR *)pitem-pitem->nsize);
	}
	else
	{
		pitem--;
	}
	while(1)
	{
		nflag = pitem->nflag;
		if(nflag & QXML_TAIL)
		{
			return qxml_locate_begin((QPNT )pitem);
		}
		if(nflag & QXML_HEAD)
		{
			break;
		}
		if(nflag & QXML_BEGIN)
		{
			break;
		}
		if(pitem->pcbprev != NULL)
		{
			pitem = pitem->pcbprev(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem-pitem->nsize);
		}
		else
		{
			pitem--;
		}
	}

	return NULL;
}

QINT qxmlCntAttr(QHDL res, QINT flag)
{
	QINT nflag, ncount;
	QXML pitem;

	if(res == NULL)
	{
		return -1;
	}
	pitem = (QXML)qxml_locate_begin(res);
	if(pitem == NULL)
	{
		return -1;
	}
	nflag = pitem->nflag;
	if( (nflag & QXML_HEAD) && (nflag & QXML_TAIL) )
	{
		return 0;
	}
	if(nflag & QXML_END)
	{
		return 0;
	}
	if(pitem->pcbnext != NULL)
	{
		pitem = pitem->pcbnext(pitem);
	}
	else if(pitem->nsize > 0)
	{
		pitem = (QXML)((QCHR *)pitem+pitem->nsize);
	}
	else
	{
		pitem++;
	}
	ncount = 0;
	while(1)
	{
		nflag = pitem->nflag;
		if(nflag & QXML_HEAD)
		{
			//  Ù–‘Ω·µ„“™‘⁄µ⁄“ª∏ˆ∫¢◊”Ω·µ„÷Æ«∞°£
			break;
		}
		if(nflag & QXML_TAIL)
		{
			//  Ù–‘Ω·µ„“™‘⁄µ⁄“ª∏ˆ∫¢◊”Ω·µ„÷Æ«∞°£
			break;
		}
		if(nflag & QXML_ATTR)
		{
			if(qxml_match_flag(flag, nflag))
			{
				ncount++;
			}
		}
		if(nflag & QXML_END)
		{
			break;
		}
		if(pitem->pcbnext != NULL)
		{
			pitem = pitem->pcbnext(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem+pitem->nsize);
		}
		else
		{
			pitem++;
		}
	}

	return ncount;
}

QHDL qxmlGetAttr(QHDL res, QINT flag, QPNT name, QINT index)
{
	QXML pitem;
	QINT nflag, nindex, nmatch;

	if(res == NULL)
	{
		return NULL;
	}
	if(name == NULL && index < 0)
	{
		return NULL;
	}
	pitem = (QXML)qxml_locate_begin(res);
	if(pitem == NULL)
	{
		return NULL;
	}
	nflag = pitem->nflag;
	if( (nflag & QXML_HEAD) && (nflag & QXML_TAIL) )
	{
		return NULL;
	}
	if(nflag & QXML_END)
	{
		return NULL;
	}
	if(pitem->pcbnext != NULL)
	{
		pitem = pitem->pcbnext(pitem);
	}
	else if(pitem->nsize > 0)
	{
		pitem = (QXML)((QCHR *)pitem+pitem->nsize);
	}
	else
	{
		pitem++;
	}
	nindex = 0;
	while(1)
	{
		nflag = pitem->nflag;
		if(nflag & (QXML_HEAD|QXML_TAIL))
		{
			//  Ù–‘Ω·µ„“™‘⁄µ⁄“ª∏ˆ∫¢◊”Ω·µ„÷Æ«∞°£
			break;
		}
		if(nflag & QXML_ATTR)
		{
			nmatch = 0;
			if(qxml_match_flag(flag, nflag))
			{
				if(name != NULL)
				{
					if(pitem->pid != NULL && qstrcmp(QSTR_ICS, pitem->pid, name, 0))
					{
						nmatch = 1;
					}
				}
				else
				{
					if(nindex == index)
					{
						nmatch = 1;
					}
				}
			}
			if(nmatch == 1)
			{
				return (QHDL )pitem;
			}
			nindex++;
		}
		if(nflag & QXML_END)
		{
			break;
		}
		if(pitem->pcbnext != NULL)
		{
			pitem = pitem->pcbnext(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem+pitem->nsize);
		}
		else
		{
			pitem++;
		}
	}

	return NULL;
}

QHDL qxmlAttrGetHead(QHDL res, QINT flag)
{
	QINT nflag;
	QXML pitem;

	if(res == NULL)
	{
		return NULL;
	}
	pitem = (QXML)qxml_locate_begin(res);
	if(pitem == NULL)
	{
		return NULL;
	}
	nflag = pitem->nflag;
	if( (nflag & QXML_HEAD) && (nflag & QXML_TAIL) )
	{
		return NULL;
	}
	if(nflag & QXML_END)
	{
		return NULL;
	}
	if(pitem->pcbnext != NULL)
	{
		pitem = pitem->pcbnext(pitem);
	}
	else if(pitem->nsize > 0)
	{
		pitem = (QXML)((QCHR *)pitem+pitem->nsize);
	}
	else
	{
		pitem++;
	}
	while(1)
	{
		nflag = pitem->nflag;
		if(nflag & (QXML_HEAD|QXML_TAIL))
		{
			//  Ù–‘Ω·µ„“™‘⁄µ⁄“ª∏ˆ∫¢◊”Ω·µ„÷Æ«∞°£
			break;
		}
		if(nflag & QXML_ATTR)
		{
			if(qxml_match_flag(flag, nflag))
			{
				return (QHDL )pitem;
			}
		}
		if(nflag & QXML_END)
		{
			break;
		}
		if(pitem->pcbnext != NULL)
		{
			pitem = pitem->pcbnext(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem+pitem->nsize);
		}
		else
		{
			pitem++;
		}
	}

	return NULL;
}

QHDL qxmlAttrGetTail(QHDL res, QINT flag)
{
	QHDL htail;
	QINT nflag;
	QXML pitem;

	if(res == NULL)
	{
		return NULL;
	}
	pitem = (QXML)qxml_locate_begin(res);
	if(pitem == NULL)
	{
		return NULL;
	}
	nflag = pitem->nflag;
	if( (nflag & QXML_HEAD) && (nflag & QXML_TAIL) )
	{
		return NULL;
	}
	if(nflag & QXML_END)
	{
		return NULL;
	}
	if(pitem->pcbnext != NULL)
	{
		pitem = pitem->pcbnext(pitem);
	}
	else if(pitem->nsize > 0)
	{
		pitem = (QXML)((QCHR *)pitem+pitem->nsize);
	}
	else
	{
		pitem++;
	}
	htail = NULL;
	while(1)
	{
		nflag = pitem->nflag;
		if(nflag & (QXML_HEAD|QXML_TAIL))
		{
			//  Ù–‘Ω·µ„“™‘⁄µ⁄“ª∏ˆ∫¢◊”Ω·µ„÷Æ«∞°£
			break;
		}
		if(nflag & QXML_ATTR)
		{
			if(qxml_match_flag(flag, nflag))
			{
				htail = (QHDL )pitem;
			}
		}
		if(nflag & QXML_END)
		{
			break;
		}
		if(pitem->pcbnext != NULL)
		{
			pitem = pitem->pcbnext(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem+pitem->nsize);
		}
		else
		{
			pitem++;
		}
	}

	return htail;
}

QHDL qxmlAttrGetNext(QHDL res, QINT flag)
{
	QINT nflag;
	QXML pitem;

	pitem = (QXML)res;
	if(pitem == NULL)
	{
		return NULL;
	}
	if(pitem->nflag & (QXML_TAIL|QXML_END))
	{
		return NULL;
	}
	if(pitem->pcbnext != NULL)
	{
		pitem = pitem->pcbnext(pitem);
	}
	else if(pitem->nsize > 0)
	{
		pitem = (QXML)((QCHR *)pitem+pitem->nsize);
	}
	else
	{
		pitem++;
	}
	while(1)
	{
		nflag = pitem->nflag;
		if(nflag & (QXML_HEAD|QXML_TAIL))
		{
			//  Ù–‘Ω·µ„“™‘⁄µ⁄“ª∏ˆ∫¢◊”Ω·µ„÷Æ«∞°£
			break;
		}
		if(nflag & QXML_ATTR)
		{
			if(qxml_match_flag(flag, nflag))
			{
				return (QHDL )pitem;
			}
		}
		else
		{
			break;
		}
		if(nflag & QXML_END)
		{
			break;
		}
		if(pitem->pcbnext != NULL)
		{
			pitem = pitem->pcbnext(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem+pitem->nsize);
		}
		else
		{
			pitem++;
		}
	}

	return NULL;
}

QHDL qxmlAttrGetPrev(QHDL res, QINT flag)
{
	QINT nflag;
	QXML pitem;

	pitem = (QXML)res;
	if(pitem == NULL)
	{
		return NULL;
	}
	if(pitem->nflag & (QXML_BEGIN|QXML_HEAD))
	{
		return NULL;
	}
	if(pitem->pcbprev != NULL)
	{
		pitem = pitem->pcbprev(pitem);
	}
	else if(pitem->nsize > 0)
	{
		pitem = (QXML)((QCHR *)pitem-pitem->nsize);
	}
	else
	{
		pitem--;
	}
	while(1)
	{
		nflag = pitem->nflag;
		if(nflag & (QXML_HEAD|QXML_TAIL))
		{
			//  Ù–‘Ω·µ„“™‘⁄µ⁄“ª∏ˆ∫¢◊”Ω·µ„÷Æ«∞°£
			break;
		}
		if(nflag & QXML_ATTR)
		{
			if(qxml_match_flag(flag, nflag))
			{
				return (QHDL )pitem;
			}
		}
		else
		{
			break;
		}
		if(nflag & QXML_BEGIN)
		{
			break;
		}
		if(pitem->pcbprev != NULL)
		{
			pitem = pitem->pcbprev(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem-pitem->nsize);
		}
		else
		{
			pitem--;
		}
	}

	return NULL;
}

QINT qxmlCntChild(QHDL res, QINT flag)
{
	QXML pitem;
	QINT nflag, ncount, nmatch;

	if(res == NULL)
	{
		return -1;
	}
	pitem = (QXML)qxml_locate_begin(res);
	if(pitem == NULL)
	{
		return -1;
	}
	nflag = pitem->nflag;
	if( (nflag & QXML_HEAD) && (nflag & QXML_TAIL) )
	{
		return 0;
	}
	if(nflag & QXML_END)
	{
		return 0;
	}
	if(pitem->pcbnext != NULL)
	{
		pitem = pitem->pcbnext(pitem);
	}
	else if(pitem->nsize > 0)
	{
		pitem = (QXML)((QCHR *)pitem+pitem->nsize);
	}
	else
	{
		pitem++;
	}
	ncount = 0;
	nmatch = 0;
	while(1)
	{
		nflag = pitem->nflag;
		if(nflag & QXML_HEAD)
		{
			if(nmatch == 0)
			{
				if(qxml_match_flag(flag, nflag))
				{
					ncount++;
				}
			}
			nmatch++;
		}
		if(nflag & QXML_TAIL)
		{
			nmatch--;
		}
		if(nmatch < 0)
		{
			break;
		}
		if(nflag & QXML_END)
		{
			break;
		}
		if(pitem->pcbnext != NULL)
		{
			pitem = pitem->pcbnext(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem+pitem->nsize);
		}
		else
		{
			pitem++;
		}
	}

	return ncount;
}

QHDL qxmlGetChild(QHDL res, QINT flag, QINT index)
{
	QHDL hchild;
	QINT nflag, nindex, ndstindex;

	nindex = 0;
	if(index >= 0)
	{
		ndstindex = index;
	}
	else
	{
		ndstindex = index*(-1)-1;
	}
	if(index >= 0)
	{
		hchild = qxmlGetHead(res);
	}
	else
	{
		hchild = qxmlGetTail(res);
	}
	while(hchild)
	{
		nflag = qxmlGetFlag(hchild);
		if(qxml_match_flag(flag, nflag))
		{
			if(nindex == ndstindex)
			{
				break;
			}
			nindex++;
		}
		if(index >= 0)
		{
			hchild = qxmlGetNext(hchild);
		}
		else
		{
			hchild = qxmlGetPrev(hchild);
		}
	}

	return hchild;
}

QHDL qxmlGetSibling(QHDL res, QINT flag, QINT index)
{
	QHDL hsibling;
	QINT nflag, nindex, ndstindex;

	if(index == 0)
	{
		return res;
	}
	if(index > 0)
	{
		ndstindex = index-1;
		hsibling = qxmlGetNext(res);
	}
	else
	{
		ndstindex = index*(-1)-1;
		hsibling = qxmlGetPrev(res);
	}
	nindex = 0;
	while(hsibling)
	{
		nflag = qxmlGetFlag(hsibling);
		if(qxml_match_flag(flag, nflag))
		{
			if(nindex == ndstindex)
			{
				break;
			}
			nindex++;
		}
		if(index > 0)
		{
			hsibling = qxmlGetNext(hsibling);
		}
		else
		{
			hsibling = qxmlGetPrev(hsibling);
		}
	}

	return hsibling;
}

QINT qxmlScanx(QHDL res, QPCB prev_cb, QPCB post_cb, QPNT params[], QINT count)
{
	QHDL hres, hparent, hchild;
	QINT nresult, nflag, nlevel;
	QCHR vflag[QSCN_BUFF_SIZE] = {0};

	hres = res;
	if(hres == NULL)
	{
		return QSCN_ERR;
	}
	nlevel = 0;
    nresult = QSCN_OK;
	nflag = QSCN_OK;
	while(hres)
	{
		// «∞–Ú¥¶¿Ì°£
		nresult = QSCN_OK;
		if(prev_cb != NULL)
		{
			nresult = prev_cb(hres, NULL, QCD_CALL, params, count);
		}
		if(nresult == QSCN_END)
		{
			break;
		}
		else if(nresult == QSCN_FAIL)
		{
			// ∫Û–Ú¥¶¿Ì°£
			nflag = QSCN_FAIL;
			break;
		}
		if(nresult == QSCN_VERT)
		{
			// º«¬º¥π÷±±È¿˙°£
			vflag[nlevel] = 1;
		}
		nlevel++;
		if(nresult == QSCN_HORZ)
		{
			hchild = NULL;
		}
		else
		{
			hchild = qxmlGetHead(hres);
		}
		if(hchild == NULL)
		{
			// ∫Û–Ú¥¶¿Ì°£
			if(post_cb != NULL)
			{
				post_cb(hres, NULL, QCD_CALL, params, count);
			}
			nlevel--;
			if(nlevel >= 0 && hres != res)
			{
				hparent = hres;
				if(vflag[nlevel] == 1)
				{
					// »•≥˝¥π÷±±Íº«°£
					vflag[nlevel] = 0;
				}
				else
				{
					// ºÃ–¯¥¶¿Ì°£
					hchild = qxmlGetNext(hparent);
				}
				while(hchild == NULL)
				{
					hparent = qxmlGetParent(hparent);
					if(hparent == NULL)
					{
						break;
					}
					// ∫Û–Ú¥¶¿Ì°£
					if(post_cb != NULL)
					{
						post_cb(hparent, NULL, QCD_CALL, params, count);
					}
					nlevel--;
					if(nlevel < 0 || hparent == res)
					{
						break;
					}
					if(vflag[nlevel] == 1)
					{
						// »•≥˝¥π÷±±Íº«°£
						vflag[nlevel] = 0;
					}
					else
					{
						// ºÃ–¯¥¶¿Ì°£
						hchild = qxmlGetNext(hparent);
					}
				}
			}
		}
		hres = hchild;
	}
	if(nflag == QSCN_FAIL)
	{
		hparent = hres;
		while(hparent)
		{
			// ∫Û–Ú¥¶¿Ì°£
			if(post_cb != NULL)
			{
				post_cb(hparent, NULL, QCD_CALL, params, count);
			}
			nlevel--;
			if(hparent == res)
			{
				break;
			}
			hparent = qxmlGetParent(hparent);
		}
	}

	return nresult;
}

QINT qxmlScan(QHDL res, QPCB prev_cb, QPCB post_cb, QINT count, ...)
{
	QINT nresult;
	QPNT *ppparams;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;

	va_start( marker, count );
	ppparams = (QPNT *)qprmmakev(res, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
	va_end( marker );

	nresult = qxmlScanx(res, prev_cb, post_cb, ppparams, count);

	qprmfree(ppparams);

	return nresult;
}

QINT qxmlGetPath(QHDL parent, QHDL child, QPNT path, QINT size)
{
	QSTR pname, ppath;
	QINT nlen, nsize, nindex, ncount;
	QXML pmxml, *ppmxml, vpmxml[QSCN_STACK_CNT];
	
	if(child == NULL)
	{
		pmxml = (QXML)parent;
	}
	else
	{
		pmxml = (QXML)child;
	}
	if(pmxml == NULL)
	{
		return 0;
	}
	ncount = 0;
	ppmxml = vpmxml;
	while(pmxml)
	{
		if(ncount >= QSCN_STACK_CNT)
		{
			break;
		}
		*ppmxml++ = pmxml;
		ncount++;
		if(pmxml == parent)
		{
			break;
		}
		pmxml = (QXML)qxmlGetParent(pmxml);
	}
	nsize = 0;
	ppath = (QSTR )path;
	if(pmxml != NULL)
	{
		pmxml = vpmxml[ncount-1];
		pname = (QSTR )qxmlGetId(pmxml);
		if(ppath != NULL)
		{
			if( (size <= 0) || (size > 0 && nsize < size) )
			{
				nlen = qstrcpy(ppath, size-nsize, pname, 0);
			}
			else
			{
				nlen = qstrlen(pname);
			}
		}
		else
		{
			nlen = qstrlen(pname);
		}
		nsize += nlen;
	}
	else
	{
		if(ppath != NULL)
		{
			if( (size <= 0) || (size > 0 && nsize < size) )
			{
				*ppath = '/';
			}
		}
		nsize++;
	}
	nindex = ncount-2;
	ppmxml = &vpmxml[nindex];
	while(nindex >= 0)
	{
		pmxml = *ppmxml--;
		pname = (QSTR )qxmlGetId(pmxml);
		if(ppath != NULL)
		{
			if( (size <= 0) || (size > 0 && nsize < size) )
			{
				*ppath = '/';
				nsize++;
				nlen = qstrcpy(ppath, size-nsize, pname, 0);
			}
			else
			{
				break;
			}
		}
		else
		{
			nlen = qstrlen(pname);
		}
		nsize += nlen;
		nindex--;
	}
	
	return nsize;
}

QHDL qxmlFind(QHDL res, QPNT path)
{
	QINT nlen;
	QHDL hchild, hparent;
	QCHR *pid, *pcur, *pchar;
	
	if(res == NULL || path == NULL)
	{
		return NULL;
	}
	pchar = (QCHR *)path;
	if(*pchar == '/')
	{
		hparent = qxmlGetRoot(res);
	}
	else
	{
		hparent = res;
	}
	while(hparent)
	{
		// ∫ˆ¬‘∂‡”‡µƒ'/'°£
		while(1)
		{
			if(*pchar == 0)
			{
				break;
			}
			else if(*pchar != '/')
			{
				break;
			}
			pchar++;
		}
		nlen = 0;
		pcur = pchar;
		while(1)
		{
			if(*pchar == 0)
			{
				break;
			}
			else if(*pchar == '?')
			{
				break;
			}
			else if(*pchar == '/')
			{
				break;
			}
			pchar++;
			nlen++;
		}
		if(nlen <= 0)
		{
			break;
		}
		hchild = qxmlGetHead(hparent);
		while(hchild)
		{
			pid = (QCHR *)qxmlGetId(hchild);
			if(pid != NULL && pid[nlen] == 0 && qstrcmp(QSTR_NONE, pid, pcur, nlen))
			{
				break;
			}
			hchild = qxmlGetNext(hchild);
		}
		hparent = hchild;
	}
	
	return hparent;
}

QHDL qxmlMake(QHDL mem)
{
	QXML pmxml;
	
	pmxml = (QXML)qmalloc(mem, sizeof(QMXML));
	if(pmxml == NULL)
	{
		return NULL;
	}
	memset(pmxml, 0, sizeof(QMXML));
	pmxml->nflag |= QXML_ALLOC;
	
	return (QHDL )pmxml;
}

QINT qxmlFree(QHDL mxml)
{
	QINT ncount;
	QXML phead, ptail, pitem, pprev;
	
	phead = (QXML)qxml_locate_begin(mxml);
	ptail = (QXML)qxml_locate_end(mxml);
	if(phead == NULL || ptail == NULL)
	{
		return NULL;
	}
	ncount = 0;
	pitem = ptail;
	while(1)
	{
		if(pitem->pcbprev != NULL)
		{
			pprev = pitem->pcbprev(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pprev = (QXML)((QCHR *)pitem-pitem->nsize);
		}
		else
		{
			pprev = pitem-1;
		}
		if(pitem->nflag & QXML_ALLOC)
		{
			qmfree(pitem);
		}
		ncount++;
		if(pitem == phead)
		{
			break;
		}
		pitem = pprev;
	}
	
	return ncount;
}

QHDL qxmlDupx(QHDL mem, QHDL mxml, QPCB dup_cb, QPNT params[], QINT count)
{
	QINT ncount;
	QPNT vpprms[4] = {0};
	QXML phead, ptail, pitem, pdup, pnew;
	
	phead = (QXML)qxml_locate_begin(mxml);
	ptail = (QXML)qxml_locate_end(mxml);
	if(phead == NULL || ptail == NULL)
	{
		return NULL;
	}
	ncount = 0;
	pitem = phead;
	while(1)
	{
		ncount++;
		if(pitem == ptail)
		{
			break;
		}
		if(pitem->pcbnext != NULL)
		{
			pitem = pitem->pcbnext(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem+pitem->nsize);
		}
		else
		{
			pitem++;
		}
	}
	pnew = (QXML)qmalloc(mem, sizeof(QMXML)*ncount);
	if(pnew == NULL)
	{
		return NULL;
	}
	memset(pnew, 0, sizeof(QMXML)*ncount);
	pdup = pnew;
	pitem = phead;
	while(1)
	{
		if(dup_cb == NULL)
		{
			memcpy(pdup, pitem, sizeof(QMXML));
		}
		else
		{
			vpprms[0] = (QPNT )pitem;
			dup_cb((QHDL )pdup, NULL, QCD_CALL, vpprms, 1);
		}
		if(pitem == ptail)
		{
			break;
		}
		if(pitem->pcbnext != NULL)
		{
			pitem = pitem->pcbnext(pitem);
		}
		else if(pitem->nsize > 0)
		{
			pitem = (QXML)((QCHR *)pitem+pitem->nsize);
		}
		else
		{
			pitem++;
		}
	}
	pnew->nflag |= QXML_ALLOC;
	
	return pnew;
}

QHDL qxmlDup(QHDL mem, QHDL mxml, QPCB dup_cb, QINT count, ...)
{
	QHDL hresult;
	QPNT *ppparams;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;
	
	va_start( marker, count );
	ppparams = (QPNT *)qprmmakev(mxml, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
	va_end( marker );
	
	hresult = qxmlDupx(mem, mxml, dup_cb, ppparams, count);
	
	qprmfree(ppparams);
	
	return hresult;
}
