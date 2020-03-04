// FTP_SOCKET.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "FTPClient.h"
#include "FTPException.h"

int main()
{
	std::string host = "175.24.105.148";
	std::string usr = "ubuntu";
	std::string pwd = "ZOUXINdefuwuqi1";
	int port = 21;
	FTPClient* client = nullptr;
	try
	{
		client = new FTPClient(host, port);
		client->Login(usr, pwd);
		client->EnterPasvMode();
		client->getFilesOfCurWorkDir();
		//std::string dir = client->getCurWorkingDir();
		client->test();
	}
	catch (FTPException e)
	{
		e.print();
	}


	/*  //要打开下面的，只需关闭左侧的/*即可
	////////////////////////////
	WSADATA wsaData;
	WORD wVersionRequested;
	int err;
	wVersionRequested = MAKEWORD(2, 0);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
	{
		WSACleanup();
		return 0;
	}

    sockaddr_in addr;
    SOCKET ctl_socket;
	memset((void*)&addr, 0, sizeof(addr));//将地址清0
	addr.sin_family = AF_INET;
	addr.sin_port = htons(21);
	addr.sin_addr.S_un.S_addr = inet_addr("175.24.105.148");
	ctl_socket = socket(AF_INET, SOCK_STREAM, 0);

	int ret;
	ret = connect(ctl_socket, (LPSOCKADDR)&addr, sizeof(addr));
	if (ret < 0)
	{
		closesocket(ctl_socket);
		return -2;
	}
	char buf[10000];
	//while (ret = recv(ctl_socket, buf, 10000, 0))
	//{
	//	buf[ret] = '\0';
	//	//std::cout << buf << std::endl;
	//}
	ret = recv(ctl_socket, buf, 10000, 0);
	char* send_buf = new char[100];

	sprintf(send_buf, "USER %s\r\n", "ubuntu");
	ret = send(ctl_socket, send_buf, strlen(send_buf), 0);
	ret = recv(ctl_socket, buf, 10000, 0);

	sprintf(send_buf, "PASS %s\r\n", "ZOUXINdefuwuqi1");
	ret = send(ctl_socket, send_buf, strlen(send_buf), 0);
	ret = recv(ctl_socket, buf, 10000, 0);
	/*char userCtl[] = "USER ubuntu\r\n";
	char pwdCtl[] = "PASS ZOUXINdefuwuqi1\r\n";*/
	
    std::cout << "Hello World!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
