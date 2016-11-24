
#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#include "qmdl_ios.h"


// -- OC类定义实现

@interface QIOSUIButton : UIButton
{
	QINT mntag;
	QMDL mpmodule;
}

@property(nonatomic, assign) QINT tag;
@property(nonatomic, assign) QMDL module;

+ (id)buttonWithType:(UIButtonType)buttonType module:(QMDL)module;

@end

@implementation QIOSUIButton

@synthesize tag = mntag;
@synthesize module = mpmodule;

+ (id)buttonWithType:(UIButtonType)type module:(QMDL)module
{
	QIOSUIButton *pbutton = [QIOSUIButton buttonWithType:type];
	if(pbutton == NULL)
	{
		return nil;
	}
	pbutton.module = module;
	
	return (id)pbutton;
}

@end


// -- C++类实现

qstr_enum_begin(QUIButtonStyle)
	qstr_enum_item( "custom",				UIButtonTypeCustom )
	qstr_enum_item( "round",				UIButtonTypeRoundedRect )
	qstr_enum_item( "detail",				UIButtonTypeDetailDisclosure )
	qstr_enum_item( "light",				UIButtonTypeInfoLight )
	qstr_enum_item( "dark",					UIButtonTypeInfoDark )
	qstr_enum_item( "add",					UIButtonTypeContactAdd )
qstr_enum_end

QUIButton::QUIButton()
{
}

QUIButton::~QUIButton()
{
}

QINT QUIButton::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	return QSCN_OK;
}

QINT QUIButtonInitFont(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIButton *pview;
	
	pview = (QUIButton *)hdl;
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
			*pval = ((QIOSUIButton *)(pview->midview)).font;
		}
		if(plen != NULL)
		{
			*plen = sizeof(UIFont*);
		}
	}
	else if(code == QCD_SET)
	{
		((QIOSUIButton *)(pview->midview)).font = (__bridge UIFont*)params[2];
	}
	
	return QNO_OK;
}

QINT QUIButtonInitNormalColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIButton *pview;
	
	pview = (QUIButton *)hdl;
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
			*pval = quiUIColor2RGBA(((QIOSUIButton *)(pview->midview)).titleLabel.textColor);
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
		((QIOSUIButton *)(pview->midview)).titleLabel.textColor = [UIColor colorWithRed:qclrGetRColor(nval)/255.0
																				  green:qclrGetGColor(nval)/255.0
																				   blue:qclrGetBColor(nval)/255.0
																				  alpha:qclrGetAColor(nval)/255.0];
	}
	
	return QSCN_OK;
}

static QINT QUIButtonInitStateTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count, QINT state)
{
	QUIButton *pview;
	
	pview = (QUIButton *)hdl;
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
			*pval = (QSTR)[[((QIOSUIButton *)(pview->midview)) titleForState:state] UTF8String];
		}
		if(plen != NULL)
		{
			*plen = sizeof(QSTR);
		}
	}
	else if(code == QCD_SET)
	{
		QSTR pval;
		QINT nlen;
		
		pval = (QSTR )params[2];
		nlen = (QINT )params[3];
		[((QIOSUIButton *)(pview->midview)) setTitle:quiUtf82NSString(pval, nlen)
											forState:state];
	}
	
	return QSCN_OK;
}

static QINT QUIButtonInitStateImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count, QINT state)
{
	QUIButton *pview;
	
	pview = (QUIButton *)hdl;
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
		__weak UIImage **pval;
		QINT *plen;
		
		pval = (__weak UIImage**)params[2];
		plen = (QINT*)params[3];
		if(pval != NULL)
		{
			*pval = (UIImage*)[((QIOSUIButton *)(pview->midview)) imageForState:state];
		}
		if(plen != NULL)
		{
			*plen = sizeof(UIImage*);
		}
	}
	else if(code == QCD_SET)
	{
		QSTR pval;
		QINT nlen;
		pval = (QSTR )params[2];
		nlen = (QINT )params[3];
		[((QIOSUIButton *)(pview->midview)) setImage:[UIImage imageNamed:quiUtf82NSString(pval, nlen)]
											forState:state];
	}
	
	return QSCN_OK;
}

static QINT QUIButtonInitStateBackgroundImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count, QINT state)
{
	QUIButton *pview;
	
	pview = (QUIButton *)hdl;
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
		__weak UIImage **pval;
		QINT *plen;
		
		pval = (__weak UIImage**)params[2];
		plen = (QINT*)params[3];
		if(pval != NULL)
		{
			*pval = (UIImage*)[((QIOSUIButton *)(pview->midview)) backgroundImageForState:state];
		}
		if(plen != NULL)
		{
			*plen = sizeof(UIImage*);
		}
	}
	else if(code == QCD_SET)
	{
		QSTR pval;
		QINT nlen;
		pval = (QSTR )params[2];
		nlen = (QINT )params[3];
		[((QIOSUIButton *)(pview->midview)) setBackgroundImage:[UIImage imageNamed:quiUtf82NSString(pval, nlen)]
													  forState:state];
	}
	
	return QSCN_OK;
}

QINT QUIButtonInitNormalTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QUIButtonInitStateTitle(hdl, name, code, params, count, UIControlStateNormal);
}

QINT QUIButtonInitNormalImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QUIButtonInitStateImage(hdl, name, code, params, count, UIControlStateNormal);
}

QINT QUIButtonInitNormalBackgroundImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QUIButtonInitStateBackgroundImage(hdl, name, code, params, count, UIControlStateNormal);
}

QINT QUIButtonInitHighlightTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QUIButtonInitStateTitle(hdl, name, code, params, count, UIControlStateHighlighted);
}

QINT QUIButtonInitHighlightImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QUIButtonInitStateImage(hdl, name, code, params, count, UIControlStateHighlighted);
}

QINT QUIButtonInitHighlightBackgroundImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QUIButtonInitStateBackgroundImage(hdl, name, code, params, count, UIControlStateHighlighted);
}

QINT QUIButtonInitDisableTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QUIButtonInitStateTitle(hdl, name, code, params, count, UIControlStateDisabled);
}

QINT QUIButtonInitDisableImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QUIButtonInitStateImage(hdl, name, code, params, count, UIControlStateDisabled);
}

QINT QUIButtonInitDisableBackgroundImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QUIButtonInitStateBackgroundImage(hdl, name, code, params, count, UIControlStateDisabled);
}

QINT QUIButtonInitSelectedTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QUIButtonInitStateTitle(hdl, name, code, params, count, UIControlStateSelected);
}

QINT QUIButtonInitSelectedImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QUIButtonInitStateImage(hdl, name, code, params, count, UIControlStateSelected);
}

QINT QUIButtonInitSelectedBackgroundImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QUIButtonInitStateBackgroundImage(hdl, name, code, params, count, UIControlStateSelected);
}

static QINT QUIButtonOnMake(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QPNT ptype, pstyle;
	QUIButton *pbutton;
	QINT nresult, nlen, ncnt, nstyle;
	
	pbutton = (QUIButton *)hdl;
	if(pbutton == NULL)
	{
		return QNO_OK;
	}
	if(pbutton->midview == nil)
	{
		nstyle = UIButtonTypeRoundedRect;
		nresult = pbutton->GetInitItem((QPNT)"style", &ptype, &pstyle, &nlen);
		if(nresult == QNO_OK)
		{
			ncnt = nlen;
			nstyle = (UIButtonType)qstr2enum(QUIButtonStyle, 1, pstyle, &ncnt);
			if(ncnt <= 0)
			{
				nstyle = UIButtonTypeRoundedRect;
			}
		}
		pbutton->midview = (id)[QIOSUIButton buttonWithType:(UIButtonType)nstyle module:pbutton];
	}
	
	return QNO_OK;
}

QINT QUIButtonSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIButton *pbutton;
	
	pbutton = (QUIButton *)hdl;
	if(pbutton == NULL)
	{
		return QNO_OK;
	}
	if(code == QCD_MAKE)
	{
		QUIButtonOnMake(hdl, name, code, params, count);
	}
	
	return QNO_OK;
}

#endif
