// CGetNameDlg.cpp: 实现文件
//

#include "pch.h"
#include "MyFTP.h"
#include "CGetNameDlg.h"
#include "afxdialogex.h"


// CGetNameDlg 对话框

IMPLEMENT_DYNAMIC(CGetNameDlg, CDialogEx)

CGetNameDlg::CGetNameDlg(CString& str, bool& clickedOk,CWnd* pParent /*=nullptr*/)
	: CDialogEx(GETNAME_DLG, pParent)
	, newFileNameStr(_T(""))
{
	str = L"";
	clickedOk = false;
	this->pStr = &str;
	this->pClickedOk = &clickedOk;
}

CGetNameDlg::~CGetNameDlg()
{
}

void CGetNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, newFileNameStr);
}

//bool CGetNameDlg::ok()
//{
//	UpdateData(TRUE);
//	return this->clickedOk;
//}

//void CGetNameDlg::reset()
//{
//	this->newFileNameStr = L"";
//	UpdateData(FALSE);
//	this->clickedOk = false;
//}

BEGIN_MESSAGE_MAP(CGetNameDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CGetNameDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CGetNameDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CGetNameDlg 消息处理程序


void CGetNameDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);
	*(this->pStr) = this->newFileNameStr;
	*(this->pClickedOk) = true;
	CDialogEx::OnOK();
}

//点击确定
void CGetNameDlg::OnBnClickedButton1()
{
	OnOK();
}

//点击取消
void CGetNameDlg::OnBnClickedButton2()
{
	CDialogEx::OnCancel();
}
