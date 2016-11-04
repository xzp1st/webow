
#include "qmdl_util.h"


#define qstr_hex2val(val)\
(\
 ((val) >= '0' && (val) <= '9') ? ((val)-'0') :\
 (\
  ((val) >= 'a' && (val) <= 'f') ? ((val)-'a'+10) :\
  (\
   ((val) >= 'A' && (val) <= 'F') ? ((val)-'A'+10) :\
   (-1)\
   )\
  )\
 )

QINT qstritem(QPNT fmt, qstr_item_t *item)
{
	QINT nchar, nmatch;
	QUCHR *pchar, chbegin, chend;
	
	if(fmt == NULL || item == NULL)
	{
		return QNO_FAIL;
	}
	item->nflag = 0;
	item->nparam = 0;
	item->nfillchar = 0;
	item->nfillsize = 0;
	item->pname = NULL;
	item->nnamesize = 0;
	item->pfmt = (QUCHR *)fmt;
	item->nfmtsize = 0;
	
	nchar = 0;
	pchar = (QUCHR *)item->pfmt;
	while(pchar)
	{
		nchar = *pchar;
		if(nchar == 0)
		{
			break;
		}
		if(nchar == '%')
		{
			break;
		}
		item->nfmtsize++;
		pchar++;
	}
	if(item->nfmtsize > 0)
	{
		return item->nfmtsize;
	}
	if(nchar == 0)
	{
		return item->nfmtsize;
	}
	item->nfmtsize++;
	// ÃÓ≥‰±Í÷æ°£
	nchar = *(++pchar);
	switch(nchar)
	{
		case '-':
			item->nfillflag = -1;
			item->nfmtsize++;
			pchar++;
			break;
			
		case '+':
			item->nfillflag = 1;
			item->nfmtsize++;
			pchar++;
			break;
			
		default:
			item->nfillflag = 0;
			break;
	}
	// ÃÓ≥‰◊÷∑˚°£
	nchar = *pchar;
	switch(nchar)
	{
		case '0':
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			item->nfillchar = nchar;
			item->nfmtsize++;
			pchar++;
			break;
			
		case '\\':
			nchar = *(++pchar);
			item->nfmtsize++;
			nchar = qstr_hex2val(nchar);
			if(nchar < 0)
			{
				item->nfillchar = '\\';
			}
			else
			{
				item->nfillchar = nchar;
				nchar = *(++pchar);
				item->nfmtsize++;
				nchar = qstr_hex2val(nchar);
				if(nchar >= 0)
				{
					pchar++;
					item->nfmtsize++;
					item->nfillchar <<= 4;
					item->nfillchar |= nchar;
				}
			}
			break;
	}
	while(pchar)
	{
		nchar = *pchar;
		if(nchar < '0' || nchar > '9')
		{
			break;
		}
		item->nfillsize = item->nfillsize*10+(nchar-'0');
		item->nfmtsize++;
		pchar++;
	}
	if(nchar == 0)
	{
		return item->nfmtsize;
	}
	item->nfmtsize++;
	if(nchar == '.')
	{
		// ∏Ò ΩªØ≤Œ ˝°£
		pchar++;
		while(pchar)
		{
			nchar = *pchar;
			if(nchar < '0' || nchar > '9')
			{
				break;
			}
			item->nparam = item->nparam*10+(nchar-'0');
			item->nfmtsize++;
			pchar++;
		}
		if(nchar == 0)
		{
			return item->nfmtsize;
		}
		item->nfmtsize++;
	}
	item->nflag = nchar;
	if(nchar == '@')
	{
		// 对象名称。
		switch(*(pchar+1))
		{
			case '(':		chbegin = '(';	chend = ')';		break;
			case '[':		chbegin = '[';	chend = ']';		break;
			case '{':		chbegin = '{';	chend = '}';		break;
			case '<':		chbegin = '<';	chend = '>';		break;
			default:		chbegin = 0;	chend = 0;			break;
		}
		if(chbegin != 0)
		{
			pchar += 2;
			item->nfmtsize += 2;
			item->pname = pchar;
			nmatch = 1;
			while(1)
			{
				nchar = *pchar;
				if(nchar == 0)
				{
					if(item->nnamesize <= 0)
					{
						item->pname = NULL;
					}
					item->nfmtsize--;
					break;
				}
				else if(nchar == chbegin)
				{
					nmatch++;
				}
				else if(nchar == chend)
				{
					nmatch--;
				}
				if(nmatch <= 0)
				{
					item->nfmtsize++;
					item->nnamesize++;
					break;
				}
				pchar++;
				item->nfmtsize++;
				item->nnamesize++;
			}
		}
	}
	
	return item->nfmtsize;
}

QINT qstrwrite(qstr_info_t *info, QPNT buff, QINT len)
{
	qstr_item_cb pcbitem;
	QCHR *psrc, *pdst, *pbuff;
	QINT nbuffsize, ndstlen, ndstsize, nstrlen;
	
	ndstlen = 0;
	nstrlen = 0;
	if(info == NULL)
	{
		return nstrlen;
	}
	pcbitem = info->pcbitem;
	psrc = (QCHR *)buff;
	pdst = (QCHR *)info->pbuff;
	ndstsize = info->nbuffsize;
	while(nstrlen < len)
	{
		if(ndstsize >= 0 && ndstlen >= ndstsize)
		{
			if(pcbitem == NULL)
			{
				break;
			}
			else
			{
				pbuff = NULL;
				nbuffsize = len;
				pcbitem(info, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
				if(pbuff == NULL)
				{
					break;
				}
				info->pbuff = pbuff;
				info->nbuffsize = nbuffsize;
				pdst = pbuff;
				ndstsize = nbuffsize;
				ndstlen = 0;
			}
		}
		*pdst++ = *psrc++;
		ndstlen++;
		nstrlen++;
	}
	
	return nstrlen;
}

#include "qmdl_module.h"

QINT qstrmakei(qstr_info_t *info)
{
	QUINT nval;
	qstr_item_cb pcbitem;
	qstr_info_t stinfo = {0};
	QCHR *parg, *pdst, *pchar, *pbuff;
	QCHR vbuff[(sizeof(QINT)<<3)+2] = {0};
	QINT nflag, nparam, nfillchar, nfillsize, nfillflag;
	QINT narg, nminus, ntmp, ndstcnt, ndstpos, nsrclen, ndstlen, ndstsize, nstrlen, nbuffsize;
	
	if(info == NULL)
	{
		return QNO_FAIL;
	}
	ndstcnt = 0;
	ndstlen = 0;
	nstrlen = 0;
	pcbitem = info->pcbitem;
	pdst = (QCHR *)info->pbuff;
	ndstsize = info->nbuffsize;
	nflag = info->stitem.nflag;
	if(nflag < 0)
	{
		goto MAKE_EXIT;
	}
	nparam = info->stitem.nparam;
	nfillflag = info->stitem.nfillflag;
	nfillchar = info->stitem.nfillchar;
	nfillsize = info->stitem.nfillsize;
	switch(nflag)
	{
		case '\0':
			parg = (QCHR *)info->stitem.pfmt;
			narg = info->stitem.nfmtsize;
			goto MAKE_STR;
			break;
			
		case '%':
			narg = '%';
			goto MAKE_CHR;
			break;
			
		case 'c':
			if(info->pargs != NULL)
			{
				narg = (QINT )va_arg(*(info->pargs), QINT);
			}
			else
			{
				narg = (QINT )(info->pparams)[0];
			}
			if(nfillsize > 0 && nfillchar == 0)
			{
				nfillchar = ' ';
			}
			ndstcnt++;
			goto MAKE_CHR;
			break;
			
		case 'd':
		case 'D':
			if(info->pargs != NULL)
			{
				narg = (QINT )va_arg(*(info->pargs), QINT);
			}
			else
			{
				narg = (QINT )(info->pparams)[0];
			}
			if(nfillsize > 0 && nfillchar == 0)
			{
				nfillchar = ' ';
			}
			if(nparam <= 0)
			{
				nparam = 10;
			}
			nminus = -1;
			ndstcnt++;
			goto MAKE_INT;
			break;
			
		case 'x':
		case 'X':
			if(info->pargs != NULL)
			{
				narg = (QINT )va_arg(*(info->pargs), QINT);
			}
			else
			{
				narg = (QINT )(info->pparams)[0];
			}
			if(nfillsize > 0 && nfillchar == 0)
			{
				nfillchar = ' ';
			}
			if(nparam <= 0)
			{
				nparam = 16;
			}
			nminus = 0;
			ndstcnt++;
			goto MAKE_INT;
			break;
			
		case 'i':
		case 'I':
			if(info->pargs != NULL)
			{
				narg = (QINT )va_arg(*(info->pargs), QINT);
			}
			else
			{
				narg = (QINT )(info->pparams)[0];
			}
			if(nfillsize > 0 && nfillchar == 0)
			{
				nfillchar = ' ';
			}
			nminus = -1;
			ndstcnt++;
			goto MAKE_INT;
			break;
			
		case 'u':
		case 'U':
			if(info->pargs != NULL)
			{
				narg = (QINT )va_arg(*(info->pargs), QINT);
			}
			else
			{
				narg = (QINT )(info->pparams)[0];
			}
			if(nfillsize > 0 && nfillchar == 0)
			{
				nfillchar = ' ';
			}
			nminus = 0;
			ndstcnt++;
			goto MAKE_INT;
			break;
			
		case 'p':
		case 'P':
			if(info->pargs != NULL)
			{
				narg = (QINT )va_arg(*(info->pargs), QINT);
			}
			else
			{
				narg = (QINT )(info->pparams)[0];
			}
			if(nfillchar == 0)
			{
				nfillchar = '0';
			}
			if(nfillsize <= 0)
			{
				nfillsize = sizeof(QPNT);
			}
			nparam = 16;
			nminus = 0;
			ndstcnt++;
			goto MAKE_INT;
			break;
			
		case 'n':
		case 'N':
			if(info->pargs != NULL)
			{
				narg = (QINT )va_arg(*(info->pargs), QINT);
			}
			else
			{
				narg = (QINT )(info->pparams)[0];
			}
			if(nparam <= 0)
			{
				nparam = sizeof(QINT);
			}
			if(nfillsize <= 0)
			{
				nfillsize = sizeof(QINT);
			}
			ndstcnt++;
			goto MAKE_NUM;
			break;
			
		case 's':
			if(info->pargs != NULL)
			{
				parg = (QCHR *)va_arg(*(info->pargs), QCHR *);
			}
			else
			{
				parg = (QCHR *)(info->pparams)[0];
			}
			if(nfillsize > 0 && nfillchar == 0)
			{
				nfillchar = ' ';
			}
			narg = 0;
			ndstcnt++;
			goto MAKE_STR;
			break;
			
		case 'S':
			if(info->pargs != NULL)
			{
				parg = (QCHR *)va_arg(*(info->pargs), QCHR *);
				narg = (QINT )va_arg(*(info->pargs), QINT);
			}
			else
			{
				parg = (QCHR *)(info->pparams)[0];
				narg = (QINT )(info->pparams)[1];
			}
			if(nfillsize > 0 && nfillchar == 0)
			{
				nfillchar = ' ';
			}
			ndstcnt += 2;
			goto MAKE_STR;
			break;
			
		case 'm':
		case 'M':
			if(info->pargs != NULL)
			{
				parg = (QCHR *)va_arg(*(info->pargs), QCHR *);
				narg = (QINT )va_arg(*(info->pargs), QINT);
			}
			else
			{
				parg = (QCHR *)(info->pparams)[0];
				narg = (QINT )(info->pparams)[1];
			}
			ndstcnt += 2;
			goto MAKE_STR;
			break;
			
		case 'v':
		case 'V':
			if(info->pargs != NULL)
			{
				parg = (QCHR *)va_arg(*(info->pargs), QCHR *);
				narg = (QINT )va_arg(*(info->pargs), QINT);
			}
			else
			{
				parg = (QCHR *)(info->pparams)[0];
				narg = (QINT )(info->pparams)[1];
			}
			if(nfillsize <= 0)
			{
				nfillsize = sizeof(QINT);
			}
			ndstcnt += 2;
			goto MAKE_VAL;
			break;
			
		case '@':
			if(info->pargs != NULL)
			{
				parg = (QCHR *)va_arg(*(info->pargs), QCHR *);
			}
			else
			{
				parg = (QCHR *)(info->pparams)[0];
			}
			ndstcnt++;
			goto MAKE_OBJ;
			break;
			
		default:
			parg = (QCHR *)info->stitem.pfmt;
			narg = info->stitem.nfmtsize;
			goto MAKE_STR;
			break;
	}
	goto MAKE_EXIT;
	
MAKE_CHR:
	if(nfillsize <= 0)
	{
		nfillsize = 1;
	}
	if(nparam <= 0)
	{
		nparam = 1;
	}
	while(nstrlen < nfillsize)
	{
		if(ndstsize >= 0 && ndstlen >= ndstsize)
		{
			if(pcbitem == NULL)
			{
				break;
			}
			else
			{
				pbuff = NULL;
				nbuffsize = 0;
				pcbitem(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
				if(pbuff == NULL)
				{
					break;
				}
				info->pbuff = pbuff;
				info->nbuffsize = nbuffsize;
				pdst = pbuff;
				ndstsize = nbuffsize;
				ndstlen = 0;
			}
		}
		if(nfillchar == 0)
		{
			*pdst++ = (QCHR )narg;
		}
		else
		{
			if(nfillflag == 1)
			{
				if(nstrlen < nfillsize-nparam)
				{
					*pdst++ = (QCHR )nfillchar;
				}
				else
				{
					*pdst++ = (QCHR )narg;
				}
			}
			else
			{
				if(nstrlen >= nparam)
				{
					*pdst++ = (QCHR )nfillchar;
				}
				else
				{
					*pdst++ = (QCHR )narg;
				}
			}
		}
		ndstlen++;
		nstrlen++;
	}
	goto MAKE_EXIT;
	
MAKE_INT:
	// 记录正负符号。
	if(nminus > 0)
	{
		nminus = 1;
	}
	else if(nminus < 0)
	{
		if(narg >= 0)
		{
			nminus = 0;
		}
		else
		{
			nminus = 1;
		}
	}
	if(nminus == 0)
	{
		nval = (QUINT )narg;
	}
	else
	{
		nval = (QUINT )(narg*(-1));
	}
	// 各种进制转换字符串。
	if(nparam <= 0)
	{
		// 默认为十进制。
		nparam = 10;
	}
	nsrclen = 0;
	pchar = vbuff;
	switch(nparam)
	{
		case 2:
			// 二进制。
			do
			{
				if(nsrclen >= (QINT )sizeof(vbuff))
				{
					break;
				}
				*pchar++ = '0'+(nval & 0x1);
				nval >>= 1;
				nsrclen++;
			}
			while(nval != 0);
			break;
			
		case 4:
			// 四进制。
			do
			{
				if(nsrclen >= (QINT )sizeof(vbuff))
				{
					break;
				}
				*pchar++ = '0'+(nval & 0x3);
				nval >>= 2;
				nsrclen++;
			}
			while(nval != 0);
			break;
			
		case 8:
			// 八进制。
			do
			{
				if(nsrclen >= (QINT )sizeof(vbuff))
				{
					break;
				}
				*pchar++ = '0'+(nval & 0x7);
				nval >>= 3;
				nsrclen++;
			}
			while(nval != 0);
			break;
			
		case 16:
			// 十六进制。
			do
			{
				if(nsrclen >= (QINT )sizeof(vbuff))
				{
					break;
				}
				ntmp = (nval & 0xf);
				if(ntmp < 10)
				{
					*pchar++ = (QCHR)('0'+ntmp);
				}
				else
				{
					if(nflag >= 'a' && nflag <= 'z')
					{
						*pchar++ = (QCHR)('a'+ntmp-10);
					}
					else
					{
						*pchar++ = (QCHR)('A'+ntmp-10);
					}
				}
				nval >>= 4;
				nsrclen++;
			}
			while(nval != 0);
			break;
			
		case 32:
			// 三十二进制。
			do
			{
				if(nsrclen >= (QINT )sizeof(vbuff))
				{
					break;
				}
				ntmp = (nval & 0x1f);
				if(ntmp < 10)
				{
					*pchar++ = (QCHR)('0'+ntmp);
				}
				else
				{
					if(nflag >= 'a' && nflag <= 'z')
					{
						*pchar++ = (QCHR)('a'+ntmp-10);
					}
					else
					{
						*pchar++ = (QCHR)('A'+ntmp-10);
					}
				}
				nval >>= 5;
				nsrclen++;
			}
			while(nval != 0);
			break;
			
		default:
			// 其他进制。
			do
			{
				if(nsrclen >= (QINT )sizeof(vbuff))
				{
					break;
				}
				ntmp = (nval % nparam);
				if(ntmp < 10)
				{
					*pchar++ = (QCHR)('0'+ntmp);
				}
				else
				{
					if(nflag >= 'a' && nflag <= 'z')
					{
						*pchar++ = (QCHR)('a'+ntmp-10);
					}
					else
					{
						*pchar++ = (QCHR)('A'+ntmp-10);
					}
				}
				nval /= nparam;
				nsrclen++;
			}
			while(nval != 0);
			break;
	}
	if(nminus == 1)
	{
		*pchar++ = '-';
		nsrclen++;
	}
	else if(nfillflag > 0)
	{
		*pchar++ = '+';
		nsrclen++;
	}
	if(nfillflag >= 0)
	{
		while(nstrlen < nfillsize-nsrclen)
		{
			if(ndstsize >= 0 && ndstlen >= ndstsize)
			{
				if(pcbitem == NULL)
				{
					goto MAKE_EXIT;
				}
				else
				{
					pbuff = NULL;
					nbuffsize = 0;
					pcbitem(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
					if(pbuff == NULL)
					{
						break;
					}
					info->pbuff = pbuff;
					info->nbuffsize = nbuffsize;
					pdst = pbuff;
					ndstsize = nbuffsize;
					ndstlen = 0;
				}
			}
			*pdst = (QCHR )nfillchar;
			if(nfillchar == '0')
			{
				if(nstrlen == 0)
				{
					if(nminus == 1)
					{
						*pdst = '-';
					}
					else if(nfillflag == 1)
					{
						*pdst = '+';
					}
				}
			}
			pdst++;
			ndstlen++;
			nstrlen++;
		}
	}
	// 倒置拷贝字符串。
	ndstpos = nsrclen-1;
	pchar = &vbuff[ndstpos];
	while(ndstpos >= 0)
	{
		if(ndstsize >= 0 && ndstlen >= ndstsize)
		{
			if(pcbitem == NULL)
			{
				goto MAKE_EXIT;
			}
			else
			{
				pbuff = NULL;
				nbuffsize = 0;
				pcbitem(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
				if(pbuff == NULL)
				{
					break;
				}
				info->pbuff = pbuff;
				info->nbuffsize = nbuffsize;
				pdst = pbuff;
				ndstsize = nbuffsize;
				ndstlen = 0;
			}
		}
		*pdst = *pchar--;
		if(nfillchar == '0')
		{
			if(ndstpos == nsrclen-1)
			{
				if(nminus == 1)
				{
					*pdst = '0';
				}
				else if(nfillflag == 1)
				{
					*pdst = '0';
				}
			}
		}
		pdst++;
		ndstpos--;
		nstrlen++;
	}
	if(nfillflag < 0)
	{
		while(nstrlen < nfillsize)
		{
			if(ndstsize >= 0 && ndstlen >= ndstsize)
			{
				if(pcbitem == NULL)
				{
					goto MAKE_EXIT;
				}
				else
				{
					pbuff = NULL;
					nbuffsize = 0;
					pcbitem(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
					if(pbuff == NULL)
					{
						break;
					}
					info->pbuff = pbuff;
					info->nbuffsize = nbuffsize;
					pdst = pbuff;
					ndstsize = nbuffsize;
					ndstlen = 0;
				}
			}
			*pdst++ = (QCHR )nfillchar;
			ndstlen++;
			nstrlen++;
		}
	}
	goto MAKE_EXIT;
	
MAKE_NUM:
	nval = (QUINT )narg;
	if(nfillflag < 0)
	{
		while(nstrlen < nfillsize-nparam)
		{
			if(ndstsize >= 0 && ndstlen >= ndstsize)
			{
				if(pcbitem == NULL)
				{
					goto MAKE_EXIT;
				}
				else
				{
					pbuff = NULL;
					nbuffsize = 0;
					pcbitem(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
					if(pbuff == NULL)
					{
						break;
					}
					info->pbuff = pbuff;
					info->nbuffsize = nbuffsize;
					pdst = pbuff;
					ndstsize = nbuffsize;
					ndstlen = 0;
				}
			}
			*pdst++ = (QCHR )nfillchar;
			ndstlen++;
			nstrlen++;
		}
	}
	if(nflag == 'n')
	{
		// 小端字节序。
		ntmp = 0;
		while(1)
		{
			if(nfillsize > 0 && nstrlen >= nfillsize)
			{
				break;
			}
			if(ndstsize >= 0 && ndstlen >= ndstsize)
			{
				if(pcbitem == NULL)
				{
					break;
				}
				else
				{
					pbuff = NULL;
					nbuffsize = 0;
					pcbitem(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
					if(pbuff == NULL)
					{
						break;
					}
					info->pbuff = pbuff;
					info->nbuffsize = nbuffsize;
					pdst = pbuff;
					ndstsize = nbuffsize;
					ndstlen = 0;
				}
			}
			if(nstrlen >= nparam)
			{
				*pdst++ = (QCHR )nfillchar;
			}
			else
			{
				// 低字节在低地址。
				*pdst++ = (QCHR )((nval>>ntmp) & 0xFF);
				ntmp += 8;
			}
			ndstlen++;
			nstrlen++;
		}
	}
	else
	{
		// 大端字节序。
		ntmp = ((nparam-1) << 3);
		while(1)
		{
			if(nfillsize > 0 && nstrlen >= nfillsize)
			{
				break;
			}
			if(ndstsize >= 0 && ndstlen >= ndstsize)
			{
				if(pcbitem == NULL)
				{
					break;
				}
				else
				{
					pbuff = NULL;
					nbuffsize = 0;
					pcbitem(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
					if(pbuff == NULL)
					{
						break;
					}
					info->pbuff = pbuff;
					info->nbuffsize = nbuffsize;
					pdst = pbuff;
					ndstsize = nbuffsize;
					ndstlen = 0;
				}
			}
			if(nstrlen >= nparam)
			{
				*pdst++ = (QCHR )nfillchar;
			}
			else
			{
				// 高字节在低地址。
				*pdst++ = (QCHR )((nval>>ntmp) & 0xFF);
				ntmp -= 8;
			}
			ndstlen++;
			nstrlen++;
		}
	}
	if(nfillflag >= 0)
	{
		while(nstrlen < nfillsize)
		{
			if(ndstsize >= 0 && ndstlen >= ndstsize)
			{
				if(pcbitem == NULL)
				{
					goto MAKE_EXIT;
				}
				else
				{
					pbuff = NULL;
					nbuffsize = 0;
					pcbitem(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
					if(pbuff == NULL)
					{
						break;
					}
					info->pbuff = pbuff;
					info->nbuffsize = nbuffsize;
					pdst = pbuff;
					ndstsize = nbuffsize;
					ndstlen = 0;
				}
			}
			*pdst++ = (QCHR )nfillchar;
			ndstlen++;
			nstrlen++;
		}
	}
	goto MAKE_EXIT;
	
MAKE_STR:
	if(parg == NULL)
	{
		while(1)
		{
			if(narg <= 0)
			{
				break;
			}
			else if(nstrlen >= narg)
			{
				break;
			}
			if(nparam > 0 && nstrlen >= nparam)
			{
				break;
			}
			if(nfillsize > 0 && nstrlen >= nfillsize)
			{
				break;
			}
			if(ndstsize >= 0 && ndstlen >= ndstsize)
			{
				if(pcbitem == NULL)
				{
					break;
				}
				else
				{
					pbuff = NULL;
					nbuffsize = 0;
					pcbitem(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
					if(pbuff == NULL)
					{
						break;
					}
					info->pbuff = pbuff;
					info->nbuffsize = nbuffsize;
					pdst = pbuff;
					ndstsize = nbuffsize;
					ndstlen = 0;
				}
			}
			*pdst++ = (QCHR )nfillchar;
			ndstlen++;
			nstrlen++;
		}
	}
	else
	{
		nsrclen = narg;
		if(nfillflag < 0)
		{
			if(nsrclen <= 0)
			{
				nsrclen = 0;
				pchar = parg;
				while(*pchar != 0)
				{
					nsrclen++;
					pchar++;
				}
			}
			if(nparam > 0)
			{
				if(nsrclen > nparam)
				{
					nsrclen = nparam;
				}
			}
			while(nstrlen < nfillsize-nsrclen)
			{
				if(ndstsize >= 0 && ndstlen >= ndstsize)
				{
					if(pcbitem == NULL)
					{
						goto MAKE_EXIT;
					}
					else
					{
						pbuff = NULL;
						nbuffsize = 0;
						pcbitem(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
						if(pbuff == NULL)
						{
							break;
						}
						info->pbuff = pbuff;
						info->nbuffsize = nbuffsize;
						pdst = pbuff;
						ndstsize = nbuffsize;
						ndstlen = 0;
					}
				}
				*pdst++ = (QCHR )nfillchar;
				ndstlen++;
				nstrlen++;
			}
		}
		ndstpos = 0;
		pchar = parg;
		while(1)
		{
			if(narg <= 0)
			{
				if(*pchar == 0)
				{
					break;
				}
			}
			else if(nstrlen >= narg)
			{
				break;
			}
			if(nparam > 0 && ndstpos >= nparam)
			{
				break;
			}
			if(ndstsize >= 0 && ndstlen >= ndstsize)
			{
				if(pcbitem == NULL)
				{
					break;
				}
				else
				{
					pbuff = NULL;
					nbuffsize = 0;
					pcbitem(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
					if(pbuff == NULL)
					{
						break;
					}
					info->pbuff = pbuff;
					info->nbuffsize = nbuffsize;
					pdst = pbuff;
					ndstsize = nbuffsize;
					ndstlen = 0;
				}
			}
			*pdst++ = *pchar++;
			ndstlen++;
			nstrlen++;
			ndstpos++;
		}
		if(nfillflag >= 0)
		{
			while(nstrlen < nfillsize)
			{
				if(ndstsize >= 0 && ndstlen >= ndstsize)
				{
					if(pcbitem == NULL)
					{
						goto MAKE_EXIT;
					}
					else
					{
						pbuff = NULL;
						nbuffsize = 0;
						pcbitem(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
						if(pbuff == NULL)
						{
							break;
						}
						info->pbuff = pbuff;
						info->nbuffsize = nbuffsize;
						pdst = pbuff;
						ndstsize = nbuffsize;
						ndstlen = 0;
					}
				}
				*pdst++ = (QCHR )nfillchar;
				ndstlen++;
				nstrlen++;
			}
		}
	}
	goto MAKE_EXIT;
	
MAKE_VAL:
	nval = (QUINT )narg;
	if(nflag == 'v')
	{
		// 小端字节序。
		ntmp = 0;
		while(1)
		{
			if(nfillsize > 0 && nstrlen >= nfillsize)
			{
				break;
			}
			if(ndstsize >= 0 && ndstlen >= ndstsize)
			{
				if(pcbitem == NULL)
				{
					break;
				}
				else
				{
					pbuff = NULL;
					nbuffsize = 0;
					pcbitem(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
					if(pbuff == NULL)
					{
						break;
					}
					info->pbuff = pbuff;
					info->nbuffsize = nbuffsize;
					pdst = pbuff;
					ndstsize = nbuffsize;
					ndstlen = 0;
				}
			}
			// 低字节在低地址。
			*pdst++ = (QCHR )((nval>>ntmp) & 0xFF);
			ntmp += 8;
			ndstlen++;
			nstrlen++;
		}
	}
	else
	{
		// 大端字节序。
		ntmp = ((nparam-1) << 3);
		while(1)
		{
			if(nfillsize > 0 && nstrlen >= nfillsize)
			{
				break;
			}
			if(ndstsize >= 0 && ndstlen >= ndstsize)
			{
				if(pcbitem == NULL)
				{
					break;
				}
				else
				{
					pbuff = NULL;
					nbuffsize = 0;
					pcbitem(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
					if(pbuff == NULL)
					{
						break;
					}
					info->pbuff = pbuff;
					info->nbuffsize = nbuffsize;
					pdst = pbuff;
					ndstsize = nbuffsize;
					ndstlen = 0;
				}
			}
			// 高字节在低地址。
			*pdst++ = (QCHR )((nval>>ntmp) & 0xFF);
			ntmp -= 8;
			ndstlen++;
			nstrlen++;
		}
	}
	pchar = parg;
	while(1)
	{
		if(nstrlen >= narg)
		{
			break;
		}
		if(ndstsize >= 0 && ndstlen >= ndstsize)
		{
			if(pcbitem == NULL)
			{
				break;
			}
			else
			{
				pbuff = NULL;
				nbuffsize = 0;
				pcbitem(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
				if(pbuff == NULL)
				{
					break;
				}
				info->pbuff = pbuff;
				info->nbuffsize = nbuffsize;
				pdst = pbuff;
				ndstsize = nbuffsize;
				ndstlen = 0;
			}
		}
		if(pchar == NULL)
		{
			*pdst++ = 0;
		}
		else
		{
			*pdst++ = *pchar++;
		}
		ndstlen++;
		nstrlen++;
	}
	goto MAKE_EXIT;
	
MAKE_OBJ:
	{
		QINT ncount;
		QPNT vpprms[8];
		QMDL pmodule = (QMDL)parg;
		if(pmodule != NULL)
		{
			ncount = 0;
			vpprms[ncount++] = (QPNT)info;
			nstrlen = pmodule->CallMethodx((QSTR)"qstrmake", QCD_CALL, vpprms, ncount);
			pdst = (QCHR*)info->pbuff;
			ndstsize = info->nbuffsize;
			ndstlen = 0;
		}
	}
	goto MAKE_EXIT;
	
MAKE_EXIT:
	info->pbuff = pdst;
	if(ndstsize < 0 || ndstlen < ndstsize)
	{
		*pdst = 0;
	}
	if(ndstlen <= ndstsize)
	{
		info->nbuffsize = ndstsize-ndstlen;
	}
	info->ncount = ndstcnt;
	
	return nstrlen;
}

QINT qstrmakex(QPNT str, QINT size, QPNT fmt, QPNT params[], QINT count)
{
	QCHR *psrc, *pdst;
	qstr_info_t stinfo = {0};
	QINT nchar, nsrccnt, nlen, nsrclen, ndstlen, ndstsize;
	
	if(str == NULL)
	{
		return QNO_FAIL;
	}
	nsrccnt = 0;
	ndstlen = 0;
	ndstsize = size;
	pdst = (QCHR *)str;
	psrc = (QCHR *)fmt;
	if(psrc == NULL)
	{
		if(params == NULL || count <= 0)
		{
			return 0;
		}
		psrc = (QCHR *)params[0];
	}
	stinfo.pstr = str;
	stinfo.nstrsize = size;
	while(psrc)
	{
		nchar = *psrc;
		if(nchar == 0)
		{
			break;
		}
		if(ndstsize >= 0 && ndstlen >= ndstsize)
		{
			break;
		}
		switch(nchar)
		{
			case '%':
				nsrclen = qstritem(psrc, &stinfo.stitem);
				if(nsrclen <= 0)
				{
					break;
				}
				stinfo.pbuff = (QPNT )pdst;
				stinfo.nbuffsize = ndstsize-ndstlen;
				stinfo.pparams = &params[nsrccnt];
				stinfo.ncount = count-nsrccnt;
				nlen = qstrmakei(&stinfo);
				if(nlen < 0)
				{
					break;
				}
				nsrccnt += stinfo.ncount;
				ndstlen += nlen;
				psrc += nsrclen;
				pdst += nlen;
				break;
				
			case '\\':
				psrc++;
				*pdst++ = *psrc++;
				ndstlen++;
				break;
				
			default:
				*pdst++ = (QCHR)nchar;
				psrc++;
				ndstlen++;
				break;
		}
	}
	if(ndstsize <= 0 || ndstlen < ndstsize)
	{
		*pdst = 0;
	}
	
	return ndstlen;
}

QINT qstrmakev(QPNT str, QINT size, QPNT fmt, va_list args)
{
	QCHR *psrc, *pdst;
	qstr_info_t stinfo = {0};
	QINT nchar, nsrccnt, nlen, nsrclen, ndstlen, ndstsize;
	
	if(str == NULL)
	{
		return QNO_FAIL;
	}
	nsrccnt = 0;
	ndstlen = 0;
	ndstsize = size;
	pdst = (QCHR *)str;
	psrc = (QCHR *)fmt;
	if(psrc == NULL)
	{
		psrc = va_arg(args, QCHR *);
	}
	stinfo.pstr = str;
	stinfo.nstrsize = size;
	while(psrc)
	{
		nchar = *psrc;
		if(nchar == 0)
		{
			break;
		}
		if(ndstsize >= 0 && ndstlen >= ndstsize)
		{
			break;
		}
		switch(nchar)
		{
			case '%':
				nsrclen = qstritem(psrc, &stinfo.stitem);
				if(nsrclen <= 0)
				{
					break;
				}
				stinfo.pbuff = (QPNT )pdst;
				stinfo.nbuffsize = ndstsize-ndstlen;
				stinfo.pargs = (va_list *)&args;
				stinfo.ncount = 0;
				nlen = qstrmakei(&stinfo);
				if(nlen < 0)
				{
					break;
				}
				nsrccnt += stinfo.ncount;
				ndstlen += nlen;
				psrc += nsrclen;
				pdst += nlen;
				break;
				
			case '\\':
				psrc++;
				*pdst++ = *psrc++;
				ndstlen++;
				break;
				
			default:
				*pdst++ = (QCHR)nchar;
				psrc++;
				ndstlen++;
				break;
		}
	}
	if(ndstsize <= 0 || ndstlen < ndstsize)
	{
		*pdst = 0;
	}
	
	return ndstlen;
}

QINT qstrmake(QPNT str, QINT size, QPNT fmt, ...)
{
	QINT nlen;
	
	va_list marker;
	
	va_start(marker, fmt);
	
	nlen = qstrmakev(str, size, fmt, marker);
	
	va_end(marker);
	
	return nlen;
}

QINT qstrmakecbx(QPNT str, QINT size, qstr_item_cb make_cb, QPNT fmt, QPNT params[], QINT count)
{
	QCHR *psrc, *pbuff;
	qstr_info_t stinfo = {0};
	QINT nsrccnt, ndstcnt, nlen, nsrclen, nstrlen, nbuffsize;
	
	if(str == NULL)
	{
		return QNO_FAIL;
	}
	nsrccnt = 0;
	psrc = (QCHR *)fmt;
	if(psrc == NULL)
	{
		if(params == NULL || count <= 0)
		{
			return 0;
		}
		psrc = (QCHR *)params[0];
		if(psrc == NULL)
		{
			return 0;
		}
	}
	stinfo.pstr = str;
	stinfo.nstrsize = size;
	if(make_cb == NULL)
	{
		nstrlen = 0;
		while(psrc)
		{
			if(*psrc == 0)
			{
				break;
			}
			nsrclen = qstritem(psrc, &stinfo.stitem);
			if(nsrclen <= 0)
			{
				break;
			}
			if(size >= 0 && nstrlen >= size)
			{
				break;
			}
			stinfo.pbuff = (QCHR *)str+nstrlen;
			if(size > 0)
			{
				stinfo.nbuffsize = size-nstrlen;
			}
			stinfo.pparams = &params[nsrccnt];
			stinfo.ncount = count-nsrccnt;
			nlen = qstrmakei(&stinfo);
			if(nlen < 0)
			{
				break;
			}
			nsrccnt += ndstcnt;
			nstrlen += nlen;
			psrc += nsrclen;
		}
	}
	else
	{
		stinfo.pcbitem = make_cb;
		stinfo.pparams = params;
		stinfo.ncount = count;
		pbuff = NULL;
		nbuffsize = 0;
		make_cb(&stinfo, QSTR_ITEM_BEGIN, (QPNT*)&pbuff, &nbuffsize);
		if(pbuff == NULL)
		{
			nbuffsize = 0;
			make_cb(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
			if(pbuff == NULL)
			{
				return QNO_FAIL;
			}
		}
		nstrlen = 0;
		while(psrc)
		{
			if(*psrc == 0)
			{
				break;
			}
			nsrclen = qstritem(psrc, &stinfo.stitem);
			if(nsrclen <= 0)
			{
				break;
			}
			stinfo.pparams = &params[nsrccnt];
			stinfo.ncount = count-nsrccnt;
			pbuff = (QCHR*)stinfo.pbuff;
			nbuffsize = stinfo.nbuffsize;
			nlen = make_cb(&stinfo, QSTR_ITEM_MAKE, (QPNT*)&pbuff, &nbuffsize);
			if(nlen < 0)
			{
				break;
			}
			nsrccnt += stinfo.ncount;
			nstrlen += nlen;
			psrc += nsrclen;
		}
		stinfo.pparams = &params[nsrccnt];
		stinfo.ncount = count-nsrccnt;
		pbuff = (QCHR*)stinfo.pbuff;
		nbuffsize = stinfo.nbuffsize;
		make_cb(&stinfo, QSTR_ITEM_END, (QPNT*)&pbuff, &nbuffsize);
	}
	
	return nstrlen;
}

QINT qstrmakecbv(QPNT str, QINT size, qstr_item_cb make_cb, QPNT fmt, va_list args)
{
	QCHR *psrc, *pbuff;
	qstr_info_t stinfo = {0};
	QINT nsrccnt, ndstcnt, nlen, nsrclen, nstrlen, nbuffsize;
	
	if(str == NULL)
	{
		return QNO_FAIL;
	}
	nsrccnt = 0;
	psrc = (QCHR *)fmt;
	if(psrc == NULL)
	{
		psrc = va_arg(args, QCHR *);
		if(psrc == NULL)
		{
			return 0;
		}
	}
	stinfo.pstr = str;
	stinfo.nstrsize = size;
	if(make_cb == NULL)
	{
		nstrlen = 0;
		while(psrc)
		{
			if(*psrc == 0)
			{
				break;
			}
			nsrclen = qstritem(psrc, &stinfo.stitem);
			if(nsrclen <= 0)
			{
				break;
			}
			if(size >= 0 && nstrlen >= size)
			{
				break;
			}
			stinfo.pbuff = (QCHR *)str+nstrlen;
			if(size > 0)
			{
				stinfo.nbuffsize = size-nstrlen;
			}
			stinfo.pargs = (va_list *)&args;
			stinfo.ncount = 0;
			nlen = qstrmakei(&stinfo);
			if(nlen < 0)
			{
				break;
			}
			nsrccnt += stinfo.ncount;
			nstrlen += nlen;
			psrc += nsrclen;
		}
	}
	else
	{
		stinfo.pcbitem = make_cb;
		stinfo.pargs = (va_list *)&args;
		pbuff = NULL;
		nbuffsize = 0;
		make_cb(&stinfo, QSTR_ITEM_BEGIN, (QPNT*)&pbuff, &nbuffsize);
		if(pbuff == NULL)
		{
			nbuffsize = 0;
			make_cb(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
			if(pbuff == NULL)
			{
				return QNO_FAIL;
			}
		}
		nstrlen = 0;
		while(psrc)
		{
			if(*psrc == 0)
			{
				break;
			}
			nsrclen = qstritem(psrc, &stinfo.stitem);
			if(nsrclen <= 0)
			{
				break;
			}
			ndstcnt = 0;
			stinfo.pargs = (va_list *)&args;
			stinfo.ncount = 0;
			pbuff = (QCHR*)stinfo.pbuff;
			nbuffsize = stinfo.nbuffsize;
			nlen = make_cb(&stinfo, QSTR_ITEM_MAKE, (QPNT*)&pbuff, &nbuffsize);
			if(nlen < 0)
			{
				break;
			}
			nsrccnt += stinfo.ncount;
			nstrlen += nlen;
			psrc += nsrclen;
		}
		stinfo.pargs = (va_list *)&args;
		stinfo.ncount = 0;
		pbuff = (QCHR*)stinfo.pbuff;
		nbuffsize = stinfo.nbuffsize;
		make_cb(&stinfo, QSTR_ITEM_END, (QPNT*)&pbuff, &nbuffsize);
	}
	
	return nstrlen;
}

QINT qstrmakecb(QPNT str, QINT size, qstr_item_cb make_cb, QPNT fmt, ...)
{
	QINT nlen;
	
	va_list marker;
	
	va_start(marker, fmt);
	
	nlen = qstrmakecbv(str, size, make_cb, fmt, marker);
	
	va_end(marker);
	
	return nlen;
}
