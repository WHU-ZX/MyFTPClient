#pragma once
#include "LoginData.h"

// CLoginDlg 对话框

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLoginDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = CONNECT_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString IP_str;
	CString username_str;
	CString pwd_str;
	void Connect();
	afx_msg void OnBnClickedButton1();
	

private:
	LoginData* data = nullptr;
	std::string readFileIntoString(char* filename);
	void reWriteFile(char* filename);
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
