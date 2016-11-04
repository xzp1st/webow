
#include "qmdl_module.h"
#include "qmdl_module_internal.h"


struct qmdl_event_item
{
    QMDL pobserved;
    QMDL pobserver;
    QSTR pname;
    QINT ncode;
	QINT nflag;
    QPFN pfnevent;
	QPCB pcbevent;
	QHDL hdefine;
    
    QHDL hobsditem;
    QHDL hobsritem;
};

static QINT qmdl_observer_free_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
    QModule *pmodule;
    struct qmdl_event_item *pevent, **ppevent;
    
	ppevent = (struct qmdl_event_item **)qlinkData(QLINK_NONE, hdl);
    if(ppevent == NULL)
    {
        return QSCN_END;
    }
    pevent = *ppevent;
    if(pevent == NULL)
    {
        return QSCN_OK;
    }
    pmodule = (QModule *)pevent->pobserver;
    if(pmodule != NULL)
    {
        pmodule->RemObserver((QHDL )pevent);
    }
    pmodule = (QModule *)pevent->pobserved;
    if(pmodule != NULL)
    {
        pmodule->RemObserved((QHDL )pevent);
    }
    if(pevent->pname != NULL)
    {
        qmfree(pevent->pname);
		pevent->pname = NULL;
    }
    pevent->ncode = 0;
	pevent->pcbevent = NULL;
    pevent->pfnevent = NULL;
    qmfree(pevent);
    
	return QSCN_OK;
}

QHDL QModule::SetObserver(QHDL observer)
{
    QHDL hitem;
	struct qmdl_event_item *pevent, **ppevent;
	
    pevent = (struct qmdl_event_item *)observer;
    if(pevent == NULL)
    {
        return NULL;
    }
	hitem = qlinkHead(QLINK_NONE, mhObsrLink);
	while(hitem)
	{
		ppevent = (struct qmdl_event_item **)qlinkData(QLINK_NONE, hitem);
		if(ppevent != NULL)
		{
			if(*ppevent == pevent)
			{
				break;
			}
		}
		hitem = qlinkNext(QLINK_NONE, hitem);
	}
	if(hitem == NULL)
	{
		hitem = qlinkMake(QLINK_NONE, NULL, &observer, sizeof(QHDL));
		if(hitem == NULL)
		{
			return NULL;
		}
		mhObsrLink = qlinkInsertNext(QLINK_NONE, mhObsrLink, NULL, hitem);
	}
    pevent->pobserver = this;
    pevent->hobsritem = hitem;

	return hitem;
}

void QModule::RemObserver(QHDL observer)
{
    struct qmdl_event_item *pevent;
    
    pevent = (struct qmdl_event_item *)observer;
    if(pevent == NULL)
    {
        return;
    }
    if(pevent->hobsritem == NULL)
    {
        return;
    }
    mhObsrLink = qlinkRemove(QLINK_NONE, mhObsrLink, pevent->hobsritem);
    qlinkFreeItemx(QLINK_NONE, pevent->hobsritem, NULL, NULL, 0);
	if(pevent->hobsditem == pevent->hobsritem)
	{
		pevent->hobsditem = NULL;
	}
	pevent->hobsritem = NULL;
}

QHDL QModule::SetObserved(QHDL observer)
{
	QHDL hitem;
	struct qmdl_event_item *pevent, **ppevent;
	
	pevent = (struct qmdl_event_item *)observer;
	if(pevent == NULL)
	{
		return NULL;
	}
	hitem = qlinkHead(QLINK_NONE, mhObsrLink);
	while(hitem)
	{
		ppevent = (struct qmdl_event_item **)qlinkData(QLINK_NONE, hitem);
		if(ppevent != NULL)
		{
			if(*ppevent == pevent)
			{
				break;
			}
		}
		hitem = qlinkNext(QLINK_NONE, hitem);
	}
	if(hitem == NULL)
	{
		hitem = qlinkMake(QLINK_NONE, NULL, &observer, sizeof(QHDL));
		if(hitem == NULL)
		{
			return NULL;
		}
		mhObsrLink = qlinkInsertNext(QLINK_NONE, mhObsrLink, NULL, hitem);
	}
    pevent->pobserved = this;
    pevent->hobsditem = hitem;

	return hitem;
}

void QModule::RemObserved(QHDL observer)
{
    struct qmdl_event_item *pevent;
    
    pevent = (struct qmdl_event_item *)observer;
    if(pevent == NULL)
    {
        return;
    }
    if(pevent->hobsditem == NULL)
    {
        return;
    }
    mhObsrLink = qlinkRemove(QLINK_NONE, mhObsrLink, pevent->hobsditem);
    qlinkFreeItemx(QLINK_NONE, pevent->hobsditem, NULL, NULL, 0);
	if(pevent->hobsritem == pevent->hobsditem)
	{
		pevent->hobsritem = NULL;
	}
	pevent->hobsditem = NULL;
}

void QModule::ClrObsrLink()
{
	if(mhObsrLink != NULL)
	{
		qlinkFreeLinkx(QLINK_NONE, mhObsrLink, qmdl_observer_free_cb, NULL, NULL);
		mhObsrLink = NULL;
	}
}

void QModule::ScanEventx(qmdl_event_scan_cb scan_cb, QPNT params[], QINT count)
{
	QHDL hitem;
	struct qmdl_event_item *pevent, **ppevent;
	
	if(scan_cb == NULL)
	{
		return;
	}
	hitem = qlinkHead(QLINK_NONE, mhObsrLink);
	while (hitem)
	{
		ppevent = (struct qmdl_event_item **)qlinkData(QLINK_NONE, hitem);
		if(ppevent != NULL)
		{
			pevent = *ppevent;
			if(pevent != NULL)
			{
				scan_cb(pevent->pobserved, pevent->pobserver, pevent->pname, pevent->ncode, pevent->pfnevent, pevent->pcbevent, params, count);
			}
		}
		hitem = qlinkNext(QLINK_NONE, hitem);
	}
}

void QModule::ScanEvent(qmdl_event_scan_cb scan_cb, QINT count, ...)
{
	QPNT *ppparams;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;
	
	va_start( marker, count );
	ppparams = (QPNT *)qprmmakev(NULL, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
	va_end( marker );
	
	ScanEventx(scan_cb, ppparams, count);
	
	qprmfree(ppparams);
	
}

void QModule::WaitEvent(QMDL module, QSTR name, QINT code, QINT flag, QPFN event_fn, QPCB event_cb)
{
	QHDL hitem;
	QINT nmatch;
	QMDL pobserved;
	struct qmdl_event_item *pevent, **ppevent;
	
	if(module == NULL)
	{
		pobserved = this;
	}
	else
	{
		pobserved = module;
	}
	pevent = NULL;
	hitem = qlinkHead(QLINK_NONE, mhObsrLink);
	while (hitem)
	{
		ppevent = (struct qmdl_event_item **)qlinkData(QLINK_NONE, hitem);
		if(ppevent != NULL)
		{
			pevent = *ppevent;
			if(pevent != NULL)
			{
				if( (pevent->pobserved == pobserved) && (pevent->pobserver == this) && !(pevent->nflag & QWAIT_NOREPLACE) )
				{
					nmatch = 1;
					if( (pevent->pname != NULL) && (name != NULL) && !qstrcmp(QSTR_CMP_NONE, pevent->pname, name, 0) )
					{
						nmatch = 0;
					}
					if( (pevent->ncode != QCD_CALL) && (code != QCD_CALL) && (pevent->ncode != code) )
					{
						nmatch = 0;
					}
					if(nmatch == 1)
					{
						break;
					}
				}
			}
		}
		hitem = qlinkNext(QLINK_NONE, hitem);
	}
	if(hitem == NULL)
	{
		pevent = (struct qmdl_event_item *)qmalloc(NULL, sizeof(struct qmdl_event_item));
		if (pevent == NULL)
		{
			return;
		}
		memset(pevent, 0, sizeof(struct qmdl_event_item));
		this->SetObserver((QHDL )pevent);
		((QModule *)pobserved)->SetObserved((QHDL )pevent);
	}
	if(pevent->pname != NULL)
	{
		qmfree(pevent->pname);
	}
	pevent->pname = (QSTR )qstrdup(NULL, name);
	pevent->ncode = code;
	pevent->nflag = flag;
	pevent->pfnevent = event_fn;
	pevent->pcbevent = event_cb;

    hitem = (QHDL)GetAttrItem((QSTR)"waitcb", QMDL_ATTRIBUTE);
    if(hitem != NULL)
    {
        QINT ncount;
        QPNT *ppprms;
        QCHR vpprms[QPRM_BUFF_SIZE] = {0};
        struct qmdl_attr_item *pitem;

        ppprms = (QPNT*)qprmmake(NULL, vpprms, sizeof(vpprms), NULL, NULL,
            &ncount, pobserved, pevent->pname, pevent->ncode, pevent->nflag, pevent->pfnevent, pevent->pcbevent, qnull);
        QXML pmxml = (QXML)qmcheck(hitem, QLCN_HXML);
        if(pmxml != NULL)
        {
            QPCB pcbval = qxmlGetValcb(pmxml);
            if(pcbval != NULL)
            {
                pcbval((QHDL)this, (QSTR)"waitcb", QCD_SET, ppprms, ncount);
            }
        }
        else
        {
            pitem = (struct qmdl_attr_item *)qlinkData(QLINK_ATTR, hitem);
            if(pitem != NULL)
            {
                if(pitem->pcbval != NULL)
                {
                    pitem->pcbval((QHDL )this, (QSTR)"waitcb", QCD_CALL, ppprms, ncount);
                }
                //else if(pitem->pfnval != NULL)
                //{
                //	(this->*pitem->pfnval)(this, name, QCD_CALL, vpprms, ncount);
                //}
            }
        }
    }
}

void QModule::DispatchEventx(QSTR name, QINT code, QPNT params[], QINT count)
{
	QINT nmatch;
    QHDL hitem, hnext;
    struct qmdl_event_item *pevent, **ppevent;
    
    hitem = qlinkHead(QLINK_NONE, mhObsrLink);
    while (hitem)
    {
		hnext = qlinkNext(QLINK_NONE, hitem);
        ppevent = (struct qmdl_event_item **)qlinkData(QLINK_NONE, hitem);
        if(ppevent != NULL)
        {
            pevent = *ppevent;
            if(pevent != NULL)
            {
                if(pevent->pobserved == this)
                {
					nmatch = 1;
					if( (pevent->pname != NULL) && (name != NULL) && !qstrcmp(QSTR_CMP_NONE, pevent->pname, name, 0) )
					{
						nmatch = 0;
					}
					if( (pevent->ncode != QCD_CALL) && (code != QCD_CALL) && (pevent->ncode != code) )
					{
						nmatch = 0;
					}
					if(nmatch == 1)
					{
						if(pevent->pfnevent != NULL)
						{
							(pevent->pobserver->*pevent->pfnevent)(pevent->pobserver, name, code, params, count);
						}
						if(pevent->pcbevent != NULL)
						{
							pevent->pcbevent(pevent->pobserver, name, code, params, count);
						}
						if(pevent->nflag & QWAIT_SINGLE)
						{
							qlinkFreeItemx(QLINK_NONE, mhObsrLink, qmdl_observer_free_cb, NULL, NULL);
						}
					}
				}
            }
        }
        hitem = hnext;
    }
}

void QModule::DispatchEvent(QSTR name, QINT code, QINT count, ...)
{
	QPNT *ppparams;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;
    
	va_start( marker, count );
	ppparams = (QPNT *)qprmmakev(NULL, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
	va_end( marker );
    
	DispatchEventx(name, code, ppparams, count);
    
	qprmfree(ppparams);
}

void IQModule::RouteEventx(QMDL parent, QSTR name, QINT code, QPNT params[], QINT count)
{
	QINT nresult;

	nresult = this->RouteCb(parent, name, code, params, count);
	if(nresult == QSCN_FAIL || nresult == QSCN_ERR || nresult == QSCN_END)
	{
		return;
	}
	this->DispatchEventx(name, code, params, count);
}

void IQModule::RouteEvent(QMDL parent, QSTR name, QINT code, QINT count, ...)
{
	QPNT *ppparams;
	QCHR vbuff[QPRM_BUFF_SIZE] = {0};
	va_list marker;
	
	va_start( marker, count );
	ppparams = (QPNT *)qprmmakev(NULL, vbuff, sizeof(vbuff), NULL, NULL, marker, &count);
	va_end( marker );
	
	RouteEventx(parent, name, code, ppparams, count);
	
	qprmfree(ppparams);
}
