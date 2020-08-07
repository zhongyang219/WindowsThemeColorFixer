//用于获取和设置Windows主题颜色
//来自：https://www.codeproject.com/articles/610909/changing-windows-aero-color

#pragma once
#include <string>

struct COLORIZATIONPARAMS
{
	COLORREF         clrColor;          //ColorizationColor
	COLORREF         clrAftGlow;	   //ColorizationAfterglow
	UINT             nIntensity;	   //ColorizationColorBalance -> 0-100
	UINT             clrAftGlowBal;    //ColorizationAfterglowBalance
	UINT		 clrBlurBal;       //ColorizationBlurBalance
	UINT		 clrGlassReflInt;  //ColorizationGlassReflectionIntensity
	BOOL             fOpaque;
};

//这里通过未公开的API设置Windows的主题颜色
class CDwmColorLibrary
{
	typedef HRESULT(WINAPI *_DwmIsCompositionEnabled)(BOOL *pfEnabled);
	typedef HRESULT(WINAPI *_DwmSetColorizationParameters) (COLORIZATIONPARAMS *colorparam, UINT unknown);
	typedef HRESULT(WINAPI *_DwmGetColorizationParameters) (COLORIZATIONPARAMS *colorparam);

public:
	CDwmColorLibrary();
	~CDwmColorLibrary();
	bool IsSuccessed() const;		//判断DLL中的函数是否获取成功
	COLORREF GetWindowsThemeColor() const;
	void SetWindowsThemeColor(COLORREF color);

protected:
	void Init(const std::wstring& dll_path);		//载入DLL文件并获取函数入口
	void UnInit();

	_DwmIsCompositionEnabled DwmIsCompositionEnabled;
	_DwmGetColorizationParameters DwmGetColorizationParameters;
	_DwmSetColorizationParameters DwmSetColorizationParameters;

protected:
	HMODULE m_dll_module;
	bool m_succeed{ false };
	mutable COLORIZATIONPARAMS m_para;
};

