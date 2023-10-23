#include "parser.h"

/* Matches str consecutive characters, if success -> advances stream position by str.size() positions 
bool TokenStream::Match(const std::vector<TokenType>& str)
{
	if (tokens.size() - position < str.size()) return false;
	for (int i = 0; i < str.size(); i++)
	{
		if (tokens[position + i].type != str[i]) return false;
	}
	position += str.size();
	return true;
}*/


template<typename...TArgs>
bool TokenStream::Match(TArgs... toks)
{
	const std::vector<TokenType>& str = { toks ... };

	if (tokens.size() - position < str.size()) return false;
	for (int i = 0; i < str.size(); i++)
	{
		if (tokens[position + i].type != str[i]) return false;
	}
	position += str.size();
	return true;
}

bool TokenStream::MatchLabel(std::string& outName)
{
	if (tokens.size() - position < 1) return false;
	if (tokens[position].type != TokenType::Label) return false;
	outName = tokens[position].data;
	position++;
	return true;
}

AST* Parser::GenerateAST(const std::vector<Token>& tokens)
{
	TokenStream stream{ tokens };
	std::vector<TopLevelStatement*> Statements;
	while (!stream.EndOf())
	{
		auto stmnt = ParseTopLevelStatement(stream);
		if (!stmnt) return NULL;

		Statements.push_back(stmnt);
	}
	return new AST(Statements);
}

Expression* Parser::ParseExpression(TokenStream& tokens)
{



	return NULL;
}

Statement* Parser::ParseStatement(TokenStream& tokens)
{
	Statement* stmnt = ParseIf(tokens);
	if (stmnt) return stmnt;

	stmnt = ParseFor(tokens);
	if (stmnt) return stmnt;

	stmnt = ParseWhile(tokens);
	if (stmnt) return stmnt;

	return NULL;
}

TopLevelStatement* Parser::ParseTopLevelStatement(TokenStream& tokens)
{
	TopLevelStatement* stmnt = ParseFunctionDefinition(tokens);
	if (stmnt) return stmnt;

	stmnt = ParseStructDefinition(tokens);
	if (stmnt) return stmnt;

	return NULL;
}

Block* Parser::ParseBlock(TokenStream& tokens)
{
	bool matchOpenCurly = tokens.Match(TokenType::LCurly);
	if (!matchOpenCurly) return NULL;

	std::vector<Statement*> statements;
	auto parsedStatement = ParseStatement(tokens);
	if (parsedStatement && !tokens.Match(TokenType::OpSemicolon)) return NULL;

	while (parsedStatement)
	{
		statements.push_back(parsedStatement);
		parsedStatement = ParseStatement(tokens);

		bool trailingSemicolon = tokens.Match(TokenType::OpSemicolon);
		if (!trailingSemicolon) return NULL;
	}

	bool matchCloseCurly = tokens.Match(TokenType::RCurly);
	if (!matchCloseCurly) return NULL;

	return new Block(statements);
}

IfStatement* Parser::ParseIf(TokenStream& tokens)
{
	bool matchStartCond = tokens.Match(TokenType::KwIf, TokenType::LParen);
	if (!matchStartCond) return NULL;

	auto condition = ParseExpression(tokens);
	if (!condition) return NULL;

	bool matchEndCond = tokens.Match(TokenType::RParen);
	if (!matchEndCond) return NULL;

	auto block = ParseBlock(tokens);
	if (!block) return NULL;

	return new IfStatement{ condition, block };
}

WhileStatement* Parser::ParseWhile(TokenStream& tokens)
{
	bool matchStartCond = tokens.Match(TokenType::KwWhile, TokenType::LParen);
	if (!matchStartCond) return NULL;

	auto condition = ParseExpression(tokens);
	if (!condition) return NULL;

	bool matchEndCond = tokens.Match(TokenType::RParen);
	if (!matchEndCond) return NULL;

	auto block = ParseBlock(tokens);
	if (!block) return NULL;

	return new WhileStatement{ condition, block };
}

ForStatement* Parser::ParseFor(TokenStream& tokens)
{
	bool matchStartCond = tokens.Match(TokenType::KwFor, TokenType::LParen);
	if (!matchStartCond) return NULL;

	auto init = ParseStatement(tokens);
	if (!init) return NULL;

	bool firstSemicolon = tokens.Match(TokenType::OpSemicolon);
	if (!firstSemicolon) return NULL;

	auto cond = ParseExpression(tokens);
	if (!cond) return NULL;

	bool secondSemicolon = tokens.Match(TokenType::OpSemicolon);
	if (!secondSemicolon) return NULL;

	auto iter = ParseStatement(tokens);
	if (!iter) return NULL;

	bool matchEndCond = tokens.Match(TokenType::RParen);
	if (!matchEndCond) return NULL;

	auto block = ParseBlock(tokens);
	if (!block) return NULL;

	return new ForStatement{ init, cond, iter, block };
}

FunctionCall* Parser::ParseFunctionCall(TokenStream& tokens)
{
	std::string funcName = "";
	bool funcLabel = tokens.MatchLabel(funcName);
	if (!funcLabel) return NULL;

	std::vector<Expression*> inputArguments;
	if (!tokens.Match(TokenType::LParen)) 
		return NULL;

	auto inputArg = ParseExpression(tokens);
	if (inputArg) // Non empty function call arguments
	{
		inputArguments.push_back(inputArg);
		bool argComma = tokens.Match(TokenType::OpComma);
		if (argComma) // More than one argument
		{
			inputArg = ParseExpression(tokens);
			if (!inputArg) return NULL;
			inputArguments.push_back(inputArg);

			while (inputArg && argComma)
			{
				argComma = tokens.Match(TokenType::OpComma);
				inputArg = ParseExpression(tokens);

				if (argComma && !inputArg) return NULL;
				inputArguments.push_back(inputArg);

			}
		}
	}

	if (!tokens.Match(TokenType::RParen))
		return NULL;

	return new FunctionCall{ funcName, inputArguments };
}

FunctionParameter* Parser::ParseFunctionParameter(TokenStream& tokens)
{
	std::string paramName = "";
	bool paramLabel = tokens.MatchLabel(paramName);
	if (!paramLabel) return NULL;

	if (!tokens.Match(TokenType::OpSemicolon)) return NULL;

	std::string typeName = "";
	bool typeLabel = tokens.MatchLabel(typeName);
	if (!typeLabel) return NULL;

	return new FunctionParameter(paramName, typeName);
}

FunctionDefinition* Parser::ParseFunctionDefinition(TokenStream& tokens)
{
	if (!tokens.Match(TokenType::KwFunc))
		return NULL;

	std::string funcName = "";
	bool funcLabel = tokens.MatchLabel(funcName);
	if (!funcLabel) return NULL;

	if (!tokens.Match(TokenType::LParen))
		return NULL;

	std::vector<FunctionParameter*> inputParameters;

	auto inputParam = ParseFunctionParameter(tokens);
	if (inputParam) // Non empty function call arguments
	{
		inputParameters.push_back(inputParam);
		bool paramComma = tokens.Match(TokenType::OpComma);
		if (paramComma) // More than one argument
		{
			inputParam = ParseFunctionParameter(tokens);
			if (!inputParam) return NULL;
			inputParameters.push_back(inputParam);

			while (inputParam && paramComma)
			{
				paramComma = tokens.Match(TokenType::OpComma);
				inputParam = ParseFunctionParameter(tokens);

				if (paramComma && !inputParam) return NULL;
				inputParameters.push_back(inputParam);
			}
		}
	}

	bool retArrow = tokens.Match(TokenType::RParen, TokenType::OpMinus, TokenType::RTriangle);
	if (!retArrow)
		return NULL;

	std::string retTypeName = "";
	bool retTypeLabel = tokens.MatchLabel(retTypeName);
	if (!retTypeLabel) return NULL;

	auto body = ParseBlock(tokens);
	if (!body) return NULL;

	return new FunctionDefinition{ retTypeName, funcName, inputParameters, body };
}

StructDefinition* Parser::ParseStructDefinition(TokenStream& tokens)
{
	if (!tokens.Match(TokenType::KwStruct))
		return NULL;

	std::string structName = "";
	bool structLabel = tokens.MatchLabel(structName);
	if (!structLabel) return NULL;

	auto body = ParseBlock(tokens);
	if (!body) return NULL;

	return new StructDefinition{ structName, body };
}

Factor* Parser::ParseFactor(TokenStream& tokens)
{
	if (!tokens)
}

CompoundFactor* Parser::ParseCompoundFactor(TokenStream& tokens)
{
	if (!tokens.Match(TokenType::LParen)) 
		return NULL;

	auto expr = ParseExpression(tokens);

	if (!tokens.Match(TokenType::RParen))
		return NULL;


	return new CompoundFactor(expr);
}