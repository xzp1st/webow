
#include "qmdl_util.h"


QINT qstrlen(QPNT str)
{
	QINT nlen;
	QUCHR *pstr;
	
	if(str == NULL)
	{
		return 0;
	}
	nlen = 0;
	pstr = (QUCHR *)str;
	while(*pstr++ != 0)
	{
		nlen++;
	}
	
	return nlen;
}

QINT qstrcpy(QPNT str1, QINT size1, QPNT str2, QINT size2)
{
	QINT ncount;
	QUCHR *pdst, *psrc;
	
	ncount = 0;
	pdst = (QUCHR *)str1;
	psrc = (QUCHR *)str2;
	while(pdst && psrc)
	{
		if(size1 > 0)
		{
			if(ncount >= size1)
			{
				break;
			}
		}
		if(size2 > 0)
		{
			if(ncount >= size2)
			{
				*pdst = 0;
				break;
			}
		}
		if(*psrc == 0)
		{
			*pdst = 0;
			break;
		}
		*pdst++ = *psrc++;
		ncount++;
	}
	
	return ncount;
}

QPNT qstrdup(QPNT mem, QPNT str, ...)
{
	QINT nlen;
	QPNT pdststr;
	QCHR vbuffer[QSTR_BUFF_SIZE];
	va_list marker;
	
	if(str == NULL)
	{
		return NULL;
	}
	va_start( marker, str );
	nlen = qstrmakev(vbuffer, sizeof(vbuffer), str, marker);
	va_end( marker );
	
	pdststr = qmalloc(mem, nlen+sizeof(QCHR));
	if(pdststr == NULL)
	{
		return NULL;
	}
	qstrcpy(pdststr, 0, vbuffer, 0);
	
	return pdststr;
}

QINT qstr2int(QINT system, QPNT str, QINT *len)
{
	QUCHR chchar, *pchar;
	QINT nint, nlen, nsize, nminus, nsystem;
	
	nint = 0;
	if(len == NULL)
	{
		nsize = 0;
	}
	else
	{
		nsize = *len;
	}
	nlen = 0;
	pchar = (QUCHR *)str;
	if(pchar == NULL)
	{
		nminus = 0;
		goto GSTR2INT_EXIT;
	}
	if(*pchar == '-')
	{
		nminus = -1;
		pchar++;
		nlen++;
	}
	else
	{
		nminus = 1;
	}
	if(system > 0)
	{
		nsystem = system;
	}
	else
	{
		//  ÆΩ¯÷∆°£
		nsystem = 10;
	}
	if( (*pchar == '0') && (*(pchar+1) == 'x' || *(pchar+1) == 'X') )
	{
		//  Æ¡˘Ω¯÷∆°£
		nsystem = 16;
		pchar += 2;
		nlen += 2;
	}
	switch(nsystem)
	{
		case 16:
		{
			//  Æ¡˘Ω¯÷∆°£
			while(1)
			{
				if(nsize > 0 && nlen >= nsize)
				{
					break;
				}
				chchar = *pchar;
				if(chchar == 0)
				{
					break;
				}
				else if(chchar >= '0' && chchar <= '9')
				{
					nint = (nint<<4)+(chchar-'0');
				}
				else if(chchar >= 'a' && chchar <= 'f')
				{
					nint = (nint<<4)+0xa+(chchar-'a');
				}
				else if(chchar >= 'A' && chchar <= 'F')
				{
					nint = (nint<<4)+0xA+(chchar-'A');
				}
				else
				{
					break;
				}
				pchar++;
				nlen++;
			}
		}
			break;
			
		case 10:
		{
			//  ÆΩ¯÷∆°£
			while(1)
			{
				if(nsize > 0 && nlen >= nsize)
				{
					break;
				}
				chchar = *pchar;
				if(chchar == 0)
				{
					break;
				}
				else if(chchar >= '0' && chchar <= '9')
				{
					nint = (nint*10)+(chchar-'0');
				}
				else
				{
					break;
				}
				pchar++;
				nlen++;
			}
		}
			break;
			
		default:
		{
			// 36Ω¯÷∆÷Æƒ⁄°£
			while(1)
			{
				if(nsize > 0 && nlen >= nsize)
				{
					break;
				}
				chchar = *pchar;
				if(chchar == 0)
				{
					break;
				}
				else if(chchar >= '0' && chchar <= '9')
				{
					nint = (nint*nsystem)+(chchar-'0');
				}
				else if(chchar >= 'a' && chchar <= 'z')
				{
					nint = (nint*nsystem)+0xa+(chchar-'a');
				}
				else if(chchar >= 'A' && chchar <= 'Z')
				{
					nint = (nint*nsystem)+0xA+(chchar-'A');
				}
				else
				{
					break;
				}
				pchar++;
				nlen++;
			}
		}
			break;
	}
	
GSTR2INT_EXIT:
	if(len != NULL)
	{
		*len = nlen;
	}
	
	return nint*nminus;
}

QINT qstrcmp(QINT flag, QPNT str1, QPNT str2, QINT size)
{
	QINT ncount;
	QUCHR chdst, chsrc, *pdst, *psrc;
	
	ncount = 0;
	pdst = (QUCHR *)str1;
	psrc = (QUCHR *)str2;
	while(pdst && psrc)
	{
		chdst = *pdst;
		chsrc = *psrc;
		if(size > 0)
		{
			// ÷∏∂®±»Ωœ≥§∂»≈–∂œ±»ΩœΩ· ¯°£
			if(ncount >= size)
			{
				return 0;
			}
		}
		else
		{
			// “‘0Ω·Œ≤◊‘∂Ø≈–∂œ±»ΩœΩ· ¯°£
			if(chdst == 0 && chsrc == 0)
			{
				return 0;
			}
			else if(chdst == 0)
			{
				return 1;
			}
			else if(chsrc == 0)
			{
				return 1;
			}
		}
		if(chdst != chsrc)
		{
			if(flag == 0)
			{
				// ≤ª∫ˆ¬‘¥Û–°–¥±»Ωœ°£
				return 1;
			}
			// ∫ˆ¬‘¥Û–°–¥±»Ωœ°£
			if(chdst >= 'a' && chdst <= 'z')
			{
				chdst -= 'a';
			}
			else if(chdst >= 'A' && chdst <= 'Z')
			{
				chdst -= 'A';
			}
			else
			{
				return 1;
			}
			if(chsrc >= 'a' && chsrc <= 'z')
			{
				chsrc -= 'a';
			}
			else if(chsrc >= 'A' && chsrc <= 'Z')
			{
				chsrc -= 'A';
			}
			else
			{
				return 1;
			}
			if(chdst != chsrc)
			{
				return 1;
			}
		}
		pdst++;
		psrc++;
		ncount++;
	}
	if(size > 0)
	{
		if(ncount == size)
		{
			return 0;
		}
	}
	
	return 1;
}

QPNT qstrchr(QINT flag, QPNT str, QINT size, QINT chr)
{
	QINT npos;
	QUCHR *pchar, ndst, nsrc;
	
	if(flag == 0)
	{
		nsrc = (QUCHR)chr;
	}
	else
	{
		if(chr >= 'A' && chr <= 'Z')
		{
			nsrc = (QUCHR)('a'+(chr-'A'));
		}
		else
		{
			nsrc = (QUCHR)chr;
		}
	}
	npos = 0;
	pchar = (QUCHR *)str;
	while(pchar)
	{
		if(size > 0 && npos >= size)
		{
			break;
		}
		ndst = *pchar;
		if(ndst == 0)
		{
			break;
		}
		if(flag != 0)
		{
			if(ndst >= 'A' && ndst <= 'Z')
			{
				ndst = 'a'+(ndst-'A');
			}
		}
		if(ndst == nsrc)
		{
			return (QPNT )pchar;
		}
		pchar++;
		npos++;
	}
	
	return NULL;
}

QPNT qstrrchr(QINT flag, QPNT str, QINT size, QINT chr)
{
	QINT npos;
	QUCHR *pchar, ndst, nsrc;
	
	if(flag == 0)
	{
		nsrc = (QUCHR)chr;
	}
	else
	{
		if(chr >= 'A' && chr <= 'Z')
		{
			nsrc = (QUCHR)('a'+(chr-'A'));
		}
		else
		{
			nsrc = (QUCHR)chr;
		}
	}
	if(size > 0)
	{
		npos = size-1;
	}
	else
	{
		npos = qstrlen(str)-1;
	}
	if(npos < 0)
	{
		return NULL;
	}
	pchar = (QUCHR *)str+npos;
	while(npos >= 0)
	{
		if(size > 0 && npos >= size)
		{
			break;
		}
		ndst = *pchar;
		if(flag != 0)
		{
			if(ndst >= 'A' && ndst <= 'Z')
			{
				ndst = 'a'+(ndst-'A');
			}
		}
		if(ndst == nsrc)
		{
			return (QPNT )pchar;
		}
		pchar--;
		npos--;
	}
	
	return NULL;
}

QPNT qstrstr(QINT flag, QPNT str, QINT size, QPNT mode)
{
	QINT npos;
	QUCHR chrstr, chrmode;
	QUCHR *pstart, *pstr, *pmode;
	
	pstr = (QUCHR *)str;
	pmode = (QUCHR *)mode;
	if(pstr == NULL || mode == NULL)
	{
		return NULL;
	}
	if(*pstr == 0 || *pmode == 0)
	{
		return NULL;
	}
	npos = 0;
	pstart = (QUCHR *)str;
	while(*pstart)
	{
		if(size > 0 && npos >= size)
		{
			break;
		}
		pstr = pstart;
		pmode = (QUCHR *)mode;
		while(1)
		{
			chrstr = *pstr++; chrmode = *pmode++;
			if(flag != 0)
			{
				if(chrstr >= 'A' && chrstr <= 'Z')
				{
					chrstr = 'a'+(chrstr-'A');
				}
				if(chrmode >= 'A' && chrmode <= 'Z')
				{
					chrmode = 'a'+(chrmode-'A');
				}
			}
			if(chrmode == 0) { return pstart; }
			if(chrstr == 0) { break; }
			if(chrstr != chrmode) { break; }
		}
		pstart++;
		npos++;
	}
	
	return NULL;
}

QINT qstrstrip(QPNT dst, QINT dstsize, QPNT src, QINT srcsize)
{
	QINT nlen, npos, nstart;
	QUCHR chsrc, *pdst, *psrc;
	
	if(dst == NULL && src == NULL)
	{
		return -1;
	}
	// »•µÙŒ≤≤øø…∫ˆ¬‘◊÷∑˚°£
	nlen = srcsize;
	psrc = (QUCHR *)src+nlen-1;
	while(nlen > 1)
	{
		chsrc = *(psrc-1);
		if( (chsrc > 0x80) && (chsrc < 0xff) )
		{
			break;
		}
		else
		{
			if(*psrc != ' ' && *psrc != '\t' && *psrc != '\r' && *psrc != '\n')
			{
				break;
			}
			psrc--;
			nlen--;
		}
	}
	// »•µÙÕ∑≤øø…∫ˆ¬‘◊÷∑˚°£
	nstart = 0;
	psrc = (QUCHR *)src;
	while(nstart < nlen)
	{
		chsrc = *psrc;
		if( (chsrc > 0x80) && (chsrc < 0xff) )
		{
			break;
		}
		else
		{
			if(*psrc != ' ' && *psrc != '\t' && *psrc != '\r' && *psrc != '\n')
			{
				break;
			}
			nstart++;
			psrc++;
		}
	}
	if(nstart <= 0)
	{
		// Õ∑≤ø√ª”–ø…∫ˆ¬‘◊÷∑˚°£
		if(dst == NULL)
		{
			*((QUCHR *)src+nlen) = 0;
		}
		else if(dst == src)
		{
			*((QUCHR *)src+nlen) = 0;
		}
		else
		{
			if(dstsize > nlen)
			{
				qstrcpy(dst, 0, src, nlen);
				*((QUCHR *)dst+nlen) = 0;
			}
			else
			{
				qstrcpy(dst, 0, src, dstsize);
				nlen = dstsize;
			}
		}
		
		return nlen;
	}
	else
	{
		// Õ∑≤ø”–ø…∫ˆ¬‘◊÷∑˚£¨–Ë“™«∞“∆°£
		if(dst == NULL)
		{
			pdst = (QUCHR *)src;
		}
		else
		{
			if(dstsize <= nlen-nstart)
			{
				nlen = dstsize;
			}
			pdst = (QUCHR *)dst;
		}
		psrc = (QUCHR *)src+nstart;
		npos = nstart;
		while(npos < nlen)
		{
			*pdst++ = *psrc++;
			npos++;
		}
		if(dstsize > npos-nstart)
		{
			*pdst = 0;
		}
		
		return nlen-nstart;
	}
	
	return 0;
}

QINT qstrword(QPNT str, QUCHR chr, QINT index, QPNT word, QINT size)
{
	QINT ncount, nindex, nlen;
	QUCHR chsep, chchar, *pchar, *pstart, *pend;
	
	if(chr == 0)
	{
		chsep = ',';
	}
	else
	{
		chsep = chr;
	}
	if(word == NULL)
	{
		ncount = 0;
		pchar = (QUCHR *)str;
		while(pchar)
		{
			if(*pchar == 0)
			{
				break;
			}
			if(*pchar == chsep)
			{
				ncount++;
			}
			chchar = *pchar;
			if( (chchar > 0x80) && (chchar < 0xff) )
			{
				pchar += 2;
			}
			else
			{
				pchar += 1;
			}
		}
		
		return ncount+1;
	}
	else
	{
		*((QUCHR *)word) = 0;
		if(index < 0)
		{
			nindex = 0;
		}
		else
		{
			nindex = index;
		}
		ncount = 0;
		pchar = (QUCHR *)str;
		pstart = (QUCHR *)str;
		while(pchar)
		{
			if(*pchar == chsep || *pchar == 0)
			{
				if(ncount == nindex)
				{
					pchar--;
					while(pstart < pchar)
					{
						if(*pstart != ' ' && *pstart != '\t' && *pstart != '\r' && *pstart != '\n')
						{
							break;
						}
						pstart++;
					}
					nlen = pchar-pstart+1;
					if(nlen >= size)
					{
						qstrcpy(word, size, pstart, size);
						nlen = size;
					}
					else
					{
						qstrcpy(word, size, pstart, nlen);
						*((QUCHR *)word+nlen) = 0;
					}
					if(nlen > 0)
					{
						pend = (QUCHR *)word+nlen-1;
						while(pend > (QUCHR *)word)
						{
							if(*pend != ' ' && *pend != '\t' && *pstart != '\r' && *pstart != '\n')
							{
								break;
							}
							*pend = 0;
							pend--;
							nlen--;
						}
					}
					
					return nlen;
				}
				pstart = pchar+1;
				ncount++;
			}
			if(*pchar == 0)
			{
				break;
			}
			chchar = *pchar;
			if( (chchar > 0x80) && (chchar < 0xff) )
			{
				pchar += 2;
			}
			else
			{
				pchar += 1;
			}
		}
		
		return -1;
	}
	
	return -1;
}

QINT qstrprm(QPNT str, QUCHR chr, QINT index, QPNT param, QINT size)
{
	QUCHR chsep, chchar, *pchar, *pstart, *pend;
	QINT ncount, nindex, nlen, nbracket, nquotation;
	
	if(chr == 0)
	{
		chsep = ',';
	}
	else
	{
		chsep = chr;
	}
	nbracket = 0;
	nquotation = 0;
	if(param == NULL)
	{
		ncount = 0;
		pchar = (QUCHR *)str;
		while(pchar)
		{
			if(*pchar == 0)
			{
				break;
			}
			if(*pchar == '\'')
			{
				// ≥ˆœ÷“˝∫≈°£
				if(nquotation == 1)
				{
					// “˝∫≈Ω· ¯°£
					nquotation = 0;
				}
				else
				{
					// “˝∫≈ø™ º°£
					nquotation = 1;
				}
			}
			else if(nquotation == 0)
			{
				// ≤ª‘⁄“˝∫≈¥Æ÷Æƒ⁄°£
				if(*pchar == '(')
				{
					// ¿®∫≈ø™ º°£
					nbracket++;
				}
				else if(*pchar == ')')
				{
					// ¿®∫≈Ω· ¯°£
					nbracket--;
				}
			}
			if(nquotation == 0 && nbracket == 0)
			{
				// ≤ª‘⁄“˝∫≈¥Æ∫Õ¿®∫≈ƒ⁄°£
				if(*pchar == chsep)
				{
					ncount++;
				}
			}
			if(*pchar == '\\')
			{
				pchar++;
			}
			chchar = *pchar;
			if( (chchar > 0x80) && (chchar < 0xff) )
			{
				pchar += 2;
			}
			else
			{
				pchar += 1;
			}
		}
		
		return ncount+1;
	}
	else
	{
		*((QUCHR *)param) = 0;
		if(index < 0)
		{
			nindex = 0;
		}
		else
		{
			nindex = index;
		}
		ncount = 0;
		pchar = (QUCHR *)str;
		pstart = (QUCHR *)str;
		while(pchar)
		{
			if(*pchar == '\'')
			{
				// ≥ˆœ÷“˝∫≈°£
				if(nquotation == 1)
				{
					// “˝∫≈Ω· ¯°£
					nquotation = 0;
				}
				else
				{
					// “˝∫≈ø™ º°£
					nquotation = 1;
				}
			}
			else if(nquotation == 0)
			{
				// ≤ª‘⁄“˝∫≈¥Æ÷Æƒ⁄°£
				if(*pchar == '(')
				{
					// ¿®∫≈ø™ º°£
					nbracket++;
				}
				else if(*pchar == ')')
				{
					// ¿®∫≈Ω· ¯°£
					nbracket--;
				}
			}
			if(nquotation == 0 && nbracket == 0)
			{
				// ≤ª‘⁄“˝∫≈¥Æ∫Õ¿®∫≈ƒ⁄°£
				if(*pchar == chsep || *pchar == 0)
				{
					if(ncount == nindex)
					{
						pchar--;
						while(pstart < pchar)
						{
							if(*pstart != ' ' && *pstart != '\t' && *pstart != '\r' && *pstart != '\n')
							{
								break;
							}
							pstart++;
						}
						nlen = pchar-pstart+1;
						if(nlen >= size)
						{
							qstrcpy(param, size, pstart, size);
							nlen = size;
						}
						else
						{
							qstrcpy(param, size, pstart, nlen);
							*((QUCHR *)param+nlen) = 0;
						}
						if(nlen > 0)
						{
							pend = (QUCHR *)param+nlen-1;
							while(pend > (QUCHR *)param)
							{
								if(*pend != ' ' && *pend != '\t' && *pstart != '\r' && *pstart != '\n')
								{
									break;
								}
								*pend = 0;
								pend--;
								nlen--;
							}
						}
						
						return nlen;
					}
					pstart = pchar+1;
					ncount++;
				}
			}
			if(*pchar == 0)
			{
				break;
			}
			if(*pchar == '\\')
			{
				pchar++;
			}
			chchar = *pchar;
			if( (chchar > 0x80) && (chchar < 0xff) )
			{
				pchar += 2;
			}
			else
			{
				pchar += 1;
			}
		}
		
		return -1;
	}
	
	return -1;
}

QINT qstrprmcbx(QPNT str, QUCHR chr, qstr_prm_cb prm_cb, QPNT params[], QINT count)
{
	QUCHR chsep, *pchar, *pstart, *pend;
	QINT ncount, nindex, nresult, nlen, nbracket, nquotation;
	
	if(chr == 0)
	{
		chsep = ',';
	}
	else
	{
		chsep = chr;
	}
	nbracket = 0;
	nquotation = 0;
	if(prm_cb == NULL)
	{
		ncount = 0;
		pchar = (QUCHR *)str;
		while(pchar)
		{
			if(*pchar == 0)
			{
				break;
			}
			if(*pchar == '\'')
			{
				// ≥ˆœ÷“˝∫≈°£
				if(nquotation == 1)
				{
					// “˝∫≈Ω· ¯°£
					nquotation = 0;
				}
				else
				{
					// “˝∫≈ø™ º°£
					nquotation = 1;
				}
			}
			else if(nquotation == 0)
			{
				// ≤ª‘⁄“˝∫≈¥Æ÷Æƒ⁄°£
				if(*pchar == '(')
				{
					// ¿®∫≈ø™ º°£
					nbracket++;
				}
				else if(*pchar == ')')
				{
					// ¿®∫≈Ω· ¯°£
					nbracket--;
				}
			}
			if(nquotation == 0 && nbracket == 0)
			{
				// ≤ª‘⁄“˝∫≈¥Æ∫Õ¿®∫≈ƒ⁄°£
				if(*pchar == chsep)
				{
					ncount++;
				}
			}
			if(*pchar == '\\')
			{
				pchar++;
			}
			pchar++;
			/*nchar = *pchar;
			 nchar &= 0x000000ff;
			 if( (nchar > 0x80) && (nchar < 0xff) )
			 {
				pchar += 2;
			 }
			 else
			 {
				pchar += 1;
			 }*/
		}
		
		return ncount+1;
	}
	else
	{
		ncount = 0;
		nindex = 0;
		pchar = (QUCHR *)str;
		pstart = (QUCHR *)str;
		while(pchar)
		{
			if(*pchar == '\'')
			{
				// ≥ˆœ÷“˝∫≈°£
				if(nquotation == 1)
				{
					// “˝∫≈Ω· ¯°£
					nquotation = 0;
				}
				else
				{
					// “˝∫≈ø™ º°£
					nquotation = 1;
				}
			}
			else if(nquotation == 0)
			{
				// ≤ª‘⁄“˝∫≈¥Æ÷Æƒ⁄°£
				if(*pchar == '(')
				{
					// ¿®∫≈ø™ º°£
					nbracket++;
				}
				else if(*pchar == ')')
				{
					// ¿®∫≈Ω· ¯°£
					nbracket--;
				}
			}
			if(nquotation == 0 && nbracket == 0)
			{
				// ≤ª‘⁄“˝∫≈¥Æ∫Õ¿®∫≈ƒ⁄°£
				if(*pchar == chsep || *pchar == 0)
				{
					while(pstart < pchar)
					{
						if(*pstart != ' ' && *pstart != '\t' && *pstart != '\r' && *pstart != '\n')
						{
							break;
						}
						pstart++;
					}
					nlen = pchar-1-pstart+1;
					if(nlen > 0)
					{
						pend = pchar-1;
						while(pend > pstart)
						{
							if(*pend != ' ' && *pend != '\t' && *pstart != '\r' && *pstart != '\n')
							{
								break;
							}
							pend--;
							nlen--;
						}
					}
					nresult = prm_cb(str, chr, nindex, pstart, nlen, params, count);
					if(nresult == 0)
					{
						break;
					}
					ncount++;
					pstart = pchar+1;
					nindex++;
				}
			}
			if(*pchar == 0)
			{
				break;
			}
			if(*pchar == '\\')
			{
				pchar++;
			}
			pchar++;
			/*nchar = *pchar;
			 nchar &= 0x000000ff;
			 if( (nchar > 0x80) && (nchar < 0xff) )
			 {
				pchar += 2;
			 }
			 else
			 {
				pchar += 1;
			 }*/
		}
		
		return ncount;
	}
	
	return -1;
}

QINT qstrprmcb(QPNT str, QUCHR chr, qstr_prm_cb prm_cb, QINT count, ...)
{
	QINT nlen;
	QPNT *ppprms;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;
	
	va_start(marker, count);
	ppprms = (QPNT *)qprmmakev(NULL, vbuff, sizeof(vbuff), NULL, marker, &count);
	va_end(marker);
	
	nlen = qstrprmcbx(str, chr, prm_cb, ppprms, count);
	
	qprmfree(ppprms);
	
	return nlen;
}

QPNT qstrnext(QPNT str, QINT *size)
{
	if(str == NULL)
	{
		return NULL;
	}
	if(*((QUCHR *)str) == 0)
	{
		if(size != NULL)
		{
			*size = 0;
		}
		
		return NULL;
	}
	else if(*((QUCHR *)str) <= 0x80)
	{
		if(size != NULL)
		{
			*size = 1;
		}
		
		return ((QUCHR *)str+1);
	}
	else
	{
		if(size != NULL)
		{
			*size = 2;
		}
		
		return ((QUCHR *)str+2);
	}
}

QINT qstrfetch(QSTR str, QINT size, QINT *flag, QSTR *read, QINT *len)
{
	QUCHR chchar, *pbegin, *pdstchar, *pdstread, *psrcread;
	QINT nflag, nend, nlen, ndstpos, ndstflag, ndstlen, nsrcflag, nsrclen;
	
	// 参数初始化。
	nsrcflag = QSTR_READ_ASC;
	if(flag != NULL)
	{
		if(*flag != QSTR_READ_NONE)
		{
			nsrcflag = *flag;
		}
	}
	if(read == NULL)
	{
		psrcread = NULL;
	}
	else
	{
		psrcread = (QUCHR *)(*read);
	}
	if(len == NULL)
	{
		nsrclen = 0;
	}
	else
	{
		nsrclen = *len;
	}
	ndstpos = 0;
	pbegin = (QUCHR *)str;
	goto READ_BEGIN;
	
READ_BEGIN:
	// 开始读取。
	ndstflag = QSTR_READ_NONE;
	pdstread = NULL;
	ndstlen = 0;
	if(pbegin == NULL)
	{
		goto READ_EXIT;
	}
	// 过滤格式符。
	nend = 0;
	chchar = 0;
	pdstchar = pbegin;
	while(pdstchar)
	{
		if(size > 0)
		{
			if(ndstpos >= size)
			{
				nend = 1;
				break;
			}
		}
		chchar = *pdstchar;
		if(chchar == 0)
		{
			nend = 1;
			break;
		}
		if(chchar != ' ' && chchar != '\t' && chchar != '\r' && chchar != '\n')
		{
			break;
		}
		pdstchar++;
		ndstpos++;
	}
	if(nend == 1)
	{
		goto READ_EXIT;
	}
	// 初步判断类型。
	if(chchar == '0')
	{
		if(*(pdstchar+1) == 'x' || *(pdstchar+1) == 'X')
		{
			// 十六进制。
			ndstflag = QSTR_READ_HEX;
		}
		else
		{
			// 十进制。
			ndstflag = QSTR_READ_DEC;
		}
	}
	else if(chchar >= '0' && chchar <= '9')
	{
		// 十进制。
		ndstflag = QSTR_READ_DEC;
	}
	else if( (chchar >= 'a' && chchar <= 'z') || (chchar >= 'A' && chchar <= 'Z') || (chchar == '_') )
	{
		// ASC词语。
		ndstflag = QSTR_READ_AWD;
	}
	else if( (chchar > 0x80) && (chchar < 0xff) )
	{
		// UNICODE词语。
		ndstflag = QSTR_READ_UWD;
	}
	else
	{
		// 符号。
		ndstflag = QSTR_READ_SYM;
		if(chchar == '\'' || chchar == '"')
		{
			if(nsrcflag & QSTR_READ_STR)
			{
				// 字符串。
				ndstflag = QSTR_READ_STR;
			}
		}
		else if(chchar == '$')
		{
			if(nsrcflag & QSTR_READ_PRM)
			{
				// 参数。
				ndstflag = QSTR_READ_PRM;
			}
		}
		else if(chchar == '@')
		{
			if(nsrcflag & QSTR_READ_VAL)
			{
				// 取值。
				ndstflag = QSTR_READ_VAL;
			}
		}
		else if(chchar == '.' || chchar == '/')
		{
			if(nsrcflag & QSTR_READ_URL)
			{
				// URL串。
				ndstflag = QSTR_READ_URL;
			}
		}
		else if(chchar == '\\')
		{
			if(*(pdstchar+1) != '.' && *(pdstchar+1) != '\\')
			{
				if(nsrcflag & QSTR_READ_URL)
				{
					// URL串。
					ndstflag = QSTR_READ_URL;
				}
			}
		}
	}
	nend = 0;
	chchar = 0;
	switch(ndstflag)
	{
		case QSTR_READ_DEC:
			// 十进制数值。
			pdstread = pdstchar;
			while(pdstchar)
			{
				if(size > 0)
				{
					if(ndstpos >= size)
					{
						nend = 1;
						break;
					}
				}
				chchar = *pdstchar;
				if(chchar == 0)
				{
					nend = 1;
					break;
				}
				if(psrcread != NULL)
				{
					if(!qstrcmp((nsrcflag&QSTR_READ_NCS), pdstchar, psrcread, nsrclen))
					{
						nend = 1;
						break;
					}
				}
				if(chchar < '0' || chchar > '9')
				{
					break;
				}
				pdstchar++;
				ndstpos++;
				ndstlen++;
			}
			break;
			
		case QSTR_READ_HEX:
			// 十六进制数值。
			pdstread = pdstchar;
			pdstchar += 2;
			ndstpos += 2;
			ndstlen += 2;
			while(pdstchar)
			{
				if(size > 0)
				{
					if(ndstpos >= size)
					{
						nend = 1;
						break;
					}
				}
				chchar = *pdstchar;
				if(chchar == 0)
				{
					nend = 1;
					break;
				}
				if(psrcread != NULL)
				{
					if(!qstrcmp((nsrcflag&QSTR_READ_NCS), pdstchar, psrcread, nsrclen))
					{
						nend = 1;
						break;
					}
				}
				nflag = 0;
				if(chchar >= '0' && chchar <= '9')
				{
					nflag = 1;
				}
				else if(chchar >= 'a' && chchar <= 'f')
				{
					nflag = 1;
				}
				else if(chchar >= 'A' && chchar <= 'F')
				{
					nflag = 1;
				}
				if(nflag == 0)
				{
					break;
				}
				pdstchar++;
				ndstpos++;
				ndstlen++;
			}
			break;
			
		case QSTR_READ_AWD:
			// ASC词语。
			pdstread = pdstchar;
			while(pdstchar)
			{
				if(size > 0)
				{
					if(ndstpos >= size)
					{
						nend = 1;
						break;
					}
				}
				chchar = *pdstchar;
				if(chchar == 0)
				{
					nend = 1;
					break;
				}
				if(psrcread != NULL)
				{
					if(!qstrcmp((nsrcflag&QSTR_READ_NCS), pdstchar, psrcread, nsrclen))
					{
						nend = 1;
						break;
					}
				}
				nflag = 0;
				if(chchar == '_')
				{
					nflag = 1;
				}
				else if(chchar >= '0' && chchar <= '9')
				{
					nflag = 1;
				}
				else if(chchar >= 'a' && chchar <= 'z')
				{
					nflag = 1;
				}
				else if(chchar >= 'A' && chchar <= 'Z')
				{
					nflag = 1;
				}
				if(nflag == 0)
				{
					break;
				}
				pdstchar++;
				ndstpos++;
				ndstlen++;
			}
			break;
			
		case QSTR_READ_UWD:
			// UNICODE词语。
			pdstread = pdstchar;
			while(pdstchar)
			{
				if(size > 0)
				{
					if(ndstpos >= size)
					{
						nend = 1;
						break;
					}
				}
				chchar = *pdstchar;
				if(chchar == 0)
				{
					nend = 1;
					break;
				}
				if(psrcread != NULL)
				{
					if(!qstrcmp((nsrcflag&QSTR_READ_NCS), pdstchar, psrcread, nsrclen))
					{
						nend = 1;
						break;
					}
				}
				nlen = 1;
				nflag = 0;
				if(chchar == '_')
				{
					nflag = 1;
				}
				else if(chchar >= '0' && chchar <= '9')
				{
					nflag = 1;
				}
				else if(chchar >= 'a' && chchar <= 'z')
				{
					nflag = 1;
				}
				else if(chchar >= 'A' && chchar <= 'Z')
				{
					nflag = 1;
				}
				else if(chchar > 0x80 && chchar < 0xff)
				{
					nflag = 1;
					if(size > 0)
					{
						if(ndstpos < size)
						{
							if(*(pdstchar+1) != 0)
							{
								nlen = 2;
							}
						}
					}
					else
					{
						if(*(pdstchar+1) != 0)
						{
							nlen = 2;
						}
						
					}
				}
				if(nflag == 0)
				{
					break;
				}
				pdstchar += nlen;
				ndstpos += nlen;
				ndstlen += nlen;
			}
			break;
			
		case QSTR_READ_SYM:
			// 符号。
			chchar = *pdstchar;
			if(chchar == 0)
			{
				nend = 1;
				break;
			}
			pdstread = pdstchar;
			if(psrcread != NULL)
			{
				if(!qstrcmp((nsrcflag&QSTR_READ_NCS), pdstchar, psrcread, nsrclen))
				{
					nend = 1;
					break;
				}
			}
			nlen = 1;
			if(chchar > 0x80 && chchar < 0xff)
			{
				if(size > 0)
				{
					if(ndstpos < size)
					{
						if(*(pdstchar+1) != 0)
						{
							nlen = 2;
						}
					}
				}
				else
				{
					if(*(pdstchar+1) != 0)
					{
						nlen = 2;
					}
					
				}
			}
			ndstpos += nlen;
			ndstlen += nlen;
			break;
			
		case QSTR_READ_STR:
			// 字符串。
			pdstread = pdstchar+1;
			while(pdstchar)
			{
				if(size > 0)
				{
					if(ndstpos >= size)
					{
						nend = 1;
						break;
					}
				}
				chchar = *pdstchar;
				if(chchar == 0)
				{
					nend = 1;
					break;
				}
				if(psrcread != NULL)
				{
					if(!qstrcmp((nsrcflag&QSTR_READ_NCS), pdstchar, psrcread, nsrclen))
					{
						nend = 1;
						break;
					}
				}
				if(pdstchar != pdstread && chchar == *pdstread)
				{
					pdstchar++;
					ndstpos++;
					break;
				}
				pdstchar++;
				ndstpos++;
				ndstlen++;
			}
			break;
			
		case QSTR_READ_PRM:
		case QSTR_READ_VAL:
			// 参数、取值。
			pdstchar++;
			ndstpos++;
			ndstlen++;
			if(*pdstchar == '(')
			{
				// 带括号。
				pdstread = pdstchar+1;
				pdstchar++;
				ndstpos++;
				ndstlen++;
				nflag = 1;
				while(pdstchar)
				{
					if(size > 0)
					{
						if(ndstpos >= size)
						{
							nend = 1;
							break;
						}
					}
					chchar = *pdstchar;
					if(chchar == 0)
					{
						nend = 1;
						break;
					}
					if(psrcread != NULL)
					{
						if(!qstrcmp((nsrcflag&QSTR_READ_NCS), pdstchar, psrcread, nsrclen))
						{
							nend = 1;
							break;
						}
					}
					if(chchar == '(')
					{
						nflag++;
					}
					else if(chchar == ')')
					{
						nflag--;
					}
					if(nflag == 0)
					{
						pdstchar++;
						ndstpos++;
						break;
					}
					pdstchar++;
					ndstpos++;
					ndstlen++;
				}
			}
			else
			{
				// 不带括号。
				pdstread = pdstchar;
				while(pdstchar)
				{
					if(size > 0)
					{
						if(ndstpos >= size)
						{
							nend = 1;
							break;
						}
					}
					chchar = *pdstchar;
					if(chchar == 0)
					{
						nend = 1;
						break;
					}
					if(psrcread != NULL)
					{
						if(!qstrcmp((nsrcflag&QSTR_READ_NCS), pdstchar, psrcread, nsrclen))
						{
							nend = 1;
							break;
						}
					}
					nlen = 1;
					nflag = 0;
					if(chchar == '_')
					{
						nflag = 1;
					}
					else if(chchar >= '0' && chchar <= '9')
					{
						nflag = 1;
					}
					else if(chchar >= 'a' && chchar <= 'z')
					{
						nflag = 1;
					}
					else if(chchar >= 'A' && chchar <= 'Z')
					{
						nflag = 1;
					}
					else if(chchar > 0x80 && chchar < 0xff)
					{
						nflag = 1;
						if(size > 0)
						{
							if(ndstpos < size)
							{
								if(*(pdstchar+1) != 0)
								{
									nlen = 2;
								}
							}
						}
						else
						{
							if(*(pdstchar+1) != 0)
							{
								nlen = 2;
							}
							
						}
					}
					if(nflag == 0)
					{
						pdstchar += nlen;
						ndstpos += nlen;
						break;
					}
					pdstchar += nlen;
					ndstpos += nlen;
					ndstlen += nlen;
				}
			}
			break;
			
		case QSTR_READ_URL:
			// URL资源路径。
			pdstread = pdstchar;
			while(pdstchar)
			{
				if(size > 0)
				{
					if(ndstpos >= size)
					{
						nend = 1;
						break;
					}
				}
				chchar = *pdstchar;
				if(chchar == 0)
				{
					nend = 1;
					break;
				}
				if(psrcread != NULL)
				{
					if(!qstrcmp((nsrcflag&QSTR_READ_NCS), pdstchar, psrcread, nsrclen))
					{
						nend = 1;
						break;
					}
				}
				pdstchar++;
				ndstpos++;
				ndstlen++;
			}
			break;
			
		default:
			ndstflag = QSTR_READ_NONE;
			pdstread = NULL;
			ndstlen = 0;
			goto READ_EXIT;
			break;
	}
	if(ndstflag & nsrcflag)
	{
		// 存在匹配标志。
		goto READ_EXIT;
	}
	if(nend == 1)
	{
		// 获取结束。
		ndstflag = QSTR_READ_NONE;
		pdstread = NULL;
		ndstlen = 0;
		goto READ_EXIT;
	}
	// 读取下一字符串，直到标志匹配。
	pbegin = pdstchar;
	goto READ_BEGIN;
	
READ_EXIT:
	if(flag != NULL)
	{
		*flag = ndstflag;
	}
	if(read != NULL)
	{
		*read = pdstread;
	}
	if(len != NULL)
	{
		*len = ndstlen;
	}
	
	return ndstpos;
}

QPNT qstrmap(const struct qstr_map_item *map, QINT value)
{
	struct qstr_map_item *pitem;
	
	if(map == NULL)
	{
		return NULL;
	}
	pitem = (struct qstr_map_item *)map;
	while(pitem)
	{
		if(pitem->pstr == NULL)
		{
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

QINT qstrval(const struct qstr_map_item *map, int flag, QPNT str, QINT *count)
{
	QINT nlen, nsize, ncount, nvalue;
	QUCHR *pcur, *pnext, vvalue[QSTR_BUFF_SIZE];
	struct qstr_map_item *pitem;
	
	if(map == NULL || str == NULL)
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
		pitem = (struct qstr_map_item *)map;
		while(pitem)
		{
			if(pitem->pstr == NULL)
			{
				break;
			}
			if(!qstrcmp(flag, pitem->pstr, vvalue, 0))
			{
				ncount++;
				nvalue |= (QINT )pitem->nvalue;
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
	QINT nchar;
	QUCHR *pchar;
	
	item->nflag = 0;
	item->nparam = 0;
	item->nfillchar = 0;
	item->nfillsize = 0;
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
	
	return item->nfmtsize;
}
