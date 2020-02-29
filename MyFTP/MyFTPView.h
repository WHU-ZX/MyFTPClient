
// MyFTPView.h: CMyFTPView 类的接口
//

#pragma once


class CMyFTPView : public CFormView
{
protected: // 仅从序列化创建
	CMyFTPView() noexcept;
	DECLARE_DYNCREATE(CMyFTPView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_MYFTP_FORM };
#endif

// 特性
public:
	CMyFTPDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMyFTPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MyFTPView.cpp 中的调试版本
inline CMyFTPDoc* CMyFTPView::GetDocument() const
   { return reinterpret_cast<CMyFTPDoc*>(m_pDocument); }
#endif

