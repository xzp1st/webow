
#include "qmdl_base.h"


QBaseView::QBaseView()
{
	memset(&mstlayout, 0, sizeof(struct QBaseLayout));
}

QBaseView::~QBaseView()
{
	if(mstlayout.hposition != NULL)
	{
		qlinkFreeLinkx(QLINK_NONE, mstlayout.hposition, NULL, NULL, 0);
		mstlayout.hposition = NULL;
	}
	if(mstlayout.hchildren != NULL)
	{
		qlinkFreeLinkx(QLINK_NONE, mstlayout.hchildren, NULL, NULL, 0);
		mstlayout.hchildren = NULL;
	}
	if(mstlayout.hrelation != NULL)
	{
		qlinkFreeLinkx(QLINK_BL_RELATION, mstlayout.hrelation, NULL, NULL, 0);
		mstlayout.hrelation = NULL;
	}
}

static QINT qmdlUIViewLayoutPosition(QMDL module, QINT layout, QINT attr, QSTR str, QINT len)
{
	// <UIView>.[width|height|left|top|right|bottom] [*|/] [int][%] [+|-] [int][%]
	QSTR pword;
	QMDL pmodule;
	QFLT fmultiplier;
	QINT nresult, npos, nlen, nflag, ndstattr, nhasattr, nminus, nremainder;
	QBaseView *psrcview, *pdstview, *pparentview;
	
	nresult = QNO_FAIL;
	npos = 0;
	pdstview = NULL;
	ndstattr = attr;
	nhasattr = 0;
	fmultiplier = 1;
	nremainder = 0;
	// 获取第一个词语。
	nflag = QSTR_UNI|QSTR_URL;
	pword = NULL;
	nlen = 0;
	npos += qstrmatch(str+npos, len-npos, &nflag, &pword, &nlen);
	if(nlen <= 0)
	{
		goto LAYOUT_EXIT;
	}
	psrcview = dynamic_cast<QBaseView *>(module);
	if(psrcview == NULL)
	{
		goto LAYOUT_EXIT;
	}
	pparentview = NULL;
	pmodule = psrcview->ParentModule();
	while(pmodule)
	{
		pparentview = dynamic_cast<QBaseView *>(pmodule);
		if(pparentview != NULL)
		{
			break;
		}
		pmodule = pmodule->ParentModule();
	}
	if(nflag & QSTR_NUM)
	{
		// 数值。
		nremainder = qstrint(0, pword, &nlen);
		// 获取下一个词语。
		nflag = QSTR_UNI;
		pword = NULL;
		nlen = 0;
		npos += qstrmatch(str+npos, len-npos, &nflag, &pword, &nlen);
	}
	else if(nflag & QSTR_WORD)
	{
		// 模块名称。
		pmodule = psrcview->FindModule(NULL, 0, (QSTR)pword, nlen);
		pdstview = dynamic_cast<QBaseView *>(pmodule);
		// 获取下一个词语。
		nflag = QSTR_UNI;
		pword = NULL;
		nlen = 0;
		npos += qstrmatch(str+npos, len-npos, &nflag, &pword, &nlen);
	}
	else if(nflag & QSTR_URL)
	{
		// 模块路径。
		pmodule = psrcview->FindModule((QSTR)pword, nlen, NULL, 0);
		pdstview = dynamic_cast<QBaseView *>(pmodule);
		// 获取下一个词语。
		nflag = QSTR_UNI;
		pword = NULL;
		nlen = 0;
		npos += qstrmatch(str+npos, len-npos, &nflag, &pword, &nlen);
	}
	if(pword == NULL || nlen != 1)
	{
		goto LAYOUT_FINISH;
	}
	if(*pword == '.')
	{
		nflag = QSTR_UNI;
		pword = NULL;
		nlen = 0;
		npos += qstrmatch(str+npos, len-npos, &nflag, &pword, &nlen);
		if(pword == NULL || nlen <= 0)
		{
			goto LAYOUT_FINISH;
		}
		nhasattr = 1;
		if(qstrcmp(QSTR_ICS, (QPNT)pword, (QPNT)"width", nlen))
		{
			ndstattr = QLayoutWidth;
		}
		else if(qstrcmp(QSTR_ICS, (QPNT)pword, (QPNT)"height", nlen))
		{
			ndstattr = QLayoutHeight;
		}
		else if(qstrcmp(QSTR_ICS, (QPNT)pword, (QPNT)"left", nlen))
		{
			ndstattr = QLayoutLeft;
		}
		else if(qstrcmp(QSTR_ICS, (QPNT)pword, (QPNT)"top", nlen))
		{
			ndstattr = QLayoutTop;
		}
		else if(qstrcmp(QSTR_ICS, (QPNT)pword, (QPNT)"right", nlen))
		{
			ndstattr = QLayoutRight;
		}
		else if(qstrcmp(QSTR_ICS, (QPNT)pword, (QPNT)"bottom", nlen))
		{
			ndstattr = QLayoutBottom;
		}
		else if(qstrcmp(QSTR_ICS, (QPNT)pword, (QPNT)"vcenter", nlen))
		{
			ndstattr = QLayoutVCenter;
		}
		else if(qstrcmp(QSTR_ICS, (QPNT)pword, (QPNT)"hcenter", nlen))
		{
			ndstattr = QLayoutHCenter;
		}
		else
		{
			nhasattr = 0;
		}
		if(pdstview == NULL)
		{
			// name=".attr*X%+Y"
			pdstview = pparentview;
		}
		nflag = QSTR_UNI;
		pword = NULL;
		nlen = 0;
		npos += qstrmatch(str+npos, len-npos, &nflag, &pword, &nlen);
		if(pword == NULL || nlen != 1)
		{
			goto LAYOUT_FINISH;
		}
	}
	if(*pword == '%')
	{
		ndstattr |= QLayoutMPercent;
		fmultiplier = nremainder;
		nremainder = 0;
		if(nhasattr == 0)
		{
			switch((ndstattr&QLayoutAttrMask))
			{
				case QLayoutLeft:
				case QLayoutRight:
					ndstattr &= ~QLayoutAttrMask;
					ndstattr |= QLayoutWidth;
					break;
					
				case QLayoutTop:
				case QLayoutBottom:
					ndstattr &= ~QLayoutAttrMask;
					ndstattr |= QLayoutHeight;
					break;
			}
		}
		nflag = QSTR_UNI;
		pword = NULL;
		nlen = 0;
		npos += qstrmatch(str+npos, len-npos, &nflag, &pword, &nlen);
		if(pword == NULL || nlen != 1)
		{
			goto LAYOUT_FINISH;
		}
	}
	else if(*pword == '*' || *pword == '/')
	{
		nflag = QSTR_NUM;
		pword = NULL;
		nlen = 0;
		npos += qstrmatch(str+npos, len-npos, &nflag, &pword, &nlen);
		if(pword == NULL || nlen <= 0)
		{
			goto LAYOUT_FINISH;
		}
		fmultiplier = qstrint(0, pword, &nlen);
		if(*pword == '/')
		{
			fmultiplier /= 1.0;
		}
		nflag = QSTR_UNI;
		pword = NULL;
		nlen = 0;
		npos += qstrmatch(str+npos, len-npos, &nflag, &pword, &nlen);
		if(pword == NULL || nlen != 1)
		{
			goto LAYOUT_FINISH;
		}
		if(*pword == '%')
		{
			ndstattr |= QLayoutMPercent;
			nflag = QSTR_UNI;
			pword = NULL;
			nlen = 0;
			npos += qstrmatch(str+npos, len-npos, &nflag, &pword, &nlen);
			if(pword == NULL || nlen != 1)
			{
				goto LAYOUT_FINISH;
			}
		}
	}
	nminus = 0;
	if(*pword == '+')
	{
		nminus = 1;
	}
	else if(*pword == '-')
	{
		nminus = -1;
	}
	else
	{
		goto LAYOUT_FINISH;
	}
	nflag = QSTR_NUM;
	pword = NULL;
	nlen = 0;
	npos += qstrmatch(str+npos, len-npos, &nflag, &pword, &nlen);
	if(pword == NULL || nlen <= 0)
	{
		goto LAYOUT_FINISH;
	}
	nremainder = qstrint(0, pword, &nlen)*nminus;
	nflag = QSTR_UNI;
	pword = NULL;
	nlen = 0;
	npos += qstrmatch(str+npos, len-npos, &nflag, &pword, &nlen);
	if(pword == NULL || nlen != 1)
	{
		goto LAYOUT_FINISH;
	}
	if(*pword == '%')
	{
		ndstattr |= QLayoutRPercent;
	}
	goto LAYOUT_FINISH;
	
LAYOUT_FINISH:
	if(ndstattr & (QLayoutMPercent|QLayoutRPercent))
	{
		if(pdstview == NULL)
		{
			pdstview = pparentview;
		}
	}
	if(layout == 0)
	{
		psrcview->AddPositionRule(attr, pdstview, ndstattr, fmultiplier, nremainder);
	}
	else
	{
		psrcview->AddLayoutRule(attr, pdstview, ndstattr, fmultiplier, nremainder);
	}
	goto LAYOUT_EXIT;
	
LAYOUT_EXIT:
	return nresult;
}

QINT QBaseViewInitWidth(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return qmdlUIViewLayoutPosition((QMDL)hdl, 0, (QINT)QLayoutWidth, (QSTR )params[2], (QINT )params[3]);
}

QINT QBaseViewInitHeight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return qmdlUIViewLayoutPosition((QMDL)hdl, 0, (QINT)QLayoutHeight, (QSTR )params[2], (QINT )params[3]);
}

QINT QBaseViewInitLeft(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return qmdlUIViewLayoutPosition((QMDL)hdl, 0, (QINT)QLayoutLeft, (QSTR )params[2], (QINT )params[3]);
}

QINT QBaseViewInitTop(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return qmdlUIViewLayoutPosition((QMDL)hdl, 0, (QINT)QLayoutTop, (QSTR )params[2], (QINT )params[3]);
}

QINT QBaseViewInitRight(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return qmdlUIViewLayoutPosition((QMDL)hdl, 0, (QINT)QLayoutRight, (QSTR )params[2], (QINT )params[3]);
}

QINT QBaseViewInitBottom(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return qmdlUIViewLayoutPosition((QMDL)hdl, 0, (QINT)QLayoutBottom, (QSTR )params[2], (QINT )params[3]);
}

QINT QBaseViewInitPosition(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval, pchar, pnext;
	QINT nlen, npos, ncnt, nindex;
	
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
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
			qmdlUIViewLayoutPosition((QMDL)hdl, 0, QLayoutLeft, (QSTR )pchar, (QINT )ncnt);
		}
		else if(nindex == 1)
		{
			qmdlUIViewLayoutPosition((QMDL)hdl, 0, QLayoutTop, (QSTR )pchar, (QINT )ncnt);
		}
		else if(nindex == 2)
		{
			qmdlUIViewLayoutPosition((QMDL)hdl, 0, QLayoutRight, (QSTR )pchar, (QINT )ncnt);
		}
		else if(nindex == 3)
		{
			qmdlUIViewLayoutPosition((QMDL)hdl, 0, QLayoutBottom, (QSTR )pchar, (QINT )ncnt);
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

QINT QBaseViewInitLayout(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval, pchar, pnext;
	QINT nlen, npos, ncnt, nindex;
	
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
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
			qmdlUIViewLayoutPosition((QMDL)hdl, 1, QLayoutLeft, (QSTR )pchar, (QINT )ncnt);
		}
		else if(nindex == 1)
		{
			qmdlUIViewLayoutPosition((QMDL)hdl, 1, QLayoutTop, (QSTR )pchar, (QINT )ncnt);
		}
		else if(nindex == 2)
		{
			qmdlUIViewLayoutPosition((QMDL)hdl, 1, QLayoutRight, (QSTR )pchar, (QINT )ncnt);
		}
		else if(nindex == 3)
		{
			qmdlUIViewLayoutPosition((QMDL)hdl, 1, QLayoutBottom, (QSTR )pchar, (QINT )ncnt);
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

