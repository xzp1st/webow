
#include "qmdl_config.h"

#ifdef QMDL_VER_GF

#include "stdafx.h"
#include "qmdl_gf.h"


QUIXtml::QUIXtml()
{
}

QUIXtml::~QUIXtml()
{
}

QINT QUIXtml::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	QPNT ppath;
	QCHR vbuff[QSTR_BUFF_SIZE];
	
	if(mpframe != nil)
	{
		return QSCN_OK;
	}
	ppath = NULL;
	if(mxml != NULL)
	{
		QXML pattr;
		
		pattr = (QXML)qxmlGetAttr(mxml, QXML_ATTR, (QPNT)"path", 0);
		if(pattr != NULL)
		{
			ppath = qxmlGetValp(pattr);
		}
	}
	else if(url != NULL)
	{
		QINT nlen;
		
		nlen = qurlGetVar(url, (QPNT)"path", vbuff, sizeof(vbuff));
		if(nlen > 0)
		{
			ppath = vbuff;
		}
	}
	if(ppath == NULL)
	{
		return QNO_FAIL;
	}
	CString strpath = quiStr2Path((QSTR)ppath, 0);
	HRESULT hresult = Util::GF::CreateGFObjectByXtml(strpath.GetBSTR(), &mpframe, NULL);
	if(hresult != S_OK)
	{
		return QNO_FAIL;
	}
	
	return QSCN_OK;
}

QINT QUIXtmlInitPath(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	
	return QSCN_OK;
}

#endif // QMDL_VER_GF
