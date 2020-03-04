#include "FileInfo.h"

std::string FileInfo::getFileName()
{
	return this->fileName;
}

std::string FileInfo::getPath()
{
	return this->path;
}

bool FileInfo::isFolder()//���ļ����򷵻�true
{
	return folder;
}

FileInfo::FileInfo(std::string strFromServerResponse,std::string curDir)
{
	int i = 0, len = strFromServerResponse.size();
	//��������ֵ���ǰ�˳��������
	std::string accessRigthStr = "";
	char isFolderChar = '\0';
	std::string num1 = "",num2 = "",num3="";
	std::string createMonth = "";//�����ļ����·�
	std::string createDay = "";//�����ļ�������
	std::string createTime = "";//�����ļ���ʱ��
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