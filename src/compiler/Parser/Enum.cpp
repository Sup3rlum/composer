#include "Parser.h"

PResult<EnumDefinition> Parser::ParseEnumDefinition(TokenStream &tokens)
{
    if (!tokens.Match(TokenType::KwEnum))
        return NONMATCH();

    auto enumName = ParseName(tokens);
    if (!enumName)
        return ERROR("Expected an identifier");

    if (!tokens.Match(TokenType::LCurly))
        return ERROR("Expected '{'");

    Name *entry = ParseName(tokens);
    if (!entry)
        return ERROR("Expected identifier");

    std::vector<Name *> values;

    auto argComma = tokens.Match(TokenType::OpComma);
    if (argComma)
    {
        entry = ParseName(tokens);
        if (!entry)
            return ERROR("Expected an identifier");

        values.push_back(entry);

        while (entry && argComma)
        {
            argComma = tokens.Match(TokenType::OpComma);
            entry = ParseName(tokens);

            if (argComma && !entry)
                return ERROR("Expected an identifier");
            if (entry)
                values.push_back(entry);
        }
    }

    if (!tokens.Match(TokenType::RCurly))
        return ERROR("Expected '}'");

    return new EnumDefinition{enumName, values};
}
