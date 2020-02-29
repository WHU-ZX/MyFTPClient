#pragma once
#include <atlstr.h>
#include <string>
class LoginData
{
public:
	CString ip;
	CString username;
	CString pwd;
	LoginData(std::string ip, std::string username, std::string pwd);
};

