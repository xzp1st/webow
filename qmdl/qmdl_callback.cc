
#include "qmdl_module.h"


QPCB QModule::GetSelfCb()
{
	return mpcbSelf;
}

void QModule::SetSelfCb(QPCB cb)
{
	mpcbSelf = cb;
}

QPCB QModule::GetUserCb()
{
	return mpcbUser;
}

void QModule::SetUserCb(QPCB cb)
{
	mpcbUser = cb;
}

QINT QModule::SelfCb(QSTR name, QINT code, QPNT params[], QINT count)
{
	if(mpcbSelf != NULL)
	{
		return mpcbSelf(this, name, code, params, count);
	}
	
	return QNO_OK;
}

QINT QModule::UserCb(QSTR name, QINT code, QPNT params[], QINT count)
{
	if(mpcbUser != NULL)
	{
		return mpcbUser(this, name, code, params, count);
	}
	
	return QNO_OK;
}

QINT QModule::ModuleCb(QSTR name, QINT code, QPNT params[], QINT count)
{
	QPNT vpprms[8];
	QINT ncount, nresult;
	
	nresult = QNO_OK;
	ncount = 0;
	vpprms[ncount++] = (QPNT )code;
	vpprms[ncount++] = (QPNT )params;
	vpprms[ncount++] = (QPNT )count;
	mnFlag &= ~QFLAG_PEEK;
	if(mpcbUser != NULL)
	{
		// 用户函数处理预备消息。
		nresult = mpcbUser(this, name, QCD_PREPARE, vpprms, ncount);
		if(mnFlag & QFLAG_PEEK)
		{
			goto CB_EXIT;
		}
	}
	if(mpcbSelf != NULL)
	{
		// 私有函数处理传入消息。
		nresult = mpcbSelf(this, name, code, params, count);
		if(mnFlag & QFLAG_PEEK)
		{
			goto CB_EXIT;
		}
		// 私有函数处理完成消息。
		nresult = mpcbSelf(this, name, QCD_FINISH, vpprms, ncount);
	}
	
CB_EXIT:
	return nresult;
}

QINT QModule::RouteCb(QMDL parent, QSTR name, QINT code, QPNT params[], QINT count)
{
	QPNT vpprms[8];
	QMDL vpmodule[QSCN_STACK_CNT];
	QMDL proot, pparent, pcur, *ppstack, *ppmodule;
	QINT nresult, nresult1, nresult2, nlevel, nmalloc, ncount, nflag, nrootflag, nthisflag;
	
	proot = this->RootModule();
	nlevel = 0;
	pparent = this;
	while(pparent)
	{
		nlevel++;
		if(pparent == parent)
		{
			break;
		}
		pparent = pparent->ParentModule();
	}
	// 创建消息模块队列(从当前模块直到根模块), 用于消息派发
	// 队列头尾均放置NULL，便于向前、向后的遍历
	nmalloc = 0;
	if(nlevel < sizeof(vpmodule)/sizeof(QMDL))
	{
		ppstack = vpmodule;
	}
	else
	{
		nmalloc = 1;
		ppstack = (QMDL*)qmalloc(this, sizeof(QMDL)*(nlevel + 2));
		if(ppstack == NULL)
		{
			return QNO_FAIL;
		}
	}
	// 队列首部放置空模块。
	ppmodule = ppstack;
	*ppmodule++ = NULL;
	pparent = this;
	while(pparent)
	{
		*ppmodule++ = pparent;
		if(pparent == parent)
		{
			break;
		}
		pparent = pparent->ParentModule();
	}
	// 队列尾部放置空模块。
	*ppmodule = NULL;
	// 从根模块开始遍历模块并派发消息，指向根模块。
	ppmodule--;
	// 获取并记录根模块当前的Peek状态，避免下面的发送消息过程中消息处理函数中存在递归调用RouteCb，
	// 导致根模块的Peek状态并清除，从而破坏消息发送的流程
	nrootflag = proot->GetFlag();
	nthisflag = this->GetFlag();
	// 清除消息遍历结束的标志
	// 子模块通过调用guiPeekMessage设置其根模块的消息遍历结束标志(~QFLAG_PEEK)标识当前处理的消息被截获
	// 消息派发遍历流程检查根模块的GUI_PEEK_FLAG标志，判断消息是否被截获，从而终止消息派发
	nflag = nrootflag & ~QFLAG_PEEK;
	proot->SetFlag(nflag);
	nflag = nthisflag & ~QFLAG_ROUTED;
	this->SetFlag(nflag);
	
	nresult = 0;
	nresult1 = 0;
	nresult2 = 0;
	ncount = 0;
	vpprms[ncount++] = (QPNT )code;
	vpprms[ncount++] = (QPNT )params;
	vpprms[ncount++] = (QPNT )count;
	// 从根模块(始祖模块)到当前模块
	while(*ppmodule)
	{
		pcur = *ppmodule--;
		// 发送预处理开始消息
		nresult1 = QSCN_OK;
		if(mpcbUser != NULL)
		{
			nresult1 = mpcbUser(pcur, NULL, QCD_PREPARE, vpprms, ncount);
		}
		nflag = proot->GetFlag();
		if(nflag & QFLAG_PEEK)
		{
			break;
		}
		// 发送消息
		nresult2 = QSCN_OK;
		if(mpcbSelf != NULL)
		{
			nresult2 = mpcbSelf(pcur, name, code, params, count);
		}
		nflag = proot->GetFlag();
		if(nflag & QFLAG_PEEK)
		{
			break;
		}
		if(nresult1 == QSCN_FAIL || nresult2 == QSCN_FAIL)
		{
			// 当前模块标识消息处理中止, 不再派发给子模块
			break;
		}
		if(nresult1 == QSCN_END || nresult2 == QSCN_END)
		{
			// 当前模块标识消息处理结束
			goto ROUTE_EXIT;
			break;
		}
	}
	//  从当前模块遍历直到根模块，发送预处理结束消息
	while(1)
	{
		// 检查消息是否已被截获(guiIsPeek) 或者 消息处理是否已结束(GTR_END)
		nflag = proot->GetFlag();
		if(nflag & QFLAG_PEEK)
		{
			break;
		}
		if(*ppmodule == NULL)
		{
			// 从队列首部的下一个模块开始(队列首部是一个空模块)
			ppmodule++;
		}
		// 从当前模块开始遍历直到根模块(始祖模块)派发预处理结束信息
		while(*ppmodule)
		{
			pcur = *ppmodule++;
			nresult1 = QSCN_OK;
			if(mpcbSelf != NULL)
			{
				nresult1 = mpcbSelf(pcur, NULL, QCD_FINISH, vpprms, ncount);
			}
			// 检查消息是否已被截获
			nflag = proot->GetFlag();
			if(nflag & QFLAG_PEEK)
			{
				break;
			}
			// 检查消息处理是否已中止或者结束
			if(nresult1 == QSCN_FAIL || nresult1 == QSCN_END)
			{
				break;
			}
		}
		break;
	}
	goto ROUTE_EXIT;
	
ROUTE_EXIT:
	// 记录当前根模块的PEEK状态，并返回给调用者处理
	nflag = proot->GetFlag();
	if(nflag & QFLAG_PEEK)
	{
		nresult = QNO_FALSE;
	}
	else
	{
		nresult = QNO_TRUE;
	}
	// 恢复根模块的PEEK状态
	if(nrootflag & QFLAG_PEEK)
	{
		nflag |= QFLAG_PEEK;
	}
	else
	{
		nflag &= ~QFLAG_PEEK;
	}
	proot->SetFlag(nflag);
	nflag = this->GetFlag();
	if(nthisflag & QFLAG_ROUTED)
	{
		nflag |= QFLAG_ROUTED;
	}
	else
	{
		nflag &= QFLAG_ROUTED;
	}
	this->SetFlag(nflag);
	// 释放消息模块队列
	if(nmalloc == 1)
	{
		qmfree(ppstack);
	}
	
	return nresult;
}
