#include "stdafx.h"
#include "DwmColorLibrary.h"


CDwmColorLibrary::CDwmColorLibrary()
{
	Init(L"dwmapi.dll");
}


CDwmColorLibrary::~CDwmColorLibrary()
{
	UnInit();
}

void CDwmColorLibrary::Init(const std::wstring & dll_path)
{
	//载入DLL
	m_dll_module = ::LoadLibrary(dll_path.c_str());
	//获取函数入口
	DwmIsCompositionEnabled = (_DwmIsCompositionEnabled)::GetProcAddress(m_dll_module, "DwmIsCompositionEnabled");
	DwmGetColorizationParameters = (_DwmGetColorizationParameters)::GetProcAddress(m_dll_module, (LPCSTR)127);
	DwmSetColorizationParameters = (_DwmSetColorizationParameters)::GetProcAddress(m_dll_module, (LPCSTR)131);
	//判断是否成功
	m_succeed = true;
	m_succeed &= (m_dll_module != NULL);
	m_succeed &= (DwmIsCompositionEnabled != NULL);
	m_succeed &= (DwmGetColorizationParameters != NULL);
	m_succeed &= (DwmSetColorizationParameters != NULL);
	BOOL enable;
	DwmIsCompositionEnabled(&enable);
	m_succeed &= (enable != FALSE);
}

void CDwmColorLibrary::UnInit()
{
	if (m_dll_module != NULL)
	{
		FreeLibrary(m_dll_module);
		m_dll_module = NULL;
	}
}

bool CDwmColorLibrary::IsSuccessed() const
{
	return m_succeed;
}

COLORREF CDwmColorLibrary::GetWindowsThemeColor() const
{
	COLORREF color{};
	if (m_succeed)
	{
		DwmGetColorizationParameters(&m_para);
		BYTE r, g, b;
		r = (m_para.clrColor >> 16) % 256;
		g = (m_para.clrColor >> 8) % 256;
		b = m_para.clrColor % 256;
		color = RGB(r, g, b);
	}
	return color;
}

void CDwmColorLibrary::SetWindowsThemeColor(COLORREF color)
{
	if (m_succeed)
	{
		DWORD dwm_color = 0;
		BYTE r, g, b;
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		dwm_color = (r << 16);
		dwm_color |= (g << 8);
		dwm_color |= b;

		m_para.clrColor = dwm_color;
		DwmSetColorizationParameters(&m_para, 0);
	}
}
