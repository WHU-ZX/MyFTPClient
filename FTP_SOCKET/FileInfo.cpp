#include "FileInfo.h"

std::string FileInfo::getFileName()
{
	return this->fileName;
}

std::string FileInfo::getPath()
{
	return this->path;
}

bool FileInfo::isFolder()//是文件夹则返回true
{
	return folder;
}

FileInfo::FileInfo(std::string strFromServerResponse,std::string curDir)
{
	int i = 0, len = strFromServerResponse.size();
	//各种属性值，是按顺序声明的
	std::string accessRigthStr = "";
	std::string isFolder = "";
	std::string num1 = "",num2 = "",num3="";
	std::string createMonth = "";//创建文件的月份
	std::string createDay = "";//创建文件的日期
	std::string createTime = "";//创建文件的时间
	std::string name = "";
	while (i < len && strFromServerResponse[i] != ' ')
	{
		accessRigthStr += strFromServerResponse[i];
		i++;
	}
	while (i < len && strFromServerResponse[i] == ' ') i++;

	while (i < len && strFromServerResponse[i] != ' ')
	{
		isFolder += strFromServerResponse[i];
		i++;
	}
	while (i < len && strFromServerResponse[i] == ' ') i++;

	while (i < len && strFromServerResponse[i] != ' ')
	{
		num1 += strFromServerResponse[i];
		i++;
	}
	while (i < len && strFromServerResponse[i] == ' ') i++;
	while (i < len && strFromServerResponse[i] != ' ')
	{
		num2 += strFromServerResponse[i];
		i++;
	}
	while (i < len && strFromServerResponse[i] == ' ') i++;
	while (i < len && strFromServerResponse[i] != ' ')
	{
		num3 += strFromServerResponse[i];
		i++;
	}
	while (i < len && strFromServerResponse[i] == ' ') i++;
	while (i < len && strFromServerResponse[i] != ' ')
	{
		createMonth += strFromServerResponse[i];
		i++;
	}
	while (i < len && strFromServerResponse[i] == ' ') i++;
	while (i < len && strFromServerResponse[i] != ' ')
	{
		createDay += strFromServerResponse[i];
		i++;
	}
	while (i < len && strFromServerResponse[i] == ' ') i++;
	while (i < len && strFromServerResponse[i] != ' ')
	{
		createTime += strFromServerResponse[i];
		i++;
	}
	while (i < len && strFromServerResponse[i] == ' ') i++;
	while (i < len && strFromServerResponse[i] != ' ')
	{
		name += strFromServerResponse[i];
		i++;
	}
	this->fileName = name;
	this->path = curDir + '/' + name;
	int num = str2UInt(isFolder);
	this->folder = (num > 1);
}

int FileInfo::str2UInt(std::string str)const
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