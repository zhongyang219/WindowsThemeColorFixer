
// WindowsThemeColorFixerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "WindowsThemeColorFixer.h"
#include "WindowsThemeColorFixerDlg.h"
#include "afxdialogex.h"
#include "Common.h"
#include "ColorConvert.h"
#include <strsafe.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    CString version_info;
    GetDlgItemText(IDC_STATIC_VERSION, version_info);
    version_info.Replace(_T("<version>"), VERSION);
#ifdef _M_X64
    version_info += _T(" (x64)");
#endif

#ifdef _DEBUG
    version_info += _T(" (Debug)");
#endif

    SetDlgItemText(IDC_STATIC_VERSION, version_info);

    //设置最后编译日期
    CString temp_str;
    GetDlgItemText(IDC_STATIC_COPYRIGHT, temp_str);
    temp_str.Replace(_T("<compile_date>"), COMPILE_DATE);
    CString year{ COMPILE_DATE };
    year = year.Left(4);
    temp_str.Replace(_T("<year>"), year);
    SetDlgItemText(IDC_STATIC_COPYRIGHT, temp_str);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


// CWindowsThemeColorFixerDlg 对话框

const UINT WM_TASKBARCREATED{ ::RegisterWindowMessage(_T("TaskbarCreated")) };	//注册任务栏建立的消息

CWindowsThemeColorFixerDlg::CWindowsThemeColorFixerDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_WINDOWSTHEMECOLORFIXER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWindowsThemeColorFixerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWindowsThemeColorFixerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_REDUCE_COLOR_CHECK, &CWindowsThemeColorFixerDlg::OnBnClickedReduceColorCheck)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CWindowsThemeColorFixerDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BUTTON2, &CWindowsThemeColorFixerDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_ADJUST_COLOR_BUTTON, &CWindowsThemeColorFixerDlg::OnBnClickedAdjustColorButton)
    ON_COMMAND(ID_SETTINGS, &CWindowsThemeColorFixerDlg::OnSettings)
    ON_MESSAGE(WM_NOTIFYICON, &CWindowsThemeColorFixerDlg::OnNotifyicon)
    ON_COMMAND(ID_APP_ABOUT, &CWindowsThemeColorFixerDlg::OnAppAbout)
    ON_WM_CLOSE()
    ON_COMMAND(ID_MENU_EXIT, &CWindowsThemeColorFixerDlg::OnMenuExit)
    ON_WM_DESTROY()
    ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, &CWindowsThemeColorFixerDlg::OnTaskbarcreated)
    ON_WM_DWMCOLORIZATIONCOLORCHANGED()
    ON_BN_CLICKED(IDC_HIDE_MAIN_WINDOW_CHECK, &CWindowsThemeColorFixerDlg::OnBnClickedHideMainWindowCheck)
    ON_WM_QUERYENDSESSION()
    ON_BN_CLICKED(IDC_AUTO_RUN_CHECK, &CWindowsThemeColorFixerDlg::OnBnClickedAutoRunCheck)
    ON_BN_CLICKED(IDC_ADJUST_ONLY_LIGHT_MODE_CHECK, &CWindowsThemeColorFixerDlg::OnBnClickedAdjustOnlyLightModeCheck)
END_MESSAGE_MAP()


// CWindowsThemeColorFixerDlg 消息处理程序

void CWindowsThemeColorFixerDlg::LoadConfig()
{
    m_auto_adjust_color = (theApp.GetProfileInt(_T("config"), _T("auto_adjust_color"), 1) != 0);
    m_hide_main_window_when_start = (theApp.GetProfileInt(_T("config"), _T("hide_main_window_when_start"), 0) != 0);
    m_adjust_only_light_mode = (theApp.GetProfileInt(_T("config"), _T("adjust_only_light_mode"), 0) != 0);
}

void CWindowsThemeColorFixerDlg::SaveConfig() const
{
    theApp.WriteProfileInt(L"config", L"auto_adjust_color", m_auto_adjust_color);
    theApp.WriteProfileInt(L"config", L"hide_main_window_when_start", m_hide_main_window_when_start);
    theApp.WriteProfileInt(L"config", L"adjust_only_light_mode", m_adjust_only_light_mode);
}

bool CWindowsThemeColorFixerDlg::AdjustWindowsThemeColor()
{
    if (m_adjust_only_light_mode && !CCommon::IsAppLightTheme())
        return false;

    COLORREF color = CCommon::GetWindowsThemeColor();
    if (CColorConvert::IncreaseLuminance(color))
    {
        CCommon::SetWindowsThemeColor(color);
        return true;
    }

    return false;
}

void CWindowsThemeColorFixerDlg::SetOpaque(int opaque)
{
    SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(0, opaque * 255 / 100, LWA_ALPHA);  //透明度取值范围为0~255
}

BOOL CWindowsThemeColorFixerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

    LoadConfig();

    //初始化控件
	((CButton*)GetDlgItem(IDC_REDUCE_COLOR_CHECK))->SetCheck(m_auto_adjust_color);
	((CButton*)GetDlgItem(IDC_HIDE_MAIN_WINDOW_CHECK))->SetCheck(m_hide_main_window_when_start);
	((CButton*)GetDlgItem(IDC_AUTO_RUN_CHECK))->SetCheck(theApp.GetAutoRun());
    CheckDlgButton(IDC_ADJUST_ONLY_LIGHT_MODE_CHECK, m_adjust_only_light_mode);
#ifndef _DEBUG
    GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
#endif

    m_menu.LoadMenu(IDR_MENU1);

	SetTimer(TIMER_ID, 1000, NULL);

    m_ntIcon.cbSize = sizeof(NOTIFYICONDATA);	//该结构体变量的大小
    m_ntIcon.hIcon = m_hIcon;
    m_ntIcon.hWnd = m_hWnd;				//接收托盘图标通知消息的窗口句柄
    CString atip{ APP_NAME };			//鼠标指向图标时显示的提示
#ifdef _DEBUG
    atip += _T(" (Debug)");
#endif
    StringCchCopy(m_ntIcon.szTip, 128, atip.GetString());
    m_ntIcon.uCallbackMessage = WM_NOTIFYICON;	//应用程序定义的消息ID号
    m_ntIcon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;	//图标的属性：设置成员uCallbackMessage、hIcon、szTip有效
    ::Shell_NotifyIcon(NIM_ADD, &m_ntIcon);	//在系统通知区域增加这个图标

    if (m_auto_adjust_color)
        AdjustWindowsThemeColor();

    if (m_hide_main_window_when_start)
        SetOpaque(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWindowsThemeColorFixerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWindowsThemeColorFixerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWindowsThemeColorFixerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWindowsThemeColorFixerDlg::OnBnClickedReduceColorCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	m_auto_adjust_color = (((CButton*)GetDlgItem(IDC_REDUCE_COLOR_CHECK))->GetCheck() != 0);
}


void CWindowsThemeColorFixerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == TIMER_ID)
	{
        if (m_first_start)
        {
            m_first_start = false;
            if (m_hide_main_window_when_start)	//设置隐藏主窗口
                ShowWindow(SW_HIDE);
            SetOpaque(100);				//重新设置窗口不透明度
        }
		if (m_auto_adjust_color)
		{
            //AdjustWindowsThemeColor();
		}
	}

    if (nIDEvent == TIMER_ID_ADJUST_COLOR)
    {
        AdjustWindowsThemeColor();
        KillTimer(TIMER_ID_ADJUST_COLOR);
    }

	CDialog::OnTimer(nIDEvent);
}


void CWindowsThemeColorFixerDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
    bool is_auto_color = CCommon::IsAutoColor();
}


//void CWindowsThemeColorFixerDlg::OnBnClickedButton2()
//{
//	// TODO: 在此添加控件通知处理程序代码
//    COLORREF color{ RGB(35, 172, 57) };
//	m_themeColorLib.SetWindowsThemeColor(color);
//    //CCommon::SetAccentColor(color);
//}


void CWindowsThemeColorFixerDlg::OnBnClickedAdjustColorButton()
{
    // TODO: 在此添加控件通知处理程序代码
    AdjustWindowsThemeColor();
}


void CWindowsThemeColorFixerDlg::OnSettings()
{
    // TODO: 在此添加命令处理程序代码
    ShowWindow(SW_RESTORE);
    SetForegroundWindow();
}


afx_msg LRESULT CWindowsThemeColorFixerDlg::OnNotifyicon(WPARAM wParam, LPARAM lParam)
{
    if (lParam == WM_RBUTTONUP)
    {
        //在通知区点击右键弹出右键菜单
        SetForegroundWindow();
        CPoint point1;	//定义一个用于确定光标位置的位置  
        GetCursorPos(&point1);	//获取当前光标的位置，以便使得菜单可以跟随光标
        m_menu.GetSubMenu(0)->SetDefaultItem(ID_SETTINGS, FALSE);		//设置默认菜单项 
        m_menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this); //在指定位置显示弹出菜单
    }
    if (lParam == WM_LBUTTONDBLCLK)
    {
        ShowWindow(SW_RESTORE);
        SetForegroundWindow();
    }
    return 0;
}


void CWindowsThemeColorFixerDlg::OnAppAbout()
{
    // TODO: 在此添加命令处理程序代码
    CAboutDlg dlg;
    dlg.DoModal();
}


void CWindowsThemeColorFixerDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    ShowWindow(HIDE_WINDOW);
    //CDialog::OnClose();
}


void CWindowsThemeColorFixerDlg::OnMenuExit()
{
    // TODO: 在此添加命令处理程序代码
    OnCancel();
}


void CWindowsThemeColorFixerDlg::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
    //程序退出时删除通知栏图标
    ::Shell_NotifyIcon(NIM_DELETE, &m_ntIcon);

    SaveConfig();
}


afx_msg LRESULT CWindowsThemeColorFixerDlg::OnTaskbarcreated(WPARAM wParam, LPARAM lParam)
{
    //重新添加通知栏图标
    ::Shell_NotifyIcon(NIM_ADD, &m_ntIcon);
    return 0;
}


void CWindowsThemeColorFixerDlg::OnColorizationColorChanged(DWORD dwColorizationColor, BOOL bOpacity)
{
    // 此功能要求 Windows Vista 或更高版本。
    // _WIN32_WINNT 符号必须 >= 0x0600。
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (m_auto_adjust_color)
    {
        SetTimer(TIMER_ID_ADJUST_COLOR, 100, NULL);     //由于系统主题色改变时，会多次收到此消息，因此延时一定时间再更改主题色
    }

    CDialog::OnColorizationColorChanged(dwColorizationColor, bOpacity);
}


void CWindowsThemeColorFixerDlg::OnBnClickedHideMainWindowCheck()
{
    // TODO: 在此添加控件通知处理程序代码
    m_hide_main_window_when_start = (((CButton*)GetDlgItem(IDC_HIDE_MAIN_WINDOW_CHECK))->GetCheck() != 0);
}


BOOL CWindowsThemeColorFixerDlg::OnQueryEndSession()
{
    if (!CDialog::OnQueryEndSession())
        return FALSE;

    // TODO:  在此添加专用的查询结束会话代码
    SaveConfig();

    return TRUE;
}


void CWindowsThemeColorFixerDlg::OnBnClickedAutoRunCheck()
{
    // TODO: 在此添加控件通知处理程序代码
    bool auto_run = (((CButton*)GetDlgItem(IDC_AUTO_RUN_CHECK))->GetCheck() != 0);
    theApp.SetAutoRun(auto_run);
}


void CWindowsThemeColorFixerDlg::OnBnClickedAdjustOnlyLightModeCheck()
{
    // TODO: 在此添加控件通知处理程序代码
    m_adjust_only_light_mode = (IsDlgButtonChecked(IDC_ADJUST_ONLY_LIGHT_MODE_CHECK) != 0);
}
