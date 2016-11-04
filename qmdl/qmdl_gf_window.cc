
#include "qmdl_config.h"

#ifdef QMDL_VER_GF

#include "stdafx.h"
#include "qmdl_gf.h"


// -- C++类实现

qstr_enum_begin(QUIWindowStyleMap)
	qstr_enum_item( "gray",				QUIWindowStyleGray )
	qstr_enum_item( "colorize",			QUIWindowStyleColorize )
	qstr_enum_item( "cache",				QUIWindowStyleCache )
	qstr_enum_item( "AutoRotate",			QUIWindowStyleAutoRotate )
qstr_enum_end


QUIWindow::QUIWindow()
{
}

QUIWindow::~QUIWindow()
{
}

QINT QUIWindow::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	QCHR vname[QSTR_BUFF_SIZE];
	IGFFrame *pframe;
	
	if(mpframe != nil)
	{
		return QSCN_OK;
	}
	pframe = HasViewWithName(env, parent, mxml, url, (QSTR)vname, sizeof(vname));
	if(pframe != NULL)
	{
		mpframe = pframe;
	}
	else
	{
		CComPtr<IGFStandardWin> pcontrol;
		Util::Core::CreateObject(&pcontrol);
		if(pcontrol != NULL)
		{
			CComPtr< ITXData > pdata;
			Util::Data::CreateTXData(&pdata);
			pdata->SetDWord(CComBSTR(_T("this")), (DWORD)this);
			pcontrol->Create(NULL, pdata);
		}
		mpframe = (CComPtr<IGFFrame>)pcontrol;
	}
	
	return QSCN_OK;
}

QINT QUIWindowInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nstyle;
	QUIWindow *pview;
	CComPtr<IGFImageStatic> pcontrol;

	pview = (QUIWindow *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
	if(pview->mpframe == NULL)
	{
		return QNO_FAIL;
	}
	pcontrol = pview->mpframe;
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	ncnt = nlen;
	nstyle = qstr2enum(QUIWindowStyleMap, 1, (QPNT )pval, &ncnt);
	if(ncnt <= 0)
	{
		return QNO_FAIL;
	}
	if(nstyle & QUIWindowStyleGray)
	{
		pcontrol->put_gray(TRUE);
	}
	else
	{
		pcontrol->put_gray(FALSE);
	}
	if(nstyle & QUIWindowStyleColorize)
	{
		pcontrol->put_colorize(TRUE);
	}
	else
	{
		pcontrol->put_colorize(FALSE);
	}
	if(nstyle & QUIWindowStyleCache)
	{
		pcontrol->put_cache(TRUE);
	}
	else
	{
		pcontrol->put_cache(FALSE);
	}
	if(nstyle & QUIWindowStyleAutoRotate)
	{
		pcontrol->put_autoRotate(TRUE);
	}
	else
	{
		pcontrol->put_autoRotate(FALSE);
	}
	
	return QNO_OK;
}

QINT QUIWindowInitTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUIWindow *pview;
	CComPtr<IGFImageStatic> pcontrol;

	pview = (QUIWindow *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
	if(pview->mpframe == NULL)
	{
		return QNO_FAIL;
	}
	pcontrol = pview->mpframe;
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	CString strfile = Util::Convert::Utf8ToWS((const char *)pval, nlen);
	pcontrol->put_file(strfile.GetBSTR());
	
	return QSCN_OK;
}

#endif // QMDL_VER_GF
