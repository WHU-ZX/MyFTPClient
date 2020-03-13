#pragma once


// CGetNameDlg 对话框

class CGetNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGetNameDlg)

public:
	CGetNameDlg(CString& str, bool& clickedOk,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGetNameDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = GETNAME_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	bool * pClickedOk = nullptr;
	CString* pStr = nullptr;
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	//bool ok();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CString newFileNameStr;

	//void reset();
};
