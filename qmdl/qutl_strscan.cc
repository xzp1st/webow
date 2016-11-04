
#include "qmdl_util.h"


QINT gstrread(qstr_info_t *info, QPNT buff, QINT len)
{
	qstr_item_cb pcbitem;
	QCHR *psrc, *pdst, *pbuff;
	QINT nbuffsize, nsrclen, nsrcsize, nstrlen;
	
	nsrclen = 0;
	nstrlen = 0;
	if(info == NULL)
	{
		return nstrlen;
	}
	pcbitem = info->pcbitem;
	psrc = (QCHR *)info->pbuff;
	nsrcsize = info->nbuffsize;
	pdst = (QCHR *)buff;
	while(nstrlen < len)
	{
		if(nsrcsize >= 0 && nsrclen >= nsrcsize)
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
				psrc = pbuff;
				nsrcsize = nbuffsize;
				nsrclen = 0;
			}
		}
		*pdst++ = *psrc++;
		nsrclen++;
		nstrlen++;
	}
	
	return nstrlen;
}

#include "qmdl_module.h"

QINT qstrscani(qstr_info_t *info)
{
	QUINT nval;
	qstr_item_cb pcbitem;
	qstr_info_t stinfo = {0};
	QCHR *parg, *psrc, *pchar, *ptmp, *pbuff;
	//QCHR vbuff[(sizeof(QINT)<<3)+2] = {0};
	QINT nflag, nparam, nfillchar, nfillsize, nfillflag, nmatchflag;
	QINT *pnarg, narg, nminus, ntmp, ndstcnt, ndstpos, ndstlen, nsrclen, nsrcsize, nstrlen, nbuffsize;
	
	if(info == NULL)
	{
		return QNO_FAIL;
	}
	ndstcnt = 0;
	nsrclen = 0;
	nstrlen = 0;
	pcbitem = info->pcbitem;
	psrc = (QCHR *)info->pbuff;
	nsrcsize = info->nbuffsize;
	nflag = info->stitem.nflag;
	if(nflag < 0)
	{
		goto SCAN_EXIT;
	}
	nparam = info->stitem.nparam;
	nfillflag = info->stitem.nfillflag;
	nfillchar = info->stitem.nfillchar;
	nfillsize = info->stitem.nfillsize;
	switch(nflag)
	{
		case '\0':
			goto SCAN_FMT;
			break;
			
		case '%':
			goto SCAN_PERCENT;
			break;
			
		case 'c':
			if(info->pargs != NULL)
			{
				pnarg = (QINT *)va_arg(*(info->pargs), QINT *);
			}
			else
			{
				pnarg = (QINT *)(info->pparams)[0];
			}
			if(nfillsize > 0 && nfillchar == 0)
			{
				nfillchar = ' ';
			}
			ndstcnt++;
			goto SCAN_CHR;
			break;
			
		case 'd':
		case 'D':
			if(info->pargs != NULL)
			{
				pnarg = (QINT *)va_arg(*(info->pargs), QINT *);
			}
			else
			{
				pnarg = (QINT *)(info->pparams)[0];
			}
			if(nfillsize > 0 && nfillchar == 0)
			{
				nfillchar = ' ';
			}
			if(nparam <= 0)
			{
				nparam = 10;
			}
			nminus = 0;
			ndstcnt++;
			goto SCAN_INT;
			break;
			
		case 'x':
		case 'X':
			if(info->pargs != NULL)
			{
				pnarg = (QINT *)va_arg(*(info->pargs), QINT *);
			}
			else
			{
				pnarg = (QINT *)(info->pparams)[0];
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
			goto SCAN_INT;
			break;
			
		case 'i':
		case 'I':
			if(info->pargs != NULL)
			{
				pnarg = (QINT *)va_arg(*(info->pargs), QINT *);
			}
			else
			{
				pnarg = (QINT *)(info->pparams)[0];
			}
			if(nfillsize > 0 && nfillchar == 0)
			{
				nfillchar = ' ';
			}
			nminus = 0;
			ndstcnt++;
			goto SCAN_INT;
			break;
			
		case 'u':
		case 'U':
			if(info->pargs != NULL)
			{
				pnarg = (QINT *)va_arg(*(info->pargs), QINT *);
			}
			else
			{
				pnarg = (QINT *)(info->pparams)[0];
			}
			if(nfillsize > 0 && nfillchar == 0)
			{
				nfillchar = ' ';
			}
			nminus = 0;
			ndstcnt++;
			goto SCAN_INT;
			break;
			
		case 'p':
		case 'P':
			if(info->pargs != NULL)
			{
				pnarg = (QINT *)va_arg(*(info->pargs), QINT *);
			}
			else
			{
				pnarg = (QINT *)(info->pparams)[0];
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
			goto SCAN_INT;
			break;
			
		case 'n':
		case 'N':
			if(info->pargs != NULL)
			{
				pnarg = (QINT *)va_arg(*(info->pargs), QINT *);
			}
			else
			{
				pnarg = (QINT *)(info->pparams)[0];
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
			goto SCAN_NUM;
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
			goto SCAN_STR;
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
			goto SCAN_STR;
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
			goto SCAN_STR;
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
			goto SCAN_VAL;
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
			goto SCAN_OBJ;
			break;
			
		default:
			parg = NULL;
			narg = 0;
			goto SCAN_STR;
			break;
	}
	goto SCAN_EXIT;
	
SCAN_FMT:
	pchar = (QCHR *)info->stitem.pfmt;
	while(nstrlen < info->stitem.nfmtsize)
	{
		if(nsrcsize >= 0 && nsrclen >= nsrcsize)
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
				psrc = pbuff;
				nsrcsize = nbuffsize;
				nsrclen = 0;
			}
		}
		if(*psrc != *pchar)
		{
			break;
		}
		psrc++;
		pchar++;
		nsrclen++;
		nstrlen++;
	}
	goto SCAN_EXIT;
	
SCAN_PERCENT:
	pchar = (QCHR *)info->stitem.pfmt;
	while(nstrlen < info->stitem.nfmtsize)
	{
		if(nsrcsize >= 0 && nsrclen >= nsrcsize)
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
				psrc = pbuff;
				nsrcsize = nbuffsize;
				nsrclen = 0;
			}
		}
		if(*psrc != '%')
		{
			break;
		}
		psrc++;
		pchar++;
		nsrclen++;
		nstrlen++;
	}
	goto SCAN_EXIT;
	
SCAN_CHR:
	if(nfillsize <= 0)
	{
		nfillsize = 1;
	}
	if(nparam <= 0)
	{
		nparam = 1;
	}
	narg = 0;
	while(nstrlen < nfillsize)
	{
		if(nsrcsize >= 0 && nsrclen >= nsrcsize)
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
				psrc = pbuff;
				nsrcsize = nbuffsize;
				nsrclen = 0;
			}
		}
		if(nfillchar == 0)
		{
			narg = *psrc;
		}
		else
		{
			if(nfillflag == 1)
			{
				if(nstrlen < nfillsize-nparam)
				{
					narg = (QCHR )nfillchar;
				}
				else
				{
					narg = *psrc;
				}
			}
			else
			{
				if(nstrlen >= nparam)
				{
					narg = (QCHR )nfillchar;
				}
				else
				{
					narg = *psrc;
				}
			}
		}
		psrc++;
		nsrclen++;
		nstrlen++;
	}
	if(pnarg != NULL)
	{
		*pnarg = narg;
	}
	goto SCAN_EXIT;
	
SCAN_INT:
	// 各种进制转换字符串。
	if(nparam <= 0)
	{
		// 默认为十进制。
		nparam = 10;
	}
	if(nfillflag >= 0)
	{
		while(nstrlen < nfillsize)
		{
			if(nsrcsize >= 0 && nsrclen >= nsrcsize)
			{
				if(pcbitem == NULL)
				{
					goto SCAN_EXIT;
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
					psrc = pbuff;
					nsrcsize = nbuffsize;
					nsrclen = 0;
				}
			}
			if(*psrc == '+')
			{
				nminus = 0;
			}
			else if(*psrc == '-')
			{
				nminus = 1;
			}
			else if(*psrc != nfillchar)
			{
				break;
			}
			psrc++;
			nsrclen++;
			nstrlen++;
		}
	}
	nval = 0;
	while(1)
	{
		if(nfillsize > 0 && nstrlen >= nfillsize)
		{
			break;
		}
		if(nsrcsize >= 0 && nsrclen >= nsrcsize)
		{
			if(pcbitem == NULL)
			{
				goto SCAN_EXIT;
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
				psrc = pbuff;
				nsrcsize = nbuffsize;
				nsrclen = 0;
			}
		}
		if(nfillflag < 0)
		{
			if(*psrc == nfillchar)
			{
				break;
			}
		}
		ntmp = *psrc;
		if(nparam == 2)
		{
			// 二进制。
			if(ntmp >= '0' && ntmp <= '1')
			{
				nval <<= 1;
				nval |= ntmp-'0';
			}
			else
			{
				break;
			}
		}
		else if(nparam == 4)
		{
			// 四进制。
			if(ntmp >= '0' && ntmp <= '3')
			{
				nval <<= 2;
				nval |= ntmp-'0';
			}
			else
			{
				break;
			}
		}
		else if(nparam == 8)
		{
			// 八进制。
			if(ntmp >= '0' && ntmp <= '7')
			{
				nval <<= 3;
				nval |= ntmp-'0';
			}
			else
			{
				break;
			}
		}
		else if(nparam == 16)
		{
			// 十六进制。
			if(ntmp >= '0' && ntmp <= '9')
			{
				nval <<= 4;
				nval |= ntmp-'0';
			}
			else if(ntmp >= 'a' && ntmp <= 'f')
			{
				nval <<= 4;
				nval |= ntmp-'a';
			}
			else if(ntmp >= 'A' && ntmp <= 'F')
			{
				nval <<= 4;
				nval |= ntmp-'A';
			}
			else
			{
				break;
			}
		}
		else if(nparam == 32)
		{
			// 三十二进制。
			if(ntmp >= '0' && ntmp <= '9')
			{
				nval <<= 5;
				nval |= ntmp-'0';
			}
			else if(ntmp >= 'a' && ntmp <= 'u')
			{
				nval <<= 5;
				nval |= ntmp-'a';
			}
			else if(ntmp >= 'A' && ntmp <= 'U')
			{
				nval <<= 5;
				nval |= ntmp-'A';
			}
			else
			{
				break;
			}
		}
		else
		{
			// 其他进制。
			if(nparam <= 10)
			{
				if(ntmp >= '0' && ntmp <= '0'+nparam-1)
				{
					nval = (nval*nparam) + (ntmp-'0');
				}
				else
				{
					break;
				}
			}
			else
			{
				if(ntmp >= '0' && ntmp <= '9')
				{
					nval = (nval*nparam) + (ntmp-'0');
				}
				else if(ntmp >= 'a' && ntmp <= 'a'+nparam-11)
				{
					nval = (nval*nparam) + (10+ntmp-'a');
				}
				else if(ntmp >= 'A' && ntmp <= 'A'+nparam-11)
				{
					nval = (nval*nparam) + (10+ntmp-'A');
				}
				else
				{
					break;
				}
			}
		}
		psrc++;
		nsrclen++;
		nstrlen++;
	}
	if(nfillflag < 0)
	{
		while(nstrlen < nfillsize)
		{
			if(nsrcsize >= 0 && nsrclen >= nsrcsize)
			{
				if(pcbitem == NULL)
				{
					goto SCAN_EXIT;
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
					psrc = pbuff;
					nsrcsize = nbuffsize;
					nsrclen = 0;
				}
			}
			if(*psrc != nfillchar)
			{
				break;
			}
			psrc++;
			nsrclen++;
			nstrlen++;
		}
	}
	if(pnarg != NULL)
	{
		if(nminus == 0)
		{
			*pnarg = (QINT )nval;
		}
		else
		{
			*pnarg = ((QINT )nval)*(-1);
		}
	}
	goto SCAN_EXIT;
	
SCAN_NUM:
	nval = 0;
	if(nfillflag < 0)
	{
		while(nstrlen < nfillsize-nparam)
		{
			if(nsrcsize >= 0 && nsrclen >= nsrcsize)
			{
				if(pcbitem == NULL)
				{
					goto SCAN_EXIT;
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
					psrc = pbuff;
					nsrcsize = nbuffsize;
					nsrclen = 0;
				}
			}
			if(*psrc != nfillchar)
			{
				break;
			}
			psrc++;
			nsrclen++;
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
			if(nsrcsize >= 0 && nsrclen >= nsrcsize)
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
					psrc = pbuff;
					nsrcsize = nbuffsize;
					nsrclen = 0;
				}
			}
			if(nstrlen < nparam)
			{
				// 低字节在低地址。
				nval |= (((QINT )(*psrc)&0x00FF) << ntmp);
				ntmp += 8;
			}
			psrc++;
			nsrclen++;
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
			if(nsrcsize >= 0 && nsrclen >= nsrcsize)
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
					psrc = pbuff;
					nsrcsize = nbuffsize;
					nsrclen = 0;
				}
			}
			if(nstrlen < nparam)
			{
				// 高字节在低地址。
				nval |= (((QINT)(*psrc)&0x00FF) << ntmp);
				ntmp -= 8;
			}
			psrc++;
			nsrclen++;
			nstrlen++;
		}
	}
	if(nfillflag >= 0)
	{
		while(nstrlen < nfillsize)
		{
			if(nsrcsize >= 0 && nsrclen >= nsrcsize)
			{
				if(pcbitem == NULL)
				{
					goto SCAN_EXIT;
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
					psrc = pbuff;
					nsrcsize = nbuffsize;
					nsrclen = 0;
				}
			}
			if(*psrc != nfillchar)
			{
				break;
			}
			psrc++;
			nsrclen++;
			nstrlen++;
		}
	}
	if(pnarg != NULL)
	{
		*pnarg = (QINT )nval;
	}
	goto SCAN_EXIT;
	
SCAN_STR:
	if(nfillflag < 0)
	{
		while(nstrlen < nfillsize)
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
			if(nsrcsize >= 0 && nsrclen >= nsrcsize)
			{
				if(pcbitem == NULL)
				{
					goto SCAN_EXIT;
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
					psrc = pbuff;
					nsrcsize = nbuffsize;
					nsrclen = 0;
				}
			}
			if(*psrc != nfillchar)
			{
				break;
			}
			psrc++;
			nsrclen++;
			nstrlen++;
		}
	}
	nval = *((QCHR *)info->stitem.pfmt+info->stitem.nfmtsize);
	if(nval == 0)
	{
		nmatchflag = 0;
	}
	else if(nval == '%')
	{
		nmatchflag = 0;
	}
	else
	{
		nmatchflag = 1;
	}
	ntmp = 0;
	ndstpos = 0;
	ptmp = NULL;
	pchar = parg;
	while(1)
	{
		if(nparam > 0 && ndstpos >= nparam)
		{
			break;
		}
		if(nsrcsize >= 0 && nsrclen >= nsrcsize)
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
				psrc = pbuff;
				nsrcsize = nbuffsize;
				nsrclen = 0;
			}
		}
		if(narg <= 0)
		{
			// 缓冲区无大小限制，源以零结尾判断。
			if(nflag == 's' || nflag == 'S')
			{
				if(*psrc == 0)
				{
					break;
				}
			}
			if(pchar != NULL)
			{
				*pchar = *psrc;
			}
		}
		else
		{
			// 缓冲区带大小限制。
			if(nstrlen < narg)
			{
				if(pchar != NULL)
				{
					*pchar = *psrc;
				}
			}
		}
		if(nmatchflag == 1)
		{
			// 需要匹配后续字符串。
			nval = *((QCHR *)info->stitem.pfmt+info->stitem.nfmtsize+ntmp);
			if(nval == '\\')
			{
				ntmp++;
				ndstlen = 2;
				nval = qstrint(16, (QCHR *)info->stitem.pfmt+info->stitem.nfmtsize+ntmp, &ndstlen);
				ntmp += ndstlen;
			}
			if(ptmp == NULL)
			{
				if((QCHR )nval == *psrc)
				{
					// 第一个字符匹配。
					ntmp++;
					if(parg != NULL)
					{
						ptmp = pchar;
					}
					else
					{
						ptmp = (QCHR *)ntmp;
					}
				}
				else
				{
					ntmp = 0;
				}
			}
			else
			{
				if((QCHR )nval == *psrc)
				{
					// 后续字符匹配。
					ntmp++;
				}
				else if((QCHR )nval == 0 || (QCHR )nval == '%')
				{
					// 匹配结束标志。
					if(parg != NULL)
					{
						if(narg <= 0)
						{
							*ptmp = 0;
						}
						else if((QINT )(ptmp-parg) < narg)
						{
							*ptmp = 0;
						}
					}
					info->stitem.nfmtsize += ntmp;
					ptmp = NULL;
					ntmp = 0;
					break;
				}
				else
				{
					// 不匹配。
					ptmp = NULL;
					ntmp = 0;
				}
			}
		}
		if(pchar != NULL)
		{
			pchar++;
		}
		psrc++;
		nsrclen++;
		nstrlen++;
		ndstpos++;
	}
	if(nfillflag >= 0)
	{
		while(nstrlen < nfillsize)
		{
			if(nsrcsize >= 0 && nsrclen >= nsrcsize)
			{
				if(pcbitem == NULL)
				{
					goto SCAN_EXIT;
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
					psrc = pbuff;
					nsrcsize = nbuffsize;
					nsrclen = 0;
				}
			}
			if(*psrc != nfillchar)
			{
				break;
			}
			psrc++;
			nsrclen++;
			nstrlen++;
		}
	}
	if(parg != NULL)
	{
		if(narg <= 0)
		{
			*pchar = 0;
		}
		else if(narg > (QINT )pchar-(QINT )parg)
		{
			*pchar = 0;
		}
	}
	goto SCAN_EXIT;
	
SCAN_VAL:
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
			if(nsrcsize >= 0 && nsrclen >= nsrcsize)
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
					psrc = pbuff;
					nsrcsize = nbuffsize;
					nsrclen = 0;
				}
			}
			// 低字节在低地址。
			nval |= (((QINT )(*psrc)&0x00FF) << ntmp);
			ntmp += 8;
			psrc++;
			nsrclen++;
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
			if(nsrcsize >= 0 && nsrclen >= nsrcsize)
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
					psrc = pbuff;
					nsrcsize = nbuffsize;
					nsrclen = 0;
				}
			}
			// 高字节在低地址。
			nval |= (((QINT )(*psrc)&0x00FF) << ntmp);
			ntmp -= 8;
			psrc++;
			nsrclen++;
			nstrlen++;
		}
	}
	ndstpos = 0;
	pchar = parg;
	while(1)
	{
		if(nstrlen >= nval)
		{
			break;
		}
		if(nsrcsize >= 0 && nsrclen >= nsrcsize)
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
				psrc = pbuff;
				nsrcsize = nbuffsize;
				nsrclen = 0;
			}
		}
		if(pchar != NULL)
		{
			if(narg <= 0)
			{
				// 缓冲区无大小限制，源以零结尾判断。
				*pchar = *psrc;
			}
			else
			{
				// 缓冲区带大小限制。
				if(nstrlen < narg)
				{
					*pchar = *psrc;
				}
			}
			pchar++;
		}
		psrc++;
		nsrclen++;
		nstrlen++;
		ndstpos++;
	}
	goto SCAN_EXIT;
	
SCAN_OBJ:
	{
		QPMK pcbmake;
		QINT ncount;
		QPNT vpprms[8];
		QMDL pmodule, *ppmodule;
		
		ppmodule = (QMDL*)parg;
		if(ppmodule == NULL)
		{
			goto SCAN_EXIT;
		}
		pmodule = *ppmodule;
		if(pmodule == NULL)
		{
			goto SCAN_EXIT;
		}
		if(info->stitem.pname != NULL)
		{
			pmodule = pmodule->FindModule((QSTR)info->stitem.pname, 0, NULL, 0);
			if(pmodule == NULL)
			{
				goto SCAN_EXIT;
			}
			pcbmake = pmodule->GetMakeCb();
			pmodule = qmdlMake(*ppmodule, *ppmodule, pcbmake, NULL, NULL, NULL);
			if(pmodule == NULL)
			{
				goto SCAN_EXIT;
			}
		}
		ncount = 0;
		vpprms[ncount++] = (QPNT)info;
		nstrlen = pmodule->CallMethodx((QSTR)"qstrscan", QCD_CALL, vpprms, ncount);
		psrc = (QCHR*)info->pbuff;
		nsrcsize = info->nbuffsize;
		nsrclen = 0;
	}
	goto SCAN_EXIT;
	
SCAN_EXIT:
	info->pbuff = psrc;
	if(nsrclen <= nsrcsize)
	{
		info->nbuffsize = nsrcsize-nsrclen;
	}
	info->ncount = ndstcnt;
	
	return nstrlen;
}

QINT gstrscanx(QPNT str, QINT size, QPNT fmt, QPNT params[], QINT count)
{
	QCHR *psrc, *pdst;
	qstr_info_t stinfo = {0};
	QINT nsrccnt, nlen, nsrclen, ndstlen, ndstsize;
	
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
		if(*psrc == 0)
		{
			break;
		}
		if(ndstsize >= 0 && ndstlen >= ndstsize)
		{
			break;
		}
		nsrclen = qstritem(psrc, &stinfo.stitem);
		if(nsrclen <= 0)
		{
			break;
		}
		stinfo.pbuff = (QPNT )pdst;
		stinfo.nbuffsize = ndstsize-ndstlen;
		stinfo.pparams = &params[nsrccnt];
		stinfo.ncount = count-nsrccnt;
		nlen = qstrscani(&stinfo);
		if(nlen < 0)
		{
			break;
		}
		nsrccnt += stinfo.ncount;
		ndstlen += nlen;
		psrc += nsrclen;
		pdst += nlen;
	}
	if(ndstsize <= 0 || ndstlen < ndstsize)
	{
		*pdst = 0;
	}
	
	return ndstlen;
}

QINT qstrscanv(QPNT str, QINT size, QPNT fmt, va_list args)
{
	QCHR *psrc, *pdst;
	qstr_info_t stinfo = {0};
	QINT nsrccnt, nlen, nsrclen, ndstlen, ndstsize;
	
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
		if(*psrc == 0)
		{
			break;
		}
		if(ndstsize >= 0 && ndstlen >= ndstsize)
		{
			break;
		}
		nsrclen = qstritem(psrc, &stinfo.stitem);
		if(nsrclen <= 0)
		{
			break;
		}
		stinfo.pbuff = (QPNT )pdst;
		stinfo.nbuffsize = ndstsize-ndstlen;
		stinfo.pargs = (va_list *)&args;
		stinfo.ncount = 0;
		nlen = qstrscani(&stinfo);
		if(nlen < 0)
		{
			break;
		}
		nsrccnt += stinfo.ncount;
		ndstlen += nlen;
		psrc += nsrclen;
		pdst += nlen;
	}
	if(ndstsize <= 0 || ndstlen < ndstsize)
	{
		*pdst = 0;
	}
	
	return ndstlen;
}

QINT qstrscan(QPNT str, QINT size, QPNT fmt, ...)
{
	QINT nlen;
	
	va_list marker;
	
	va_start(marker, fmt);
	
	nlen = qstrscanv(str, size, fmt, marker);
	
	va_end(marker);
	
	return nlen;
}

QINT qstrscancbx(QPNT str, QINT size, qstr_item_cb scan_cb, QPNT fmt, QPNT params[], QINT count)
{
	QCHR *psrc, *pbuff;
	qstr_info_t stinfo = {0};
	QINT nsrccnt, nlen, nsrclen, nstrlen, nbuffsize;
	
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
	if(scan_cb == NULL)
	{
		nstrlen = 0;
		while(psrc)
		{
			if(*psrc == 0)
			{
				break;
			}
			if(size >= 0 && nstrlen >= size)
			{
				break;
			}
			nsrclen = qstritem(psrc, &stinfo.stitem);
			if(nsrclen <= 0)
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
			nlen = qstrscani(&stinfo);
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
		stinfo.pcbitem = scan_cb;
		stinfo.pparams = params;
		stinfo.ncount = count;
		pbuff = NULL;
		nbuffsize = 0;
		scan_cb(&stinfo, QSTR_ITEM_BEGIN, (QPNT*)&pbuff, &nbuffsize);
		if(pbuff == NULL)
		{
			nbuffsize = 0;
			scan_cb(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
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
			nlen = scan_cb(&stinfo, QSTR_ITEM_SCAN, (QPNT*)&pbuff, &nbuffsize);
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
		scan_cb(&stinfo, QSTR_ITEM_END, (QPNT*)&pbuff, &nbuffsize);
	}
	
	return nstrlen;
}

QINT gstrscancbv(QPNT str, QINT size, qstr_item_cb scan_cb, QPNT fmt, va_list args)
{
	QCHR *psrc, *pbuff;
	qstr_info_t stinfo = {0};
	QINT nsrccnt, nlen, nsrclen, nstrlen, nbuffsize;
	
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
	if(scan_cb == NULL)
	{
		nstrlen = 0;
		while(psrc)
		{
			if(*psrc == 0)
			{
				break;
			}
			if(size >= 0 && nstrlen >= size)
			{
				break;
			}
			nsrclen = qstritem(psrc, &stinfo.stitem);
			if(nsrclen <= 0)
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
			nlen = qstrscani(&stinfo);
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
		stinfo.pcbitem = scan_cb;
		stinfo.pargs = (va_list *)&args;
		pbuff = NULL;
		nbuffsize = 0;
		scan_cb(&stinfo, QSTR_ITEM_BEGIN, (QPNT*)&pbuff, &nbuffsize);
		if(pbuff == NULL)
		{
			nbuffsize = 0;
			scan_cb(&stinfo, QSTR_ITEM_BUFF, (QPNT*)&pbuff, &nbuffsize);
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
			stinfo.pargs = (va_list *)&args;
			stinfo.ncount = 0;
			pbuff = (QCHR*)stinfo.pbuff;
			nbuffsize = stinfo.nbuffsize;
			nlen = scan_cb(&stinfo, QSTR_ITEM_SCAN, (QPNT*)&pbuff, &nbuffsize);
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
		scan_cb(&stinfo, QSTR_ITEM_END, (QPNT*)&pbuff, &nbuffsize);
	}
	
	return nstrlen;
}

QINT gstrscancb(QPNT str, QINT size, qstr_item_cb scan_cb, QPNT fmt, ...)
{
	QINT nlen;
	
	va_list marker;
	
	va_start(marker, fmt);
	
	nlen = gstrscancbv(str, size, scan_cb, fmt, marker);
	
	va_end(marker);
	
	return nlen;
}
