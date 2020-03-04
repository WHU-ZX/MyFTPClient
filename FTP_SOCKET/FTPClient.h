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
    //���뱻��ģʽ
    bool EnterPasvMode();
    //�����ļ��б�
    void UpdateFileList();
    //��ȡָ���ļ���Ϣ
    File GetFileInfo(const std::string& f);
    void DisplayLog() const;
    //�Զ����Ƹ�ʽ�����ļ�
    bool DownloadBinary(const std::string& f);
    //��ASCII��ʽ�����ļ�
    bool DownloadASCII(const std::string& f);
    //�ϴ��ļ�
    bool Upload(const std::string& f, bool binary);
    //ɾ��ָ���ļ�
    bool Delete(const std::string& f);
    //�˳��ͻ���
    bool Quit();
    int CleanUp();//����WSACleanup();
    void WSAStart();

    std::vector<FileInfo> getFilesOfCurWorkDir();//��õ�ǰ����·���µ��ļ���Ϣ,���ô˺���ǰ���뱣֤�����˱���ģʽ

    std::string getCurWorkingDir();

    void test();
private:
    //Logger logger;
    MySocket sock_data; //���ڴ������ݵ�socket
    MySocket sock_ctl; //���ڴ��������socket

    bool WSAStarted = false;

    std::string host;
    std::string user = "";
    std::string pwd = "";
    int port;//���ƶ˿�?
    //
    void GetFileList();
    bool EnterASCIIMode(); //����ASCIIģʽ
    bool EnterBinaryMode(); //���������ģʽ

    std::vector<std::string> getFileInfoStrs(std::string listRetStr);
};

std::string getFileNameWithTheRetOfPWD(std::string pwdRetStr,int len);
std::string getIpByPasvRet(std::string pasvRetStr, int len);//����Pasv�������������ص��ַ����������ṩ��IP
int getPortByPasvRet(std::string pasvRetStr, int len);//����Pasv�������������ص��ַ����������ṩ��port
int str2UInt(std::string str);