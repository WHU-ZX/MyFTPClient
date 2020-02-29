// CLoginDlg.cpp: 实现文件
//

#include "pch.h"
#include "MyFTP.h"
#include "CLoginDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <sstream>
#include "Afxinet.h"

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
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CLoginDlg::OnBnClickedButton1)
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
	CInternetSession* pSession;     //定义会话对象指针变量
	CFtpConnection* pConnection;   //定义连接对象指针变量
	CFtpFileFind* pFileFind;          //定义文件查询对象指针变量
	CString strFileName;
	BOOL bContinue;

	pConnection = NULL;      //初始化
	pFileFind = NULL;

	UpdateData(TRUE);      // 获得用户的当前输入（服务器名，用户名和口令）

	pSession = new CInternetSession(          // 创建Internet会话类对象
		AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);

	try
	{  // 试图建立与指定FTP服务器的连接
		pConnection =
			pSession->GetFtpConnection(IP_str, username_str, pwd_str, (INTERNET_PORT)22U);
	}
	catch (CInternetException * e) {
		e->Delete();                          // 无法建立连接，进行错误处理   12002

		pConnection = NULL;
	}

	if (pConnection != NULL)
	{
		// 创建CFtpFileFind对象，向构造函数传递CFtpConnection对象的指针
		pFileFind = new CFtpFileFind(pConnection);

		bContinue = pFileFind->FindFile(_T("*"));  // 查找服务器上当前目录的任意文件
		if (!bContinue)   // 如果一个文件都找不到，结束查找
		{

			pFileFind->Close();
			pFileFind = NULL;
		}

		while (bContinue)  // 找到了第一个文件，继续找其它的文件
		{
			bContinue = pFileFind->FindNextFile();
			strFileName = pFileFind->GetFileName();  // 获得找到的文件的文件名
			// 如果找到的是否目录，将目录名放在括弧中
			if (pFileFind->IsDirectory())  strFileName = _T("[") + strFileName + _T("]");
			// 将找到的文件或目录名显示在列表框中。
			//m_listFile.AddString(strFileName);
			//对文件或目录名进行处理

		}

		if (pFileFind != NULL)
		{
			pFileFind->Close();   // 结束查询
			pFileFind = NULL;
		}
	}
	delete pFileFind;              // 删除文件查询对象
	if (pConnection != NULL)
	{
		pConnection->Close();
		delete pConnection;      // 删除FTP连接对象
	}
	delete pSession;             // 删除Internet 会话对象 
}






// CLoginDlg 消息处理程序

//点击“”按钮后调用此函数，用于连接服务器，获取文件列表
void CLoginDlg::OnBnClickedButton1()
{
	//保存登陆信息
	UpdateData(TRUE);
	if (IP_str.IsEmpty() || username_str.IsEmpty() || pwd_str.IsEmpty())
	{
		//弹出消息框

		return;
	}
	reWriteFile("./login.ini");

	//连接操作，获取文件列表
	Connect();
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
