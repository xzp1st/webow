
#include "qmdl_config.h"

#ifdef QMDL_VER_GF

#include "stdafx.h"
#include "qmdl_gf.h"


QUIGFView::QUIGFView(): mpview(NULL), mpparent(NULL), mpEventListener(this), mpMessageListener(this)
{
	mpMessageListener->Hook_OnMessage(&QUIGFView::OnMessage);
}

QUIGFView::~QUIGFView()
{
	mpEventListener.UnlistenAllEvent();
	mpMessageListener->UnhookAll();
}

HRESULT QUIGFView::Initialize(IGFFrame *pParent)
{
	mpparent = pParent;
	return S_OK;
}

HRESULT QUIGFView::OnMessage(IGFFrame *pTargetFrame, tagBaseArg *pArg, BOOL* pbWinMsgHandled, LRESULT *pRet, BOOL *pbProceedMsgFlow, BOOL *pbCallNextMsgListener)
{
	QINT ncount;
	QPNT vpprms[8];

	if(mpview == NULL)
	{
		return S_OK;
	}
	ncount = 0;
	vpprms[ncount++] = (QPNT)pTargetFrame;
	vpprms[ncount++] = (QPNT)pArg;
	vpprms[ncount++] = (QPNT)pbWinMsgHandled;
	vpprms[ncount++] = (QPNT)pRet;
	vpprms[ncount++] = (QPNT)pbProceedMsgFlow;
	vpprms[ncount++] = (QPNT)pbCallNextMsgListener;
	mpview->DispatchEventx(NULL, pArg->nId, vpprms, ncount);

	return S_OK;
}

void QUIGFView::OnGFEvent(int nEvent, IGFEventSupporter* pSupporter, ITXData* pData)
{
	if(pData == NULL)
	{
		return;
	}
	QUIView *pview;
	pData->GetDWord(CComBSTR(_T("this")), (DWORD *)&pview);
	if(pview == NULL)
	{
		return;
	}
	QINT ncount;
	QPNT vpprms[8];
	ncount = 0;
	vpprms[ncount++] = (QPNT)pview->mpframe;
	pview->DispatchEventx(NULL, nEvent, vpprms, ncount);
}

static QINT qui_init_event_scan_cb(QMDL observed, QMDL observer,
	QSTR name, QINT code, QPFN event_fn, QPCB event_cb, QPNT params[], QINT count)
{
	QMDL pobserved;
	QUIView *pview;

	pobserved = (QMDL )params[0];
	if(pobserved != observed)
	{
		return QSCN_OK;
	}
	pview = dynamic_cast<QUIView *>(pobserved);
	if(pview == NULL)
	{
		return QSCN_OK;
	}
	if( (code & QSystemMask) != 0 )
	{
		// 模块系统消息，不需要做GF平台的消息监听。
		return QSCN_OK;
	}
	if(code >= GFM_Mouse_Begin && code < GFM_WndUserMsg)
	{
		// WM消息。
	}
	else if(code >= GFE_None && code < GFE_User)
	{
		if(pview->mpframe != NULL)
		{
			pview->mcgfview.mpEventListener.ListenEvent(pview->mpframe, GFE_Click, &QUIGFView::OnGFEvent);
		}
	}

	return QSCN_OK;
}

QINT QUIViewOnEvent(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QMDL pview;

	pview = (QMDL)hdl;
	if(pview == NULL)
	{
		return QNO_OK;
	}
	if(code == QCD_MAKE)
	{
		pview->ScanEvent(qui_init_event_scan_cb, 1, pview);
	}

	return QNO_OK;
}

QUIView::QUIView()
{
	mpframe = NULL;
	mcgfview.mpview = this;

	this->WaitEvent(NULL, NULL, QCD_NONE, QWAIT_NOREPLACE, NULL, QUIViewOnEvent);
}

QUIView::~QUIView()
{
	mcgfview.mpview = NULL;
	if(mpframe != NULL)
	{
		mpframe = NULL;
	}
}

QINT QUIView::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	return QSCN_OK;
}

QINT QUIView::MakeModuleEnd(QMDL env, QXML mxml, QSTR url)
{
	return QSCN_OK;
}

IGFFrame *QUIView::HasViewWithName(QMDL env, QMDL parent, QXML mxml, QSTR url, QSTR name, QINT size)
{
	QPNT pname;
	QCHR vname[QSTR_BUFF_SIZE];
	QINT nindex, nlen, ncnt;
	IGFFrame *pframe;
	QMDL pparent;
	QUIView *pview;
	QXML pattr;
	
	nlen = 0;
	pname = NULL;
	if(mxml != NULL)
	{
		pattr = (QXML)qxmlGetAttr((QHDL)mxml, 0, (QHDL)"name", 0);
		if(pattr != NULL)
		{
			pname = (QPNT)qxmlGetValp((QHDL)pattr);
			nlen = qstrlen(pname);
		}
	}
	else if(url != NULL)
	{
		nlen = (QINT)qurlGetVar((QPNT)url, (QPNT)"name", (QPNT)vname, sizeof(vname));
		if(nlen > 0)
		{
			pname = vname;
		}
	}
	if(pname == NULL || nlen <= 0)
	{
		return NULL;
	}
	ncnt = nlen;
	nindex = qstrint(0, pname, &ncnt);
	if(ncnt == nlen)
	{
		pname = NULL;
	}
	
	pframe = NULL;
	pparent = parent;
	while(pparent)
	{
		pview = dynamic_cast<QUIView *>(pparent);
		if(pview->mpframe != NULL)
		{
			if(pname == NULL)
			{
				HRESULT hresult = pview->mpframe->GetElementByIndex(nindex, (IGFElement **)&pframe);
				if(hresult != S_OK)
				{
					pframe = NULL;
				}
			}
			else
			{
				CString strname = Util::Convert::Utf8ToWS((const char *)pname);
				HRESULT hresult = pview->mpframe->GetElementByName(strname.GetBSTR(), (IGFElement **)&pframe);
				if(hresult != S_OK)
				{
					pframe = NULL;
				}
			}
			if(pframe != NULL)
			{
				break;
			}
		}
		pparent = pparent->ParentModule();
	}

	if(name != NULL)
	{
		qstrcpy(name, size, pname, 0);
	}
	
	return pframe;
}

QINT QUIView::MakeModuleInit(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	QMDL pparent;
	QUIView *pview;
	IGFFrame *pframe;
	
	pframe = NULL;
	if(mpframe != NULL)
	{
		mpframe->GetParentFrame(&pframe);
		if(pframe == NULL)
		{
			pparent = parent;
			while(pparent)
			{
				pview = dynamic_cast<QUIView *>(pparent);
				if(pview != NULL)
				{
					if(pview->mpframe != NULL)
					{
						pview->mpframe->AddFrame(mpframe, NULL);
						break;
					}
				}
				pparent = pparent->ParentModule();
			}
		}
	}
	
	return QNO_TRUE;
}

QINT QUIViewInitHidden(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUIView *pthis;
	
	pthis = (QUIView *)hdl;
	if(pthis == NULL)
	{
		return QNO_FAIL;
	}
	if(pthis->mpframe == NULL)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	if(qstrcmp(QSTR_ICS, (QPNT)pval, (QPNT)"yes", nlen) || qstrcmp(QSTR_ICS, (QPNT)pval, (QPNT)"true", nlen))
	{
		pthis->mpframe->put_hidden(TRUE);
	}
	else
	{
		pthis->mpframe->put_hidden(FALSE);
	}
	
	return QNO_OK;
}

QINT QUIViewInitEnable(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUIView *pthis;
	
	pthis = (QUIView *)hdl;
	if(pthis == NULL)
	{
		return QNO_FAIL;
	}
	if(pthis->mpframe == NULL)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	if(qstrcmp(QSTR_ICS, (QPNT)pval, (QPNT)"yes", nlen) || qstrcmp(QSTR_ICS, (QPNT)pval, (QPNT)"true", nlen))
	{
		pthis->mpframe->put_enable(TRUE);
	}
	else
	{
		pthis->mpframe->put_enable(FALSE);
	}
	
	return QNO_OK;
}

QINT QUIViewInitAlpha(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nval;
	QUIView *pthis;
	
	pthis = (QUIView *)hdl;
	if(pthis == NULL)
	{
		return QNO_FAIL;
	}
	if(pthis->mpframe == NULL)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	ncnt = nlen;
	nval = qstrint(0, pval, &ncnt);
	if(ncnt == nlen)
	{
		pthis->mpframe->put_alpha((BYTE)nval);
	}
	else
	{
		pthis->mpframe->put_disableAlpha((BYTE)256);
	}
	
	return QNO_OK;
}

QINT QUIViewInitOpaque(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nval;
	QUIView *pthis;

	pthis = (QUIView *)hdl;
	if(pthis == NULL)
	{
		return QNO_FAIL;
	}
	if(pthis->mpframe == NULL)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	ncnt = nlen;
	nval = qstrint(0, pval, &ncnt);
	if(ncnt == nlen)
	{
		pthis->mpframe->put_opaque((BYTE)nval);
	}
	else if(qstrcmp(QSTR_ICS, pval, "TRUE", nlen) || qstrcmp(QSTR_ICS, pval, "YES", nlen))
	{
		pthis->mpframe->put_opaque(TRUE);
	}
	else
	{
		pthis->mpframe->put_opaque(FALSE);
	}

	return QNO_OK;
}

QINT QUIViewInitTransparent(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nval;
	QUIView *pthis;

	pthis = (QUIView *)hdl;
	if(pthis == NULL)
	{
		return QNO_FAIL;
	}
	if(pthis->mpframe == NULL)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	ncnt = nlen;
	nval = qstrint(0, pval, &ncnt);
	if(ncnt == nlen)
	{
		pthis->mpframe->put_transparent((BOOL)nval);
	}
	else if(qstrcmp(QSTR_ICS, pval, "TRUE", nlen) || qstrcmp(QSTR_ICS, pval, "YES", nlen))
	{
		pthis->mpframe->put_transparent(TRUE);
	}
	else
	{
		pthis->mpframe->put_transparent(FALSE);
	}

	return QNO_OK;
}

QINT QUIViewInitMargin(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nval;
	QUIView *pthis;

	pthis = (QUIView *)hdl;
	if(pthis == NULL)
	{
		return QNO_FAIL;
	}
	if(pthis->mpframe == NULL)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	ncnt = nlen;
	nval = qstrint(0, pval, &ncnt);
	if(ncnt == nlen)
	{
		//pthis->mpframe->put_margin((BOOL)nval);
	}

	return QNO_OK;
}

QINT QUIViewInitPadding(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nval;
	QUIView *pthis;

	pthis = (QUIView *)hdl;
	if(pthis == NULL)
	{
		return QNO_FAIL;
	}
	if(pthis->mpframe == NULL)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	ncnt = nlen;
	nval = qstrint(0, pval, &ncnt);
	if(ncnt == nlen)
	{
		//pthis->mpframe->put_padding((BOOL)nval);
	}

	return QNO_OK;
}

QINT QUIViewInitTip(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUIView *pthis;

	pthis = (QUIView *)hdl;
	if(pthis == NULL)
	{
		return QNO_FAIL;
	}
	if(pthis->mpframe == NULL)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	CString strtip = Util::Convert::Utf8ToWS((const char *)pval, nlen);
	pthis->mpframe->put_toolTip(strtip.GetBSTR());

	return QNO_OK;
}

QINT QUIViewInitCaption(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUIView *pthis;

	pthis = (QUIView *)hdl;
	if(pthis == NULL)
	{
		return QNO_FAIL;
	}
	if(pthis->mpframe == NULL)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	CString strcaption = Util::Convert::Utf8ToWS((const char *)pval, nlen);
	pthis->mpframe->put_caption(strcaption.GetBSTR());

	return QNO_OK;
}

static QINT qmdl_find_uiview_by_name_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pname;
	QUIView *psrcview;
	IGFFrame *pdstview, **ppdstview;
	QMDL pmodule, pchild, *ppfound;
	
	pmodule = (QMDL )hdl;
	if(pmodule == NULL)
	{
		return QSCN_OK;
	}
	psrcview = dynamic_cast<QUIView *>(pmodule);
	if(psrcview == NULL)
	{
		return QSCN_OK;
	}
	else if(psrcview->mpframe == NULL)
	{
		return QSCN_OK;
	}
	pname = (QSTR)params[0];
	if(pname == NULL)
	{
		return QSCN_END;
	}
	ppfound = (QMDL *)params[1];
	if(ppfound == NULL)
	{
		return QSCN_END;
	}
	ppdstview = (IGFFrame **)params[2];
	if(ppdstview == NULL)
	{
		return QSCN_END;
	}
	pchild = (QMDL)params[3];
	if(pchild == pmodule)
	{
		return QSCN_HORZ;
	}
	CString strname = Util::Convert::Utf8ToWS((const char *)pname);
	HRESULT hresult = psrcview->mpframe->GetElementByName(strname.GetBSTR(), (IGFElement **)&pdstview);
	if(hresult != S_OK)
	{
		pdstview = NULL;
	}
	if(pdstview == NULL)
	{
		return QSCN_OK;
	}
	*ppfound = pmodule;
	*ppdstview = pdstview;
	
	return QSCN_END;
}

enum NSLayoutAttribute
{
	NSLayoutAttributeLeft = 1,
	NSLayoutAttributeRight,
	NSLayoutAttributeTop,
	NSLayoutAttributeBottom,
	NSLayoutAttributeLeading,
	NSLayoutAttributeTrailing,
	NSLayoutAttributeWidth,
	NSLayoutAttributeHeight,
	NSLayoutAttributeHCenter,
	NSLayoutAttributeVCenter,
	NSLayoutAttributeCenter,
	NSLayoutAttributeAppend,
};

static QINT qmdlUIViewInitPosition(QMDL module, QINT flag, QSTR str, QINT len)
{
	// <UIView>.[width|height|left|top|right|bottom] [*|/] [times] [+|-] [remainder]
	QMDL pmodule;
	QSTR pchar, pbuff;
	QCHR vbuff[QSTR_BUFF_SIZE];
	QINT npos, nlen, ncnt, nval;
	IGFFrame *psrcview, *pdstview, *pparentview;
	QINT nattrflag, nmulflag, ntimes, naddflag, nremainder;
	QFLT fmultiplier;
	
	psrcview = (IGFFrame *)(((QUIView *)module)->mpframe);
	if(psrcview == NULL)
	{
		return QNO_TRUE;
	}
	HRESULT hresult = psrcview->GetParentFrame(&pparentview);
	if(hresult != S_OK)
	{
		pparentview = NULL;
	}
	pdstview = NULL;
	nattrflag = 0;
	nmulflag = 0;
	ntimes = 1;
	naddflag = 0;
	nremainder = 0;
	// 分析UIView。
	npos = 0;
	pchar = str;
	while(npos < len)
	{
		if( *pchar != ' ' &&
		    *pchar != '\t' &&
		    *pchar != '\r' &&
		    *pchar != '\n' )
		{
			break;
		}
		pchar++;
		npos++;
	}
	nlen = 0;
	pbuff = (QSTR)vbuff;
	while(npos < len)
	{
		if( *pchar == ' ' ||
		   *pchar == '\t' ||
		   *pchar == '\r' ||
		   *pchar == '\n' )
		{
			break;
		}
		if(*pchar == '.')
		{
			break;
		}
		*pbuff++ = *pchar++;
		nlen++;
		npos++;
	}
	if(npos >= len)
	{
		goto PARSE_COMPUTE;
	}
	ncnt = nlen;
	nval = qstrint(0, vbuff, &ncnt);
	if(ncnt != nlen)
	{
		nval = 0;
	}
	*pbuff = 0;
	pchar = (QSTR)qstrchr(0, vbuff, 0, '/');
	if(pchar != NULL)
	{
		pmodule = module->FindModule(NULL, 0, (QSTR)vbuff, 0);
		if(pmodule != NULL)
		{
			pdstview = (IGFFrame *)(((QUIView *)pmodule)->mpframe);
		}
	}
	else
	{
		QINT ncount;
		QPNT vpprms[5];
		QMDL pchild, pfound;
		
		pfound = NULL;
		pchild = NULL;
		pmodule = module;
		while(pmodule)
		{
			ncount = 0;
			vpprms[ncount++] = (QPNT)vbuff;
			vpprms[ncount++] = &pfound;
			vpprms[ncount++] = &pdstview;
			vpprms[ncount++] = pchild;
			pmodule->ScanModulex(pmodule, qmdl_find_uiview_by_name_cb, NULL, vpprms, ncount);
			if(pfound != NULL)
			{
				break;
			}
			pchild = pmodule;
			pmodule = pmodule->ParentModule();
		}
	}
	if(pdstview == NULL)
	{
		nremainder = nval;
		goto PARSE_COMPUTE;
	}
	while(npos < len)
	{
		if( *pchar != ' ' &&
		   *pchar != '\t' &&
		   *pchar != '\r' &&
		   *pchar != '\n' )
		{
			break;
		}
		pchar++;
		npos++;
	}
	if(npos >= len)
	{
		goto PARSE_COMPUTE;
	}
	if(*pchar == '.')
	{
		nlen = 0;
		if(qstrcmp(QSTR_ICS, (QPNT)(pchar+1), (QPNT)"width", 5))
		{
			nattrflag = NSLayoutAttributeWidth;
			nlen = 5;
		}
		else if(qstrcmp(QSTR_ICS, (QPNT)(pchar+1), (QPNT)"height", 6))
		{
			nattrflag = NSLayoutAttributeHeight;
			nlen = 6;
		}
		else if(qstrcmp(QSTR_ICS, (QPNT)(pchar+1), (QPNT)"left", 4))
		{
			nattrflag = NSLayoutAttributeLeft;
			nlen = 4;
		}
		else if(qstrcmp(QSTR_ICS, (QPNT)(pchar+1), (QPNT)"top", 3))
		{
			nattrflag = NSLayoutAttributeTop;
			nlen = 3;
		}
		else if(qstrcmp(QSTR_ICS, (QPNT)(pchar+1), (QPNT)"right", 5))
		{
			nattrflag = NSLayoutAttributeRight;
			nlen = 5;
		}
		else if(qstrcmp(QSTR_ICS, (QPNT)(pchar+1), (QPNT)"bottom", 6))
		{
			nattrflag = NSLayoutAttributeBottom;
			nlen = 6;
		}
		else if(qstrcmp(QSTR_ICS, (QPNT)(pchar+1), (QPNT)"vcenter", 7))
		{
			nattrflag = NSLayoutAttributeVCenter;
			nlen = 7;
		}
		else if(qstrcmp(QSTR_ICS, (QPNT)(pchar+1), (QPNT)"hcenter", 7))
		{
			nattrflag = NSLayoutAttributeHCenter;
			nlen = 7;
		}
		if(nlen <= 0)
		{
			return QNO_TRUE;
		}
		pchar += 1+nlen;
		npos += 1+nlen;
		while(npos < len)
		{
			if( *pchar != ' ' &&
			   *pchar != '\t' &&
			   *pchar != '\r' &&
			   *pchar != '\n' )
			{
				break;
			}
			pchar++;
			npos++;
		}
		if(npos >= len)
		{
			goto PARSE_COMPUTE;
		}
	}
	nlen = 0;
	if(*pchar == '*')
	{
		nmulflag = 1;
		nlen = 1;
	}
	else if(*pchar == '/')
	{
		nmulflag = 2;
		nlen = 1;
	}
	if(nlen > 0)
	{
		nlen = len-npos;
		nval = qstrint(0, (QPNT)(pchar+1), &nlen);
		if(nlen <= 0)
		{
			ntimes = 1;
			nlen = 0;
		}
		else
		{
			ntimes = nval;
		}
		pchar += 1+nlen;
		npos += 1+nlen;
		while(npos < len)
		{
			if( *pchar != ' ' &&
			   *pchar != '\t' &&
			   *pchar != '\r' &&
			   *pchar != '\n' )
			{
				break;
			}
			pchar++;
			npos++;
		}
		if(npos >= len)
		{
			goto PARSE_COMPUTE;
		}
	}
	nlen = 0;
	if(*pchar == '+')
	{
		naddflag = 1;
		nlen = 1;
	}
	else if(*pchar == '/')
	{
		naddflag = 2;
		nlen = 1;
	}
	if(nlen > 0)
	{
		nlen = len-npos;
		nval = qstrint(0, (QPNT)(pchar+1), &nlen);
		if(nlen <= 0)
		{
			ntimes = 1;
			nlen = 0;
		}
		else
		{
			ntimes = nval;
		}
		pchar += 1+nlen;
		npos += 1+nlen;
		while(npos < len)
		{
			if( *pchar != ' ' &&
			   *pchar != '\t' &&
			   *pchar != '\r' &&
			   *pchar != '\n' )
			{
				break;
			}
			pchar++;
			npos++;
		}
		if(npos >= len)
		{
			goto PARSE_COMPUTE;
		}
	}
	if(nlen > 0)
	{
		nlen = len-npos;
		nval = qstrint(0, (QPNT)(pchar+1), &nlen);
		if(nlen <= 0)
		{
			nremainder = 0;
			nlen = 0;
		}
		else
		{
			nremainder = nval;
		}
	}
	goto PARSE_COMPUTE;
	
PARSE_COMPUTE:
	if(nattrflag == 0)
	{
		nattrflag = flag;
	}
	fmultiplier = (QFLT)ntimes;
	if(nmulflag == 0)
	{
		fmultiplier = 1;
	}
	else if(nmulflag == 2)
	{
		fmultiplier = 1/fmultiplier;
	}
	if(naddflag == 2)
	{
		nremainder *= (-1);
	}
	SIZE stsrcsize, stdstsize;
	POINT stsrcpoint, stdstpoint;
	psrcview->get_size(&stsrcsize);
	psrcview->get_location(&stsrcpoint);
	if(nattrflag == NSLayoutAttributeWidth)
	{
		if(pdstview != NULL)
		{
			pdstview->get_size(&stdstsize);
			stsrcsize.cx = (QINT)(stdstsize.cx*fmultiplier) + nremainder;
		}
		else
		{
			stsrcsize.cx = nremainder;
		}
	}
	else if(nattrflag == NSLayoutAttributeHeight)
	{
		if(pdstview != NULL)
		{
			pdstview->get_size(&stdstsize);
			stsrcsize.cy = (QINT)(stdstsize.cy*fmultiplier) + nremainder;
		}
		else
		{
			stsrcsize.cy = nremainder;
		}
	}
	else if(nattrflag == NSLayoutAttributeLeft)
	{
		if(pdstview != NULL)
		{
			pdstview->get_location(&stdstpoint);
			stsrcpoint.x = (QINT)(stdstpoint.x*fmultiplier) + nremainder;
		}
		else
		{
			stsrcpoint.x = nremainder;
		}
	}
	else if(nattrflag == NSLayoutAttributeTop)
	{
		if(pdstview != NULL)
		{
			pdstview->get_location(&stdstpoint);
			stsrcpoint.y = (QINT)(stdstpoint.y*fmultiplier) + nremainder;
		}
		else
		{
			stsrcpoint.y = nremainder;
		}
	}
	else if(nattrflag == NSLayoutAttributeRight)
	{
		if(pdstview != NULL)
		{
			pdstview->get_size(&stdstsize);
			pdstview->get_location(&stdstpoint);
			if(stsrcsize.cx <= 0)
			{
				stsrcsize.cx = (QINT)((stdstpoint.x+stdstsize.cx)*fmultiplier) + nremainder - stsrcpoint.x;
			}
			else
			{
				stsrcpoint.x = (QINT)((stdstpoint.x+stdstsize.cx)*fmultiplier) + nremainder - stsrcsize.cx;
			}
		}
		else
		{
			if(stsrcsize.cx <= 0)
			{
				stsrcsize.cx = nremainder - stsrcpoint.x;
			}
			else
			{
				stsrcpoint.x = nremainder - stsrcsize.cx;
			}
		}
	}
	else if(nattrflag == NSLayoutAttributeBottom)
	{
		if(pdstview != NULL)
		{
			pdstview->get_size(&stdstsize);
			pdstview->get_location(&stdstpoint);
			if(stsrcsize.cy <= 0)
			{
				stsrcsize.cy = (QINT)((stdstpoint.y+stdstsize.cy)*fmultiplier) + nremainder - stsrcpoint.y;
			}
			else
			{
				stsrcpoint.y = (QINT)((stdstpoint.y+stdstsize.cy)*fmultiplier) + nremainder - stsrcsize.cy;
			}
		}
		else
		{
			if(stsrcsize.cy <= 0)
			{
				stsrcsize.cy = nremainder - stsrcpoint.y;
			}
			else
			{
				stsrcpoint.y = nremainder - stsrcsize.cy;
			}
		}
	}
	else if(nattrflag == NSLayoutAttributeHCenter)
	{
		if(pdstview != NULL)
		{
			pdstview->get_size(&stdstsize);
			pdstview->get_location(&stdstpoint);
			stsrcpoint.x = (QINT)((stdstpoint.x+stdstsize.cx)/2*fmultiplier) + nremainder;
		}
		else
		{
			stsrcpoint.x = nremainder;
		}
	}
	else if(nattrflag == NSLayoutAttributeVCenter)
	{
		if(pdstview != NULL)
		{
			pdstview->get_size(&stdstsize);
			pdstview->get_location(&stdstpoint);
			stsrcpoint.y = (QINT)((stdstpoint.y+stdstsize.cy)/2*fmultiplier) + nremainder;
		}
		else
		{
			stsrcpoint.y = nremainder;
		}
	}
	else if(nattrflag == NSLayoutAttributeCenter)
	{
		if(pdstview != NULL)
		{
			pdstview->get_size(&stdstsize);
			pdstview->get_location(&stdstpoint);
			stsrcpoint.x = (QINT)((stdstpoint.x+stdstsize.cx)/2*fmultiplier) + nremainder;
			stsrcpoint.y = (QINT)((stdstpoint.y+stdstsize.cy)/2*fmultiplier) + nremainder;
		}
		else
		{
			stsrcpoint.x = nremainder;
			stsrcpoint.y = nremainder;
		}
	}
	if(flag == NSLayoutAttributeWidth || flag == NSLayoutAttributeHeight)
	{
		psrcview->put_size(stsrcsize);
	}
	else if(flag == NSLayoutAttributeLeft || flag == NSLayoutAttributeTop)
	{
		psrcview->put_location(stsrcpoint);
	}
	else if(flag == NSLayoutAttributeRight || flag == NSLayoutAttributeBottom)
	{
		psrcview->put_size(stsrcsize);
		psrcview->put_location(stsrcpoint);
	}
	else if(flag == NSLayoutAttributeHCenter)
	{
		if(stsrcsize.cx > 0)
		{
			stsrcpoint.x -= stsrcsize.cx/2;
			psrcview->put_location(stsrcpoint);
		}
	}
	else if(flag == NSLayoutAttributeVCenter)
	{
		if(stsrcsize.cy > 0)
		{
			stsrcpoint.y -= stsrcsize.cy/2;
			psrcview->put_location(stsrcpoint);
		}
	}
	else if(flag == NSLayoutAttributeCenter)
	{
		if(stsrcsize.cx > 0 || stsrcsize.cy > 0)
		{
			stsrcpoint.x -= stsrcsize.cx/2;
			stsrcpoint.y -= stsrcsize.cy/2;
			psrcview->put_location(stsrcpoint);
		}
	}
	
	return QNO_TRUE;
}

QINT QUIViewInitWidth(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return qmdlUIViewInitPosition((QMDL)hdl, (QINT)NSLayoutAttributeWidth, (QSTR )params[1], (QINT )params[2]);
}

QINT QUIViewInitHeight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return qmdlUIViewInitPosition((QMDL)hdl, (QINT)NSLayoutAttributeHeight, (QSTR )params[1], (QINT )params[2]);
}

QINT QUIViewInitLeft(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return qmdlUIViewInitPosition((QMDL)hdl, (QINT)NSLayoutAttributeLeft, (QSTR )params[1], (QINT )params[2]);
}

QINT QUIViewInitTop(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return qmdlUIViewInitPosition((QMDL)hdl, (QINT)NSLayoutAttributeTop, (QSTR )params[1], (QINT )params[2]);
}

QINT QUIViewInitRight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return qmdlUIViewInitPosition((QMDL)hdl, (QINT)NSLayoutAttributeRight, (QSTR )params[1], (QINT )params[2]);
}

QINT QUIViewInitBottom(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return qmdlUIViewInitPosition((QMDL)hdl, (QINT)NSLayoutAttributeBottom, (QSTR )params[1], (QINT )params[2]);
}

QINT QUIViewInitVCenter(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return qmdlUIViewInitPosition((QMDL)hdl, (QINT)NSLayoutAttributeVCenter, (QSTR )params[1], (QINT )params[2]);
}

QINT QUIViewInitHCenter(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return qmdlUIViewInitPosition((QMDL)hdl, (QINT)NSLayoutAttributeHCenter, (QSTR )params[1], (QINT )params[2]);
}

QINT QUIViewInitCenter(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	qmdlUIViewInitPosition((QMDL)hdl, (QINT)NSLayoutAttributeHCenter, (QSTR )params[1], (QINT )params[2]);
	qmdlUIViewInitPosition((QMDL)hdl, (QINT)NSLayoutAttributeVCenter, (QSTR )params[1], (QINT )params[2]);
	
	return QSCN_OK;
}

QINT QUIViewInitPosition(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval, pchar, pnext;
	QINT nlen, npos, ncnt, nindex;
	
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	npos = 0;
	nindex = 0;
	pchar = pval;
	while(pchar)
	{
		pnext = (QSTR)qstrchr(0, (QPNT)pchar, nlen-npos, (QINT)',');
		if(pnext == NULL)
		{
			ncnt = nlen;
		}
		else
		{
			ncnt = (QINT)(pnext-pchar);
		}
		if(nindex == 0)
		{
			qmdlUIViewInitPosition((QMDL)hdl, (QINT)NSLayoutAttributeLeft, (QSTR )pchar, (QINT )ncnt);
		}
		else if(nindex == 1)
		{
			qmdlUIViewInitPosition((QMDL)hdl, (QINT)NSLayoutAttributeTop, (QSTR )pchar, (QINT )ncnt);
		}
		else if(nindex == 2)
		{
			qmdlUIViewInitPosition((QMDL)hdl, (QINT)NSLayoutAttributeRight, (QSTR )pchar, (QINT )ncnt);
		}
		else if(nindex == 3)
		{
			qmdlUIViewInitPosition((QMDL)hdl, (QINT)NSLayoutAttributeBottom, (QSTR )pchar, (QINT )ncnt);
		}
		else
		{
			break;
		}
		if(pnext == NULL)
		{
			break;
		}
		pchar += ncnt+1;
		npos += ncnt+1;
		nindex++;
	}
	
	return QSCN_OK;
}

#endif // QMDL_VER_GF
