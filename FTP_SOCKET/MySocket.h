#pragma once
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <string>

class MySocket
{
public:
	MySocket();
	virtual ~MySocket();
	//重载向SOCKET类型转换的运算符
	operator SOCKET() const;//当将MySocket隐式转换为SOCKET的时候调用此函数
	//设置地址信息
	void SetAddrInfo(std::string host, int port);
	bool Connect();

	bool Connected();//是否已连接到服务器
	//bool Logined();//是否已经登录

	bool Disconnect();
	bool Create(int af = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
	bool Close();
	//获取主机ip
	std::string GetHostIP() const;
	//获取主机端口
	int GetPort() const;
	int Receive(char* buf, int len, int flags);
	int Send(char* send_buf, int len, int flags);

	int getSocketLoaclPort();

private:
	SOCKET sock;
	SOCKADDR_IN addr_in; //记录连接的服务器的地址信息
	bool conn_flag = false; //判断是否已连接
	bool login_flag = false;//判断当前是否处于登陆状态
};

