#include "./utils.hpp"

std::vector<std::string> split(std::string content, std::string sparator)
{
    std::vector<std::string> splt;
    std::string res;
    size_t pos;

    int seplength = sparator.length();
    pos = content.find(sparator);
    while (pos != std::string::npos)
    {
        res = content.substr(0, pos);
        if (res.length() > 0)
            splt.push_back(res);
        content.erase(0, pos + seplength);
        pos = content.find(sparator);
    }
    if (content.length() > 0)
        splt.push_back(content);
    return splt;
}

std::string trim(std::string content, std::string sparator)
{
    int pos = content.find(sparator);
    while (pos == 0)
    {
        content.erase(0, sparator.length());
        pos = content.find(sparator);
        ;
    }
    pos = content.rfind(sparator);
    while (content[pos + sparator.length()] == '\0')
    {
        content.erase(pos, pos + sparator.length());
        pos = content.rfind(sparator);
    }

    return content;
}