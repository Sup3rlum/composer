#include "string_util.h"

std::vector<std::string> SplitString(std::string val, char delimiter)
{
    std::stringstream test(val);
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(test, segment, delimiter))
    {
        seglist.push_back(segment);
    }
    return seglist;
}
