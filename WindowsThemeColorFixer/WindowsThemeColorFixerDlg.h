
// WindowsThemeColorFixerDlg.h: 头文件
//

#pragma once
#include "DwmColorLibrary.h"

// CWindowsThemeColorFixerDlg 对话框
class CWindowsThemeColorFixerDlg : public CDialog
{
// 构造
public:
	CWindowsThemeColorFixerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDOWSTHEMECOLORFIXER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	bool m_auto_adjust_color{ true };
    bool m_waiting_for_adjust_color{ false };
    bool m_hide_main_window_when_start{ false };
    bool m_adjust_only_light_mode{ false };
    bool m_enhanced_mode{ false };
    CDwmColorLibrary m_dwm_lib;

    NOTIFYICONDATA m_ntIcon;	//通知区域图标
    CMenu m_menu;

    bool m_first_start{ true };		//初始时为true，在定时器第一次启动后置为flase

    void LoadConfig();
    void SaveConfig() const;

    bool AdjustWindowsThemeColor();
    void SetOpaque(int opaque);     //设置不透明度
    void StartAdjustWindosThemeColor();

    COLORREF GetThemeColor();
    void SetThemeColor(COLORREF color);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReduceColorCheck();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
//	afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedAdjustColorButton();
    afx_msg void OnSettings();
protected:
    afx_msg LRESULT OnNotifyicon(WPARAM wParam, LPARAM lParam);
public:
    afx_msg void OnAppAbout();
    afx_msg void OnClose();
    afx_msg void OnMenuExit();
    afx_msg void OnDestroy();
protected:
    afx_msg LRESULT OnTaskbarcreated(WPARAM wParam, LPARAM lParam);
public:
    afx_msg void OnColorizationColorChanged(DWORD dwColorizationColor, BOOL bOpacity);
    afx_msg void OnBnClickedHideMainWindowCheck();
    afx_msg BOOL OnQueryEndSession();
    afx_msg void OnBnClickedAutoRunCheck();
    afx_msg void OnBnClickedAdjustOnlyLightModeCheck();
    afx_msg void OnBnClickedEnhancedCheck();
};
