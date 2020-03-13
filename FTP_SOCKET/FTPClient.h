#pragma once
#include <string>
#include "File.h"
#include "Logger.h"
#include <vector>
#include "FileInfo.h"
#include "RetInfo.h"

class FTPClient
{
public:
    FTPClient(const std::string & host, int port);
    virtual ~FTPClient();
    bool Login(const std::string& usr, const std::string& pwd);
    //进入被动模式
    bool EnterPasvMode();
    //更新文件列表 ------此功能应该在前端完成，前端点击刷新时，重新获取当前路径的文件列表并显示出来
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

    bool createFolderAtWorkingDir(std::string folderName);//在当前工作区创建一个文件夹
    bool createFileAtWorkingDir(std::string fileName);//在当前工作区创建一个文件
    bool returnToParentDir();//将工作路径设为上一工作目录
    bool deleteFileAtCurDir(std::string fileName);//删除当前工作区的一个文件
    bool deleteFolderAtCurDir(std::string folderName);//删除当前工作区的一个文件夹
    bool enterFolder(std::string folderName);//进入某一文件夹
    bool Download(const std::string& des, const std::string& src);//下载文件
    bool Upload(const std::string& pathName, const std::string& fileName);//上传文件
    bool rename(std::string from, std::string to);//文件重命名

    void disConnect();
    std::string getDir();

    void test();
private:
    //Logger logger;
    MySocket sock_data; //用于传输数据的socket
    MySocket sock_ctl; //用于传输命令的socket

    bool WSAStarted = false;

    std::string host;
    std::string user = "";
    std::string pwd = "";
    std::string curDir = "";
    int port;//控制端口?
    //
    void GetFileList();
    bool EnterASCIIMode(); //进入ASCII模式
    bool EnterBinaryMode(); //进入二进制模式

    std::vector<std::string> getFileInfoStrs(std::string listRetStr);
    std::vector<std::string> divideRetStrs(std::string retStr);
    bool setWorkSpace(std::string workSpace);

private:
    std::string getFileNameWithTheRetOfPWD(std::string pwdRetStr, int len) const;
    std::string getIpByPasvRet(std::string pasvRetStr, int len)const;//根据Pasv命令后服务器返回的字符串生成其提供的IP
    int getPortByPasvRet(std::string pasvRetStr, int len)const;//根据Pasv命令后服务器返回的字符串生成其提供的port
    int str2UInt(std::string str)const;
    RetInfo getRetInfo(std::string str);
    RetInfo getFirstRet(std::string str);
    RetInfo getLastRet(std::string str);
    bool containsCode(std::string str, std::string code);
    bool isDigit(char c)const;
};

