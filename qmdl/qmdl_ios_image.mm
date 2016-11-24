
#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#include "qmdl_ios.h"


// -- OC类定义实现

@interface QIOSUIImage : UIImage
{
	QMDL mpmodule;
}

@property(nonatomic, assign) QMDL module;

- (id)initWithModule:(QMDL)module;

@end

@implementation QIOSUIImage

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


QUIImage::QUIImage()
{
}

QUIImage::~QUIImage()
{
}

QINT QUIImageInitStyle(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QNO_OK;
}

QINT QUIImageInitText(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QNO_OK;
}

QINT QUIImageInitFont(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QNO_OK;
}

QINT QUIImageInitHighlightColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIImage *pview;
	
	pview = (QUIImage *)hdl;
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
	}
	else if(code == QCD_SET)
	{
	}
	
	return QNO_OK;
}


QINT QUIImageInitShadowColor(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QINT nval, nlen;
	QUIImage *pview;
	
	pview = (QUIImage *)hdl;
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
	}
	else if(code == QCD_SET)
	{
	}
	
	return QNO_OK;
}


QINT QUIImageInitShadowOffset(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIImage *pview;
	
	pview = (QUIImage *)hdl;
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
	}
	else if(code == QCD_SET)
	{
	}
	
	return QNO_OK;
}

QINT QUIImageInitLines(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIImage *pview;
	
	pview = (QUIImage *)hdl;
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
	}
	else if(code == QCD_SET)
	{
	}
	
	return QNO_OK;
}

static QINT QUIImageOnMake(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIImage *plabel;
	
	plabel = (QUIImage *)hdl;
	if(plabel == NULL)
	{
		return QNO_OK;
	}
	if(plabel->midview == nil)
	{
		plabel->midview  = [[QIOSUIImage alloc] initWithModule:plabel];
	}
	
	return QNO_OK;
}

QINT QUIImageSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIImage *plabel;
	
	plabel = (QUIImage *)hdl;
	if(plabel == NULL)
	{
		return QNO_OK;
	}
	if(code == QCD_MAKE)
	{
		QUIImageOnMake(hdl, name, code, params, count);
	}
	
	return QNO_OK;
}

#endif
