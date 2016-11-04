
#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#include "qmdl_ios.h"


UIColor *quiStr2UIColor(QSTR color)
{
	QINT nlen, ncount, ncolor, nrcolor, ngcolor, nbcolor, nacolor;
	
	if(color == NULL)
	{
		return nil;
	}
	ncount = 0;
	ncolor = qstr2enum(QUIColor, 1, (QPNT )color, &ncount);
	if(ncount <= 0)
	{
		return nil;
	}
	if(color[0] != '#')
	{
		nrcolor = qclrGetRColor(ncolor);
		ngcolor = qclrGetGColor(ncolor);
		nbcolor = qclrGetBColor(ncolor);
		nacolor = qclrGetAColor(ncolor);
	}
	else
	{
		nlen = qstrlen(color);
		if(nlen <= 6)
		{
			return nil;
		}
		ncount = 2;
		nrcolor = qstrint(16, (QPNT )&color[1], &ncount);
		ncount = 2;
		ngcolor = qstrint(16, (QPNT )&color[3], &ncount);
		ncount = 2;
		nbcolor = qstrint(16, (QPNT )&color[5], &ncount);
		if(nlen < 8)
		{
			nacolor = 255;
		}
		else
		{
			ncount = 2;
			nacolor = qstrint(16, (QPNT )&color[7], &ncount);
		}
	}
	
	return [UIColor colorWithRed:nrcolor/255.0 green:ngcolor/255.0 blue:nbcolor/255.0 alpha:nacolor/255.0];
}

struct qmdl_make_uiview_info
{
	QMDL penv;
	QMDL pparent;
	QMDL pmodule;
	id idview;
};

static QINT qmdl_make_uiview_scan_cb(QPNT url, QPNT name, QPNT value, QINT size, QPNT params[], QINT count)
{
	id idparent;
	QINT ntag, nlen;
	QMDL pparent;
	struct qmdl_make_uiview_info *pinfo;
	
	if(!qstrcmp(QSTR_CMP_ICASE, name, (QPNT )"tag", 0))
	{
		return QSCN_OK;
	}
	nlen = size;
	ntag = qstrint(0, value, &nlen);
	if(nlen <= 0 || nlen != size)
	{
		return QSCN_OK;
	}
	pinfo = (struct qmdl_make_uiview_info *)params[0];
	pparent = pinfo->pparent;
	while(pparent)
	{
		idparent = ((QUIView *)pparent)->midview;
		if(idparent != nil)
		{
			pinfo->idview = [idparent viewWithTag:ntag];
			if(pinfo->idview != nil)
			{
				return QSCN_END;
			}
		}
		pparent = pparent->ParentModule();
	}
	
	return QSCN_OK;
}

QINT quiMakeUIView(QMDL env, QMDL parent, QMDL module, QXML mxml, QSTR url)
{
	QINT nlen;
	QPNT pid, pval, vpprms[4];
	QXML pattr;
	struct qmdl_make_uiview_info stinfo;

	if(module == NULL)
	{
		return QNO_FAIL;
	}
	if(((QUIView *)module)->midview != nil)
	{
		return QNO_OK;
	}
	stinfo.penv = env;
	stinfo.pparent = parent;
	stinfo.pmodule = module;
	stinfo.idview = nil;
	vpprms[0] = (QPNT )&stinfo;
	qurlScanx((QPNT )url, qmdl_make_uiview_scan_cb, vpprms, 1);
	if(stinfo.idview == nil)
	{
		pattr = (QXML)qxmlAttrGetHead(mxml, 0);
		while(pattr)
		{
			pid = qxmlGetId(pattr);
			pval = qxmlGetValp(pattr);
			nlen = qstrlen(pval);
			qmdl_make_uiview_scan_cb(NULL, pid, pval, nlen, vpprms, 1);
			if(stinfo.idview != nil)
			{
				break;
			}
			pattr = (QXML)qxmlAttrGetNext(pattr, 0);
		}
	}
	if(stinfo.idview == nil)
	{
		return QNO_FAIL;
	}
	((QUIView *)module)->midview = stinfo.idview;
	
	return QNO_OK;
}

#endif
