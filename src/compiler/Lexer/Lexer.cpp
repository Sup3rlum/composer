#include "Lexer.h"
#include "Token.h"
#include <sstream>

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

char SourceStream::GetNextChar()
{
    char c = PeekChar();
    if (c != -1)
    {
        position++;
        return c;
    }
    return -1;
}

char SourceStream::GetChar()
{
    return input[position];
}

char SourceStream::PeekChar()
{
    if (position + 1 >= input.length())
        return -1;
    return input[position + 1];
}

SourceLocation SourceStream::CurrLocation()
{
    return SourceLocation{sourceName, currentLine, currentLineChar};
}

Token SourceStream::ReadToken()
{
    char currentChar = GetChar();
    std::string buffer = "";

    // Remove whitespaces
    while (IsWhiteSpace(currentChar))
    {
        if (currentChar == '\n')
        {
            currentLine++;
            currentLineChar = 0;
        }
        currentChar = GetNextChar();
    }
    auto currentLocation = CurrLocation();

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
        TokenType tokenType;

        /* Match nay keywords */
        if (buffer == "if")
            tokenType = TokenType::KwIf;
        else if (buffer == "else")
            tokenType = TokenType::KwElse;
        else if (buffer == "struct")
            tokenType = TokenType::KwStruct;
        else if (buffer == "for")
            tokenType = TokenType::KwFor;
        else if (buffer == "while")
            tokenType = TokenType::KwWhile;
        else if (buffer == "comp")
            tokenType = TokenType::KwComp;
        else if (buffer == "return")
            tokenType = TokenType::KwReturn;
        else if (buffer == "void")
            tokenType = TokenType::KwVoid;
        else if (buffer == "constexpr")
            tokenType = TokenType::KwConstexpr;
        else if (buffer == "match")
            tokenType = TokenType::KwVoid;
        else if (buffer == "func")
            tokenType = TokenType::KwFunc;
        else if (buffer == "let")
            tokenType = TokenType::KwLet;
        else if (buffer == "const")
            tokenType = TokenType::KwConst;
        else if (buffer == "var")
            tokenType = TokenType::KwVar;
        else if (buffer == "enum")
            tokenType = TokenType::KwEnum;
        else
            tokenType = TokenType::Label;

        return Token{currentLocation, tokenType, buffer};
    }
    // Match numeric data
    else if (IsDigit(currentChar))
    {
        bool foundDecimal = false;
        int wpos = position;
        char examinedChar = currentChar;
        while ((IsDigit(examinedChar) || (examinedChar == '.' && !foundDecimal)) && wpos < input.length())
        {
            if (examinedChar == '.')
                foundDecimal = true;
            buffer.push_back(examinedChar);
            examinedChar = input[++wpos];
        }
        position = wpos;
        return Token{currentLocation, TokenType::Numeric, buffer};
    }

    // Match Operators and brackets
    else
    {
        TokenType tokenType;
        position++;
        switch (currentChar)
        {
        /* Brackets */
        case '(':
            tokenType = TokenType::LParen;
            break;
        case '<':
            tokenType = TokenType::LTriangle;
            break;
        case '[':
            tokenType = TokenType::LSquare;
            break;
        case '{':
            tokenType = TokenType::LCurly;
            break;
        case ')':
            tokenType = TokenType::RParen;
            break;
        case '>':
            tokenType = TokenType::RTriangle;
            break;
        case ']':
            tokenType = TokenType::RSquare;
            break;
        case '}':
            tokenType = TokenType::RCurly;
            break;

        /* Operators */
        case '=':
            tokenType = TokenType::OpEqual;
            break;
        case '.':
            tokenType = TokenType::OpDot;
            break;
        case '+':
            tokenType = TokenType::OpPlus;
            break;
        case '-':
            tokenType = TokenType::OpMinus;
            break;
        case '*':
            tokenType = TokenType::OpAsterisk;
            break;
        case '/':
            tokenType = TokenType::OpSlash;
            break;
        case '!':
            tokenType = TokenType::OpExcl;
            break;
        case ',':
            tokenType = TokenType::OpComma;
            break;
        case ';':
            tokenType = TokenType::OpSemicolon;
            break;
        case ':':
            tokenType = TokenType::OpColon;
            break;
        default:
            break;
        }

        return Token{currentLocation, tokenType, std::string{currentChar}};
    }
}

bool TokenStream::MatchLabel(std::string &outName)
{
    if (tokens.size() - position < 1)
        return false;
    if (tokens[position].type != TokenType::Label)
        return false;
    outName = tokens[position].data;
    position++;
    return true;
}

bool TokenStream::MatchNumber(std::string &outValue, bool &outDecimal)
{
    if (tokens.size() - position < 1)
        return false;
    if (tokens[position].type != TokenType::Numeric)
        return false;
    outValue = tokens[position].data;

    for (char c : outValue)
        if (c == '.')
            outDecimal = true;

    position++;
    return true;
}

TokenStream Tokenizer::Tokenize(SourceStream &source)
{
    std::vector<Token> tokenList;

    while (!source.EndOf())
    {
        Token token = source.ReadToken();
        tokenList.push_back(token);
    }

    return TokenStream{tokenList};
}
