#include "FTPException.h"
#include <iostream>

std::string detailTab[] = {"启动WinSocket库失败","创建Socket失败","连接服务器失败","登录失败","账号或密码错误",
							"进入被动模式失败","获取文件信息失败","获取当前工作路径失败","其他错误：如网络异常，或服务不可用，即将关闭控制连接等",
							"文件不可用","当前已是最上层目录","改变工作路径失败","获取文件大小失败","下载文件失败","上传文件失败"};
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
	std::cout << "错误代码:E" << (int)errCode << ",错误信息:" << info << ",错误原因:" << detail << std::endl;
}

std::string FTPException::printInfo()
{
	std::string info;
	if (errInfo == "") info = "unknown";
	else info = errInfo;
	std::string ret = "错误代码:E" + std::to_string((int)errCode) + ",错误信息:" + info + ",错误原因:" + detail;
	return ret;
}