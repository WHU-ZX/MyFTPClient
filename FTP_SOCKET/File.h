#pragma once
#include <iostream>
class File
{
    friend std::ostream& operator<<(std::ostream& os, const File& file);
public:
    //斜杠代表根目录
    File(const std::string & n = "", const std::string & t = "", const int& s = 0, const std::string & p = "/");
    int GetSize() const;
private:
    std::string name;
    std::string path;
    std::string create_time;
    int size;
};

