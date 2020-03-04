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
    //发送命令
    void SendCmd(const std::string & cmd);
    //接收来自服务器的响应
    void RecvResponse();
    //记录信息
    void Log(log_type type, const std::string & cmd);
    //获取最后一条交互信息,用于验证命令是否执行成功
    std::string GetLastLog() const;
    void DisplayLog() const;

private:
    MySocket sock_cmd; //发送接收命令的socket
    std::vector<Record> vec_rcd; //保存此次客户端运行的交互信息
    //将信息记录到文本文件中
    void WriteRecord();
};

