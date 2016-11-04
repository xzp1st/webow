
#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#include "qmdl_ios.h"


// -- OC类定义实现

@interface QIOSUITextField : UITextField
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
	
	return self;
}

@end


// -- C++类实现

QUIEdit::QUIEdit()
{
}

QUIEdit::~QUIEdit()
{
}

QINT QUIEdit::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	QSTR pid;
	QINT ntag;
	
	if(midview != nil)
	{
		return QSCN_OK;
	}
	pid = (QSTR)qxmlGetId(mxml);
	if(pid != NULL && qstrcmp(QSTR_CMP_ICASE, (QPNT)pid, (QPNT)qmdl_name(QUIEdit), 0))
	{
		ntag = 0;
		midview = FindViewByTag(env, parent, NULL, mxml, url, &ntag);
		if(midview == nil)
		{
			midview = [[QIOSUITextField alloc] initWithModule:this];
			((QIOSUITextField *)midview).module = this;
			if(ntag != 0)
			{
				((UIView *)midview).tag = ntag;
			}
		}
	}
	
	return QSCN_OK;
}

#endif
