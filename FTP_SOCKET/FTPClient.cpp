#include "FTPClient.h"
#include <WinSock2.h>
#include "FTPException.h"
#include <fstream>
#pragma comment(lib,"ws2_32.lib")

#define MAX_BUF (5000)

FTPClient::FTPClient(const std::string& host, int port)//创建MySokcet并将其连接
{
	//this->logger = Logger(host, port);


	//判断端口号是否符合要求?

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
		if (first3Str != "220")//没有连接成功
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

FTPClient::~FTPClient()//要进行关闭连接、关闭库？等操作
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
		if (listRetStr[i] == '\r')//尾部
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
		if (retStr[i] == '\r')//尾部
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
	sprintf(buf, "LIST\r\n");//获取当前工作目录下的文件列表
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
		str = buf;//特点：每条文件信息之间用\r\n分隔，一条信息中，各种属性用1或多个空格分隔
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
	this->curDir = getFileNameWithTheRetOfPWD(str, str.size());
	return this->curDir;
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
		if (!containsCode(str,"230"))//报错
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
	sprintf(buf, "LIST\r\n");//获取当前工作目录下的文件列表
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
		str = buf;//特点：每条文件信息之间用\r\n分隔，一条信息中，各种属性用1或多个空格分隔
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

bool FTPClient::createFolderAtWorkingDir(std::string folderName)//可能会产生文件夹名不符合规范的异常
{
	char buf[MAX_BUF];
	memset(buf, 0, MAX_BUF);

	sprintf(buf, "MKD %s\r\n", folderName.c_str());
	int num = sock_ctl.Send(buf, strlen(buf), 0);

	memset(buf, 0, MAX_BUF);
	num = sock_ctl.Receive(buf, MAX_BUF, 0);
	std::string str = buf;

	if (num < 3)
	{
		throw FTPException(ExType::OtherFails);
	}
	else
	{
		if (!containsCode(str, "257"))
		{
			throw FTPException(ExType::CreateFolderFail, str);
		}
	}
	return true;
}

bool FTPClient::createFileAtWorkingDir(std::string fileName)//可能会产生文件名不符合规范的异常
{
	return true;
}

bool FTPClient::returnToParentDir()//若当前为最上层目录，应抛出一个异常     调用之后还需要改变curDir，目前还没改
{
	std::string curDir = this->curDir;//可能需要调用getCurWorkingDir函数重新获取
	if (curDir.empty()) return "";
	int index = curDir.find_last_of('/');
	if (index == curDir.size() - 1)
	{
		if (index == 0) throw FTPException(ExType::AtTopDir);
		else
		{
			curDir.pop_back();
			index = curDir.find_last_of('/');
		}
	}
	std::string aimDir = curDir.substr(0, index);

	char buf[MAX_BUF];
	memset(buf, 0, MAX_BUF);
	if (aimDir == "/")
	{
		sprintf(buf, "CDUP\r\n");
	}
	else
	{
		sprintf(buf, "CWD %s\r\n", aimDir.c_str());
	}
	int num = sock_ctl.Send(buf, strlen(buf), 0);

	memset(buf, 0, MAX_BUF);
	num = sock_ctl.Receive(buf, MAX_BUF, 0);

	std::string str = buf;
	std::cout << str;
	if (num < 3)
	{
		throw FTPException(ExType::OtherFails);
	}
	else if(!containsCode(str, "250"))
	{
		throw FTPException(ExType::ChangeWorkSpaceFail, str);
	}
	return true;
}

bool FTPClient::setWorkSpace(std::string workSpace)
{
	char buf[MAX_BUF];
	memset(buf, 0, MAX_BUF);

	sprintf(buf, "CWD %s\r\n", workSpace.c_str());
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
		throw FTPException(ExType::ChangeWorkSpaceFail, str);
	}
	this->curDir = workSpace;
	return true;
}

bool FTPClient::deleteFileAtCurDir(std::string fileName)//删除当前工作区的一个文件-----
{
	char buf[MAX_BUF];
	/*memset(buf, 0, MAX_BUF);
	while (sock_ctl.Receive(buf, MAX_BUF, 0) != 0)
	{
		memset(buf, 0, MAX_BUF);
	}*/
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
		if (containsCode(str, "550") || containsCode(str, "450"))//文件不可用
		{
			throw FTPException(ExType::FileAccessFail);
		}
		else//其他错误
		{
			throw FTPException(ExType::OtherFails);
		}
	}
	return true;
}

bool FTPClient::deleteFolderAtCurDir(std::string folderName)//删除当前工作区的一个文件夹
{
	char buf[MAX_BUF];
	/*memset(buf, 0, MAX_BUF);
	while (sock_ctl.Receive(buf, MAX_BUF, 0) != 0)
	{
		memset(buf, 0, MAX_BUF);
	}*/
	memset(buf, 0, MAX_BUF);
	sprintf(buf, "RMD %s\r\n", folderName.c_str());

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
		if (containsCode(str, "550") || containsCode(str, "450"))//文件不可用
		{
			throw FTPException(ExType::FileAccessFail,str);
		}
		else//其他错误
		{
			throw FTPException(ExType::OtherFails);
		}
	}
	return true;
}

bool FTPClient::enterFolder(std::string folderName)//进入某一文件夹
{
	std::string aimStr = this->curDir + '/' + folderName;
	try 
	{
		setWorkSpace(aimStr);
	}
	catch (FTPException e)
	{
		throw e;
	}
	return true;
}

bool FTPClient::Download(const std::string& des, const std::string& src)
{
	char buf[MAX_BUF];
	memset(buf, 0, MAX_BUF);
	sprintf(buf, "SIZE %s\r\n", src.c_str());//是否需要完整路径?
	int num = sock_ctl.Send(buf, strlen(buf),0);

	memset(buf, 0, MAX_BUF);
	num = sock_ctl.Receive(buf, MAX_BUF, 0);
	std::string str = buf;
	if (num < 3)
	{
		throw FTPException(ExType::OtherFails);
	}
	else
	{
		if (!containsCode(str, "213"))
		{
			throw FTPException(ExType::GetFileSizeFail, str);
		}
	}
	int index = str.find("213");
	std::string sizeStr = "";
	for (int i = index + 4; i < str.size() && str[i] != '\r'; i++)
	{
		sizeStr += str[i];
	}
	int size = str2UInt(sizeStr);
	EnterPasvMode();
	memset(buf, 0, MAX_BUF);

	sprintf(buf, "RETR %s\r\n", src.c_str());
	num = sock_ctl.Send(buf, strlen(buf), 0);
	num = sock_ctl.Receive(buf, MAX_BUF, 0);
	str = buf;
	if (num < 3)
	{
		throw FTPException(ExType::OtherFails);
	}
	else
	{
		if (!containsCode(str, "150"))
		{
			throw FTPException(ExType::FileDownloadFail, str);
		}
	}

	FILE* file = fopen(des.c_str(), "w");
	memset(buf, 0, MAX_BUF);
	while ((num = sock_data.Receive(buf, MAX_BUF, 0)) != 0)
	{
		fprintf(file, buf);
		memset(buf, 0, MAX_BUF);
	}
	fclose(file);
	return true;
}

bool FTPClient::setASCIImode()
{
	char buf[MAX_BUF];
	memset(buf, 0, MAX_BUF);
	sprintf(buf, "TYPE I\r\n");
	int num = sock_ctl.Send(buf, strlen(buf), 0);

	memset(buf, 0, MAX_BUF);
	num = sock_ctl.Receive(buf, MAX_BUF, 0);//200
	std::string str = buf;
	if (num < 3)
	{
		throw FTPException(ExType::OtherFails, str);
	}
	else if (!containsCode(str, "200"))
	{
		throw FTPException(ExType::SetBinModeFail, str);
	}
	return true;
}

bool FTPClient::DownloadWithPos(const std::string& des, const std::string& src)
{
	int len = getFileLength(des);
	
	char buf[MAX_BUF];
	memset(buf, 0, MAX_BUF);
	sprintf(buf, "SIZE %s\r\n", src.c_str());//是否需要完整路径?
	int num = sock_ctl.Send(buf, strlen(buf), 0);

	memset(buf, 0, MAX_BUF);
	num = sock_ctl.Receive(buf, MAX_BUF, 0);
	std::string str = buf;
	if (num < 3)
	{
		throw FTPException(ExType::OtherFails);
	}
	else
	{
		if (!containsCode(str, "213"))
		{
			throw FTPException(ExType::GetFileSizeFail, str);
		}
	}
	int index = str.find("213");
	std::string sizeStr = "";
	for (int i = index + 4; i < str.size() && str[i] != '\r'; i++)
	{
		sizeStr += str[i];
	}
	int size = str2UInt(sizeStr);

	if (len >= size) return false;
	if (len == -1) len = 0;

	//设置pos
	if (len > 0)
	{
		memset(buf, 0, MAX_BUF);
		sprintf(buf, "REST %d\r\n", len);
		num = sock_ctl.Send(buf, strlen(buf), 0);
		num = sock_ctl.Receive(buf, MAX_BUF, 0);//350
		str = buf;
		if (num < 3)
		{
			throw FTPException(ExType::OtherFails, str);
		}
		else if (!containsCode(str, "350"))
		{
			throw FTPException(ExType::SetPosFail, str);
		}
		setASCIImode();
	}
	EnterPasvMode();
	memset(buf, 0, MAX_BUF);

	sprintf(buf, "RETR %s\r\n", src.c_str());
	num = sock_ctl.Send(buf, strlen(buf), 0);
	num = sock_ctl.Receive(buf, MAX_BUF, 0);
	str = buf;
	if (num < 3)
	{
		throw FTPException(ExType::OtherFails);
	}
	else
	{
		if (!containsCode(str, "150"))
		{
			throw FTPException(ExType::FileDownloadFail, str);
		}
	}

	FILE* file = fopen(des.c_str(), "a+");
	memset(buf, 0, MAX_BUF);
	while ((num = sock_data.Receive(buf, MAX_BUF, 0)) != 0)
	{
		fprintf(file, buf);
		memset(buf, 0, MAX_BUF);
	}
	fclose(file);
	return true;
}

int FTPClient::parseFileSize(std::string response)
{
	int index = response.find("213");
	std::string str = "";
	for (int i = index + 4; i < response.size() && response[i] != '\r'; i++)
	{
		str += response[i];
	}
	int length = -1;
	if (!str.empty())
	{
		length = str2UInt(str);
	}
	return length;
}

bool FTPClient::Upload(const std::string& pathName, const std::string& fileName)//上传文件
{
	FILE* pFile = fopen(pathName.c_str(), "rb");  // 以只读方式打开  且文件必须存在

	char buf[MAX_BUF];
	EnterPasvMode();

	memset(buf, 0, MAX_BUF);
	sprintf(buf, "SIZE %s\r\n", fileName.c_str());//是否需要完整路径?
	int num = sock_ctl.Send(buf, strlen(buf), 0);
	memset(buf, 0, MAX_BUF);
	num = sock_ctl.Receive(buf, MAX_BUF, 0);// 成功示例：213 7\r\n    失败示例：550 Could not get the file size.\r\n
	std::string str = buf;

	int fileSize = -1;
	//根据服务器上是否有数据选择合适的方式传输
	if (num < 3)
	{
		throw FTPException(ExType::OtherFails, str);
	}
	else
	{
		if (containsCode(str, "550"))//不存在该目录
		{
			//就让fileSize = -1；
			memset(buf, 0, MAX_BUF);
			sprintf(buf, "STOR %s\r\n", fileName.c_str());//是否需要完整路径?
		}
		else if (containsCode(str, "213"))//存在该目录
		{
			fileSize = parseFileSize(str);
			memset(buf, 0, MAX_BUF);
			sprintf(buf, "APPE %s\r\n", fileName.c_str());//是否需要完整路径?
		}
		else
		{
			throw FTPException(ExType::OtherFails, str);
		}
	}
	num = sock_ctl.Send(buf, strlen(buf), 0);
	memset(buf, 0, MAX_BUF);
	num = sock_ctl.Receive(buf, MAX_BUF, 0);
	str = buf;

	if (num < 3)
	{
		throw FTPException(ExType::OtherFails, str);
	}
	else if(!containsCode(str,"150"))
	{
		throw FTPException(ExType::UploadRefused,str);
	}

	fseek(pFile, fileSize, SEEK_SET);
	int nLen;
	const unsigned long dataLen = MAX_BUF;
	char strBuf[dataLen] = { 0 };
	while (!feof(pFile))
	{
		nLen = fread(strBuf, 1, dataLen, pFile);
		if (nLen < 0)
		{
			break;
		}

		if ((num = sock_data.Send(strBuf,strlen(buf),0)) < 0)
		{
			//报错
			fclose(pFile);
			throw FTPException(ExType::DataUploadFail);
		}
	}
	sock_data.Disconnect();
	fclose(pFile);

	memset(buf, 0, MAX_BUF);
	num = sock_ctl.Receive(buf, MAX_BUF, 0);//226

	if (num < 3 || !containsCode(str, "150"))
	{
		throw FTPException(ExType::OtherFails, str);
	}
	
	
	//num = sock_ctl.Send(buf, strlen(buf), 0);
	//memset(buf, 0, MAX_BUF);
	//num = sock_ctl.Receive(buf, MAX_BUF, 0);
	//std::string str = buf;
	//if (num < 3)
	//{
	//	throw FTPException(ExType::OtherFails);
	//}
	//else//150
	//{
	//	if (!containsCode(str, "150"))
	//	{
	//		throw FTPException(ExType::FileUploadFail, str);
	//	}
	//}
	////开始传输数据
	return true;
}

bool FTPClient::rename(std::string from, std::string to)
{
	//见：https://www.cnblogs.com/hongyuyingxiao/p/10486036.html 三分之二左右处
	char buf[MAX_BUF];

	return true;
}

std::string FTPClient::getDir()
{
	if (this->curDir == "") return getCurWorkingDir();
	return this->curDir;
}

void FTPClient::disConnect()
{
	//关闭数据连接,退出 FTP 服务器并关闭控制连接
	closesocket(sock_data);
	char buf[MAX_BUF];
	memset(buf, 0, MAX_BUF);
	//接受返回 客户端接收服务器的响应码和信息，正常为 ”226 Transfer complete.”
	/*recv(sock_ctl, buf, MAX_BUF,0);
	
	memset(buf, 0, MAX_BUF);*/
	sprintf(buf, "QUIT\r\n");
	send(sock_ctl, buf, strlen(buf), 0);

	memset(buf, 0, MAX_BUF);
	//recv(sock_ctl, buf, MAX_BUF, 0);

	closesocket(sock_ctl);
	/*if (sock_ctl.Connected())
	{
		sock_ctl.Disconnect();
	}
	if (sock_data.Connected())
	{
		sock_data.Disconnect();
	}*/
}

//进入被动模式
bool FTPClient::EnterPasvMode()
{
	char buf[MAX_BUF];
	memset(buf, 0, MAX_BUF);
	sprintf(buf, "PASV\r\n");
	int ret = sock_ctl.Send(buf, strlen(buf), 0);
	ret = sock_ctl.Receive(buf, MAX_BUF, 0);
	std::string str = buf;
	std::string subStr = str.substr(0, ret);//227
	if (subStr.size() < 3 || !containsCode(subStr,"227"))
	{
		throw FTPException(ExType::EnterPasvFail);
	}

	//获取服务器端的ip和其打开的端口号
	std::string host = getIpByPasvRet(subStr, subStr.size());
	int port = getPortByPasvRet(subStr, subStr.size());
	if (port < 0)
	{
		throw FTPException(ExType::EnterPasvFail);
	}

	//sock_data与新开的端口进行连接
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


//获取指定文件信息
//File FTPClient::GetFileInfo(const std::string& f)
//{
//	File file;
//	return file;
//}

void FTPClient::DisplayLog() const
{

}

//以二进制格式下载文件
bool FTPClient::DownloadBinary(const std::string& f)
{
	return true;
}

//以ASCII格式下载文件
bool FTPClient::DownloadASCII(const std::string& f)
{
	return true;
}

//上传文件
bool FTPClient::Upload(const std::string& f, bool binary)
{
	return true;
}

//删除指定文件
bool FTPClient::Delete(const std::string& f)
{
	return true;
}

//退出客户端
bool FTPClient::Quit()
{
	return true;
}

void FTPClient::GetFileList()
{

}

bool FTPClient::EnterASCIIMode()//进入ASCII模式
{
	return true;
}

bool FTPClient::EnterBinaryMode()//进入二进制模式
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
			//保存最后一个str
			strs[numCount] = tStr;
			break;
		}
		else if (pasvRetStr[i] == ',')
		{
			//保存上一个str
			strs[numCount] = tStr;
			//numCount++
			numCount++;
			//清空tStr
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
			//保存最后一个str
			strs[numCount] = tStr;
			break;
		}
		else if (pasvRetStr[i] == ',')
		{
			//保存上一个str
			strs[numCount] = tStr;
			//numCount++
			numCount++;
			//清空tStr
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

int FTPClient::getFileLength(std::string fileName) {
	int length;
	std::ifstream is;
	is.open(fileName.c_str(), std::ios::binary);

	// get length of file: 
	is.seekg(0, std::ios::end);
	length = is.tellg();
	return length;
}