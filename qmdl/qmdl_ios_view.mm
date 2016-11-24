
#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#include "qmdl_ios.h"


QUIView::QUIView()
{
	midview = nil;
	midcontroller = nil;
	midevent = nil;
}

QUIView::~QUIView()
{
	midview = nil;
	midcontroller = nil;
	midevent = nil;
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
			nstate = !(((UIView *)midview).hidden);
			((UIView *)midview).hidden = !flag;
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
			nstate = !(((UIView *)midview).hidden);
			break;
	}
	
	return nstate;
}

void QUIView::SetPosition(QRECT *rect)
{
	if(rect == NULL)
	{
		return;
	}
	[(UIView *)midview setFrame:CGRectMake(rect->left, rect->top, QRECTW(*rect), QRECTH(*rect))];
}

void QUIView::GetPosition(QRECT *rect)
{
	if(rect == NULL)
	{
		return;
	}
	if(midview == nil)
	{
		return;
	}
	rect->left = ((UIView *)midview).frame.origin.x;
	rect->top = ((UIView *)midview).frame.origin.y;
	rect->right = ((UIView *)midview).frame.origin.x+((UIView *)midview).frame.size.width;
	rect->bottom = ((UIView *)midview).frame.origin.y+((UIView *)midview).frame.size.height;
}

void QUIView::InvalidateDisplay(QRECT *rect)
{
	if(midview == nil)
	{
		return;
	}
	if(rect == NULL)
	{
		[(UIView *)midview setNeedsDisplay];
	}
	else
	{
		[(UIView *)midview setNeedsDisplayInRect:CGRectMake(rect->left, rect->top, rect->right, rect->bottom)];
	}
}

QINT QUIViewSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIView *pview;
	
	pview = (QUIView *)hdl;
	if(pview == NULL)
	{
		return QNO_OK;
	}
	if(code == QCD_MAKE)
	{
		pview->InitUIEvent();
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

id QUIView::FindViewByTag(QMDL env, QMDL parent, QMDL *module, QXML mxml, QSTR url, QINT *tag)
{
	id idview;
	QPNT ptag;
	QCHR vtag[QSTR_BUFF_SIZE];
	QINT nresult, ntag, nlen;
	QXML pattr;
	QMDL pparent;
	QUIView *pview;
	
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
	else
	{
		nresult = ((QModule *)module)->GetInitItem((QPNT)"tag", NULL, (QPNT*)&ntag, &nlen);
		if(nresult != QNO_OK)
		{
			ntag = 0;
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
		return nil;
	}
	
	idview = nil;
	pparent = parent;
	while(pparent)
	{
		pview = dynamic_cast<QUIView *>(pparent);
		if(pview->midview != nil)
		{
			idview = [pview->midview viewWithTag:ntag];
			if(idview != nil)
			{
				break;
			}
		}
		pparent = pparent->ParentModule();
	}
	if(module != NULL)
	{
		*module = pparent;
	}
	
	return idview;
}

QINT QUIView::MakeModuleInit(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	if(parent != NULL)
	{
		QMDL pparent;
		QUIView *pview;
		
		if(midview != nil && ((UIView *)midview).superview == nil)
		{
			pparent = parent;
			while(pparent)
			{
				pview = dynamic_cast<QUIView *>(pparent);
				if(pview != NULL)
				{
					if(pview->midview != nil)
					{
						[((UIView *)pview->midview) addSubview:(UIView *)midview];
						break;
					}
				}
				pparent = pparent->ParentModule();
			}
		}
		if(midcontroller != nil && ((UIViewController *)midcontroller).parentViewController == nil)
		{
			pparent = parent;
			while(pparent)
			{
				pview = dynamic_cast<QUIView *>(pparent);
				if(pview != NULL)
				{
					if(pview->midcontroller != nil)
					{
						[((UIViewController *)pview->midcontroller) addChildViewController:(UIViewController *)midcontroller];
						break;
					}
				}
				pparent = pparent->ParentModule();
			}
		}
	}
	
	return QNO_TRUE;
}

static QINT qui_view_children_hidden_prev_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIView *pself, *pview;
	
	pview = dynamic_cast<QUIView *>((QMDL)hdl);
	if(pview == NULL)
	{
		return QSCN_OK;
	}
	pself = (QUIView *)params[0];
	if(pview == pself)
	{
		return QSCN_OK;
	}
	if((QINT)params[1])
	{
		((UIView *)pview->midview).hidden = YES;
	}
	else
	{
		((UIView *)pview->midview).hidden = NO;
	}
	
	return QSCN_HORZ;
}

QINT QUIViewInitChildrenHidden(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUIView *pview;
	
	pview = (QUIView *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
	if(pview->midview == nil)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	if(pval)
	{
		pview->ScanModule(pview, qui_view_children_hidden_prev_cb, NULL, 2, pview, 1);
	}
	else
	{
		pview->ScanModule(pview, qui_view_children_hidden_prev_cb, NULL, 2, pview, 0);
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
	if(pview->midview == nil)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	if(pval)
	{
		((UIView *)pview->midview).hidden = YES;
	}
	else
	{
		((UIView *)pview->midview).hidden = NO;
	}
	
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
	if(pview->midview == nil)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	if(pval)
	{
		((UIView *)pview->midview).userInteractionEnabled = YES;
	}
	else
	{
		((UIView *)pview->midview).userInteractionEnabled = NO;
	}
	
	return QNO_OK;
}

QINT QUIViewInitBackgroundColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QINT nval, nlen;
	QUIView *pview;
	
	pview = (QUIView *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
	if(pview->midview == nil)
	{
		return QNO_FAIL;
	}
	nval = (QINT)params[2];
	nlen = (QINT)params[3];
	((UIView *)(pview->midview)).backgroundColor = [UIColor colorWithRed:qclrGetRColor(nval)/255.0
																   green:qclrGetGColor(nval)/255.0
																	blue:qclrGetBColor(nval)/255.0
																   alpha:qclrGetAColor(nval)/255.0];
	
	return QSCN_OK;
}

static QINT qmdl_find_uiview_by_tag_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QINT ntag;
	UIView *pdstview;
	QUIView *psrcview;
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
	pchild = (QMDL)params[2];
	if(pchild == pmodule)
	{
		return QSCN_HORZ;
	}
	ppfound = (QMDL *)params[1];
	if(ppfound == NULL)
	{
		return QSCN_END;
	}
	ntag = (QINT)params[0];
	pdstview = [(UIView *)(psrcview->midview) viewWithTag:ntag];
	if(pdstview == nil)
	{
		return QSCN_OK;
	}
	*ppfound = pmodule;
	
	return QSCN_END;
}

static QINT qmdlUIViewInitPosition(QMDL module, QINT flag, QSTR str, QINT len)
{
	// <UIView>.[width|height|left|top|right|bottom] [*|/] [int] [+|-] [int]
	QMDL pmodule;
	QSTR pchar, pbuff;
	QCHR vbuff[QSTR_BUFF_SIZE];
	QINT npos, nlen, nval, ncnt;
	UIView *psrcview, *pdstview, *pparentview;
	QINT nattrflag, nmulflag, ntimes, naddflag, nremainder;
	CGFloat fmultiplier;
	
	psrcview = (UIView *)(((QUIView *)module)->midview);
	if(psrcview == nil)
	{
		return QNO_TRUE;
	}
	pparentview = [psrcview superview];
	pdstview = nil;
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
		nremainder = qstrint(0, vbuff, NULL);
		goto PARSE_COMPUTE;
	}
	if(nlen <= 0)
	{
		pdstview = pparentview;
	}
	else
	{
		*pbuff = 0;
		ncnt = nlen;
		nval = qstrint(0, vbuff, &ncnt);
		if(ncnt != nlen)
		{
			pmodule = module->FindModule(NULL, 0, (QSTR)vbuff, 0);
			if(pmodule != NULL)
			{
				pdstview = (UIView *)(((QUIView *)pmodule)->midview);
			}
		}
		else
		{
			QINT ncount;
			QPNT vpprms[4];
			QMDL pchild, pfound;
			
			pfound = NULL;
			pchild = NULL;
			pmodule = module;
			while(pmodule)
			{
				ncount = 0;
				vpprms[ncount++] = (QPNT)nval;
				vpprms[ncount++] = &pfound;
				vpprms[ncount++] = pchild;
				pmodule->ScanModulex(pmodule, qmdl_find_uiview_by_tag_cb, NULL, vpprms, ncount);
				if(pfound != NULL)
				{
					break;
				}
				pchild = pmodule;
				pmodule = pmodule->ParentModule();
			}
			pmodule = pfound;
			if(pmodule != NULL)
			{
				pdstview = [(UIView *)(((QUIView *)pmodule)->midview) viewWithTag:nval];
			}
		}
		if(pdstview == NULL)
		{
			nremainder = nval;
			goto PARSE_COMPUTE;
		}
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
			nattrflag = NSLayoutAttributeCenterY;
			nlen = 7;
		}
		else if(qstrcmp(QSTR_ICS, (QPNT)(pchar+1), (QPNT)"hcenter", 7))
		{
			nattrflag = NSLayoutAttributeCenterX;
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
	else if(*pchar == '-')
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
			nremainder = 0;
			nlen = 0;
		}
		else
		{
			nremainder = nval;
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
	fmultiplier = ntimes;
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
	if(pdstview != NULL)
	{
		NSLayoutConstraint *constraint;
		for(constraint in pparentview.constraints)
		{
			if( (constraint.firstItem == psrcview) && (constraint.firstAttribute == (NSLayoutAttribute)flag) )
			{
				if( (constraint.secondItem == pdstview) && (constraint.secondAttribute == (NSLayoutAttribute)nattrflag) )
				{
					if(constraint.multiplier == fmultiplier)
					{
						break;
					}
				}
				constraint.active = NO;
			}
		}
		if(constraint != nil)
		{
			constraint.active = YES;
			constraint.constant = nremainder;
		}
		else
		{
			[pparentview addConstraint:
			 [NSLayoutConstraint constraintWithItem:psrcview
										  attribute:(NSLayoutAttribute)flag
										  relatedBy:NSLayoutRelationEqual
											 toItem:pdstview
										  attribute:(NSLayoutAttribute)nattrflag
										 multiplier:fmultiplier
										   constant:nremainder]];
		}
	}
	else
	{
		if(flag == NSLayoutAttributeWidth || flag == NSLayoutAttributeHeight)
		{
			NSLayoutConstraint *constraint;
			for(constraint in pparentview.constraints)
			{
				if( (constraint.firstItem == psrcview) && (constraint.firstAttribute == (NSLayoutAttribute)flag) )
				{
					if(constraint.multiplier == fmultiplier)
					{
						break;
					}
					constraint.active = NO;
				}
			}
			if(constraint != nil)
			{
				constraint.active = YES;
				constraint.constant = nremainder;
			}
			else
			{
				[pparentview addConstraint:
				 [NSLayoutConstraint constraintWithItem:psrcview
											  attribute:(NSLayoutAttribute)flag
											  relatedBy:NSLayoutRelationEqual
												 toItem:nil
											  attribute:(NSLayoutAttribute)flag
											 multiplier:fmultiplier
											   constant:nremainder]];
			}
		}
		else if(flag == NSLayoutAttributeLeft)
		{
			psrcview.frame.origin = CGPointMake(nremainder, psrcview.frame.origin.y);
		}
		else if(flag == NSLayoutAttributeTop)
		{
			psrcview.frame.origin = CGPointMake(psrcview.frame.origin.x, nremainder);
		}
		else if(flag == NSLayoutAttributeRight)
		{
			psrcview.frame.size = CGSizeMake(nremainder-psrcview.frame.origin.x, psrcview.frame.size.height);
		}
		else if(flag == NSLayoutAttributeBottom)
		{
			psrcview.frame.size = CGSizeMake(psrcview.frame.size.width, nremainder-psrcview.frame.origin.y);
		}
	}
	
	return QNO_TRUE;
}

#endif
