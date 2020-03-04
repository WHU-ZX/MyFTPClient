#pragma once
#include <string>
#include "Record.h"
#include <vector>
#include "MySocket.h"

class Logger
{
public:
    Logger(const std::string& host, int port);
    ~Logger();
    Logger(const Logger & logger) = delete;
    Logger & operator=(const Logger & logger) = delete;
    //��������
    void SendCmd(const std::string & cmd);
    //�������Է���������Ӧ
    void RecvResponse();
    //��¼��Ϣ
    void Log(log_type type, const std::string & cmd);
    //��ȡ���һ��������Ϣ,������֤�����Ƿ�ִ�гɹ�
    std::string GetLastLog() const;
    void DisplayLog() const;

private:
    MySocket sock_cmd; //���ͽ��������socket
    std::vector<Record> vec_rcd; //����˴οͻ������еĽ�����Ϣ
    //����Ϣ��¼���ı��ļ���
    void WriteRecord();
};

