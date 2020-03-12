#include "FTPException.h"
#include <iostream>

std::string detailTab[] = {"����WinSocket��ʧ��","����Socketʧ��","���ӷ�����ʧ��","��¼ʧ��","�˺Ż��������",
							"���뱻��ģʽʧ��","��ȡ�ļ���Ϣʧ��","��ȡ��ǰ����·��ʧ��","���������������쳣������񲻿��ã������رտ������ӵ�",
							"�ļ�������","��ǰ�������ϲ�Ŀ¼","�ı乤��·��ʧ��","��ȡ�ļ���Сʧ��","�����ļ�ʧ��","�ϴ��ļ�ʧ��"};
FTPException::FTPException(ExType errCode)
{
	this->errCode = errCode;
	this->detail = detailTab[(int)errCode];
}

FTPException::FTPException(ExType errCode, std::string errInfo):FTPException(errCode)
{
	this->errInfo = errInfo;
}

std::string FTPException::getDetail()
{
	return this->detail;
}

ExType FTPException::getErrorCode()
{
	return this->errCode;
}

void FTPException::print()
{
	std::string info;
	if (errInfo == "") info = "unknown";
	else info = errInfo;
	std::cout << "�������:E" << (int)errCode << ",������Ϣ:" << info << ",����ԭ��:" << detail << std::endl;
}

std::string FTPException::printInfo()
{
	std::string info;
	if (errInfo == "") info = "unknown";
	else info = errInfo;
	std::string ret = "�������:E" + std::to_string((int)errCode) + ",������Ϣ:" + info + ",����ԭ��:" + detail;
	return ret;
}