#include "FTPException.h"
#include <iostream>

std::string detailTab[] = {"启动WinSocket库失败","创建Socket失败","连接服务器失败","登录失败","账号或密码错误",
							"进入被动模式失败","获取文件信息失败","获取当前工作路径失败"};
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
	std::cout << "错误代码:E" << (int)errCode << ",错误原因:" << detail << std::endl;
}