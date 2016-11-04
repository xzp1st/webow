
#include "qmdl_module.h"


struct qmdl_method_item
{
	QSTR pname;
	QINT ncode;
	QPFN pfnmethod;
	QPCB pcbmethod;
};


void QModule::SetMethod(QSTR name, QINT code, QPFN method_fn, QPCB method_cb)
{
	QINT nmatch;
	QHDL hitem;
	struct qmdl_method_item *pitem;
	
	hitem = qlinkHead(QLINK_NONE, mhMthdLink);
	while(hitem)
	{
		pitem = (struct qmdl_method_item *)qlinkData(QLINK_NONE, hitem);
		if(pitem != NULL)
		{
			nmatch = 1;
			if(pitem->pname != NULL && name != NULL && !qstrcmp(QSTR_CMP_NONE, pitem->pname, name, 0))
			{
				nmatch = 0;
			}
			if(pitem->ncode != QCD_CALL && code != QCD_CALL && pitem->ncode != code)
			{
				nmatch = 0;
			}
			if(nmatch == 1)
			{
				break;
			}
		}
		hitem = qlinkNext(QLINK_NONE, hitem);
	}
	if(hitem == NULL)
	{
		hitem = qlinkMake(QLINK_NONE, NULL, NULL, sizeof(struct qmdl_method_item));
		mhMthdLink = qlinkInsertNext(QLINK_NONE, mhMthdLink, NULL, hitem);
	}
	pitem = (struct qmdl_method_item *)qlinkData(QLINK_NONE, hitem);
	if(pitem == NULL)
	{
		return;
	}
	pitem->pname = (QSTR )qstrdup(NULL, name);
	pitem->ncode = code;
	pitem->pfnmethod = method_fn;
	pitem->pcbmethod = method_cb;
}

void QModule::GetMethod(QSTR name, QINT code, QPFN *method_fn, QPCB *method_cb)
{
	QINT nmatch;
	QHDL hitem;
	struct qmdl_method_item *pitem;
	
	hitem = qlinkHead(QLINK_NONE, mhMthdLink);
	while(hitem)
	{
		pitem = (struct qmdl_method_item *)qlinkData(QLINK_NONE, hitem);
		if(pitem != NULL)
		{
			nmatch = 1;
			if(pitem->pname != NULL && name != NULL && !qstrcmp(QSTR_CMP_NONE, pitem->pname, name, 0))
			{
				nmatch = 0;
			}
			if(pitem->ncode != QCD_CALL && code != QCD_CALL && pitem->ncode != code)
			{
				nmatch = 0;
			}
			if(nmatch == 1)
			{
				break;
			}
		}
		hitem = qlinkNext(QLINK_NONE, hitem);
	}
	pitem = (struct qmdl_method_item *)qlinkData(QLINK_NONE, hitem);
	if(pitem == NULL)
	{
		if(method_fn != NULL)
		{
			*method_fn = NULL;
		}
		if(method_cb != NULL)
		{
			*method_cb = NULL;
		}
	}
	else
	{
		if(method_fn != NULL)
		{
			*method_fn = pitem->pfnmethod;
		}
		if(method_cb != NULL)
		{
			*method_cb = pitem->pcbmethod;
		}
	}
}

QINT QModule::CallMethodx(QSTR name, QINT code, QPNT params[], QINT count)
{
	QINT nmatch;
	QHDL hitem;
	struct qmdl_method_item *pitem;
	
	hitem = qlinkHead(QLINK_NONE, mhMthdLink);
	while(hitem)
	{
		pitem = (struct qmdl_method_item *)qlinkData(QLINK_NONE, hitem);
		if(pitem != NULL)
		{
			nmatch = 1;
			if(pitem->pname != NULL && name != NULL && !qstrcmp(QSTR_CMP_NONE, pitem->pname, name, 0))
			{
				nmatch = 0;
			}
			if(pitem->ncode != QCD_CALL && code != QCD_CALL && pitem->ncode != code)
			{
				nmatch = 0;
			}
			if(nmatch == 1)
			{
				break;
			}
		}
		hitem = qlinkNext(QLINK_NONE, hitem);
	}
	pitem = (struct qmdl_method_item *)qlinkData(QLINK_NONE, hitem);
	if(pitem != NULL)
	{
		return QCD_FAIL;
	}
	if(pitem->pfnmethod != NULL)
	{
		return (this->*pitem->pfnmethod)(this, name, code, params, count);
	}
	if(pitem->pcbmethod != NULL)
	{
		return pitem->pcbmethod(this, name, code, params, count);
	}
	
	return QCD_FAIL;
}

QINT QModule::CallMethod(QSTR name, QINT code, QINT count, ...)
{
	QINT nresult;
	QPNT *ppparams;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;
	
	va_start( marker, count );
	ppparams = (QPNT *)qprmmakev(NULL, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
	va_end( marker );
	
	nresult = CallMethodx(name, code, ppparams, count);
	
	qprmfree(ppparams);
	
	return nresult;
}
