#include "stdafx.h"
#include "Common.h"
#include "WindowsThemeColorApi.h"


CCommon::CCommon()
{
}


CCommon::~CCommon()
{
}

COLORREF CCommon::GetWindowsThemeColor()
{
	DWORD crColorization;
	BOOL fOpaqueBlend;
	COLORREF theme_color{};
	HRESULT result = DwmGetColorizationColor(&crColorization, &fOpaqueBlend);
	if (result == S_OK)
	{
		BYTE r, g, b;
		r = (crColorization >> 16) % 256;
		g = (crColorization >> 8) % 256;
		b = crColorization % 256;
		theme_color = RGB(r, g, b);
	}
	return theme_color;
}

bool CCommon::SetWindowsThemeColor(COLORREF color)
{
    //CRegKey key;
    //if (key.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\DWM")) == ERROR_SUCCESS)
    //{
    //    DWORD accent_color{ 0xff000000 | color };
    //    if (key.SetDWORDValue(_T("AccentColor"), accent_color) == ERROR_SUCCESS)
    //        return true;
    //}
    //return false;
    WindowsThemeColorApi::SetDwmColorizationColor(color);
    WindowsThemeColorApi::SetAccentColor(color);
    return true;
}


bool CCommon::IsAutoColor()
{
    CRegKey key;
    if (key.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\History")) == ERROR_SUCCESS)
    {
        DWORD auto_color{};
        if (key.QueryDWORDValue(_T("AutoColor"), auto_color) == ERROR_SUCCESS)
            return (auto_color != 0);
    }
    return false;
}

COLORREF CCommon::DWColorToCOLORREF(DWORD dwColor)
{
    BYTE r, g, b;
    r = (dwColor >> 16) % 256;
    g = (dwColor >> 8) % 256;
    b = dwColor % 256;
    return RGB(r, g, b);
}

DWORD CCommon::COLORREFToDWColor(COLORREF color)
{
    DWORD dw_color = 0;
    BYTE r, g, b;
    r = GetRValue(color);
    g = GetGValue(color);
    b = GetBValue(color);

    dw_color = (r << 16);
    dw_color |= (g << 8);
    dw_color |= b;
    return dw_color;
}

bool CCommon::IsSystemLightTheme()
{
    return GetRegisgerDWordValue(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"), _T("SystemUsesLightTheme")) != 0;
 }

bool CCommon::IsAppLightTheme()
{
    return GetRegisgerDWordValue(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"), _T("AppsUseLightTheme")) != 0;
}

DWORD CCommon::GetRegisgerDWordValue(LPCTSTR path, LPCTSTR item_name)
{
    CRegKey key;
    if (key.Open(HKEY_CURRENT_USER, path) == ERROR_SUCCESS)
    {
        DWORD data{};
        if (key.QueryDWORDValue(item_name, data) == ERROR_SUCCESS)
            return data;
    }
    return 0;

}
