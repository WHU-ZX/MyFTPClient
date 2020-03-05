
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "MyFTP.h"
#include "MainFrm.h"
#include <fstream>
#include <sstream>
#include "MyFTPView.h"
#include "../FTP_SOCKET/FTPException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_32771, &CMainFrame::OnClickConnect)
	ON_COMMAND(ID_32772, &CMainFrame::OnClickDownload)
	ON_COMMAND(ID_32773, &CMainFrame::OnClickUpload)
	ON_COMMAND(ID_32774, &CMainFrame::OnClickDelete)
	ON_COMMAND(ID_32775, &CMainFrame::OnClickNewFileFolder)
	ON_COMMAND(ID_32776, &CMainFrame::OnClickNewFile)
	ON_COMMAND(ID_32777, &CMainFrame::OnClickSearch)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//设置图标  FTP_ICON 为icon的ID
	SetClassLong(m_hWnd, GCL_HICON, (LONG)AfxGetApp()->LoadIconW(FTP_ICON));

	//设置窗口居中
	CenterWindow();

	//设置右侧标题
	SetTitle(TEXT("客户端"));

	//创建登陆窗口
	loginDlg.Create(CONNECT_DLG);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG
//自定义函数区
std::string CMainFrame::readFileIntoString(char* filename)
{
	std::ifstream ifile(filename);
	//将文件读入到ostringstream对象buf中
	std::ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
		buf.put(ch);
	//返回与流对象buf关联的字符串
	return buf.str();
}



// CMainFrame 消息处理程序


//点击“连接到服务器”后调用
void CMainFrame::OnClickConnect()
{
	loginDlg.ShowWindow(SW_SHOWNORMAL);
}

//点击“下载”后调用
void CMainFrame::OnClickDownload()
{
	CMyFTPView* pView = (CMyFTPView*)this->GetActiveView();
	HTREEITEM item = pView->m_tree.GetSelectedItem();
	CString str = pView->m_tree.GetItemText(item);
	//判断str是否是文件夹

	
}

//点击“上传”后调用
void CMainFrame::OnClickUpload()
{
	// TODO: 在此添加命令处理程序代码
}

//点击“删除”后调用
void CMainFrame::OnClickDelete()
{
	// TODO: 在此添加命令处理程序代码
}

//点击“新建文件夹”后调用
void CMainFrame::OnClickNewFileFolder()
{
	// TODO: 在此添加命令处理程序代码
}

//点击“新建文件”后调用
void CMainFrame::OnClickNewFile()
{
	// TODO: 在此添加命令处理程序代码
}

void CMainFrame::updateFileDir()
{
	CMyFTPView* pView = (CMyFTPView*)this->GetActiveView();
	//pView->m_tree.
	pView->m_tree.DeleteAllItems();
	pView->m_tree.InsertItem(L"...", 0, 0, NULL);
	
	std::vector<FileInfo> fileInfo = client->getFilesOfCurWorkDir();

	for (int i = 0; i < fileInfo.size(); i++)
	{
		CString cStrFileName(fileInfo[i].getFileName().c_str());

		if (fileInfo[i].isFolder())//是文件
		{
			pView->m_tree.InsertItem(cStrFileName, 0, 0, NULL);
			pView->isFolderMap[cStrFileName] = true;
		}
		else
		{
			pView->m_tree.InsertItem(cStrFileName, 1, 1, NULL);
			pView->isFolderMap[cStrFileName] = false;
		}
	}
}

//点击“查询目录”后调用
void CMainFrame::OnClickSearch()
{
	/*if (!connected)
	{
		MessageBox(L"请先连接到服务器!", L"Fail", MB_ICONEXCLAMATION);
		return;
	}*/
	try
	{
		loginDlg.Connect();
		updateFileDir();
	}
	catch (FTPException e)
	{
		CString cStr = CString(e.printInfo().c_str());
		MessageBox(cStr, L"Error", MB_ICONEXCLAMATION);
	}
}
