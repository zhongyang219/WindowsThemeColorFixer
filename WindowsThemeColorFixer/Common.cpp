#include "stdafx.h"
#include "Common.h"


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

bool CCommon::SetAccentColor(COLORREF color)
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

//bool CCommon::SetColorizationColor(COLORREF color)
//{
//    CRegKey key;
//    if (key.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\DWM")) == ERROR_SUCCESS)
//    {
//        DWORD colorization_color{};
//        BYTE r, g, b;
//        r = GetRValue(color);
//        g = GetGValue(color);
//        b = GetBValue(color);
//
//        colorization_color = (r << 16);
//        colorization_color |= (g << 8);
//        colorization_color |= b;
//
//        if (key.SetDWORDValue(_T("ColorizationColor"), colorization_color) == ERROR_SUCCESS)
//            return true;
//    }
//    return false;
//
//}
//
//bool CCommon::SetColorizationAfterglow(COLORREF color)
//{
//    CRegKey key;
//    if (key.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\DWM")) == ERROR_SUCCESS)
//    {
//        DWORD colorization_color{};
//        BYTE r, g, b;
//        r = GetRValue(color);
//        g = GetGValue(color);
//        b = GetBValue(color);
//
//        colorization_color = (r << 16);
//        colorization_color |= (g << 8);
//        colorization_color |= b;
//        colorization_color |= 0xc4000000;
//
//        if (key.SetDWORDValue(_T("ColorizationAfterglow"), colorization_color) == ERROR_SUCCESS)
//            return true;
//    }
//    return false;
//}

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

//void CCommon::SetWindowsThemeColor(COLORREF color)
//{
//	//DWORD crColorization;
//	//BOOL fOpaqueBlend;
//	//COLORREF theme_color{};
//	//HRESULT result = DwmSetColorizationColor(&crColorization, &fOpaqueBlend);
//	//DwmSet
//
//}
