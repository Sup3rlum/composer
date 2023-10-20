#include "tokenizer.h"
#include <sstream>



std::vector<Token> Tokenizer::Tokenize(const std::string& input)
{
	std::vector<Token> tokenList;
	//std::stringstream ss;
	//ss << input;
	
	for (int cpos = 0; cpos < input.length(); cpos++)
	{
		char currentChar = input[cpos];
		std::string buffer = "";

		// Skip
		if (currentChar == ' ')
		{
			continue;
		}
		// Start matching a word
		else if (IsLetter(currentChar))
		{
			int wpos = cpos;
			char examinedChar = currentChar;
			while (IsAlphanumeric(examinedChar) && wpos < input.length())
			{
				buffer.push_back(examinedChar);
				examinedChar = input[++wpos];
			}
			cpos = wpos-1;
			tokenList.push_back(Token{ TokenType::Label, buffer });
			buffer = "";
		}
		//match numeric data
		else if (IsDigit(currentChar))
		{
			bool foundDecimal = false;
			int wpos = cpos;
			char examinedChar = currentChar;
			while ((IsDigit(examinedChar) || examinedChar == '.') && wpos < input.length())
			{
				buffer.push_back(examinedChar);
				examinedChar = input[++wpos];
			}
			cpos = wpos-1;
			tokenList.push_back(Token{ TokenType::Numeric, buffer });
			buffer = "";
		}
		// Match Operators and brackets
		else switch (currentChar)
		{
		case '(': tokenList.push_back(Token{ TokenType::LParen,		std::string{ currentChar } }); break;
		case '<': tokenList.push_back(Token{ TokenType::LTriangle,	std::string{ currentChar } }); break;
		case '[': tokenList.push_back(Token{ TokenType::LSquare,	std::string{ currentChar } }); break;
		case '{': tokenList.push_back(Token{ TokenType::LCurly,		std::string{ currentChar } }); break;
		case ')': tokenList.push_back(Token{ TokenType::RParen,		std::string{ currentChar } }); break;
		case '>': tokenList.push_back(Token{ TokenType::RTriangle,	std::string{ currentChar } }); break;
		case ']': tokenList.push_back(Token{ TokenType::RSquare,	std::string{ currentChar } }); break;
		case '}': tokenList.push_back(Token{ TokenType::RCurly,		std::string{ currentChar } }); break;
		case '=': tokenList.push_back(Token{ TokenType::OpEqual,	std::string{ currentChar } }); break;
		case '.': tokenList.push_back(Token{ TokenType::OpDot,		std::string{ currentChar } }); break;
		case '+': tokenList.push_back(Token{ TokenType::OpPlus,		std::string{ currentChar } }); break;
		case '-': tokenList.push_back(Token{ TokenType::OpMinus,	std::string{ currentChar } }); break;
		case '*': tokenList.push_back(Token{ TokenType::OpAsterisk, std::string{ currentChar } }); break;
		case '/': tokenList.push_back(Token{ TokenType::OpSlash,	std::string{ currentChar } }); break;
		case '!': tokenList.push_back(Token{ TokenType::OpExcl,		std::string{ currentChar } }); break;
		case ',': tokenList.push_back(Token{ TokenType::OpComma,	std::string{ currentChar } }); break;
		default:
			break;
		}
	}

	return tokenList;
}

bool Tokenizer::IsLetter(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool Tokenizer::IsDigit(char c)
{
	return (c >= '0' && c <= '9');
}
bool Tokenizer::IsLetterOrDigit(char c)
{
	return IsLetter(c) || IsDigit(c);
}
bool Tokenizer::IsAlphanumeric(char c)
{
	return IsLetterOrDigit(c) || c == '_';
}