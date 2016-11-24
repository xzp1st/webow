
#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#include "qmdl_ios.h"


// -- OC类定义实现

// - QIOSUITextField

@interface QIOSUITextField : UITextField<UITextFieldDelegate>
{
	QMDL mpmodule;
}

@property(nonatomic, assign) QMDL module;

- (id)initWithModule:(QMDL)module;

@end

@implementation QIOSUITextField

@synthesize module = mpmodule;

- (id)initWithModule:(QMDL)module
{
	self = [super init];
	if(self == nil)
	{
		return nil;
	}
	self.module = module;
	self.delegate = self;
	[self addTarget:self action:@selector(textFieldEditChanged:) forControlEvents:UIControlEventEditingChanged];
	
	return self;
}

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField
{
	if(self.module == NULL)
	{
		return YES;
	}
	QUIEdit *pedit = dynamic_cast<QUIEdit*>(self.module);
	if(pedit == NULL)
	{
		return YES;
	}
	
	return (pedit->mnstyle&QEDTS_ReadOnly)?NO:YES;
}

- (void)textFieldEditChanged:(UITextField *)textField
{
	QUIEdit *pedit = dynamic_cast<QUIEdit*>(self.module);
	if(pedit == NULL)
	{
		return;
	}
	pedit->DispatchEvent(NULL, QCD_CHANGED, 1, [((QIOSUITextField *)pedit->midview).text UTF8String]);
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
	QUIEdit *pedit = dynamic_cast<QUIEdit*>(self.module);
	if(pedit == NULL)
	{
		return YES;
	}
	NSString *ptext = [textField.text stringByReplacingCharactersInRange:range withString:string];
	pedit->DispatchEvent(NULL, QCD_CHANGING, 4, [ptext UTF8String], [string UTF8String], range.location, range.length);
	
	return YES;
}

- (BOOL)textFieldShouldClear:(UITextField *)textField
{
	QUIEdit *pedit = dynamic_cast<QUIEdit*>(self.module);
	if(pedit == NULL)
	{
		return YES;
	}
	pedit->DispatchEvent(NULL, QCD_CLEAR, 0);
	
	return YES;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
	[textField resignFirstResponder];
	
	QUIEdit *pedit = dynamic_cast<QUIEdit*>(self.module);
	if(pedit == NULL)
	{
		return YES;
	}
	pedit->DispatchEvent(NULL, QCD_DONE, 0);
	
	return YES;
}

@end

// - QIOSUITextView

@interface QIOSUITextView : UITextView<UITextViewDelegate>
{
	QMDL mpmodule;
}

@property(nonatomic, assign) QMDL module;

- (id)initWithModule:(QMDL)module;

@end

@implementation QIOSUITextView

@synthesize module = mpmodule;

- (id)initWithModule:(QMDL)module
{
	self = [super init];
	if(self == nil)
	{
		return nil;
	}
	self.module = module;
	self.delegate = self;
	
	return self;
}

- (void)textViewDidChange:(UITextView *)textView
{
	QUIEdit *pedit = dynamic_cast<QUIEdit*>(self.module);
	if(pedit == NULL)
	{
		return;
	}
	pedit->DispatchEvent(NULL, QCD_CHANGED, 1, [((QIOSUITextView *)pedit->midview).text UTF8String]);
}

- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
	QUIEdit *pedit = dynamic_cast<QUIEdit*>(self.module);
	if(pedit == NULL)
	{
		return YES;
	}
	NSString *ptext = [textView.text stringByReplacingCharactersInRange:range withString:text];
	pedit->DispatchEvent(NULL, QCD_CHANGING, 4, [ptext UTF8String], [text UTF8String], range.location, range.length);
	
	return YES;
}

- (void)textViewDidChangeSelection:(UITextView *)textView
{
	QUIEdit *pedit = dynamic_cast<QUIEdit*>(self.module);
	if(pedit == NULL)
	{
		return;
	}
	pedit->DispatchEvent(NULL, QCD_SELECTED, 1, [((QIOSUITextView *)pedit->midview).text UTF8String]);
}

- (BOOL)textView:(UITextView *)textView shouldInteractWithURL:(NSURL *)URL inRange:(NSRange)characterRange
{
	QUIEdit *pedit = dynamic_cast<QUIEdit*>(self.module);
	if(pedit == NULL)
	{
		return YES;
	}
	pedit->DispatchEvent(NULL, QCD_CLICKED, 1, [[URL absoluteString] UTF8String]);
	
	return YES;
}

- (BOOL)textView:(UITextView *)textView shouldInteractWithTextAttachment:(NSTextAttachment *)textAttachment inRange:(NSRange)characterRange
{

}

@end


// -- C++类实现

qstr_enum_begin(QEDTSEnum)
	qstr_enum_item( "None",				QEDTS_None )
	qstr_enum_item( "Line",				QEDTS_Line )
	qstr_enum_item( "Bezel",			QEDTS_Bezel )
	qstr_enum_item( "RoundedRect",		QEDTS_RoundedRect )
	
	qstr_enum_item( "MultiLine",		QEDTS_MultiLine )
	qstr_enum_item( "Password",			QEDTS_Password )
	qstr_enum_item( "ReadOnly",			QEDTS_ReadOnly )
	qstr_enum_item( "ClearsOnBeginEditing",	QEDTS_ClearsOnBeginEditing )
	qstr_enum_item( "AutoClear",		QEDTS_AutoClear )
	qstr_enum_item( "FitFont",			QEDTS_FitFont )
	
	qstr_enum_item( "WordCap",			QEDTS_WordCap )
	qstr_enum_item( "SentenceCap",		QEDTS_SentenceCap )
	qstr_enum_item( "CharCap",			QEDTS_CharCap )
	
	qstr_enum_item( "DefCorrection",	QEDTS_DefCorrection )
	qstr_enum_item( "HasCorrection",	QEDTS_HasCorrection )
	qstr_enum_item( "NoCorrection",		QEDTS_NoCorrection )
	
	qstr_enum_item( "AlignLeft",		QEDTS_AlignLeft )
	qstr_enum_item( "AlignHCenter",		QEDTS_AlignHCenter )
	qstr_enum_item( "AlignRight",		QEDTS_AlignRight )
	qstr_enum_item( "AlignTop",			QEDTS_AlignTop )
	qstr_enum_item( "AlignVCenter",		QEDTS_AlignVCenter )
	qstr_enum_item( "AlignBottom",		QEDTS_AlignBottom )
	qstr_enum_item( "AlignFill",		QEDTS_AlignFill )
	qstr_enum_item( "AlignCenter",		QEDTS_AlignCenter )

	qstr_enum_item( "Left",				QEDTS_AlignLeft )
	qstr_enum_item( "HCenter",			QEDTS_AlignHCenter )
	qstr_enum_item( "Right",			QEDTS_AlignRight )
	qstr_enum_item( "Top",				QEDTS_AlignTop )
	qstr_enum_item( "VCenter",			QEDTS_AlignVCenter )
	qstr_enum_item( "Bottom",			QEDTS_AlignBottom )
	qstr_enum_item( "Fill",				QEDTS_AlignFill )
	qstr_enum_item( "Center",			QEDTS_AlignCenter )
qstr_enum_end

qstr_enum_begin(QEDTViewModeEnum)
	qstr_enum_item( "Never",			QEDTViewMode_Never )
	qstr_enum_item( "WhileEditing",		QEDTViewMode_WhileEditing )
	qstr_enum_item( "UnlessEditing",	QEDTViewMode_UnlessEditing )
	qstr_enum_item( "Always",			QEDTViewMode_Always )
	qstr_enum_item( "Editing",			QEDTViewMode_Editing )
	qstr_enum_item( "Idle",				QEDTViewMode_Idle )
qstr_enum_end

qstr_enum_begin(QEDTKeyboardEnum)
	qstr_enum_item( "Default",			QEDTKeyboard_Default )
	qstr_enum_item( "ASC",				QEDTKeyboard_ASC )
	qstr_enum_item( "NumberPunctuation",QEDTKeyboard_NumberPunctuation )
	qstr_enum_item( "URL",				QEDTKeyboard_URL )
	qstr_enum_item( "Number",			QEDTKeyboard_Number )
	qstr_enum_item( "Num",				QEDTKeyboard_Num )
	qstr_enum_item( "Phone",			QEDTKeyboard_Phone )
	qstr_enum_item( "NamePhone",		QEDTKeyboard_NamePhone )
	qstr_enum_item( "Email",			QEDTKeyboard_Email )
	qstr_enum_item( "Decimal",			QEDTKeyboard_Decimal )
	qstr_enum_item( "Twitter",			QEDTKeyboard_Twitter )
	qstr_enum_item( "Alphabet",			QEDTKeyboard_Alphabet )
	
	qstr_enum_item( "DefAppearance",	QEDTKeyboard_DefAppearance )
	qstr_enum_item( "AlertAppearance",	QEDTKeyboard_AlertAppearance )
	qstr_enum_item( "DefaultAppearance",QEDTKeyboard_DefaultAppearance )
	
	qstr_enum_item( "ReturnKeyDefault",	QEDTKeyboard_ReturnKeyDefault )
	qstr_enum_item( "ReturnKeyGo",		QEDTKeyboard_ReturnKeyGo )
	qstr_enum_item( "ReturnKeyGoogle",	QEDTKeyboard_ReturnKeyGoogle )
	qstr_enum_item( "ReturnKeyJoin",	QEDTKeyboard_ReturnKeyJoin )
	qstr_enum_item( "ReturnKeyNext",	QEDTKeyboard_ReturnKeyNext )
	qstr_enum_item( "ReturnKeyRoute",	QEDTKeyboard_ReturnKeyRoute )
	qstr_enum_item( "ReturnKeySearch",	QEDTKeyboard_ReturnKeySearch )
	qstr_enum_item( "ReturnKeySend",	QEDTKeyboard_ReturnKeySend )
	qstr_enum_item( "ReturnKeyYahoo",	QEDTKeyboard_ReturnKeyYahoo )
	qstr_enum_item( "ReturnEmergencyCall",QEDTKeyboard_ReturnEmergencyCall )
qstr_enum_end


QUIEdit::QUIEdit()
{
	mnstyle = 0;
}

QUIEdit::~QUIEdit()
{
}

QINT QUIEditInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = pview->mnstyle;
		}
		if(plen != NULL)
		{
			*plen = sizeof(QINT);
		}
	}
	else if(code == QCD_SET)
	{
		QINT nval, nlen, nalign, ncorrection;
		
		nval = (QINT)params[2];
		nlen = (QINT)params[3];
		((QIOSUITextField *)pview->midview).borderStyle = (UITextBorderStyle)(nval&QEDTS_BorderMask);
		((QIOSUITextField *)pview->midview).secureTextEntry = (nval&QEDTS_Password)?YES:NO;
		((QIOSUITextField *)pview->midview).clearsOnBeginEditing = (nval&QEDTS_AutoClear)?YES:NO;
		((QIOSUITextField *)pview->midview).adjustsFontSizeToFitWidth = (nval&QEDTS_FitFont)?YES:NO;
		nalign = 0;
		if(nval & QEDTS_AlignLeft)
		{
			nalign = NSTextAlignmentLeft;
		}
		if(nval & QEDTS_AlignRight)
		{
			nalign = NSTextAlignmentRight;
		}
		if(nval & QEDTS_AlignHCenter)
		{
			nalign = NSTextAlignmentCenter;
		}
		((QIOSUITextField *)pview->midview).textAlignment = (NSTextAlignment)nalign;
		nalign = UIControlContentVerticalAlignmentCenter;
		if(nval & QEDTS_AlignTop)
		{
			nalign = UIControlContentVerticalAlignmentTop;
		}
		else if(nval & QEDTS_AlignRight)
		{
			nalign = UIControlContentVerticalAlignmentBottom;
		}
		else if(nval & QEDTS_AlignHCenter)
		{
			nalign = UIControlContentVerticalAlignmentCenter;
		}
		else if(nval & QEDTS_AlignFill)
		{
			nalign = UIControlContentVerticalAlignmentFill;
		}
		((QIOSUITextField *)pview->midview).contentVerticalAlignment = (UIControlContentVerticalAlignment)nalign;
		ncorrection = UITextAutocorrectionTypeDefault;
		if(nval & QEDTS_DefCorrection)
		{
			ncorrection = UITextAutocorrectionTypeDefault;
		}
		else if(nval & QEDTS_HasCorrection)
		{
			ncorrection = UITextAutocorrectionTypeYes;
		}
		else if(nval & QEDTS_NoCorrection)
		{
			ncorrection = UITextAutocorrectionTypeNo;
		}
		((QIOSUITextField *)pview->midview).autocorrectionType = (UITextAutocorrectionType)ncorrection;
		pview->mnstyle = nval;
	}
	
	return QNO_OK;
}

QINT QUIEditInitPassword(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (QINT)((QIOSUITextField *)pview->midview).secureTextEntry;
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
		((QIOSUITextField *)pview->midview).secureTextEntry = (BOOL)nval;
		if(nval == YES)
		{
			pview->mnstyle |= QEDTS_Password;
		}
		else
		{
			pview->mnstyle &= ~QEDTS_Password;
		}
	}
	
	return QNO_OK;
}

QINT QUIEditInitReadOnly(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (pview->mnstyle&QEDTS_ReadOnly)?QNO_TRUE:QNO_FALSE;
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
		if(nval == YES)
		{
			pview->mnstyle |= QEDTS_ReadOnly;
		}
		else
		{
			pview->mnstyle &= QEDTS_ReadOnly;
		}
	}
	
	return QNO_OK;
}

QINT QUIEditInitAutoCorrection(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
		QINT nval, *pval, *plen;
		
		pval = (QINT*)params[2];
		plen = (QINT*)params[3];
		if(pval != NULL)
		{
			nval = (QINT)((QIOSUITextField *)pview->midview).autocorrectionType;
			if(nval == UITextAutocorrectionTypeDefault)
			{
				*pval = QEDTS_DefCorrection;
			}
			else if(nval == UITextAutocorrectionTypeYes)
			{
				*pval = QEDTS_HasCorrection;
			}
			else if(nval == UITextAutocorrectionTypeNo)
			{
				*pval = QEDTS_NoCorrection;
			}
			else
			{
				*pval = QEDTS_DefCorrection;
			}
		}
		if(plen != NULL)
		{
			*plen = sizeof(QINT);
		}
	}
	else if(code == QCD_SET)
	{
		QINT nval, nlen, ncorrection;
		
		nval = (QINT)params[2];
		nlen = (QINT)params[3];
		ncorrection = UITextAutocorrectionTypeDefault;
		if(nval & QEDTS_DefCorrection)
		{
			ncorrection = UITextAutocorrectionTypeDefault;
		}
		else if(nval & QEDTS_HasCorrection)
		{
			ncorrection = UITextAutocorrectionTypeYes;
		}
		else if(nval & QEDTS_NoCorrection)
		{
			ncorrection = UITextAutocorrectionTypeNo;
		}
		((QIOSUITextField *)pview->midview).autocorrectionType = (UITextAutocorrectionType)ncorrection;
	}
	
	return QNO_OK;
}

QINT QUIEditInitAutoClear(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (QINT)((QIOSUITextField *)pview->midview).clearsOnBeginEditing;
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
		((QIOSUITextField *)pview->midview).clearsOnBeginEditing = (BOOL)nval;
		if(nval == YES)
		{
			pview->mnstyle |= QEDTS_AutoClear;
		}
		else
		{
			pview->mnstyle &= ~QEDTS_AutoClear;
		}
	}
	
	return QNO_OK;
}

QINT QUIEditInitClearButtonMode(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (QINT)((QIOSUITextField *)pview->midview).clearButtonMode;
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
		((QIOSUITextField *)pview->midview).clearButtonMode = (UITextFieldViewMode)nval;
	}
	
	return QNO_OK;
}

QINT QUIEditInitTextAlignment(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (QINT)((QIOSUITextField *)pview->midview).textAlignment;
		}
		if(plen != NULL)
		{
			*plen = sizeof(QINT);
		}
	}
	else if(code == QCD_SET)
	{
		QINT nval, nlen, nalign;
		
		nval = (QINT)params[2];
		nlen = (QINT)params[3];
		nalign = NSTextAlignmentLeft;
		if(nval & QEDTS_AlignLeft)
		{
			nalign = NSTextAlignmentLeft;
		}
		else if(nval & QEDTS_AlignRight)
		{
			nalign = NSTextAlignmentRight;
		}
		else if(nval & QEDTS_AlignHCenter)
		{
			nalign = NSTextAlignmentCenter;
		}
		((QIOSUITextField *)pview->midview).textAlignment = (NSTextAlignment)nalign;
		nalign = UIControlContentVerticalAlignmentCenter;
		if(nval & QEDTS_AlignTop)
		{
			nalign = UIControlContentVerticalAlignmentTop;
		}
		else if(nval & QEDTS_AlignRight)
		{
			nalign = UIControlContentVerticalAlignmentBottom;
		}
		else if(nval & QEDTS_AlignHCenter)
		{
			nalign = UIControlContentVerticalAlignmentCenter;
		}
		else if(nval & QEDTS_AlignFill)
		{
			nalign = UIControlContentVerticalAlignmentFill;
		}
		((QIOSUITextField *)pview->midview).contentVerticalAlignment = (UIControlContentVerticalAlignment)nalign;
	}
	
	return QNO_OK;
}

QINT QUIEditInitFont(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = ((QIOSUITextField *)(pview->midview)).font;
		}
		if(plen != NULL)
		{
			*plen = sizeof(UIFont*);
		}
	}
	else if(code == QCD_SET)
	{
		((QIOSUITextField *)(pview->midview)).font = (__bridge UIFont*)params[2];
	}
	
	return QNO_OK;
}

QINT QUIEditInitFitFont(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (QINT)((QIOSUITextField *)pview->midview).adjustsFontSizeToFitWidth;
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
		((QIOSUITextField *)pview->midview).adjustsFontSizeToFitWidth = (BOOL)nval;
		if(nval == YES)
		{
			pview->mnstyle |= QEDTS_FitFont;
		}
		else
		{
			pview->mnstyle &= ~QEDTS_FitFont;
		}
	}
	
	return QNO_OK;
}

QINT QUIEditInitMinFontSize(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (QINT)((QIOSUITextField *)pview->midview).minimumFontSize;
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
		((QIOSUITextField *)pview->midview).minimumFontSize = (CGFloat)nval;
	}
	
	return QNO_OK;
}

QINT QUIEditInitText(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (QSTR)[((QIOSUITextField *)pview->midview).text UTF8String];
		}
		if(plen != NULL)
		{
			*plen = (QINT)qstrlen((QPNT)[((QIOSUITextField *)pview->midview).text UTF8String]);
		}
	}
	else if(code == QCD_SET)
	{
		((QIOSUITextField *)pview->midview).text = quiUtf82NSString((QSTR)params[2], (QINT)params[3]);
	}
	
	return QNO_OK;
}

QINT QUIEditInitPlaceholder(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (QSTR)[((QIOSUITextField *)(pview->midview)).placeholder UTF8String];
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
		((QIOSUITextField *)(pview->midview)).placeholder = quiUtf82NSString((QSTR)pval, nlen);
	}
	
	return QSCN_OK;
}

QINT QUIEditInitBackgroundColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (QINT)quiUIColor2RGBA(((QIOSUITextField *)pview->midview).backgroundColor);
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
		((QIOSUITextField *)pview->midview).backgroundColor = [UIColor colorWithRed:qclrGetRColor(nval)/255.0
																			  green:qclrGetGColor(nval)/255.0
																			   blue:qclrGetBColor(nval)/255.0
																			  alpha:qclrGetAColor(nval)/255.0];
	}
	
	return QNO_OK;
}

QINT QUIEditInitTextColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (QINT)quiUIColor2RGBA(((QIOSUITextField *)pview->midview).textColor);
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
		((QIOSUITextField *)pview->midview).textColor = [UIColor colorWithRed:qclrGetRColor(nval)/255.0
																		green:qclrGetGColor(nval)/255.0
																		 blue:qclrGetBColor(nval)/255.0
																		alpha:qclrGetAColor(nval)/255.0];
	}
	
	return QNO_OK;
}

QINT QUIEditInitBackgroundImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (UIImage*)((QIOSUITextField*)(pview->midview)).background;
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
		((QIOSUITextField *)(pview->midview)).background = [UIImage imageNamed:quiUtf82NSString(pval, nlen)];
	}
	
	return QNO_OK;
}

QINT QUIEditInitDisabledImage(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (UIImage*)((QIOSUITextField*)(pview->midview)).disabledBackground;
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
		((QIOSUITextField *)(pview->midview)).disabledBackground = [UIImage imageNamed:quiUtf82NSString(pval, nlen)];
	}
	
	return QNO_OK;
}

QINT QUIEditInitLeftViewMode(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (QINT)((QIOSUITextField *)pview->midview).leftViewMode;
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
		((QIOSUITextField *)pview->midview).leftViewMode = (UITextFieldViewMode)nval;
	}
	
	return QNO_OK;
}

QINT QUIEditInitRightViewMode(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (QINT)((QIOSUITextField *)pview->midview).rightViewMode;
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
		((QIOSUITextField *)pview->midview).rightViewMode = (UITextFieldViewMode)nval;
	}
	
	return QNO_OK;
}

QINT QUIEditInitLeftView(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (UIImage*)((QIOSUITextField*)(pview->midview)).leftView;
		}
		if(plen != NULL)
		{
			*plen = sizeof(UIImage*);
		}
	}
	else if(code == QCD_SET)
	{
		((QIOSUITextField *)(pview->midview)).leftView = (__bridge UIView*)params[2];
	}
	
	return QNO_OK;
}

QINT QUIEditInitRightView(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (UIImage*)((QIOSUITextField*)(pview->midview)).leftView;
		}
		if(plen != NULL)
		{
			*plen = sizeof(UIImage*);
		}
	}
	else if(code == QCD_SET)
	{
		((QIOSUITextField *)(pview->midview)).leftView = (__bridge UIView*)params[2];
	}
	
	return QNO_OK;
}

QINT QUIEditInitKeyboard(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pview;
	
	pview = (QUIEdit *)hdl;
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
			*pval = (QINT)( ( ((QIOSUITextField *)pview->midview).keyboardType			<< QEDTKeyboard_TypeMove		) |
						    ( ((QIOSUITextField *)pview->midview).keyboardAppearance	<< QEDTKeyboard_AppearanceMove	) |
						    ( ((QIOSUITextField *)pview->midview).returnKeyType			<< QEDTKeyboard_ReturnKeyMove	)
						   );
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
		if((nval&QEDTKeyboard_TypeMask) == 0)
		{
			((QIOSUITextField *)pview->midview).keyboardType = (UIKeyboardType)(QEDTKeyboard_Default>>QEDTKeyboard_TypeMove);
		}
		else
		{
			((QIOSUITextField *)pview->midview).keyboardType = (UIKeyboardType)((nval&QEDTKeyboard_TypeMask)>>QEDTKeyboard_TypeMove);
		}
		if((nval&QEDTKeyboard_AppearanceMask) == 0)
		{
			((QIOSUITextField *)pview->midview).keyboardAppearance = (UIKeyboardAppearance)(QEDTKeyboard_DefaultAppearance>>QEDTKeyboard_AppearanceMove);
		}
		else
		{
			((QIOSUITextField *)pview->midview).keyboardAppearance = (UIKeyboardAppearance)((nval&QEDTKeyboard_AppearanceMask)>>QEDTKeyboard_AppearanceMove);
		}
		if((nval&QEDTKeyboard_ReturnKeyMask) == 0)
		{
			((QIOSUITextField *)pview->midview).returnKeyType = (UIReturnKeyType)(QEDTKeyboard_ReturnKeyDefault>>QEDTKeyboard_ReturnKeyMove);
		}
		else
		{
			((QIOSUITextField *)pview->midview).returnKeyType = (UIReturnKeyType)((nval&QEDTKeyboard_ReturnKeyMask)>>QEDTKeyboard_ReturnKeyMove);
		}
	}
	
	return QNO_OK;
}

static QINT QUIEditOnMake(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pedit;
	
	pedit = (QUIEdit *)hdl;
	if(pedit == NULL)
	{
		return QNO_OK;
	}
	if(pedit->midview == nil)
	{
		pedit->midview = [[QIOSUITextField alloc] initWithModule:pedit];
	}
	
	return QNO_OK;
}

QINT QUIEditSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIEdit *pedit;
	
	pedit = (QUIEdit *)hdl;
	if(pedit == NULL)
	{
		return QNO_OK;
	}
	if(code == QCD_MAKE)
	{
		QUIEditOnMake(hdl, name, code, params, count);
	}
	
	return QNO_OK;
}

#endif
