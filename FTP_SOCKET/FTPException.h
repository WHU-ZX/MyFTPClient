#pragma once
#include <string>

enum class ExType{StartUpFail=0,CreateSocketFail,ConnectFail,LoginFail,UsrOrPwdWrong,EnterPasvFail,
					GetFileInfoFail,GetCurDirFail};
class FTPException
{
private:
	ExType errCode;
	std::string detail;

public:
	FTPException(ExType errCode);
	std::string getDetail();
	ExType getErrorCode();
	void print();
};

