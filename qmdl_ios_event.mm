
#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#include "qmdl_ios.h"


// -- OC类定义实现

@interface QIOSUIEvent : NSObject 
{
	QMDL mpmodule;
}

@property(nonatomic, assign) QMDL module;

- (id)initWithModule: (QMDL )module;

- (void)onTouchDown: (id)sender;
- (void)onTouchDownRepeat: (id)sender;
- (void)onTouchDragEnter: (id)sender;
- (void)onTouchDragExit: (id)sender;
- (void)onTouchDragInside: (id)sender;
- (void)onTouchDragOutside: (id)sender;
- (void)onTouchUpInside: (id)sender;
- (void)onTouchUpOutside: (id)sender;
- (void)onTouchCancel: (id)sender;
- (void)onValueChanged: (id)sender;

- (void)onTap: (UITapGestureRecognizer*)recognizer;
- (void)onPinch: (UITapGestureRecognizer*)recognizer;
- (void)onRotation: (UITapGestureRecognizer*)recognizer;
- (void)onSwipe: (UITapGestureRecognizer*)recognizer;
- (void)onPan: (UITapGestureRecognizer*)recognizer;
- (void)onLongPress: (UITapGestureRecognizer*)recognizer;

@end

@implementation QIOSUIEvent

@synthesize module = mpmodule;

- (id)initWithModule:(QMDL )module
{
	self = [self init];
	if(self != nil)
	{
		self.module = module;
	}
	
	return self;
}

- (void)onTouchDown: (id)sender
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QCD_TOUCHDOWN, 0);
	}
}

- (void)onTouchDownRepeat: (id)sender
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QCD_DBLCLK, 0);
	}
}

- (void)onTouchUpInside: (id)sender
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QCD_TOUCHUP, 0);
		mpmodule->RouteEvent(NULL, NULL,  QCD_CLICK, 0);
	}
}

- (void)onTouchUpOutside:(id)sender
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QCD_TOUCHUP, 0);
	}
}

- (void)onValueChanged:(id)sender
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QCD_CHANGE, 0);
	}
}

- (void)onTouchDragEnter: (id)sender
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QEventTouchDragEnter, 0);
	}
}

- (void)onTouchDragExit: (id)sender
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QEventTouchDragExit, 0);
	}
}

- (void)onTouchDragInside: (id)sender
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QEventTouchDragInside, 0);
	}
}

- (void)onTouchDragOutside: (id)sender
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QEventTouchDragOutside, 0);
	}
}

- (void)onTouchCancel: (id)sender
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QEventTouchCancel, 0);
	}
}

- (void)onTap: (UITapGestureRecognizer*)recognizer
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QCD_TAP, 1, recognizer);
	}
}

- (void)onPinch: (UIPinchGestureRecognizer*)recognizer
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QCD_PINCH, 1, recognizer);
	}
}

- (void)onRotation: (UIRotationGestureRecognizer*)recognizer
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QCD_ROTATION, 1, recognizer);
	}
}

- (void)onSwipe: (UISwipeGestureRecognizer*)recognizer
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QCD_SWIPE, 1, recognizer);
	}
}

- (void)onPan: (UIPanGestureRecognizer*)recognizer
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QCD_PAN, 1, recognizer);
	}
}

- (void)onLongPress: (UILongPressGestureRecognizer*)recognizer
{
	if(mpmodule != NULL)
	{
		mpmodule->RouteEvent(NULL, NULL,  QCD_LONGPRESS, 1, recognizer);
	}
}


@end


static QINT qui_init_event_scan_cb(QMDL observed, QMDL observer,
								   QSTR name, QINT code, QPFN event_fn, QPCB event_cb, QPNT params[], QINT count)
{
	id idview, idevent;
	QUIView *pobserved;
	
	pobserved = (QUIView *)params[0];
	if(pobserved != observed)
	{
		return QSCN_OK;
	}
	idview = pobserved->midview;
	idevent = pobserved->midevent;
	if(idevent == nil)
	{
		if(code != 0)
		{
			idevent = [[QIOSUIEvent alloc] initWithModule: pobserved];
			pobserved->midevent = idevent;
		}
	}
	switch(code)
	{
		case QCD_TOUCHDOWN:
			if([idview isKindOfClass:[UIControl class]])
			{
				[(UIControl *)idview addTarget:idevent action:@selector(onTouchDown:) forControlEvents:UIControlEventTouchDown];
			}
			break;
			
		case QCD_DBLCLK:
			if([idview isKindOfClass:[UIControl class]])
			{
				[(UIControl *)idview addTarget:idevent action:@selector(onTouchDownRepeat:) forControlEvents:UIControlEventTouchDownRepeat];
			}
			break;
			
		case QCD_TOUCHUP:
			if([idview isKindOfClass:[UIControl class]])
			{
				[(UIControl *)idview addTarget:idevent action:@selector(onTouchUpInside:) forControlEvents:UIControlEventTouchUpInside];
				[(UIControl *)idview addTarget:idevent action:@selector(onTouchUpOutside:) forControlEvents:UIControlEventTouchUpOutside];
			}
			break;
			
		case QCD_CLICK:
			if([idview isKindOfClass:[UIControl class]])
			{
				[(UIControl *)idview addTarget:idevent action:@selector(onTouchUpInside:) forControlEvents:UIControlEventTouchUpInside];
			}
			break;
			
		case QCD_CHANGE:
			if([idview isKindOfClass:[UIControl class]])
			{
				[(UIControl *)idview addTarget:idevent action:@selector(onValueChanged:) forControlEvents:UIControlEventValueChanged];
			}
			break;
			
		case QCD_TAP:
			if([idview isKindOfClass:[UIView class]])
			{
				UITapGestureRecognizer *precognizer = [[UITapGestureRecognizer alloc] initWithTarget:idevent action:@selector(onTap:)];
				if(precognizer != nil)
				{
					precognizer.delegate = idevent;
					[(UIView*)idview addGestureRecognizer:precognizer];
				}
			}
			break;
			
		case QCD_PAN:
			if([idview isKindOfClass:[UIView class]])
			{
				UIPanGestureRecognizer *precognizer = [[UIPanGestureRecognizer alloc] initWithTarget:idevent action:@selector(onPan:)];
				if(precognizer != nil)
				{
					precognizer.delegate = idevent;
					[(UIView*)idview addGestureRecognizer:precognizer];
				}
			}
			break;
			
		case QCD_PINCH:
			if([idview isKindOfClass:[UIView class]])
			{
				UIPinchGestureRecognizer *precognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:idevent action:@selector(onPinch:)];
				if(precognizer != nil)
				{
					precognizer.delegate = idevent;
					[(UIView*)idview addGestureRecognizer:precognizer];
				}
			}
			break;
			
		case QCD_SWIPE:
			if([idview isKindOfClass:[UIView class]])
			{
				UISwipeGestureRecognizer *precognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:idevent action:@selector(onSwipe:)];
				if(precognizer != nil)
				{
					precognizer.delegate = idevent;
					[(UIView*)idview addGestureRecognizer:precognizer];
				}
			}
			break;
			
		case QCD_ROTATION:
			if([idview isKindOfClass:[UIView class]])
			{
				UIRotationGestureRecognizer *precognizer = [[UIRotationGestureRecognizer alloc] initWithTarget:idevent action:@selector(onRotation:)];
				if(precognizer != nil)
				{
					precognizer.delegate = idevent;
					[(UIView*)idview addGestureRecognizer:precognizer];
				}
			}
			break;
			
		case QCD_LONGPRESS:
			if([idview isKindOfClass:[UIView class]])
			{
				UILongPressGestureRecognizer *precognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:idevent action:@selector(onLongPress:)];
				if(precognizer != nil)
				{
					precognizer.delegate = idevent;
					[(UIView*)idview addGestureRecognizer:precognizer];
				}
			}
			break;
			
		default:
			break;
	}
	
	return QSCN_OK;
}

void QUIView::InitUIEvent()
{
	ScanEvent(qui_init_event_scan_cb, 1, this);
}

#endif
