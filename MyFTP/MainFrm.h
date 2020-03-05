
// MainFrm.h: CMainFrame 类的接口
//
#include "../FTP_SOCKET/FTPClient.h"
#pragma once
#include "CLoginDlg.h"
#include "Afxinet.h"

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar        m_wndStatusBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

private:
	CLoginDlg loginDlg;
	std::string readFileIntoString(char* filename);


public:
	afx_msg void OnClickConnect();
	afx_msg void OnClickDownload();
	afx_msg void OnClickUpload();
	afx_msg void OnClickDelete();
	afx_msg void OnClickNewFileFolder();
	afx_msg void OnClickNewFile();

public:
	//CInternetSession* pSession = NULL;     //定义会话对象指针变量
	//CFtpConnection* pConnection= NULL;   //定义连接对象指针变量
	//CFtpFileFind* pFileFind = NULL;          //定义文件查询对象指针变量

	void updateFileDir();
	FTPClient* client = nullptr;
	afx_msg void OnClickSearch();
	BOOL connected = FALSE;
};


