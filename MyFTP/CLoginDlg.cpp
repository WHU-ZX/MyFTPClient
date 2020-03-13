// CLoginDlg.cpp: 实现文件
//

#include "pch.h"
#include "MyFTP.h"
#include "CLoginDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <sstream>
#include "Afxinet.h"
#include "MyFTP.h"
#include "MainFrm.h"
#include "../FTP_SOCKET/FTPException.h"

#define FTP_CTL_PORT (21)
// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CONNECT_DLG, pParent)
	, IP_str(_T(""))
	, username_str(_T(""))
	, pwd_str(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
	if (this->data) delete this->data;
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, IP_str);
	DDX_Text(pDX, IDC_EDIT2, username_str);
	DDX_Text(pDX, IDC_EDIT3, pwd_str);
	DDX_Control(pDX, IDC_BUTTON1, connectStateText);
	DDX_Control(pDX, IDC_BUTTON2, disconnectBtn);
	DDX_Control(pDX, IDC_CONNECT_TEXT, cStateText);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CLoginDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLoginDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

// 自定义函数区
std::string CLoginDlg::readFileIntoString(char* filename)
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

void CLoginDlg::reWriteFile(char* filename)
{
	std::ofstream outfile;
	std::string ip = CT2A(IP_str), username = CT2A(username_str), pwd = CT2A(pwd_str);

	outfile.open(filename, std::ios_base::out | std::ios_base::trunc);  //删除文件重写
	std::stringstream ss_ip, ss_un, ss_pwd;
	ss_ip << ip; outfile << ss_ip.str() << std::endl;
	ss_un << username; outfile << ss_un.str() << std::endl;
	ss_pwd << pwd; outfile << ss_pwd.str();

	outfile.close();
}

void CLoginDlg::Connect()
{
	CMainFrame* frame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);      // 获得用户的当前输入（服务器名，用户名和口令）
	int port = FTP_CTL_PORT;
	std::string host = CT2A(IP_str.GetBuffer());
	std::string username = CT2A(username_str.GetBuffer());
	std::string pwd = CT2A(pwd_str.GetBuffer());
	try
	{
		if (frame->client == nullptr)//曾今没有连接过
		{
			frame->client = new FTPClient(host,port);
			frame->client->Login(username, pwd);
			frame->client->EnterPasvMode();
		}
		else
		{
			//清理上次连接的东西
			delete frame->client;
			//创建新Client
			frame->client = new FTPClient(host, port);
			frame->client->Login(username, pwd);
			frame->client->EnterPasvMode();
		}
		frame->connected = true;
		//MessageBox(L"连接成功!", L"Success", MB_ICONEXCLAMATION);
		cStateText.SetWindowTextW(L"已连接");
		disconnectBtn.EnableWindow(TRUE);
		connectStateText.EnableWindow(FALSE);
	}
	catch (FTPException e)
	{
		CString cStr = CString(e.printInfo().c_str());
		MessageBox(cStr, L"Error", MB_ICONEXCLAMATION);
	}
}

// CLoginDlg 消息处理程序

//点击“连接”按钮后调用此函数，用于连接服务器，获取文件列表
void CLoginDlg::OnBnClickedButton1()
{
	//保存登陆信息
	UpdateData(TRUE);
	if (IP_str.IsEmpty() || username_str.IsEmpty() || pwd_str.IsEmpty())
	{
		//弹出消息框
		MessageBox(L"输入不能为空！", L"警告", MB_ICONEXCLAMATION);
		return;
	}
	reWriteFile("./login.ini");

	//连接操作，获取文件列表
	Connect();
	MessageBox(L"连接成功！", L"Success", MB_ICONEXCLAMATION);
}

//重写，删除OnOK函数，否则文本框点击回车会退出
void CLoginDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	//CDialogEx::OnOK();
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	std::string str = readFileIntoString("./login.ini");
	std::string ip = "", username = "", pwd = "";
	int state = 0;
	for (int i = 0; i < str.length(); i++)
	{
		switch (str[i])
		{
		case '\n':
			state++;
			break;
		default:
			switch (state)
			{
			case 0:
				ip += str[i];
				break;
			case 1:
				username += str[i];
				break;
			case 2:
				pwd += str[i];
				break;
			}
			break;
		}
	}
	if (!(ip.empty() || username.empty() || pwd.empty()))
	{
		if (this->data)
		{
			delete this->data;
			this->data = nullptr;
		}
		this->data = new LoginData(ip, username, pwd);
		this->IP_str = data->ip;
		this->username_str = data->username;
		this->pwd_str = data->pwd;
		UpdateData(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//点击”断开连接“执行
void CLoginDlg::OnBnClickedButton2()
{
	CMainFrame* frame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	frame->client->disConnect();
	disconnectBtn.EnableWindow(FALSE);
	connectStateText.EnableWindow(TRUE);
	cStateText.SetWindowTextW(L"未连接");
	frame->connected = FALSE;
	MessageBox(L"成功断开连接！", L"OK", MB_ICONEXCLAMATION);
}
