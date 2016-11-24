
#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#include "qmdl_ios.h"


// -- OC类定义实现

@interface QIOSUITable : UITableView
{
	QMDL mpmodule;
}

@property(nonatomic, assign) QMDL module;

- (id)initWithModule:(QMDL)module;

@end

@implementation QIOSUITable

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

QUIList::QUIList()
{
}

QUIList::~QUIList()
{
}

QINT QUIList::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	QSTR pid;
	QINT ntag;
	
	if(midview != nil)
	{
		return QSCN_OK;
	}
	pid = (QSTR)qxmlGetId(mxml);
	if(pid != NULL && qstrcmp(QSTR_ICS, (QPNT)pid, (QPNT)qmdl_name(QUISlider), 0))
	{
		ntag = 0;
		midview = FindViewByTag(env, parent, NULL, mxml, url, &ntag);
		if(midview == nil)
		{
			midview = [[QIOSUITable alloc] initWithModule:this];
			((QIOSUITable *)midview).module = this;
			if(ntag != 0)
			{
				((UIView *)midview).tag = ntag;
			}
		}
	}
	
	return QSCN_OK;
}

#endif
