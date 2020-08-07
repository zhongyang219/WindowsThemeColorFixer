
// WindowsThemeColorFixer.cpp: 定义应用程序的类行为。
//

#include "stdafx.h"
#include "WindowsThemeColorFixer.h"
#include "WindowsThemeColorFixerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWindowsThemeColorFixerApp

BEGIN_MESSAGE_MAP(CWindowsThemeColorFixerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CWindowsThemeColorFixerApp 构造

CWindowsThemeColorFixerApp::CWindowsThemeColorFixerApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CWindowsThemeColorFixerApp 对象

CWindowsThemeColorFixerApp theApp;


// CWindowsThemeColorFixerApp 初始化

BOOL CWindowsThemeColorFixerApp::InitInstance()
{
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);
    m_module_path = path;
    if (m_module_path.find(L' ') != std::wstring::npos)
    {
        //如果路径中有空格，则在程序路径前后添加双引号
        m_module_path_reg = L'\"';
        m_module_path_reg += m_module_path;
        m_module_path_reg += L'\"';
    }
    else
    {
        m_module_path_reg = m_module_path;
    }

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("Apps By ZhongYang"));

	CWindowsThemeColorFixerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}


void CWindowsThemeColorFixerApp::SetAutoRun(bool auto_run)
{
    CRegKey key;
    //打开注册表项
    if (key.Open(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run")) != ERROR_SUCCESS)
    {
        return;
    }
    if (auto_run)		//写入注册表项
    {
        if (key.SetStringValue(APP_NAME, m_module_path_reg.c_str()) != ERROR_SUCCESS)
        {
            return;
        }
    }
    else		//删除注册表项
    {
        //删除前先检查注册表项是否存在，如果不存在，则直接返回
        wchar_t buff[256];
        ULONG size{ 256 };
        if (key.QueryStringValue(APP_NAME, buff, &size) != ERROR_SUCCESS)
            return;
        if (key.DeleteValue(APP_NAME) != ERROR_SUCCESS)
        {
            return;
        }
    }
}

bool CWindowsThemeColorFixerApp::GetAutoRun()
{
    CRegKey key;
    if (key.Open(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run")) != ERROR_SUCCESS)
    {
        //打开注册表“Software\\Microsoft\\Windows\\CurrentVersion\\Run”失败，则返回false
        return false;
    }
    wchar_t buff[256];
    ULONG size{ 256 };
    if (key.QueryStringValue(APP_NAME, buff, &size) == ERROR_SUCCESS)		//如果找到了APP_NAME键
    {
        return (m_module_path_reg == buff);	//如果APP_NAME的值是当前程序的路径，就返回true，否则返回false
    }
    else
    {
        return false;		//没有找到APP_NAME键，返回false
    }
}
