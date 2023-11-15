#include "parser.h"

PResult<AST> Parser::GenerateAST(TokenStream &tokens)
{
    std::vector<ModuleStatement *> Statements;
    while (!tokens.EndOf())
    {
        auto stmnt = ParseModuleStatement(tokens);
        if (!stmnt)
        {
            return ERROR("Error parsing source file");
        }

        Statements.push_back(stmnt);
    }

    return new AST{Statements};
}
