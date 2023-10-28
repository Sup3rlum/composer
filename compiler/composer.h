#pragma once

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "tokenizer.h"
#include "parser/parser.h"
#include "console_helper.h"

struct CompilerTask
{
	std::string filePath;
	std::string outputPath;

	CompilerTask(const std::string& file, const std::string& output) : filePath(file), outputPath(outputPath) {}
	int Run();

};