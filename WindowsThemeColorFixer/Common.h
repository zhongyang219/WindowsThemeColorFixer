#pragma once
class CCommon
{
public:
	CCommon();
	~CCommon();

	static COLORREF GetWindowsThemeColor();
	//static void SetWindowsThemeColor(COLORREF color);

    //����Windows��AccentColor��ע�⣬AccentColor��ֵΪAABBGGRR����COLORREF���ͼ���
    static bool SetAccentColor(COLORREF color);

    ////����Windows��ColorizationColor��ColorizationColor��ֵΪAARRGGBB����COLORREF���͵�R��B�෴
    //static bool SetColorizationColor(COLORREF color);

    //static bool SetColorizationAfterglow(COLORREF color);

    //��ǰ�Ƿ����������ҵı����Զ�ѡȡһ������ɫ��
    static bool IsAutoColor();

    static COLORREF DWColorToCOLORREF(DWORD dwColor);
    static DWORD COLORREFToDWColor(COLORREF color);
};

