#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>


enum class TokenType
{
	OpEqual,
	OpPlus,
	OpMinus,
	OpAsterisk,
	OpWedge,
	OpSlash,
	OpDot,
	OpComma,
	OpSemicolon,
	OpExcl,

	Label,
	Numeric,

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

class Tokenizer
{
public:
	Tokenizer() {}
	std::vector<Token> Tokenize(const std::string& str);
private:

	bool IsLetter(char);
	bool IsLetterOrDigit(char);
	bool IsDigit(char);
	bool IsAlphanumeric(char);

};