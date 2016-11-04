
#include "qmdl_base.h"


QINT qmdlPost(QMDL module, QSTR name, QINT code, QPNT params[], QINT count)
{
	QMDL pmodule;
	QBaseMain *pmain;
	
	if(module == NULL)
	{
		return QNO_FAIL;
	}
	pmodule = module->MainModule();
	if(pmodule == NULL)
	{
		return QNO_FAIL;
	}
	pmain = dynamic_cast<QBaseMain *>(pmodule);
	if(pmain == NULL)
	{
		return QNO_FAIL;
	}
	
	return pmain->PostEvent(module, name, code, params, count);
}

QINT qmdlSend(QMDL module, QSTR name, QINT code, QPNT params[], QINT count)
{
	QMDL pmodule;
	QBaseMain *pmain;
	
	if(module == NULL)
	{
		return QNO_FAIL;
	}
	pmodule = module->MainModule();
	if(pmodule == NULL)
	{
		return QNO_FAIL;
	}
	pmain = dynamic_cast<QBaseMain *>(pmodule);
	if(pmain == NULL)
	{
		return QNO_FAIL;
	}
	
	return pmain->SendEvent(module, name, code, params, count);
}

QINT qmdlChange(QMDL env, QMDL module)
{
	QMDL pmodule;
	QBaseMain *pmain;
	
	if(env == NULL)
	{
		return QNO_FAIL;
	}
	pmodule = env->MainModule();
	if(pmodule == NULL)
	{
		return QNO_FAIL;
	}
	pmain = dynamic_cast<QBaseMain *>(pmodule);
	if(pmain == NULL)
	{
		return QNO_FAIL;
	}
	pmain->PopModule(env);
	
	return pmain->PushModule(module);
}

QINT qmdlPush(QMDL env, QMDL module)
{
	QMDL pmodule;
	QBaseMain *pmain;
	
	if(env == NULL)
	{
		return QNO_FAIL;
	}
	pmodule = env->MainModule();
	if(pmodule == NULL)
	{
		return QNO_FAIL;
	}
	pmain = dynamic_cast<QBaseMain *>(pmodule);
	if(pmain == NULL)
	{
		return QNO_FAIL;
	}
	
	return pmain->PushModule(module);
}

QINT qmdlPop(QMDL env)
{
	QMDL pmodule;
	QBaseMain *pmain;
	
	if(env == NULL)
	{
		return QNO_FAIL;
	}
	pmodule = env->MainModule();
	if(pmodule == NULL)
	{
		return QNO_FAIL;
	}
	pmain = dynamic_cast<QBaseMain *>(pmodule);
	if(pmain == NULL)
	{
		return QNO_FAIL;
	}
	
	return pmain->PopModule(env);
}

QINT qmdlLoadString(QMDL env, QSTR file, QSTR name, QSTR value, QINT size)
{
	QMDL pmodule;
	QBaseMain *pmain;
	
	if(env == NULL)
	{
		return QNO_FAIL;
	}
	pmodule = env->MainModule();
	if(pmodule == NULL)
	{
		return QNO_FAIL;
	}
	pmain = dynamic_cast<QBaseMain *>(pmodule);
	if(pmain == NULL)
	{
		return QNO_FAIL;
	}
	
	return pmain->LoadString(file, name, value, size);
}

QINT qmdlGetScreenRect(QMDL env, QRECT *rect)
{
	QMDL pmodule;
	QBaseMain *pmain;
	
	if(env == NULL)
	{
		return QNO_FAIL;
	}
	pmodule = env->MainModule();
	if(pmodule == NULL)
	{
		return QNO_FAIL;
	}
	pmain = dynamic_cast<QBaseMain *>(pmodule);
	if(pmain == NULL)
	{
		return QNO_FAIL;
	}
	
	return pmain->GetScreenRect(rect);
}
