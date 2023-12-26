// Composer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "composer.h"
#include "Lexer/Lexer.h"
#include "Util/console_helper.h"
#include <chrono>

#define COMPILE_OK 0
#define COMPILE_ERROR 1

void PrintHelp()
{
    std::cout << "Usage: " << CText<FG_LIGHT_CYAN>("cc.exe") << CText<FG_LIGHT_YELLOW>(" file1 file2 ...") << std::endl;
}

int CompilerTask::Run()
{
    auto fin = std::ifstream(filePath);

    if (!fin.good())
    {
        std::cout << CText<ColorCode::FG_RED>("Error opening file " + filePath) << std::endl;
        return COMPILE_ERROR;
    }

    std::string source((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    auto sourceStream = SourceStream(filePath, source);
    auto tokens = Tokenizer().Tokenize(sourceStream);
    auto ast = Parser().GenerateAST(tokens);

    if (ast.parseErrors.size() > 0 || ast.Ptr == NULL)
    {
        std::cout << CText<FG_RED>("\t - Error compiling: ") << CText<FG_RED>(filePath) << std::endl;
        for (auto error : ast.parseErrors)
        {
            std::cout << CText<FG_RED>("\t\t") << error.sourceLocation.sourceName << ":" << error.sourceLocation.line
                      << ":" << error.sourceLocation.character << " " << CText<FG_RED>(error.message) << std::endl;
        }
        return COMPILE_ERROR;
    }

    std::cout << CText<FG_GREEN>("\t - Compiled source: ") << CText<FG_GREEN>(filePath) << std::endl;
    return COMPILE_OK;
}

int main(int argc, char **argv)
{

    std::vector<std::string> args;
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            args.push_back(argv[i]);
        }

        std::vector<CompilerTask> tasks;
        for (auto &arg : args)
        {
            tasks.push_back(CompilerTask(arg, ""));
        }

        std::cout << "Compiling " << tasks.size() << " source files:" << std::endl;
        std::cout << "_____________________________________\n" << std::endl;

        const auto start{std::chrono::steady_clock::now()};
        for (auto &t : tasks)
        {
            t.Run();
        }
        const auto end{std::chrono::steady_clock::now()};
        const std::chrono::duration<double> elapsed_seconds{end - start};

        std::cout << "\n_____________________________________\n" << std::endl;
        // std::cout << "Done in: " << elapsed_seconds << "!" << std::endl;
    }
    else
    {
        PrintHelp();
    }
}
