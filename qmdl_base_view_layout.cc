
#include "qmdl_base.h"


enum
{
	QLayoutAttrNone			= (0),
	QLayoutAttrWidth		= (QLayoutWidth-QLayoutWidth),
	QLayoutAttrHeight		= (QLayoutHeight-QLayoutWidth),
	QLayoutAttrLeft			= (QLayoutLeft-QLayoutWidth),
	QLayoutAttrTop			= (QLayoutTop-QLayoutWidth),
	QLayoutAttrRight		= (QLayoutRight-QLayoutWidth),
	QLayoutAttrBottom		= (QLayoutBottom-QLayoutWidth),
	QLayoutAttrType			= (QLayoutBottom-QLayoutWidth+1),
	QLayoutAttrAlign		= (QLayoutBottom-QLayoutWidth+2),
	QLayoutAttrFlag			= (QLayoutBottom-QLayoutWidth+3),
	
	// 8bits x 6: (QLayoutAttrWidth、QLayoutAttrHeight、QLayoutAttrLeft、QLayoutAttrTop、QLayoutAttrRight、QLayoutAttrBottom)
	QLayoutFlagEqual        = (QLayoutEqual>>QLayoutFlagMove),
	QLayoutFlagLess         = (QLayoutLess>>QLayoutFlagMove),
	QLayoutFlagMore         = (QLayoutMore>>QLayoutFlagMove),
	QLayoutFlagMPercent     = (QLayoutMPercent>>QLayoutFlagMove),
	QLayoutFlagRPercent     = (QLayoutRPercent>>QLayoutFlagMove),
	QLayoutFlagFinished     = (QLayoutFinished>>QLayoutFlagMove),
	
	// 8bits x 1: QLayoutAttrType
	QLayoutAppendNone		= (0),
	QLayoutAppendL			= (1),
	QLayoutAppendT			= (2),
	QLayoutAppendR			= (3),
	QLayoutAppendB			= (4),
	QLayoutAppendLT			= (5),
	QLayoutAppendTR			= (6),
	QLayoutAppendRB			= (7),
	QLayoutAppendBL			= (8),
	QLayoutAppendTL			= (9),
	QLayoutAppendRT			= (10),
	QLayoutAppendBR			= (11),
	QLayoutAppendLB			= (12),
	
	// 8bits x 1: QLayoutAttrAlign
	QLayoutAlignLeft        = 0x00000001,
	QLayoutAlignTop         = 0x00000002,
	QLayoutAlignRight       = 0x00000004,
	QLayoutAlignBottom      = 0x00000008,
	QLayoutAlignHCenter     = 0x00000010,
	QLayoutAlignVCenter     = 0x00000020,
	QLayoutAlignHDirection  = (QLayoutAlignLeft|QLayoutAlignHCenter|QLayoutAlignRight),
	QLayoutAlignVDirection  = (QLayoutAlignTop|QLayoutAlignVCenter|QLayoutAlignBottom),
	
	// 8bits x 1: QLayoutAttrFlag
	QLayoutFlagNewline      = 0x00000001,
	QLayoutFlagSelf         = 0x00000002,
	QLayoutFlagChildren     = 0x00000004,
};


#define QREALW(view) ((view)?(QRECTW((view)->mstlayout.rcposition)-(view)->mstlayout.rcmargin.left+(view)->mstlayout.rcmargin.right):(0))
#define QREALH(view) ((view)?(QRECTH((view)->mstlayout.rcposition)-(view)->mstlayout.rcmargin.top+(view)->mstlayout.rcmargin.bottom):(0))
#define QREALL(view) ((view)?((view)->mstlayout.rcmargin.left):(0))
#define QREALT(view) ((view)?((view)->mstlayout.rcmargin.top):(0))
#define QREALR(view) ((view)?((view)->mstlayout.rcposition.right-(view)->mstlayout.rcposition.left-(view)->mstlayout.rcmargin.right):(0))
#define QREALB(view) ((view)?((view)->mstlayout.rcposition.bottom-(view)->mstlayout.rcposition.top-(view)->mstlayout.rcmargin.bottom):(0))


struct qmdl_layout_item
{
	QBaseView *psrcview;
	QINT nsrcattr;													// QLayoutXXX
	QINT nflag;														// QLayoutFlagXXX
	QBaseView *pdstview;
	QINT ndstattr;													// QLayoutXXX
	QINT nmultiplier;
	QINT nremainder;
};


static QINT QViewSetLayoutFlag(QBaseView *view, QINT attr, QINT flag)
{
	QINT noldflag;
	
	if(view == NULL)
	{
		return 0;
	}
	noldflag = 0;
	switch(attr)
	{
		case QLayoutAttrWidth:
		case QLayoutAttrHeight:
		case QLayoutAttrLeft:
		case QLayoutAttrTop:
		case QLayoutAttrRight:
		case QLayoutAttrBottom:
		case QLayoutAttrType:
		case QLayoutAttrAlign:
		case QLayoutAttrFlag:
			noldflag = view->mstlayout.vflag[attr];
			view->mstlayout.vflag[attr] = flag;
			break;
			
		default:
			break;
	}
	
	return noldflag;
}

static QINT QViewGetLayoutFlag(QBaseView *view, QINT attr)
{
	if(view == NULL)
	{
		return 0;
	}
	switch(attr)
	{
		case QLayoutAttrWidth:
		case QLayoutAttrHeight:
		case QLayoutAttrLeft:
		case QLayoutAttrTop:
		case QLayoutAttrRight:
		case QLayoutAttrBottom:
		case QLayoutAttrType:
		case QLayoutAttrAlign:
		case QLayoutAttrFlag:
			return view->mstlayout.vflag[attr];
			break;
			
		default:
			break;
	}
	
	return 0;
}

static QINT QViewSetLayoutFinished(QBaseView *view, QINT attr, QINT flag)
{
	if(view == NULL)
	{
		return QNO_FAIL;
	}
	switch(attr)
	{
		case QLayoutWidth:
		case QLayoutHeight:
		case QLayoutLeft:
		case QLayoutTop:
		case QLayoutRight:
		case QLayoutBottom:
			if(flag)
			{
				view->mstlayout.vflag[attr-QLayoutWidth] |= QLayoutFlagFinished;
			}
			else
			{
				view->mstlayout.vflag[attr-QLayoutWidth] &= ~QLayoutFlagFinished;
			}
			break;
			
		default:
			return QNO_FAIL;
			break;
	}
	
	return QNO_OK;
}

static QINT QViewHasLayoutFinished(QBaseView *view, QINT attr)
{
	if(view == NULL)
	{
		return 0;
	}
	switch(attr)
	{
		case QLayoutWidth:
			if(view->mstlayout.vflag[QLayoutAttrWidth] & QLayoutFlagFinished)
			{
				return QLayoutFlagFinished;
			}
			if( view->mstlayout.vflag[QLayoutAttrLeft] & QLayoutFlagFinished &&
			    view->mstlayout.vflag[QLayoutAttrRight] & QLayoutFlagFinished )
			{
				return QLayoutFlagFinished;
			}
			break;
			
		case QLayoutHeight:
			if(view->mstlayout.vflag[QLayoutAttrHeight] & QLayoutFlagFinished)
			{
				return QLayoutFlagFinished;
			}
			if( view->mstlayout.vflag[QLayoutAttrTop] & QLayoutFlagFinished &&
			    view->mstlayout.vflag[QLayoutAttrBottom] & QLayoutFlagFinished )
			{
				return QLayoutFlagFinished;
			}
			break;
			
		case QLayoutLeft:
			return (view->mstlayout.vflag[QLayoutAttrLeft] & QLayoutFlagFinished);
			break;
			
		case QLayoutTop:
			return (view->mstlayout.vflag[QLayoutAttrTop] & QLayoutFlagFinished);
			break;
			
		case QLayoutRight:
			return (view->mstlayout.vflag[QLayoutAttrRight] & QLayoutFlagFinished);
			break;
			
		case QLayoutBottom:
			return (view->mstlayout.vflag[QLayoutAttrBottom] & QLayoutFlagFinished);
			break;
			
		case QLayoutHCenter:
			if( view->mstlayout.vflag[QLayoutAttrLeft] & QLayoutFlagFinished &&
			   view->mstlayout.vflag[QLayoutAttrRight] & QLayoutFlagFinished )
			{
				return QLayoutFlagFinished;
			}
			break;
			
		case QLayoutVCenter:
			if( view->mstlayout.vflag[QLayoutAttrTop] & QLayoutFlagFinished &&
			   view->mstlayout.vflag[QLayoutAttrBottom] & QLayoutFlagFinished )
			{
				return QLayoutFlagFinished;
			}
			break;
			
		default:
			return QLayoutFlagFinished;
			break;
	}
	
	return 0;
}

static QINT QViewGetRealPosition(struct qmdl_layout_item *item, QINT position)
{
	QMDL pparentmdl;
	QINT nposition;
	struct qmdl_layout_item *pitem;
	QBaseView *pdstview, *psrcview, *pparentview;
	
	nposition = position;
	pitem = item;
	if(pitem == NULL)
	{
		return nposition;
	}
	psrcview = pitem->psrcview;
	pdstview = pitem->pdstview;
	if(psrcview == NULL || pdstview == NULL)
	{
		return nposition;
	}
	switch(pitem->ndstattr)
	{
		case QLayoutLeft:
		case QLayoutTop:
		case QLayoutRight:
		case QLayoutBottom:
		case QLayoutHCenter:
		case QLayoutVCenter:
			if(psrcview->ParentModule() != pdstview->ParentModule())
			{
				// 不同父view下，需要换算成根view下的绝对位置得出通父view下的相对位置，再进行计算。
				pparentmdl = psrcview->ParentModule();
				while(pparentmdl)
				{
					pparentview = dynamic_cast<QBaseView*>(pparentmdl);
					pparentmdl = pparentmdl->ParentModule();
					if(pparentmdl == NULL)
					{
						// 根view的位置内部相对位置增加值。
						break;
					}
					if(pparentview != NULL)
					{
						switch(pitem->ndstattr)
						{
							case QLayoutLeft:
							case QLayoutRight:
							case QLayoutHCenter:
								nposition += pparentview->mstlayout.rcposition.left;
								break;
								
							case QLayoutTop:
							case QLayoutBottom:
							case QLayoutVCenter:
								nposition += pparentview->mstlayout.rcposition.top;
								break;
						}
					}
				}
				pparentmdl = pdstview->ParentModule();
				while(pparentmdl)
				{
					pparentview = dynamic_cast<QBaseView*>(pparentmdl);
					pparentmdl = pparentmdl->ParentModule();
					if(pparentmdl == NULL)
					{
						// 根view的位置内部相对位置增加值。
						break;
					}
					if(pparentview != NULL)
					{
						switch(pitem->ndstattr)
						{
							case QLayoutLeft:
							case QLayoutRight:
							case QLayoutHCenter:
								nposition -= pparentview->mstlayout.rcposition.left;
								break;
								
							case QLayoutTop:
							case QLayoutBottom:
							case QLayoutVCenter:
								nposition -= pparentview->mstlayout.rcposition.top;
								break;
						}
					}
				}
			}
			break;
	}
	
	return nposition;
}

static QINT QViewAddLayoutRelation(QBaseView *view, QINT attr, struct qmdl_layout_item *relation)
{
	QHDL hitem;
	QBaseView *pdstview;
	struct qmdl_layout_item *pitem;
	
	if(view == NULL || relation == NULL)
	{
		return QNO_FAIL;
	}
	pdstview = relation->pdstview;
	if(pdstview == NULL)
	{
		return QNO_FAIL;
	}
	switch(attr)
	{
		case QLayoutWidth:
		case QLayoutHeight:
		case QLayoutLeft:
		case QLayoutTop:
		case QLayoutRight:
		case QLayoutBottom:
			break;
			
		default:
			return QNO_FAIL;
			break;
	}
	hitem = qlinkHead(QLINK_BL_RELATION, pdstview->mstlayout.hrelation);
	while(hitem)
	{
		pitem = (struct qmdl_layout_item *)qlinkData(QLINK_BL_RELATION, hitem);
		if(pitem != NULL)
		{
			if(pitem->psrcview == view && pitem->nsrcattr == attr)
			{
				break;
			}
		}
		hitem = qlinkNext(QLINK_BL_RELATION, hitem);
	}
	if(hitem == NULL)
	{
		hitem = qlinkMake(QLINK_BL_RELATION, NULL, NULL, sizeof(struct qmdl_layout_item));
		if(hitem == NULL)
		{
			return QNO_FAIL;
		}
		pdstview->mstlayout.hrelation = qlinkInsertNext(QLINK_BL_RELATION, pdstview->mstlayout.hrelation, NULL, hitem);
	}
	pitem = (struct qmdl_layout_item *)qlinkData(QLINK_BL_RELATION, hitem);
	if(pitem == NULL)
	{
		return QNO_FAIL;
	}
	memcpy(pitem, relation, sizeof(struct qmdl_layout_item));
	pitem->nflag &= ~QLayoutFlagFinished;
	
	return QNO_OK;
}

static QINT QViewFinishLayoutRelation(QBaseView *view)
{
	QHDL hitem, hprev;
	QHDL vpitem[QSCN_STACK_CNT];
	struct qmdl_layout_item *pitem;
	QBaseView *vpview[QSCN_STACK_CNT];
	QBaseView *pcurview, *psrcview, *pdstview, *pchild, *pparent;
	QINT ndepty, nposition, nlflag, ntflag, nrflag, nbflag;
	
	pcurview = view;
	if(pcurview == NULL)
	{
		return QNO_FAIL;
	}
	if(pcurview->mstlayout.hrelation == NULL)
	{
		return QNO_OK;
	}
	ndepty = 0;
	vpitem[0] = NULL;
	vpview[0] = NULL;
	while(pcurview)
	{
		hitem = qlinkHead(QLINK_BL_RELATION, pcurview->mstlayout.hrelation);
		while(hitem)
		{
			pitem = (struct qmdl_layout_item *)qlinkData(QLINK_BL_RELATION, hitem);
			if(pitem != NULL && !(pitem->nflag&QLayoutFlagFinished))
			{
				psrcview = pitem->psrcview;
				pdstview = pitem->pdstview;
				if(psrcview != NULL && pdstview != NULL && QViewHasLayoutFinished(pdstview, pitem->ndstattr))
				{
					nposition = 0;
					switch(pitem->ndstattr)
					{
						case QLayoutWidth:		nposition = QRECTW(pdstview->mstlayout.rcposition);											break;
						case QLayoutHeight:		nposition = QRECTH(pdstview->mstlayout.rcposition);											break;
							
						case QLayoutLeft:		nposition = pdstview->mstlayout.rcposition.left;											break;
						case QLayoutTop:		nposition = pdstview->mstlayout.rcposition.top;												break;
						case QLayoutRight:		nposition = pdstview->mstlayout.rcposition.right;											break;
						case QLayoutBottom:		nposition = pdstview->mstlayout.rcposition.bottom;											break;
							
						case QLayoutHCenter:	nposition = (pdstview->mstlayout.rcposition.left+pdstview->mstlayout.rcposition.right)>>1;	break;
						case QLayoutVCenter:	nposition = (pdstview->mstlayout.rcposition.top+pdstview->mstlayout.rcposition.bottom)>>1;	break;
					}
					nposition = QViewGetRealPosition(pitem, nposition);
					nposition = nposition*pitem->nmultiplier+pitem->nremainder;
					switch(pitem->nsrcattr)
					{
						case QLayoutWidth:
							nlflag = QViewHasLayoutFinished(psrcview, QLayoutLeft);
							nrflag = QViewHasLayoutFinished(psrcview, QLayoutRight);
							if(nlflag && nrflag)
							{
								// 左右位置已完成，强行修改右侧位置。
								psrcview->mstlayout.rcposition.right = psrcview->mstlayout.rcposition.left+nposition;
							}
							else if(nlflag)
							{
								// 左侧位置已完成，修改右侧位置并设置完成。
								psrcview->mstlayout.rcposition.right = psrcview->mstlayout.rcposition.left+nposition;
								QViewSetLayoutFinished(psrcview, QLayoutRight, 1);
							}
							else if(nrflag)
							{
								// 右侧位置已完成，修改左侧位置并设置完成。
								psrcview->mstlayout.rcposition.left = psrcview->mstlayout.rcposition.right-nposition;
								QViewSetLayoutFinished(psrcview, QLayoutLeft, 1);
							}
							else
							{
								// 左右位置未完成，左侧为0右侧为宽度，并设置宽度为完成。
								psrcview->mstlayout.rcposition.left = 0;
								psrcview->mstlayout.rcposition.right = nposition;
							}
							QViewSetLayoutFinished(psrcview, QLayoutWidth, 1);
							break;
							
						case QLayoutHeight:
							ntflag = QViewHasLayoutFinished(psrcview, QLayoutTop);
							nbflag = QViewHasLayoutFinished(psrcview, QLayoutBottom);
							if(ntflag && nbflag)
							{
								// 上下位置已完成，强行修改下侧位置。
								psrcview->mstlayout.rcposition.bottom = psrcview->mstlayout.rcposition.top+nposition;
							}
							else if(ntflag)
							{
								// 上侧位置已完成，修改下侧位置并设置完成。
								psrcview->mstlayout.rcposition.bottom = psrcview->mstlayout.rcposition.top+nposition;
								QViewSetLayoutFinished(psrcview, QLayoutBottom, 1);
							}
							else if(nbflag)
							{
								// 下侧位置已完成，修改上侧位置并设置完成。
								psrcview->mstlayout.rcposition.top = psrcview->mstlayout.rcposition.bottom-nposition;
								QViewSetLayoutFinished(psrcview, QLayoutTop, 1);
							}
							else
							{
								// 上下位置未完成，上侧为0右侧为高度，并设置高度为完成。
								psrcview->mstlayout.rcposition.top = 0;
								psrcview->mstlayout.rcposition.bottom = nposition;
							}
							QViewSetLayoutFinished(psrcview, QLayoutHeight, 1);
							break;
							
						case QLayoutLeft:
							if(QViewHasLayoutFinished(psrcview, QLayoutWidth))
							{
								psrcview->mstlayout.rcposition.right = nposition+QRECTW(psrcview->mstlayout.rcposition);
								QViewSetLayoutFinished(psrcview, QLayoutRight, 1);
							}
							psrcview->mstlayout.rcposition.left = nposition;
							QViewSetLayoutFinished(psrcview, QLayoutLeft, 1);
							break;
							
						case QLayoutTop:
							if(QViewHasLayoutFinished(psrcview, QLayoutHeight))
							{
								psrcview->mstlayout.rcposition.bottom = nposition+QRECTH(psrcview->mstlayout.rcposition);
								QViewSetLayoutFinished(psrcview, QLayoutBottom, 1);
							}
							psrcview->mstlayout.rcposition.top = nposition;
							QViewSetLayoutFinished(psrcview, QLayoutTop, 1);
							break;
							
						case QLayoutRight:
							if(QViewHasLayoutFinished(psrcview, QLayoutWidth))
							{
								psrcview->mstlayout.rcposition.left = nposition-QRECTW(psrcview->mstlayout.rcposition);
								QViewSetLayoutFinished(psrcview, QLayoutLeft, 1);
							}
							psrcview->mstlayout.rcposition.right = nposition;
							QViewSetLayoutFinished(psrcview, QLayoutRight, 1);
							break;
							
						case QLayoutBottom:
							if(QViewHasLayoutFinished(psrcview, QLayoutHeight))
							{
								psrcview->mstlayout.rcposition.top = nposition-QRECTH(psrcview->mstlayout.rcposition);
								QViewSetLayoutFinished(psrcview, QLayoutTop, 1);
							}
							psrcview->mstlayout.rcposition.bottom = nposition;
							QViewSetLayoutFinished(psrcview, QLayoutBottom, 1);
							break;
					}
					pitem->nflag |= QLayoutFlagFinished;
				}
			}
			hitem = qlinkNext(QLINK_BL_RELATION, hitem);
		}
		vpview[ndepty] = pcurview;
		ndepty++;
		// 获取第一个关联源view，继续触发排版完成。
		pchild = NULL;
		hitem = qlinkHead(QLINK_BL_RELATION, pcurview->mstlayout.hrelation);
		while(hitem)
		{
			pitem = (struct qmdl_layout_item *)qlinkData(QLINK_BL_RELATION, hitem);
			if(pitem != NULL)
			{
				psrcview = pitem->psrcview;
				pdstview = pitem->pdstview;
				if(psrcview != NULL && pdstview != NULL &&
				   psrcview->mstlayout.hrelation != NULL && QViewHasLayoutFinished(psrcview, pitem->nsrcattr))
				{
					// 判断是否已经处理过该控件的关联树，如已处理则跳过。
					hprev = qlinkPrev(QLINK_BL_RELATION, hitem);
					while(hprev)
					{
						pitem = (struct qmdl_layout_item *)qlinkData(QLINK_BL_RELATION, hprev);
						if(pitem != NULL)
						{
							if(pitem->psrcview == psrcview)
							{
								break;
							}
						}
						hprev = qlinkPrev(QLINK_BL_RELATION, hprev);
					}
					if(hprev == NULL)
					{
						vpitem[ndepty] = hitem;
						pchild = psrcview;
						break;
					}
				}
			}
			hitem = qlinkNext(QLINK_BL_RELATION, hitem);
		}
		if(pchild == NULL)
		{
			// 没有子树，遍历下一兄弟结点。
			ndepty--;
			if(ndepty >= 0 && pcurview != view)
			{
				while(pchild == NULL)
				{
					hitem = vpitem[ndepty];
					pparent = vpview[ndepty];
					if(hitem == NULL || pparent == NULL)
					{
						break;
					}
					hitem = qlinkNext(QLINK_BL_RELATION, hitem);
					while(hitem)
					{
						pitem = (struct qmdl_layout_item *)qlinkData(QLINK_BL_RELATION, hitem);
						if(pitem != NULL)
						{
							psrcview = pitem->psrcview;
							pdstview = pitem->pdstview;
							if(psrcview != NULL && pdstview != NULL &&
							   psrcview->mstlayout.hrelation != NULL && QViewHasLayoutFinished(psrcview, pitem->nsrcattr))
							{
								// 判断是否已经处理过该控件的关联树，如已处理则跳过。
								hprev = qlinkPrev(QLINK_BL_RELATION, hitem);
								while(hprev)
								{
									pitem = (struct qmdl_layout_item *)qlinkData(QLINK_BL_RELATION, hprev);
									if(pitem != NULL)
									{
										if(pitem->psrcview == psrcview)
										{
											break;
										}
									}
									hprev = qlinkPrev(QLINK_BL_RELATION, hprev);
								}
								if(hprev == NULL)
								{
									vpitem[ndepty] = hitem;
									pchild = psrcview;
									break;
								}
							}
						}
						hitem = qlinkNext(QLINK_BL_RELATION, hitem);
					}
					if(pchild != NULL)
					{
						break;
					}
					ndepty--;
					if(ndepty < 0 || pparent == view)
					{
						// 到达根节点或遍历顶结点。
						break;
					}
				}
			}
		}
		pcurview = pchild;
	}
	
	return QNO_OK;
}

QINT QBaseView::SetNewline(QINT flag)
{
	QINT nflag;
	
	nflag = QViewGetLayoutFlag(this, QLayoutAttrFlag);
	if(flag)
	{
		nflag |= QLayoutFlagNewline;
	}
	else
	{
		nflag &= ~QLayoutFlagNewline;
	}
	
	return QViewSetLayoutFlag(this, QLayoutAttrFlag, nflag);
}

QINT QBaseView::GetNewline()
{
	QINT nflag;
	
	nflag = QViewGetLayoutFlag(this, QLayoutAttrFlag);
	
	return (nflag & QLayoutFlagNewline);
}

QINT QBaseView::HasLayoutPosition()
{
	if(mstlayout.hposition != NULL)
	{
		return QNO_TRUE;
	}
	if(mstlayout.hchildren != NULL)
	{
		return QNO_TRUE;
	}
	if( mstlayout.rcposition.left != 0 ||
	    mstlayout.rcposition.top != 0 ||
	    mstlayout.rcposition.right != 0 ||
	    mstlayout.rcposition.bottom != 0 )
	{
		return QNO_TRUE;
	}
	if( mstlayout.rcmargin.left != 0 ||
	   mstlayout.rcmargin.top != 0 ||
	   mstlayout.rcmargin.right != 0 ||
	   mstlayout.rcmargin.bottom != 0 )
	{
		return QNO_TRUE;
	}
	
	return QNO_FALSE;
}

void QBaseView::SetLayoutPosition(QRECT *rect)
{
	if(rect == NULL)
	{
		return;
	}
	memcpy(&mstlayout.rcposition, rect, sizeof(QRECT));
}

void QBaseView::SetLayoutMargin(QRECT *rect)
{
	if(rect == NULL)
	{
		return;
	}
	memcpy(&mstlayout.rcmargin, rect, sizeof(QRECT));
}

void QBaseView::GetLayoutPosition(QRECT *rect)
{
	if(rect == NULL)
	{
		return;
	}
	memcpy(rect, &mstlayout.rcposition, sizeof(QRECT));
}

void QBaseView::GetLayoutMargin(QRECT *rect)
{
	if(rect == NULL)
	{
		return;
	}
	memcpy(rect, &mstlayout.rcmargin, sizeof(QRECT));
}

static QINT qmdl_move_position_prev_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QBaseView *pview;
	
	pview = dynamic_cast<QBaseView *>((QMDL)hdl);
	if(pview == NULL)
	{
		return QSCN_OK;
	}
	pview->mstlayout.rcposition.left += (QINT)params[0];
	pview->mstlayout.rcposition.top += (QINT)params[1];
	pview->mstlayout.rcposition.right += (QINT)params[0];
	pview->mstlayout.rcposition.bottom += (QINT)params[1];
	
	return QSCN_OK;
}

void QBaseView::MovePosition(QINT x, QINT y)
{
	QINT ncount;
	QPNT vpprms[8];
	
	if(x == this->mstlayout.rcposition.left && y == this->mstlayout.rcposition.top)
	{
		return;
	}
	ncount = 0;
	vpprms[ncount++] = (QPNT)x;
	vpprms[ncount++] = (QPNT)y;
	this->ScanModulex(NULL, qmdl_move_position_prev_cb, NULL, vpprms, ncount);
}

void QBaseView::AddPositionRule(QINT srcattr, QMDL dstmdl, QINT dstattr, QFLT multiplier, QINT remainder)
{
	QHDL hitem;
	struct qmdl_layout_item *pitem;
	
	hitem = qlinkHead(QLINK_NONE, mstlayout.hposition);
	while(hitem)
	{
		pitem = (struct qmdl_layout_item *)qlinkData(QLINK_NONE, hitem);
		if(pitem != NULL)
		{
			if(pitem->nsrcattr == (srcattr&QLayoutAttrMask))
			{
				break;
			}
		}
		hitem = qlinkNext(QLINK_NONE, hitem);
	}
	if(hitem == NULL)
	{
		hitem = qlinkMake(QLINK_NONE, NULL, NULL, sizeof(struct qmdl_layout_item));
		if(hitem == NULL)
		{
			return;
		}
		mstlayout.hposition = qlinkInsertNext(QLINK_NONE, mstlayout.hposition, NULL, hitem);
	}
	pitem = (struct qmdl_layout_item *)qlinkData(QLINK_NONE, hitem);
	if(pitem == NULL)
	{
		return;
	}
	pitem->psrcview = this;
	pitem->nsrcattr = (srcattr&QLayoutAttrMask);
	pitem->nflag = (dstattr&QLayoutFlagMask)>>QLayoutFlagMove;
	pitem->pdstview = dynamic_cast<QBaseView*>(dstmdl);
	pitem->ndstattr = (dstattr&QLayoutAttrMask);
	pitem->nmultiplier = multiplier;
	pitem->nremainder = remainder;
}

void QBaseView::AddLayoutRule(QINT srcattr, QMDL dstmdl, QINT dstattr, QFLT multiplier, QINT remainder)
{
	QHDL hitem;
	struct qmdl_layout_item *pitem;
	
	hitem = qlinkHead(QLINK_NONE, mstlayout.hchildren);
	while(hitem)
	{
		pitem = (struct qmdl_layout_item *)qlinkData(QLINK_NONE, hitem);
		if(pitem != NULL)
		{
			if(pitem->nsrcattr == (srcattr&QLayoutAttrMask))
			{
				break;
			}
		}
		hitem = qlinkNext(QLINK_NONE, hitem);
	}
	if(hitem == NULL)
	{
		hitem = qlinkMake(QLINK_NONE, NULL, NULL, sizeof(struct qmdl_layout_item));
		if(hitem == NULL)
		{
			return;
		}
		mstlayout.hchildren = qlinkInsertNext(QLINK_NONE, mstlayout.hchildren, NULL, hitem);
	}
	pitem = (struct qmdl_layout_item *)qlinkData(QLINK_NONE, hitem);
	if(pitem == NULL)
	{
		return;
	}
	pitem->psrcview = this;
	pitem->nsrcattr = (srcattr&QLayoutAttrMask);
	pitem->nflag = (dstattr&QLayoutFlagMask)>>QLayoutFlagMove;
	pitem->pdstview = dynamic_cast<QBaseView*>(dstmdl);
	pitem->ndstattr = (dstattr&QLayoutAttrMask);
	pitem->nmultiplier = multiplier;
	pitem->nremainder = remainder;
}

// 返回排版类型。
static QINT QViewGetLayoutType(struct qmdl_layout_item items[])
{
	if(items[QLayoutAttrLeft].ndstattr == QLayoutAppend)
	{
		if(items[QLayoutAttrTop].ndstattr == QLayoutAppend)
		{
			if(items[QLayoutAttrLeft].nmultiplier >= items[QLayoutAttrTop].nmultiplier)
			{
				// 左上角append。
				return QLayoutAppendLT;
			}
			else
			{
				// 上左角append。
				return QLayoutAppendTL;
			}
		}
		else if(items[QLayoutAttrBottom].ndstattr == QLayoutAppend)
		{
			if(items[QLayoutAttrLeft].nmultiplier >= items[QLayoutAttrBottom].nmultiplier)
			{
				// 左下角append。
				return QLayoutAppendLB;
			}
			else
			{
				// 下左角append。
				return QLayoutAppendBL;
			}
		}
		else
		{
			// 左端append。
			return QLayoutAppendL;
		}
	}
	else if(items[QLayoutAttrTop].ndstattr == QLayoutAppend)
	{
		if(items[QLayoutAttrRight].ndstattr == QLayoutAppend)
		{
			if(items[QLayoutAttrTop].nmultiplier >= items[QLayoutAttrRight].nmultiplier)
			{
				// 上右角append。
				return QLayoutAppendTR;
			}
			else
			{
				// 右上角append。
				return QLayoutAppendRT;
			}
		}
		else
		{
			// 右端append。
			return QLayoutAppendT;
		}
	}
	else if(items[QLayoutAttrRight].ndstattr == QLayoutAppend)
	{
		if(items[QLayoutAttrBottom].ndstattr == QLayoutAppend)
		{
			if(items[QLayoutAttrRight].nmultiplier >= items[QLayoutAttrBottom].nmultiplier)
			{
				// 右下角append。
				return QLayoutAppendRB;
			}
			else
			{
				// 下右角append。
				return QLayoutAppendBR;
			}
		}
		else
		{
			// 右端append。
			return QLayoutAppendR;
		}
	}
	else if(items[QLayoutAttrBottom].ndstattr == QLayoutAppend)
	{
		// 下端append。
		return QLayoutAppendB;
	}
	
	return QLayoutAppendNone;
}

// 计算排版信息的最终取值。
static QINT QViewGetLayoutPosition(QBaseView *view, QBaseView *parent, QINT type, struct qmdl_layout_item items[], QINT attr)
{
	struct qmdl_layout_item *pitem;
	QBaseView *pdstview, *psrcview;
	QINT nattr, nposition, nmpercent, nrpercent;
	
	if(items == NULL)
	{
		return 0;
	}
	nattr = attr-QLayoutWidth;
	if(nattr < QLayoutAttrWidth || nattr > QLayoutAttrBottom)
	{
		return 0;
	}
	if(type != QLayoutAppendNone)
	{
		if(nattr != QLayoutAttrWidth && nattr != QLayoutAttrHeight)
		{
			// append版面不支持相对位置。
			return 0;
		}
	}
	pitem = &items[nattr];
	if(pitem->nsrcattr == QLayoutNone)
	{
		return 0;
	}
	psrcview = view;
	pdstview = pitem->pdstview;
	if(psrcview == NULL || pdstview == NULL)
	{
		// 没有依赖关系。
		QViewSetLayoutFinished(psrcview, pitem->nsrcattr, 1);
		nrpercent = pitem->nflag & QLayoutFlagRPercent;
		return pitem->nremainder/(nrpercent?100:1);
	}
	// 判断相对view的相应位置是否准备好。
	if(!QViewHasLayoutFinished(pdstview, pitem->ndstattr))
	{
		// 没有准备好，添加相关性规则项。
		QViewAddLayoutRelation(psrcview, pitem->nsrcattr, pitem);
		QViewSetLayoutFinished(psrcview, pitem->nsrcattr, 0);
		return 0;
	}
	QViewSetLayoutFinished(psrcview, pitem->nsrcattr, 1);
	// 计算相对位置。
	nposition = 0;
	switch(pitem->ndstattr)
	{
		case QLayoutWidth:		nposition = QRECTW(pdstview->mstlayout.rcposition);											break;
		case QLayoutHeight:		nposition = QRECTH(pdstview->mstlayout.rcposition);											break;
			
		case QLayoutLeft:		nposition = pdstview->mstlayout.rcposition.left;											break;
		case QLayoutTop:		nposition = pdstview->mstlayout.rcposition.top;												break;
		case QLayoutRight:		nposition = pdstview->mstlayout.rcposition.right;											break;
		case QLayoutBottom:		nposition = pdstview->mstlayout.rcposition.bottom;											break;
			
		case QLayoutHCenter:	nposition = (pdstview->mstlayout.rcposition.left+pdstview->mstlayout.rcposition.right)>>1;	break;
		case QLayoutVCenter:	nposition = (pdstview->mstlayout.rcposition.top+pdstview->mstlayout.rcposition.bottom)>>1;	break;
			
		case QLayoutAppend:		return 0;																					break;
		case QLayoutDiv:		return 0;																					break;
			
		case QLayoutCenter:
			// 因父亲和自身的view尺寸有可能不确定，居中位置留到子view都确定位置后统一做处理。
			switch(attr)
			{
				case QLayoutAttrWidth:	return pitem->nremainder;															break;
				case QLayoutAttrHeight:	return pitem->nremainder;															break;
				default:				return 0;																			break;
			}
			break;
	}
	// 计算不同父view下的相对位置。
	nposition = QViewGetRealPosition(pitem, nposition);
	nmpercent = pitem->nflag & QLayoutFlagMPercent;
	
	return nposition*pitem->nmultiplier/(nmpercent?100:1)+pitem->nremainder;
}

// 计算排版信息的空隙取值。
static QINT QViewGetLayoutMargin(QINT type, struct qmdl_layout_item items[], QINT attr)
{
	struct qmdl_layout_item *pitem;
	
	if(items == NULL || attr < QLayoutAttrLeft || attr > QLayoutAttrBottom)
	{
		return 0;
	}
	pitem = &items[attr];
	if(pitem->nsrcattr == QLayoutNone)
	{
		return 0;
	}
	if(type != QLayoutAppendNone)
	{
		// append类型下取值都为空隙值。
		return pitem->nremainder;
	}
	// 非append类型下如果横纵维度上有一方出现居中，则也为空隙值。
	switch(pitem->ndstattr)
	{
		case QLayoutLeft:
		case QLayoutRight:
			if(items[QLayoutAttrLeft].ndstattr == QLayoutCenter || items[QLayoutAttrRight].ndstattr == QLayoutCenter)
			{
				return pitem->nremainder;
			}
			break;
			
		case QLayoutTop:
		case QLayoutBottom:
			if(items[QLayoutAttrTop].ndstattr == QLayoutCenter || items[QLayoutAttrBottom].ndstattr == QLayoutCenter)
			{
				return pitem->nremainder;
			}
			break;
	}
	
	return 0;
}

// 获取父亲和最后孩子view。
static void QViewGetParentAndPrevView(QBaseView *view, QBaseView **parent, QBaseView **prev)
{
	QMDL pmodule;
	QBaseView *pview;
	
	if(view == NULL)
	{
		return;
	}
	if(parent != NULL)
	{
		*parent = NULL;
	}
	if(prev != NULL)
	{
		*prev = NULL;
	}
	pview = NULL;
	pmodule = view->ParentModule();
	while(pmodule)
	{
		pview = dynamic_cast<QBaseView *>(pmodule);
		if(pview != NULL)
		{
			if(pview->GetState(QStateVisual))
			{
				break;
			}
		}
		pmodule = pmodule->ParentModule();
	}
	if(parent != NULL)
	{
		*parent = pview;
	}
	pview = NULL;
	pmodule = view->PrevModule();
	while(pmodule)
	{
		pview = dynamic_cast<QBaseView *>(pmodule);
		if(pview != NULL)
		{
			if(pview->GetState(QStateVisual))
			{
				break;
			}
		}
		pmodule = pmodule->PrevModule();
	}
	if(prev != NULL)
	{
		*prev = pview;
	}
}

static QINT QViewGetPeakValue(QBaseView *view, QINT attr)
{
	QINT nval;
	QMDL pprevmdl;
	QBaseView *pcurview, *pprevview;
	
	if(view == NULL)
	{
		return 0;
	}
	switch(attr)
	{
		case QLayoutAttrLeft:		nval = view->mstlayout.rcposition.left;		break;
		case QLayoutAttrTop:		nval = view->mstlayout.rcposition.top;		break;
		case QLayoutAttrRight:		nval = view->mstlayout.rcposition.right;	break;
		case QLayoutAttrBottom:		nval = view->mstlayout.rcposition.bottom;	break;
		default:					return 0;									break;
	}
	pprevmdl = view;
	pprevview = NULL;
	while(pprevmdl)
	{
		pcurview = dynamic_cast<QBaseView *>(pprevmdl);
		if(pcurview != NULL)
		{
			if(pcurview->GetState(QStateVisual))
			{
				if(attr == QLayoutAttrLeft)
				{
					if(pprevview != NULL && pcurview->mstlayout.rcposition.left >= pprevview->mstlayout.rcposition.right)
					{
						// 上一行位置，提前跳出。
						break;
					}
					else if(nval > pcurview->mstlayout.rcposition.left)
					{
						nval = pcurview->mstlayout.rcposition.left;
					}
				}
				else if(attr == QLayoutAttrTop)
				{
					if(pprevview != NULL && pcurview->mstlayout.rcposition.top >= pprevview->mstlayout.rcposition.bottom)
					{
						// 上一行位置，提前跳出。
						break;
					}
					else if(nval > pcurview->mstlayout.rcposition.top)
					{
						nval = pcurview->mstlayout.rcposition.top;
					}
				}
				else if(attr == QLayoutAttrRight)
				{
					if(pprevview != NULL && pcurview->mstlayout.rcposition.right <= pprevview->mstlayout.rcposition.left)
					{
						// 上一行位置，提前跳出。
						break;
					}
					else if(nval < pcurview->mstlayout.rcposition.right)
					{
						nval = pcurview->mstlayout.rcposition.right;
					}
				}
				else if(attr == QLayoutAttrBottom)
				{
					if(pprevview != NULL && pcurview->mstlayout.rcposition.bottom <= pprevview->mstlayout.rcposition.top)
					{
						// 上一行位置，提前跳出。
						break;
					}
					else if(nval < pcurview->mstlayout.rcposition.bottom)
					{
						nval = pcurview->mstlayout.rcposition.bottom;
					}
				}
				else
				{
					break;
				}
				pprevview = pcurview;
			}
		}
		pprevmdl = pprevmdl->PrevModule();
	}
	
	return nval;
}

// 处理无append类型。
static QINT QViewGetBasePositionMargin(QBaseView *view, QBaseView *parent, QINT type, struct qmdl_layout_item items[])
{
	QRECT rcposition;
	QINT nwidth, nheight, nflag;
	
	if(view == NULL || items == NULL)
	{
		return QNO_FAIL;
	}
	// 保存比较标志。
	QViewSetLayoutFlag(view, QLayoutAttrWidth, items[QLayoutAttrWidth].nflag);
	QViewSetLayoutFlag(view, QLayoutAttrHeight, items[QLayoutAttrHeight].nflag);
	QViewSetLayoutFlag(view, QLayoutAttrLeft, items[QLayoutAttrLeft].nflag);
	QViewSetLayoutFlag(view, QLayoutAttrTop, items[QLayoutAttrTop].nflag);
	QViewSetLayoutFlag(view, QLayoutAttrRight, items[QLayoutAttrRight].nflag);
	QViewSetLayoutFlag(view, QLayoutAttrBottom, items[QLayoutAttrBottom].nflag);
	
	nwidth = view->GetWidth();
	if(nwidth > 0)
	{
		// 用户指定宽度优先。
		QViewSetLayoutFinished(view, QLayoutWidth, 1);
	}
	else
	{
		nwidth = 0;
		if(items[QLayoutAttrWidth].nsrcattr != QLayoutNone)
		{
			nwidth = QViewGetLayoutPosition(view, parent, type, items, QLayoutWidth);
		}
	}
	nheight = view->GetHeight();
	if(nheight > 0)
	{
		// 用户指定高度优先。
		QViewSetLayoutFinished(view, QLayoutWidth, 1);
	}
	else
	{
		nheight = 0;
		if(items[QLayoutAttrHeight].nsrcattr != QLayoutNone)
		{
			nheight = QViewGetLayoutPosition(view, parent, type, items, QLayoutHeight);
		}
	}
	// 保存初步横向位置。
	if(items[QLayoutAttrLeft].nsrcattr != QLayoutNone)
	{
		rcposition.left = QViewGetLayoutPosition(view, parent, type, items, QLayoutLeft);
		if(items[QLayoutAttrRight].nsrcattr != QLayoutNone)
		{
			rcposition.right = QViewGetLayoutPosition(view, parent, type, items, QLayoutRight);
			if(nwidth > 0)
			{
				// 宽度优先。
				rcposition.right = rcposition.left+nwidth;
				if(QViewHasLayoutFinished(view, QLayoutLeft))
				{
					QViewSetLayoutFinished(view, QLayoutRight, 1);
				}
			}
		}
		else
		{
			if(nwidth > 0)
			{
				rcposition.right = rcposition.left+nwidth;
				if(QViewHasLayoutFinished(view, QLayoutLeft))
				{
					QViewSetLayoutFinished(view, QLayoutRight, 1);
				}
			}
			else
			{
				rcposition.right = rcposition.left;
			}
		}
	}
	else
	{
		if(items[QLayoutAttrRight].nsrcattr != QLayoutNone)
		{
			rcposition.right = QViewGetLayoutPosition(view, parent, type, items, QLayoutRight);
			if(nwidth > 0)
			{
				if(QViewHasLayoutFinished(view, QLayoutRight))
				{
					rcposition.left = rcposition.right-nwidth;
					QViewSetLayoutFinished(view, QLayoutLeft, 1);
				}
				else
				{
					rcposition.left = 0;
					rcposition.right = nwidth;
				}
			}
			else
			{
				rcposition.left = rcposition.right;
			}
		}
		else
		{
			rcposition.left = 0;
			if(nwidth != 0)
			{
				rcposition.right = rcposition.left+nwidth;
			}
			else
			{
				rcposition.right = rcposition.left;
			}
		}
	}
	// 保存初步纵向位置。
	if(items[QLayoutAttrTop].nsrcattr != QLayoutNone)
	{
		rcposition.top = QViewGetLayoutPosition(view, parent, type, items, QLayoutTop);
		if(items[QLayoutAttrBottom].nsrcattr != QLayoutNone)
		{
			rcposition.bottom = QViewGetLayoutPosition(view, parent, type, items, QLayoutBottom);
			if(nheight > 0)
			{
				// 高度优先。
				rcposition.bottom = rcposition.top+nheight;
				if(QViewHasLayoutFinished(view, QLayoutTop))
				{
					QViewSetLayoutFinished(view, QLayoutBottom, 1);
				}
			}
		}
		else
		{
			if(nheight > 0)
			{
				rcposition.bottom = rcposition.top+nheight;
				if(QViewHasLayoutFinished(view, QLayoutTop))
				{
					QViewSetLayoutFinished(view, QLayoutBottom, 1);
				}
			}
			else
			{
				rcposition.bottom = rcposition.top;
			}
		}
	}
	else
	{
		if(items[QLayoutAttrBottom].nsrcattr != QLayoutNone)
		{
			rcposition.bottom = QViewGetLayoutPosition(view, parent, type, items, QLayoutBottom);
			if(nheight > 0)
			{
				if(QViewHasLayoutFinished(view, QLayoutBottom))
				{
					rcposition.top = rcposition.bottom-nheight;
					QViewSetLayoutFinished(view, QLayoutTop, 1);
				}
				else
				{
					rcposition.top = 0;
					rcposition.bottom = nheight;
				}
			}
			else
			{
				rcposition.top = rcposition.bottom;
			}
		}
		else
		{
			rcposition.top = 0;
			if(nheight != 0)
			{
				rcposition.bottom = rcposition.top+nheight;
			}
			else
			{
				rcposition.bottom = rcposition.top;
			}
		}
	}
	// 保存初步位置。
	memcpy(&view->mstlayout.rcposition, &rcposition, sizeof(QRECT));
	
	// 保存空隙位置。
	view->mstlayout.rcmargin.left = QViewGetLayoutMargin(type, items, QLayoutAttrLeft);
	view->mstlayout.rcmargin.top = QViewGetLayoutMargin(type, items, QLayoutAttrTop);
	view->mstlayout.rcmargin.right = QViewGetLayoutMargin(type, items, QLayoutAttrRight);
	view->mstlayout.rcmargin.bottom = QViewGetLayoutMargin(type, items, QLayoutAttrBottom);
	
	// 保存对齐标志。
	nflag = QViewGetLayoutFlag(view, QLayoutAttrAlign);
	switch(type)
	{
		case QLayoutAppendNone:
			// 非append类型只有居中对齐需要后续处理，其他对齐直接用相对位置做处理。
			if(items[QLayoutAttrLeft].ndstattr == QLayoutCenter || items[QLayoutAttrRight].ndstattr == QLayoutCenter)
			{
				nflag |= QLayoutAlignHCenter;
			}
			else
			{
				nflag &= ~QLayoutAlignHCenter;
			}
			if(items[QLayoutAttrTop].ndstattr == QLayoutCenter || items[QLayoutAttrBottom].ndstattr == QLayoutCenter)
			{
				nflag |= QLayoutAlignVCenter;
			}
			else
			{
				nflag &= ~QLayoutAlignVCenter;
			}
			break;
			
		case QLayoutAppendL:
		case QLayoutAppendR:
			// 横向方向不定长。
		case QLayoutAppendLT:
		case QLayoutAppendRT:
			// 向下方向不定长。
		case QLayoutAppendLB:
		case QLayoutAppendRB:
			// 向上方向不定长。
			nflag &= ~QLayoutAlignHCenter;
			if(items[QLayoutAttrTop].ndstattr == QLayoutCenter || items[QLayoutAttrBottom].ndstattr == QLayoutCenter)
			{
				// 横向居中对齐。
				nflag |= QLayoutAlignVCenter;
			}
			else
			{
				nflag &= ~QLayoutAlignVCenter;
				if(items[QLayoutAttrTop].ndstattr == QLayoutTop || items[QLayoutAttrBottom].ndstattr == QLayoutTop)
				{
					// 靠上对齐。
					nflag |= QLayoutAlignTop;
				}
				else
				{
					nflag &= ~QLayoutAlignTop;
				}
				if(items[QLayoutAttrTop].ndstattr == QLayoutBottom || items[QLayoutAttrBottom].ndstattr == QLayoutBottom)
				{
					// 靠下对齐。
					nflag |= QLayoutAlignBottom;
				}
				else
				{
					nflag &= ~QLayoutAlignBottom;
				}
				if(items[QLayoutAttrLeft].ndstattr == QLayoutLeft || items[QLayoutAttrRight].ndstattr == QLayoutLeft)
				{
					// 靠左对齐。
					nflag |= QLayoutAlignLeft;
				}
				else
				{
					nflag &= ~QLayoutAlignLeft;
				}
				if(items[QLayoutAttrLeft].ndstattr == QLayoutRight || items[QLayoutAttrRight].ndstattr == QLayoutRight)
				{
					// 靠右对齐。
					nflag |= QLayoutAlignRight;
				}
				else
				{
					nflag &= ~QLayoutAlignRight;
				}
			}
			break;
			
		case QLayoutAppendT:
		case QLayoutAppendB:
			// 纵向方向不定长。
		case QLayoutAppendTL:
		case QLayoutAppendBL:
			// 向右方向不定长。
		case QLayoutAppendTR:
		case QLayoutAppendBR:
			// 向左方向不定长。
			if(items[QLayoutAttrLeft].ndstattr == QLayoutCenter || items[QLayoutAttrRight].ndstattr == QLayoutCenter)
			{
				// 横向居中对齐。
				nflag |= QLayoutAlignHCenter;
			}
			else
			{
				nflag &= ~QLayoutAlignHCenter;
				if(items[QLayoutAttrLeft].ndstattr == QLayoutLeft || items[QLayoutAttrRight].ndstattr == QLayoutLeft)
				{
					// 靠左对齐。
					nflag |= QLayoutAlignLeft;
				}
				else
				{
					nflag &= ~QLayoutAlignLeft;
				}
				if(items[QLayoutAttrLeft].ndstattr == QLayoutRight || items[QLayoutAttrRight].ndstattr == QLayoutRight)
				{
					// 靠右对齐。
					nflag |= QLayoutAlignRight;
				}
				else
				{
					nflag &= ~QLayoutAlignRight;
				}
				if(items[QLayoutAttrTop].ndstattr == QLayoutTop || items[QLayoutAttrBottom].ndstattr == QLayoutTop)
				{
					// 靠上对齐。
					nflag |= QLayoutAlignTop;
				}
				else
				{
					nflag &= ~QLayoutAlignTop;
				}
				if(items[QLayoutAttrTop].ndstattr == QLayoutBottom || items[QLayoutAttrBottom].ndstattr == QLayoutBottom)
				{
					// 靠下对齐。
					nflag |= QLayoutAlignBottom;
				}
				else
				{
					nflag &= ~QLayoutAlignBottom;
				}
			}
			nflag &= ~QLayoutAlignVCenter;
			break;
	}
	QViewSetLayoutFlag(view, QLayoutAttrAlign, nflag);
	
	return QNO_OK;
}

static QINT QViewExecLayoutAppendLinear(QBaseView *view, QINT type, struct qmdl_layout_item items[])
{
	QINT nwidth, nheight;
	QBaseView *pparent, *pprev;
	
	if(view == NULL)
	{
		return QNO_FAIL;
	}
	QViewGetParentAndPrevView(view, &pparent, &pprev);
	QViewGetBasePositionMargin(view, pparent, type, items);
	if(pparent == NULL)
	{
		return QNO_OK;
	}
	if(pprev == NULL)
	{
		switch(type)
		{
			case QLayoutAppendL:
			case QLayoutAppendT:
				view->mstlayout.rcposition.left		+= QREALL(pparent);
				view->mstlayout.rcposition.top		+= QREALT(pparent);
				view->mstlayout.rcposition.right	+= QREALL(pparent);
				view->mstlayout.rcposition.bottom	+= QREALT(pparent);
				break;

			case QLayoutAppendR:
				nwidth = QRECTW(view->mstlayout.rcposition);
				view->mstlayout.rcposition.left		+= QREALR(pparent)-nwidth;
				view->mstlayout.rcposition.top		+= QREALT(pparent);
				view->mstlayout.rcposition.right	+= QREALR(pparent)-nwidth;
				view->mstlayout.rcposition.bottom	+= QREALT(pparent);
				break;
				
			case QLayoutAppendB:
				nheight = QRECTH(view->mstlayout.rcposition);
				view->mstlayout.rcposition.left		+= QREALL(pparent);
				view->mstlayout.rcposition.top		+= QREALB(pparent)-nheight;
				view->mstlayout.rcposition.right	+= QREALL(pparent);
				view->mstlayout.rcposition.bottom	+= QREALB(pparent)-nheight;
				break;
		}
	}
	else
	{
		switch(type)
		{
			case QLayoutAppendL:
				view->mstlayout.rcposition.left		+= pprev->mstlayout.rcposition.right;
				view->mstlayout.rcposition.top		+= QREALT(pparent);
				view->mstlayout.rcposition.right	+= pprev->mstlayout.rcposition.right;
				view->mstlayout.rcposition.bottom	+= QREALT(pparent);
				break;
				
			case QLayoutAppendT:
				view->mstlayout.rcposition.left		+= QREALL(pparent);
				view->mstlayout.rcposition.top		+= pprev->mstlayout.rcposition.bottom;
				view->mstlayout.rcposition.right	+= QREALL(pparent);
				view->mstlayout.rcposition.bottom	+= pprev->mstlayout.rcposition.bottom;
				break;
				
			case QLayoutAppendR:
				nwidth = QRECTW(view->mstlayout.rcposition);
				view->mstlayout.rcposition.left		+= pprev->mstlayout.rcposition.left-nwidth;
				view->mstlayout.rcposition.top		+= QREALT(pparent);
				view->mstlayout.rcposition.right	+= pprev->mstlayout.rcposition.left-nwidth;
				view->mstlayout.rcposition.bottom	+= QREALT(pparent);
				break;
				
			case QLayoutAppendB:
				nheight = QRECTH(view->mstlayout.rcposition);
				view->mstlayout.rcposition.left		+= QREALL(pparent);
				view->mstlayout.rcposition.top		+= pprev->mstlayout.rcposition.top-nheight;
				view->mstlayout.rcposition.right	+= QREALL(pparent);
				view->mstlayout.rcposition.bottom	+= pprev->mstlayout.rcposition.top-nheight;
				break;
		}
	}
	
	return QNO_OK;
}

static QINT QViewExecLayoutAppendPage(QBaseView *view, QINT type, struct qmdl_layout_item items[])
{
	QBaseView *pparent, *pprev;
	QINT nflag, nnewline, nwidth, nheight, npeak;
	
	if(view == NULL)
	{
		return QNO_FAIL;
	}
	// 获取父亲view和前一兄弟view。
	QViewGetParentAndPrevView(view, &pparent, &pprev);
	// 执行基本的信息处理。
	QViewGetBasePositionMargin(view, pparent, type, items);
	if(pparent == NULL)
	{
		return QNO_OK;
	}
	if(pprev == NULL)
	{
		switch(type)
		{
			case QLayoutAppendLT:
			case QLayoutAppendTL:
				view->mstlayout.rcposition.left		+= QREALL(pparent);
				view->mstlayout.rcposition.top		+= QREALT(pparent);
				view->mstlayout.rcposition.right	+= QREALL(pparent);
				view->mstlayout.rcposition.bottom	+= QREALT(pparent);
				break;
				
			case QLayoutAppendTR:
			case QLayoutAppendRT:
				nwidth = QRECTW(view->mstlayout.rcposition);
				view->mstlayout.rcposition.left		+= QREALR(pparent)-nwidth;
				view->mstlayout.rcposition.top		+= QREALT(pparent);
				view->mstlayout.rcposition.right	+= QREALR(pparent)-nwidth;
				view->mstlayout.rcposition.bottom	+= QREALT(pparent);
				break;
				
			case QLayoutAppendRB:
			case QLayoutAppendBR:
				nwidth = QRECTW(view->mstlayout.rcposition);
				nheight = QRECTH(view->mstlayout.rcposition);
				view->mstlayout.rcposition.left		+= QREALR(pparent)-nwidth;
				view->mstlayout.rcposition.top		+= QREALB(pparent)-nheight;
				view->mstlayout.rcposition.right	+= QREALR(pparent)-nwidth;
				view->mstlayout.rcposition.bottom	+= QREALB(pparent)-nheight;
				break;
				
			case QLayoutAppendBL:
			case QLayoutAppendLB:
				nheight = QRECTH(view->mstlayout.rcposition);
				view->mstlayout.rcposition.left		+= QREALL(pparent);
				view->mstlayout.rcposition.top		+= QREALB(pparent)-nheight;
				view->mstlayout.rcposition.right	+= QREALL(pparent);
				view->mstlayout.rcposition.bottom	+= QREALB(pparent)-nheight;
				break;
		}
	}
	else
	{
		switch(type)
		{
			case QLayoutAppendLT:
				nnewline = 0;
				nflag = QViewGetLayoutFlag(pparent, QLayoutAttrWidth);
				if( (nflag == 0) || (nflag & (QLayoutFlagLess|QLayoutFlagEqual)) )
				{
					// 限制最大宽度。
					if( (QREALW(pparent) > 0) &&
					    (pprev->mstlayout.rcposition.right+QRECTW(view->mstlayout.rcposition) > QREALR(pparent))
					   )
					{
						nnewline = 1;
					}
				}
				if(nnewline == 0)
				{
					view->mstlayout.rcposition.left		+= pprev->mstlayout.rcposition.right;
					view->mstlayout.rcposition.top		+= pprev->mstlayout.rcposition.top;
					view->mstlayout.rcposition.right	+= pprev->mstlayout.rcposition.right;
					view->mstlayout.rcposition.bottom	+= pprev->mstlayout.rcposition.top;
				}
				else
				{
					npeak = QViewGetPeakValue(pprev, QLayoutAttrBottom);
					view->mstlayout.rcposition.left		+= QREALL(pparent);
					view->mstlayout.rcposition.top		+= npeak;
					view->mstlayout.rcposition.right	+= QREALL(pparent);
					view->mstlayout.rcposition.bottom	+= npeak;
				}
				break;
				
			case QLayoutAppendLB:
				nnewline = 0;
				nflag = QViewGetLayoutFlag(pparent, QLayoutAttrWidth);
				if( (nflag == 0) || (nflag & (QLayoutFlagLess|QLayoutFlagEqual)) )
				{
					// 限制最大宽度。
					if( (QREALW(pparent) > 0) &&
					    (pprev->mstlayout.rcposition.right+QRECTW(view->mstlayout.rcposition) > QREALR(pparent))
					   )
					{
						nnewline = 1;
					}
				}
				nheight = QRECTH(view->mstlayout.rcposition);
				if(nnewline == 0)
				{
					view->mstlayout.rcposition.left		+= pprev->mstlayout.rcposition.right;
					view->mstlayout.rcposition.top		+= pprev->mstlayout.rcposition.bottom-nheight;
					view->mstlayout.rcposition.right	+= pprev->mstlayout.rcposition.right;
					view->mstlayout.rcposition.bottom	+= pprev->mstlayout.rcposition.bottom-nheight;
				}
				else
				{
					npeak = QViewGetPeakValue(pprev, QLayoutAttrTop);
					view->mstlayout.rcposition.left		+= QREALL(pparent);
					view->mstlayout.rcposition.top		+= npeak-nheight;
					view->mstlayout.rcposition.right	+= QREALL(pparent);
					view->mstlayout.rcposition.bottom	+= npeak-nheight;
				}
				break;
				
			case QLayoutAppendRT:
				nnewline = 0;
				nflag = QViewGetLayoutFlag(pparent, QLayoutAttrWidth);
				if( (nflag == 0) || (nflag & (QLayoutFlagLess|QLayoutFlagEqual)) )
				{
					// 限制最大宽度。
					if( (QREALW(pparent) > 0) &&
					    (pprev->mstlayout.rcposition.left-QRECTW(view->mstlayout.rcposition) < QREALL(pparent))
					   )
					{
						nnewline = 1;
					}
				}
				if(nnewline == 0)
				{
					nwidth = QREALW(pparent);
					view->mstlayout.rcposition.left		+= pprev->mstlayout.rcposition.left-nwidth;
					view->mstlayout.rcposition.top		+= pprev->mstlayout.rcposition.top;
					view->mstlayout.rcposition.right	+= pprev->mstlayout.rcposition.left-nwidth;
					view->mstlayout.rcposition.bottom	+= pprev->mstlayout.rcposition.top;
				}
				else
				{
					npeak = QViewGetPeakValue(pprev, QLayoutAttrBottom);
					view->mstlayout.rcposition.left		+= QREALR(pparent);
					view->mstlayout.rcposition.top		+= npeak;
					view->mstlayout.rcposition.right	+= QREALR(pparent);
					view->mstlayout.rcposition.bottom	+= npeak;
				}
				break;
				
			case QLayoutAppendRB:
				nnewline = 0;
				nflag = QViewGetLayoutFlag(pparent, QLayoutAttrWidth);
				if( (nflag == 0) || (nflag & (QLayoutFlagLess|QLayoutFlagEqual)) )
				{
					// 限制最大宽度。
					if( (QREALW(pparent) > 0) &&
					    (pprev->mstlayout.rcposition.left-QRECTW(view->mstlayout.rcposition) < QREALL(pparent))
					   )
					{
						nnewline = 1;
					}
				}
				nheight = QRECTH(view->mstlayout.rcposition);
				if(nnewline == 0)
				{
					nwidth = QREALW(pparent);
					view->mstlayout.rcposition.left		+= pprev->mstlayout.rcposition.left-nwidth;
					view->mstlayout.rcposition.top		+= pprev->mstlayout.rcposition.bottom-nheight;
					view->mstlayout.rcposition.right	+= pprev->mstlayout.rcposition.left-nwidth;
					view->mstlayout.rcposition.bottom	+= pprev->mstlayout.rcposition.bottom-nheight;
				}
				else
				{
					npeak = QViewGetPeakValue(pprev, QLayoutAttrTop);
					view->mstlayout.rcposition.left		+= QREALR(pparent);
					view->mstlayout.rcposition.top		+= npeak-nheight;
					view->mstlayout.rcposition.right	+= QREALR(pparent);
					view->mstlayout.rcposition.bottom	+= npeak-nheight;
				}
				break;
				
			case QLayoutAppendTL:
				nnewline = 0;
				nflag = QViewGetLayoutFlag(pparent, QLayoutAttrHeight);
				if( (nflag == 0) || (nflag & (QLayoutFlagLess|QLayoutFlagEqual)) )
				{
					// 限制最大高度。
					if( (QREALH(pparent) > 0) &&
					    (pprev->mstlayout.rcposition.top+QRECTH(view->mstlayout.rcposition) > QREALB(pparent))
					   )
					{
						nnewline = 1;
					}
				}
				if(nnewline == 0)
				{
					view->mstlayout.rcposition.left		+= pprev->mstlayout.rcposition.left;
					view->mstlayout.rcposition.top		+= pprev->mstlayout.rcposition.bottom;
					view->mstlayout.rcposition.right	+= pprev->mstlayout.rcposition.left;
					view->mstlayout.rcposition.bottom	+= pprev->mstlayout.rcposition.bottom;
				}
				else
				{
					npeak = QViewGetPeakValue(pprev, QLayoutAttrRight);
					view->mstlayout.rcposition.left		+= npeak;
					view->mstlayout.rcposition.top		+= QREALT(pparent);
					view->mstlayout.rcposition.right	+= npeak;
					view->mstlayout.rcposition.bottom	+= QREALT(pparent);
				}
				break;
				
			case QLayoutAppendTR:
				nnewline = 0;
				nflag = QViewGetLayoutFlag(pparent, QLayoutAttrHeight);
				if( (nflag == 0) || (nflag & (QLayoutFlagLess|QLayoutFlagEqual)) )
				{
					// 限制最大高度。
					if( (QREALH(pparent) > 0) &&
					    (pprev->mstlayout.rcposition.top+QRECTH(view->mstlayout.rcposition) > QREALB(pparent))
					   )
					{
						nnewline = 1;
					}
				}
				nwidth = QRECTW(view->mstlayout.rcposition);
				if(nnewline == 0)
				{
					view->mstlayout.rcposition.left		+= pprev->mstlayout.rcposition.right-nwidth;
					view->mstlayout.rcposition.top		+= pprev->mstlayout.rcposition.bottom;
					view->mstlayout.rcposition.right	+= pprev->mstlayout.rcposition.right-nwidth;
					view->mstlayout.rcposition.bottom	+= pprev->mstlayout.rcposition.bottom;
				}
				else
				{
					npeak = QViewGetPeakValue(pprev, QLayoutAttrLeft);
					view->mstlayout.rcposition.left		+= npeak-nwidth;
					view->mstlayout.rcposition.top		+= QREALT(pparent);
					view->mstlayout.rcposition.right	+= npeak-nwidth;
					view->mstlayout.rcposition.bottom	+= QREALT(pparent);
				}
				break;
				
			case QLayoutAppendBL:
				nnewline = 0;
				nflag = QViewGetLayoutFlag(pparent, QLayoutAttrHeight);
				if( (nflag == 0) || (nflag & (QLayoutFlagLess|QLayoutFlagEqual)) )
				{
					// 限制最大高度。
					if( (QREALH(pparent) > 0) &&
					    (pprev->mstlayout.rcposition.bottom-QRECTH(view->mstlayout.rcposition) < QREALT(pparent))
					   )
					{
						nnewline = 1;
					}
				}
				nheight = QRECTH(view->mstlayout.rcposition);
				if(nnewline == 0)
				{
					view->mstlayout.rcposition.left		+= pprev->mstlayout.rcposition.left;
					view->mstlayout.rcposition.top		+= pprev->mstlayout.rcposition.top-nheight;
					view->mstlayout.rcposition.right	+= pprev->mstlayout.rcposition.left;
					view->mstlayout.rcposition.bottom	+= pprev->mstlayout.rcposition.top-nheight;
				}
				else
				{
					npeak = QViewGetPeakValue(pprev, QLayoutAttrRight);
					view->mstlayout.rcposition.left		+= npeak;
					view->mstlayout.rcposition.top		+= QREALB(pparent)-nheight;
					view->mstlayout.rcposition.right	+= npeak;
					view->mstlayout.rcposition.bottom	+= QREALB(pparent)-nheight;
				}
				break;
				
			case QLayoutAppendBR:
				nnewline = 0;
				nflag = QViewGetLayoutFlag(pparent, QLayoutAttrHeight);
				if( (nflag == 0) || (nflag & (QLayoutFlagLess|QLayoutFlagEqual)) )
				{
					// 限制最大高度。
					if( (QREALH(pparent) > 0) &&
					    (pprev->mstlayout.rcposition.bottom-QRECTH(view->mstlayout.rcposition) < QREALT(pparent))
					   )
					{
						nnewline = 1;
					}
				}
				nwidth = QRECTW(view->mstlayout.rcposition);
				nheight = QRECTH(view->mstlayout.rcposition);
				if(nnewline == 0)
				{
					view->mstlayout.rcposition.left		+= pprev->mstlayout.rcposition.right-nwidth;
					view->mstlayout.rcposition.top		+= pprev->mstlayout.rcposition.top-nheight;
					view->mstlayout.rcposition.right	+= pprev->mstlayout.rcposition.right-nwidth;
					view->mstlayout.rcposition.bottom	+= pprev->mstlayout.rcposition.top-nheight;
				}
				else
				{
					npeak = QViewGetPeakValue(pprev, QLayoutAttrLeft);
					view->mstlayout.rcposition.left		+= npeak-nwidth;
					view->mstlayout.rcposition.top		+= QREALB(pparent)-nheight;
					view->mstlayout.rcposition.right	+= npeak-nwidth;
					view->mstlayout.rcposition.bottom	+= QREALB(pparent)-nheight;
				}
				break;
		}
	}
	
	return QNO_OK;
}

static QINT QViewExecLayoutPosition(QBaseView *view)
{
	QRECT rcview;
	QMDL pchildmdl;
	QBaseView *pchildview;
	QINT nwidth, nheight, nflag, nwflag, nhflag, nchildtype;
	
	if(view == NULL)
	{
		return QNO_OK;
	}
	pchildmdl = view->HeadModule();
	if(pchildmdl == NULL)
	{
		return QNO_OK;
	}
	nwidth = QRECTW(view->mstlayout.rcposition);
	nflag = QViewGetLayoutFlag(view, QLayoutAttrWidth);
	if(nwidth == 0 || (nflag&QLayoutFlagMore))
	{
		nwflag = 1;
	}
	else
	{
		nwflag = 0;
	}
	nheight = QRECTH(view->mstlayout.rcposition);
	nflag = QViewGetLayoutFlag(view, QLayoutAttrHeight);
	if(nwidth == 0 || (nflag&QLayoutFlagMore))
	{
		nhflag = 1;
	}
	else
	{
		nhflag = 0;
	}
	if(nwflag == 0 && nhflag == 0)
	{
		// 尺寸已定则返回。
		return QNO_OK;
	}
	memcpy(&rcview, &view->mstlayout.rcposition, sizeof(QRECT));
	while(pchildmdl)
	{
		pchildview = dynamic_cast<QBaseView *>(pchildmdl);
		if(pchildview != NULL)
		{
			if(pchildview->GetState(QStateVisual))
			{
				nchildtype = QViewGetLayoutFlag(pchildview, QLayoutAttrType);
				switch(nchildtype)
				{
					case QLayoutAppendNone:
						break;
						
					case QLayoutAppendL:
					case QLayoutAppendTL:
					case QLayoutAppendBL:
						// 向右方向不定长。
						if(nwflag != 0)
						{
							if(rcview.right < pchildview->mstlayout.rcposition.right)
							{
								rcview.right = pchildview->mstlayout.rcposition.right;
							}
						}
						break;
						
					case QLayoutAppendR:
					case QLayoutAppendTR:
					case QLayoutAppendBR:
						// 向左方向不定长。
						if(nwflag != 0)
						{
							if(rcview.left > pchildview->mstlayout.rcposition.left)
							{
								rcview.left = pchildview->mstlayout.rcposition.left;
							}
						}
						break;
						
					case QLayoutAppendT:
					case QLayoutAppendLT:
					case QLayoutAppendRT:
						// 向下方向不定长。
						if(nhflag != 0)
						{
							if(rcview.bottom < pchildview->mstlayout.rcposition.bottom)
							{
								rcview.bottom = pchildview->mstlayout.rcposition.bottom;
							}
						}
						break;
						
					case QLayoutAppendB:
					case QLayoutAppendLB:
					case QLayoutAppendRB:
						// 向上方向不定长。
						if(nhflag != 0)
						{
							if(rcview.bottom < pchildview->mstlayout.rcposition.bottom)
							{
								rcview.bottom = pchildview->mstlayout.rcposition.bottom;
							}
						}
						break;
				}
			}
		}
		pchildmdl = pchildmdl->NextModule();
	}
	memcpy(&view->mstlayout.rcposition, &rcview, sizeof(QRECT));

	return QNO_OK;
}

static QINT QViewExecLayoutAlign(QBaseView *view)
{
	QRECT rcpeak;
	QMDL pchildmdl, pnextmdl, psiblingmdl;
	QBaseView *pchildview, *pnextview, *pbeginview, *psiblingview;
	QINT nnewline, nrowalign, nchildalign, nchildtype, nchildflag, nxpos, nypos;
	
	if(view == NULL)
	{
		return QSCN_OK;
	}
	// 尺寸确定完成，进行子view的居中处理。
	nrowalign = 0;
	pbeginview = NULL;
	pchildview = NULL;
	pchildmdl = view->HeadModule();
	while(pchildmdl)
	{
		pchildview = dynamic_cast<QBaseView *>(pchildmdl);
		if(pchildview != NULL)
		{
			if(pchildview->GetState(QStateVisual))
			{
				break;
			}
		}
		pchildmdl = pchildmdl->NextModule();
		if(pchildmdl == NULL)
		{
			pchildview = NULL;
		}
	}
	while(pchildview)
	{
		pnextview = NULL;
		pnextmdl = pchildview->NextModule();
		while(pnextmdl)
		{
			pnextview = dynamic_cast<QBaseView *>(pnextmdl);
			if(pnextview != NULL)
			{
				if(pnextview->GetState(QStateVisual))
				{
					break;
				}
			}
			pnextmdl = pnextmdl->NextModule();
		}
		if(pbeginview == NULL)
		{
			// 记录第一个极值范围。
			pbeginview = pchildview;
			memcpy(&rcpeak, &pchildview->mstlayout.rcposition, sizeof(QRECT));
		}
		nchildalign = QViewGetLayoutFlag(pchildview, QLayoutAttrAlign);
		nchildtype = QViewGetLayoutFlag(pchildview, QLayoutAttrType);
		nchildflag = QViewGetLayoutFlag(pchildview, QLayoutAttrFlag);
		switch(nchildtype)
		{
			case QLayoutAppendNone:
				if(pbeginview != NULL)
				{
					// 更新极值范围。
					if(rcpeak.left > pchildview->mstlayout.rcposition.left)
					{
						rcpeak.left = pchildview->mstlayout.rcposition.left;
					}
					if(rcpeak.top > pchildview->mstlayout.rcposition.top)
					{
						rcpeak.top = pchildview->mstlayout.rcposition.top;
					}
					if(rcpeak.right < pchildview->mstlayout.rcposition.right)
					{
						rcpeak.right = pchildview->mstlayout.rcposition.right;
					}
					if(rcpeak.bottom < pchildview->mstlayout.rcposition.bottom)
					{
						rcpeak.bottom = pchildview->mstlayout.rcposition.bottom;
					}
				}
				if(nchildalign & (QLayoutAlignHCenter|QLayoutAlignVCenter))
				{
					if(nchildalign & QLayoutAlignHCenter)
					{
						nxpos = ((view->mstlayout.rcposition.left+view->mstlayout.rcposition.right)>>1)-(QRECTW(pchildview->mstlayout.rcposition)>>1);
					}
					else
					{
						nxpos = view->mstlayout.rcposition.left;
					}
					if(nchildalign & QLayoutAlignVCenter)
					{
						nypos = ((view->mstlayout.rcposition.top+view->mstlayout.rcposition.bottom)>>1)-(QRECTH(pchildview->mstlayout.rcposition)>>1);
					}
					else
					{
						nypos = view->mstlayout.rcposition.top;
					}
					pchildview->MovePosition(nxpos, nypos);
				}
				break;
				
			case QLayoutAppendL:
			case QLayoutAppendR:
				if(pbeginview != NULL)
				{
					// 更新极值范围。
					if(rcpeak.left > pchildview->mstlayout.rcposition.left)
					{
						rcpeak.left = pchildview->mstlayout.rcposition.left;
					}
					if(rcpeak.top > pchildview->mstlayout.rcposition.top)
					{
						rcpeak.top = pchildview->mstlayout.rcposition.top;
					}
					if(rcpeak.right < pchildview->mstlayout.rcposition.right)
					{
						rcpeak.right = pchildview->mstlayout.rcposition.right;
					}
					if(rcpeak.bottom < pchildview->mstlayout.rcposition.bottom)
					{
						rcpeak.bottom = pchildview->mstlayout.rcposition.bottom;
					}
				}
				if(nchildalign & QLayoutAlignVDirection)
				{
					if(nchildalign & QLayoutAlignVCenter)
					{
						nxpos = pchildview->mstlayout.rcposition.left;
						nypos = view->mstlayout.rcposition.top+((QRECTH(view->mstlayout.rcposition)-QRECTH(pchildview->mstlayout.rcposition))>>1);
						pchildview->MovePosition(nxpos, nypos);
					}
					else if( (nchildalign & QLayoutAlignTop) && (nchildalign & QLayoutAlignBottom) )
					{
						// 纵向拉伸。
						nxpos = pchildview->mstlayout.rcposition.left;
						nypos = view->mstlayout.rcposition.top;
						pchildview->MovePosition(nxpos, nypos);
						pchildview->mstlayout.rcposition.bottom = view->mstlayout.rcposition.bottom;
					}
					else if(nchildalign & QLayoutAlignTop)
					{
						nxpos = pchildview->mstlayout.rcposition.left;
						nypos = view->mstlayout.rcposition.top;
						pchildview->MovePosition(nxpos, nypos);
					}
					else if(nchildalign & QLayoutAlignBottom)
					{
						nxpos = pchildview->mstlayout.rcposition.left;
						nypos = view->mstlayout.rcposition.bottom-QRECTH(pchildview->mstlayout.rcposition);
						pchildview->MovePosition(nxpos, nypos);
					}
				}
				if(nchildalign & QLayoutAlignHDirection)
				{
					// 只有有一个子view做水平对齐，则整行都做同样的对齐。
					nrowalign |= (nchildalign & QLayoutAlignHDirection);
				}
				if(pnextview == NULL)
				{
					if(nrowalign & QLayoutAlignHDirection)
					{
						nxpos = 0;
						if(nrowalign & QLayoutAlignHCenter)
						{
							nxpos = view->mstlayout.rcposition.left+((QRECTW(view->mstlayout.rcposition)-QRECTW(rcpeak))>>1)-rcpeak.left;
						}
						else if(nrowalign & QLayoutAlignLeft)
						{
							nxpos = view->mstlayout.rcposition.left-rcpeak.left;
						}
						else if(nrowalign & QLayoutAlignRight)
						{
							nxpos = view->mstlayout.rcposition.right-rcpeak.right;
						}
						if(nxpos != 0)
						{
							psiblingmdl = pbeginview;
							while(psiblingmdl)
							{
								psiblingview = dynamic_cast<QBaseView *>(psiblingmdl);
								if(psiblingview != NULL)
								{
									if(psiblingview->GetState(QStateVisual))
									{
										psiblingview->MovePosition(psiblingview->mstlayout.rcposition.left+nxpos, psiblingview->mstlayout.rcposition.top);
									}
								}
								psiblingmdl = psiblingmdl->NextModule();
							}
						}
					}
				}
				break;
				
			case QLayoutAppendT:
			case QLayoutAppendB:
				if(pbeginview != NULL)
				{
					// 更新极值范围。
					if(rcpeak.left > pchildview->mstlayout.rcposition.left)
					{
						rcpeak.left = pchildview->mstlayout.rcposition.left;
					}
					if(rcpeak.top > pchildview->mstlayout.rcposition.top)
					{
						rcpeak.top = pchildview->mstlayout.rcposition.top;
					}
					if(rcpeak.right < pchildview->mstlayout.rcposition.right)
					{
						rcpeak.right = pchildview->mstlayout.rcposition.right;
					}
					if(rcpeak.bottom < pchildview->mstlayout.rcposition.bottom)
					{
						rcpeak.bottom = pchildview->mstlayout.rcposition.bottom;
					}
				}
				if(nchildalign & QLayoutAlignHDirection)
				{
					if(nchildalign & QLayoutAlignHCenter)
					{
						nxpos = view->mstlayout.rcposition.left+((QRECTW(view->mstlayout.rcposition)-QRECTW(pchildview->mstlayout.rcposition))>>1);
						nypos = pchildview->mstlayout.rcposition.top;
						pchildview->MovePosition(nxpos, nypos);
					}
					else if( (nchildalign & QLayoutAlignLeft) && (nchildalign & QLayoutAlignRight) )
					{
						// 横向拉伸。
						nxpos = view->mstlayout.rcposition.left;
						nypos = pchildview->mstlayout.rcposition.top;
						pchildview->MovePosition(nxpos, nypos);
						pchildview->mstlayout.rcposition.right = view->mstlayout.rcposition.right;
					}
					else if(nchildalign & QLayoutAlignLeft)
					{
						nxpos = view->mstlayout.rcposition.left;
						nypos = pchildview->mstlayout.rcposition.top;
						pchildview->MovePosition(nxpos, nypos);
					}
					else if(nchildalign & QLayoutAlignRight)
					{
						nxpos = view->mstlayout.rcposition.right-QRECTW(pchildview->mstlayout.rcposition);
						nypos = pchildview->mstlayout.rcposition.top;
						pchildview->MovePosition(nxpos, nypos);
					}
				}
				if(nchildalign & QLayoutAlignVDirection)
				{
					// 只有有一个子view做水平对齐，则整行都做同样的对齐。
					nrowalign |= (nchildalign & QLayoutAlignVDirection);
				}
				if(pnextview == NULL)
				{
					if(nrowalign & QLayoutAlignVDirection)
					{
						nypos = 0;
						if(nrowalign & QLayoutAlignVCenter)
						{
							nypos = view->mstlayout.rcposition.top+((QRECTH(view->mstlayout.rcposition)-QRECTH(rcpeak))>>1)-rcpeak.top;
						}
						else if(nrowalign & QLayoutAlignTop)
						{
							nypos = view->mstlayout.rcposition.top-rcpeak.top;
						}
						else if(nrowalign & QLayoutAlignBottom)
						{
							nypos = view->mstlayout.rcposition.bottom-rcpeak.bottom;
						}
						if(nypos != 0)
						{
							psiblingmdl = pbeginview;
							while(psiblingmdl)
							{
								psiblingview = dynamic_cast<QBaseView *>(psiblingmdl);
								if(psiblingview != NULL)
								{
									if(psiblingview->GetState(QStateVisual))
									{
										psiblingview->MovePosition(psiblingview->mstlayout.rcposition.left, psiblingview->mstlayout.rcposition.top+nypos);
									}
								}
								psiblingmdl = psiblingmdl->NextModule();
							}
						}
					}
				}
				break;
				
			case QLayoutAppendLT:
			case QLayoutAppendRT:
				// 向下方向不定长。
			case QLayoutAppendLB:
			case QLayoutAppendRB:
				// 向上方向不定长。
				if(nchildalign & QLayoutAlignHDirection)
				{
					// 只有有一个子view做水平对齐，则整行都做同样的对齐。
					nrowalign |= (nchildalign & QLayoutAlignHDirection);
				}
				// 判断是否换行。
				if(nchildflag & QLayoutFlagNewline)
				{
					nnewline = 1;
				}
				else
				{
					nnewline = 0;
					switch(nchildtype)
					{
						case QLayoutAppendLT:
							if(pchildview->mstlayout.rcposition.top >= rcpeak.bottom)
							{
								if(pchildview->mstlayout.rcposition.left <= rcpeak.left)
								{
									nnewline = 1;
								}
							}
							break;
							
						case QLayoutAppendRT:
							if(pchildview->mstlayout.rcposition.top >= rcpeak.bottom)
							{
								if(pchildview->mstlayout.rcposition.right >= rcpeak.right)
								{
									nnewline = 1;
								}
							}
							break;
							
						case QLayoutAppendLB:
							if(pchildview->mstlayout.rcposition.bottom <= rcpeak.top)
							{
								if(pchildview->mstlayout.rcposition.left <= rcpeak.left)
								{
									nnewline = 1;
								}
							}
							break;
							
						case QLayoutAppendRB:
							if(pchildview->mstlayout.rcposition.bottom <= rcpeak.top)
							{
								if(pchildview->mstlayout.rcposition.right >= rcpeak.right)
								{
									nnewline = 1;
								}
							}
							break;
					}
				}
				if(nnewline == 0)
				{
					if(pbeginview != NULL)
					{
						// 更新极值范围。
						if(rcpeak.left > pchildview->mstlayout.rcposition.left)
						{
							rcpeak.left = pchildview->mstlayout.rcposition.left;
						}
						if(rcpeak.top > pchildview->mstlayout.rcposition.top)
						{
							rcpeak.top = pchildview->mstlayout.rcposition.top;
						}
						if(rcpeak.right < pchildview->mstlayout.rcposition.right)
						{
							rcpeak.right = pchildview->mstlayout.rcposition.right;
						}
						if(rcpeak.bottom < pchildview->mstlayout.rcposition.bottom)
						{
							rcpeak.bottom = pchildview->mstlayout.rcposition.bottom;
						}
					}
					if(pnextview == NULL)
					{
						nnewline = 1;
					}
				}
				if(nnewline == 1)
				{
					// 换行处理。
					nxpos = 0;
					if(nrowalign & QLayoutAlignHCenter)
					{
						nxpos = view->mstlayout.rcposition.left+((QRECTW(view->mstlayout.rcposition)-QRECTW(rcpeak))>>1)-rcpeak.left;
					}
					else if(nrowalign & QLayoutAlignLeft)
					{
						nxpos = view->mstlayout.rcposition.left-rcpeak.left;
					}
					else if(nrowalign & QLayoutAlignRight)
					{
						nxpos = view->mstlayout.rcposition.right-rcpeak.right;
					}
					psiblingmdl = pbeginview;
					while(psiblingmdl)
					{
						psiblingview = dynamic_cast<QBaseView *>(psiblingmdl);
						if(psiblingview != NULL)
						{
							if(psiblingview->GetState(QStateVisual))
							{
								if(pnextview != NULL)
								{
									if(psiblingview == pchildview)
									{
										break;
									}
								}
								if(nchildalign & QLayoutAlignVDirection)
								{
									if(nchildalign & QLayoutAlignVCenter)
									{
										nypos = view->mstlayout.rcposition.top+((QRECTH(view->mstlayout.rcposition)-QRECTH(psiblingview->mstlayout.rcposition))>>1);
										psiblingview->MovePosition(psiblingview->mstlayout.rcposition.left+nxpos, nypos);
									}
									else if( (nchildalign & QLayoutAlignTop) && (nchildalign & QLayoutAlignBottom) )
									{
										// 纵向拉伸。
										nypos = view->mstlayout.rcposition.top;
										psiblingview->MovePosition(psiblingview->mstlayout.rcposition.left+nxpos, nypos);
										psiblingview->mstlayout.rcposition.bottom = view->mstlayout.rcposition.bottom;
									}
									else if(nchildalign & QLayoutAlignTop)
									{
										nypos = view->mstlayout.rcposition.top;
										psiblingview->MovePosition(psiblingview->mstlayout.rcposition.left+nxpos, nypos);
									}
									else if(nchildalign & QLayoutAlignBottom)
									{
										nypos = view->mstlayout.rcposition.bottom-QRECTH(psiblingview->mstlayout.rcposition);
										psiblingview->MovePosition(psiblingview->mstlayout.rcposition.left+nxpos, nypos);
									}
								}
							}
						}
						psiblingmdl = psiblingmdl->NextModule();
					}
					if(pnextview != NULL)
					{
						pbeginview = pnextview;
						memcpy(&rcpeak, &pchildview->mstlayout.rcposition, sizeof(QRECT));
						nrowalign = 0;
					}
				}
				break;
				
			case QLayoutAppendTL:
			case QLayoutAppendBL:
				// 向右方向不定长。
			case QLayoutAppendTR:
			case QLayoutAppendBR:
				// 向左方向不定长。
				if(nchildalign & QLayoutAlignVDirection)
				{
					// 只有有一个子view做水平对齐，则整行都做同样的对齐。
					nrowalign |= (nchildalign & QLayoutAlignVDirection);
				}
				// 判断是否换行。
				if(nchildflag & QLayoutFlagNewline)
				{
					nnewline = 1;
				}
				else
				{
					nnewline = 0;
					switch(nchildtype)
					{
						case QLayoutAppendTL:
							if(pchildview->mstlayout.rcposition.left >= rcpeak.right)
							{
								if(pchildview->mstlayout.rcposition.top <= rcpeak.top)
								{
									nnewline = 1;
								}
							}
							break;
							
						case QLayoutAppendBL:
							if(pchildview->mstlayout.rcposition.left >= rcpeak.right)
							{
								if(pchildview->mstlayout.rcposition.bottom >= rcpeak.bottom)
								{
									nnewline = 1;
								}
							}
							break;
							
						case QLayoutAppendTR:
							if(pchildview->mstlayout.rcposition.right <= rcpeak.left)
							{
								if(pchildview->mstlayout.rcposition.top <= rcpeak.top)
								{
									nnewline = 1;
								}
							}
							break;
							
						case QLayoutAppendBR:
							if(pchildview->mstlayout.rcposition.right <= rcpeak.left)
							{
								if(pchildview->mstlayout.rcposition.bottom >= rcpeak.bottom)
								{
									nnewline = 1;
								}
							}
							break;
					}
				}
				if(nnewline == 0)
				{
					if(pbeginview != NULL)
					{
						// 更新极值范围。
						if(rcpeak.left > pchildview->mstlayout.rcposition.left)
						{
							rcpeak.left = pchildview->mstlayout.rcposition.left;
						}
						if(rcpeak.top > pchildview->mstlayout.rcposition.top)
						{
							rcpeak.top = pchildview->mstlayout.rcposition.top;
						}
						if(rcpeak.right < pchildview->mstlayout.rcposition.right)
						{
							rcpeak.right = pchildview->mstlayout.rcposition.right;
						}
						if(rcpeak.bottom < pchildview->mstlayout.rcposition.bottom)
						{
							rcpeak.bottom = pchildview->mstlayout.rcposition.bottom;
						}
					}
					if(pnextview == NULL)
					{
						// 最后一个view。
						nnewline = 1;
					}
				}
				if(nnewline == 1)
				{
					// 换行处理。
					nypos = 0;
					if(nrowalign & QLayoutAlignVCenter)
					{
						nypos = view->mstlayout.rcposition.top+((QRECTH(view->mstlayout.rcposition)-QRECTH(rcpeak))>>1)-rcpeak.top;
					}
					else if(nrowalign & QLayoutAlignTop)
					{
						nypos = view->mstlayout.rcposition.top-rcpeak.top;
					}
					else if(nrowalign & QLayoutAlignBottom)
					{
						nypos = view->mstlayout.rcposition.bottom-rcpeak.bottom;
					}
					psiblingmdl = pbeginview;
					while(psiblingmdl)
					{
						psiblingview = dynamic_cast<QBaseView *>(psiblingmdl);
						if(psiblingview != NULL)
						{
							if(psiblingview->GetState(QStateVisual))
							{
								if(pnextview != NULL)
								{
									if(psiblingview == pchildview)
									{
										break;
									}
								}
								if(nchildalign & QLayoutAlignHDirection)
								{
									if(nchildalign & QLayoutAlignHCenter)
									{
										nxpos = view->mstlayout.rcposition.left+((QRECTW(view->mstlayout.rcposition)-QRECTW(psiblingview->mstlayout.rcposition))>>1);
										psiblingview->MovePosition(nxpos, psiblingview->mstlayout.rcposition.top+nypos);
									}
									else if( (nchildalign & QLayoutAlignLeft) && (nchildalign & QLayoutAlignRight) )
									{
										// 纵向拉伸。
										nxpos = view->mstlayout.rcposition.left;
										psiblingview->MovePosition(nxpos, psiblingview->mstlayout.rcposition.top+nypos);
										psiblingview->mstlayout.rcposition.right = view->mstlayout.rcposition.right;
									}
									else if(nchildalign & QLayoutAlignLeft)
									{
										nxpos = view->mstlayout.rcposition.left;
										psiblingview->MovePosition(nxpos, psiblingview->mstlayout.rcposition.top+nypos);
									}
									else if(nchildalign & QLayoutAlignRight)
									{
										nxpos = view->mstlayout.rcposition.right-QRECTW(psiblingview->mstlayout.rcposition);
										psiblingview->MovePosition(nxpos, psiblingview->mstlayout.rcposition.top+nypos);
									}
								}
							}
						}
						psiblingmdl = psiblingmdl->NextModule();
					}
					if(pnextview != NULL)
					{
						// 进入新一行处理。
						pbeginview = pnextview;
						memcpy(&rcpeak, &pchildview->mstlayout.rcposition, sizeof(QRECT));
						nrowalign = 0;
					}
				}
				break;
		}
		pchildview = pnextview;
	}
	
	return QSCN_OK;
}

// 遍历获取排版信息。
static QINT qmdl_layout_item_scan_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QINT ntype, *pcount;
	struct qmdl_layout_item *pitem, *pattrs, *pattr;
	
	pitem = (struct qmdl_layout_item *)qlinkData(QLINK_NONE, hdl);
	if(pitem == NULL)
	{
		return QSCN_OK;
	}
	ntype = (QINT)params[1];
	pattrs = (struct qmdl_layout_item *)params[2];
	if(pattrs == NULL)
	{
		return QSCN_END;
	}
	pattr = &pattrs[pitem->nsrcattr-QLayoutWidth];
	if(ntype == QLayoutAppendNone)
	{
		memcpy(pattr, pitem, sizeof(struct qmdl_layout_item));
	}
	else
	{
		if(pitem->ndstattr != QLayoutAppend)
		{
			pattr->ndstattr = pitem->ndstattr;
		}
		pattr->nmultiplier = pitem->nmultiplier;
		pattr->nremainder = pitem->nremainder;
	}
	if(pattr->nflag == 0)
	{
		pattr->nflag = QLayoutFlagEqual;
	}
	pattr->pdstview = dynamic_cast<QBaseView *>(pitem->pdstview);
	if(pitem->ndstattr == QLayoutNone)
	{
		// 默认和源属性相同。
		pattr->ndstattr = pitem->nsrcattr;
	}
	pcount = (QINT *)params[3];
	if(pcount != NULL)
	{
		*pcount += 1;
	}
	
	return QSCN_OK;
}

static QINT qmdl_layout_position_prev_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QMDL pparentmdl;
	QINT nflag, ntype, ncount;
	QBaseView *pview, *pparentview;
	struct qmdl_layout_item vitems[8] = {0};
	
	pview = dynamic_cast<QBaseView *>((QMDL)hdl);
	if(pview == NULL)
	{
		return QSCN_OK;
	}
	nflag = QViewGetLayoutFlag(pview, QLayoutAttrFlag);
	if( !(nflag & QLayoutFlagSelf) )
	{
		// 获取父亲view的layout信息。
		pparentview = NULL;
		pparentmdl = pview->ParentModule();
		while(pparentmdl)
		{
			pparentview = dynamic_cast<QBaseView *>(pparentmdl);
			if(pparentview != NULL)
			{
				break;
			}
			pparentmdl = pparentmdl->ParentModule();
		}
		ncount = 0;
		ntype = QLayoutAppendNone;
		if(pparentview != NULL)
		{
			if(pparentview->mstlayout.hchildren != NULL)
			{
				qlinkScan(QLINK_NONE, pparentview->mstlayout.hchildren, qmdl_layout_item_scan_cb, 4, pview, ntype, vitems, &ncount);
			}
			ntype = QViewGetLayoutType(vitems);
		}
		// 获取自身的position信息。
		if(pview->mstlayout.hposition != NULL)
		{
			qlinkScan(QLINK_NONE, pview->mstlayout.hposition, qmdl_layout_item_scan_cb, 4, pview, ntype, vitems, &ncount);
		}
		if(ncount > 0)
		{
			ntype = QViewGetLayoutType(vitems);
			switch(ntype)
			{
				case QLayoutAppendNone:
					// 指定位置与相对位置。
					QViewGetBasePositionMargin(pview, pparentview, ntype, vitems);
					break;
					
				case QLayoutAppendL:
				case QLayoutAppendT:
				case QLayoutAppendR:
				case QLayoutAppendB:
					// 线性排版。
					QViewExecLayoutAppendLinear(pview, ntype, vitems);
					break;
					
				case QLayoutAppendLT:
				case QLayoutAppendTR:
				case QLayoutAppendRB:
				case QLayoutAppendBL:
				case QLayoutAppendTL:
				case QLayoutAppendRT:
				case QLayoutAppendBR:
				case QLayoutAppendLB:
					// 页面排版。
					QViewExecLayoutAppendPage(pview, ntype, vitems);
					break;
			}
			// 保存排版类型。
			QViewSetLayoutFlag(pview, QLayoutAttrType, ntype);
		}
	}
	if(nflag & QLayoutFlagChildren)
	{
		return QSCN_HORZ;
	}
	
	return QSCN_OK;
}

static QINT qmdl_layout_position_post_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QINT nflag;
	QBaseView *pview;
	
	pview = dynamic_cast<QBaseView *>((QMDL)hdl);
	if(pview == NULL)
	{
		return QSCN_OK;
	}
	nflag = QViewGetLayoutFlag(pview, QLayoutAttrFlag);
	if( !(nflag & QLayoutFlagChildren) )
	{
		// 确定不定长尺寸为定长。
		QViewExecLayoutPosition(pview);
		// 尺寸确定完成，进行子view的居中处理。
		QViewExecLayoutAlign(pview);
	}
	nflag |= (QLayoutFlagSelf|QLayoutFlagChildren);
	QViewSetLayoutFlag(pview, QLayoutAttrFlag, nflag);
	
	return QSCN_OK;
}

static QINT qmdl_update_position_prev_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QRECT rcview;
	QBaseView *pview;
	
	pview = dynamic_cast<QBaseView *>((QMDL)hdl);
	if(pview == NULL)
	{
		return QSCN_OK;
	}
	memcpy(&rcview, &pview->mstlayout.rcposition, sizeof(QRECT));
	rcview.left += pview->mstlayout.rcmargin.left;
	rcview.top += pview->mstlayout.rcmargin.top;
	rcview.right -= pview->mstlayout.rcmargin.right;
	rcview.bottom -= pview->mstlayout.rcmargin.bottom;
	pview->SetPosition(&rcview);
	
	return QSCN_OK;
}

static QINT qmdl_relation_position_prev_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QBaseView *pview;
	
	pview = dynamic_cast<QBaseView *>((QMDL)hdl);
	if(pview == NULL)
	{
		return QSCN_OK;
	}
	QViewFinishLayoutRelation(pview);
	
	return QSCN_OK;
}

static QINT qmdl_invalidate_position_prev_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QHDL hitem;
	QINT nflag;
	QBaseView *pview;
	struct qmdl_layout_item *pitem;
	
	pview = dynamic_cast<QBaseView *>((QMDL)hdl);
	if(pview == NULL)
	{
		return QSCN_OK;
	}
	nflag = QViewGetLayoutFlag(pview, QLayoutAttrFlag);
	nflag &= ~QLayoutFlagSelf;
	QViewSetLayoutFlag(pview, QLayoutAttrFlag, nflag);
	hitem = qlinkHead(QLINK_BL_RELATION, pview->mstlayout.hrelation);
	while(hitem)
	{
		pitem = (struct qmdl_layout_item *)qlinkData(QLINK_BL_RELATION, hitem);
		if(pitem != NULL)
		{
			pitem->nflag &= ~QLayoutFlagFinished;
		}
		hitem = qlinkNext(QLINK_BL_RELATION, hitem);
	}
	
	return QSCN_OK;
}

void QBaseView::LayoutPosition()
{
	this->ScanModulex(this, qmdl_layout_position_prev_cb, qmdl_layout_position_post_cb, NULL, 0);
	this->ScanModulex(this, qmdl_relation_position_prev_cb, NULL, NULL, 0);
}

void QBaseView::UpdatePosition()
{
	this->ScanModulex(this, qmdl_update_position_prev_cb, NULL, NULL, 0);
}

void QBaseView::InvalidatePosition()
{
	this->ScanModulex(this, qmdl_invalidate_position_prev_cb, NULL, NULL, 0);
}
