
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "MyFTP.h"
#include "MainFrm.h"
#include <fstream>
#include <sstream>
#include "MyFTPView.h"


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
	ON_COMMAND(ID_32777, &CMainFrame::OnClickSearch)
	ON_COMMAND(ID_32779, &CMainFrame::OnClickRename)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

void CMainFrame::showExceptionByMessageBox(FTPException e)
{
	CString cStr = CString(e.printInfo().c_str());
	MessageBox(cStr, L"Error", MB_ICONEXCLAMATION);
}

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
	//getNameDlg.Create(GETNAME_DLG);

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
	if (!connected)
	{
		MessageBox(L"请先连接到服务器!", L"Fail", MB_ICONEXCLAMATION);
		return;
	}
	CMyFTPView* pView = (CMyFTPView*)this->GetActiveView();
	HTREEITEM item = pView->m_tree.GetSelectedItem();
	CString str = pView->m_tree.GetItemText(item);
	//判断str是否是文件夹
	if (pView->isFolderMap[str])//如果是文件夹
	{
		MessageBox(L"暂时不提供文件夹下载功能!", L"Fail", MB_ICONEXCLAMATION);
	}
	else
	{
		//选择的是文件
		CString strDestName;
		CFileDialog dlg(FALSE); //定义了一个文件对话框对象变量

		if (dlg.DoModal() == IDOK)        //激活文件对话框
		{
			//获得下载文件在本地机上存储的路径和名称
			strDestName = dlg.GetPathName();
			std::string des = CT2A(strDestName.GetBuffer());
			std::string src = CT2A(str.GetBuffer());
			//调用函数下载文件
			//client->Download(des, src);
			try
			{
				client->DownloadWithPos(des, src);
				MessageBox(L"下载完成!", L"Success", MB_ICONEXCLAMATION);
			}
			catch (FTPException e)
			{
				showExceptionByMessageBox(e);
			}
		}
		else {
			MessageBox(L"请写入文件名!", L"Fail", MB_ICONEXCLAMATION);
		}
	}
}

//点击“上传”后调用
void CMainFrame::OnClickUpload()
{
	if (!connected)
	{
		MessageBox(L"请先连接到服务器!", L"Fail", MB_ICONEXCLAMATION);
		return;
	}
	CMyFTPView* pView = (CMyFTPView*)this->GetActiveView();
	std::string workSpace = client->getDir();
	CString strPathName;
	CString strFileName;
	CFileDialog dlg(TRUE); //定义了一个文件对话框对象变量
	if (dlg.DoModal() == IDOK)
	{
		strPathName = dlg.GetPathName();
		strFileName = dlg.GetFileName();
		std::string pathName = CT2A(strPathName.GetBuffer());
		std::string fileName = CT2A(strFileName.GetBuffer());
		client->Upload(pathName, fileName);
		updateFileDir();
	}
	else
	{
		MessageBox(L"请选择文件!", L"Fail", MB_ICONEXCLAMATION);
	}
}

//点击“删除”后调用
void CMainFrame::OnClickDelete()
{
	if (!connected)
	{
		MessageBox(L"请先连接到服务器!", L"Fail", MB_ICONEXCLAMATION);
		return;
	}
	CMyFTPView* pView = (CMyFTPView*)this->GetActiveView();
	HTREEITEM item = pView->m_tree.GetSelectedItem();
	CString fileStr = pView->m_tree.GetItemText(item);

	bool isFolder = pView->isFolderMap[fileStr];
	if (isFolder)//后期要修改，要支持它
	{
		MessageBox(L"暂不支持删除整个文件夹!", L"Sorry", MB_ICONEXCLAMATION);
		/*std::string fileName = CT2A(fileStr.GetBuffer());
		try
		{
			client->deleteFolderAtCurDir(fileName);
			MessageBox(L"删除成功!", L"Success", MB_ICONEXCLAMATION);
			updateFileDir();
		}
		catch (FTPException e)
		{
			showExceptionByMessageBox(e);
		}*/
	}
	else
	{
		std::string fileName = CT2A(fileStr.GetBuffer());
		try
		{
			client->deleteFileAtCurDir(fileName);
			MessageBox(L"删除成功!", L"Success", MB_ICONEXCLAMATION);
			updateFileDir();
		}
		catch (FTPException e)
		{
			showExceptionByMessageBox(e);
		}
	}
}

//点击“新建文件夹”后调用
void CMainFrame::OnClickNewFileFolder()
{
	if (!connected)
	{
		MessageBox(L"请先连接到服务器!", L"Fail", MB_ICONEXCLAMATION);
		return;
	}
	CString newFileName;
	bool clickedOk;
	CGetNameDlg getNameDlg(newFileName,clickedOk);
	getNameDlg.DoModal();
	if (clickedOk)
	{
		//CString newName = getNameDlg.newFileNameStr();
		if (newFileName.IsEmpty())
		{
			MessageBox(L"输入为空!", L"Warning", MB_ICONEXCLAMATION);
		}
		else//做真正的事
		{
			std::string fileName = CT2A(newFileName.GetBuffer());
			try 
			{
				client->createFolderAtWorkingDir(fileName);
				MessageBox(L"创建成功!", L"Success", MB_ICONEXCLAMATION);
				updateFileDir();
			}
			catch (FTPException e)
			{
				showExceptionByMessageBox(e);
			}
		}
	}
}


void CMainFrame::updateFileDir()
{
	if (!connected)
	{
		MessageBox(L"请先连接到服务器!", L"Fail", MB_ICONEXCLAMATION);
		return;
	}
	CMyFTPView* pView = (CMyFTPView*)this->GetActiveView();
	pView->isFolderMap.clear();
	//pView->m_tree.
	pView->m_tree.DeleteAllItems();
	pView->m_tree.InsertItem(L"...", 0, 0, NULL);
	
	std::vector<FileInfo> fileInfo = client->getFilesOfCurWorkDir();
	std::string workspace = client->getDir();
	this->workSpace = workspace.c_str();

	pView->path_text.SetWindowTextW(this->workSpace);

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

bool CMainFrame::returnToParentDir()
{
	if (!connected)
	{
		MessageBox(L"请先连接到服务器!", L"Fail", MB_ICONEXCLAMATION);
		return false;
	}
	return client->returnToParentDir();
}

//点击“查询目录”后调用
void CMainFrame::OnClickSearch()
{
	if (!connected)
	{
		MessageBox(L"请先连接到服务器!", L"Fail", MB_ICONEXCLAMATION);
		return;
	}
	try
	{
		//loginDlg.Connect();
		updateFileDir();
	}
	catch (FTPException e)
	{
		CString cStr = CString(e.printInfo().c_str());
		MessageBox(cStr, L"Error", MB_ICONEXCLAMATION);
	}
}

//点击“重命名”后调用
void CMainFrame::OnClickRename()
{
	if (!connected)
	{
		MessageBox(L"请先连接到服务器!", L"Fail", MB_ICONEXCLAMATION);
		return;
	}
	CMyFTPView* pView = (CMyFTPView*)this->GetActiveView();
	HTREEITEM item = pView->m_tree.GetSelectedItem();
	CString fromCStr = pView->m_tree.GetItemText(item);

	CString toCStr;
	bool clickedOk;
	CGetNameDlg getNameDlg(toCStr, clickedOk);
	getNameDlg.DoModal();
	if (clickedOk)
	{
		if (toCStr.IsEmpty())
		{
			MessageBox(L"输入为空!", L"Warning", MB_ICONEXCLAMATION);
		}
		else//做真正的事
		{
			std::string toStr = CT2A(toCStr.GetBuffer());
			std::string fromStr = CT2A(fromCStr.GetBuffer());
			try
			{
				client->rename(fromStr, toStr);
				MessageBox(L"重命名成功!", L"Success", MB_ICONEXCLAMATION);
				updateFileDir();
			}
			catch (FTPException e)
			{
				showExceptionByMessageBox(e);
			}
		}
	}
}
