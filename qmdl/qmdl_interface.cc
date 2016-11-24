
#include "qmdl_module.h"


QINT IQModule::IsRouted()
{
	return (GetFlag() & QFLAG_ROUTED);
}

QINT IQModule::IsPeek()
{
	return (GetFlag() & QFLAG_PEEK);
}

QMDL IQModule::RootModule()
{
	QMDL pmodule, pparent;
	
	pmodule = (QMDL )this;
	while(pmodule)
	{
		pparent = pmodule->ParentModule();
		if(pparent == NULL)
		{
			break;
		}
		pmodule = pparent;
	}
	
	return pmodule;
}

static QINT qmdl_find_module_by_name_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QINT nnamelen;
	QPNT psrcname, pdstname;
	QMDL pmodule, *ppfound, pchild;
	
	pmodule = (QMDL )hdl;
	if(pmodule == NULL)
	{
		return QSCN_OK;
	}
	pchild = (QMDL)params[3];
	if(pchild == pmodule)
	{
		return QSCN_HORZ;
	}
	psrcname = (QPNT)params[0];
	nnamelen = (QINT)params[1];
	pdstname = pmodule->GetName();
	if(!qstrcmp(QSTR_ICS, psrcname, pdstname, nnamelen))
	{
		return QSCN_OK;
	}
	if(nnamelen > 0)
	{
		if(*((QSTR)pdstname+nnamelen) != 0)
		{
			return QSCN_OK;
		}
	}
	ppfound = (QMDL *)params[2];
	if(ppfound == NULL)
	{
		return QSCN_END;
	}
	*ppfound = pmodule;

	return QSCN_END;
}

QMDL IQModule::FindModule(QSTR path, QINT pathlen, QSTR name, QINT namelen)
{
	QINT nlen, npos;
	QMDL pmodule, pchild;
	QSTR ppath, pname, pnext, pchildname;

	pmodule = (QMDL )this;
	if(path != NULL)
	{
		npos = 0;
		ppath = path;
		if(*ppath == '/' || *ppath == '\\')
		{
			pmodule = RootModule();
		}
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
			if(qstrcmp(QSTR_ICS, (QPNT)pname, (QPNT)".", nlen))
			{
				pchild = pmodule;
			}
			else if(qstrcmp(QSTR_ICS, (QPNT)pname, (QPNT)"..", nlen))
			{
				pchild = pmodule->ParentModule();
			}
			else
			{
				pchild = pmodule->HeadModule();
				while(pchild)
				{
					pchildname = pchild->GetName();
					if(pchildname != NULL && qstrcmp(QSTR_ICS, pchildname, pname, nlen))
					{
						break;
					}
					pchild = pchild->NextModule();
				}
			}
			if(pchild == NULL)
			{
				pmodule = NULL;
				break;
			}
			pmodule = pchild;
			ppath = pnext;
		}
		if(pmodule == NULL)
		{
			return NULL;
		}
	}
	if(name != NULL)
	{
		QINT ncount;
		QPNT vpprms[8];
		QMDL pfound;
		
		if(path != NULL)
		{
			pfound = NULL;
			ncount = 0;
			vpprms[ncount++] = (QPNT)name;
			vpprms[ncount++] = (QPNT)namelen;
			vpprms[ncount++] = (QPNT)&pfound;
			vpprms[ncount++] = (QPNT)NULL;
			ScanModulex(pmodule, qmdl_find_module_by_name_cb, NULL, vpprms, ncount);
			pmodule = pfound;
		}
		else
		{
			pfound = NULL;
			pchild = NULL;
			while(pmodule)
			{
				ncount = 0;
				vpprms[ncount++] = (QPNT)name;
				vpprms[ncount++] = (QPNT)namelen;
				vpprms[ncount++] = (QPNT)&pfound;
				vpprms[ncount++] = (QPNT)pchild;
				ScanModulex(pmodule, qmdl_find_module_by_name_cb, NULL, vpprms, ncount);
				if(pfound != NULL)
				{
					break;
				}
				pchild = pmodule;
				pmodule = pmodule->ParentModule();
			}
			pmodule = pfound;
		}
	}

	return (QMDL )pmodule;
}

QINT IQModule::ScanModulex(QMDL module, QPCB prev_cb, QPCB post_cb, QPNT params[], QINT count)
{
	QINT nresult, nflag, ndepty;
	QUCHR vflag[QSCN_STACK_CNT] = {0};
	QMDL pmodule, pgrand, pparent, pchild;

	if(module == NULL)
	{
		pmodule = (QMDL )this;
	}
	else
	{
		pmodule = (QMDL )module;
	}
	ndepty = 0;
	nresult = QSCN_OK;
	nflag = QSCN_OK;
	while(pmodule)
	{
		// 执行前序遍历函数。
		nresult = QSCN_OK;
		if(prev_cb != NULL)
		{
			nresult = prev_cb(pmodule, NULL, QCD_CALL, params, count);
		}
		if(nresult == QSCN_END)
		{
			// 返回中止，退出遍历。
			break;
		}
		else if(nresult == QSCN_FAIL)
		{
			// 返回失败，退出遍历。
			nflag = QSCN_FAIL;
			break;
		}
		if(nresult == QSCN_VERT)
		{
			// 垂直遍历，不遍历兄弟结点。
			vflag[ndepty] = 1;
		}
		ndepty++;
		pchild = NULL;
		if(nresult != QSCN_HORZ)
		{
			// 非水平遍历，进入子树遍历。
			pchild = pmodule->HeadModule();
		}
		if(pchild == NULL)
		{
			// 没有子树，遍历下一兄弟结点。
			pparent = pmodule;
			ndepty--;
			if(ndepty >= 0 && pmodule != module)
			{
				// 未到根节点或遍历顶结点。
				if(vflag[ndepty] == 1)
				{
					// 垂直遍历，回到父结点的兄弟继续遍历。
					vflag[ndepty] = 0;
				}
				else
				{
					// 非垂直遍历，继续兄弟结点遍历。
					pchild = pparent->NextModule();
				}
				pgrand = pparent->ParentModule();
				// 执行后序遍历函数。
				if(post_cb != NULL)
				{
					post_cb(pparent, NULL, QCD_CALL, params, count);
				}
				while(pchild == NULL)
				{
					pparent = pgrand;
					if(pparent == NULL)
					{
						break;
					}
					ndepty--;
					if(vflag[ndepty] == 1)
					{
						// 垂直遍历，回到父结点的兄弟继续遍历。
						vflag[ndepty] = 0;
					}
					else
					{
						// 非垂直遍历，继续兄弟结点遍历。
						pchild = pparent->NextModule();
					}
					// 执行后序遍历函数。
					pgrand = pparent->ParentModule();
					if(post_cb != NULL)
					{
						post_cb(pparent, NULL, QCD_CALL, params, count);
					}
					if(ndepty < 0 || pparent == module)
					{
						// 到达根节点或遍历顶结点。
						pchild = NULL;
						break;
					}
				}
			}
		}
		pmodule = pchild;
	}
	if(nflag == QSCN_FAIL)
	{
		// 返回失败退出时，需要做未完成的后序函数执行。
		while(pmodule)
		{
			pparent = pmodule->ParentModule();
			if(post_cb != NULL)
			{
				post_cb(pmodule, NULL, QCD_CALL, params, count);
			}
			ndepty--;
			if(pmodule == module)
			{
				break;
			}
			pmodule = pparent;
		}
	}

	return nresult;
}

QINT IQModule::ScanModule(QMDL module, QPCB prev_cb, QPCB post_cb, QINT count, ...)
{
	QINT nresult;
	QPNT *ppparams;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;

	va_start( marker, count );
	ppparams = (QPNT *)qprmmakev(module, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
	va_end( marker );

	nresult = ScanModulex(module, prev_cb, post_cb, ppparams, count);

	qprmfree(ppparams);

	return nresult;
}

QINT IQModule::GetPath(QMDL parent, QPNT path, QINT size)
{
	QSTR pname, ppath;
	QINT nlen, nsize, nindex, ncount;
	QMDL pmodule, *ppmodule, vpmodule[QSCN_STACK_CNT];
	
	ncount = 0;
	pmodule = this;
	ppmodule = vpmodule;
	while(pmodule)
	{
		if(ncount >= QSCN_STACK_CNT)
		{
			break;
		}
		*ppmodule++ = pmodule;
		ncount++;
		if(pmodule == parent)
		{
			break;
		}
		pmodule = (QMDL )pmodule->ParentModule();
	}
	nsize = 0;
	ppath = (QSTR )path;
	if(pmodule != NULL)
	{
		pmodule = vpmodule[ncount-1];
		pname = (QSTR )pmodule->GetName();
		if(ppath != NULL)
		{
			if( (size <= 0) || (size > 0 && nsize < size) )
			{
				nlen = qstrcpy(ppath, size-nsize, pname, 0);
				ppath += nlen;
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
				*ppath++ = '/';
				*ppath = '\0';
			}
		}
		nsize++;
	}
	nindex = ncount-2;
	ppmodule = &vpmodule[nindex];
	while(nindex >= 0)
	{
		pmodule = *ppmodule--;
		pname = (QSTR )pmodule->GetName();
		if(ppath != NULL)
		{
			if( (size <= 0) || (size > 0 && nsize < size) )
			{
				*ppath++ = '/';
				nsize++;
				nlen = qstrcpy(ppath, size-nsize, pname, 0);
				ppath += nlen;
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
