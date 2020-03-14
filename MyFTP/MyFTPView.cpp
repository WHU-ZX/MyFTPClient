
// MyFTPView.cpp: CMyFTPView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MyFTP.h"
#endif

#include "MyFTPDoc.h"
#include "MyFTPView.h"
#include "Afxinet.h"
#include "MainFrm.h"
#include "../FTP_SOCKET/FTPException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyFTPView

IMPLEMENT_DYNCREATE(CMyFTPView, CFormView)

BEGIN_MESSAGE_MAP(CMyFTPView, CFormView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CMyFTPView::OnNMDblclkTree1)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CMyFTPView 构造/析构

CMyFTPView::CMyFTPView() noexcept
	: CFormView(IDD_MYFTP_FORM)
{
	// TODO: 在此处添加构造代码


	
	
}

CMyFTPView::~CMyFTPView()
{
}

void CMyFTPView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Control(pDX, PATH_TEXT, path_text);
}

BOOL CMyFTPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CMyFTPView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	//初始化字体
	m_textFont.CreatePointFont(150, L"楷体");
	path_text.SetFont(&m_textFont);
	//初始化TreeControl
	initTree();
}

//执行实际的下载任务
//BOOL CMyFTPView::Download(CString strSName, CString strDName)
//{
//	CMainFrame* frame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
//	
//	return TRUE;
//}

//执行实际的上传任务
BOOL CMyFTPView::Upload(CString strSName, CString strDName)
{
	CMainFrame* frame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if (!frame->connected)
	{
		MessageBox(L"请连接到服务器！", L"Error", MB_ICONEXCLAMATION);
		return FALSE;
	}
	
	return TRUE;
}

//初始化TreeControl
void CMyFTPView::initTree()
{
	//准备HICON图标
	HICON icons[2];
	icons[0] = AfxGetApp()->LoadIconW(FILEFOLDER_ICON);
	icons[1] = AfxGetApp()->LoadIconW(FILE_ICON);

	//创建图片集合
	list.Create(20, 20, ILC_COLOR32, 2, 2);
	//添加具体图片
	list.Add(icons[0]);
	list.Add(icons[1]);

	m_tree.SetImageList(&list, TVSIL_NORMAL);
	//m_tree.InsertItem(L"1", 0, 0, NULL);
}

void CMyFTPView::setWorkSpace(CString workspace)//将工作路径设置为workspace
{

}

void CMyFTPView::returnToParentDir() //进入上层目录
{
	
}

void CMyFTPView::enterDir(CString dir)//进入dir  会调用setWorkSpace函数
{
	

	//如果是一个文件，则提示：暂时不支持打开文件功能

}

// CMyFTPView 打印

BOOL CMyFTPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMyFTPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMyFTPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMyFTPView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 在此处添加自定义打印代码
}


// CMyFTPView 诊断

#ifdef _DEBUG
void CMyFTPView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMyFTPView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMyFTPDoc* CMyFTPView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyFTPDoc)));
	return (CMyFTPDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyFTPView 消息处理程序



//双击树控件的item时调用的函数
void CMyFTPView::OnNMDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMainFrame* frame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	*pResult = 0;
	HTREEITEM item = m_tree.GetSelectedItem();
	CString str = m_tree.GetItemText(item);
	bool isFileFolder = isFolderMap[str];
	if (str == L"...")//点击了上级目录
	{
		try
		{
			frame->returnToParentDir();
			frame->updateFileDir();
		}
		catch(FTPException e)
		{
			CString cStr = CString(e.printInfo().c_str());
			MessageBox(cStr, L"Error", MB_ICONEXCLAMATION);
		}
		//MessageBox(L"双击上级目录", L"Test", MB_ICONEXCLAMATION);
	}
	else if (isFileFolder)//是文件夹
	{
		std::string folderName = CT2A(str.GetBuffer());
		frame->client->enterFolder(folderName);
		frame->updateFileDir();

		//MessageBox(L"双击了文件夹" + str, L"Test", MB_ICONEXCLAMATION);
	}
	else
	{
		MessageBox(L"暂不提供文件预览功能", L"Sorry", MB_ICONEXCLAMATION);
	}
}


HBRUSH CMyFTPView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	if (pWnd->GetDlgCtrlID() == PATH_TEXT)
	{
		pDC->SetTextColor(RGB(0, 0, 255));//设置字体颜色
		//pDC->SetBkColor(RGB(0, 255, 0));//设置背景颜色
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
