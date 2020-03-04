#pragma once
#include <iostream>

//枚举类型,CMD代表命令信息,RES代表响应信息
enum log_type { CMD = 1, RES = 2 };
class Record
{
	friend std::ostream& operator<<(std::ostream& os, const Record& rcd);

public:
	Record(log_type t, std::string m);
	Record(const Record & rcd);
	Record & operator=(const Record & rcd);
	//获取信息内容
	std::string GetMsg() const;

private:
	log_type type; //信息类型
	std::string msg;
};

