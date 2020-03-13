#pragma once
#include <string>


class FileInfo
{
private:
	std::string fileName;//文件名
	std::string path;//文件绝对路径
	bool folder;
	int str2UInt(std::string str)const;

public:
	FileInfo(std::string strFromServerResponse,std::string curDir);//传入服务器返回的信息中，每一个文件对应的字符串，其中，要去掉原有的\r\n
	std::string getFileName();
	std::string getPath();
	bool isFolder();//是文件夹则返回true
};

