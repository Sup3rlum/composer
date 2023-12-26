#pragma once

#include "Token.h"
#include <iostream>
#include <stack>
#include <string_view>
#include <unordered_map>
#include <vector>

class Name;

class SourceStream
{
    typedef std::vector<std::string> SourceLines;

  private:
    // SourceLines lines;
    std::string sourceName;
    std::string input;
    int currentLine;
    int currentLineChar;
    int position;

  public:
    SourceStream(const std::string &name, const std::string &input)
        : sourceName(name), input(input), position(0), currentLine(0), currentLineChar(0)
    {
    }
    Token ReadToken();
    SourceLocation CurrLocation();

    char GetChar();
    char GetNextChar();
    char PeekChar();

    bool EndOf()
    {
        return position >= input.length() - 1;
    }

    bool IsLetter(char);
    bool IsLetterOrDigit(char);
    bool IsDigit(char);
    bool IsAlphanumeric(char);
    bool IsWhiteSpace(char);
};

class TokenStream
{
  public:
    TokenStream(const std::vector<Token> &tokens) : tokens(tokens), position(0)
    {
    }
    // bool Match(const std::vector<TokenType>& str);

    template <typename... TArgs> bool Match(TArgs... toks)
    {
        const std::vector<TokenType> &str = {toks...};

        if (tokens.size() - position < str.size())
            return false;
        for (int i = 0; i < str.size(); i++)
        {
            if (tokens[position + i].type != str[i])
                return false;
        }
        position += str.size();
        return true;
    }

    bool MatchLabel(std::string &outName);
    bool MatchNumber(std::string &outValue, bool &outDecimal);
    bool EndOf()
    {
        return position >= tokens.size();
    }
    void Mark()
    {
        marks.push(position);
    }
    Token CurrentToken()
    {
        return tokens[position];
    }
    void Backtrack()
    {
        position = marks.top();
        marks.pop();
    }

    void PushScope(Name *name = NULL)
    {
        scopes.push(name);
    }

    void PopScope()
    {
        scopes.pop();
    }

    Name *TopScope()
    {
        return scopes.top();
    }

  private:
    std::stack<Name *> scopes;
    std::vector<std::string> errorList;
    std::stack<int> marks;

    std::vector<Token> tokens;
    int position;
};

class Tokenizer
{
  public:
    Tokenizer()
    {
    }
    TokenStream Tokenize(SourceStream &source);
};
