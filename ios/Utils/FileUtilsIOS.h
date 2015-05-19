//文件 处理工具



#pragma once

#include <string>


class FileUtilsIOS
{
public:
    static bool createDirectory(std::string path);
    static bool createFile(std::string absFilepath);
};
