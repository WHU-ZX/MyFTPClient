
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
