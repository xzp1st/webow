
#include "qmdl_module.h"
#include "qmdl_module_internal.h"
#include "qmdl_base.h"


static QINT qmdl_attr_item_free_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	struct qmdl_attr_item *pitem;
	
	pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hdl);
	if(pitem == NULL)
	{
		return QSCN_OK;
	}
	if(pitem->pname != NULL)
	{
		qmfree(pitem->pname);
		pitem->pname = NULL;
	}
	pitem->pcbval = NULL;
	if(pitem->hsublink != NULL)
	{
		qlinkFreeLinkx(QLINK_ATTR, pitem->hsublink, qmdl_attr_item_free_cb, NULL, 0);
		pitem->hsublink = NULL;
	}
	
	pitem->pxmlval = NULL;
	
	return QSCN_OK;
}

void QModule::SetAttrXml(QINT copy, QXML mxml)
{
	QPNT pid;
	QHDL hitem;
	QPCB pcbval;
	QINT nstackcnt, nflag;
	QXML pmxml, pchild, pparent, pattr;
	struct qmdl_attr_item *pitem, *plast, *vlast[QSCN_STACK_CNT];

	if(mxml == NULL)
	{
		return;
	}
	if(copy == 0)
	{
		goto ADD_ATTR;
	}
	else
	{
		goto DUP_ATTR;
	}
	
ADD_ATTR:
	hitem = qlinkMake(QLINK_ATTR, NULL, NULL, sizeof(struct qmdl_attr_item));
	pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
	if(pitem == NULL)
	{
		return;
	}
	nflag = qxmlGetFlag(mxml);
	pitem->nflag = (nflag&QMDL_MASK);
	pitem->pname = NULL;
	pitem->pcbval = NULL;
	pitem->hsublink = NULL;
	pitem->pxmlval = mxml;
	mhAttrLink = qlinkInsertNext(QLINK_ATTR, mhAttrLink, NULL, hitem);
	return;
	
DUP_ATTR:
	hitem = qlinkTail(QLINK_ATTR, mhAttrLink);
	if(hitem != NULL)
	{
		pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
		if(pitem == NULL)
		{
			return;
		}
	}
	else
	{
		hitem = qlinkMake(QLINK_ATTR, NULL, NULL, sizeof(struct qmdl_attr_item));
		pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
		if(pitem == NULL)
		{
			return;
		}
		pitem->pname = NULL;
		pitem->pcbval = NULL;
		pitem->hsublink = NULL;
		pitem->pxmlval = mxml;
		mhAttrLink = qlinkInsertNext(QLINK_ATTR, mhAttrLink, NULL, hitem);
	}
	plast = pitem;
	nstackcnt = 0;
	pmxml = mxml;
	while(pmxml)
	{
		nflag = qxmlGetFlag(mxml) & QMDL_MASK;
		pattr = (QXML)qxmlAttrGetHead(pmxml, 0);
		while(pattr)
		{
			pid = (QSTR )qxmlGetId(pattr);
			pcbval = qxmlGetValcb(pattr);
			if(pcbval != NULL)
			{
				hitem = SetAttrItem((QSTR )pid, nflag);
				pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
				if(pitem != NULL)
				{
					pitem->pcbval = pcbval;
					pchild = (QXML)qxmlGetChild(pmxml, QXML_PARAM, 0);
					if(pchild != NULL)
					{
						pitem->ptype = (QPNT)pmxml;
					}
				}
			}
			pattr = (QXML)qxmlAttrGetNext(pattr, 0);
		}
		pid = (QSTR )qxmlGetId(pmxml);
		if(pid == NULL)
		{
			pitem = NULL;
			goto NEXT_INIT_NODE;
		}
		hitem = qlinkHead(QLINK_ATTR, plast->hsublink);
		while(hitem)
		{
			pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
			if(pitem != NULL)
			{
				if(pitem->nflag == nflag && qstrcmp(QSTR_CMP_ICASE, pitem->pname, pid, 0))
				{
					break;
				}
			}
			hitem = qlinkNext(QLINK_ATTR, hitem);
		}
		if(hitem == NULL)
		{
			hitem = qlinkMake(QLINK_ATTR, NULL, NULL, sizeof(struct qmdl_attr_item));
			plast->hsublink = qlinkInsertNext(QLINK_ATTR, plast->hsublink, NULL, hitem);
		}
		pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
		if(pitem == NULL)
		{
			return;
		}
		if(pitem->pname == NULL)
		{
			pitem->pname = (QSTR)qstrdup(NULL, pid);
		}
		pitem->nflag = nflag;
		pcbval = qxmlGetValcb(pmxml);
		if(pcbval != NULL)
		{
			pitem->pcbval = pcbval;
		}
		goto NEXT_INIT_NODE;
		
NEXT_INIT_NODE:
		vlast[nstackcnt] = plast;
		nstackcnt++;
		
		// 记录上一个结点。
		plast = pitem;
		
		pchild = (QXML)qxmlGetHead(pmxml);
		if(pchild == NULL)
		{
			nstackcnt--;
			plast = vlast[nstackcnt];
			if(nstackcnt >= 0 && pmxml != mxml)
			{
				pparent = pmxml;
				pchild = (QXML)qxmlGetNext(pparent);
				while(pchild == NULL)
				{
					pparent = (QXML)qxmlGetParent(pparent);
					if(pparent == NULL)
					{
						break;
					}
					nstackcnt--;
					plast = vlast[nstackcnt];
					if(nstackcnt < 0 || pmxml == mxml)
					{
						break;
					}
					pchild = (QXML)qxmlGetNext(pparent);
				}
			}
		}
		pmxml = pchild;
	}
	return;
}

QHDL QModule::SetAttrItem(QSTR name, QINT flag)
{
	QINT nlen;
	QHDL hitem;
	QSTR phead, ptail;
	struct qmdl_attr_item *pitem, *plast;
	
	hitem = qlinkHead(QLINK_ATTR, mhAttrLink);
	if(hitem != NULL)
	{
		pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
		if(pitem == NULL)
		{
			return NULL;
		}
	}
	else
	{
		hitem = qlinkMake(QLINK_ATTR, NULL, NULL, sizeof(struct qmdl_attr_item));
		pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
		if(pitem == NULL)
		{
			return NULL;
		}
		pitem->pname = NULL;
		pitem->pcbval = NULL;
		pitem->hsublink = NULL;
		pitem->pxmlval = NULL;
		mhAttrLink = qlinkInsertNext(QLINK_ATTR, mhAttrLink, NULL, hitem);
	}
	plast = pitem;
	phead = name;
	while(phead)
	{
		ptail = (QSTR)qstrchr(0, phead, 0, '.');
		if(ptail == NULL)
		{
			nlen = qstrlen(phead);
			if(nlen <= 0)
			{
				break;
			}
		}
		else
		{
			nlen = (QINT)(ptail-phead);
			if(nlen <= 0)
			{
				continue;
			}
		}
		hitem = qlinkHead(QLINK_ATTR, plast->hsublink);
		while(hitem)
		{
			pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
			if(pitem != NULL)
			{
				if(qstrcmp(QSTR_CMP_ICASE, pitem->pname, phead, nlen))
				{
					if(ptail != NULL)
					{
						break;
					}
					else
					{
						if(pitem->nflag == flag)
						{
							break;
						}
					}
				}
			}
			hitem = qlinkNext(QLINK_ATTR, hitem);
		}
		if(hitem == NULL)
		{
			hitem = qlinkMake(QLINK_ATTR, NULL, NULL, sizeof(struct qmdl_attr_item));
			plast->hsublink = qlinkInsertNext(QLINK_ATTR, plast->hsublink, NULL, hitem);
		}
		pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
		if(pitem == NULL)
		{
			return NULL;
		}
		if(pitem->pname != NULL)
		{
			qmfree(pitem->pname);
		}
		if(nlen <= 0)
		{
			pitem->pname = NULL;
		}
		else
		{
			pitem->pname = (QSTR)qmalloc(NULL, nlen+1);
			if(pitem->pname != NULL)
			{
				memset(pitem->pname, 0, nlen+1);
				qstrcpy(pitem->pname, nlen+1, phead, nlen);
			}
		}
		if(ptail == NULL)
		{
			pitem->nflag = flag;
			break;
		}
		phead = ptail+1;
		plast = pitem;
	}
	
	return hitem;
}

QHDL QModule::GetAttrItem(QSTR name, QINT flag)
{
	QINT nlen, nflag;
	QHDL hnode, hitem;
	QXML pmxml, pchild;
	QSTR phead, ptail, pid;
	struct qmdl_attr_item *pnode, *pitem, *plast;
	
	hitem = NULL;
	pmxml = NULL;
	hnode = qlinkTail(QLINK_ATTR, mhAttrLink);
	while(hnode)
	{
		pnode = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hnode);
		if(pnode == NULL)
		{
			break;
		}
		phead = name;
		plast = pnode;
		while(phead)
		{
			ptail = (QSTR)qstrchr(0, phead, 0, '.');
			if(ptail == NULL)
			{
				nlen = 0;
			}
			else
			{
				nlen = (QINT)(ptail-phead);
				if(nlen <= 0)
				{
					phead = ptail+1;
					continue;
				}
			}
			hitem = qlinkHead(QLINK_ATTR, plast->hsublink);
			while(hitem)
			{
				pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
				if(pitem != NULL)
				{
					if(qstrcmp(QSTR_CMP_ICASE, pitem->pname, phead, nlen))
					{
						if(ptail != NULL)
						{
							break;
						}
						else
						{
							if(pitem->nflag == flag)
							{
								break;
							}
						}
					}
				}
				hitem = qlinkNext(QLINK_ATTR, hitem);
			}
			if(hitem == NULL)
			{
				break;
			}
			pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
			if(pitem == NULL)
			{
				break;
			}
			if(ptail == NULL)
			{
				break;
			}
			phead = ptail+1;
			plast = pitem;
		}
		if(hitem != NULL)
		{
			break;
		}
		pmxml = NULL;
		if(pnode->pxmlval != NULL)
		{
			phead = name;
			ptail = (QSTR)qstrchr(0, phead, 0, '.');
			if(ptail == NULL)
			{
				nlen = 0;
			}
			else
			{
				nlen = (QINT)(ptail-phead);
			}
			pid = (QSTR)qxmlGetId(pnode->pxmlval);
			if(pid != NULL && qstrcmp(QSTR_CMP_ICASE, pid, phead, nlen))
			{
				if(ptail == NULL)
				{
					phead = NULL;
				}
				else
				{
					phead = ptail+1;
				}
			}
			else
			{
				phead = name;
			}
			pmxml = pnode->pxmlval;
			while(phead && pmxml)
			{
				ptail = (QSTR)qstrchr(0, phead, 0, '.');
				if(ptail == NULL)
				{
					nlen = 0;
				}
				else
				{
					nlen = (QINT)(ptail-phead);
					if(nlen <= 0)
					{
						phead = ptail+1;
						continue;
					}
				}
				pchild = (QXML)qxmlGetHead(pmxml);
				while(pchild)
				{
					pid = (QSTR)qxmlGetId(pchild);
					if(pid != NULL && qstrcmp(QSTR_CMP_ICASE, pid, phead, nlen))
					{
						if(ptail != NULL)
						{
							break;
						}
						else
						{
							nflag = qxmlGetFlag(pchild);
							if((nflag&QMDL_MASK) == flag)
							{
								break;
							}
						}
					}
					pchild = (QXML)qxmlGetNext(pchild);
				}
				pmxml = pchild;
				if(ptail == NULL)
				{
					break;
				}
				phead = ptail+1;
			}
			if(pmxml != NULL)
			{
				break;
			}
		}
		hnode = qlinkPrev(QLINK_ATTR, hnode);
	}
	
	if(pmxml != NULL)
	{
		return pmxml;
	}
	
	return (QHDL)hitem;
}

static QINT qxml_set_attr_scan_prev_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pid;
	QPCB pcbval;
	QPNT *ppprms;
	QModule *pmodule;
	QINT nflag, ncount;
	
	pid = (QSTR)qxmlGetId(hdl);
	if(pid != NULL)
	{
		return QSCN_OK;
	}
	pmodule = (QModule *)params[0];
	if(pmodule == NULL)
	{
		return QSCN_END;
	}
	nflag = qxmlGetFlag(hdl) & QMDL_MASK;
	if(nflag != (QINT)params[1])
	{
		return QSCN_OK;
	}
	pcbval = (QPCB)qxmlGetValcb(hdl);
	if(pcbval == NULL)
	{
		return QSCN_OK;
	}
	ppprms = (QPNT *)params[2];
	ncount = (QINT )params[3];
	pcbval((QHDL )pmodule, pmodule->mpPath, QCD_CALL, ppprms, ncount);
	
	return QSCN_OK;
}

void QModule::SetAttrx(QMDL env, QSTR name, QPNT params[], QINT count)
{
	QXML pmxml;
	QHDL hnode, hitem;
	struct qmdl_attr_item *pnode, *pitem;
	
	if(name == NULL)
	{
		// 必须执行的初始化，以最终的URL和OBJ信息。
		hnode = qlinkTail(QLINK_ATTR, mhAttrLink);
		while(hnode)
		{
			pnode = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hnode);
			if(pnode == NULL)
			{
				break;
			}
			hitem = qlinkHead(QLINK_ATTR, pnode->hsublink);
			while(hitem)
			{
				pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
				if(pitem != NULL)
				{
					if(pitem->pname == NULL && pitem->nflag == QMDL_ATTRIBUTE)
					{
						if(pitem->pcbval != NULL)
						{
							pitem->pcbval((QHDL )this, mpPath, QCD_CALL, params, count);
						}
					}
				}
				hitem = qlinkNext(QLINK_ATTR, hitem);
			}
			if(pnode->pxmlval != NULL)
			{
				QINT ncount;
				QPNT vpprms[8];
				
				ncount = 0;
				vpprms[ncount++] = (QPNT )this;
				vpprms[ncount++] = (QPNT )QMDL_ATTRIBUTE;
				vpprms[ncount++] = (QPNT )params;
				vpprms[ncount++] = (QPNT )count;
				qxmlScanx(pnode->pxmlval, qxml_set_attr_scan_prev_cb, NULL, vpprms, ncount);
			}
			hnode = qlinkPrev(QLINK_ATTR, hnode);
		}
	}
	else
	{
		hitem = GetAttrItem(name, QMDL_ATTRIBUTE);
		if(hitem == NULL)
		{
            hitem = (QHDL)GetAttrItem((QSTR)"attrcb", QMDL_ATTRIBUTE);
		}
        if(hitem != NULL)
        {
            QPCB pcbval;
            QPNT vpprms[8];
            QCHR vbuff[QSTR_BUFF_SIZE];
            QINT nlen, nsize, ncount;
            QPNT pval, psrctype, pdsttype;

            pdsttype = NULL;
            psrctype = params[1];
			pval = NULL;
            pcbval = NULL;
		    pmxml = (QXML)qmcheck(hitem, QLCN_HXML);
		    if(pmxml != NULL)
		    {
                pdsttype = qxmlGetTag(pmxml);
                pval = (QPNT)qxmlGetValp(pmxml);
			    pcbval = qxmlGetValcb(pmxml);
		    }
		    else
		    {
			    pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
			    if(pitem != NULL)
			    {
                    pdsttype = pitem->ptype;
                    pval = (QPNT)pitem->pval;
                    pcbval = pitem->pcbval;
			    }
		    }
            if(pcbval != NULL)
            {
                nlen = 0;
                if(psrctype == qstr || psrctype == NULL)
                {
                    nlen = qmdlCopy(this, NULL, NULL, 0, this->mpPath, vbuff, sizeof(vbuff), pval, NULL);
                    pval = vbuff;
                }
                nsize = sizeof(vbuff);
                pval = qprmconvert(pdsttype, vbuff, &nsize, psrctype, pval, nlen);
                nlen = nsize;
                ncount = 0;
                vpprms[ncount++] = (QPNT )params[0];
                vpprms[ncount++] = (QPNT )params[1];
                vpprms[ncount++] = (QPNT )pval;
                vpprms[ncount++] = (QPNT )nlen;
                pcbval((QHDL )this, name, QCD_CALL, params, count);
            }
        }
	}
}

void QModule::SetAttr(QMDL env, QSTR name, QINT count, ...)
{
	QPNT *ppparams;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;
	
	va_start( marker, count );
	ppparams = (QPNT *)qprmmakev(NULL, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
	va_end( marker );
	
	SetAttrx(env, name, ppparams, count);
	
	qprmfree(ppparams);
}

void QModule::ClrAttrLink()
{
	if(mhAttrLink != NULL)
	{
		qlinkFreeLinkx(QLINK_ATTR, mhAttrLink, qmdl_attr_item_free_cb, NULL, 0);
		mhAttrLink = NULL;
	}
}

static QINT qurl_init_scan_prev_cb(QPNT url, QPNT name, QPNT value, QINT size, QPNT params[], QINT count)
{
	QPNT vpprms[8];
	QPNT purl, pbuff;
	QMDL penv, pmodule;
	QINT nsize, ncount, nlen;
	
	penv = (QMDL)params[0];
	pmodule = (QMDL)params[1];
	purl = (QPNT)params[2];
	pbuff = (QPNT)params[3];
	nsize = (QINT)params[4];
	if(pmodule == NULL)
	{
		return QSCN_END;
	}
	nlen = qmdlCopy(pmodule, NULL, NULL, 0, purl, pbuff, nsize, value, &size);
	ncount = 0;
	vpprms[ncount++] = (QPNT )penv;
    vpprms[ncount++] = (QPNT )qstr;
	vpprms[ncount++] = (QPNT )pbuff;
	vpprms[ncount++] = (QPNT )nlen;
	((QModule *)pmodule)->SetAttrx(penv, (QSTR )name, vpprms, ncount);
	
	return QSCN_OK;
}

QINT qmdlInit(QMDL env, QMDL module, QXML mxml, QSTR url)
{
	QSTR pid;
	QHDL hitem;
	QModule *pmodule;
	QXML pattr, pmxml;
	QINT ncount, nlen, nsize;
	QPNT psrctype, pdsttype, pval, vpprms[8];
	QCHR vbuff[QSTR_BUFF_SIZE];
	struct qmdl_attr_item *pitem;
	
	pmodule = dynamic_cast<QModule*>(module);
	if(pmodule == NULL)
	{
		return QNO_FAIL;
	}
	if(pmodule->mhAttrLink == NULL)
	{
		return QNO_FAIL;
	}
	// 必须执行的初始化。
	ncount = 0;
	vpprms[ncount++] = (QPNT )env;
    vpprms[ncount++] = (QPNT )NULL;
	vpprms[ncount++] = (QPNT )mxml;
	vpprms[ncount++] = (QPNT )url;
	pmodule->SetAttrx(env, NULL, vpprms, ncount);
	if(mxml != NULL)
	{
		// xml属性初始化。
		pattr = (QXML)qxmlAttrGetHead(mxml, 0);
		while(pattr)
		{
			pid = (QSTR)qxmlGetId(pattr);
			psrctype = (QPNT)qxmlGetTag(pattr);
			if(psrctype == NULL)
			{
				psrctype = (QPNT)qstr;
			}
			hitem = (QHDL)pmodule->GetAttrItem((QSTR)pid, QMDL_ATTRIBUTE);
			if(hitem == NULL)
			{
				hitem = (QHDL)pmodule->SetAttrItem((QSTR)pid, QMDL_ATTRIBUTE);
				pitem = (struct qmdl_attr_item*)qlinkData(QLINK_ATTR, hitem);
				if(pitem != NULL)
				{
					pitem->ptype = psrctype;
					pitem->pval = (QPNT)qxmlGetValp(pattr);
				}
			}
			else
			{
				pdsttype = NULL;
				pmxml = (QXML)qmcheck(hitem, QLCN_HXML);
				if(pmxml != NULL)
				{
					pdsttype = qxmlGetTag(pmxml);
				}
				pval = (QPNT)qxmlGetValp(pattr);
				nlen = 0;
				if(psrctype == qstr || psrctype == NULL)
				{
					nlen = qmdlCopy(module, NULL, NULL, 0, url, vbuff, sizeof(vbuff), pval, NULL);
					pval = vbuff;
				}
				nsize = sizeof(vbuff);
				pval = qprmconvert(pdsttype, vbuff, &nsize, psrctype, pval, nlen);
				nlen = nsize;
				ncount = 0;
				vpprms[ncount++] = (QPNT )env;
                vpprms[ncount++] = (QPNT )psrctype;
				vpprms[ncount++] = (QPNT )pval;
				vpprms[ncount++] = (QPNT )nlen;
				if(pmxml != NULL)
				{
					QPCB pcbval;
					
					pcbval = qxmlGetValcb(pmxml);
					if(pcbval != NULL)
					{
						pcbval((QHDL)pmodule, pid, QCD_SET, vpprms, ncount);
					}
				}
				else
				{
					pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
					if(pitem == NULL)
					{
						if(pitem->pcbval != NULL)
						{
							pitem->pcbval((QHDL)pmodule, pid, QCD_SET, vpprms, ncount);
						}
					}
				}
				
			}
			pattr = (QXML)qxmlAttrGetNext(pattr, 0);
		}
	}
	if(url != NULL)
	{
		// url变量初始化。
		ncount = 0;
		vpprms[ncount++] = (QPNT )env;
		vpprms[ncount++] = (QPNT )module;
		vpprms[ncount++] = (QPNT )url;
		vpprms[ncount++] = (QPNT )vbuff;
		vpprms[ncount++] = (QPNT )sizeof(vbuff);
		qurlScanx((QPNT )url, qurl_init_scan_prev_cb, vpprms, ncount);
	}
	
	return QNO_OK;
}

static QINT qmdl_find_module_by_name_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QINT nnamelen;
	QPNT psrcname, pdstname;
	QMDL pmodule, *ppfound;
	
	pmodule = (QMDL )hdl;
	if(pmodule == NULL)
	{
		return QSCN_OK;
	}
	psrcname = (QPNT)params[0];
	nnamelen = (QINT)params[1];
	pdstname = pmodule->GetName();
	if(!qstrcmp(QSTR_CMP_ICASE, psrcname, pdstname, nnamelen))
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

static QMDL qmdlMakeByMxml(QMDL env, QMDL parent, QMDL module, QXML mxml, QSTR url)
{
	QPMK pcbmake;
	QPCB pcbval;
	QPNT pid, pval;
	QPNT vpprms[8];
	QINT nval, nlen, nflag, ndepth, nscan, ninit, ncount;
	QCHR vbuff[QSTR_BUFF_SIZE];
	QMDL vpcurmdl[QSCN_STACK_CNT] = {0};
	QMDL pmodule, pcurmdl, pparentmdl, pdstmdl, pfindmdl, pcssmdl, pnewmdl;
	QXML pmxml, pcurmxml, prootmxml, pparentmxml, pchildmxml, pattrmxml;
	
	if(env == NULL && module == NULL)
	{
		return NULL;
	}
	prootmxml = mxml;
	if(prootmxml == NULL)
	{
		return module;
	}
	ndepth = 0;
	pdstmdl = module;
	pcurmdl = module;
	if(parent != NULL)
	{
		pparentmdl = parent;
	}
	else if(module != NULL)
	{
		pparentmdl = module->ParentModule();
	}
	else
	{
		pparentmdl = NULL;
	}
	pcurmxml = prootmxml;
	while(pcurmxml)
	{
		pmodule = NULL;
		nscan = QSCN_OK;
		nflag = qxmlGetFlag(pcurmxml);
		nflag &= QMDL_MASK;
		if(nflag == QMDL_OBJECT)
		{
			// 模块结点。
			pfindmdl = NULL;
			if(pcurmxml == prootmxml)
			{
				pfindmdl = module;
			}
			else
			{
				// 非根结点。
				if(pcurmdl != NULL)
				{
					// 查找是否已存在的模块。
					pid = NULL;
					pattrmxml = (QXML)qxmlGetAttr(pcurmxml, QMDL_ATTR, (QPNT)"cssid", 0);
					if(pattrmxml != NULL)
					{
						pid = qxmlGetValp(pattrmxml);
					}
					else
					{
						pid = qxmlGetId(pcurmxml);
					}
					if(pid != NULL)
					{
						ncount = 0;
						vpprms[ncount++] = (QPNT)pid;
						vpprms[ncount++] = (QPNT)0;
						vpprms[ncount++] = (QPNT)&pfindmdl;
						vpprms[ncount++] = (QPNT)NULL;
						pcurmdl->ScanModulex(pcurmdl, qmdl_find_module_by_name_cb, NULL, vpprms, ncount);
					}
				}
			}
			pcssmdl = NULL;
			if(pfindmdl == NULL)
			{
				// 查找不到模块进行模版处理。
				pcbmake = (QPMK)qxmlGetValcb(pcurmxml);
				pval = qxmlGetValp(pcurmxml);
				if(pval == NULL)
				{
					if(pcbmake != NULL)
					{
						pcssmdl = pcbmake(env, pparentmdl, NULL);
					}
				}
				else
				{
					nlen = qurlCopy(url, vbuff, sizeof(vbuff), pval, NULL);
					if(pcbmake != NULL)
					{
						if(nlen <= 0)
						{
							pcssmdl = pcbmake(env, pparentmdl, NULL);
						}
						else
						{
							pcssmdl = pcbmake(env, pparentmdl, (QSTR)vbuff);
						}
					}
					else
					{
						if(nlen > 0)
						{
							if(vbuff[0] == '~')
							{
								// 使用现有的模板初始化。
								pmxml = (QXML)qxmlFind(prootmxml, vbuff);
								if(pmxml != NULL)
								{
									qmdlMakeByMxml(env, pparentmdl, pcurmdl, pmxml, (QSTR)vbuff);
								}
							}
							else
							{
								// 创建新的模块。
								if(env != NULL)
								{
									pnewmdl = env->FindModule((QSTR)vbuff, 0, NULL, 0);
								}
								else
								{
									pnewmdl = module->FindModule((QSTR)vbuff, 0, NULL, 0);
								}
								if(pnewmdl != NULL)
								{
									pcbmake = pnewmdl->GetMakeCb();
									if(pcbmake != NULL)
									{
										pcssmdl = pcbmake(env, pparentmdl, (QSTR)vbuff);
									}
								}
							}
						}
					}
				}
			}
			// 遍历css模板进行处理。
			pattrmxml = (QXML)qxmlAttrGetHead(pcurmxml, 0);
			while(pattrmxml)
			{
				pid = qxmlGetId(pattrmxml);
				if(pid != NULL && qstrcmp(QSTR_CMP_ICASE, (QPNT)pid, (QPNT)"css", 0))
				{
					pval = qxmlGetValp(pattrmxml);
					if(pval != NULL)
					{
						pmxml = (QXML)qmcheck(pval, QLCN_HXML);
						if(pmxml != NULL)
						{
							// 直接指向现有模板。
							pval = qxmlGetTag(pattrmxml);
							qmdlMakeByMxml(env, pparentmdl, pcurmdl, pmxml, (QSTR)pval);
						}
						else
						{
							// 路径指向。
							nlen = qurlCopy(url, vbuff, sizeof(vbuff), pval, NULL);
							if(nlen > 0)
							{
								if(vbuff[0] == '~')
								{
									// 使用现有的模板初始化。
									pmxml = (QXML)qxmlFind(prootmxml, vbuff);
									if(pmxml != NULL)
									{
										qmdlMakeByMxml(env, pparentmdl, pcurmdl, pmxml, (QSTR)vbuff);
									}
								}
								else
								{
									// 创建新的模块。
									if(env != NULL)
									{
										pnewmdl = env->FindModule((QSTR)vbuff, 0, NULL, 0);
									}
									else
									{
										pnewmdl = module->FindModule((QSTR)vbuff, 0, NULL, 0);
									}
									if(pnewmdl != NULL)
									{
										pcbmake = pnewmdl->GetMakeCb();
										if(pcbmake != NULL)
										{
											pnewmdl = pcbmake(env, pparentmdl, (QSTR)vbuff);
											if(pnewmdl != NULL)
											{
												if(pcssmdl == NULL)
												{
													pcssmdl = pnewmdl;
												}
												else
												{
													pcssmdl->RegisterModule(NULL, pnewmdl);
												}
											}
										}
									}
								}
							}
						}
					}//pval != NULL
				}//qstrcmp(QSTR_CMP_ICASE, (QPNT)pid, (QPNT)"css", 0)
				pattrmxml = (QXML)qxmlAttrGetNext(pattrmxml, QMDL_ATTR);
			}
			if(pcssmdl != NULL)
			{
				pid = qxmlGetId(pcurmxml);
				if(pid != NULL)
				{
					pcssmdl->SetName((QSTR)pid);
				}
			}
			if(pfindmdl != NULL)
			{
				pmodule = pfindmdl;
			}
			else if(pcssmdl != NULL)
			{
				pmodule = pcssmdl;
			}
			else
			{
				pmodule = new QModule();
				if(pmodule == NULL)
				{
					break;
				}
			}
			pchildmxml = (QXML)qxmlGetChild(pcurmxml, QMDL_ATTRIBUTE, 0);
			while(pchildmxml)
			{
				((QModule *)pmodule)->SetAttrXml(0, pchildmxml);
				pchildmxml = (QXML)qxmlGetSibling(pchildmxml, QMDL_ATTRIBUTE, 1);
			}
			if(pcurmdl != NULL && pmodule != pcurmdl)
			{
				pcurmdl->RegisterModule(NULL, pmodule);
			}
			nscan = pmodule->MakeModuleBegin(env, pparentmdl, pcurmxml, url);
			ninit = pmodule->MakeModuleInit(env, pparentmdl, pcurmxml, url);
			if(ninit == QNO_TRUE)
			{
				// FIXME：继承链上的每个类都做此操作，需要避免。
				if(((QModule *)pmodule)->mnInitDepth <= 0)
				{
					qmdlInit(env, pmodule, pcurmxml, url);
				}
			}
			if(pdstmdl == NULL)
			{
				pdstmdl = pmodule;
			}
		}
        else if(nflag == QMDL_INCLUDE)
        {
			// 声明结点。
			if(pcurmdl != NULL)
			{
				pcbmake = (QPMK )qxmlGetValcb(pcurmxml);
				if(pcbmake != NULL)
				{
					pid = qxmlGetId(pcurmxml);
					pval = qxmlGetValp(pcurmxml);
					if(pval == NULL)
					{
						pmodule = new QModule((QSTR )pid, 0);
						pcurmdl->RegisterModule(NULL, pmodule);
					}
					else
					{
						QCHR vpath[QSTR_BUFF_SIZE];
						
						pcurmdl->GetPath(NULL, vpath, sizeof(vpath));
						qurlCopy(url, vbuff, sizeof(vbuff), pval, NULL);
						qurlExpandPath(vpath, vbuff, sizeof(vbuff));
						if(pid == NULL)
						{
							pcurmdl->RegisterModule((QSTR )vbuff, pcbmake);
						}
						else
						{
							pmodule = new QModule((QSTR )pid, 0);
							if(pmodule != NULL)
							{
								pmodule->SetMakeCb(pcbmake);
								pcurmdl->RegisterModule((QSTR )vbuff, pmodule);
							}
						}
					}
				}
			}
		}
		else if(nflag == QMDL_METHOD)
		{
			// 方法结点。
			if(pcurmdl != NULL)
			{
				pid = (QSTR )qxmlGetId(pcurmxml);
				nval = qxmlGetValn(pcurmxml);
				pcbval = qxmlGetValcb(pcurmxml);
				if(pcbval != NULL)
				{
					pcurmdl->SetMethod((QSTR )pid, nval, NULL, pcbval);
				}
			}
		}
		else if(nflag == QMDL_SELFCB)
		{
			// 监听结点。
			if(pcurmdl != NULL)
			{
				pcbval = qxmlGetValcb(pcurmxml);
				if(pcbval != NULL)
				{
					pcurmdl->SetSelfCb(pcbval);
				}
			}
		}
		else if(nflag == QMDL_USERCB)
		{
			// 监听结点。
			if(pcurmdl != NULL)
			{
				pcbval = qxmlGetValcb(pcurmxml);
				if(pcbval != NULL)
				{
					pcurmdl->SetUserCb(pcbval);
				}
			}
		}
		else if(nflag == QMDL_WAITCB)
		{
			// 监听结点。
			if(pcurmdl != NULL)
			{
				pid = (QSTR )qxmlGetId(pcurmxml);
				pval = (QSTR )qxmlGetValp(pcurmxml);
				nval = qxmlGetValn(pcurmxml);
				pcbval = qxmlGetValcb(pcurmxml);
				if(pcbval != NULL)
				{
					if(pid == NULL)
					{
						pfindmdl = pcurmdl;
					}
					else
					{
						pfindmdl = pcurmdl->FindModule((QSTR)pid, 0, NULL, 0);
					}
					pcurmdl->WaitEvent(pfindmdl, (QSTR )pval, nval, 0, NULL, pcbval);
				}
			}
		}
		else if(nflag == QMDL_CLASS)
		{
			if(pcurmxml != prootmxml)
			{
				nscan = QSCN_HORZ;
			}
		}
		if(nflag == QMDL_OBJECT)
		{
			// 模块结点。
			pparentmdl = pcurmdl;
			pcurmdl = pmodule;
			vpcurmdl[ndepth] = pmodule;
			ndepth++;
		}
		else
		{
			// 其他结点。
			if(ndepth <= 0)
			{
				vpcurmdl[0] = pcurmdl;
			}
			else
			{
				vpcurmdl[ndepth] = vpcurmdl[ndepth-1];
			}
			ndepth++;
		}
		if(nscan == QSCN_END)
		{
			break;
		}
		if(nscan == QSCN_HORZ)
		{
			pchildmxml = NULL;
		}
		else
		{
			pchildmxml = (QXML)qxmlGetHead(pcurmxml);
		}
		if(pchildmxml == NULL)
		{
			ndepth--;
			if(ndepth <= 0)
			{
				pcurmdl->MakeModuleEnd(env, pcurmxml, url);
				pcurmdl = module;
			}
			else if(pcurmdl != vpcurmdl[ndepth-1])
			{
				pcurmdl->MakeModuleEnd(env, pcurmxml, url);
				pcurmdl = vpcurmdl[ndepth-1];
			}
			if(ndepth >= 0 && pcurmxml != prootmxml)
			{
				pparentmxml = pcurmxml;
				pchildmxml = (QXML)qxmlGetNext(pparentmxml);
				while(pchildmxml == NULL)
				{
					pparentmxml = (QXML)qxmlGetParent(pparentmxml);
					if(pparentmxml == NULL)
					{
						break;
					}
					ndepth--;
					if( (ndepth <= 0) || (ndepth > 0 && pcurmdl != vpcurmdl[ndepth-1]) )
					{
						pcurmdl->MakeModuleEnd(env, pparentmxml, url);
					}
					if(ndepth <= 0 || pcurmxml == prootmxml)
					{
						// 遍历到起始结点，退出遍历。
						pcurmdl = module;
						break;
					}
					pcurmdl = vpcurmdl[ndepth-1];
					pchildmxml = (QXML)qxmlGetNext(pparentmxml);
				}
			}
			if(pcurmdl == NULL)
			{
				pparentmdl = parent;
			}
			else
			{
				pparentmdl = pcurmdl->ParentModule();
				if(pparentmdl == NULL)
				{
					pparentmdl = parent;
				}
			}
		}
		pcurmxml = pchildmxml;
	}
	
	return pdstmdl;
}

static QMDL qmdlMakeByUrl(QMDL env, QMDL parent, QMDL module, QXML mxml, QSTR url)
{
	QPMK pcbmake;
	QINT nlen, nflag;
	QUCHR *purl, *pend, vurl[QSTR_BUFF_SIZE], vbuff[QSTR_BUFF_SIZE];
	QMDL pmodule, pfindmdl, pdstmdl, pcssmdl, pnewmdl, pparentmdl;
	
	qstrcpy(vurl, sizeof(vurl), url, 0);
	purl = vurl;
	pdstmdl = module;
	while(1)
	{
		pend = (QUCHR *)qstrchr(0, (QPNT )purl, 0, ';');
		if(pend != NULL)
		{
			*pend = 0;
		}
		pfindmdl = NULL;
		nlen = qurlGetVar((QPNT )purl, (QPNT )"id", (QPNT )vbuff, sizeof(vbuff));
		if(nlen > 0)
		{
			if(qurlIsPath((QPNT)vbuff))
			{
				pfindmdl = module->FindModule((QSTR )vbuff, 0, NULL, 0);
			}
			else
			{
				pfindmdl = module->FindModule(NULL, 0, (QSTR )vbuff, 0);
			}
		}
		pcssmdl = NULL;
		if(pfindmdl == NULL)
		{
			// 查找不到模块，进行模版处理。
			nlen = qurlGetPath((QPNT )purl, (QPNT )vbuff, sizeof(vbuff));
			if(nlen > 0)
			{
				pnewmdl = module->FindModule((QSTR)vbuff, 0, NULL, 0);
				if(pnewmdl != NULL)
				{
					nflag = pnewmdl->GetFlag();
					if(nflag & QFLAG_CLAXX)
					{
						pcbmake = pnewmdl->GetMakeCb();
						if(pcbmake != NULL)
						{
							pcssmdl = pcbmake(env, parent, (QSTR )vbuff);
						}
					}
				}
			}
		}
		if(pfindmdl != NULL)
		{
			pmodule = pfindmdl;
		}
		else if(pcssmdl != NULL)
		{
			pmodule = pcssmdl;
		}
		else
		{
			pmodule = module;
			if(pmodule == NULL)
			{
				break;
			}
		}
		if(module != NULL && pmodule != module)
		{
			module->RegisterModule((QSTR )vbuff, pmodule);
			pparentmdl = module;
		}
		else
		{
			pparentmdl = parent;
		}
		pmodule->MakeModuleBegin(env, pparentmdl, NULL, url);
		nflag = pmodule->MakeModuleInit(env, pparentmdl, NULL, url);
		if(nflag == QNO_TRUE)
		{
			qmdlInit(env, pmodule, NULL, url);
		}
		if(pdstmdl == NULL)
		{
			pdstmdl = pmodule;
		}
		if(pend == NULL)
		{
			break;
		}
		purl = pend+1;
	}
	
	return pdstmdl;
}

QMDL qmdlMake(QMDL env, QMDL parent, QPMK cls, QMDL module, QXML mxml, QSTR url)
{
	// FIXME：函数内buff可以复用，避免栈过大膨胀。
	QMDL pdst;
	
	pdst = NULL;
	if(cls != NULL)
	{
		pdst = cls(env, parent, NULL);
	}
	else
	{
		pdst = module;
	}
	if(mxml != NULL)
	{
		pdst = qmdlMakeByMxml(env, parent, pdst, mxml, url);
	}
	else if(url != NULL)
	{
		pdst = qmdlMakeByUrl(env, parent, pdst, mxml, url);
	}
	if(pdst == NULL)
	{
		pdst = new QModule();
		if(env != NULL)
		{
			((QModule *)pdst)->main = env->MainModule();
		}
	}
	
	return pdst;
}
