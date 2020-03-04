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
	char isFolderChar = '\0';
	std::string num1 = "",num2 = "",num3="";
	std::string createMonth = "";//创建文件的月份
	std::string createDay = "";//创建文件的日期
	std::string createTime = "";//创建文件的时间
	std::string name = "";
	while (strFromServerResponse[i] != ' ' && i < len)
	{
		accessRigthStr += strFromServerResponse[i];
		i++;
	}
	while (strFromServerResponse[i] == ' ' && i < len) i++;
	isFolderChar = strFromServerResponse[i];
	i++;
	while (strFromServerResponse[i] == ' ' && i < len) i++;
	while (strFromServerResponse[i] != ' ' && i < len)
	{
		num1 += strFromServerResponse[i];
		i++;
	}
	while (strFromServerResponse[i] == ' ' && i < len) i++;
	while (strFromServerResponse[i] != ' ' && i < len)
	{
		num2 += strFromServerResponse[i];
		i++;
	}
	while (strFromServerResponse[i] == ' ' && i < len) i++;
	while (strFromServerResponse[i] != ' ' && i < len)
	{
		num3 += strFromServerResponse[i];
		i++;
	}
	while (strFromServerResponse[i] == ' ' && i < len) i++;
	while (strFromServerResponse[i] != ' ' && i < len)
	{
		createMonth += strFromServerResponse[i];
		i++;
	}
	while (strFromServerResponse[i] == ' ' && i < len) i++;
	while (strFromServerResponse[i] != ' ' && i < len)
	{
		createDay += strFromServerResponse[i];
		i++;
	}
	while (strFromServerResponse[i] == ' ' && i < len) i++;
	while (strFromServerResponse[i] != ' ' && i < len)
	{
		createTime += strFromServerResponse[i];
		i++;
	}
	while (strFromServerResponse[i] == ' ' && i < len) i++;
	while (strFromServerResponse[i] != ' ' && i < len)
	{
		name += strFromServerResponse[i];
		i++;
	}
	this->fileName = name;
	this->path = curDir + '/' + name;
	this->folder = isFolderChar == '2';
}