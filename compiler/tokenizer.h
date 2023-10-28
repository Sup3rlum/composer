#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string_view>
#include <stack>


enum class TokenType
{

	/* Operators */
	OpEqual,
	OpPlus,
	OpMinus,
	OpAsterisk,
	OpSlash,
	OpWedge,
	OpDot,
	OpComma,
	OpExcl,
	OpSquestion,
	OpSemicolon,
	OpColon,
	OpAmpersand,

	/* KeyWords */
	KwIf,
	KwStruct,
	KwElse,
	KwFor,
	KwWhile,
	KwComp,
	KwReturn,
	KwVoid,
	KwConstexpr,
	KwMatch,
	KwFunc,
	KwConst,
	KwLet,
	KwVar,
	KwEnum,

	Label,
	Numeric,


	/* Brackets */
	LParen,
	RParen,
	LSquare,
	RSquare,
	LCurly,
	RCurly,
	LTriangle,
	RTriangle,

};

struct Token
{
	TokenType type;
	std::string data;
};

class SourceStream
{
private:
	const std::string& input;
	int position;
public:
	SourceStream(const std::string& input) : input(input), position(0) {}
	Token ReadToken();

	int Position() { return position; }
	int End() { return input.length() + 1; }

	bool IsLetter(char);
	bool IsLetterOrDigit(char);
	bool IsDigit(char);
	bool IsAlphanumeric(char);
	bool IsWhiteSpace(char);
};

class Tokenizer
{
public:
	Tokenizer() {}
	std::vector<Token> Tokenize(const std::string& str);

};

class TokenStream
{
public:
	TokenStream(const std::vector<Token>& tokens) : tokens(tokens), position(0) {}
	//bool Match(const std::vector<TokenType>& str);

	template<typename...TArgs>
	bool Match(TArgs... toks);
	bool MatchLabel(std::string& outName);
	bool MatchNumber(std::string& outValue);

	bool EndOf() { return position >= tokens.size(); }

	void Mark() 
	{
		marks.push(position);
	}
	int Error(const std::string& errorName)
	{
		errorList.push_back(errorName);
		return NonMatch();
	}
	int NonMatch()
	{
		position = marks.top();
		marks.pop();
		return NULL;
	}

	auto ErrorList()
	{
		return errorList;
	}

private:

	std::vector<std::string> errorList;
	std::stack<int> marks;

	const std::vector<Token>& tokens;
	int position;
};