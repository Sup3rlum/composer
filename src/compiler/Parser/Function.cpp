#include "Parser.h"

PResult<FunctionProto> Parser::ParseFunctionProto(TokenStream &tokens)
{
    MARK();

    if (!tokens.Match(TokenType::KwFunc))
        return NONMATCH();

    auto funcName = ParseName(tokens);
    if (!funcName)
        return ERROR("Expected an identifier");

    if (!tokens.Match(TokenType::LParen))
        return ERROR("Expected a '('");

    std::vector<FunctionParameter *> inputParameters;

    auto inputParam = ParseFunctionParameter(tokens);
    if (inputParam) // Non empty function call arguments
    {
        inputParameters.push_back(inputParam);
        bool paramComma = tokens.Match(TokenType::OpComma);
        if (paramComma) // More than one argument
        {
            inputParam = ParseFunctionParameter(tokens);
            if (!inputParam)
                return ERROR("Expected a parameter");
            inputParameters.push_back(inputParam);

            while (inputParam && paramComma)
            {
                paramComma = tokens.Match(TokenType::OpComma);
                inputParam = ParseFunctionParameter(tokens);

                if (paramComma && !inputParam)
                    return ERROR("Expected a parameter");
                if (inputParam)
                    inputParameters.push_back(inputParam);
            }
        }
    }

    bool retArrow = tokens.Match(TokenType::RParen, TokenType::OpMinus, TokenType::RTriangle);
    if (!retArrow)
        return ERROR("Expected a return type");

    auto retTypeName = ParseName(tokens);
    if (!retTypeName)
        return ERROR("Expected an identifier");

    return new FunctionProto{retTypeName, funcName, inputParameters};
}

PResult<FunctionCall> Parser::ParseFunctionCall(TokenStream &tokens)
{
    MARK();

    auto funcName = ParseName(tokens);
    if (!funcName)
        return NONMATCH();

    std::vector<Expression *> inputArguments;
    if (!tokens.Match(TokenType::LParen))
        return NONMATCH();

    auto inputArg = ParseExpression(tokens);
    if (inputArg) // Non empty function call arguments
    {
        inputArguments.push_back(inputArg);
        bool argComma = tokens.Match(TokenType::OpComma);
        if (argComma) // More than one argument
        {
            inputArg = ParseExpression(tokens);
            if (!inputArg)
                return ERROR("Expected an expression");
            inputArguments.push_back(inputArg);

            while (inputArg && argComma)
            {
                argComma = tokens.Match(TokenType::OpComma);
                inputArg = ParseExpression(tokens);

                if (argComma && !inputArg)
                    return ERROR("Expected an expression");
                if (inputArg)
                    inputArguments.push_back(inputArg);
            }
        }
    }

    if (!tokens.Match(TokenType::RParen))
        return ERROR("Expected a ')'");

    return new FunctionCall{funcName, inputArguments};
}

PResult<FunctionParameter> Parser::ParseFunctionParameter(TokenStream &tokens)
{
    tokens.Mark();

    auto paramName = ParseName(tokens);
    if (!paramName)
        return NONMATCH();

    if (!tokens.Match(TokenType::OpColon))
        return ERROR("Expected a ':'");

    auto typeName = ParseName(tokens);
    if (!typeName)
        return ERROR("Expected an identifier");

    return new FunctionParameter{paramName, typeName};
}

PResult<ModuleFunctionDefinition> Parser::ParseModuleFunctionDefinition(TokenStream &tokens)
{
    MARK();

    auto proto = ParseFunctionProto(tokens);
    if (!proto)
        return NONMATCH();

    auto body = ParseBlock(tokens);
    if (!body)
        return ERROR("Expected a function body");

    return new ModuleFunctionDefinition{proto, body};
}

PResult<MemberFunctionDefinition> Parser::ParseMemberFunctionDefinition(TokenStream &tokens)
{
    MARK();

    auto proto = ParseFunctionProto(tokens);
    if (!proto)
        return NONMATCH();

    auto body = ParseBlock(tokens);
    if (!body)
        return ERROR("Expected a function body");

    return new MemberFunctionDefinition{tokens.TopScope(), proto, body};
}
