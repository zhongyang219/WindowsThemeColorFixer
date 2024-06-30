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
    CRegKey key;
    if (key.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\DWM")) == ERROR_SUCCESS)
    {
        DWORD accent_color{ 0xff000000 | color };
        if (key.SetDWORDValue(_T("AccentColor"), accent_color) == ERROR_SUCCESS)
            return true;
    }
    return false;
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

static bool ColorValueSimilar(BYTE val1, BYTE val2)
{
    return std::abs(val1 - val2) < 16;
}

bool CCommon::IsColorSimilar(COLORREF color1, COLORREF color2)
{
    BYTE r1 = GetRValue(color1);
    BYTE g1 = GetGValue(color1);
    BYTE b1 = GetBValue(color1);
    BYTE r2 = GetRValue(color2);
    BYTE g2 = GetGValue(color2);
    BYTE b2 = GetBValue(color2);

    return ColorValueSimilar(r1, r2) && ColorValueSimilar(g1, g2) && ColorValueSimilar(b1, b2);
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
