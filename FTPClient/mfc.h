#pragma once
#include <afxwin.h>

class MyApp :public CWinApp //Ӧ�ó�����
{
public:
	//�������
	virtual BOOL InitInstance();

};

class MyFrame :public CFrameWnd //���ڿ����

{
public:
	MyFrame();

	//�����꣬�ṩ��Ϣӳ�����
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDblClk(UINT, CPoint);

};