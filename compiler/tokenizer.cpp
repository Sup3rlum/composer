#include "tokenizer.h"

#include <sstream>

Token SourceStream::ReadToken()
{
    char currentChar = input[position];
    std::string buffer = "";

    // Remove whitespaces
    while (IsWhiteSpace(currentChar))
    {
        currentChar = input[++position];
    }

    // Start matching a word
    if (IsLetter(currentChar))
    {
        int wpos = position;
        char examinedChar = currentChar;
        while (IsAlphanumeric(examinedChar) && wpos < input.length())
        {
            buffer.push_back(examinedChar);
            examinedChar = input[++wpos];
        }
        position = wpos;

        /* Match nay keywords */
        if (buffer == "if")
            return Token{ TokenType::KwIf, buffer };
        else if (buffer == "else")
            return Token{ TokenType::KwElse, buffer };
        else if (buffer == "struct")
            return Token{ TokenType::KwStruct, buffer };
        else if (buffer == "for")
            return Token{ TokenType::KwFor, buffer };
        else if (buffer == "while")
            return Token{ TokenType::KwWhile, buffer };
        else if (buffer == "comp")
            return Token{ TokenType::KwComp, buffer };
        else if (buffer == "return")
            return Token{ TokenType::KwReturn, buffer };
        else if (buffer == "void")
            return Token{ TokenType::KwVoid, buffer };
        else if (buffer == "constexpr")
            return Token{ TokenType::KwVoid, buffer };
        else if (buffer == "match")
            return Token{ TokenType::KwVoid, buffer };
        else if (buffer == "func")
            return Token{ TokenType::KwFunc, buffer };
        else if (buffer == "let")
            return Token{ TokenType::KwLet, buffer };
        else if (buffer == "const")
            return Token{ TokenType::KwConst, buffer };
        else if (buffer == "var")
            return Token{ TokenType::KwVar, buffer };
        else if (buffer == "enum")
            return Token{ TokenType::KwEnum, buffer };
        else
            return Token{ TokenType::Label, buffer };
    }
    // Match numeric data
    else if (IsDigit(currentChar))
    {
        bool foundDecimal = false;
        int wpos = position;
        char examinedChar = currentChar;
        while ((IsDigit(examinedChar) || examinedChar == '.') && wpos < input.length())
        {
            buffer.push_back(examinedChar);
            examinedChar = input[++wpos];
        }
        position = wpos;
        return Token{TokenType::Numeric, buffer};
    }

    // Match Operators and brackets
    else
    {
        position++;
        switch (currentChar)
        {
        /* Brackets */
        case '(':
            return Token{TokenType::LParen, std::string{currentChar}};
            break;
        case '<':
            return Token{TokenType::LTriangle, std::string{currentChar}};
            break;
        case '[':
            return Token{TokenType::LSquare, std::string{currentChar}};
            break;
        case '{':
            return Token{TokenType::LCurly, std::string{currentChar}};
            break;
        case ')':
            return Token{TokenType::RParen, std::string{currentChar}};
            break;
        case '>':
            return Token{TokenType::RTriangle, std::string{currentChar}};
            break;
        case ']':
            return Token{TokenType::RSquare, std::string{currentChar}};
            break;
        case '}':
            return Token{TokenType::RCurly, std::string{currentChar}};
            break;

        /* Operators */
        case '=':
            return Token{TokenType::OpEqual, std::string{currentChar}};
            break;
        case '.':
            return Token{TokenType::OpDot, std::string{currentChar}};
            break;
        case '+':
            return Token{TokenType::OpPlus, std::string{currentChar}};
            break;
        case '-':
            return Token{TokenType::OpMinus, std::string{currentChar}};
            break;
        case '*':
            return Token{TokenType::OpAsterisk, std::string{currentChar}};
            break;
        case '/':
            return Token{TokenType::OpSlash, std::string{currentChar}};
            break;
        case '!':
            return Token{TokenType::OpExcl, std::string{currentChar}};
            break;
        case ',':
            return Token{TokenType::OpComma, std::string{currentChar}};
            break;
        case ';':
            return Token{TokenType::OpSemicolon, std::string{currentChar}};
            break;
        case ':':
            return Token{ TokenType::OpColon, std::string{currentChar} };
            break;
        default:
            break;
        }
    }
}

std::vector<Token> Tokenizer::Tokenize(const std::string &input)
{
    std::vector<Token> tokenList;
    auto source = SourceStream(input);

    while (source.Position() < source.End() - 1)
    {
        Token token = source.ReadToken();
        tokenList.push_back(token);
    }

    return tokenList;
}

bool SourceStream::IsLetter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool SourceStream::IsDigit(char c)
{
    return (c >= '0' && c <= '9');
}
bool SourceStream::IsLetterOrDigit(char c)
{
    return IsLetter(c) || IsDigit(c);
}
bool SourceStream::IsAlphanumeric(char c)
{
    return IsLetterOrDigit(c) || c == '_';
}
bool SourceStream::IsWhiteSpace(char c)
{
    return (c == '\n' || c == '\r' || c == ' ' || c == '\t');
}
