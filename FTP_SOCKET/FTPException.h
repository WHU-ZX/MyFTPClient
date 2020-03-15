#pragma once
#include <string>

enum class ExType{StartUpFail=0,CreateSocketFail,ConnectFail,LoginFail,UsrOrPwdWrong,EnterPasvFail,
					GetFileInfoFail,GetCurDirFail,OtherFails,FileAccessFail,AtTopDir,ChangeWorkSpaceFail,
					GetFileSizeFail,FileDownloadFail,FileUploadFail,CreateFolderFail,DataUploadFail,
					UploadRefused,SetPosFail,SetBinModeFail,RNFRFail,RNTOFail};
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

