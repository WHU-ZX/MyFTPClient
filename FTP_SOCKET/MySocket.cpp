#include "MySocket.h"
#include "FTPException.h"
#include <windows.h>
#define SLEEP_TIME (100)

MySocket::MySocket()
{

}

MySocket::~MySocket()
{

}

//���õ�ַ��Ϣ
void MySocket::SetAddrInfo(std::string host, int port)
{
	memset((void*)&addr_in, 0, sizeof(addr_in));//����ַ��0
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(port);
	addr_in.sin_addr.S_un.S_addr = inet_addr(host.c_str());
}

bool MySocket::Connect()//connect���ص���ϢҪ���洦��
{
	int err = connect(sock, (LPSOCKADDR)&addr_in, sizeof(addr_in));
	if (err == 0)
	{
		conn_flag = true;
	}
	return err == 0;
}

bool MySocket::Connected()//�Ƿ������ӵ�������
{
	return conn_flag;
}

//bool MySocket::Logined()
//{
//	return login_flag;
//}

int MySocket::Receive(char* buf, int len, int flags)
{
	Sleep(SLEEP_TIME);
	return recv(sock, buf, len, flags);
}

int MySocket::Send(char* send_buf, int len, int flags)
{
	return send(sock, send_buf, len, flags);
}

bool MySocket::Disconnect()
{
	closesocket(sock);
	return true;
}

bool MySocket::Create(int af, int type, int protocol)
{
	sock = socket(af, type, protocol);
	return sock != INVALID_SOCKET;//����socket�Ƿ���Ч
}

int MySocket::getSocketLoaclPort()
{
	sockaddr_in addr;
	int len = sizeof(addr);
	getsockname(sock, (struct sockaddr*) & addr, &len);
	return ntohs(addr.sin_port);
}

bool MySocket::Close()
{

	return true;
}

//��ȡ����ip
std::string MySocket::GetHostIP() const
{
	char * pC = inet_ntoa(addr_in.sin_addr);
	return pC;
}

//��ȡ�����˿�
int MySocket::GetPort() const
{
	return ntohs(addr_in.sin_port);
}

MySocket::operator SOCKET() const
{
	return sock;
}

