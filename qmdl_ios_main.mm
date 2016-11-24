
#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#include "qmdl_ios.h"
#include "qmdl_ios_internal.h"


static QPMK qpnwMainModule;


// -- QUINavigationController

@interface QUINavigationController : UINavigationController

@end

@implementation QUINavigationController

- (void)viewDidLoad
{
	[super viewDidLoad];
	// Do any additional setup after loading the view.
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(viewControllerOrientationFinished:)
												 name:UIDeviceOrientationDidChangeNotification
											   object:[UIDevice currentDevice]];
}

- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];
	// Dispose of any resources that can be recreated.
}

- (BOOL)shouldAutorotate
{
	return self.topViewController.shouldAutorotate;
}

- (NSUInteger)supportedInterfaceOrientations
{
	return self.topViewController.supportedInterfaceOrientations;
}

- (void)viewControllerOrientationFinished:(id)notif
{
	
}

/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */

@end


// -- QAppDelegate

@interface QAppDelegate : UIResponder <UIApplicationDelegate>
{
	QMain *mpmain;
}

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) QUINavigationController *navigation;

@property (strong, nonatomic) dispatch_queue_t mpqueue;

@end

@implementation QAppDelegate

@synthesize window;
@synthesize navigation;

@synthesize mpqueue;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	// Override point for customization after application launch.
	
	window = nil;
	navigation = nil;
	
	// 创建事件队列。
	mpqueue = dispatch_get_main_queue();
	
	qmdl_mxml_begin(main)
		qmdl_object_head("main", NULL, "/lib/QMain")
			qmdl_attrp("application", CFBridgingRetain(application))
			qmdl_attrp("delegate", CFBridgingRetain(self))
		qmdl_object_tail
	qmdl_mxml_end
	mpmain = (QMain *)qmdlMake(NULL, NULL, qmdl_claxx(QMain), NULL, NULL, NULL);
	mpmain->SetName((QSTR)"main");
	qmdlMake(mpmain, NULL, NULL, mpmain, (QXML)main, (QSTR)"/main");
	if(qpnwMainModule != NULL)
	{
		QMDL pmodule = qmdlMake(mpmain, mpmain, qpnwMainModule, NULL, NULL, (QSTR)"/");
		if(pmodule != NULL)
		{
			qmdlPush(mpmain, pmodule);
		}
	}
	
	return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
	// Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
	// Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
	// Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
	// If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
	// Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	// Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	// Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end


// -- QMain

QMain::QMain()
{
	main = this;
}

QMain::~QMain()
{
}

QINT QMain::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	return QSCN_OK;
}

QINT QMainInitApplication(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QMain *pview;
	
	pview = (QMain *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
	if(pview->midapplication != nil)
	{
		return QNO_OK;
	}
	pview->midapplication = (__bridge id)params[2];
	if(pview->midapplication == nil)
	{
		return QNO_FAIL;
	}
	
	return QNO_OK;
}

QINT QMainInitDelegate(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QMain *pview;
	
	pview = (QMain *)hdl;
	if(pview == NULL)
	{
		return QNO_FAIL;
	}
	if(pview->middelegate != nil)
	{
		return QNO_OK;
	}
	pview->middelegate = (__bridge id)params[2];
	if(pview->middelegate == nil)
	{
		return QNO_FAIL;
	}
	QAppDelegate *pdelegate = (QAppDelegate *)pview->middelegate;
	pdelegate.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	
	return QNO_OK;
}

QINT QMain::PostEvent(QMDL module, QSTR name, QINT code, QPNT params[], QINT count)
{
	QAppDelegate *pdelegate = (QAppDelegate *)middelegate;
	if(pdelegate == nil)
	{
		return QNO_FAIL;
	}
	if(pdelegate.mpqueue == nil)
	{
		return QNO_FAIL;
	}
	dispatch_async(pdelegate.mpqueue, ^{
		QMDL pmodule = module;
		if(pmodule == NULL)
		{
			pmodule = this;
		}
		QINT nresult = pmodule->RouteCb(NULL, name, code, params, count);
		if(nresult == QNO_TRUE)
		{
			pmodule->DispatchEventx(name, code, params, count);
		}
	});
	
	return QNO_OK;
}

QINT QMain::SendEvent(QMDL module, QSTR name, QINT code, QPNT params[], QINT count)
{
	QMDL pmodule = module;
	if(pmodule == NULL)
	{
		pmodule = this;
	}
	QINT nresult = pmodule->RouteCb(NULL, name, code, params, count);
	if(nresult == QSCN_FAIL || nresult == QSCN_ERR || nresult == QSCN_END)
	{
		return nresult;
	}
	pmodule->DispatchEventx(name, code, params, count);
	
	return QNO_OK;
}

QINT QMain::PushModule(QMDL module)
{
	if(module == NULL)
	{
		return QNO_FAIL;
	}
	QAppDelegate *pdelegate = (QAppDelegate *)middelegate;
	if(pdelegate == nil)
	{
		return QNO_FAIL;
	}
	if(pdelegate.mpqueue == nil)
	{
		return QNO_FAIL;
	}
	QUIWindowController *pcontroller = [[QUIWindowController alloc] initWithApplication:midapplication
																			   delegate:middelegate
																					env:this
																				 module:module];
	if(pdelegate.navigation == nil)
	{
		pdelegate.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
		pdelegate.navigation = [[QUINavigationController alloc] initWithRootViewController:pcontroller];
		pdelegate.navigation.navigationBarHidden = YES;
		pdelegate.window.rootViewController = pdelegate.navigation;
		[pdelegate.window makeKeyAndVisible];
	}
	else
	{
		[pdelegate.navigation pushViewController:(UIViewController *)pcontroller animated:YES];
	}
	
	return QNO_OK;
}

QINT QMain::PopModule(QMDL module)
{
	QAppDelegate *pdelegate = (QAppDelegate *)middelegate;
	if(pdelegate == nil)
	{
		return QNO_FAIL;
	}
	if(pdelegate.mpqueue == nil)
	{
		return QNO_FAIL;
	}
	dispatch_async(pdelegate.mpqueue, ^{
		QMDL pmodule, pprev, pparent;
		QUIWindow *pwindow, *pwinmdl;
		
		pwinmdl = NULL;
		if(module != NULL)
		{
			pparent = module;
			while(pparent)
			{
				pwinmdl = dynamic_cast<QUIWindow*>(pparent);
				if(pwinmdl != NULL)
				{
					break;
				}
				pparent = pparent->ParentModule();
			}
		}
		pmodule = FindModule((QSTR)QPATH_STK, 0, NULL, 0);
		if(pmodule != NULL)
		{
			pmodule = pmodule->TailModule();
		}
		pwindow = dynamic_cast<QUIWindow*>(pmodule);
		while(pwindow)
		{
			pprev = pwindow->PrevModule();
			// 广播模块退栈释放消息。
			[pdelegate.navigation popViewControllerAnimated:YES];
			if(pwindow == pwinmdl)
			{
				// 多级退栈。
				break;
			}
			if(pwinmdl == NULL)
			{
				// 单级退栈。
				break;
			}
			pwindow = dynamic_cast<QUIWindow*>(pprev);
		}
	});
	
	return QNO_OK;
}

QINT QMain::ShowModule(QMDL module)
{
	return PushModule(module);
}

QINT QMain::HideModule(QMDL module)
{
	return PopModule(module);
}

QINT QMain::LoadString(QSTR file, QSTR name, QSTR value, QINT size)
{
	// 国际化变量读取。
	NSString *strname, *strvalue;
	
	strname = [NSString stringWithUTF8String:(const char *)name];
	strvalue = NSLocalizedString(strname, @"");
	[strvalue getCString:(char *)value maxLength:size encoding:NSUTF8StringEncoding];
	
	return [strvalue length];
};

QINT QMain::GetScreenRect(QRECT *rect)
{
	if(rect == NULL)
	{
		return QNO_FAIL;
	}
	rect->left = [UIScreen mainScreen].bounds.origin.x;
	rect->top = [UIScreen mainScreen].bounds.origin.y;
	rect->right = [UIScreen mainScreen].bounds.size.width;
	rect->bottom = [UIScreen mainScreen].bounds.size.height;
	
	return QNO_OK;
};

QINT qmdlMain(int argc, char * argv[], QPMK main)
{
	qpnwMainModule = main;
	@autoreleasepool {
		return UIApplicationMain(argc, argv, @"UIApplication", NSStringFromClass([QAppDelegate class]));
	}
}


#endif
