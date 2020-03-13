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
    //���뱻��ģʽ
    bool EnterPasvMode();
    //�����ļ��б� ------�˹���Ӧ����ǰ����ɣ�ǰ�˵��ˢ��ʱ�����»�ȡ��ǰ·�����ļ��б���ʾ����
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

    bool createFolderAtWorkingDir(std::string folderName);//�ڵ�ǰ����������һ���ļ���
    bool createFileAtWorkingDir(std::string fileName);//�ڵ�ǰ����������һ���ļ�
    bool returnToParentDir();//������·����Ϊ��һ����Ŀ¼
    bool deleteFileAtCurDir(std::string fileName);//ɾ����ǰ��������һ���ļ�
    bool deleteFolderAtCurDir(std::string folderName);//ɾ����ǰ��������һ���ļ���
    bool enterFolder(std::string folderName);//����ĳһ�ļ���
    bool Download(const std::string& des, const std::string& src);//�����ļ�
    bool Upload(const std::string& pathName, const std::string& fileName);//�ϴ��ļ�
    bool rename(std::string from, std::string to);//�ļ�������

    void disConnect();
    std::string getDir();

    void test();
private:
    //Logger logger;
    MySocket sock_data; //���ڴ������ݵ�socket
    MySocket sock_ctl; //���ڴ��������socket

    bool WSAStarted = false;

    std::string host;
    std::string user = "";
    std::string pwd = "";
    std::string curDir = "";
    int port;//���ƶ˿�?
    //
    void GetFileList();
    bool EnterASCIIMode(); //����ASCIIģʽ
    bool EnterBinaryMode(); //���������ģʽ

    std::vector<std::string> getFileInfoStrs(std::string listRetStr);
    std::vector<std::string> divideRetStrs(std::string retStr);
    bool setWorkSpace(std::string workSpace);

private:
    std::string getFileNameWithTheRetOfPWD(std::string pwdRetStr, int len) const;
    std::string getIpByPasvRet(std::string pasvRetStr, int len)const;//����Pasv�������������ص��ַ����������ṩ��IP
    int getPortByPasvRet(std::string pasvRetStr, int len)const;//����Pasv�������������ص��ַ����������ṩ��port
    int str2UInt(std::string str)const;
    RetInfo getRetInfo(std::string str);
    RetInfo getFirstRet(std::string str);
    RetInfo getLastRet(std::string str);
    bool containsCode(std::string str, std::string code);
    bool isDigit(char c)const;
};

