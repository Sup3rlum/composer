#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string_view>


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

	bool EndOf() { return position >= tokens.size(); }


private:
	const std::vector<Token>& tokens;
	int position;
};