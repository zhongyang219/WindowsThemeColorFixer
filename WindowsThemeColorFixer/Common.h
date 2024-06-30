#pragma once
class CCommon
{
public:
	CCommon();
	~CCommon();

	static COLORREF GetWindowsThemeColor();
    static bool SetWindowsThemeColor(COLORREF color);

    //当前是否启动“从我的背景自动选取一种主题色”
    static bool IsAutoColor();

    static COLORREF DWColorToCOLORREF(DWORD dwColor);
    static DWORD COLORREFToDWColor(COLORREF color);

    static bool IsSystemLightTheme();
    static bool IsAppLightTheme();

    static bool IsColorSimilar(COLORREF color1, COLORREF color2);

protected:
    static DWORD GetRegisgerDWordValue(LPCTSTR path, LPCTSTR item_name);
};
