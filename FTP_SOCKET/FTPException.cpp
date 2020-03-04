#include "FTPException.h"
#include <iostream>

std::string detailTab[] = {"����WinSocket��ʧ��","����Socketʧ��","���ӷ�����ʧ��","��¼ʧ��","�˺Ż��������",
							"���뱻��ģʽʧ��","��ȡ�ļ���Ϣʧ��","��ȡ��ǰ����·��ʧ��"};
FTPException::FTPException(ExType errCode)
{
	this->errCode = errCode;
	this->detail = detailTab[(int)errCode];
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
	std::cout << "�������:E" << (int)errCode << ",����ԭ��:" << detail << std::endl;
}