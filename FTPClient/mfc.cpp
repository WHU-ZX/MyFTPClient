#include "mfc.h"
MyApp app;//ȫ��Ӧ�ó���������ҽ���һ��


BOOL MyApp::InitInstance()
{
	//��������
	MyFrame* frame = new MyFrame;
	//��ʾ�͸���
	frame->ShowWindow(SW_SHOWNORMAL);//��һ������ʾ
	frame->UpdateWindow();

	m_pMainWnd = frame;//ָ��Ӧ�ó���������ڵ�ָ��   ����m_pMainWndָ��ʵ���¼����Զ�����
	return TRUE;
}

//�ֽ��
BEGIN_MESSAGE_MAP(MyFrame, CFrameWnd)
	ON_WM_LBUTTONDBLCLK()




END_MESSAGE_MAP()

MyFrame::MyFrame()
{
	Create(NULL, TEXT("FTP Client"));
}
void MyFrame::OnLButtonDblClk(UINT, CPoint point)
{
	/*TCHAR buf[1024];
	wsprintf(buf, TEXT("x = %d , y = %d"), point.x, point.y);
	MessageBox(buf);*/

	CString str;
	str.Format(TEXT("x = %d , y = %d"), point.x, point.y);
	MessageBox(str);
}