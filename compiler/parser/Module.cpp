#include "parser.h"

PResult<ModuleStatement> Parser::ParseModuleStatement(TokenStream &tokens)
{
    MARK();
    ModuleStatement *stmnt = ParseModuleFunctionDefinition(tokens);
    if (stmnt)
        return stmnt;

    stmnt = ParseStructDefinition(tokens);
    if (stmnt)
        return stmnt;

    stmnt = ParseCompDefinition(tokens);
    if (stmnt)
        return stmnt;

    stmnt = ParseEnumDefinition(tokens);
    if (stmnt)
        return stmnt;

    return NONMATCH();
}
