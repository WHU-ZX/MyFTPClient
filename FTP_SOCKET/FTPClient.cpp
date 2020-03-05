#include "FTPClient.h"
#include <WinSock2.h>
#include "FTPException.h"
#pragma comment(lib,"ws2_32.lib")

#define MAX_BUF (5000)

FTPClient::FTPClient(const std::string& host, int port)//����MySokcet����������
{
	//this->logger = Logger(host, port);


	//�ж϶˿ں��Ƿ����Ҫ��?

	this->host = host;
	this->port = port;

	try
	{
		WSAStart();
	}
	catch (FTPException e)
	{
		throw e;
	}
	sock_data = MySocket();
	sock_ctl = MySocket();
	sock_ctl.SetAddrInfo(host, port);

	if (!sock_ctl.Create())
	{
		//CleanUp();
		throw FTPException(ExType::CreateSocketFail);
	}
	if (!sock_ctl.Connect())
	{
		//CleanUp();
		throw FTPException(ExType::ConnectFail);
	}

	char buf[MAX_BUF];
	int code = sock_ctl.Receive(buf, MAX_BUF, 0);
	if (code >= 3)
	{
		buf[code] = '\0';
		std::string str = buf;
		std::string first3Str = str.substr(0, 3);
		if (first3Str != "220")//û�����ӳɹ�
		{
			//CleanUp();
			throw FTPException(ExType::ConnectFail);
		}
	}
	else
	{
		//CleanUp();
		throw FTPException(ExType::ConnectFail);
	}
	//curDir = getCurWorkingDir();
}

FTPClient::~FTPClient()//Ҫ���йر����ӡ��رտ⣿�Ȳ���
{
	CleanUp();
}

void FTPClient::WSAStart()
{
	if (!WSAStarted)
	{
		WSADATA wsaData;
		WORD wVersionRequested;
		wVersionRequested = MAKEWORD(2, 0);
		int err = WSAStartup(wVersionRequested, &wsaData);
		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0 || err != 0)
		{
			//CleanUp();
			throw FTPException(ExType::StartUpFail);
		}
		else
		{
			WSAStarted = true;
		}
	}
}

int FTPClient::CleanUp()
{
	int ret = 0;
	if (WSAStarted)
	{
		ret = WSACleanup();
		WSAStarted = false;
	}
	return ret;
}

std::vector<std::string> FTPClient::getFileInfoStrs(std::string listRetStr)
{
	std::vector<std::string> ret;
	std::string tStr;
	int len = listRetStr.size();
	for (int i = 0; i < len; i++)
	{
		if (listRetStr[i] == '\r')//β��
		{
			i++;
			ret.push_back(tStr);
			tStr = "";
		}
		else
		{
			tStr += listRetStr[i];
		}
	}
	return ret;
}

std::vector<std::string> FTPClient::divideRetStrs(std::string retStr)
{
	std::vector<std::string> ret;
	std::string tStr;
	int len = retStr.size();
	for (int i = 0; i < len; i++)
	{
		if (retStr[i] == '\r')//β��
		{
			i++;
			ret.push_back(tStr);
			tStr = "";
		}
		else
		{
			tStr += retStr[i];
		}
	}
	return ret;
}

std::vector<FileInfo> FTPClient::getFilesOfCurWorkDir()
{
	std::string curDir = getCurWorkingDir();
	EnterPasvMode();
	char buf[MAX_BUF];
	memset(buf, 0, MAX_BUF);
	sprintf(buf, "LIST\r\n");//��ȡ��ǰ����Ŀ¼�µ��ļ��б�
	int ret = sock_ctl.Send(buf, strlen(buf), 0);
	memset(buf, 0, MAX_BUF);
	ret = sock_ctl.Receive(buf, MAX_BUF, 0);
	std::string str = buf;
	std::cout << str;

	if (ret < 3)
	{
		throw FTPException(ExType::OtherFails,str);
	}
	else
	{
		if (!containsCode(str,"150"))
		{
			throw FTPException(ExType::GetFileInfoFail,str);
		}
	}
	std::string fileInfo = "";
	memset(buf, 0, MAX_BUF);
	while ((ret = sock_data.Receive(buf, MAX_BUF, 0)) != 0)
	{
		str = buf;//�ص㣺ÿ���ļ���Ϣ֮����\r\n�ָ���һ����Ϣ�У�����������1�����ո�ָ�
		fileInfo += str;
		memset(buf, 0, MAX_BUF);
	}
	std::vector<std::string> fileInfos = getFileInfoStrs(fileInfo);
	std::vector<FileInfo> retVec;
	int len = fileInfos.size();
	
	for (int i = 0; i < len; i++)
	{
		retVec.push_back(FileInfo(fileInfos[i],curDir));
	}
	return retVec;
}

std::string FTPClient::getCurWorkingDir()
{
	char buf[MAX_BUF];
	memset(buf, 0, MAX_BUF);
	sprintf(buf, "PWD\r\n");
	int ret = sock_ctl.Send(buf, strlen(buf), 0);
	memset(buf, 0, MAX_BUF);
	ret = sock_ctl.Receive(buf, MAX_BUF, 0);
	std::string str = buf;
	if (ret < 3)
	{
		throw FTPException(ExType::OtherFails,str);
	}
	else if (!containsCode(str, "257"))
	{
		throw FTPException(ExType::GetCurDirFail,str);
	}
	return getFileNameWithTheRetOfPWD(str, str.size());
}

bool FTPClient::Login(const std::string& usr, const std::string& pwd)
{
	char buf[MAX_BUF];
	sprintf(buf, "USER %s\r\n", usr.c_str());
	std::string str;

	int ret = sock_ctl.Send(buf, strlen(buf), 0);
	memset(buf, 0, MAX_BUF);
	ret = sock_ctl.Receive(buf, MAX_BUF, 0);
	str = buf;
	if (!containsCode(str, "331"))
	{
		throw FTPException(ExType::LoginFail);
	}
	
	sprintf(buf, "PASS %s\r\n", pwd.c_str());
	ret = sock_ctl.Send(buf, strlen(buf), 0);
	memset(buf, 0, MAX_BUF);
	ret = sock_ctl.Receive(buf, MAX_BUF, 0);
	if (ret >= 3)
	{
		str = buf;
		if (!containsCode(str,"230"))//����
		{
			if (containsCode(str, "530"))
			{
				//CleanUp();
				throw FTPException(ExType::UsrOrPwdWrong);
			}
			else
			{
				//CleanUp();
				throw FTPException(ExType::LoginFail);
			}
		}
	}
	else
	{
		//CleanUp();
		throw FTPException(ExType::LoginFail);
	}
	return true;
}

void FTPClient::test()
{
	char buf[MAX_BUF];
	sprintf(buf, "PWD\r\n");
	int ret = sock_ctl.Send(buf, strlen(buf), 0);
	ret = sock_ctl.Receive(buf, MAX_BUF, 0);
	std::string str = buf;
	std::string subStr = str.substr(0, ret);
	std::cout << subStr;
	EnterPasvMode();

	memset(buf, 0, MAX_BUF);
	sprintf(buf, "LIST\r\n");//��ȡ��ǰ����Ŀ¼�µ��ļ��б�
	ret = sock_ctl.Send(buf, strlen(buf), 0);
	memset(buf, 0, MAX_BUF);
	ret = sock_ctl.Receive(buf, MAX_BUF, 0);
	//ret = sock_data.Receive(buf, MAX_BUF, 0);
	str = buf;
	subStr = str.substr(0, ret);
	std::cout << subStr;


	std::string myStr = "";
	memset(buf, 0, MAX_BUF);
	while ((ret = sock_data.Receive(buf, MAX_BUF, 0)) != 0)
	{
		str = buf;//�ص㣺ÿ���ļ���Ϣ֮����\r\n�ָ���һ����Ϣ�У�����������1�����ո�ָ�
		subStr = str.substr(0, ret);
		myStr += subStr;
		std::cout << subStr;
		memset(buf, 0, MAX_BUF);
	}

	char newBuf[MAX_BUF];
	memset(buf, 0, MAX_BUF);
	ret = sock_data.Receive(newBuf, MAX_BUF, 0);
	str = newBuf;
	subStr = str.substr(0, ret);
	myStr += subStr;
	std::cout << subStr;
}

bool FTPClient::createFolderAtWorkingDir(std::string folderName)//���ܻ�����ļ����������Ϲ淶���쳣
{
	return true;
}

bool FTPClient::createFileAtWorkingDir(std::string fileName)//���ܻ�����ļ��������Ϲ淶���쳣
{
	return true;
}

bool FTPClient::returnToParentDir()//����ǰΪ���ϲ�Ŀ¼��Ӧ�׳�һ���쳣
{
	return true;
}

bool FTPClient::deleteFileAtCurDir(std::string fileName)//ɾ����ǰ��������һ���ļ�-----
{
	char buf[MAX_BUF];
	memset(buf, 0, MAX_BUF);
	while (sock_ctl.Receive(buf, MAX_BUF, 0) != 0)
	{
		memset(buf, 0, MAX_BUF);
	}
	memset(buf, 0, MAX_BUF);
	sprintf(buf, "DELE %s\r\n",fileName.c_str());

	int num = sock_ctl.Send(buf, strlen(buf), 0);

	memset(buf, 0, MAX_BUF);
	num = sock_ctl.Receive(buf, MAX_BUF, 0);

	std::string str = buf;
	std::cout << str;
	if (num < 3)
	{
		throw FTPException(ExType::OtherFails);
	}
	else if (!containsCode(str, "250"))
	{
		std::string tStr = str.substr(0, 3);
		if (containsCode(str, "550") || containsCode(str, "450"))//�ļ�������
		{
			throw FTPException(ExType::FileAccessFail);
		}
		else//��������
		{
			throw FTPException(ExType::OtherFails);
		}
	}
	return true;
}

bool FTPClient::deleteFolderAtCurDir(std::string folderName)//ɾ����ǰ��������һ���ļ���
{
	return true;
}

bool FTPClient::enterFolder(std::string folderName)//����ĳһ�ļ���
{



	return true;
}

void FTPClient::disConnect()
{
	if (sock_ctl.Connected())
	{
		sock_ctl.Disconnect();
	}
	if (sock_data.Connected())
	{
		sock_data.Disconnect();
	}
}

//���뱻��ģʽ
bool FTPClient::EnterPasvMode()
{
	char buf[MAX_BUF];
	sprintf(buf, "PASV\r\n");
	int ret = sock_ctl.Send(buf, strlen(buf), 0);
	ret = sock_ctl.Receive(buf, MAX_BUF, 0);
	std::string str = buf;
	std::string subStr = str.substr(0, ret);//227
	if (subStr.size() < 3 or str.substr(0, 3) != "227")
	{
		throw FTPException(ExType::EnterPasvFail);
	}

	//��ȡ�������˵�ip����򿪵Ķ˿ں�
	std::string host = getIpByPasvRet(subStr, subStr.size());
	int port = getPortByPasvRet(subStr, subStr.size());
	if (port < 0)
	{
		throw FTPException(ExType::EnterPasvFail);
	}

	//sock_data���¿��Ķ˿ڽ�������
	sock_data.Disconnect();
	sock_data.SetAddrInfo(host, port);
	if (!sock_data.Create())
	{
		CleanUp();
		throw FTPException(ExType::CreateSocketFail);
	}
	if (!sock_data.Connect())
	{
		CleanUp();
		throw FTPException(ExType::ConnectFail);
	}
	int port_ctl = sock_ctl.getSocketLoaclPort();
	int port_data = sock_data.getSocketLoaclPort();
	std::cout << "control port:" << port_ctl << std::endl;
	std::cout << "data port:" << port_data << std::endl;

	return true;
}


//��ȡָ���ļ���Ϣ
//File FTPClient::GetFileInfo(const std::string& f)
//{
//	File file;
//	return file;
//}

void FTPClient::DisplayLog() const
{

}

//�Զ����Ƹ�ʽ�����ļ�
bool FTPClient::DownloadBinary(const std::string& f)
{
	return true;
}

//��ASCII��ʽ�����ļ�
bool FTPClient::DownloadASCII(const std::string& f)
{
	return true;
}

//�ϴ��ļ�
bool FTPClient::Upload(const std::string& f, bool binary)
{
	return true;
}

//ɾ��ָ���ļ�
bool FTPClient::Delete(const std::string& f)
{
	return true;
}

//�˳��ͻ���
bool FTPClient::Quit()
{
	return true;
}

void FTPClient::GetFileList()
{

}

bool FTPClient::EnterASCIIMode()//����ASCIIģʽ
{
	return true;
}

bool FTPClient::EnterBinaryMode()//���������ģʽ
{
	return true;
}

std::string FTPClient::getIpByPasvRet(std::string pasvRetStr, int len)const
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		if (pasvRetStr[i] == '(') break;
	}
	i++;
	int numCount = 0;
	std::string strs[6];
	std::string tStr = "";
	for (; i < len; i++)
	{
		if (pasvRetStr[i] == ')')
		{
			//�������һ��str
			strs[numCount] = tStr;
			break;
		}
		else if (pasvRetStr[i] == ',')
		{
			//������һ��str
			strs[numCount] = tStr;
			//numCount++
			numCount++;
			//���tStr
			tStr = "";
		}
		else
		{
			tStr += pasvRetStr[i];
		}
	}
	std::string ret = "";
	for (int i = 0; i < 4; i++)
	{
		ret += strs[i];
		if (i != 3)
		{
			ret += '.';
		}
	}
	return ret;
}

int FTPClient::getPortByPasvRet(std::string pasvRetStr, int len)const
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		if (pasvRetStr[i] == '(') break;
	}
	i++;
	int numCount = 0;
	std::string strs[6];
	std::string tStr = "";
	for (; i < len; i++)
	{
		if (pasvRetStr[i] == ')')
		{
			//�������һ��str
			strs[numCount] = tStr;
			break;
		}
		else if (pasvRetStr[i] == ',')
		{
			//������һ��str
			strs[numCount] = tStr;
			//numCount++
			numCount++;
			//���tStr
			tStr = "";
		}
		else
		{
			tStr += pasvRetStr[i];
		}
	}
	int factor = str2UInt(strs[4]);
	int add = str2UInt(strs[5]);
	if (factor >= 0 && add >= 0)
	{
		return 256 * factor + add;
	}
	else
	{
		return -1;
	}
}

std::string FTPClient::getFileNameWithTheRetOfPWD(std::string pwdRetStr,int len)const
{
	std::string ret = "";
	bool in = false;
	for (int i = 0; i < len; i++)
	{
		if (pwdRetStr[i] == '\"')
		{
			if (in)
			{
				break;
			}
			else
			{
				in = true;
			}
		}
		else
		{
			if (in)
			{
				ret += pwdRetStr[i];
			}
		}
	}
	return ret;
}

int FTPClient::str2UInt(std::string str)const
{
	if (str.empty()) return -1;
	int ret = 0;
	for (int i = 0; i < str.size(); i++)
	{
		ret *= 10;
		ret += str[i] - '0';
	}
	return ret;
}

bool FTPClient::isDigit(char c)const
{
	return c >= '0' && c <= '9';
}

RetInfo FTPClient::getRetInfo(std::string str)
{
	std::string code = "", otherInfo = "";
	int i = 0;
	while (i < str.size() && isDigit(str[i]))
	{
		code += str[i];
		i++;
	}
	while (i < str.size() && str[i] == ' ') i++;
	while (i < str.size() && str[i] != '\r')
	{
		otherInfo += str[i];
		i++;
	}
	return RetInfo(code, otherInfo);
}

RetInfo FTPClient::getFirstRet(std::string str)
{
	std::vector<std::string> strs = divideRetStrs(str);
	if (strs.empty()) return getRetInfo("");
	return getRetInfo(strs[0]);
}

RetInfo FTPClient::getLastRet(std::string str)
{
	std::vector<std::string> strs = divideRetStrs(str);
	if (strs.empty()) return getRetInfo("");
	return getRetInfo(strs[strs.size() - 1]);
}

bool FTPClient::containsCode(std::string str, std::string code)
{
	if (str.size() < code.size()) return false;
	int index = str.find(code);
	if (index < 0)
		return false;
	else if (index == 0)
		return true;
	else 
	{
		if (index < 2) return false;
		else 
		{
			if (str[index - 2] == '\r' && str[index - 1] == '\n')
				return true;
			else
				return false;
		}
	}
}