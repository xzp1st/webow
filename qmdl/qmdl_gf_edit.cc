
#include "qmdl_config.h"

#ifdef QMDL_VER_GF

#include "stdafx.h"
#include "qmdl_gf.h"


// -- C++类实现

qstr_enum_begin(QUIEditStyleMap)
	qstr_enum_item( "Center",			QUIEditStyleCenter )
	qstr_enum_item( "Right",			QUIEditStyleRight )
	qstr_enum_item( "MultiLine",		QUIEditStyleMultiLine )
	qstr_enum_item( "UpperCase",		QUIEditStyleUpperCase )
	qstr_enum_item( "LowerCase",		QUIEditStyleLowerCase )
	qstr_enum_item( "Password",			QUIEditStylePassword )
	qstr_enum_item( "AutoVScroll",		QUIEditStyleAutoVScroll )
	qstr_enum_item( "AutoHScroll",		QUIEditStyleAutoHScroll )
	qstr_enum_item( "NoHideSel",		QUIEditStyleNoHideSel )
	qstr_enum_item( "OEMConvert",		QUIEditStyleOEMConvert )
	qstr_enum_item( "ReadOnly",			QUIEditStyleReadOnly )
	qstr_enum_item( "WantReturn",		QUIEditStyleWantReturn )
	qstr_enum_item( "Number",			QUIEditStyleNumber )
qstr_enum_end

QUIEdit::QUIEdit()
{
}

QUIEdit::~QUIEdit()
{
}

QINT QUIEdit::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
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
		CComPtr<IGFEdit> pcontrol;
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

QINT QUIEditInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nstyle;
	QUIEdit *pview;
	CComPtr<IGFEdit> pcontrol;

	pview = (QUIEdit *)hdl;
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
	nstyle = qstr2enum(QUIEditStyleMap, 1, (QPNT )pval, &ncnt);
	if(ncnt <= 0)
	{
		nstyle = GBStyle_Normal;
	}
	pcontrol->put_style((DWORD)nstyle);

	return QSCN_OK;
}

#endif // QMDL_VER_GF
