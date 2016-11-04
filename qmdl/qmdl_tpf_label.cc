
#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#include "qmdl_ios.h"


// -- OC类定义实现

@interface QIOSUILabel : UILabel
{
	QMDL mpmodule;
}

@property(nonatomic, assign) QMDL module;

- (id)initWithModule:(QMDL)module;

@end

@implementation QIOSUILabel

@synthesize module = mpmodule;

- (id)initWithModule:(QMDL)module
{
	self = [super init];
	if(self == nil)
	{
		return nil;
	}
	self.module = module;
	
	return self;
}

@end


// -- C++类实现

qstr_enum_begin(QUILabelStyleMap)
	qstr_enum_item( "default",			QUILabelStyleCenter )
	qstr_enum_item( "top",				QUILabelStyleTop )
	qstr_enum_item( "bottom",			QUILabelStyleBottom )
	qstr_enum_item( "left",				QUILabelStyleLeft )
	qstr_enum_item( "right",			QUILabelStyleRight )
	qstr_enum_item( "vcenter",			QUILabelStyleVCenter )
	qstr_enum_item( "hcenter",			QUILabelStyleHCenter )
	qstr_enum_item( "center",			QUILabelStyleCenter )

	qstr_enum_item( "wordwrap",			QUILabelStyleWordWrap )
	qstr_enum_item( "charwrap",			QUILabelStyleCharWrap )
	qstr_enum_item( "clip",				QUILabelStyleClip )
	qstr_enum_item( "headtrunc",		QUILabelStyleHeadTrunc )
	qstr_enum_item( "tailtrunc",		QUILabelStyleTailTrunc )
	qstr_enum_item( "middletrunc",		QUILabelStyleMiddleTrunc )
qstr_enum_end


QUILabel::QUILabel()
{
}

QUILabel::~QUILabel()
{
}

QINT QUILabel::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	QSTR pid;
	QINT ntag;
	
	if(midview != nil)
	{
		return QSCN_OK;
	}
	ntag = 0;
	pid = (QSTR)qxmlGetId(mxml);
	if(pid != NULL && qstrcmp(QSTR_CMP_ICASE, (QPNT)pid, (QPNT)qmdl_name(QUILabel), 0))
	{
		midview = FindViewByTag(env, parent, NULL, mxml, url, &ntag);
		if(midview == nil)
		{
			midview = [[QIOSUILabel alloc] initWithModule:this];
			if(ntag != 0)
			{
				((UIView *)midview).tag = ntag;
			}
		}
		
	}
	
	return QSCN_OK;
}

QINT QUILabelInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nstyle, nmode, nalign, nadjust;
	QUILabel *pview;
	
	pview = (QUILabel *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
	if(pview->midview == nil)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	ncnt = nlen;
	nstyle = qstr2enum(QUILabelStyleMap, 1, (QPNT )pval, &ncnt);
	if(ncnt <= 0)
	{
		return QSCN_OK;
	}
	nmode = 0;
	if(nstyle & QUILabelStyleWordWrap)
	{
		nmode |= UILineBreakModeWordWrap;
	}
	if(nstyle & QUILabelStyleCharWrap)
	{
		nmode |= UILineBreakModeCharacterWrap;
	}
	if(nstyle & QUILabelStyleClip)
	{
		nmode |= UILineBreakModeClip;
	}
	if(nstyle & QUILabelStyleHeadTrunc)
	{
		nmode |= UILineBreakModeHeadTruncation;
	}
	if(nstyle & QUILabelStyleTailTrunc)
	{
		nmode |= UILineBreakModeTailTruncation;
	}
	if(nstyle & QUILabelStyleMiddleTrunc)
	{
		nmode |= UILineBreakModeMiddleTruncation;
	}
	if(nmode != 0)
	{
		((QIOSUILabel *)pview->midview).lineBreakMode = (NSLineBreakMode )nmode;
	}
	nalign = 0;
	if(nstyle & QUILabelStyleLeft)
	{
		nalign |= NSTextAlignmentLeft;
	}
	if(nstyle & QUILabelStyleRight)
	{
		nalign |= NSTextAlignmentRight;
	}
	if(nstyle & QUILabelStyleHCenter)
	{
		nalign |= NSTextAlignmentCenter;
	}
	if(nalign != 0)
	{
		((QIOSUILabel *)pview->midview).textAlignment = (NSTextAlignment )nalign;
	}
	nadjust = 0;
	if(nstyle & QUILabelStyleTop)
	{
		nalign |= UIBaselineAdjustmentNone;
	}
	if(nstyle & QUILabelStyleBottom)
	{
		nalign |= UIBaselineAdjustmentAlignBaselines;
	}
	if(nstyle & QUILabelStyleVCenter)
	{
		nalign |= UIBaselineAdjustmentAlignCenters;
	}
	if(nadjust != 0)
	{
		((QIOSUILabel *)pview->midview).baselineAdjustment = (UIBaselineAdjustment )nadjust;
	}
	if(nstyle & QUILabelStyleFitFont)
	{
		((QIOSUILabel *)pview->midview).adjustsFontSizeToFitWidth = YES;
	}
	
	return QNO_OK;
}

QINT QUILabelInitText(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUILabel *pview;
	
	pview = (QUILabel *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
	if(pview->midview == nil)
	{
		return QNO_FAIL;
	}
	((QIOSUILabel *)pview->midview).text = [NSString stringWithCString: (const char *)params[1] length:(NSUInteger)params[2]];
	
	return QNO_OK;
}

QINT QUILabelInitFont(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nfont;
	QCHR vfont[QSTR_BUFF_SIZE];
	QUILabel *pview;
	
	pview = (QUILabel *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
	if(pview->midview == nil)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	ncnt = nlen;
	nfont = qstrint(0, pval, &ncnt);
	if(ncnt == nlen)
	{
		((QIOSUILabel *)pview->midview).font = [UIFont systemFontOfSize:nfont];
	}
	else
	{
		qstrscan((QPNT)pval, nlen, (QPNT)"%s-%d", vfont, &nfont);
		((QIOSUILabel *)pview->midview).font = [UIFont fontWithName:[NSString stringWithUTF8String:(const char *)vfont] size:nfont];
	}
	
	return QNO_OK;
}

QINT QUILabelInitHighlightColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUILabel *pview;
	
	pview = (QUILabel *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
	if(pview->midview == nil)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	((QIOSUILabel *)pview->midview).highlightedTextColor = quiStr2UIColor(pval);
	
	return QNO_OK;
}


QINT QUILabelInitShadowColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUILabel *pview;
	
	pview = (QUILabel *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
	if(pview->midview == nil)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	((QIOSUILabel *)pview->midview).shadowColor = quiStr2UIColor(pval);
	
	return QNO_OK;
}


QINT QUILabelInitShadowOffset(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval, psep;
	QINT nlen, ncnt, nxval, nyval;
	QUILabel *pview;
	
	pview = (QUILabel *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
	if(pview->midview == nil)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	psep = (QSTR)qstrchr(1, (QPNT)pval, nlen, ',');

	ncnt = nlen;
	nxval = qstrint(0, pval, &ncnt);
	if(psep == NULL)
	{
		nyval = ((QIOSUILabel *)pview->midview).shadowOffset.height;
	}
	else
	{
		ncnt = nlen-(psep-pval+1);
		nyval = qstrint(0, pval, &ncnt);
	}

	((QIOSUILabel *)pview->midview).shadowOffset = CGSizeMake(nxval, nyval);
	
	return QNO_OK;
}

QINT QUILabelInitLines(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	QUILabel *pview;
	
	pview = (QUILabel *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
	if(pview->midview == nil)
	{
		return QNO_FAIL;
	}
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	((QIOSUILabel *)pview->midview).numberOfLines = qstrint(0, pval, &nlen);
	
	return QNO_OK;
}

#endif
