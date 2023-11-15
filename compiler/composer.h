#pragma once

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Util/console_helper.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>

struct CompilerTask
{
    std::string filePath;
    std::string outputPath;

    CompilerTask(const std::string &file, const std::string &output) : filePath(file), outputPath(outputPath)
    {
    }
    int Run();
};
