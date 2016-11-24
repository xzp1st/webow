
#include "qmdl_base.h"


static QINT qmdl_read_var_name(QSTR str, QINT size, QINT *flag, QSTR name, QINT namesize)
{
	// 变量名称读取。
	QINT nlen, nmatch, nsrcpos;
	QUCHR *pname, *psrc, chsrc, chbegin, chend;
	
	psrc = str;
	nsrcpos = 0;
	pname = name;
	nlen = 0;
	switch(*(psrc+1))
	{
		case '(':		chbegin = '(';	chend = ')';		break;
		case '[':		chbegin = '[';	chend = ']';		break;
		case '{':		chbegin = '{';	chend = '}';		break;
		case '<':		chbegin = '<';	chend = '>';		break;
		default:		chbegin = 0;	chend = 0;			break;
	}
	if(chbegin != 0)
	{
		psrc += 2;
		nsrcpos += 2;
		nmatch = 1;
		while(1)
		{
			if(size > 0 && nsrcpos > size)
			{
				break;
			}
			if(namesize > 0 && nlen >= namesize)
			{
				break;
			}
			chsrc = *psrc;
			if(chsrc == 0)
			{
				break;
			}
			else if(chsrc == chbegin)
			{
				nmatch++;
			}
			else if(chsrc == chend)
			{
				nmatch--;
			}
			if(nmatch <= 0)
			{
				psrc++;
				nsrcpos++;
				break;
			}
			*pname++ = *psrc++;
			nsrcpos++;
			nlen++;
		}
	}
	else
	{
		psrc += 1;
		nsrcpos += 1;
		while(1)
		{
			if(size > 0 && nsrcpos > size)
			{
				break;
			}
			if(namesize > 0 && nlen >= namesize)
			{
				break;
			}
			chsrc = *psrc;
			if(chsrc == 0)
			{
				break;
			}
			else if( !( (chsrc >= '0' && chsrc <= '9') || (chsrc >= 'a' && chsrc <= 'z') || (chsrc >= 'A' && chsrc <= 'Z') ) )
			{
				break;
			}
			else if(chsrc == ' ' || chsrc == '\t' || chsrc == '\r' || chsrc == '\n')
			{
				break;
			}
			*pname++ = *psrc++;
			nsrcpos++;
			nlen++;
		}
	}
	if(flag != NULL)
	{
		*flag = (QINT )chbegin;
	}
	
	return nlen;
}

QINT qmdlCopy(QMDL module, QMDL caller, QPNT params[], QINT count, QPNT url, QPNT dst, QINT dstsize, QPNT src, QINT *srcsize)
{
	QUCHR *pdst, *psrc, chsrc;
	QINT nlen, nflag, ndstpos, nsrcpos, nsrcsize;
	
	if(srcsize == NULL)
	{
		nsrcsize = 0;
	}
	else
	{
		nsrcsize = *srcsize;
	}
	ndstpos = 0;
	nsrcpos = 0;
	pdst = (QUCHR *)dst;
	psrc = (QUCHR *)src;
	while(psrc)
	{
		if(dstsize > 0)
		{
			if(ndstpos >= dstsize)
			{
				break;
			}
		}
		if(nsrcsize > 0)
		{
			if(nsrcpos >= nsrcsize)
			{
				break;
			}
		}
		chsrc = *psrc;
		if(chsrc == 0)
		{
			break;
		}
		if(chsrc == '$')
		{
			// URL变量读取。
			QMDL pmodule;
			QUCHR *ppath, vname[QSTR_BUFF_SIZE] = {0};
			
			nlen = qmdl_read_var_name(psrc, nsrcsize-nsrcpos, &nflag, vname, sizeof(vname));
			if(nlen > 0)
			{
				if(nflag == '{')
				{
					// ${name}，国际化变量读取。
					if(module == NULL)
					{
						nlen = 0;
					}
					else
					{
						if(((QModule *)module)->main == NULL)
						{
							nlen = 0;
						}
						else
						{
							nlen = qmdlLoadString(module, NULL, vname, (QSTR)pdst, dstsize-ndstpos);
						}
					}
				}
				else if(nflag == '[')
				{
					// $[name]，调用者变量读取，caller->name(params, count, buff, size)。
					nlen = 0;
					if(caller != NULL)
					{
						QINT ncount;
						QPNT *ppprms, vpprms[QPRM_BUFF_SIZE];
						
						ncount = count+2;
						ppprms = (QPNT *)qprmmakex(NULL, vpprms, QPRM_BUFF_SIZE, NULL, NULL, NULL, &ncount);
						if(ppprms != NULL)
						{
							if(params != NULL)
							{
								memcpy(ppprms, params, sizeof(QPNT)*ncount);
							}
							ppprms[ncount-2] = (QPNT )pdst;
							ppprms[ncount-1] = (QPNT )(dstsize-ndstpos);
							nlen = caller->CallMethodx(vname, QCD_CALL, ppprms, ncount);
						}
					}
				}
				else
				{
					// $(name)，url参数读取。
					nlen = 0;
					if(url != NULL)
					{
						nlen = qurlGetVar(ppath, vname, pdst, 0);
					}
					if(nlen <= 0)
					{
						pmodule = module;
						while(pmodule)
						{
							ppath = pmodule->GetPath();
							if(dstsize <= 0)
							{
								nlen = qurlGetVar(ppath, vname, pdst, 0);
							}
							else
							{
								nlen = qurlGetVar(ppath, vname, pdst, dstsize-ndstpos);
							}
							if(nlen > 0)
							{
								break;
							}
							pmodule = pmodule->ParentModule();
						}
					}
				}
				if(nlen > 0)
				{
					if(pdst != NULL)
					{
						pdst += nlen;
					}
					ndstpos += nlen;
				}
			}
		}// chsrc=='$'
		else
		{
			if(pdst == NULL)
			{
				psrc++;
			}
			else
			{
				*pdst++ = *psrc++;
			}
			ndstpos++;
			nsrcpos++;
		}
	}// psrc
	if(srcsize != NULL)
	{
		*srcsize = nsrcpos;
	}
	if(pdst != NULL)
	{
		if(dstsize > 0 && ndstpos < dstsize)
		{
			*pdst = 0;
		}
	}
	
	return ndstpos;
}
