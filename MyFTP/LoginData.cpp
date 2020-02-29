#include "pch.h"
#include "LoginData.h"

LoginData::LoginData(std::string ip, std::string username, std::string pwd)
{
	CA2T ca2tIP(ip.c_str()),ca2tUsername(username.c_str()),ca2tPwd(pwd.c_str());
	this->ip = (LPCTSTR)ca2tIP;
	this->username = (LPCTSTR)ca2tUsername;
	this->pwd = (LPCTSTR)ca2tPwd;
}