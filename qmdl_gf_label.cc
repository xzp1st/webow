
#include "qmdl_config.h"

#ifdef QMDL_VER_GF

#include "stdafx.h"
#include "qmdl_gf.h"


// -- C++类实现

qstr_enum_begin(QUILabelStyleMap)
	qstr_enum_item( "default",			QUILabelStyleCenter )
	qstr_enum_item( "top",				QUILabelStyleTop )
	qstr_enum_item( "bottom",				QUILabelStyleBottom )
	qstr_enum_item( "left",				QUILabelStyleLeft )
	qstr_enum_item( "right",				QUILabelStyleRight )
	qstr_enum_item( "vcenter",			QUILabelStyleVCenter )
	qstr_enum_item( "hcenter",			QUILabelStyleHCenter )
	qstr_enum_item( "center",				QUILabelStyleCenter )

	qstr_enum_item( "SingleLine",			QUILabelStyleSingleLine )
	qstr_enum_item( "WordBreak",			QUILabelStyleWordBreak )
	qstr_enum_item( "ellipsis",			QUILabelStyleEllipsis )
	qstr_enum_item( "colorize",			QUILabelStyleColorize )
qstr_enum_end


QUILabel::QUILabel()
{
}

QUILabel::~QUILabel()
{
}

QINT QUILabel::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
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
		CComPtr<IGFStatic> pcontrol;
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

QINT QUILabelInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nstyle;
	QUILabel *pview;
	CComPtr<IGFStatic> pcontrol;

	pview = (QUILabel *)hdl;
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
	nstyle = qstr2enum(QUILabelStyleMap, 1, (QPNT )pval, &ncnt);
	if(ncnt <= 0)
	{
		return QNO_FAIL;
	}
	if(nstyle & QUILabelStyleEllipsis)
	{
		pcontrol->put_endEllipsis(TRUE);
		nstyle &= ~QUILabelStyleEllipsis;
	}
	else
	{
		pcontrol->put_endEllipsis(FALSE);
	}
	if(nstyle & QUILabelStyleColorize)
	{
		pcontrol->put_colorize(TRUE);
		nstyle &= ~QUILabelStyleColorize;
	}
	else
	{
		pcontrol->put_colorize(FALSE);
	}
	pcontrol->put_format(nstyle);
	
	return QNO_OK;
}

QINT QUILabelInitText(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUILabel *pview;
	CComPtr<IGFStatic> pcontrol;

	pview = (QUILabel *)hdl;
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

QINT QUILabelInitColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QCLR ncolor;
	QUILabel *pview;
	CComPtr<IGFStatic> pcontrol;

	pview = (QUILabel *)hdl;
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
	pcontrol->put_color((ARGB32)stargb);

	return QSCN_OK;
}

QINT QUILabelInitFontName(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	CComPtr<IGFFont> pfont;
	QUILabel *pview;
	CComPtr<IGFStatic> pcontrol;

	pview = (QUILabel *)hdl;
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

QINT QUILabelInitFontWeight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nval;
	CComPtr<IGFFont> pfont;
	QUILabel *pview;
	CComPtr<IGFStatic> pcontrol;

	pview = (QUILabel *)hdl;
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

QINT QUILabelInitFontWidth(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nval;
	CComPtr<IGFFont> pfont;
	QUILabel *pview;
	CComPtr<IGFStatic> pcontrol;

	pview = (QUILabel *)hdl;
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

QINT QUILabelInitFontHeight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nval;
	CComPtr<IGFFont> pfont;
	QUILabel *pview;
	CComPtr<IGFStatic> pcontrol;

	pview = (QUILabel *)hdl;
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

QINT QUILabelInitMaxRow(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nval;
	QUILabel *pview;
	CComPtr<IGFStatic> pcontrol;

	pview = (QUILabel *)hdl;
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
	nval = qstrint(0, pval, &ncnt);
	pcontrol->put_maxRow(nval);

	return QSCN_OK;
}

QINT QUILabelInitLineSpace(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nval;
	QUILabel *pview;
	CComPtr<IGFStatic> pcontrol;

	pview = (QUILabel *)hdl;
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
	nval = qstrint(0, pval, &ncnt);
	pcontrol->put_lineSpace(nval);

	return QSCN_OK;
}

QINT QUILabelInitIndent(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nval;
	QUILabel *pview;
	CComPtr<IGFStatic> pcontrol;

	pview = (QUILabel *)hdl;
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
	nval = qstrint(0, pval, &ncnt);
	pcontrol->put_indent(nval);

	return QSCN_OK;
}

#endif // QMDL_VER_GF
