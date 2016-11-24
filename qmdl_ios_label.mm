
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

qstr_enum_begin(QLBLSEnum)
	qstr_enum_item( "default",			QLBLS_Center )
	qstr_enum_item( "top",				QLBLS_Top )
	qstr_enum_item( "bottom",			QLBLS_Bottom )
	qstr_enum_item( "left",				QLBLS_Left )
	qstr_enum_item( "right",			QLBLS_Right )
	qstr_enum_item( "vcenter",			QLBLS_VCenter )
	qstr_enum_item( "hcenter",			QLBLS_HCenter )
	qstr_enum_item( "center",			QLBLS_Center )

	qstr_enum_item( "wordwrap",			QLBLS_WordWrap )
	qstr_enum_item( "charwrap",			QLBLS_CharWrap )
	qstr_enum_item( "clip",				QLBLS_Clip )
	qstr_enum_item( "headtrunc",		QLBLS_HeadTrunc )
	qstr_enum_item( "tailtrunc",		QLBLS_TailTrunc )
	qstr_enum_item( "middletrunc",		QLBLS_MiddleTrunc )
qstr_enum_end


QUILabel::QUILabel()
{
}

QUILabel::~QUILabel()
{
}

QINT QUILabelInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
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
	if(code == QCD_GET)
	{
		QINT *pval, nlen;
		
		pval = (QINT*)params[2];
		nlen = (QINT)params[3];
		if(pval != NULL)
		{
			*pval = pview->mnstyle;
		}
	}
	else if(code == QCD_SET)
	{
		QINT nval, nlen, nmode, nalign, nadjust;
		
		nval = (QINT)params[2];
		nlen = (QINT)params[3];
		nmode = 0;
		if(nval & QLBLS_WordWrap)
		{
			nmode |= UILineBreakModeWordWrap;
		}
		if(nval & QLBLS_CharWrap)
		{
			nmode |= UILineBreakModeCharacterWrap;
		}
		if(nval & QLBLS_Clip)
		{
			nmode |= UILineBreakModeClip;
		}
		if(nval & QLBLS_HeadTrunc)
		{
			nmode |= UILineBreakModeHeadTruncation;
		}
		if(nval & QLBLS_TailTrunc)
		{
			nmode |= UILineBreakModeTailTruncation;
		}
		if(nval & QLBLS_MiddleTrunc)
		{
			nmode |= UILineBreakModeMiddleTruncation;
		}
		if(nmode != 0)
		{
			((QIOSUILabel *)pview->midview).lineBreakMode = (NSLineBreakMode )nmode;
		}
		nalign = 0;
		if(nval & QLBLS_Left)
		{
			nalign |= NSTextAlignmentLeft;
		}
		if(nval & QLBLS_Right)
		{
			nalign |= NSTextAlignmentRight;
		}
		if(nval & QLBLS_HCenter)
		{
			nalign |= NSTextAlignmentCenter;
		}
		if(nalign != 0)
		{
			((QIOSUILabel *)pview->midview).textAlignment = (NSTextAlignment )nalign;
		}
		nadjust = 0;
		if(nval & QLBLS_Top)
		{
			nadjust |= UIBaselineAdjustmentNone;
		}
		if(nval & QLBLS_Bottom)
		{
			nadjust |= UIBaselineAdjustmentAlignBaselines;
		}
		if(nval & QLBLS_VCenter)
		{
			nadjust |= UIBaselineAdjustmentAlignCenters;
		}
		if(nadjust != 0)
		{
			((QIOSUILabel *)pview->midview).baselineAdjustment = (UIBaselineAdjustment )nadjust;
		}
		if(nval & QLBLS_FitFont)
		{
			((QIOSUILabel *)pview->midview).adjustsFontSizeToFitWidth = YES;
		}
		pview->mnstyle = nval;
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
	if(code == QCD_GET)
	{
		QSTR *pval;
		QINT *plen;
		
		pval = (QSTR*)params[2];
		plen = (QINT*)params[3];
		if(pval != NULL)
		{
			*pval = (QSTR)[((QIOSUILabel *)pview->midview).text UTF8String];
		}
		if(plen != NULL)
		{
			*plen = (QINT)qstrlen((QPNT)[((QIOSUILabel *)pview->midview).text UTF8String]);
		}
	}
	else if(code == QCD_SET)
	{
		((QIOSUILabel *)pview->midview).text = quiUtf82NSString((QSTR)params[2], (QINT)params[3]);
	}
	
	return QNO_OK;
}

QINT QUILabelInitFont(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
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
	if(code == QCD_GET)
	{
		__weak UIFont **pval;
		QINT *plen;
		
		pval = (__weak UIFont**)params[2];
		plen = (QINT*)params[3];
		if(pval != NULL)
		{
			*pval = ((QIOSUILabel *)pview->midview).font;
		}
		if(plen != NULL)
		{
			*plen = sizeof(UIFont*);
		}
	}
	else if(code == QCD_SET)
	{
		UIFont *pval;
		QINT nlen;
		
		pval = (__bridge UIFont*)params[2];
		nlen = (QINT )params[3];
		((QIOSUILabel *)pview->midview).font = pval;
	}
	
	return QNO_OK;
}

QINT QUILabelInitHighlightColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
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
	if(code == QCD_GET)
	{
		QINT *pval, *plen;
		
		pval = (QINT*)params[2];
		plen = (QINT*)params[3];
		if(pval != NULL)
		{
			*pval = quiUIColor2RGBA(((QIOSUILabel *)pview->midview).highlightedTextColor);
		}
		if(plen != NULL)
		{
			*plen = sizeof(QINT);
		}
	}
	else if(code == QCD_SET)
	{
		QINT nval, nlen;
		
		nval = (QINT)params[2];
		nlen = (QINT)params[3];
		((QIOSUILabel *)pview->midview).highlightedTextColor = [UIColor colorWithRed:qclrGetRColor(nval)/255.0
																			   green:qclrGetGColor(nval)/255.0
																				blue:qclrGetBColor(nval)/255.0
																			   alpha:qclrGetAColor(nval)/255.0];
	}
	
	return QNO_OK;
}


QINT QUILabelInitShadowColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QINT nval, nlen;
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
	if(code == QCD_GET)
	{
		QINT *pval, *plen;
		
		pval = (QINT*)params[2];
		plen = (QINT*)params[3];
		if(pval != NULL)
		{
			*pval = quiUIColor2RGBA(((QIOSUILabel *)pview->midview).shadowColor);
		}
		if(plen != NULL)
		{
			*plen = sizeof(QINT);
		}
	}
	else if(code == QCD_SET)
	{
		QINT nval, nlen;
		
		nval = (QINT)params[2];
		nlen = (QINT)params[3];
		((QIOSUILabel *)pview->midview).shadowColor = [UIColor colorWithRed:qclrGetRColor(nval)/255.0
																	  green:qclrGetGColor(nval)/255.0
																	   blue:qclrGetBColor(nval)/255.0
																	  alpha:qclrGetAColor(nval)/255.0];
	}
	
	return QNO_OK;
}


QINT QUILabelInitShadowOffset(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
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
	if(code == QCD_GET)
	{
		CGSize *pval;
		QINT *plen;
		
		pval = (CGSize*)params[2];
		plen = (QINT*)params[3];
		if(pval != NULL)
		{
			*pval = ((QIOSUILabel *)pview->midview).shadowOffset;
		}
		if(plen != NULL)
		{
			*plen = sizeof(UIFont*);
		}
	}
	else if(code == QCD_SET)
	{
		QSTR pval, psep;
		QINT nlen, ncnt, nxval, nyval;
		
		pval = (QSTR )params[2];
		nlen = (QINT )params[3];
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
	}
	
	return QNO_OK;
}

QINT QUILabelInitLines(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
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
	if(code == QCD_GET)
	{
		QINT *pval, *plen;
		
		pval = (QINT*)params[2];
		plen = (QINT*)params[3];
		if(pval != NULL)
		{
			*pval = (QINT)((QIOSUILabel *)pview->midview).numberOfLines;
		}
		if(plen != NULL)
		{
			*plen = sizeof(QINT);
		}
	}
	else if(code == QCD_SET)
	{
		QINT nval, nlen;
		
		nval = (QINT)params[2];
		nlen = (QINT)params[3];
		((QIOSUILabel *)pview->midview).numberOfLines = (QINT)nval;
	}
	
	return QNO_OK;
}

static QINT QUILabelOnMake(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUILabel *plabel;
	
	plabel = (QUILabel *)hdl;
	if(plabel == NULL)
	{
		return QNO_OK;
	}
	if(plabel->midview == nil)
	{
		plabel->midview  = [[QIOSUILabel alloc] initWithModule:plabel];
	}
	
	return QNO_OK;
}

QINT QUILabelSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUILabel *plabel;
	
	plabel = (QUILabel *)hdl;
	if(plabel == NULL)
	{
		return QNO_OK;
	}
	if(code == QCD_MAKE)
	{
		QUILabelOnMake(hdl, name, code, params, count);
	}
	
	return QNO_OK;
}

#endif
