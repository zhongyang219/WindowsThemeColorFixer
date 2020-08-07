#pragma once
class CCommon
{
public:
	CCommon();
	~CCommon();

	static COLORREF GetWindowsThemeColor();
	//static void SetWindowsThemeColor(COLORREF color);

    //设置Windows的AccentColor，注意，AccentColor的值为AABBGGRR，和COLORREF类型兼容
    static bool SetAccentColor(COLORREF color);

    ////设置Windows的ColorizationColor，ColorizationColor的值为AARRGGBB，和COLORREF类型的R和B相反
    //static bool SetColorizationColor(COLORREF color);

    //static bool SetColorizationAfterglow(COLORREF color);

    //当前是否启动“从我的背景自动选取一种主题色”
    static bool IsAutoColor();

    static COLORREF DWColorToCOLORREF(DWORD dwColor);
    static DWORD COLORREFToDWColor(COLORREF color);
};

