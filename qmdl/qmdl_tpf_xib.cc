
#include "qmdl_config.h"

#ifdef QMDL_VER_IOS

#include "qmdl_ios.h"


QUIXib::QUIXib()
{
}

QUIXib::~QUIXib()
{
}

QINT QUIXib::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	QINT ntag;
	QPNT pval, pname;
	Class pclass;
	id idview, idcontroller;
	
	if(midview != nil)
	{
		return QSCN_OK;
	}
	ntag = 0;
	idview = FindViewByTag(env, parent, NULL, mxml, url, &ntag);
	if(idview != nil)
	{
		midview = idview;
	}
	
	idcontroller = nil;
	pclass = NULL;
	pname = NULL;
	if(mxml != NULL)
	{
		QXML pattr;
		
		pattr = (QXML)qxmlGetAttr(mxml, QXML_ATTR, (QPNT)"class", 0);
		if(pattr != NULL)
		{
			pval = qxmlGetValp(pattr);
			if(pval != NULL)
			{
				pclass = NSClassFromString([NSString stringWithUTF8String:(const char *)pval]);
				pname = pval;
			}
		}
		pattr = (QXML)qxmlGetAttr(mxml, QXML_ATTR, (QPNT)"nibname", 0);
		if(pattr != NULL)
		{
			pval = qxmlGetValp(pattr);
			if(pval != NULL)
			{
				if(pclass == nil)
				{
					pclass = NSClassFromString([NSString stringWithUTF8String:(const char *)pval]);
					pname = pval;
				}
			}
		}
		if(pclass != NULL)
		{
			if(pname == NULL)
			{
				idcontroller = [(UIViewController *)pclass initWithNibName:[NSString stringWithUTF8String:(const char *)object_getClassName(pclass)]
																	bundle:nil];
			}
			else
			{
				idcontroller = [(UIViewController *)pclass initWithNibName:[NSString stringWithUTF8String:(const char *)pname]
																	bundle:nil];
			}
		}
	}
	else if(url != NULL)
	{
		QINT nlen;
		QCHR vbuff[QSTR_BUFF_SIZE];
		
		nlen = qurlGetVar(url, (QPNT)"class", vbuff, sizeof(vbuff));
		if(nlen > 0)
		{
			pclass = NSClassFromString([NSString stringWithUTF8String:(const char *)vbuff]);
		}
		nlen = qurlGetVar(url, (QPNT)"nibname", vbuff, sizeof(vbuff));
		if(nlen > 0)
		{
			pname = vbuff;
		}
		if(pclass != NULL)
		{
			if(pname == NULL)
			{
				idcontroller = [(UIViewController *)pclass initWithNibName:[NSString stringWithUTF8String:(const char *)object_getClassName(pclass)]
																	bundle:nil];
			}
			else
			{
				idcontroller = [(UIViewController *)pclass initWithNibName:[NSString stringWithUTF8String:(const char *)pname]
																	bundle:nil];
			}
		}
	}
	if(idcontroller != nil)
	{
		if(midcontroller == nil)
		{
			midcontroller = idcontroller;
		}
		else
		{
			[midcontroller addChildViewController:(UIViewController *)idcontroller];
		}
		if(midview == nil)
		{
			midview = ((UIViewController *)idcontroller).view;
		}
		else
		{
			[midview addSubview:((UIViewController *)idcontroller).view];
		}
	}
	
	return QSCN_OK;
}

QINT QUIXibInitClass(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	
	return QSCN_OK;
}

QINT QUIXibInitNibName(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	
	return QSCN_OK;
}

QINT QUIXibInitPath(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QSTR pval;
	QINT nlen;
	
	pval = (QSTR )params[1];
	nlen = (QINT )params[2];
	
	return QSCN_OK;
}

#endif
