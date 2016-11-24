
#include "qmdl_util.h"


#define F_OK									0


QINT qurlIsPath(QPNT url)
{
	QUCHR chchar, *pchar;
	
	pchar = (QUCHR *)url;
	while(pchar)
	{
		chchar = *pchar++;
		if(chchar == 0)
		{
			break;
		}
		if(chchar == '/' || chchar == '\\' || chchar == '~' || chchar == '?')
		{
			return QNO_TRUE;
		}
	}
	
	return QNO_FALSE;
}

QINT qurlAccess(QPNT url, QINT mode)
{
	return 1;
}

QINT qurlMergeFilePath(QPNT defpath, QPNT filepath, QPNT path, QINT size)
{
	QINT nlen, nsize, npathlen;
	QUCHR vpath[QSTR_BUFF_SIZE];
	QUCHR vbuffer[QSTR_BUFF_SIZE];
	QUCHR *pchar, *pfilepath, *psrcchar, *pdstchar;

	pfilepath = (QUCHR *)filepath;
	if(pfilepath == NULL)
	{
		return 0;
	}
	else if(defpath == NULL)
	{
		return qstrlen(pfilepath);
	}
	npathlen = qurlGetPath(defpath, vpath, sizeof(vpath));
	if(npathlen <= 0)
	{
		return qstrlen(pfilepath);
	}
	pchar = &vpath[npathlen-1];
	while(pchar != vpath)
	{
		if(*pchar == '/' || *pchar == '\\')
		{
			break;
		}
		*pchar = 0;
		pchar--;
		npathlen--;
	}
	while(pchar != vpath)
	{
		if(*pchar != '/' && *pchar != '\\')
		{
			break;
		}
		*pchar = 0;
		pchar--;
		npathlen--;
	}
	if(npathlen <= 0)
	{
		return qstrlen(pfilepath);
	}
	psrcchar = pfilepath;
	while(*psrcchar != 0)
	{
		if(*psrcchar == '/' || *psrcchar == '\\')
		{
			break;
		}
		psrcchar++;
	}
	while(*psrcchar != 0)
	{
		if(*psrcchar != '/' && *psrcchar != '\\')
		{
			break;
		}
		psrcchar++;
	}
	if(path == NULL)
	{
		nsize = sizeof(vbuffer);
		pdstchar = vbuffer;
	}
	else
	{
		nsize = size;
		pdstchar = (QUCHR * )path;
	}
	if(*psrcchar == 0)
	{
		// file-name.
		nlen = sprintf((char *)pdstchar, "%s/%s", vpath, pfilepath);
	}
	else
	{
		if(pfilepath[0] != '.')
		{
			if(pfilepath[0] == '/')
			{
				return qstrlen(pfilepath);
			}
			else
			{
				nlen = sprintf((char *)pdstchar, "%s/%s", vpath, pfilepath);
			}
		}
		else
		{
			if(pfilepath[1] == '.')
			{
				// ../file-name
				pchar = &vpath[npathlen-1];
				while(pchar != vpath)
				{
					if(*pchar == '/' || *pchar == '\\')
					{
						break;
					}
					*pchar = 0;
					pchar--;
					npathlen--;
				}
				while(pchar != vpath)
				{
					if(*pchar != '/' && *pchar != '\\')
					{
						break;
					}
					*pchar = 0;
					pchar--;
					npathlen--;
				}
				if(npathlen <= 0)
				{
					nlen = sprintf((char *)pdstchar, "/%s", psrcchar);
				}
				else
				{
					nlen = sprintf((char *)pdstchar, "%s/%s", vpath, psrcchar);
				}
			}
			else
			{
				// ./file-name
				nlen = sprintf((char *)pdstchar, "%s/%s", vpath, psrcchar);
			}
		}
	}
	if(path == NULL)
	{
		qstrcpy(pfilepath, size, vbuffer, 0);
	}

	return nlen;
}

QINT qurlExpandPath(QPNT path, QPNT url, QINT size)
{
	QINT nlen;
	QUCHR *pchar, *purl;
	QUCHR vpath[QSTR_BUFF_SIZE];
	QUCHR vbuffer[QSTR_BUFF_SIZE];

	if(qstrcmp(QSTR_NONE, (QPNT )url, (QPNT )"./", 2) || qstrcmp(QSTR_NONE, (QPNT )url, (QPNT )"../", 3))
	{
		// 替换当前路径。
		qstrcpy(vbuffer, sizeof(vbuffer), url, 0);
		purl = vbuffer;
		qurlGetPath(path, vpath, sizeof(vpath));
		nlen = qstrlen(vpath);
		pchar = (QUCHR *)qstrrchr(0, vpath, nlen, '/');
		if(pchar == NULL)
		{
			// 根路径。
			qstrcpy((QPNT )vpath, sizeof(vpath), (QPNT )"/", 0);
		}
		else
		{
			*pchar = 0;
		}
		nlen = qstrlen(vpath);
		if(qstrcmp(QSTR_NONE, (QPNT )purl, (QPNT )"./", 2))
		{
			// 当前路径。
			qstrcpy(url, size, vpath, 0);
			purl += 2;
		}
		else
		{
			// 上一级路径。
			pchar = (QUCHR *)qstrrchr(0, vpath, nlen, '/');
			if(pchar == NULL)
			{
				qstrcpy((QPNT )vpath, sizeof(vpath), (QPNT )"/", 0);
			}
			else
			{
				*pchar = 0;
			}
			nlen = qstrlen(vpath);
			qstrcpy(url, size, vpath, 0);
			purl += 3;
		}
		qstrcpy((QUCHR *)url+nlen, size-nlen, purl, 0);
		nlen += qstrlen(purl);
	}
	else
	{
		// 不是路径起始位置。
		nlen = qstrlen(url);
	}

	return nlen;
}

QINT qurlGetProtocol(QPNT url, QPNT protocol, QINT size)
{
	QINT nlen;
	QUCHR chchar, *pchar, *purl;

	purl = (QUCHR *)url;
	if(purl == NULL || protocol == NULL || size <= 0)
	{
		return -1;
	}
	pchar = (QUCHR * )url;
	while(pchar)
	{
		chchar = *pchar;
		if(chchar == 0 || chchar == '?' || chchar == ':')
		{
			break;
		}
		pchar++;
	}
	if(pchar == NULL || *pchar != ':')
	{
		((QUCHR *)protocol)[0] = 0;

		return 0;
	}
	nlen = pchar-purl;
	if( (nlen == 1) &&
		((purl[0] >= 'a' && purl[0] <= 'z') || (purl[0] >= 'A' && purl[0] <= 'Z'))
		)
	{
		return -1;
	}
	if(nlen < size)
	{
		qstrcpy(protocol, size, purl, nlen);
	}
	else
	{
		qstrcpy(protocol, size, purl, size-1);
		nlen = size-1;
	}
	((QUCHR *)protocol)[nlen] = 0;

	return nlen;
}

QINT qurlGetPath(QPNT url, QPNT path, QINT size)
{
	QINT nlen, nprotocol;
	QUCHR chchar, chnext, *pchar, *ppath, *purl;

	nlen = 0;
	nprotocol = 0;
	pchar = (QUCHR * )url;
	purl = (QUCHR *)url;
	ppath = (QUCHR *)path;
	while(pchar)
	{
		if(size > 0 && nlen >= size)
		{
			break;
		}
		chchar = *pchar;
		chnext = *(pchar+1);
		if(chchar == '?' || chchar == 0 || (size > 0 && nlen >= size))
		{
			break;
		}
		else if(chchar == '\\' && (chnext == '?' || chnext == ':') )
		{
			ppath[nlen] = chnext;
			pchar += 2;
			nlen++;
		}
		else if(nprotocol == 0 && chchar == ':')
		{
			// 只对第一次遇到的冒号做判断。
			nprotocol = 1;
			if( (nlen == 1) &&
				((purl[0] >= 'a' && purl[0] <= 'z') || (purl[0] >= 'A' && purl[0] <= 'Z'))
				)
			{
				// 冒号前仅有一个拉丁字母，则为盘符。
				((QUCHR * )path)[nlen] = chchar;
				pchar++;
				nlen++;
			}
			else
			{
				nlen = 0;
				pchar++;
				while(pchar)
				{
					if(*pchar != ':')
					{
						break;
					}
					pchar++;
				}
				if(*pchar == 0)
				{
					break;
				}
				((QUCHR * )path)[nlen] = *pchar;
				pchar++;
				nlen++;
			}
		}
		else
		{
			((QUCHR * )path)[nlen] = chchar;
			pchar++;
			nlen++;
		}
	}
	if( (size <= 0) || (size > 0 && nlen < size) )
	{
		((QUCHR * )path)[nlen] = 0;
	}

	return nlen;
}

QINT qurlGetFile(QPNT url, QPNT file, QINT size)
{
	QINT nlen, naccess;
	QUCHR chchar, *pchar, *pfile;

	nlen = qurlGetPath(url, file, size);
	if(nlen <= 0)
	{
		return nlen;
	}
	if(qurlAccess(file, F_OK) == 0)
	{
		return nlen;
	}
	naccess = 0;
	pfile = (QUCHR * )file;
	pchar = pfile+nlen-1;
	while(pchar != pfile)
	{
		chchar = *pchar;
		if(chchar == '\\' || chchar == '/')
		{
			break;
		}
		else if(chchar == '.')
		{
			*pchar = 0;
			if(qurlAccess(file, F_OK) == 0)
			{
				naccess = 1;
				break;
			}
		}
		pchar--;
	}
	if(naccess == 0 || pchar == pfile)
	{
		((QUCHR * )file)[0] = 0;

		return 0;
	}

	return (QINT )(pchar-pfile);
}

QINT qurlGetDir(QPNT url, QPNT dir, QINT size)
{
	QINT nlen;
	QUCHR chchar, *pchar, *pdir;

	nlen = qurlGetPath(url, dir, size);
	if(nlen <= 0)
	{
		return nlen;
	}
	pdir = (QUCHR * )dir;
	pchar = pdir+nlen-1;
	while(pchar != pdir)
	{
		chchar = *pchar;
		if(chchar == '\\' || chchar == '/')
		{
			*pchar = 0;
			break;
		}
		pchar--;
	}
	if(pchar == pdir)
	{
		((QUCHR * )dir)[0] = 0;

		return 0;
	}

	return (QINT )(pchar-pdir);
}

QINT qurlGetDomainName(QPNT url, QPNT name, QINT size)
{
	QINT nlen;
	QUCHR *pchar, *pname, chchar;

	if(url == NULL || name == NULL || size <= 0)
	{
		return -1;
	}
	nlen = 0;
	pchar = (QUCHR * )url;
	pname = (QUCHR * )name;
	while(pchar)
	{
		if(size > 0 && nlen >= size)
		{
			break;
		}
		chchar = *pchar;
		if(chchar == 0)
		{
			break;
		}
		else if(chchar == ':')
		{
			*pname++ = chchar;
			nlen++;
			pchar++;
			while(pchar)
			{
				if(size > 0 && nlen >= size)
				{
					break;
				}
				chchar = *pchar;
				*pname++ = chchar;
				nlen++;
				if(chchar == 0)
				{
					break;
				}
				else if(chchar != '\\' && chchar != '/')
				{
					break;
				}
				pchar++;
			}
			if(chchar == 0)
			{
				break;
			}
		}
		else if(chchar == '\\' || chchar == '/')
		{
			*pname = 0;
			break;
		}
		else
		{
			*pname++ = chchar;
			nlen++;
		}
		pchar++;
	}

	return nlen;
}

QINT qurlGetName(QPNT url, QPNT name, QINT size)
{
	QINT nlen;
	QUCHR vbuffer[QSTR_BUFF_SIZE];
	QUCHR chchar, *pchar, *pext;

	nlen = qurlGetFile(url, vbuffer, sizeof(vbuffer));
	if(nlen <= 0)
	{
		((QUCHR * )name)[0] = 0;

		return 0;
	}
	pext = NULL;
	pchar = vbuffer+nlen-1;
	while(pchar != vbuffer)
	{
		chchar = *pchar;
		if(chchar == '\\' || chchar == '/')
		{
			break;
		}
		else if(chchar == '.')
		{
			if(pext == NULL)
			{
				pext = pchar;
			}
		}
		pchar--;
	}
	if(pext != NULL)
	{
		*pext = 0;
	}
	if(*pchar == '\\' || *pchar == '/')
	{
		qstrcpy(name, size, pchar+1, 0);
		nlen = (QINT )(vbuffer+nlen-pchar-1);
	}
	else
	{
		qstrcpy(name, size, pchar, 0);
	}

	return nlen;
}

QINT qurlGetExt(QPNT url, QPNT ext, QINT size)
{
	QINT nlen;
	QUCHR vbuffer[QSTR_BUFF_SIZE];
	QUCHR chchar, *pchar;

	nlen = qurlGetFile(url, vbuffer, sizeof(vbuffer));
	if(nlen <= 0)
	{
		((QUCHR * )ext)[0] = 0;

		return 0;
	}
	pchar = vbuffer+nlen-1;
	while(pchar != vbuffer)
	{
		chchar = *pchar;
		if(chchar == '.')
		{
			break;
		}
		pchar--;
	}
	if(pchar == vbuffer)
	{
		((QUCHR * )ext)[0] = 0;

		return 0;
	}
	qstrcpy(ext, size, pchar+1, 0);

	return (QINT )(vbuffer+nlen-pchar-1);
}

QINT qurlGetParam(QPNT url, QPNT param, QINT size)
{
	QINT nlen, naccess;
	QUCHR vfile[QSTR_BUFF_SIZE];
	QUCHR chchar, *pchar, *pfile, *ptail;

	((QUCHR * )param)[0] = 0;
	nlen = qurlGetPath(url, vfile, sizeof(vfile));
	if(nlen <= 0)
	{
		return nlen;
	}
	if(qurlAccess(vfile, F_OK) == 0)
	{
		return 0;
	}
	naccess = 0;
	ptail = NULL;
	pfile = vfile;
	pchar = pfile+nlen-1;
	while(pchar != pfile)
	{
		chchar = *pchar;
		if(chchar == '\\' || chchar == '/')
		{
			break;
		}
		else if(chchar == '.')
		{
			ptail = pchar;
			*pchar = 0;
			if(qurlAccess(vfile, F_OK) == 0)
			{
				naccess = 1;
				break;
			}
			*pchar = '.';
		}
		pchar--;
	}
	if(naccess == 0 || ptail == NULL)
	{
		return 0;
	}
	nlen = pfile+nlen-1-ptail;
	qstrcpy(param, size, ptail+1, 0);
	if( (size <= 0) || (size > 0 && nlen < size) )
	{
		((QUCHR * )param)[nlen] = 0;
	}

	return nlen;
}

static QINT qurl_strncmp(QPNT urlstr, QPNT str, QINT size)
{
	QUCHR curlchar;
	QINT nurlpos, npos, ncnt;

	npos = 0;
	nurlpos = 0;
	while(nurlpos < size)
	{
		curlchar = ((QUCHR * )urlstr)[nurlpos];
		if(curlchar == '%')
		{
			ncnt = 2;
			curlchar = (QUCHR)qstrint(16, &(((QUCHR * )urlstr)[nurlpos]), &ncnt);
			if(curlchar != ((QUCHR * )str)[npos])
			{
				return 1;
			}
			nurlpos += 3;
		}
		else if(curlchar == ((QUCHR * )str)[npos])
		{
			nurlpos += 1;
		}
		else
		{
			return 1;
		}
		npos++;
	}

	return 0;
}

QINT qurlGetVar(QPNT url, QPNT name, QPNT value, QINT size)
{
	QUCHR *pchar, *pname;
	QINT nlen, ncnt, ncount, nindex, nmatch;

	pchar = (QUCHR * )url;
	while(pchar)
	{
		if(*pchar == '?' || *pchar == 0)
		{
			break;
		}
		pchar++;
	}
	if(pchar == NULL || *pchar == 0)
	{
		return -1;
	}
	if(((QUCHR * )name)[0] == '%')
	{
		//ncount = sscanf(name, "%%%d", &nindex);
		nlen = -1;
		nindex = qstrint(10, &(((QUCHR * )name)[1]), &nlen);
		if(nlen <= 0)
		{
			nindex = -1;
		}
	}
	else
	{
		nindex = -1;
	}
	ncount = 0;
	pchar++;
	while(pchar)
	{
		pname = pchar;
		while(pchar)
		{
			if(*pchar == '=' || *pchar == 0)
			{
				break;
			}
			pchar++;
		}
		if(*pchar == 0)
		{
			break;
		}
		nlen = (QINT )(pchar-pname);
		if( (nindex >= 0 && ncount >= nindex) ||
			(nindex < 0 && !qurl_strncmp(pname, name, nlen)) )
		{
			if(nindex >= 0)
			{
				qstrcpy(name, 0, pname, nlen);
				((QUCHR * )name)[nlen] = 0;
			}
			pchar++;
			nlen = 0;
			nmatch = 0;
			while(pchar)
			{
				if(size > 0 && nlen >= size)
				{
					break;
				}
				if(*pchar == 0)
				{
					break;
				}
				else if(*pchar == '&' && nmatch <= 0)
				{
					break;
				}
				else if(*pchar == '[' && *(pchar+1) != '[')
				{
					nmatch++;
				}
				else if(*pchar == ']' && *(pchar+1) != ']')
				{
					nmatch--;
				}
				else if(*pchar == '%')
				{
					if(value != NULL)
					{
						ncnt = 2;
						*((QUCHR *)value+nlen) = (QUCHR)qstrint(16, pchar+1, &ncnt);
					}
					pchar += 2;
				}
				else
				{
					if(value != NULL)
					{
						*((QUCHR *)value+nlen) = *pchar;
					}
				}
				nlen++;
				pchar++;
			}
			*((QUCHR *)value+nlen) = 0;

			return nlen;
		}
		else
		{
			while(pchar)
			{
				if(*pchar == '&' || *pchar == 0)
				{
					break;
				}
				pchar++;
			}
			if(*pchar == 0)
			{
				break;
			}
		}
		pchar++;
		ncount++;
	}

	return -1;
}

QINT qurlSetVar(QPNT url, QPNT name, QPNT value, QINT size)
{
	QINT nlen, nmatch;
	QUCHR *pchar, *pname, *pvalue;

	pchar = (QUCHR * )url;
	while(pchar)
	{
		if(*pchar == '?' || *pchar == 0)
		{
			break;
		}
		pchar++;
	}
	if(pchar == NULL)
	{
		return -1;
	}
	else if(*pchar == 0)
	{
		*pchar = '?';
		pchar++;
	}
	else
	{
		pchar++;
		while(pchar)
		{
			pname = pchar;
			while(pchar)
			{
				if(*pchar == '=' || *pchar == 0)
				{
					break;
				}
				pchar++;
			}
			if(*pchar == 0)
			{
				break;
			}
			if(!qurl_strncmp(pname, name, (QINT )(pchar-pname)))
			{
				// 找到同名变量。
				nmatch = 0;
				pchar++;
				while(pchar)
				{
					if(*pchar == 0)
					{
						break;
					}
					else if(*pchar == '&' && nmatch <= 0)
					{
						break;
					}
					else if(*pchar == '[' && *(pchar+1) != '[')
					{
						nmatch++;
					}
					else if(*pchar == ']' && *(pchar+1) != ']')
					{
						nmatch--;
					}
					pchar++;
				}
				if(*pchar == 0)
				{
					// 修改最后一个变量，需要清掉该变量前的'&'连接符。
					pchar = pname;
					while(pchar != url)
					{
						if(*pchar == '&' || *pchar == '?')
						{
							break;
						}
						pchar--;
					}
					if(pchar == url)
					{
						return 0;
					}
					else if(*pchar == '?')
					{
						// 第一个变量。
						pchar++;
					}
					else if(*pchar == '&')
					{
						// 前面还有其他变量。
						while(pchar != url)
						{
							if(*pchar != '&')
							{
								break;
							}
							pchar--;
						}
						pchar++;
						*pchar = '&';
						pchar++;
					}
					else
					{
						return 0;
					}
				}
				else if(*pchar == '&')
				{
					// 忽略多余的'&'连接符。
					pchar++;
					while(pchar)
					{
						if(*pchar != '&')
						{
							break;
						}
						pchar++;
					}
					// 后续变量往前移动。
					while(pchar)
					{
						if(*pchar == 0)
						{
							break;
						}
						*pname = *pchar;
						pname++;
						pchar++;
					}
					pchar = pname;
					*pchar = '&';
					pchar++;
				}
				break;
			}
			else
			{
				// 定位到下一变量开始处。
				nmatch = 0;
				while(pchar)
				{
					if(*pchar == 0)
					{
						break;
					}
					else if(*pchar == '&' && nmatch <= 0)
					{
						break;
					}
					else if(*pchar == '[' && *(pchar+1) != '[')
					{
						nmatch++;
					}
					else if(*pchar == ']' && *(pchar+1) != ']')
					{
						nmatch--;
					}
					pchar++;
				}
				if(*pchar == 0)
				{
					// 没有找到下一变量。
					*pchar = '&';
					pchar++;
					break;
				}
			}
			pchar++;
		}
		*pchar = '&';
		pchar++;
	}
	pname = (QUCHR * )name;
	while(pchar && pname)
	{
		if(*pname == 0)
		{
			break;
		}
		else if(*pname == '%')
		{
			pchar[0] = '%';
			pchar[1] = '2';
			pchar[2] = '5';
			pchar += 3;
		}
		else if(*pname == '&')
		{
			pchar[0] = '%';
			pchar[1] = '2';
			pchar[2] = '6';
			pchar += 3;
		}
		else if(*pname == '=')
		{
			pchar[0] = '%';
			pchar[1] = '3';
			pchar[2] = 'D';
			pchar += 3;
		}
		else if(*pname == '?')
		{
			pchar[0] = '%';
			pchar[1] = '3';
			pchar[2] = 'F';
			pchar += 3;
		}
		else
		{
			*pchar = *pname;
			pchar++;
		}
		pname++;
	}
	*pchar = '=';
	pchar++;
	nlen = 0;
	pvalue = (QUCHR * )value;
	while(pchar && pvalue)
	{
		if(size > 0 && (QINT )(pvalue-(QUCHR * )value) >= size)
		{
			break;
		}
		else if(*pvalue == '%')
		{
			pchar[0] = '%';
			pchar[1] = '2';
			pchar[2] = '5';
			pchar += 3;
			nlen += 3;
		}
		else if(*pvalue == '&')
		{
			pchar[0] = '%';
			pchar[1] = '2';
			pchar[2] = '6';
			pchar += 3;
			nlen += 3;
		}
		else if(*pvalue == '=')
		{
			pchar[0] = '%';
			pchar[1] = '3';
			pchar[2] = 'D';
			pchar += 3;
			nlen += 3;
		}
		else if(*pvalue == '?')
		{
			pchar[0] = '%';
			pchar[1] = '3';
			pchar[2] = 'F';
			pchar += 3;
		}
		else
		{
			*pchar = *pvalue;
			pchar++;
			nlen++;
		}
		pvalue++;
	}
	*pchar = 0;

	return nlen;
}

QINT qurlDelVar(QPNT url, QPNT name)
{
	QINT nmatch;
	QUCHR *pchar, *pname;

	pchar = (QUCHR * )url;
	while(pchar)
	{
		if(*pchar == '?' || *pchar == 0)
		{
			break;
		}
		pchar++;
	}
	if(pchar == NULL)
	{
		return -1;
	}
	else if(*pchar != 0)
	{
		if(name == NULL)
		{
			// name==NULL，表示删除全部变量。
			*pchar = 0;
			pchar++;
		}
		else
		{
			// 寻找name变量值。
			pchar++;
			while(pchar)
			{
				pname = pchar;
				while(pchar)
				{
					if(*pchar == '=' || *pchar == 0)
					{
						break;
					}
					pchar++;
				}
				if(*pchar == 0)
				{
					break;
				}
				if(!qurl_strncmp(pname, name, (QINT )(pchar-pname)))
				{
					nmatch = 0;
					pchar++;
					while(pchar)
					{
						if(*pchar == 0)
						{
							break;
						}
						else if(*pchar == '&' && nmatch <= 0)
						{
							break;
						}
						else if(*pchar == '[' && *(pchar+1) != '[')
						{
							nmatch++;
						}
						else if(*pchar == ']' && *(pchar+1) != ']')
						{
							nmatch--;
						}
						pchar++;
					}
					if(*pchar == 0)
					{
						*pname = 0;
						if(*(pname-1) == '?')
						{
							*(pname-1) = 0;
						}
					}
					else if(*pchar == '&')
					{
						pchar++;
						while(pchar)
						{
							if(*pchar == 0)
							{
								break;
							}
							*pname = *pchar;
							pname++;
							pchar++;
						}
						pchar = pname;
					}
					break;
				}
				else
				{
					pchar++;
					while(pchar)
					{
						if(*pchar == '&' || *pchar == 0)
						{
							break;
						}
						pchar++;
					}
				}
				pchar++;
			}
		}
	}

	return qstrlen(url);
}

QINT qurlCpyVar(QPNT dsturl, QPNT dstname, QPNT srcurl, QPNT srcname)
{
	QINT nlen, ncount;
	QUCHR szname[QSTR_BUFF_SIZE];
	QUCHR szvalue[QSTR_BUFF_SIZE];

	if(dsturl == NULL || srcurl == NULL)
	{
		return -1;
	}
	if(srcname == NULL)
	{
		ncount = 0;
		while(1)
		{
			sprintf((char *)szname, "%%%ld", ncount);
			nlen = qurlGetVar(srcurl, szname, szvalue, sizeof(szvalue));
			if(nlen < 0)
			{
				break;
			}
			qurlSetVar(dsturl, szname, szvalue, nlen);
			ncount++;
		}
	}
	else
	{
		nlen = qurlGetVar(srcurl, srcname, szvalue, sizeof(szvalue));
		if(nlen < 0)
		{
			return -1;
		}
		if(dstname == NULL)
		{
			qurlSetVar(dsturl, srcname, szvalue, nlen);
		}
		else
		{
			qurlSetVar(dsturl, dstname, szvalue, nlen);
		}
		ncount = 1;
	}

	return ncount;
}

QINT qurlScanx(QPNT url, qurl_scan_cb scan_cb, QPNT params[], QINT count)
{
	QUCHR szname[QSTR_BUFF_SIZE];
	QUCHR szvalue[QSTR_BUFF_SIZE];
	QUCHR *pchar, *pname, *pdstname, *pvalue;
	QINT nlen, ncnt, ncount, nindex, nresult, nmatch;

	if(scan_cb == NULL)
	{
		return -1;
	}
	pchar = (QUCHR * )url;
	while(pchar)
	{
		if(*pchar == '\\')
		{
			pchar++;
		}
		else if(*pchar == '?' || *pchar == 0)
		{
			break;
		}
		pchar++;
	}
	if(pchar == NULL || *pchar == 0)
	{
		return -1;
	}
	nindex = -1;
	ncount = 0;
	pchar++;
	while(pchar)
	{
		nlen = 0;
		pname = pchar;
		pdstname = szname;
		while(pchar && nlen < sizeof(szname))
		{
			if(*pchar == '=' || *pchar == 0)
			{
				break;
			}
			nlen++;
			*pdstname++ = *pchar++;
		}
		if(*pchar == 0)
		{
			break;
		}
		if(nlen < sizeof(szname)-1)
		{
			szname[nlen] = 0;
		}
		pchar++;
		nlen = 0;
		nmatch = 0;
		pvalue = szvalue;
		while(pchar && nlen < sizeof(szvalue))
		{
			if(*pchar == 0)
			{
				break;
			}
			else if(*pchar == '&' && nmatch <= 0)
			{
				break;
			}
			else if(*pchar == '[' && *(pchar+1) != '[')
			{
				nmatch++;
			}
			else if(*pchar == ']' && *(pchar+1) != ']')
			{
				nmatch--;
			}
			else if(*pchar == '%')
			{
				ncnt = 2;
				*pvalue = (QUCHR)qstrint(16, pchar+1, &ncnt);
				pchar += 2;
			}
			else
			{
				*pvalue = *pchar;
			}
			nlen++;
			pchar++;
			pvalue++;
		}
		*pvalue = 0;
		nresult = scan_cb(url, szname, szvalue, nlen, params, count);
		if(nresult != QSCN_OK)
		{
			break;
		}
		if(*pchar == 0)
		{
			break;
		}
		pchar++;
		ncount++;
	}

	return -1;
}

QINT qurlScan(QPNT url, qurl_scan_cb scan_cb, QINT count, ...)
{
	QINT nresult;
	QPNT *ppprms;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;

	va_start(marker, count);
	ppprms = (QPNT *)qprmmakev(url, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
	va_end(marker);

	nresult = qurlScanx(url, scan_cb, ppprms, count);

	qprmfree(ppprms);

	return nresult;
}

#define qurl_cmp_value(chr) ( ((chr)=='\\')?('/'):((chr)) )
#define qurl_icmp_value(chr) ( ((chr)>='A'&&(chr)<='Z') ? ('a'+(chr)-'A') : ( ((chr)=='\\')?('/'):((chr)) ) )


struct qurl_cmp_data
{
	QINT nhas;
	QINT ncmp;
	QUCHR *pname;
	QUCHR *pvalue;
	QINT nsize;
};

static QINT qurl_has_cmp_scan_cb2(QPNT url, QPNT name, QPNT value, QINT size, QPNT params[], QINT count)
{
	struct qurl_cmp_data *pdata;

	pdata = (struct qurl_cmp_data *)params[0];
	if(pdata == NULL)
	{
		return 0;
	}
	if(!qstrcmp(QSTR_NONE, pdata->pname, name, 0))
	{
		return 1;
	}
	pdata->nhas = 1;
	if(pdata->nsize != size)
	{
		pdata->ncmp = 1;

		return 0;
	}
	if(memcmp(pdata->pvalue, value, size))
	{
		pdata->ncmp = 1;

		return 0;
	}

	return 1;
}

static QINT qurl_has_cmp_scan_cb1(QPNT url, QPNT name, QPNT value, QINT size, QPNT params[], QINT count)
{
	QINT *pcmp;
	QUCHR *purl2;
	QPNT vpprms[4];
	struct qurl_cmp_data stdata;

	pcmp = (QINT *)params[1];
	if(pcmp == NULL)
	{
		return 0;
	}
	purl2 = (QUCHR *)params[0];
	if(purl2 == NULL)
	{
		*pcmp = 1;

		return 0;
	}
	stdata.nhas = 0;
	stdata.ncmp = 0;
	stdata.pname = (QUCHR * )name;
	stdata.pvalue = (QUCHR * )value;
	stdata.nsize = size;
	vpprms[0] = &stdata;
	qurlScanx(purl2, qurl_has_cmp_scan_cb2, vpprms, 1);
	if(stdata.nhas == 0 || stdata.ncmp != 0)
	{
		*pcmp = 1;

		return 0;
	}

	return 1;
}

static QINT qurl_has_scan_cb1(QPNT url, QPNT name, QPNT value, QINT size, QPNT params[], QINT count)
{
	struct qurl_cmp_data *pdata;

	pdata = (struct qurl_cmp_data *)params[0];
	if(pdata == NULL)
	{
		return 0;
	}
	if(!qstrcmp(QSTR_NONE, pdata->pname, name, 0))
	{
		return 1;
	}
	pdata->nhas = 1;

	return 0;
}

static QINT qurl_has_scan_cb2(QPNT url, QPNT name, QPNT value, QINT size, QPNT params[], QINT count)
{
	QINT *pcmp;
	QUCHR *purl1;
	QPNT vpprms[4];
	struct qurl_cmp_data stdata;

	pcmp = (QINT *)params[1];
	if(pcmp == NULL)
	{
		return 0;
	}
	purl1 = (QUCHR *)params[0];
	if(purl1 == NULL)
	{
		*pcmp = 1;

		return 0;
	}
	stdata.nhas = 0;
	stdata.ncmp = 0;
	stdata.pname = (QUCHR * )name;
	stdata.pvalue = (QUCHR * )value;
	stdata.nsize = size;
	vpprms[0] = &stdata;
	qurlScanx(purl1, qurl_has_scan_cb1, vpprms, 1);
	if(stdata.nhas == 0)
	{
		*pcmp = 1;

		return 0;
	}

	return 1;
}

QINT qurlCmp(QINT flag, QPNT url1, QPNT url2)
{
	QPNT vpprms[4];
	QINT ncmp, npro1, npro2;
	QUCHR chrurl1, *purl1, *ppro1, *pdisk1, *ppath1, *pparams1;
	QUCHR chrurl2, *purl2, *ppro2, *pdisk2, *ppath2, *pparams2;

	if(url1 == NULL && url2 == NULL)
	{
		return 0;
	}
	else if(url1 == NULL)
	{
		return qstrlen(url2);
	}
	else if(url2 == NULL)
	{
		return qstrlen(url1);
	}
	// 分析第一个url的协议、路径。
	pparams1 = NULL;
    chrurl1 = 0;
	npro1 = 0;
	ppro1 = (QUCHR * )url1;
	purl1 = (QUCHR * )url1;
	while(purl1)
	{
		chrurl1 = *purl1;
		if(chrurl1 == '\0')
		{
			break;
		}
		else if(chrurl1 == ':')
		{
			break;
		}
		else if(chrurl1 == '?')
		{
			pparams1 = purl1;
			break;
		}
		else if(chrurl1 == '/' || chrurl1 == '\\')
		{
			break;
		}
		npro1++;
		purl1++;
	}
	pdisk1 = NULL;
	if(chrurl1 != ':')
	{
		npro1 = 0;
		ppro1 = NULL;
		ppath1 = purl1;
	}
	else
	{
		if( (npro1 == 1) && ((*ppro1 >= 'a' && *ppro1 <= 'z') || (*ppro1 >= 'A' && *ppro1 <= 'Z')) )
		{
			npro1 = 0;
			ppro1 = NULL;
			pdisk1 = (QUCHR * )url1;
		}
		ppath1 = purl1+1;
	}
	// 分析第二个url的协议、路径。
	pparams2 = NULL;
    chrurl2 = 0;
	npro2 = 0;
	ppro2 = (QUCHR * )url2;
	purl2 = (QUCHR * )url2;
	while(purl2)
	{
		chrurl2 = *purl2;
		if(chrurl2 == '\0')
		{
			break;
		}
		else if(chrurl2 == ':')
		{
			break;
		}
		else if(chrurl2 == '?')
		{
			pparams2 = purl2;
			break;
		}
		else if(chrurl2 == '/' || chrurl2 == '\\')
		{
			break;
		}
		npro2++;
		purl2++;
	}
	pdisk2 = NULL;
	if(chrurl2 != ':')
	{
		npro2 = 0;
		ppro2 = NULL;
		ppath2 = purl2;
	}
	else
	{
		if( (npro2 == 1) && ((*ppro2 >= 'a' && *ppro2 <= 'z') || (*ppro2 >= 'A' && *ppro2 <= 'Z')) )
		{
			npro2 = 0;
			ppro2 = NULL;
			pdisk2 = (QUCHR * )url2;
		}
		ppath2 = purl2+1;
	}
	// 比较协议。
	if(ppro1 == NULL || ppro2 == NULL)
	{
		// 至少有一个没有协议。
		if(ppro1 != NULL || ppro2 != NULL)
		{
			// 至少有一个有协议。
			return 0;
		}
	}
	else
	{
		if(npro1 != npro2)
		{
			// 协议长度不相同。
			return 0;
		}
		else if(!qstrcmp(QSTR_ICS, ppro1, ppro2, npro1))
		{
			// 协议不相同。
			return 0;
		}
	}
	// 比较盘符。
	if(pdisk1 == NULL || pdisk2 == NULL)
	{
		if(pdisk1 != NULL)
		{
			if(*pdisk1 != 'c' && *pdisk1 != 'C')
			{
				return 0;
			}
		}
		else if(pdisk2 != NULL)
		{
			if(*pdisk2 != 'c' && *pdisk2 != 'C')
			{
				return 0;
			}
		}
	}
	else
	{
		if(qurl_icmp_value(*pdisk1) != qurl_icmp_value(*pdisk2))
		{
			return 0;
		}
	}
	// 比较路径。
	purl1 = ppath1;
	purl2 = ppath2;
	while(1)
	{
		while(purl1)
		{
			if(*purl1 != '/' && *purl1 != '\\')
			{
				break;
			}
			purl1++;
		}
		while(purl2)
		{
			if(*purl2 != '/' && *purl2 != '\\')
			{
				break;
			}
			purl2++;
		}
		while(1)
		{
			if(flag & QSTR_ICS)
			{
				chrurl1 = qurl_icmp_value(*purl1);
				chrurl2 = qurl_icmp_value(*purl2);
			}
			else
			{
				chrurl1 = qurl_cmp_value(*purl1);
				chrurl2 = qurl_cmp_value(*purl2);
			}
			if(chrurl1 != chrurl2)
			{
				return 0;
			}
			if(chrurl1 == '\0')
			{
				break;
			}
			else if(chrurl1 == '/')
			{
				break;
			}
			else if(chrurl1 == '?')
			{
				pparams1 = purl1;
				pparams2 = purl2;
				break;
			}
			purl1++;
			purl2++;
		}
		if(chrurl1 == '\0')
		{
			break;
		}
		else if(chrurl1 == '?')
		{
			break;
		}
	}
	if(pparams1 != NULL || pparams2 != NULL)
	{
		if(pparams1 == NULL || pparams2 == NULL)
		{
			return 0;
		}
		else
		{
			ncmp = 0;
			vpprms[0] = (QPNT )url2;
			vpprms[1] = (QPNT )&ncmp;
			qurlScanx(url1, qurl_has_cmp_scan_cb1, vpprms, 2);
			if(ncmp != 0)
			{
				return 0;
			}
			vpprms[0] = (QPNT )url1;
			vpprms[1] = (QPNT )&ncmp;
			qurlScanx(url2, qurl_has_scan_cb2, vpprms, 2);
			if(ncmp != 0)
			{
				return 0;
			}
		}
	}

	return 1;
}

QINT qurlStrip(QPNT dst, QINT dstsize, QPNT src, QINT srcsize)
{
	QINT nsrc, ndst, nbyte;
	QUCHR chsrc, *psrc, *pdst;

	if(src == NULL)
	{
		psrc = (QUCHR * )dst;
	}
	else
	{
		psrc = (QUCHR * )src;
	}
	nbyte = sizeof(QUCHR);
	pdst = (QUCHR * )dst;
	if(psrc == NULL || pdst == NULL)
	{
		return 0;
	}
	ndst = 0;
	nsrc = 0;
	while(ndst < dstsize)
	{
		if(src == NULL)
		{
			chsrc = (QUCHR )(*((QUCHR *)psrc));
		}
		else
		{
			chsrc = (QUCHR )(*((QUCHR *)psrc));
		}
		if(nsrc >= srcsize)
		{
			*pdst = 0;
			break;
		}
		else if(chsrc == 0)
		{
			*pdst = 0;
			break;
		}
		else if(chsrc == '\\' || chsrc == '/')
		{
			*pdst++ = '/';
			ndst++;
			psrc += nbyte;
			nsrc++;
			while(psrc)
			{
				chsrc = *psrc;
				if(chsrc != '\\' && chsrc != '/')
				{
					break;
				}
				psrc += nbyte;
				nsrc++;
			}
		}
		else
		{
			*pdst++ = ((chsrc>='A'&&chsrc<='Z')?(chsrc+'a'-'A'):chsrc);
			ndst++;
			psrc += nbyte;
			nsrc++;
		}
	}

	return ndst;
}

QINT qurlCopy(QPNT url, QPNT dst, QINT dstsize, QPNT src, QINT *srcsize)
{
	QUCHR *pdst, *psrc, chsrc;
	QINT ndstpos, nsrcpos, nsrcsize;

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
		if(chsrc != '$')
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
		else
		{
			// 读取变量。
			QINT nlen, nbracket;
			QUCHR *pname, vname[QSTR_BUFF_SIZE] = {0};

			if(*(psrc+1) == '(')
			{
				psrc += 2;
				nsrcpos += 2;
				nlen = 0;
				nbracket = 1;
				pname = vname;
				while(nlen < sizeof(vname))
				{
					chsrc = *psrc;
					if(chsrc == 0)
					{
						break;
					}
					else if(chsrc == '(')
					{
						nbracket++;
					}
					else if(chsrc == ')')
					{
						nbracket--;
					}
					if(nbracket <= 0)
					{
						psrc++;
						nsrcpos++;
						break;
					}
					*pname++ = *psrc++;
					nsrcpos++;
					nlen++;
				}
			}// chsrc=='('
			else
			{
				psrc += 1;
				nsrcpos += 1;
				nlen = 0;
				pname = vname;
				while(nlen < sizeof(vname))
				{
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
			}// chsrc!='('
			if(nlen > 0)
			{
				if(dstsize <= 0)
				{
					nlen = qurlGetVar(url, vname, pdst, 0);
				}
				else
				{
					nlen = qurlGetVar(url, vname, pdst, dstsize-ndstpos);
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
