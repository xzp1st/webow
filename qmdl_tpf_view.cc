
#include "qmdl_config.h"

#ifdef QMDL_VER_TPF

#include "qmdl_tpf.h"
#include "UIProperty/uiproperty_i.h"
#include "str_cov.h"


QTPFView::~QTPFView()
{
}

int QTPFView::GetEventVersion()
{
    return 0;
}

void QTPFView::OnControlMessage(DWORD id, IVariableParam* param)
{
    if(param == NULL || mpview == NULL)
    {
        return;
    }
    QINT ncount;
    QPNT *ppprms;
    QPNT vpprms[QPRM_BUFF_SIZE];
    int nevent = param->GetInt(EPARAMID_EVENTID, FALSE);
    if(nevent == EI_BASEMESSAGE)
    {
        UINT nmsg = param->GetInt(PARAMID_MESSAGE, FALSE);
        WPARAM wparam = (WPARAM)param->GetDword(PARAMID_WPARAM, FALSE);
        LPARAM lparam = (LPARAM)param->GetDword(PARAMID_LPARAM, FALSE);
        ppprms = (QPNT*)qprmmake(NULL, vpprms, sizeof(vpprms), NULL, NULL, &ncount, nmsg, wparam, lparam, qnull);
    }
    else
    {
        ppprms = (QPNT*)qprmmake(NULL, vpprms, sizeof(vpprms), NULL, NULL, &ncount, param, qnull);
    }
    QINT nresult = mpview->RouteCb(NULL, (QSTR)NULL, (QINT)nevent, (QPNT*)ppprms, (QINT)ncount);
    if(nresult == QNO_TRUE)
    {
        mpview->DispatchEventx((QSTR)NULL, (QINT)nevent, (QPNT*)ppprms, (QINT)ncount);
    }
    param->SetBYTEValue(EPARAMID_EVENTHANDLED, (BYTE)1);
}

QUIView *QUIView::GetRootView()
{
    QMDL pparent;
    QUIView *pview, *proot;

    proot = NULL;
    pparent = this;
    while(pparent)
    {
        pview = dynamic_cast<QUIView*>(pparent);
        if(pview != NULL)
        {
            proot = pview;
        }
        pparent = pparent->ParentModule();
    }

    return proot;
}

QUIView *QUIView::GetParentView()
{
    QMDL pparent;
    QUIView *pview;

    pparent = this;
    while(pparent)
    {
        pview = dynamic_cast<QUIView*>(pparent);
        if(pview != NULL && pview != this)
        {
            break;
        }
        pparent = pparent->ParentModule();
    }

    return (QUIView *)pparent;
}

QUIView::QUIView()
{
    mcview.mpview = this;
    mcview.mpviewtype = NULL;
    mcview.mpcommontype = NULL;
}

QUIView::~QUIView()
{
    QUIView *proot;

    proot = GetRootView();
    if(proot == this)
    {
        if(proot->mcview.mpuilib != NULL)
        {
            proot->mcview.ReleaseTenioComponent(&proot->mcview.mpuilib);
        }
    }
    proot->mcview.mpuilib = NULL;
}

QINT QUIView::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	return QSCN_OK;
}

QINT QUIView::MakeModuleEnd(QMDL env, QXML mxml, QSTR url)
{
	return QSCN_OK;
}

QINT QUIView::SetState(QINT state, QINT flag)
{
	QINT nstate;
	
	nstate = QNO_TRUE;
	switch(state)
	{
		case QStateVisual:
            if(mcview.mpcontrol != NULL)
            {
                mcview.mpcontrol->Show(flag);
            }
			break;
	}
	
	return nstate;
}

QINT QUIView::GetState(QINT state)
{
	QINT nstate;
	
	nstate = QNO_TRUE;
	switch(state)
	{
		case QStateVisual:
            if(mcview.mpcontrol == NULL)
            {
                nstate = QNO_FALSE;
            }
            else
            {
                nstate = mcview.mpcontrol->IsVisible();
            }
			break;
	}
	
	return nstate;
}

void QUIView::SetPosition(QRECT *rect)
{
	QMDL pparentmdl;
	QUIView *pview;
    RECT rcview, rcparent;
	
	if(rect == NULL)
	{
		return;
	}
	if(mcview.mpcontrol == NULL)
	{
		return;
	}
	rcview.left = rect->left;
	rcview.top = rect->top;
	pparentmdl = this;
	while(pparentmdl)
	{
		pview = dynamic_cast<QUIView *>(pparentmdl);
		if(pview != NULL)
		{
            if(pview->mcview.mpcontrol != NULL)
            {
                rcparent = pview->mcview.mpcontrol->GetRect(FALSE);
                rcview.left -= rcparent.left;
                rcview.top -= rcparent.top;
            }
		}
		pparentmdl = pparentmdl->ParentModule();
	}
    pview->mcview.mpcontrol->Move(rcview);
}

void QUIView::GetPosition(QRECT *rect)
{
    QMDL pparentmdl;
    QUIView *pview;
    RECT rcview, rcparent;
	
	if(rect == NULL)
	{
		return;
	}
    if(mcview.mpcontrol == NULL)
	{
		return;
	}
	rcview.left = 0;
	rcview.top = 0;
	pparentmdl = this;
	while(pparentmdl)
	{
		pview = dynamic_cast<QUIView *>(pparentmdl);
		if(pview != NULL)
		{
            if(pview->mcview.mpcontrol != NULL)
            {
                rcparent = pview->mcview.mpcontrol->GetRect(FALSE);
                if(pview == this)
                {
                    rcview.left = rcparent.left;
                    rcview.top = rcparent.top;
                    rcview.right = rcparent.right;
                    rcview.bottom = rcparent.bottom;
                }
                else
                {
                    rcview.left += rcparent.left;
                    rcview.top += rcparent.top;
                    rcview.right += rcparent.left;
                    rcview.bottom += rcparent.top;
                }
            }
		}
		pparentmdl = pparentmdl->ParentModule();
	}
	rect->left = rcview.left;
	rect->top = rcview.top;
	rect->right = rcview.right;
	rect->bottom = rcview.bottom;
}

QINT QUIViewSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIView *pview;
	
	pview = (QUIView*)hdl;
	if(pview == NULL)
	{
		return QNO_OK;
	}
	if(code == QCD_MAKE)
	{
        QUIView *proot = pview->GetRootView();
        if(proot == NULL)
        {
            return QNO_FAIL;
        }
        if(pview->mcview.mpuilib == NULL)
        {
            if(proot == pview)
            {
                QBOOL bresult = proot->mcview.CreateTenioComponent(&proot->mcview.mpuilib);
                if(bresult == FALSE)
                {
                    proot->mcview.mpuilib = NULL;
                }
            }
            else
            {
                pview->mcview.mpuilib = proot->mcview.mpuilib;
            }
        }
        if(pview->mcview.mpuilib == NULL)
        {
            return QNO_FAIL;
        }
        DWORD dwparent = 0;
        QUIView *pparent = pview->GetParentView();
        if(pparent != NULL)
        {
            dwparent = pparent->mcview.mdwcontrol;
        }
        if(pview->mcview.mpcontrol == NULL)
        {
            std::wstring strname;
            ierd_tgp::common::u8to16((char*)pview->mpName, strname);
            pview->mcview.mdwcontrol = pview->mcview.mpuilib->CreateControl(pview->mcview.mpparam, strname.c_str(), dwparent);
        }
	}
	
	return QNO_OK;
}

QINT QUIView::GetAttrByName(QMDL module, QXML mxml, QSTR url, QPNT name, QPNT value, QINT size)
{
	QINT nlen;
	QXML pattr;
	QPNT pvalue;
	
	nlen = 0;
	if(mxml != NULL)
	{
		pattr = (QXML)qxmlGetAttr((QHDL)mxml, 0, (QPNT)name, 0);
		if(pattr != NULL)
		{
			pvalue = (QPNT)qxmlGetValp((QHDL)pattr);
			nlen = size;
			nlen = qmdlCopy(module, NULL, NULL, 0, NULL, value, size, pvalue, &nlen);
		}
	}
	if(nlen <= 0)
	{
		if(url != NULL)
		{
			nlen = (QINT)qurlGetVar((QPNT)url, (QPNT)name, (QPNT)value, size);
		}
	}
	
	return nlen;
}

QHDL QUIView::FindViewByTag(QMDL env, QMDL parent, QMDL *module, QXML mxml, QSTR url, QINT *tag)
{
	QPNT ptag;
	QCHR vtag[QSTR_BUFF_SIZE];
	QINT ntag, nlen;
	QMDL pparent;
	QUIView *pview;
    IControl *pcontrol;
	QXML pattr;
	
	ntag = 0;
	if(mxml != NULL)
	{
		pattr = (QXML)qxmlGetAttr((QHDL)mxml, 0, (QHDL)"tag", 0);
		if(pattr != NULL)
		{
			ptag = (QPNT)qxmlGetValp((QHDL)pattr);
			if(ptag != NULL)
			{
				ntag = qstrint(0, ptag, NULL);
			}
		}
	}
	else if(url != NULL)
	{
		nlen = (QINT)qurlGetVar((QPNT)url, (QPNT)"tag", (QPNT)vtag, sizeof(vtag));
		if(nlen > 0)
		{
			ntag = qstrint(0, vtag, NULL);
		}
	}
	if(tag != NULL)
	{
		*tag = ntag;
	}
	if(ntag == 0)
	{
		if(module != NULL)
		{
			*module = NULL;
		}
		return NULL;
	}
    pcontrol = NULL;
    if(mcview.mpuilib != NULL)
    {
	    pcontrol = mcview.mpuilib->GetControlPtr(ntag);
	    if(pcontrol != NULL)
	    {
		    *module = this;
	    }
    }
	
	return pcontrol;
}

QINT QUIView::MakeModuleInit(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	if(parent != NULL)
	{
		QMDL pparent;
		QUIView *pview;
		
		if(mcview.mpcontrol != NULL && mcview.mpcontrol->GetParent() == NULL)
		{
			pparent = parent;
			while(pparent)
			{
				pview = dynamic_cast<QUIView *>(pparent);
				if(pview != NULL)
				{
					if(pview->mcview.mpcontrol != NULL)
					{
                        mcview.mpcontrol->SetParent(pview->mcview.mpcontrol);
						break;
					}
				}
				pparent = pparent->ParentModule();
			}
		}
	}
	
	return QNO_TRUE;
}

QINT QUIViewInitAttrcb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
    QUIView *pview;
    TTenioPtr<IUIProperty> pprop;

    pview = dynamic_cast<QUIView*>((QMDL)hdl);
    if(pview == NULL)
    {
        return QNO_FAIL;
    }
    if(pview->mcview.mpviewtype == NULL || pview->mcview.mpcommontype == NULL)
    {
        return QNO_FAIL;
    }
    std::wstring strname;
    ierd_tgp::common::u8to16((char*)name, strname);
    std::wstring strvalue;
    QPNT ptype = (QPNT)params[1];
    if(ptype == qint)
    {
        WCHAR vvalue[QSTR_BUFF_SIZE] ={0};
        pprop->Int2String((int)params[2], vvalue, sizeof(vvalue)/sizeof(WCHAR));
        strvalue = vvalue;
    }
    else
    {
        ierd_tgp::common::u8to16((char*)name, strvalue);
    }
    pprop->EnterPropertySection(pview->mcview.mpviewtype->dwtype);
    pprop->SetPropertyToParam((LPCWSTR)strname.c_str(), strvalue.c_str(), pview->mcview.mpparam);
    pprop->ExitPropertySection();
    if(pview->mcview.mpcontrol != NULL)
    {
        pview->mcview.mpcontrol->SetProperty(pview->mcview.mpparam);
        pview->mcview.mpparam->Clear();
    }

    return QNO_TRUE;
}

QINT QUIViewInitWaitcb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
    DWORD dweventid;
    QUIView *pmodule, *pobersed;

    pmodule = dynamic_cast<QUIView*>((QMDL)hdl);
    pobersed = dynamic_cast<QUIView*>((QMDL)params[0]);
    if(pmodule == NULL || pobersed == NULL)
    {
        return QNO_OK;
    }
    if(pmodule->mcview.mpuilib == NULL)
    {
        return QNO_FAIL;
    }
    dweventid = (DWORD)params[2];
    pmodule->mcview.mpuilib->RegisterEventHandle(dweventid, &pobersed->mcview);

    return QNO_OK;
}

static QINT qurl_uiview_prop_scan_cb(QPNT url, QPNT name, QPNT value, QINT size, QPNT params[], QINT count)
{
    QINT nsize;
    QCHR *pbuff;
    QUIView *pview;

    pview = (QUIView *)params[0];
    pbuff = (QCHR*)params[1];
    nsize = (QINT)params[2];
    if(pview == NULL || pbuff == NULL)
    {
        return QSCN_END;
    }

    return QSCN_OK;
}

QINT QUIViewInitBase(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
    QMDL penv;
    QINT nlen;
    QSTR purl;
    QPNT pvalue;
    QCHR vbuff[QSTR_BUFF_SIZE];
    QMain *pmain;
    QUIView *pview;
    QXML pxml, pattr;

    pview = (QUIView *)hdl;
    penv = (QMDL)params[0];
    if(pview == NULL || penv == NULL)
    {
        return QNO_FAIL;
    }
    pmain = (QMain *)penv->MainModule();
    if(pmain == NULL)
    {
        return QNO_FAIL;
    }
    pxml = (QXML)params[1];
    purl = (QSTR)params[2];
    // 找到控件对应的type
    std::map<string, QMainTypeInfo*>::iterator type;
    if(pview->mcview.mpviewtype == NULL)
    {
        nlen = pview->GetAttrByName(pview, pxml, purl, (QPNT)"type", vbuff, sizeof(vbuff));
        if(nlen <= 0)
        {
            return QNO_FAIL;
        }
        string strtype = vbuff;
        type = pmain->mmaptypes.find(strtype);
        if(type == pmain->mmaptypes.end())
        {
            return QNO_FAIL;
        }
        QMainTypeInfo *pinfo = type->second;
        if(pinfo == NULL)
        {
            return QNO_FAIL;
        }
        pview->mcview.mpviewtype = pinfo;
    }
    if(pview->mcview.mpcommontype == NULL)
    {
        type = pmain->mmaptypes.find("CommonProperty");
        if(type != pmain->mmaptypes.end())
        {
            pview->mcview.mpcommontype = type->second;
        }
    }
    // 保存控件对应的属性
    std::map<string, QMainPropInfo*>::iterator prop;
	if(pxml != NULL)
	{
		pattr = (QXML)qxmlAttrGetHead((QHDL)pxml, 0);
		while(pattr)
		{
			pvalue = (QPNT)qxmlGetValp((QHDL)pattr);
			nlen = 0;
			nlen = qmdlCopy(pview, NULL, NULL, 0, NULL, vbuff, sizeof(vbuff), pvalue, &nlen);
            pattr = (QXML)qxmlAttrGetNext((QHDL)pattr, 0);
		}
	}
	if(nlen <= 0)
	{
		if(purl != NULL)
		{
			nlen = (QINT)qurlScan((QPNT)purl, qurl_uiview_prop_scan_cb, 3, pview, (QPNT)vbuff, sizeof(vbuff));
		}
	}

    return QNO_OK;
}

QINT QUIViewInitHidden(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUIView *pview;
	
	pview = (QUIView *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
	if(pview->mcview.mpcontrol == NULL)
	{
		return QNO_FAIL;
	}
	pval = (QSTR)params[1];
	nlen = (QINT)params[2];
    pview->mcview.mpcontrol->Show((BOOL)pval);
	
	return QNO_OK;
}

QINT QUIViewInitActive(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUIView *pview;
	
	pview = (QUIView *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
    if(pview->mcview.mpcontrol == NULL)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
    pview->mcview.mpcontrol->Enable((BOOL)pval);
	
	return QNO_OK;
}

#endif
