
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
	qstr_enum_item( "dark",				UIButtonTypeInfoDark )
	qstr_enum_item( "add",				UIButtonTypeContactAdd )
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

QINT QUIButtonInitBase(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	id idview;
	QMDL penv;
	QXML pxml;
	QUCHR vbuff[QSTR_BUFF_SIZE];
	QINT nlen, ncnt, ntag;
	QUIButton *pview;
	UIButtonType ntype;
	
	pview = (QUIButton *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
	if(pview->midview != nil)
	{
		return QNO_OK;
	}
	penv = (QMDL)params[0];
	pxml = (QXML)params[2];
	ntag = 0;
	idview = pview->FindViewByTag(penv, pview, NULL, pxml, (QSTR)name, &ntag);
	if(idview != nil)
	{
		pview->midview = idview;
		((QIOSUIButton *)pview->midview).tag = ntag;
		return QSCN_OK;
	}
	nlen = pview->GetAttrByName(pview, pxml, NULL, (QPNT)"style", vbuff, sizeof(vbuff));
	ncnt = nlen;
	ntype = (UIButtonType)qstr2enum(QUIButtonStyle, 1, (QPNT)vbuff, &ncnt);
	if(ncnt <= 0)
	{
		ntype = UIButtonTypeRoundedRect;
	}
	pview->midview = (id)[QIOSUIButton buttonWithType:ntype module:pview];
	
	return QSCN_OK;
}

QINT QUIButtonInitFont(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen, ncnt, nsize;
	QCHR vfont[QSTR_BUFF_SIZE];
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
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	ncnt = nlen;
	nsize = qstrint(0, pval, &ncnt);
	if(ncnt == nlen)
	{
		((QIOSUIButton *)(pview->midview)).font = [UIFont systemFontOfSize:nsize];
	}
	else
	{
		qstrscan((QPNT)pval, nlen, (QPNT)"%s-%d", vfont, &nsize);
		((QIOSUIButton *)(pview->midview)).font = [UIFont fontWithName:[NSString stringWithUTF8String:(const char *)vfont] size:nsize];
	}
	
	return QNO_OK;
}

QINT QUIButtonInitNormalTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
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
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	[((QIOSUIButton *)(pview->midview)) setTitle:[NSString stringWithCString:(const char *)pval length:nlen]
										forState:UIControlStateNormal];
	
	return QSCN_OK;
}

QINT QUIButtonInitNormalColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
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
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	((QIOSUIButton *)(pview->midview)).titleLabel.textColor = quiStr2UIColor(pval);
	
	return QSCN_OK;
}

QINT QUIButtonInitNormalImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
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
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	[((QIOSUIButton *)(pview->midview)) setImage:[UIImage imageNamed:[NSString stringWithCString:(const char *)pval length:nlen]]
										forState:UIControlStateNormal];
	
	return QSCN_OK;
}

QINT QUIButtonInitNormalBackgroundImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
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
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	[((QIOSUIButton *)(pview->midview)) setBackgroundImage:[UIImage imageNamed:[NSString stringWithCString:(const char *)pval length:nlen]]
												  forState:UIControlStateNormal];
	
	return QSCN_OK;
}

QINT QUIButtonInitHighlightTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
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
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	[((QIOSUIButton *)(pview->midview)) setTitle:[NSString stringWithCString:(const char *)pval length:nlen]
										forState:UIControlStateHighlighted];
	
	return QSCN_OK;
}

QINT QUIButtonInitHighlightImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
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
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	[((QIOSUIButton *)(pview->midview)) setImage:[UIImage imageNamed:[NSString stringWithCString:(const char *)pval length:nlen]]
										forState:UIControlStateHighlighted];
	
	return QSCN_OK;
}

QINT QUIButtonInitHighlightBackgroundImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
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
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	[((QIOSUIButton *)(pview->midview)) setBackgroundImage:[UIImage imageNamed:[NSString stringWithCString:(const char *)pval length:nlen]]
												  forState:UIControlStateHighlighted];
	
	return QSCN_OK;
}

QINT QUIButtonInitDisableTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
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
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	[((QIOSUIButton *)(pview->midview)) setTitle:[NSString stringWithCString:(const char *)pval length:nlen]
										forState:UIControlStateDisabled];
	
	return QSCN_OK;
}

QINT QUIButtonInitDisableImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
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
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	[((QIOSUIButton *)(pview->midview)) setBackgroundImage:[UIImage imageNamed:[NSString stringWithCString:(const char *)pval length:nlen]]
										forState:UIControlStateDisabled];
	
	return QSCN_OK;
}

QINT QUIButtonInitDisableBackgroundImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
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
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	[((QIOSUIButton *)(pview->midview)) setBackgroundImage:[UIImage imageNamed:[NSString stringWithCString:(const char *)pval length:nlen]]
										forState:UIControlStateDisabled];
	
	return QSCN_OK;
}

QINT QUIButtonInitSelectedTitle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
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
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	[((QIOSUIButton *)(pview->midview)) setTitle:[NSString stringWithCString:(const char *)pval length:nlen]
										forState:UIControlStateSelected];
	
	return QSCN_OK;
}

QINT QUIButtonInitSelectedImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
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
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	[((QIOSUIButton *)(pview->midview)) setImage:[UIImage imageNamed:[NSString stringWithCString:(const char *)pval length:nlen]]
										forState:UIControlStateSelected];
	
	return QSCN_OK;
}

QINT QUIButtonInitSelectedBackgroundImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
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
	pval = (QSTR )params[2];
	nlen = (QINT )params[3];
	[((QIOSUIButton *)(pview->midview)) setBackgroundImage:[UIImage imageNamed:[NSString stringWithCString:(const char *)pval length:nlen]]
												  forState:UIControlStateSelected];
	
	return QSCN_OK;
}

#endif
