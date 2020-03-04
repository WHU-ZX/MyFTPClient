#pragma once
#include <string>
#include "File.h"
#include "Logger.h"
#include <vector>
#include "FileInfo.h"

class FTPClient
{
public:
    FTPClient(const std::string & host, int port);
    virtual ~FTPClient();
    bool Login(const std::string& usr, const std::string& pwd);
    //进入被动模式
    bool EnterPasvMode();
    //更新文件列表
    void UpdateFileList();
    //获取指定文件信息
    File GetFileInfo(const std::string& f);
    void DisplayLog() const;
    //以二进制格式下载文件
    bool DownloadBinary(const std::string& f);
    //以ASCII格式下载文件
    bool DownloadASCII(const std::string& f);
    //上传文件
    bool Upload(const std::string& f, bool binary);
    //删除指定文件
    bool Delete(const std::string& f);
    //退出客户端
    bool Quit();
    int CleanUp();//调用WSACleanup();
    void WSAStart();

    std::vector<FileInfo> getFilesOfCurWorkDir();//获得当前工作路径下的文件信息,调用此函数前必须保证进入了被动模式

    std::string getCurWorkingDir();

    void test();
private:
    //Logger logger;
    MySocket sock_data; //用于传输数据的socket
    MySocket sock_ctl; //用于传输命令的socket

    bool WSAStarted = false;

    std::string host;
    std::string user = "";
    std::string pwd = "";
    int port;//控制端口?
    //
    void GetFileList();
    bool EnterASCIIMode(); //进入ASCII模式
    bool EnterBinaryMode(); //进入二进制模式

    std::vector<std::string> getFileInfoStrs(std::string listRetStr);
};

std::string getFileNameWithTheRetOfPWD(std::string pwdRetStr,int len);
std::string getIpByPasvRet(std::string pasvRetStr, int len);//根据Pasv命令后服务器返回的字符串生成其提供的IP
int getPortByPasvRet(std::string pasvRetStr, int len);//根据Pasv命令后服务器返回的字符串生成其提供的port
int str2UInt(std::string str);