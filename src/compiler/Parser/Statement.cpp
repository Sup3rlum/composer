#include "Parser.h"

PResult<Block> Parser::ParseBlock(TokenStream &tokens)
{
    MARK();
    bool matchOpenCurly = tokens.Match(TokenType::LCurly);
    if (!matchOpenCurly)
        return NONMATCH();

    std::vector<Statement *> statements;
    auto parsedStatement = ParseStatement(tokens);

    while (parsedStatement)
    {
        statements.push_back(parsedStatement);
        parsedStatement = ParseStatement(tokens);
    }

    bool matchCloseCurly = tokens.Match(TokenType::RCurly);
    if (!matchCloseCurly)
        return ERROR("Expected '}'");

    return new Block{statements};
}

PResult<Statement> Parser::ParseBlockStatement(TokenStream &tokens)
{
    MARK();
    Statement *stmnt = ParseIf(tokens);
    if (stmnt)
        return stmnt;

    stmnt = ParseFor(tokens);
    if (stmnt)
        return stmnt;

    stmnt = ParseWhile(tokens);
    if (stmnt)
        return stmnt;

    return NONMATCH();
}

PResult<Statement> Parser::ParseNormalStatement(TokenStream &tokens)
{
    MARK();

    Statement *stmnt = ParseVarDeclaration(tokens);
    if (stmnt)
        return stmnt;

    stmnt = ParseReturn(tokens);
    if (stmnt)
        return stmnt;

    return NONMATCH();
}

PResult<Statement> Parser::ParseStatement(TokenStream &tokens)
{
    MARK();

    Statement *stmnt = ParseBlockStatement(tokens);
    if (stmnt)
        return stmnt;

    stmnt = ParseNormalStatement(tokens);
    if (stmnt)
    {
        auto semicolon = tokens.Match(TokenType::OpSemicolon);
        if (!semicolon)
            return ERROR("Expected ';'");
        return stmnt;
    }
    return NONMATCH();
}

PResult<IfStatement> Parser::ParseIf(TokenStream &tokens)
{
    MARK();
    bool matchStartCond = tokens.Match(TokenType::KwIf, TokenType::LParen);
    if (!matchStartCond)
        return NONMATCH();

    auto condition = ParseExpression(tokens);
    if (!condition)
        return ERROR("Expected an expression");

    bool matchEndCond = tokens.Match(TokenType::RParen);
    if (!matchEndCond)
        return ERROR(")");

    auto block = ParseBlock(tokens);
    if (!block)
        return ERROR("Expected a body");

    return new IfStatement{condition, block};
}

PResult<WhileStatement> Parser::ParseWhile(TokenStream &tokens)
{
    MARK();
    bool matchStartCond = tokens.Match(TokenType::KwWhile, TokenType::LParen);
    if (!matchStartCond)
        return NONMATCH();

    auto condition = ParseExpression(tokens);
    if (!condition)
        return ERROR("Expected an expression");

    bool matchEndCond = tokens.Match(TokenType::RParen);
    if (!matchEndCond)
        return ERROR(")");

    auto block = ParseBlock(tokens);
    if (!block)
        return ERROR("Expected a body");

    return new WhileStatement{condition, block};
}

PResult<ForStatement> Parser::ParseFor(TokenStream &tokens)
{
    MARK();
    bool matchStartCond = tokens.Match(TokenType::KwFor, TokenType::LParen);
    if (!matchStartCond)
        return NONMATCH();

    auto init = ParseStatement(tokens);
    if (!init)
        return ERROR("Expected initializer statement");

    bool firstSemicolon = tokens.Match(TokenType::OpSemicolon);
    if (!firstSemicolon)
        return ERROR("Expected ';'");

    auto cond = ParseExpression(tokens);
    if (!cond)
        return ERROR("Expected an expression");

    bool secondSemicolon = tokens.Match(TokenType::OpSemicolon);
    if (!secondSemicolon)
        return ERROR("Expected ';'");

    auto iter = ParseStatement(tokens);
    if (!iter)
        return ERROR("Expected an iterator statement");

    bool matchEndCond = tokens.Match(TokenType::RParen);
    if (!matchEndCond)
        return ERROR("Expected ')'");

    auto block = ParseBlock(tokens);
    if (!block)
        return ERROR("Expected a body");

    return new ForStatement{init, cond, iter, block};
}

PResult<ReturnStatement> Parser::ParseReturn(TokenStream &tokens)
{
    MARK();
    if (!tokens.Match(TokenType::KwReturn))
        return NONMATCH();

    auto expr = ParseExpression(tokens);
    if (!expr)
        return ERROR("Expected an expression");

    return new ReturnStatement{expr};
}
