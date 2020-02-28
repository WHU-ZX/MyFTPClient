#include "mfc.h"
MyApp app;//全局应用程序对象，有且仅有一个


BOOL MyApp::InitInstance()
{
	//创建窗口
	MyFrame* frame = new MyFrame;
	//显示和更新
	frame->ShowWindow(SW_SHOWNORMAL);//以一般风格显示
	frame->UpdateWindow();

	m_pMainWnd = frame;//指向应用程序的主窗口的指针   利用m_pMainWnd指针实现事件的自动管理
	return TRUE;
}

//分界宏
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