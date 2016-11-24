
#include "qmdl_util.h"


QHDL qlinkMake(QINT flag, QHDL mem, QHDL data, QINT size)
{
	QHDL hitem;
	QINT nheader;

	nheader = sizeof(struct qmdl_link_item *);
	if(flag & QLINK_DBL)
	{
		nheader += sizeof(struct qmdl_link_item *);
	}
	if(flag & QLINK_MEM)
	{
		nheader += sizeof(QHDL);
	}
	hitem = (QHDL )qmalloc(mem, nheader+size);
	if(hitem == NULL)
	{
		return NULL;
	}
	memset(hitem, 0, nheader+size);
	if(flag & QLINK_MEM)
	{
		//*((QHDL *)((QCHR *)hitem+nheader-sizeof(QHDL))) = qmhost(mem);
		*((QHDL *)((QCHR *)hitem+nheader-sizeof(QHDL))) = NULL;
	}
	if(data != NULL)
	{
		memcpy((QCHR *)hitem+nheader, data, size);
	}

	return hitem;
}

QHDL qlinkTail(QINT flag, QHDL link)
{
	struct qmdl_link_item *pitem;

	if(link == NULL)
	{
		return NULL;
	}
	if(flag & QLINK_HDR)
	{
		return ((struct qmdl_link_data *)(link))->ptail;
	}
	if( (flag & QLINK_DBL) && (flag & QLINK_RNG) )
	{
		return ((struct qmdl_link_item *)(link))->pprev;
	}
	pitem = (struct qmdl_link_item *)link;
	while(pitem)
	{
		if(pitem->pnext == NULL)
		{
			break;
		}
		if((QHDL )(pitem->pnext) == link)
		{
			break;
		}
		pitem = pitem->pnext;
	}

	return (QHDL )pitem;
}

QPNT qlinkItem(QINT flag, QHDL data)
{
	QINT nheader;

	if(data == NULL)
	{
		return NULL;
	}
	nheader = sizeof(struct qmdl_link_item *);
	if(flag & QLINK_DBL)
	{
		nheader += sizeof(struct qmdl_link_item *);
	}
	if(flag & QLINK_MEM)
	{
		nheader += sizeof(QHDL);
	}

	return (QPNT )((QCHR *)data-nheader);
}

QHDL qlinkLink(QINT flag, QHDL item)
{
	struct qmdl_link_item *pitem;

	if( (flag & (QLINK_HDR|QLINK_RNG)) || !(flag & QLINK_DBL) )
	{
		return NULL;
	}
	pitem = (struct qmdl_link_item *)item;
	while(pitem)
	{
		if(pitem->pprev == NULL)
		{
			break;
		}
		pitem = pitem->pprev;
	}

	return (QHDL )pitem;
}

QHDL qlinkInsertNext(QINT flag, QHDL link, QHDL base, QHDL item)
{
	QHDL hlink;
	struct qmdl_link_data *pdata;
	struct qmdl_link_item *pbase, *pitem, *phead;

	hlink = link;
	pitem = (struct qmdl_link_item *)item;
	if(pitem == NULL)
	{
		return hlink;
	}
	if(hlink == NULL)
	{
		if(flag & QLINK_HDR)
		{
			// 带头信息。
			pdata = (struct qmdl_link_data *)qmalloc(link, sizeof(struct qmdl_link_data));
			if(pdata == NULL)
			{
				return hlink;
			}
			pdata->phead = pitem;
			pdata->ptail = pitem;
			hlink = (QHDL )pdata;
		}
		else
		{
			// 无头信息。
			pdata = NULL;
			hlink = (QHDL )pitem;
		}
		if(flag & QLINK_RNG)
		{
			pitem->pnext = pitem;
			if(flag & QLINK_DBL)
			{
				pitem->pprev = pitem;
			}
		}
		else
		{
			pitem->pnext = NULL;
			if(flag & QLINK_DBL)
			{
				pitem->pprev = NULL;
			}
		}
	}//if(link == NULL)
	else
	{
		// 找到插入位置基点。
		if(flag & QLINK_HDR)
		{
			pdata = (struct qmdl_link_data *)hlink;
			if(base != NULL)
			{
				pbase = (struct qmdl_link_item *)base;
			}
			else
			{
				// 默认为插入到尾巴。
				pbase = pdata->ptail;
			}
		}
		else
		{
			pdata = NULL;
			phead = (struct qmdl_link_item *)hlink;
			if(base != NULL)
			{
				pbase = (struct qmdl_link_item *)base;
			}
			else
			{
				// 默认为插入到尾巴。
				if( (flag & QLINK_RNG) && (flag & QLINK_DBL) )
				{
					pbase = phead->pprev;
				}
				else
				{
					pbase = phead;
					while(pbase)
					{
						if(pbase->pnext == NULL)
						{
							break;
						}
						if(pbase->pnext == phead)
						{
							break;
						}
						pbase = pbase->pnext;
					}
				}
			}
		}
		if(pbase == NULL)
		{
			if(flag & QLINK_HDR)
			{
				// 第一个插入结点。
				pdata->phead = pitem;
				pdata->ptail = pitem;
				if(flag & QLINK_RNG)
				{
					pitem->pnext = pitem;
					if(flag & QLINK_DBL)
					{
						pitem->pprev = pitem;
					}
				}
				else
				{
					pitem->pnext = NULL;
					if(flag & QLINK_DBL)
					{
						pitem->pprev = NULL;
					}
				}
			}
			else
			{
				// 出错。
				return hlink;
			}
		}//if(pbase == NULL)
		else
		{
			pitem->pnext = pbase->pnext;
			if(flag & QLINK_DBL)
			{
				pitem->pprev = pbase;
				if(pbase->pnext != NULL)
				{
					pbase->pnext->pprev = pitem;
				}
			}
			pbase->pnext = pitem;
			if(flag & QLINK_HDR)
			{
				if(pbase == pdata->ptail)
				{
					pdata->ptail = pitem;
				}
			}
		}//if(pbase != NULL)
	}//if(link != NULL)

	return hlink;
}

QHDL qlinkInsertPrev(QINT flag, QHDL link, QHDL base, QHDL item)
{
	QHDL hlink;
	struct qmdl_link_data *pdata;
	struct qmdl_link_item *pbase, *pitem, *phead;

	hlink = link;
	pitem = (struct qmdl_link_item *)item;
	if(pitem == NULL)
	{
		return hlink;
	}
	if(hlink == NULL)
	{
		if(flag & QLINK_HDR)
		{
			// 带头信息。
			pdata = (struct qmdl_link_data *)qmalloc(link, sizeof(struct qmdl_link_data));
			if(pdata == NULL)
			{
				return hlink;
			}
			pdata->phead = pitem;
			pdata->ptail = pitem;
			hlink = (QHDL )pdata;
		}
		else
		{
			// 无头信息。
			pdata = NULL;
			hlink = (QHDL )pitem;
		}
		if(flag & QLINK_RNG)
		{
			pitem->pnext = pitem;
			if(flag & QLINK_DBL)
			{
				pitem->pprev = pitem;
			}
		}
		else
		{
			pitem->pnext = NULL;
			if(flag & QLINK_DBL)
			{
				pitem->pprev = NULL;
			}
		}
	}//if(link == NULL)
	else
	{
		// 找到插入位置基点。
		if(flag & QLINK_HDR)
		{
			pdata = (struct qmdl_link_data *)hlink;
			phead = NULL;
			if(base != NULL)
			{
				pbase = (struct qmdl_link_item *)base;
			}
			else
			{
				// 默认为插入到尾巴。
				pbase = pdata->phead;
			}
		}
		else
		{
			pdata = NULL;
			phead = (struct qmdl_link_item *)hlink;
			if(base != NULL)
			{
				pbase = (struct qmdl_link_item *)base;
			}
			else
			{
				// 默认为插入到尾巴。
				pbase = phead;
			}
		}
		if(pbase == NULL)
		{
			if(flag & QLINK_HDR)
			{
				// 第一个插入结点。
				pdata->phead = pitem;
				pdata->ptail = pitem;
				if(flag & QLINK_RNG)
				{
					pitem->pnext = pitem;
					if(flag & QLINK_DBL)
					{
						pitem->pprev = pitem;
					}
				}
				else
				{
					pitem->pnext = NULL;
					if(flag & QLINK_DBL)
					{
						pitem->pprev = NULL;
					}
				}
			}
			else
			{
				// 出错。
				return hlink;
			}
		}//if(pbase == NULL)
		else
		{
			pitem->pnext = pbase;
			if(flag & QLINK_DBL)
			{
				pitem->pprev = pbase->pprev;
				if(pbase->pprev != NULL)
				{
					pbase->pprev->pnext = pitem;
				}
				pbase->pprev = pitem;
			}
			if(flag & QLINK_HDR)
			{
				if(pbase == pdata->phead)
				{
					pdata->phead = pitem;
				}
			}
			else
			{
				if(pbase == phead)
				{
					hlink = (QHDL )pitem;
				}
			}
		}//if(pbase != NULL)
	}//if(link != NULL)

	return hlink;
}

QHDL qlinkRemove(QINT flag, QHDL link, QHDL item)
{
	QHDL hlink;
	struct qmdl_link_data *pdata;
	struct qmdl_link_item *pbase, *pitem, *phead;

	hlink = link;
	pitem = (struct qmdl_link_item *)item;
	if(hlink == NULL || pitem == NULL)
	{
		return hlink;
	}
	// 定位到将要删除结点的前一结点，空表示删除头结点。
	if(flag & QLINK_HDR)
	{
		pdata = (struct qmdl_link_data *)hlink;
		phead = pdata->phead;
	}
	else
	{
		pdata = NULL;
		phead = (struct qmdl_link_item *)hlink;
	}
	if(flag & QLINK_DBL)
	{
		pbase = pitem->pprev;
	}
	else
	{
		pbase = phead;
		while(pbase)
		{
			if(pbase->pnext == pitem)
			{
				break;
			}
			pbase = pbase->pnext;
			if(pbase == phead)
			{
				pbase = NULL;
				break;
			}
		}
		if(pbase == NULL)
		{
			if( (flag & QLINK_RNG) || (pitem != phead) )
			{
				// 没有找到要删除的结点，出错。
				return hlink;
			}
		}
	}
	if(pbase == NULL)
	{
		if(flag & QLINK_HDR)
		{
			if(pitem == pdata->phead && pitem == pdata->ptail)
			{
				pdata->phead = NULL;
				pdata->ptail = NULL;
			}
			else
			{
				pdata->phead = pitem->pnext;
			}
		}
		else
		{
			if(pitem == pitem->pnext)
			{
				hlink = NULL;
			}
			else
			{
				hlink = (QHDL )pitem->pnext;
			}
		}
		if(flag & QLINK_DBL)
		{
			if(pitem != pitem->pnext)
			{
				if(pitem->pnext != NULL)
				{
					pitem->pnext->pprev = pitem->pprev;
				}
				if(pitem->pprev != NULL)
				{
					pitem->pprev->pnext = pitem->pnext;
				}
			}
		}
	}
	else
	{
		if(flag & QLINK_HDR)
		{
			if(pitem == pdata->phead)
			{
				pdata->phead = pitem->pnext;
			}
			if(pitem == pdata->ptail)
			{
				pdata->ptail = pbase;
			}
		}
		else
		{
			if(pitem == pitem->pnext)
			{
				hlink = NULL;
			}
			else if(pitem == phead)
			{
				hlink = (QHDL )pitem->pnext;
			}
		}
		if(pitem != pitem->pnext)
		{
			if(flag & QLINK_DBL)
			{
				if(pitem->pnext != NULL)
				{
					pitem->pnext->pprev = pbase;
				}
			}
			pbase->pnext = pitem->pnext;
		}
	}
	pitem->pnext = NULL;
	if(flag & QLINK_DBL)
	{
		pitem->pprev = NULL;
	}

	return hlink;
}

QHDL qlinkFreeLinkx(QINT flag, QHDL link, QPCB free_cb, QPNT params[], QINT count)
{
	struct qmdl_link_data *pdata;
	struct qmdl_link_item *pnext, *pitem, *phead;

	if(link == NULL)
	{
		return NULL;
	}
	if(flag & QLINK_HDR)
	{
		pdata = (struct qmdl_link_data *)link;
		phead = pdata->phead;
	}
	else
	{
		pdata = NULL;
		phead = (struct qmdl_link_item *)link;
	}
	pitem = phead;
	while(pitem)
	{
		pnext = pitem->pnext;
		if(free_cb != NULL)
		{
			free_cb((QHDL )pitem, NULL, QCD_CALL, params, count);
		}
		qmfree(pitem);
		pitem = pnext;
		if(pitem == phead)
		{
			break;
		}
	}
	if(flag & QLINK_HDR)
	{
		qmfree(pdata);
	}

	return NULL;
}

QHDL qlinkFreeLink(QINT flag, QHDL link, QPCB free_cb, QINT count, ...)
{
	struct qmdl_link_data *pdata;
	struct qmdl_link_item *pnext, *pitem, *phead;
	QPNT *ppprms;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;

	if(link == NULL)
	{
		return NULL;
	}
	if(flag & QLINK_HDR)
	{
		pdata = (struct qmdl_link_data *)link;
		phead = pdata->phead;
	}
	else
	{
		pdata = NULL;
		phead = (struct qmdl_link_item *)link;
	}

	if(count > 0)
	{
		va_start(marker, count);
		ppprms = (QPNT *)qprmmakev(link, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
		va_end(marker);
	}
	else
	{
		ppprms = (QPNT *)vbuff;
	}

	pitem = phead;
	while(pitem)
	{
		pnext = pitem->pnext;
		if(free_cb != NULL)
		{
			free_cb((QHDL )pitem, NULL, QCD_CALL, ppprms, count);
		}
		qmfree(pitem);
		pitem = pnext;
		if(pitem == phead)
		{
			break;
		}
	}

	if(count > 0)
	{
		qprmfree(ppprms);
	}

	if(flag & QLINK_HDR)
	{
		qmfree(pdata);
	}

	return NULL;
}

QINT qlinkFreeItemx(QINT flag, QHDL item, QPCB free_cb, QPNT params[], QINT count)
{
	if(free_cb != NULL)
	{
		free_cb((QHDL )item, NULL, QCD_CALL, params, count);
	}
	qmfree(item);

	return 0;
}

QHDL qlinkFreeItem(QINT flag, QHDL item, QPCB free_cb, QINT count, ...)
{
	QPNT *ppprms;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;

	va_start(marker, count);
	ppprms = (QPNT *)qprmmakev(item, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
	va_end(marker);

	if(free_cb != NULL)
	{
		free_cb((QHDL )item, NULL, QCD_CALL, ppprms, count);
	}
	qmfree(item);

	qprmfree(ppprms);

	return 0;
}

QHDL qlinkClearLinkx(QINT flag, QHDL link, QPCB free_cb, QPNT params[], QINT count)
{
	struct qmdl_link_data *pdata;
	struct qmdl_link_item *pnext, *pitem, *phead;

	if(link == NULL)
	{
		return NULL;
	}
	if(flag & QLINK_HDR)
	{
		pdata = (struct qmdl_link_data *)link;
		phead = pdata->phead;
	}
	else
	{
		pdata = NULL;
		phead = (struct qmdl_link_item *)link;
	}
	pitem = phead;
	while(pitem)
	{
		pnext = pitem->pnext;
		if(free_cb != NULL)
		{
			free_cb((QHDL )pitem, NULL, QCD_CALL, params, count);
		}
		qmfree(pitem);
		pitem = pnext;
		if(pitem == phead)
		{
			break;
		}
	}
	if( !(flag & QLINK_HDR) )
	{
		pdata = NULL;
	}

	return (QHDL )pdata;
}

QHDL qlinkClearLink(QINT flag, QHDL link, QPCB free_cb, QINT count, ...)
{
	QHDL hresult;
	QPNT *ppprms;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;

	va_start(marker, count);
	ppprms = (QPNT *)qprmmakev(link, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
	va_end(marker);

	hresult = qlinkClearLinkx(flag, link, free_cb, ppprms, count);

	qprmfree(ppprms);

	return hresult;
}

QINT qlinkClearItemx(QINT flag, QHDL item, QPCB free_cb, QPNT params[], QINT count)
{
	if(free_cb != NULL)
	{
		free_cb((QHDL )item, NULL, QCD_CALL, params, count);
	}

	return 0;
}

QINT qlinkClearItem(QINT flag, QHDL item, QPCB free_cb, QINT count, ...)
{
	QPNT *ppprms;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;

	va_start(marker, count);
	ppprms = (QPNT *)qprmmakev(item, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
	va_end(marker);

	if(free_cb != NULL)
	{
		free_cb((QHDL )item, NULL, QCD_CALL, ppprms, count);
	}

	qprmfree(ppprms);

	return 0;
}

QINT qlinkScanx(QINT flag, QHDL link, QPCB scan_cb, QPNT params[], QINT count)
{
	QINT nresult, ncount;
	struct qmdl_link_data *pdata;
	struct qmdl_link_item *pnext, *pitem, *phead;
	
	ncount = 0;
	if(link == NULL)
	{
		return ncount;
	}
	if(flag & QLINK_HDR)
	{
		pdata = (struct qmdl_link_data *)link;
		phead = pdata->phead;
	}
	else
	{
		pdata = NULL;
		phead = (struct qmdl_link_item *)link;
	}
	pitem = phead;
	while(pitem)
	{
		pnext = pitem->pnext;
		if(scan_cb != NULL)
		{
			nresult = scan_cb((QHDL)pitem, NULL, QCD_CALL, params, count);
			if(nresult != QSCN_OK)
			{
				break;
			}
		}
		ncount++;
		pitem = pnext;
		if(pitem == phead)
		{
			break;
		}
	}
	
	return ncount;
	
}

QINT qlinkScan(QINT flag, QHDL link, QPCB scan_cb, QINT count, ...)
{
	QINT nresult;
	QPNT *ppprms;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;
	
	va_start(marker, count);
	ppprms = (QPNT *)qprmmakev(link, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
	va_end(marker);
	
	nresult = qlinkScanx(flag, link, scan_cb, ppprms, count);
	
	qprmfree(ppprms);
	
	return 0;
}
