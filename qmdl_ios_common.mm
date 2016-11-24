
#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#include "qmdl_ios.h"


static QINT QFontEnum_cb(QENUM vals, QINT flag, QPNT str, QINT *count, QINT value);

qstr_enum_begin(QFontEnum)
	qstr_enum_cb( QFontEnum_cb )
qstr_enum_end

static QINT QFontEnum_cb(QENUM vals, QINT flag, QPNT str, QINT *count, QINT value)
{
	// 字体格式：name-size
	QSTR pname, psize;
	QINT nlen, ncnt, nfont;
	QCHR vfont[QSTR_BUFF_SIZE];
	UIFont *pfont;
	
	if(str == NULL)
	{
		return 0;
	}
	ncnt = nlen;
	nfont = qstrint(0, str, &ncnt);
	if(ncnt == nlen)
	{
		pfont = [UIFont systemFontOfSize:nfont];
	}
	else
	{
		psize = (QSTR)qstrrchr(QSTR_NONE, str, 0, (QINT)'-');
		if(psize == NULL)
		{
			pname = (QSTR)str;
			nfont = 16;
		}
		else
		{
			ncnt = 0;
			nfont = qstrint(10, psize+1, &ncnt);
			if(ncnt <= 0)
			{
				pname = (QSTR)str;
				nfont = 16;
			}
			else
			{
				qstrcpy(vfont, sizeof(vfont), str, (QINT)(psize-(QSTR)str));
				pname = (QSTR)vfont;
			}
		}
		pfont = [UIFont fontWithName:[NSString stringWithUTF8String:(const char *)pname] size:nfont];
	}
	
	return (QINT)pfont;
}

UIColor *quiStr2UIColor(QSTR color)
{
	QINT ncount, ncolor, nrcolor, ngcolor, nbcolor, nacolor;
	
	if(color == NULL)
	{
		return nil;
	}
	ncount = 0;
	ncolor = qstr2enum(QCLREnum, 1, (QPNT)color, &ncount);
	if(ncount <= 0)
	{
		return nil;
	}
	nrcolor = qclrGetRColor(ncolor);
	ngcolor = qclrGetGColor(ncolor);
	nbcolor = qclrGetBColor(ncolor);
	nacolor = qclrGetAColor(ncolor);
	
	return [UIColor colorWithRed:nrcolor/255.0 green:ngcolor/255.0 blue:nbcolor/255.0 alpha:nacolor/255.0];
}

UIColor *quiRGBA2UIColor(QINT color)
{
	QINT nrcolor, ngcolor, nbcolor, nacolor;
	
	nrcolor = qclrGetRColor(color);
	ngcolor = qclrGetGColor(color);
	nbcolor = qclrGetBColor(color);
	nacolor = qclrGetAColor(color);
	
	return [UIColor colorWithRed:nrcolor/255.0 green:ngcolor/255.0 blue:nbcolor/255.0 alpha:nacolor/255.0];
}

QINT quiUIColor2RGBA(UIColor *color)
{
	QINT nrcolor, ngcolor, nbcolor, nacolor;
	
	[color getRed:(CGFloat*)&nrcolor green:(CGFloat*)&ngcolor blue:(CGFloat*)&nbcolor alpha:(CGFloat*)&nacolor];
	
	return qclrMakeRGBA(nrcolor, ngcolor, nbcolor, nacolor);
}

NSString *quiUtf82NSString(QSTR str, QINT len)
{
	QSTR pstr;
	QCHR vbuff[QSTR_BUFF_SIZE];
	
	if(len <= 0)
	{
		pstr = str;
	}
	else
	{
		qstrcpy(vbuff, sizeof(vbuff), str, len);
		pstr = (QSTR)vbuff;
	}
	
	return [NSString stringWithUTF8String:(const char *)pstr];
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
	
	if(!qstrcmp(QSTR_ICS, name, (QPNT )"tag", 0))
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
