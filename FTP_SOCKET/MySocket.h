#pragma once
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <string>

class MySocket
{
public:
	MySocket();
	virtual ~MySocket();
	//������SOCKET����ת���������
	operator SOCKET() const;//����MySocket��ʽת��ΪSOCKET��ʱ����ô˺���
	//���õ�ַ��Ϣ
	void SetAddrInfo(std::string host, int port);
	bool Connect();

	bool Connected();//�Ƿ������ӵ�������
	//bool Logined();//�Ƿ��Ѿ���¼

	bool Disconnect();
	bool Create(int af = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
	bool Close();
	//��ȡ����ip
	std::string GetHostIP() const;
	//��ȡ�����˿�
	int GetPort() const;
	int Receive(char* buf, int len, int flags);
	int Send(char* send_buf, int len, int flags);

	int getSocketLoaclPort();

private:
	SOCKET sock;
	SOCKADDR_IN addr_in; //��¼���ӵķ������ĵ�ַ��Ϣ
	bool conn_flag = false; //�ж��Ƿ�������
	bool login_flag = false;//�жϵ�ǰ�Ƿ��ڵ�½״̬
};

