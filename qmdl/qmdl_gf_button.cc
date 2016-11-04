
#include "qmdl_config.h"

#ifdef QMDL_VER_GF

#include "stdafx.h"
#include "qmdl_gf.h"


// -- C++类实现

qstr_enum_begin(QUIButtonStyle)
	qstr_enum_item( "Normal",			GBStyle_Normal )
	qstr_enum_item( "PushLike",			GBStyle_PushLike )
	qstr_enum_item( "DoubleFace",		GBStyle_DoubleFace )
	qstr_enum_item( "Tab",				GBStyle_Tab )
qstr_enum_end

QUIButton::QUIButton()
{
}

QUIButton::~QUIButton()
{
}

QINT QUIButton::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
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
		CComPtr<IGFButton> pcontrol;
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

QINT QUIButtonInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nstyle;
	QUIButton *pview;
	CComPtr<IGFButton> pcontrol;
	
	pview = (QUIButton *)hdl;
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
	nstyle = qstr2enum(QUIButtonStyle, 1, (QPNT )pval, &ncnt);
	if(ncnt <= 0)
	{
		nstyle = GBStyle_Normal;
	}
	pcontrol->put_style((EGFButtonStyle)nstyle);
	
	return QSCN_OK;
}

QINT QUIButtonInitFontName(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	CComPtr<IGFFont> pfont;
	QUIButton *pview;
	CComPtr<IGFButton> pcontrol;

	pview = (QUIButton *)hdl;
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
	if(pval == NULL || nlen <= 0)
	{
		return QNO_FAIL;
	}
	pfont = NULL;
	pcontrol->get_font(&pfont);
	if(pfont == NULL)
	{
		Util::GF::CreateObject(&pfont);
		if(pfont == NULL)
		{
			return QNO_FAIL;
		}
		pcontrol->put_font(pfont);
	}
	CString strname = Util::Convert::Utf8ToWS((const char *)pval, nlen);
	pfont->put_facename(strname.GetBSTR());
	
	return QNO_OK;
}

QINT QUIButtonInitFontWeight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nval;
	CComPtr<IGFFont> pfont;
	QUIButton *pview;
	CComPtr<IGFButton> pcontrol;

	pview = (QUIButton *)hdl;
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
	if(pval == NULL || nlen <= 0)
	{
		return QNO_FAIL;
	}
	ncnt = nlen;
	nval = qstrint(0, pval, &ncnt);
	if(ncnt != nlen)
	{
		return QNO_FAIL;
	}
	pfont = NULL;
	pcontrol->get_font(&pfont);
	if(pfont == NULL)
	{
		Util::GF::CreateObject(&pfont);
		if(pfont == NULL)
		{
			return QNO_FAIL;
		}
		pcontrol->put_font(pfont);
	}
	pfont->put_weight(nval);

	return QNO_OK;
}

QINT QUIButtonInitFontWidth(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nval;
	CComPtr<IGFFont> pfont;
	QUIButton *pview;
	CComPtr<IGFButton> pcontrol;

	pview = (QUIButton *)hdl;
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
	if(pval == NULL || nlen <= 0)
	{
		return QNO_FAIL;
	}
	ncnt = nlen;
	nval = qstrint(0, pval, &ncnt);
	if(ncnt != nlen)
	{
		return QNO_FAIL;
	}
	pfont = NULL;
	pcontrol->get_font(&pfont);
	if(pfont == NULL)
	{
		Util::GF::CreateObject(&pfont);
		if(pfont == NULL)
		{
			return QNO_FAIL;
		}
		pcontrol->put_font(pfont);
	}
	pfont->put_width(nval);

	return QNO_OK;
}

QINT QUIButtonInitFontHeight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nval;
	CComPtr<IGFFont> pfont;
	QUIButton *pview;
	CComPtr<IGFButton> pcontrol;

	pview = (QUIButton *)hdl;
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
	if(pval == NULL || nlen <= 0)
	{
		return QNO_FAIL;
	}
	ncnt = nlen;
	nval = qstrint(0, pval, &ncnt);
	if(ncnt != nlen)
	{
		return QNO_FAIL;
	}
	pfont = NULL;
	pcontrol->get_font(&pfont);
	if(pfont == NULL)
	{
		Util::GF::CreateObject(&pfont);
		if(pfont == NULL)
		{
			return QNO_FAIL;
		}
		pcontrol->put_font(pfont);
	}
	pfont->put_height(nval);

	return QNO_OK;
}

QINT QUIButtonInitNormalTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUIButton *pview;
	CComPtr<IGFButton> pcontrol;

	pview = (QUIButton *)hdl;
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
	CString strtitle = Util::Convert::Utf8ToWS((const char *)pval, nlen);
	pcontrol->put_text(strtitle.GetBSTR());
	
	return QSCN_OK;
}

QINT QUIButtonInitNormalColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QCLR ncolor;
	QUIButton *pview;
	CComPtr<IGFButton> pcontrol;

	pview = (QUIButton *)hdl;
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
	ncolor = quiStr2Color(pval, nlen);
	ARGB32 stargb;
	stargb.alpha = qclrGetAColor(ncolor);
	stargb.red = qclrGetRColor(ncolor);
	stargb.green = qclrGetGColor(ncolor);
	stargb.blue = qclrGetBColor(ncolor);
	pcontrol->put_normalColor((ARGB32)stargb);

	return QSCN_OK;
}

QINT QUIButtonInitNormalImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUIButton *pview;
	CComPtr<IGFButton> pcontrol;

	pview = (QUIButton *)hdl;
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
	CString strpath = quiStr2Path(pval, nlen);
    CComPtr<IGFTexture> ptexture;
    Util::GF::CreateObject(&ptexture);
    ptexture->put_file(strpath.GetBSTR());
	pcontrol->put_logoDraw(ptexture);

	return QSCN_OK;
}

QINT QUIButtonInitNormalBackground(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUIButton *pview;
	CComPtr<IGFButton> pcontrol;

	pview = (QUIButton *)hdl;
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
	CString strpath = quiStr2Path(pval, nlen);
    CComPtr<IGFTexture> ptexture;
    Util::GF::CreateObject(&ptexture);
    ptexture->put_file(strpath.GetBSTR());
	pcontrol->put_normalBackground(ptexture);

	return QSCN_OK;
}

QINT QUIButtonInitHighlightColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QCLR ncolor;
	QUIButton *pview;
	CComPtr<IGFButton> pcontrol;

	pview = (QUIButton *)hdl;
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
	ncolor = quiStr2Color(pval, nlen);
	ARGB32 stargb;
	stargb.alpha = qclrGetAColor(ncolor);
	stargb.red = qclrGetRColor(ncolor);
	stargb.green = qclrGetGColor(ncolor);
	stargb.blue = qclrGetBColor(ncolor);
	pcontrol->put_highlightColor((ARGB32)stargb);

	return QSCN_OK;
}

QINT QUIButtonInitHighlightImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUIButton *pview;
	CComPtr<IGFButton> pcontrol;

	pview = (QUIButton *)hdl;
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
	CString strpath = quiStr2Path(pval, nlen);
    CComPtr<IGFTexture> ptexture;
    Util::GF::CreateObject(&ptexture);
    ptexture->put_file(strpath.GetBSTR());
	pcontrol->put_highlightlogoDraw(ptexture);

	return QSCN_OK;
}

QINT QUIButtonInitHighlightBackground(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUIButton *pview;
	CComPtr<IGFButton> pcontrol;

	pview = (QUIButton *)hdl;
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
	CString strpath = quiStr2Path(pval, nlen);
    CComPtr<IGFTexture> ptexture;
    Util::GF::CreateObject(&ptexture);
    ptexture->put_file(strpath.GetBSTR());
	pcontrol->put_highlightBackground(ptexture);

	return QSCN_OK;
}

QINT QUIButtonInitPushedColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QCLR ncolor;
	QUIButton *pview;
	CComPtr<IGFButton> pcontrol;

	pview = (QUIButton *)hdl;
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
	ncolor = quiStr2Color(pval, nlen);
	ARGB32 stargb;
	stargb.alpha = qclrGetAColor(ncolor);
	stargb.red = qclrGetRColor(ncolor);
	stargb.green = qclrGetGColor(ncolor);
	stargb.blue = qclrGetBColor(ncolor);
	pcontrol->put_pushedColor((ARGB32)stargb);

	return QSCN_OK;
}

QINT QUIButtonInitPushedImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUIButton *pview;
	CComPtr<IGFButton> pcontrol;

	pview = (QUIButton *)hdl;
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
	CString strpath = quiStr2Path(pval, nlen);
    CComPtr<IGFTexture> ptexture;
    Util::GF::CreateObject(&ptexture);
    ptexture->put_file(strpath.GetBSTR());
	pcontrol->put_pushedlogoDraw(ptexture);

	return QSCN_OK;
}

QINT QUIButtonInitPushedBackground(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUIButton *pview;
	CComPtr<IGFButton> pcontrol;

	pview = (QUIButton *)hdl;
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
	CString strpath = quiStr2Path(pval, nlen);
    CComPtr<IGFTexture> ptexture;
    Util::GF::CreateObject(&ptexture);
    ptexture->put_file(strpath.GetBSTR());
	pcontrol->put_pushedBackground(ptexture);

	return QSCN_OK;
}

#endif
