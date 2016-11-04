
#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#include "qmdl_ios.h"
#include "qmdl_ios_internal.h"


// -- QUIWindowController

static QINT qmdl_attach_window_scan_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QMDL pparentmdl;
	QUIWindow *pwindow;
	QUIView *pview, *pparentview;
	
	pview = dynamic_cast<QUIView *>((QMDL)hdl);
	pwindow = (QUIWindow *)params[0];
	if(pview == NULL || pwindow == NULL)
	{
		return QSCN_OK;
	}
	if(pview == pwindow)
	{
		return QSCN_OK;
	}
	if(pview->midview != nil && ((UIView *)pview->midview).superview == nil)
	{
		pparentmdl = pview->ParentModule();
		while(pparentmdl)
		{
			pparentview = dynamic_cast<QUIView *>(pparentmdl);
			if(pparentview != NULL)
			{
				if(pparentview->midview != nil)
				{
					[((UIView *)pparentview->midview) addSubview:(UIView *)pview->midview];
					break;
				}
			}
			pparentmdl = pparentmdl->ParentModule();
		}
	}
	if(pview->midcontroller != nil && ((UIViewController *)pview->midcontroller).parentViewController == nil)
	{
		pparentmdl = pview->ParentModule();
		while(pparentmdl)
		{
			pview = dynamic_cast<QUIView *>(pparentmdl);
			if(pview != NULL)
			{
				if(pview->midcontroller != nil)
				{
					[((UIViewController *)pview->midcontroller) addChildViewController:(UIViewController *)pview->midcontroller];
					break;
				}
			}
			pparentmdl = pparentmdl->ParentModule();
		}
	}
	
	return QSCN_OK;
}

static QINT qmdl_dispatch_event_scan_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QMDL pmodule = (QMDL)hdl;
	if(pmodule == NULL)
	{
		return QSCN_OK;
	}
	QINT nresult = pmodule->ModuleCb(NULL, (QINT)params[0], &params[1], count-1);
	if(nresult == QSCN_FAIL || nresult == QSCN_ERR || nresult == QSCN_END)
	{
		return nresult;
	}
	pmodule->DispatchEventx(NULL, (QINT)params[0], &params[1], count-1);
	
	return QSCN_OK;
}

@implementation QUIWindowController

@synthesize window = mpwindow;

- (id)initWithApplication:(id)application delegate:(id)delegate env:(QMDL)env module:(QMDL)module
{
	self = [super init];
	if(self == nil)
	{
		return nil;
	}
	mpwindow = dynamic_cast<QUIWindow *>(module);;
	if(mpwindow == NULL)
	{
		mpwindow = (QUIWindow*)qmdlMake(env, NULL, qmdl_claxx(QUIWindow), NULL, NULL, NULL);
		if(mpwindow == NULL)
		{
			return self;
		}
		mpwindow->RegisterModule(NULL, module);
	}
	mpwindow->midapplication = application;
	mpwindow->middelegate = delegate;
	
	return self;
}

- (void)viewDidLoad
{
	[super viewDidLoad];
	// Do any additional setup after loading the view.
	
	if(mpwindow == NULL)
	{
		return;
	}
	mpwindow->midcontroller = self;
	mpwindow->PushWindow();
}

- (void)viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
	
	if(mpwindow == NULL)
	{
		return;
	}
	mpwindow->PaintWindow();
}

- (void)viewDidUnload
{
	[super viewDidUnload];
	
	mpwindow->PopWindow();
	mpwindow = NULL;
}

- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];
	// Dispose of any resources that can be recreated.
}

@end


QUIWindow::QUIWindow()
{
	midview = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	AddPositionRule(QLayoutWidth, NULL, QLayoutNone, 1, [[UIScreen mainScreen] bounds].size.width);
	AddPositionRule(QLayoutHeight, NULL, QLayoutNone, 1, [[UIScreen mainScreen] bounds].size.height);
	[(UIView *)midview setBackgroundColor:[UIColor colorWithRed:92/255.0 green:185/255.0 blue:204/255.0 alpha:1.0]];
	[(UIView *)midview setBackgroundColor:[UIColor lightGrayColor]];
}

QUIWindow::~QUIWindow()
{
}

QINT QUIWindowSelfCb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QUIWindow *pwindow;
	
	pwindow = (QUIWindow *)hdl;
	if(pwindow == NULL)
	{
		return QNO_OK;
	}
	if(code == QCD_MAKE)
	{
	}
	
	return QNO_OK;
}

QINT QUIWindow::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	return QSCN_OK;
}

QINT QUIWindow::PushWindow()
{
	QINT ncount;
	QPNT vpprms[8];
	
	RegisterModule((QSTR)QPATH_STK, this);
	
	// 创建窗口view。
	[((QUIWindowController *)midcontroller).view addSubview:midview];
	
	// 关联入栈界面。
	ncount = 0;
	vpprms[ncount++] = (QPNT)this;
	ScanModulex(this, qmdl_attach_window_scan_cb, NULL, vpprms, ncount);
	// 广播模块创建消息。
	ncount = 0;
	vpprms[ncount++] = (QPNT)QCD_MAKE;
	vpprms[ncount++] = (QPNT)this;
	ScanModulex(this, qmdl_dispatch_event_scan_cb, NULL, vpprms, ncount);
	// 广播模块压栈消息。
	ncount = 0;
	vpprms[ncount++] = (QPNT)QCD_PUSH;
	vpprms[ncount++] = (QPNT)this;
	ScanModulex(this, qmdl_dispatch_event_scan_cb, NULL, vpprms, ncount);
	
	return QNO_OK;
	
}

QINT QUIWindow::PopWindow()
{
	QINT ncount;
	QPNT vpprms[8];
	
	ncount = 0;
	vpprms[ncount++] = (QPNT)QCD_POP;
	vpprms[ncount++] = (QPNT)this;
	ScanModulex(this, qmdl_dispatch_event_scan_cb, NULL, vpprms, ncount);
	ncount = 0;
	vpprms[ncount++] = (QPNT)QCD_FREE;
	vpprms[ncount++] = (QPNT)this;
	ScanModulex(this, qmdl_dispatch_event_scan_cb, NULL, vpprms, ncount);
	
	UnregisterModule(NULL, NULL);
	
	return QNO_OK;
}

QINT QUIWindow::PaintWindow()
{
	QINT ncount;
	QPNT vpprms[8];
	
	LayoutPosition();
	if(HasLayoutPosition() == QNO_FALSE)
	{
		if(MainModule() != NULL)
		{
			QRECT rcwindow;
			qmdlGetScreenRect(this, &rcwindow);
			SetLayoutPosition(&rcwindow);
		}
	}
	UpdatePosition();
	// 广播模块刷新消息。
	ncount = 0;
	vpprms[ncount++] = (QPNT)QCD_PAINT;
	vpprms[ncount++] = (QPNT)this;
	ScanModulex(this, qmdl_dispatch_event_scan_cb, NULL, vpprms, ncount);
	
	return QNO_OK;
}


#endif
