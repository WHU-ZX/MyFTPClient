#pragma once
#include <string>

enum class ExType{StartUpFail=0,CreateSocketFail,ConnectFail,LoginFail,UsrOrPwdWrong,EnterPasvFail,
					GetFileInfoFail,GetCurDirFail,OtherFails,FileAccessFail};
class FTPException
{
private:
	ExType errCode;
	std::string detail;
	std::string errInfo = "";

public:
	FTPException(ExType errCode);
	FTPException(ExType errCode, std::string errInfo);
	std::string getDetail();
	ExType getErrorCode();
	void print();
	std::string printInfo();
};

