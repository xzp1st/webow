
#include "qmdl_util.h"


// [free_cb] + [define] + <check> + <count(0xFFFF0000)|flag(0x0000FFF0)|licence(0x0000000F)> + <prmlist>


static QHDL qprm_find_and_make_path(QHDL mem, QINT make, QHDL list, QPNT path, QINT pathlen, QINT *index, QHDL *define)
{
	QHDL hlist;
	QPNT pvalue;
	QXML pparent, pchild;
	QINT nlen, npos, ncount, nindex, nvalue;
	QSTR ppath, pname, pnext, pchildname;
	
	hlist = qmcheck(list, QLCN_HPRM);
	if(hlist == NULL)
	{
		goto FIND_MAKE_FAILED;
	}
	pparent = (QXML)qprmdfn(hlist);
	pparent = (QXML)qmcheck(pparent, QLCN_HXML);
	pchild = NULL;
	if(path == NULL)
	{
		// find by index.
		if(index == NULL)
		{
			goto FIND_MAKE_FAILED;
		}
		nindex = *index;
		if(nindex < 0 || nindex >= qprmcnt(hlist))
		{
			goto FIND_MAKE_FAILED;
		}
		nvalue = nindex;
		if(define != NULL)
		{
			if(pparent != NULL)
			{
				pchild = (QXML)qxmlGetChild(pparent, QXML_PARAM, nindex);
			}
		}
		goto FIND_MAKE_OK;
	}
	if(pparent == NULL)
	{
		goto FIND_MAKE_FAILED;
	}
	pvalue = NULL;
	nvalue = -1;
	nindex = 0;
	npos = 0;
	ppath = (QSTR)path;
	while(1)
	{
		if(pathlen > 0)
		{
			if(npos >= pathlen)
			{
				break;
			}
		}
		pname = ppath;
		while(*pname != 0)
		{
			if(*pname != '/' && *pname != '\\')
			{
				break;
			}
			pname++;
			npos++;
		}
		nlen = 0;
		pnext = pname;
		while(*pnext != 0)
		{
			if(*pnext == '?')
			{
				break;
			}
			if(*pnext == '/' || *pnext == '\\')
			{
				break;
			}
			pnext++;
			nlen++;
			npos++;
		}
		if(nlen <= 0)
		{
			break;
		}
		nindex = 0;
		pchild = (QXML)qxmlGetChild(pparent, QXML_PARAM, 0);
		while(pchild)
		{
			pchildname = (QSTR)qxmlGetId(pchild);
			if(pchildname != NULL && qstrcmp(QSTR_ICS, pchildname, pname, nlen))
			{
				break;
			}
			pchild = (QXML)qxmlGetSibling(pchild, QXML_PARAM, 1);
			nindex++;
		}
		if(pchild == NULL)
		{
			break;
		}
		if(nvalue >= 0)
		{
			if(qmcheck(pvalue, QLCN_HPRM) == NULL)
			{
				if(make == 0)
				{
					// just finding, but failed.
					pchild = NULL;
					break;
				}
				ncount = qxmlCntChild(pparent, QXML_PARAM);
				pvalue = qprmmakex(mem, NULL, 0, pparent, qncb, NULL, &ncount);
				if(pvalue == NULL)
				{
					// make new list, but failed.
					pchild = NULL;
					break;
				}
				((QPNT *)hlist)[nvalue] = pvalue;
			}
			hlist = (QHDL)pvalue;
		}
		// update to current location.
		pvalue = (QPNT)((QPNT *)hlist)[nindex];
		nvalue = nindex;
		pparent = pchild;
		ppath = pnext;
	}
	if(pchild == NULL)
	{
		goto FIND_MAKE_FAILED;
	}
	goto FIND_MAKE_OK;
	
FIND_MAKE_OK:
	if(define != NULL)
	{
		*define = pchild;
	}
	if(index != NULL)
	{
		*index = nvalue;
	}
	
	return (QHDL)hlist;
	
FIND_MAKE_FAILED:
	if(define != NULL)
	{
		*define = NULL;
	}
	if(index != NULL)
	{
		*index = -1;
	}
	
	return NULL;
}

QHDL qprmmakev(QHDL mem, QPNT buff, QINT size, QHDL define, QPCB free_cb, va_list marker, QINT *count)
{
	QPNT psrcval, *ppdstval;
	QUCHR *pbuff, *pnew;
	struct qmdl_header *pheader;
	QINT nlen, nsize, nflag, ncount, ndefcnt, nprmcnt;

	if(count == NULL)
	{
		nprmcnt = -1;
	}
	else
	{
		nprmcnt = *count;
	}
	ndefcnt = 0;
	if(qmcheck(define, QLCN_HXML))
	{
		ndefcnt = qxmlCntChild(define, QXML_PARAM);
	}
	ncount = (nprmcnt > ndefcnt) ? nprmcnt : ndefcnt;
	if(ncount >= 0)
	{
		// œ‘ Ω∏¯≥ˆ≤Œ ˝∏ˆ ˝°£
		nsize = sizeof(struct qmdl_header)+sizeof(QPNT)*ncount;
		if(define != NULL)
		{
			nsize += sizeof(QHDL);
		}
		if(free_cb != NULL)
		{
			nsize += sizeof(QPCB);
		}
	}
	else
	{
		// nprmcnt–°”⁄¡„£¨±Ì æ“‘qnullŒ™≤Œ ˝±ÌµƒΩ· ¯°£
		if(size < sizeof(QPCB)+sizeof(QHDL)+sizeof(struct qmdl_header)+sizeof(QPNT))
		{
			nsize = QPRM_BUFF_SIZE;
		}
		else
		{
			nsize = size;
		}
	}
	if(buff != NULL && nsize <= size)
	{
		nsize = size;
		pbuff = (QUCHR *)buff;
	}
	else
	{
		if(nprmcnt == 0)
		{
			return NULL;
		}
		pbuff = (QUCHR *)qmalloc(mem, nsize);
		if(pbuff == NULL)
		{
			return NULL;
		}
	}
	nlen = sizeof(struct qmdl_header);
	if(define != NULL)
	{
		nlen += sizeof(QHDL);
	}
	if(free_cb != NULL)
	{
		nlen += sizeof(QPCB);
	}
	ppdstval = (QPNT *)(pbuff+nlen);
	ncount = 0;
	while(1)
	{
		if(nprmcnt >= 0)
		{
			// œ‘ Ω∏¯≥ˆ≤Œ ˝∏ˆ ˝°£
			if(ncount >= nprmcnt)
			{
				while(ncount < ndefcnt)
				{
					*ppdstval++ = NULL;
					ncount++;
				}
				break;
			}
			*ppdstval++ = va_arg(marker, QPNT);
		}
		else
		{
			// nprmcnt–°”⁄¡„£¨±Ì æ“‘qnullŒ™≤Œ ˝±ÌµƒΩ· ¯°£
			psrcval = va_arg(marker, QPNT);
			if(psrcval == qnull)
			{
				break;
			}
			if((QINT )(nlen+sizeof(QPNT)) > nsize)
			{
				pnew = (QUCHR *)qmalloc(mem, nsize+QPRM_BUFF_SIZE);
				if(pnew == NULL)
				{
					break;
				}
				memcpy(pnew, pbuff, nsize);
				if(pbuff != buff)
				{
					qmfree(pbuff);
				}
				pbuff = pnew;
				nsize += QPRM_BUFF_SIZE;
				ppdstval = (QPNT *)(pbuff+nlen);
			}
			*ppdstval++ = psrcval;
		}
		nlen += sizeof(QPNT);
		ncount++;
	}
	nflag = (ncount<<16)|QLCN_HPRM;
	if(pbuff != buff)
	{
		nflag |= QPRM_ALLOC;
	}
	if(define != NULL)
	{
		nflag |= QPRM_DEFINE;
		if(define == qnull)
		{
			*((QHDL *)pbuff) = NULL;
		}
		else
		{
			*((QHDL *)pbuff) = define;
		}
		pbuff += sizeof(QHDL);
	}
	if(free_cb != NULL)
	{
		nflag |= QPRM_FREECB;
		if(free_cb == qncb || (QPNT)free_cb == qnull)
		{
			*((QPCB *)pbuff) = NULL;
		}
		else
		{
			*((QPCB *)pbuff) = free_cb;
		}
		pbuff += sizeof(QPCB);
	}
	pheader = (struct qmdl_header *)pbuff;
	pheader->hcheck = (QHDL)(pbuff+sizeof(struct qmdl_header));
	pheader->nflag = nflag;
	pbuff += sizeof(struct qmdl_header);
	if(count != NULL)
	{
		*count = ncount;
	}

	return (QHDL )pbuff;
}

QHDL qprmmake(QHDL mem, QPNT buff, QINT size, QHDL define, QPCB free_cb, QINT *count, ...)
{
	QHDL hlist;
	va_list marker;

	va_start(marker, count);
	hlist = qprmmakev(mem, buff, size, define, free_cb, marker, count);
	va_end(marker);

	return hlist;
}

QHDL qprmmakex(QHDL mem, QPNT buff, QINT size, QHDL define, QPCB free_cb, QPNT params[], QINT *count)
{
	QHDL hdefine;
	QPCB pcbfree;
	struct qmdl_header *pheader;
	QXML psrcxml, pdstxml, pchildxml;
	QPNT psrcval, *ppsrcval, *ppdstval;
	QSTR pbuff, pnew, psrcname, pdstname;
	QINT nlen, nsize, nflag, nindex, ncount, ndefcnt, nprmcnt;

	pcbfree = free_cb;
	if(count != NULL)
	{
		nprmcnt = *count;
	}
	else
	{
		nprmcnt = qprmcnt(params);
	}
	hdefine = NULL;
	ndefcnt = 0;
	if(qmcheck(define, QLCN_HXML))
	{
		hdefine = define;
		ndefcnt = qxmlCntChild(define, QXML_PARAM);
	}
	else
	{
		if(qmcheck(params, QLCN_HXML))
		{
			hdefine = params;
			ndefcnt = qxmlCntChild(params, QXML_PARAM);
		}
		else if(qmcheck(params, QLCN_HPRM))
		{
			hdefine = qprmdfn(params);
		}
	}
	ncount = (nprmcnt > ndefcnt) ? nprmcnt : ndefcnt;
	if(ncount >= 0)
	{
		// œ‘ Ω∏¯≥ˆ≤Œ ˝∏ˆ ˝°£
		nsize = sizeof(struct qmdl_header)+sizeof(QPNT)*ncount;
		if(define != NULL)
		{
			nsize += sizeof(QHDL);
		}
		if(free_cb != NULL)
		{
			nsize += sizeof(QPCB);
		}
	}
	else
	{
		// nprmcnt–°”⁄¡„£¨±Ì æ“‘qnullŒ™≤Œ ˝±ÌµƒΩ· ¯°£
		if(size < sizeof(QPCB)+sizeof(QHDL)+sizeof(struct qmdl_header)+sizeof(QPNT))
		{
			nsize = QPRM_BUFF_SIZE;
		}
		else
		{
			nsize = size;
		}
	}
	if(buff != NULL && nsize <= size)
	{
		nsize = size;
		pbuff = (QUCHR *)buff;
	}
	else
	{
		if(nprmcnt == 0)
		{
			return NULL;
		}
		pbuff = (QUCHR *)qmalloc(mem, nsize);
		if(pbuff == NULL)
		{
			return NULL;
		}
	}
	nlen = sizeof(struct qmdl_header);
	if(define != NULL)
	{
		nlen += sizeof(QHDL);
	}
	if(free_cb != NULL)
	{
		nlen += sizeof(QPCB);
	}
	ncount = 0;
	ppdstval = (QPNT *)(pbuff+nlen);
	if(qmcheck(params, QLCN_HXML))
	{
		goto MAKE_HXML;
	}
	else if(qmcheck(params, QLCN_HPRM))
	{
		goto MAKE_HPRM;
	}
	else
	{
		goto MAKE_COPY;
	}
	goto MAKE_OK;
	
MAKE_HXML:
	pcbfree = qxmlGetValcb(params);
	if(pcbfree == NULL)
	{
		pcbfree = free_cb;
	}
	if(define == NULL)
	{
		psrcxml = (QXML)qxmlGetChild(params, QXML_PARAM, 0);
		while(psrcxml)
		{
			if(ncount >= ndefcnt)
			{
				break;
			}
			pchildxml = (QXML)qxmlGetChild(psrcxml, QXML_PARAM, 0);
			if(pchildxml == NULL)
			{
				*ppdstval++ = qxmlGetValp(psrcxml);
			}
			else
			{
				*ppdstval++ = qprmmakex(mem, NULL, 0, (QHDL)qnull, NULL, (QPNT*)psrcxml, NULL);
			}
			psrcxml = (QXML)qxmlGetSibling(psrcxml, QXML_PARAM, 1);
			ncount++;
		}
	}
	else
	{
		if(pcbfree == NULL)
		{
			pcbfree = qxmlGetValcb(define);
		}
		pdstxml = (QXML)qxmlGetChild(define, QXML_PARAM, 0);
		while(pdstxml)
		{
			if(ncount >= ndefcnt)
			{
				break;
			}
			pdstname = (QSTR)qxmlGetId(pdstxml);
			if(pdstname == NULL)
			{
				*ppdstval++ = NULL;
			}
			else
			{
				nindex = 0;
				psrcxml = (QXML)qxmlGetChild(params, QXML_PARAM, 0);
				while(psrcxml)
				{
					if(nindex >= qprmcnt(params))
					{
						break;
					}
					psrcname = (QSTR)qxmlGetId(psrcxml);
					if(psrcname != NULL && qstrcmp(QSTR_NONE, psrcname, pdstname, 0))
					{
						break;
					}
					psrcxml = (QXML)qxmlGetSibling(psrcxml, QXML_PARAM, 1);
					nindex++;
				}
				if(psrcxml == NULL)
				{
					*ppdstval++ = NULL;
				}
				else
				{
					pchildxml = (QXML)qxmlGetChild(psrcxml, QXML_PARAM, 0);
					if(pchildxml == NULL)
					{
						*ppdstval++ = qxmlGetValp(psrcxml);
					}
					else
					{
						*ppdstval++ = qprmmakex(mem, NULL, 0, pdstxml, NULL, (QPNT*)psrcxml, NULL);
					}
				}
			}
			pdstxml = (QXML)qxmlGetSibling(pdstxml, QXML_PARAM, 1);
			ncount++;
		}
	}
	goto MAKE_OK;
	
MAKE_HPRM:
	if(define == NULL || hdefine == NULL)
	{
		goto MAKE_COPY;
	}
	pcbfree = qprmGetFreeCb(params);
	if(pcbfree == NULL)
	{
		pcbfree = free_cb;
		if(pcbfree == NULL)
		{
			pcbfree = qxmlGetValcb(define);
		}
	}
	pdstxml = (QXML)qxmlGetChild(define, QXML_PARAM, 0);
	while(pdstxml)
	{
		if(ncount >= ndefcnt)
		{
			break;
		}
		pdstname = (QSTR)qxmlGetId(pdstxml);
		if(pdstname == NULL)
		{
			*ppdstval++ = NULL;
		}
		else
		{
			nindex = 0;
			ppsrcval = params;
			psrcxml = (QXML)qxmlGetChild(hdefine, QXML_PARAM, 0);
			while(psrcxml)
			{
				if(nindex >= qprmcnt(params))
				{
					break;
				}
				psrcname = (QSTR)qxmlGetId(psrcxml);
				if(psrcname != NULL && qstrcmp(QSTR_NONE, psrcname, pdstname, 0))
				{
					break;
				}
				psrcxml = (QXML)qxmlGetSibling(psrcxml, QXML_PARAM, 1);
				ppsrcval++;
				nindex++;
			}
			if(psrcxml == NULL)
			{
				*ppdstval++ = NULL;
			}
			else
			{
				*ppdstval++ = *ppsrcval;
			}
		}
		pdstxml = (QXML)qxmlGetSibling(pdstxml, QXML_PARAM, 1);
		ncount++;
	}
	goto MAKE_OK;
	
MAKE_COPY:
	pcbfree = qprmGetFreeCb(params);
	if(pcbfree == NULL)
	{
		pcbfree = free_cb;
		if(pcbfree == NULL)
		{
			if(hdefine != NULL)
			{
				pcbfree = qxmlGetValcb(hdefine);
			}
		}
	}
	ppsrcval = params;
	while(1)
	{
		if(nprmcnt >= 0)
		{
			// œ‘ Ω∏¯≥ˆ≤Œ ˝∏ˆ ˝°£
			if(ncount >= nprmcnt)
			{
				while(ncount < ndefcnt)
				{
					*ppdstval++ = NULL;
					ncount++;
				}
				break;
			}
			if(ppsrcval == NULL)
			{
				*ppdstval++ = NULL;
			}
			else
			{
				*ppdstval++ = *ppsrcval++;
			}
		}
		else
		{
			// nprmcnt–°”⁄¡„£¨±Ì æ“‘qnullŒ™≤Œ ˝±ÌµƒΩ· ¯°£
			if(ppsrcval == NULL)
			{
				psrcval = NULL;
			}
			else
			{
				psrcval = *ppsrcval++;
				if(psrcval == qnull)
				{
					break;
				}
			}
			if((QINT )(nlen+sizeof(QPNT)) > nsize)
			{
				if(params == NULL)
				{
					break;
				}
				pnew = (QUCHR *)qmalloc(mem, nsize+QPRM_BUFF_SIZE);
				if(pnew == NULL)
				{
					break;
				}
				memcpy(pnew, pbuff, nsize);
				if(pbuff != buff)
				{
					qmfree(pbuff);
				}
				pbuff = pnew;
				nsize += QPRM_BUFF_SIZE;
				ppdstval = (QPNT *)(pbuff+nlen);
			}
			*ppdstval++ = psrcval;
		}
		nlen += sizeof(QPNT);
		ncount++;
	}
	goto MAKE_OK;
	
MAKE_OK:
	nflag = (ncount<<16)|QLCN_HPRM;
	if(pbuff != buff)
	{
		nflag |= QPRM_ALLOC;
	}
	if(define != NULL)
	{
		nflag |= QPRM_DEFINE;
		if(hdefine == qnull)
		{
			*((QHDL *)pbuff) = NULL;
		}
		else
		{
			*((QHDL *)pbuff) = hdefine;
		}
		pbuff += sizeof(QHDL);
	}
	if(free_cb != NULL)
	{
		nflag |= QPRM_FREECB;
		if(pcbfree == qncb || (QPNT)pcbfree == qnull)
		{
			*((QPCB *)pbuff) = NULL;
		}
		else
		{
			*((QPCB *)pbuff) = pcbfree;
		}
		pbuff += sizeof(QPCB);
	}
	pheader = (struct qmdl_header *)pbuff;
	pheader->hcheck = (QHDL)(pbuff+sizeof(struct qmdl_header));
	pheader->nflag = nflag;
	pbuff += sizeof(struct qmdl_header);
	if(count != NULL)
	{
		*count = ncount;
	}

	return (QHDL)pbuff;
}

QPNT *qprmlist(QHDL list, QINT *cnt)
{
	struct qmdl_header *pheader;
	
	pheader = (struct qmdl_header *)qmheader(list, QLCN_HPRM);
	if(pheader == NULL)
	{
		if(cnt != NULL)
		{
			*cnt = 0;
		}

		return (QPNT *)qnull;
	}
	if(cnt != NULL)
	{
		*cnt = (pheader->nflag>>16);
	}

	return (QPNT *)list;
}

QINT qprmflag(QHDL list)
{
	struct qmdl_header *pheader;
	
	pheader = (struct qmdl_header *)qmheader(list, QLCN_HPRM);
	if(pheader == NULL)
	{
		return QNO_FAIL;
	}
	
	return (pheader->nflag&QPRM_MASK_FLAG);
}

QINT qprmSetFlag(QHDL list, QINT flag)
{
	QINT nflag;
	struct qmdl_header *pheader;
	
	pheader = (struct qmdl_header *)qmheader(list, QLCN_HPRM);
	if(pheader == NULL)
	{
		return QNO_FAIL;
	}
	nflag = pheader->nflag;
	pheader->nflag &= ~QPRM_MASK_FLAG;
	pheader->nflag |= (flag&QPRM_MASK_FLAG);
	
	return nflag;
}

QINT qprmGetFlag(QHDL list)
{
	struct qmdl_header *pheader;
	
	pheader = (struct qmdl_header *)qmheader(list, QLCN_HPRM);
	if(pheader == NULL)
	{
		return QNO_FAIL;
	}
	
	return (pheader->nflag&QPRM_MASK_FLAG);
}

QPCB qprmSetFreeCb(QHDL list, QPCB free_cb)
{
	QPCB pcbold;
	QUCHR *pbuff;
	struct qmdl_header *pheader;

	pheader = (struct qmdl_header *)qmheader(list, QLCN_HPRM);
	if(pheader == NULL)
	{
		return NULL;
	}
	pcbold = NULL;
	pbuff = (QUCHR *)pheader;
	if(pheader->nflag & QPRM_DEFINE)
	{
		pbuff -= sizeof(QHDL);
	}
	if(pheader->nflag & QPRM_FREECB)
	{
		pbuff -= sizeof(QPCB);
		pcbold = *((QPCB *)pbuff);
		if(free_cb == qncb || (QPNT)free_cb == qnull)
		{
			*((QPCB *)pbuff) = NULL;
		}
		else
		{
			*((QPCB *)pbuff) = free_cb;
		}
	}

	return pcbold;
}

QPCB qprmGetFreeCb(QHDL list)
{
	QUCHR *pbuff;
	struct qmdl_header *pheader;
	
	pheader = (struct qmdl_header *)qmheader(list, QLCN_HPRM);
	if(pheader == NULL)
	{
		return NULL;
	}
	pbuff = (QUCHR *)pheader;
	if(pheader->nflag & QPRM_DEFINE)
	{
		pbuff -= sizeof(QHDL);
	}
	if(pheader->nflag & QPRM_FREECB)
	{
		pbuff -= sizeof(QPCB);
		return *((QPCB *)pbuff);
	}
	
	return NULL;
}

QHDL qprmSetDefine(QHDL list, QHDL define)
{
	QHDL hdefine;
	QUCHR *pbuff;
	struct qmdl_header *pheader;
	
	pheader = (struct qmdl_header *)qmheader(list, QLCN_HPRM);
	if(pheader == NULL)
	{
		return NULL;
	}
	hdefine = NULL;
	pbuff = (QUCHR *)pheader;
	if(pheader->nflag & QPRM_DEFINE)
	{
		pbuff -= sizeof(QHDL);
		hdefine = *((QHDL *)pbuff);
		if(define == qnull)
		{
			*((QHDL *)pbuff) = NULL;
		}
		else
		{
			*((QHDL *)pbuff) = define;
		}
	}
	
	return hdefine;
}

QHDL qprmGetDefine(QHDL list)
{
	QUCHR *pbuff;
	struct qmdl_header *pheader;
	
	pheader = (struct qmdl_header *)qmheader(list, QLCN_HPRM);
	if(pheader == NULL)
	{
		return NULL;
	}
	pbuff = (QUCHR *)pheader;
	if(pheader->nflag & QPRM_DEFINE)
	{
		pbuff -= sizeof(QHDL);
		return *((QHDL *)pbuff);
	}
	
	return NULL;
}

QHDL qprmSetValue(QHDL list, QPNT name, QINT *index, QHDL *define, QPNT value)
{
	QHDL hlist;
	QINT nindex;
	
	nindex = 0;
	hlist = qprm_find_and_make_path(qmhost(list), 1, list, name, 0, &nindex, define);
	if(hlist == NULL)
	{
		return NULL;
	}
	if(index != NULL)
	{
		// return the location of value.
		*index = nindex;
	}
	((QPNT *)hlist)[nindex] = value;
	
	return hlist;
}

QHDL qprmGetValue(QHDL list, QPNT name, QINT *index, QHDL *define, QPNT *value)
{
	QHDL hlist;
	QINT nindex;
	
	nindex = 0;
	hlist = qprm_find_and_make_path(qmhost(list), 0, list, name, 0, &nindex, define);
	if(hlist == NULL)
	{
		return NULL;
	}
	if(index != NULL)
	{
		*index = nindex;
	}
	if(value != NULL)
	{
		*value = ((QPNT *)hlist)[nindex];
	}
	
	return hlist;
}

QINT qprmfree(QHDL list)
{
	QSTR pbuff;
	QPNT *pplist;
	QPCB pcbfree;
	QINT nflag, ncount, nindex;
	QXML pdefine, pnode, pchild;
	struct qmdl_header *pheader;

	pheader = (struct qmdl_header *)qmheader(list, QLCN_HPRM);
	if(pheader == NULL)
	{
		return 0;
	}
	ncount = (pheader->nflag>>16);
	pdefine = (QXML)qprmdfn(list);
	if(pdefine != NULL)
	{
		nindex = 0;
		pplist = (QPNT*)list;
		pnode = (QXML)qxmlGetChild(pdefine, QXML_PARAM, 0);
		while(pnode)
		{
			if(nindex >= ncount)
			{
				break;
			}
			pchild = (QXML)qxmlGetChild(pnode, QXML_PARAM, 0);
			if(pchild == NULL)
			{
				pcbfree = qxmlGetValcb(pnode);
				if(pcbfree != NULL && pcbfree != qncb && (QPNT)pcbfree != qnull)
				{
					pcbfree(*pplist, NULL, QCD_CALL, (QPNT*)list, ncount);
				}
			}
			else
			{
				if(qmcheck(*pplist, QLCN_HPRM))
				{
					qmfree(*pplist);
				}
			}
			pnode = (QXML)qxmlGetSibling(pnode, QXML_PARAM, 1);
			pplist++;
			nindex++;
		}
	}
	nflag = pheader->nflag;
	pbuff = (QUCHR *)pheader;
	if(nflag & QPRM_DEFINE)
	{
		pbuff -= sizeof(QHDL);
	}
	if(nflag & QPRM_FREECB)
	{
		pbuff -= sizeof(QPCB);
		pcbfree = *((QPCB *)pbuff);
		if(pcbfree != NULL)
		{
			pcbfree((QHDL )pbuff, NULL, QCD_CALL, (QPNT *)list, ncount);
		}
	}
	pheader->hcheck = NULL;
	pheader->nflag = 0;
	if(nflag & QPRM_ALLOC)
	{
		qmfree(pbuff);
	}

	return ncount;
}

QPNT qprmconvert(QHDL dsttype, QPNT buff, QINT *size, QHDL srctype, QPNT srcval, QINT srcsize)
{
	QPNT pval;
	QINT nlen, nsize;
	
	if(size == NULL)
	{
		nsize = 0;
	}
	else
	{
		nsize = *size;
	}
	pval = srcval;
	nlen = srcsize;
	if(srctype == qbool)
	{
		if(dsttype == qstr)
		{
			nlen = qstrmake(buff, nsize, (QPNT)"%s", srcval?"TRUE":"FALSE");
			pval = buff;
		}
	}
	else if(srctype == qint)
	{
		if(dsttype == qstr)
		{
			nlen = qstrmake(buff, nsize, (QPNT)"%d", srcval);
			pval = buff;
		}
	}
	else if(srctype == qpnt)
	{
		//
	}
	else if(srctype == qpcb)
	{
		//
	}
	else if(srctype == qstr || srctype == NULL)
	{
		if(dsttype == qint)
		{
			pval = (QPNT)qstrint(-1, srcval, &nlen);
			nlen = sizeof(QINT);
		}
		else if(dsttype == qbool)
		{
			if(qstrcmp(QSTR_ICS, srcval, (QPNT)"true", 0))
			{
				pval = (QPNT)1;
			}
			else if(qstrcmp(QSTR_ICS, srcval, (QPNT)"false", 0))
			{
				pval = (QPNT)0;
			}
			else
			{
				pval = (QPNT)qstrint(-1, srcval, NULL);
			}
			nlen = sizeof(QINT);
		}
		else if(qmcheck(dsttype, QLCN_HENM))
		{
			pval = (QPNT)qstr2enum((QENUM)dsttype, 1, srcval, &nlen);
			nlen = sizeof(QINT);
		}
	}
	if(size != NULL)
	{
		*size = nlen;
	}
	
	return pval;
}
