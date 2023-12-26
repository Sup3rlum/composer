#include "Parser.h"

PResult<AST> Parser::GenerateAST(TokenStream &tokens)
{
    std::vector<ModuleStatement *> Statements;
    while (!tokens.EndOf())
    {
        auto stmnt = ParseModuleStatement(tokens);
        if (!stmnt)
        {
            return ERROR(stmnt);
        }
        Statements.push_back(stmnt);
    }

    return new AST{Statements};
}
