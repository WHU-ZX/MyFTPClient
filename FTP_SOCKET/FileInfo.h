#pragma once
#include <string>


class FileInfo
{
private:
	std::string fileName;//�ļ���
	std::string path;//�ļ�����·��
	bool folder;
	int str2UInt(std::string str)const;

public:
	FileInfo(std::string strFromServerResponse,std::string curDir);//������������ص���Ϣ�У�ÿһ���ļ���Ӧ���ַ��������У�Ҫȥ��ԭ�е�\r\n
	std::string getFileName();
	std::string getPath();
	bool isFolder();//���ļ����򷵻�true
};

