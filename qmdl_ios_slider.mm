
#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#include "qmdl_ios.h"


// -- OC类定义实现

@interface QIOSUISlider : UISlider
{
	QMDL mpmodule;
}

@property(nonatomic, assign) QMDL module;

- (id)initWithModule:(QMDL)module;

@end

@implementation QIOSUISlider

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

QUISlider::QUISlider()
{
}

QUISlider::~QUISlider()
{
}

QINT QUISlider::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	return QSCN_OK;
}

static QINT QUISliderOnMake(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUILabel *pslider;
	
	pslider = (QUILabel *)hdl;
	if(pslider == NULL)
	{
		return QNO_OK;
	}
	if(pslider->midview == nil)
	{
		pslider->midview = [[QIOSUISlider alloc] initWithModule:pslider];
	}
	
	return QNO_OK;
}

QINT QUISliderSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUILabel *pslider;
	
	pslider = (QUILabel *)hdl;
	if(pslider == NULL)
	{
		return QNO_OK;
	}
	if(code == QCD_MAKE)
	{
		QUISliderOnMake(hdl, name, code, params, count);
	}
	
	return QNO_OK;
}

#endif
