
#include "qmdl_module.h"


QModule::QModule():
	main(NULL),
	mnFlag(0),
    mpName(NULL),
	mpPath(NULL),
	mnRefer(0),
	mpcbMake(NULL),
	mpUserData(NULL),
	mpcbSelf(NULL),
	mpcbUser(NULL),
	mhAttrLink(NULL),
	mhMthdLink(NULL),
	mhObsrLink(NULL),
	mhInitLink(NULL),
	mpParentModule(NULL),
	mpPrevModule(NULL),
	mpNextModule(NULL),
	mpHeadModule(NULL),
	mpTailModule(NULL),
	mnInitDepth(0)
{
}

QModule::QModule(QSTR name, QINT size):
	main(NULL),
	mnFlag(0),
	mpName(NULL),
    mpPath(NULL),
    mnRefer(0),
    mpcbMake(NULL),
	mpUserData(NULL),
	mpcbSelf(NULL),
	mpcbUser(NULL),
	mhAttrLink(NULL),
	mhMthdLink(NULL),
	mhObsrLink(NULL),
	mhInitLink(NULL),
	mpParentModule(NULL),
    mpPrevModule(NULL),
    mpNextModule(NULL),
    mpHeadModule(NULL),
    mpTailModule(NULL),
	mnInitDepth(0)
{
    QINT nlen;
    
    if(size > 0)
    {
        nlen = size;
    }
    else
    {
        nlen = qstrlen(name);
    }
    if(nlen <= 0)
    {
        mpName = NULL;
    }
    else
    {
        mpName = (QSTR )qmalloc(NULL, nlen+1);
        if(mpName != NULL)
        {
            qstrcpy(mpName, nlen+1, name, nlen);
        }
    }
}

QModule::~QModule()
{
	if(mpName != NULL)
	{
		qmfree(mpName);
		mpName = NULL;
	}
	if(mpPath != NULL)
	{
		qmfree(mpPath);
		mpPath = NULL;
	}
	ClrAttrLink();
    ClrObsrLink();
}

QINT QModule::GetFlag()
{
	return mnFlag;
}

void QModule::SetFlag(QINT flag)
{
	mnFlag = flag;
}

QSTR QModule::GetName()
{
	return mpName;
}

void QModule::SetName(QSTR name)
{
	if(mpName != NULL)
	{
		qmfree(mpName);
	}
	mpName = (QSTR )qstrdup(NULL, name);
}

QSTR QModule::GetPath()
{
	return mpPath;
}

void QModule::SetPath(QSTR path)
{
	if(mpPath != NULL)
	{
		qmfree(mpPath);
	}
	mpPath = (QSTR )qstrdup(NULL, path);
}

void QModule::SetMakeCb(QPMK new_cb)
{
	mpcbMake = new_cb;
}

QPMK QModule::GetMakeCb()
{
	return mpcbMake;
}

QPNT QModule::GetSelfData()
{
	return NULL;
}

void QModule::SetSelfData(QPNT selfdata)
{
}

QPNT QModule::GetUserData()
{
	return mpUserData;
}

void QModule::SetUserData(QPNT userdata)
{
	mpUserData = userdata;
}

QMDL QModule::MainModule()
{
	return main;
}

QMDL QModule::ParentModule()
{
	return mpParentModule;
}

QMDL QModule::PrevModule()
{
	return mpPrevModule;
}

QMDL QModule::NextModule()
{
	return mpNextModule;
}

QMDL QModule::HeadModule()
{
	return mpHeadModule;
}

QMDL QModule::TailModule()
{
	return mpTailModule;
}

QMDL QModule::AddParentModule(QMDL child, QMDL parent)
{
	QModule *pparent, *pmodule, *pchild;
	
	pparent = (QModule *)parent;
	if(pparent == NULL)
	{
		return NULL;
	}
	pchild = (QModule *)child;
	if(pchild == NULL)
	{
		return pparent;
	}
	if(pchild->mpParentModule == NULL)
	{
		pparent->mpHeadModule = pchild;
		pparent->mpTailModule = pchild;
		pchild->mpParentModule = pparent;
	}
	else
	{
		pparent->mpParentModule = pchild->mpParentModule;
		pparent->mpHeadModule = ((QModule *)pchild->mpParentModule)->mpHeadModule;
		pmodule = (QModule *)((QModule *)pchild->mpParentModule)->mpHeadModule;
		while(pmodule)
		{
			pmodule->mpParentModule = pparent;
			if(pmodule->mpNextModule == NULL)
			{
				break;
			}
			pmodule = (QModule *)pmodule->mpNextModule;
		}
		pparent->mpTailModule = (QMDL)pmodule;
		((QModule *)pchild->mpParentModule)->mpHeadModule = pparent;
		((QModule *)pchild->mpParentModule)->mpTailModule = pparent;
	}
	// 返回树根。
	while(pparent)
	{
		if(pparent->mpParentModule == NULL)
		{
			break;
		}
		pparent = (QModule *)pparent->mpParentModule;
	}
	
	return (QMDL)pparent;
}

QMDL QModule::AddChildModule(QMDL parent, QMDL child)
{
	QModule *pparent, *pmodule, *pchild;
	
	pchild = (QModule *)child;
	if(pchild == NULL)
	{
		return NULL;
	}
	pparent = (QModule *)parent;
	if(pparent == NULL)
	{
		return (QMDL)pchild;
	}
	if(pparent->mpHeadModule == NULL)
	{
		pchild->mpParentModule = pparent;
		pparent->mpHeadModule = pchild;
		pparent->mpTailModule = pchild;
	}
	else
	{
		pchild->mpParentModule = pparent;
		pchild->mpHeadModule = pparent->mpHeadModule;
		pmodule = (QModule *)pparent->mpHeadModule;
		while(pmodule)
		{
			pmodule->mpParentModule = pchild;
			if(pmodule->mpNextModule == NULL)
			{
				break;
			}
			pmodule = (QModule *)pmodule->mpNextModule;
		}
		pchild->mpTailModule = pmodule;
		pparent->mpHeadModule = pchild;
		pparent->mpTailModule = pchild;
	}
	// 返回树根。
	while(pparent)
	{
		if(pparent->mpParentModule == NULL)
		{
			break;
		}
		pparent = (QModule *)pparent->mpParentModule;
	}
	
	return (QMDL)pparent;
}

QMDL QModule::AddPrevModule(QMDL parent, QMDL base, QMDL next)
{
	QModule *pparent, *pprev, *pbase, *pmodule;
	
	pmodule = (QModule *)next;
	if(pmodule == NULL)
	{
		return NULL;
	}
	pparent = (QModule *)parent;
	pbase = (QModule *)base;
	if(pparent == NULL)
	{
		if(pbase == NULL)
		{
			// 独立成树。
			return (QMDL)pmodule;
		}
		if(pbase->mpParentModule == NULL)
		{
			// 根结点不能插入兄弟。
			return (QMDL)pmodule;
		}
		pparent = (QModule *)pbase->mpParentModule;
	}
	else
	{
		if(pbase != NULL)
		{
			if(pbase->mpParentModule != pparent)
			{
				// 基结点不对应。
				pbase = NULL;
			}
		}
	}
	if(pparent->mpHeadModule == NULL)
	{
		// 插入第一个孩子。
		pmodule->mpParentModule = pparent;
		pparent->mpHeadModule = pmodule;
		pparent->mpTailModule = pmodule;
	}
	else
	{
		if(pbase == NULL)
		{
			// 插入到最前面。
			pmodule->mpParentModule = pparent;
			pmodule->mpNextModule = pparent->mpHeadModule;
			pmodule->mpPrevModule = NULL;
			((QModule *)pparent->mpHeadModule)->mpPrevModule = pmodule;
			pparent->mpHeadModule = pmodule;
		}
		else
		{
			// 找到基结点的前一结点。
			if(pbase == pparent->mpHeadModule)
			{
				pprev = NULL;
			}
			else
			{
				pprev = (QModule *)pbase->mpPrevModule;
			}
			if(pprev == NULL)
			{
				// 插入到最前面。
				pmodule->mpParentModule = pparent;
				pmodule->mpNextModule = pbase;
				pmodule->mpPrevModule = NULL;
				pbase->mpPrevModule = pmodule;
				pparent->mpHeadModule = pmodule;
			}
			else
			{
				// 插入到某一结点之前。
				pmodule->mpParentModule = pparent;
				pmodule->mpNextModule = pbase;
				pmodule->mpPrevModule = pprev;
				pprev->mpNextModule = pmodule;
				pbase->mpPrevModule = pmodule;
			}
		}
	}
	// 返回树根。
	while(pparent)
	{
		if(pparent->mpParentModule == NULL)
		{
			break;
		}
		pparent = (QModule *)pparent->mpParentModule;
	}
	
	return (QMDL)pparent;
}

QMDL QModule::AddNextModule(QMDL parent, QMDL base, QMDL prev)
{
	QModule *pparent, *ptail, *pbase, *pmodule;
	
	pmodule = (QModule *)prev;
	if(pmodule == NULL)
	{
		return NULL;
	}
	pparent = (QModule *)parent;
	pbase = (QModule *)base;
	if(pparent == NULL)
	{
		if(pbase == NULL)
		{
			// 独立成树。
			return (QMDL)pmodule;
		}
		if(pbase->mpParentModule == NULL)
		{
			// 根结点不能插入兄弟。
			return (QMDL)pmodule;
		}
		pparent = (QModule *)pbase->mpParentModule;
	}
	else
	{
		if(pbase != NULL)
		{
			if(pbase->mpParentModule != pparent)
			{
				// 基结点不对应。
				pbase = NULL;
			}
		}
	}
	if(pparent->mpHeadModule == NULL)
	{
		// 插入第一个孩子。
		pmodule->mpParentModule = pparent;
		pparent->mpHeadModule = pmodule;
		pparent->mpTailModule = pmodule;
	}
	else
	{
		if(pbase == NULL)
		{
			// 插入到最后面。
			ptail = (QModule *)pparent->mpTailModule;
			pmodule->mpParentModule = pparent;
			pmodule->mpNextModule = NULL;
			pmodule->mpPrevModule = ptail;
			ptail->mpNextModule = pmodule;
			pparent->mpTailModule = pmodule;
		}
		else
		{
			if(pbase->mpNextModule == NULL)
			{
				// 插入到最后面。
				pmodule->mpParentModule = pparent;
				pmodule->mpNextModule = NULL;
				pmodule->mpPrevModule = pbase;
				pbase->mpNextModule = pmodule;
				pparent->mpTailModule = pmodule;
			}
			else
			{
				// 插入到某一结点之前。
				pmodule->mpParentModule = pparent;
				pmodule->mpNextModule = pbase->mpNextModule;
				pmodule->mpPrevModule = pbase;
				((QModule *)pbase->mpNextModule)->mpPrevModule = pmodule;
				pbase->mpNextModule = pmodule;
			}
		}
	}
	// 返回树根。
	while(pparent)
	{
		if(pparent->mpParentModule == NULL)
		{
			break;
		}
		pparent = (QModule *)pparent->mpParentModule;
	}
	
	return (QMDL)pparent;
}

QMDL QModule::RemoveModule(QMDL module)
{
	QModule *pmodule;

	if(module == NULL)
	{
		pmodule = this;
	}
	else
	{
		pmodule = (QModule *)module;
	}
	if(pmodule->mpParentModule != NULL)
	{
		if(pmodule->mpPrevModule == NULL)
		{
			if(pmodule->mpNextModule == NULL)
			{
				((QModule *)pmodule->mpParentModule)->mpHeadModule = NULL;
				((QModule *)pmodule->mpParentModule)->mpTailModule = NULL;
			}
			else
			{
				((QModule *)pmodule->mpNextModule)->mpPrevModule = NULL;
				((QModule *)pmodule->mpParentModule)->mpHeadModule = pmodule->mpNextModule;
			}
		}
		else
		{
			if(pmodule->mpNextModule == NULL)
			{
				((QModule *)pmodule->mpPrevModule)->mpNextModule = NULL;
				((QModule *)pmodule->mpParentModule)->mpTailModule = pmodule->mpPrevModule;
			}
			else
			{
				((QModule *)pmodule->mpPrevModule)->mpNextModule = pmodule->mpNextModule;
				((QModule *)pmodule->mpNextModule)->mpPrevModule = pmodule->mpPrevModule;
			}
		}
	}
	pmodule->mpParentModule = NULL;
	pmodule->mpPrevModule = NULL;
	pmodule->mpNextModule = NULL;
	
	return pmodule;
}

QMDL QModule::RegisterModule(QSTR path, QPMK new_cb)
{
	QINT nlen, nflag;
	QMDL pmodule, pchild;
	QSTR ppath, pname, pnext, pchildname;

	if(path == NULL)
	{
		SetMakeCb(new_cb);
		return this;
	}
	ppath = path;
	if(*ppath == '/' || *ppath == '\\')
	{
		pmodule = RootModule();
	}
	else
	{
		pmodule = (QMDL )this;
	}
	while(1)
	{
		pname = ppath;
		while(*pname != 0)
		{
			if(*pname != '/' && *pname != '\\')
			{
				break;
			}
			pname++;
		}
		nlen = 0;
		pnext = pname;
		while(*pnext != 0)
		{
			if(*pnext == '/' || *pnext == '\\')
			{
				break;
			}
			pnext++;
			nlen++;
		}
		if(nlen <= 0)
		{
			break;
		}
		pchild = pmodule->HeadModule();
		while(pchild)
		{
			pchildname = pchild->GetName();
			if(pchildname != NULL && qstrcmp(QSTR_NONE, pchildname, pname, nlen))
			{
				break;
			}
			pchild = pchild->NextModule();
		}
		if(pchild == NULL)
		{
			QModule *pchild = new QModule(pname, nlen);
			if(pchild == NULL)
			{
				return NULL;
			}
			AddNextModule((QModule *)pmodule, NULL, (QModule *)pchild);
		}
		pmodule = pchild;
		ppath = pnext;
	}
	pmodule->SetMakeCb(new_cb);
	nflag = pmodule->GetFlag();
	nflag |= QFLAG_CLAXX;
	pmodule->SetFlag(nflag);

	return pmodule;
}

QMDL QModule::RegisterModule(QSTR path, QMDL module)
{
	QINT nlen;
	QSTR ppath, pname, pnext, pchildname;
	QMDL pmodule, pchild;

	if(module == NULL)
	{
		return NULL;
	}
	if(module->ParentModule())
	{
		return NULL;
	}
	if(path == NULL)
	{
		AddNextModule(this, NULL, (QModule *)module);
		return module;
	}
	ppath = path;
	if(*ppath == '/' || *ppath == '\\')
	{
		pmodule = RootModule();
	}
	else
	{
		pmodule = (QMDL )this;
	}
	if(pmodule == module)
	{
		return module;
	}
	while(1)
	{
		pname = ppath;
		while(*pname != 0)
		{
			if(*pname != '/' && *pname != '\\')
			{
				break;
			}
			pname++;
		}
		nlen = 0;
		pnext = pname;
		while(*pnext != 0)
		{
			if(*pnext == '?')
			{
				break;
			}
			if(*pnext == '/' || *pnext == '\\')
			{
				break;
			}
			pnext++;
			nlen++;
		}
		if(nlen <= 0)
		{
			break;
		}
		if(*pnext == 0)
		{
			break;
		}
		pchild = pmodule->HeadModule();
		while(pchild)
		{
			pchildname = pchild->GetName();
			if(pchildname != NULL && qstrcmp(QSTR_NONE, pchildname, pname, nlen))
			{
				break;
			}
			pchild = pchild->NextModule();
		}
		if(pchild != NULL)
		{
			pmodule = pchild;
		}
		else
		{
			QModule *pchild = new QModule(pname, nlen);
			if(pchild == NULL)
			{
				return NULL;
			}
			AddNextModule((QModule *)pmodule, NULL, (QModule *)pchild);
			pmodule = pchild;
		}
		ppath = pnext;
	}
	if(nlen > 0)
	{
		if(pname != NULL)
		{
			module->SetName(pname);
		}
	}
	AddNextModule((QModule *)pmodule, NULL, (QModule *)module);
	if(module->MainModule() == NULL)
	{
		((QModule *)module)->main = MainModule();
	}

	return module;
}

static QINT qmdl_module_delete_post_cb(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	QPNT vpprms[8];
	QModule *pmodule;
	QINT nresult, ncount;
	
	pmodule = (QModule *)hdl;
	if(pmodule == NULL)
	{
		return QSCN_END;
	}
	ncount = 0;
	vpprms[ncount++] = (QPNT)pmodule;
	nresult = pmodule->ModuleCb(NULL, (QINT)QCD_FREE, vpprms, ncount);
	if(nresult != QSCN_FAIL && nresult != QSCN_ERR && nresult != QSCN_END)
	{
		pmodule->DispatchEventx(NULL, (QINT)QCD_FREE, vpprms, ncount);
	}
	pmodule->RemoveModule(NULL);
	delete pmodule;

	return QSCN_OK;
}

void QModule::UnregisterModule(QSTR path, QMDL module)
{
	QModule *pmodule;

	if(path != NULL)
	{
		pmodule = (QModule *)FindModule(path, 0, NULL, 0);
	}
	else if(module != NULL)
	{
		pmodule = (QModule *)module;
	}
	else
	{
		pmodule = this;
	}
	if(pmodule == NULL)
	{
		return;
	}
	ScanModulex(pmodule, NULL, qmdl_module_delete_post_cb, NULL, 0);
}

QINT QModule::ReferModule(QINT count)
{
	QINT ncount;
	QModule *pmodule;
	
	if(count <= 0)
	{
		ncount = 1;
	}
	else
	{
		ncount = count;
	}
	pmodule = this;
	while(pmodule)
	{
		pmodule->mnRefer += ncount;
		pmodule = (QModule *)pmodule->mpParentModule;
	}
	
	return ncount;
}

QINT QModule::UnreferModule(QINT count)
{
	QINT ncount;
	QModule *pmodule, *pparent;
	
	if(count < 0)
	{
		ncount = this->mnRefer;
	}
	else if(count == 0)
	{
		ncount = 1;
	}
	else
	{
		ncount = count;
	}
	pmodule = this;
	while(pmodule)
	{
		pparent = (QModule *)pmodule->mpParentModule;
		pmodule->mnRefer -= ncount;
		if(pmodule->mnRefer <= 0)
		{
			ScanModulex(pmodule, NULL, qmdl_module_delete_post_cb, NULL, 0);
		}
		pmodule = pparent;
	}
	
	return ncount;
}

QINT QModule::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	return QSCN_OK;
}

QINT QModule::MakeModuleInit(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	return QNO_TRUE;
}

QINT QModule::MakeModuleEnd(QMDL env, QXML mxml, QSTR url)
{
	return QSCN_OK;
}

