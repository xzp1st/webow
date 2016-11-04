
#include "qmdl_config.h"

#ifdef QMDL_VER_TPF

#include "qmdl_tpf.h"

#define WM_MAIN_POST                         (WM_USER+0x1001)
#define WM_MAIN_PUSH                         (WM_USER+0x1002)
#define WM_MAIN_POP                          (WM_USER+0x1003)
#define WM_MAIN_SHOW                         (WM_USER+0x1004)
#define WM_MAIN_HIDE                         (WM_USER+0x1005)

static QPMK qpnwMainModule;


// -- QMain

LRESULT CALLBACK QMainWndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    QMain *pmain = (QMain*)::GetWindowLong(wnd, GWL_USERDATA);
    if(pmain != NULL)
    {
        if(msg == WM_MAIN_POST)
        {
            QMDL pmodule = (QMDL)wParam;
            QPNT *ppprms = (QPNT*)lParam;
            if(pmodule != NULL)
            {
                QINT nresult = pmodule->RouteCb(NULL, (QSTR)ppprms[0], (QINT)ppprms[1], (QPNT*)ppprms[2], (QINT)ppprms[3]);
                if(nresult == QNO_TRUE)
                {
                    pmodule->DispatchEventx((QSTR)ppprms[0], (QINT)ppprms[1], (QPNT*)ppprms[2], (QINT)ppprms[3]);
                }
            }
        }
        else if(msg == WM_MAIN_PUSH)
        {

        }
        else if(msg == WM_MAIN_POP)
        {
            /*QUIWindow *pwindow;
            QMDL pdstmdl, pmodule, pprev;

            pmodule = pmain->FindModule(NULL, 0, (QSTR)QPATH_STK, 0);
            if(pmodule != NULL)
            {
                pmodule = pmodule->TailModule();
            }
            pdstmdl = dynamic_cast<QMDL>((QMDL)lParam);
            pwindow = dynamic_cast<QUIWindow*>(pmodule);
            while(pwindow)
            {
                pprev = pwindow->PrevModule();
                // 广播模块退栈释放消息。
                pwindow->PopWindow();
                if(pwindow == pdstmdl)
                {
                    // 多级退栈。
                    break;
                }
                if(pdstmdl == NULL)
                {
                    // 单级退栈。
                    break;
                }
                pwindow = dynamic_cast<QUIWindow*>(pprev);
            }*/
        }
    }

    return DefWindowProc(wnd, msg, wParam, lParam);
}

void QMain::InitWndproc()
{
    mpwndproc = NULL;
    mhwnd = CreateWindow(
        _T("static"),					// window class name
        _T("QMainWnd"),					// window caption
        NULL,							// window style
        CW_USEDEFAULT,					// initial x position
        CW_USEDEFAULT,					// initial y position
        CW_USEDEFAULT,					// initial x size
        CW_USEDEFAULT,					// initial y size
        NULL,							// parent window handle
        NULL,							// window menu handle
        NULL,							// program instance handle
        NULL); 						    // creation parameters
    if(mhwnd != NULL)
    {
        BOOL bresult = FALSE;
        ::SetLastError(0);
        mpwndproc = (WNDPROC)::SetWindowLong(mhwnd, GWL_WNDPROC, (LONG)QMainWndProc);
        if(mpwndproc != NULL || ::GetLastError() == 0)
        {
            ::SetLastError(0);
            if (::SetWindowLong(mhwnd, GWL_USERDATA, (LONG)this) || ::GetLastError() == 0)
            {
                bresult = TRUE;
                ShowWindow(mhwnd, SW_HIDE);
                UpdateWindow(mhwnd);
            }
        }
        if(bresult == FALSE)
        {
            ::DestroyWindow(mhwnd);
            mhwnd = NULL;
        }
    }
}

void QMain::ExitWndproc()
{
    if(mhwnd != NULL)
    {
        ::SetWindowLong(mhwnd, GWL_WNDPROC, (LONG)mpwndproc);
        ::DestroyWindow(mhwnd);
        mhwnd = NULL;
    }
    mpwndproc = NULL;
}

QMain::QMain()
{
	main = this;

    InitTypes();
    InitWndproc();
}

QMain::~QMain()
{
    ExitWndproc();
    ExitTypes();
}

QINT QMain::MakeModuleBegin(QMDL env, QMDL parent, QXML mxml, QSTR url)
{
	return QSCN_OK;
}

QINT QMainInitApplication(QHDL hdl, QPNT name, QINT code, QPNT params[], QINT count)
{
	return QNO_OK;
}

QINT QMain::PostEvent(QMDL module, QSTR name, QINT code, QPNT params[], QINT count)
{
	if(mhwnd == NULL)
	{
		return QNO_FAIL;
	}
    QSTR pname = (QSTR)qstrdup(NULL, name);
    QHDL ppprms = qprmmake(NULL, NULL, 0, NULL, NULL, NULL, pname, code, params, qnull);
    ::PostMessage(mhwnd, WM_MAIN_POST, (WPARAM)module, (LPARAM)ppprms);
	
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
    ::PostMessage(mhwnd, WM_MAIN_POST, (WPARAM)0, (LPARAM)module);
	
	return QNO_OK;
}

QINT QMain::PopModule(QMDL module)
{
    ::PostMessage(mhwnd, WM_MAIN_POP, (WPARAM)0, (LPARAM)module);
	
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
	return 0;
};

QINT QMain::GetScreenRect(QRECT *rect)
{
	if(rect == NULL)
	{
		return QNO_FAIL;
	}
    RECT rcdesktop;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rcdesktop, 0);
	rect->left = rcdesktop.left;
	rect->top = rcdesktop.top;
	rect->right = rcdesktop.right;
	rect->bottom = rcdesktop.bottom;
	
	return QNO_OK;
};

QINT qmdlMain(int argc, char * argv[], QPMK main)
{
	qpnwMainModule = main;

    return QNO_OK;
}


#endif
