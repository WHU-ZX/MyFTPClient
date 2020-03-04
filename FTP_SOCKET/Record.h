#pragma once
#include <iostream>

//ö������,CMD����������Ϣ,RES������Ӧ��Ϣ
enum log_type { CMD = 1, RES = 2 };
class Record
{
	friend std::ostream& operator<<(std::ostream& os, const Record& rcd);

public:
	Record(log_type t, std::string m);
	Record(const Record & rcd);
	Record & operator=(const Record & rcd);
	//��ȡ��Ϣ����
	std::string GetMsg() const;

private:
	log_type type; //��Ϣ����
	std::string msg;
};

