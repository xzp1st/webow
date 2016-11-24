
#ifndef __QMDL_IOS_INTERNAL_H__
#define __QMDL_IOS_INTERNAL_H__

#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

#include "qmdl_module.h"
#include "qmdl_base.h"
#include "qmdl_ios.h"


// -- QUIWindowController

@interface QUIWindowController : UIViewController
{
	QUIWindow *mpwindow;
}

@property(nonatomic, assign) QUIWindow *window;

- (id)initWithApplication:(id)application delegate:(id)delegate env:(QMDL)env module:(QMDL)module;

@end


#endif //QMDL_VER_IOS

#endif //__QMDL_IOS_INTERNAL_H__
