#include "parser.h"



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

bool TokenStream::MatchNumber(std::string& outValue)
{
	if (tokens.size() - position < 1) return false;
	if (tokens[position].type != TokenType::Numeric) return false;
	outValue = tokens[position].data;
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
		if (!stmnt)
		{
			auto est = new AST({});
			est->ErrorList = stream.ErrorList();
			return est;
		}

		Statements.push_back(stmnt);
	}

	auto ast = new AST(Statements);
	ast->ErrorList = stream.ErrorList();
	return ast;
}

PResult<Expression> Parser::ParseExpression(TokenStream& tokens)
{
	MARK();
	auto lor = ParseLogicalOr(tokens);
	if (!lor) return NONMATCH();

	return new Expression{ lor };
}

PResult<Statement> Parser::ParseBlockStatement(TokenStream& tokens)
{
	MARK();
	Statement* stmnt = ParseIf(tokens);
	if (stmnt) return stmnt;

	stmnt = ParseFor(tokens);
	if (stmnt) return stmnt;

	stmnt = ParseWhile(tokens);
	if (stmnt) return stmnt;

	return NONMATCH();
}

PResult<Statement> Parser::ParseNormalStatement(TokenStream& tokens)
{
	MARK();

	Statement* stmnt = ParseVarDeclaration(tokens);
	if (stmnt) return stmnt;

	stmnt = ParseReturn(tokens);
	if (stmnt) return stmnt;

	return NONMATCH();
}

PResult<Statement> Parser::ParseStatement(TokenStream& tokens)
{
	MARK();

	Statement* stmnt = ParseBlockStatement(tokens);
	if (stmnt) return stmnt;

	stmnt = ParseNormalStatement(tokens);
	if (stmnt)
	{
		auto semicolon = tokens.Match(TokenType::OpSemicolon);
		if (!semicolon) return ERROR("Expected ';'");
		return stmnt;
	}
	return NONMATCH();
}

PResult<TopLevelStatement> Parser::ParseTopLevelStatement(TokenStream& tokens)
{
	MARK();
	TopLevelStatement* stmnt = ParseTopLevelFunctionDefinition(tokens);
	if (stmnt) return stmnt;

	stmnt = ParseStructDefinition(tokens);
	if (stmnt) return stmnt;

	stmnt = ParseCompDefinition(tokens);
	if (stmnt) return stmnt;

	stmnt = ParseEnumDefinition(tokens);
	if (stmnt) return stmnt;

	return NONMATCH();
}

PResult<Block> Parser::ParseBlock(TokenStream& tokens)
{
	MARK();
	bool matchOpenCurly = tokens.Match(TokenType::LCurly);
	if (!matchOpenCurly) return NONMATCH();

	std::vector<Statement*> statements;
	auto parsedStatement = ParseStatement(tokens);
	//if (parsedStatement && !tokens.Match(TokenType::OpSemicolon)) return ERROR("Expected ';'");
	//if (parsedStatement && !tokens.Match(TokenType::OpSemicolon)) return ERROR("Expected ';'");

	while (parsedStatement)
	{
		statements.push_back(parsedStatement);
		parsedStatement = ParseStatement(tokens);

		//bool trailingSemicolon = tokens.Match(TokenType::OpSemicolon);
		//if (parsedStatement && !trailingSemicolon) return ERROR("Expected ';'");
	}

	bool matchCloseCurly = tokens.Match(TokenType::RCurly);
	if (!matchCloseCurly) return ERROR("Expected '}'");

	return new Block(statements);
}

PResult<IfStatement> Parser::ParseIf(TokenStream& tokens)
{
	MARK();
	bool matchStartCond = tokens.Match(TokenType::KwIf, TokenType::LParen);
	if (!matchStartCond) return NONMATCH();

	auto condition = ParseExpression(tokens);
	if (!condition) return ERROR("Expected an expression");

	bool matchEndCond = tokens.Match(TokenType::RParen);
	if (!matchEndCond) return ERROR(")");

	auto block = ParseBlock(tokens);
	if (!block) return ERROR("Expected a body");

	return new IfStatement{ condition, block };
}

PResult<WhileStatement> Parser::ParseWhile(TokenStream& tokens)
{
	MARK();
	bool matchStartCond = tokens.Match(TokenType::KwWhile, TokenType::LParen);
	if (!matchStartCond) return NONMATCH();

	auto condition = ParseExpression(tokens);
	if (!condition) return ERROR("Expected an expression");

	bool matchEndCond = tokens.Match(TokenType::RParen);
	if (!matchEndCond) return ERROR(")");

	auto block = ParseBlock(tokens);
	if (!block) return ERROR("Expected a body");

	return new WhileStatement{ condition, block };
}

PResult<ForStatement> Parser::ParseFor(TokenStream& tokens)
{
	MARK();
	bool matchStartCond = tokens.Match(TokenType::KwFor, TokenType::LParen);
	if (!matchStartCond) return NONMATCH();

	auto init = ParseStatement(tokens);
	if (!init) return NULL;

	bool firstSemicolon = tokens.Match(TokenType::OpSemicolon);
	if (!firstSemicolon) return ERROR("Expected ';'");

	auto cond = ParseExpression(tokens);
	if (!cond) return ERROR("Expected an expression");

	bool secondSemicolon = tokens.Match(TokenType::OpSemicolon);
	if (!secondSemicolon) return ERROR("Expected ';'");

	auto iter = ParseStatement(tokens);
	if (!iter) return ERROR("Expected an iterator statement");

	bool matchEndCond = tokens.Match(TokenType::RParen);
	if (!matchEndCond) return ERROR("Expected ')'");

	auto block = ParseBlock(tokens);
	if (!block) return ERROR("Expected a body");

	return new ForStatement{ init, cond, iter, block };
}

PResult<ReturnStatement> Parser::ParseReturn(TokenStream& tokens)
{
	MARK();
	if (!tokens.Match(TokenType::KwReturn)) return NONMATCH();

	auto expr = ParseExpression(tokens);
	if (!expr) return ERROR("Expected an expression");

	return new ReturnStatement{ expr };
}

PResult<FunctionCall> Parser::ParseFunctionCall(TokenStream& tokens)
{
	MARK();

	std::string funcName = "";
	bool funcLabel = tokens.MatchLabel(funcName);
	if (!funcLabel) return NONMATCH();

	std::vector<Expression*> inputArguments;
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
			if (!inputArg) return ERROR("Expected an expression");
			inputArguments.push_back(inputArg);

			while (inputArg && argComma)
			{
				argComma = tokens.Match(TokenType::OpComma);
				inputArg = ParseExpression(tokens);

				if (argComma && !inputArg) return ERROR("Expected an expression");
				if (inputArg)
					inputArguments.push_back(inputArg);
			}
		}
	}

	if (!tokens.Match(TokenType::RParen))
		return ERROR("Expected a ')'");

	return new FunctionCall{ funcName, inputArguments };
}

PResult<FunctionParameter> Parser::ParseFunctionParameter(TokenStream& tokens)
{
	MARK();
	auto paramName = ParseName(tokens);
	if (!paramName) return NONMATCH();

	if (!tokens.Match(TokenType::OpColon)) return ERROR("Expected a ':'");

	auto typeName = ParseName(tokens);
	if (!typeName) return ERROR("Expected an identifier");

	return new FunctionParameter{ paramName, typeName };
}

PResult<TopLevelFunctionDefinition> Parser::ParseTopLevelFunctionDefinition(TokenStream& tokens)
{
	MARK();
	if (!tokens.Match(TokenType::KwFunc))
		return NONMATCH();

	auto funcName = ParseName(tokens);
	if (!funcName) return ERROR("Expected an identifier");

	if (!tokens.Match(TokenType::LParen))
		return ERROR("Expected a '('");

	std::vector<FunctionParameter*> inputParameters;

	auto inputParam = ParseFunctionParameter(tokens);
	if (inputParam) // Non empty function call arguments
	{
		inputParameters.push_back(inputParam);
		bool paramComma = tokens.Match(TokenType::OpComma);
		if (paramComma) // More than one argument
		{
			inputParam = ParseFunctionParameter(tokens);
			if (!inputParam) return ERROR("Expected a parameter");
			inputParameters.push_back(inputParam);

			while (inputParam && paramComma)
			{
				paramComma = tokens.Match(TokenType::OpComma);
				inputParam = ParseFunctionParameter(tokens);

				if (paramComma && !inputParam) return ERROR("Expected a parameter");
				if (inputParam)
					inputParameters.push_back(inputParam);
			}
		}
	}

	bool retArrow = tokens.Match(TokenType::RParen, TokenType::OpMinus, TokenType::RTriangle);
	if (!retArrow)
		return ERROR("Expected a return type");

	auto retTypeName = ParseName(tokens);
	if (!retTypeName) return ERROR("Expected an identifier");

	auto body = ParseBlock(tokens);
	if (!body) return ERROR("Expected a function body");

	return new TopLevelFunctionDefinition{ retTypeName, funcName, inputParameters, body };
}

PResult<MemberFunctionDefinition> Parser::ParseMemberFunctionDefinition(TokenStream& tokens)
{
	MARK();
	if (!tokens.Match(TokenType::KwFunc))
		return NONMATCH();

	auto funcName = ParseName(tokens);
	if (!funcName) return ERROR("Expected an identifier");

	if (!tokens.Match(TokenType::LParen))
		return ERROR("Expected a '('");

	std::vector<FunctionParameter*> inputParameters;

	auto inputParam = ParseFunctionParameter(tokens);
	if (inputParam) // Non empty function call arguments
	{
		inputParameters.push_back(inputParam);
		bool paramComma = tokens.Match(TokenType::OpComma);
		if (paramComma) // More than one argument
		{
			inputParam = ParseFunctionParameter(tokens);
			if (!inputParam) return ERROR("Expected a parameter");
			inputParameters.push_back(inputParam);

			while (inputParam && paramComma)
			{
				paramComma = tokens.Match(TokenType::OpComma);
				inputParam = ParseFunctionParameter(tokens);

				if (paramComma && !inputParam) return ERROR("Expected a parameter");
				if (inputParam)
					inputParameters.push_back(inputParam);
			}
		}
	}

	bool retArrow = tokens.Match(TokenType::RParen, TokenType::OpMinus, TokenType::RTriangle);
	if (!retArrow)
		return ERROR("Expected a return type");

	auto retTypeName = ParseName(tokens);
	if (!retTypeName) return ERROR("Expected an identifier");

	auto body = ParseBlock(tokens);
	if (!body) return ERROR("Expected a function body");

	return new MemberFunctionDefinition{ retTypeName, funcName, inputParameters, body };
}

PResult<LogicalOr> Parser::ParseLogicalOr(TokenStream& tokens)
{
	MARK();
	auto land = ParseLogicalAnd(tokens);
	if (!land) return NONMATCH();

	LogicalOr* tail = NULL;
	bool op = tokens.Match(TokenType::OpAmpersand, TokenType::OpAmpersand);
	if (op)
	{
		tail = ParseLogicalOr(tokens);
		if (!tail) return ERROR("Expected an expression");
	}
	return new LogicalOr{ land, tail };
}
PResult<LogicalAnd> Parser::ParseLogicalAnd(TokenStream& tokens)
{
	MARK();
	auto eq = ParseEq(tokens);
	if (!eq) return NONMATCH();

	LogicalAnd* tail = NULL;
	bool op = tokens.Match(TokenType::OpAmpersand, TokenType::OpAmpersand);
	if (op)
	{
		tail = ParseLogicalAnd(tokens);
		if (!tail) return ERROR("Expected an expression");
	}
	return new LogicalAnd{ eq, tail };
}
PResult<Eq> Parser::ParseEq(TokenStream& tokens)
{
	MARK();
	auto cond = ParseCond(tokens);
	if (!cond) return NONMATCH();

	Eq* tail = NULL;
	bool neq = false;
	bool eq = tokens.Match(TokenType::OpEqual, TokenType::OpEqual);
	if (!eq) neq = tokens.Match(TokenType::OpMinus);
	bool op = eq || neq;

	if (op)
	{
		tail = ParseEq(tokens);
		if (!tail) return ERROR("Expected an expression");
	}
	return new Eq{ cond, tail };
}
PResult<Cond> Parser::ParseCond(TokenStream& tokens)
{
	MARK();
	auto sum = ParseSum(tokens);
	if (!sum) return NONMATCH();

	Cond* tail = NULL;
	bool l = false;
	bool g = false;
	bool lt = false;
	bool gt = tokens.Match(TokenType::RTriangle, TokenType::OpEqual);
	if (!gt)
	{
		lt = tokens.Match(TokenType::LTriangle, TokenType::OpEqual);
		if (!lt)
		{
			g = tokens.Match(TokenType::RTriangle);
			if (!g)
			{
				l = tokens.Match(TokenType::LTriangle);
			}
		}
	}

	bool op = l || g || lt || gt;
	if (op)
	{
		tail = ParseCond(tokens);
		if (!tail) return ERROR("Expected an expression");
	}
	return new Cond{ sum, tail };
}
PResult<Sum> Parser::ParseSum(TokenStream& tokens)
{
	MARK();
	auto term = ParseTerm(tokens);
	if (!term) return NONMATCH();

	Sum* tail = NULL;
	bool minus = false;
	bool plus = tokens.Match(TokenType::OpPlus);
	if (!plus) minus = tokens.Match(TokenType::OpMinus);
	bool op = plus || minus;

	if (op)
	{
		tail = ParseSum(tokens);
		if (!tail) return ERROR("Expected an expression");
	}
	return new Sum{ term, tail };
}

PResult<ExprFactor> Parser::ParseExprFactor(TokenStream& tokens)
{
	MARK();
	if (!tokens.Match(TokenType::LParen)) 
		return NONMATCH();

	auto expr = ParseExpression(tokens);
	if (!expr) return ERROR("Expected an expression");

	if (!tokens.Match(TokenType::RParen))
		return ERROR("Expected ')'");


	return new ExprFactor{ expr };
}

PResult<Term> Parser::ParseTerm(TokenStream& tokens)
{
	MARK();
	auto factor = ParseFactor(tokens);
	if (!factor) return NONMATCH();

	Term* tail = NULL;
	bool div = false;
	bool mul = tokens.Match(TokenType::OpAsterisk);
	if (!mul) div = tokens.Match(TokenType::OpSlash);
	bool op = mul || div;

	if (op)
	{
		tail = ParseTerm(tokens);
		if (!tail) return ERROR("Expected an expression");
	}
	return new Term{ factor, tail };
}

PResult<Factor> Parser::ParseFactor(TokenStream& tokens)
{
	MARK();
	auto exprFactor = ParseExprFactor(tokens);
	if (exprFactor) return (Factor*)exprFactor;

	auto atomFactor = ParseAtomicFactor(tokens);
	if (atomFactor) return (Factor*)atomFactor;
	
	return NONMATCH();
}
PResult<AtomicFactor> Parser::ParseAtomicFactor(TokenStream& tokens)
{
	MARK();
	auto atom = ParseAtom(tokens);
	if (!atom) return NONMATCH();

	return new AtomicFactor{ atom };
}

PResult<Atom> Parser::ParseAtom(TokenStream& tokens)
{
	MARK();
	auto dispatch = ParseFunctionCall(tokens);
	if (dispatch) return (Atom*)dispatch;

	auto number = ParseNumber(tokens);
	if (number) return (Atom*)number;

	auto name = ParseName(tokens);
	if (name) return (Atom*)name;

	return NONMATCH();
}

PResult<Number> Parser::ParseNumber(TokenStream& tokens)
{
	MARK();
	std::string numVal = "";
	auto number = tokens.MatchNumber(numVal);
	if (!number) return NONMATCH();

	return new Number{ numVal };
}

PResult<Name> Parser::ParseName(TokenStream& tokens)
{
	MARK();
	std::string name = "";
	auto nameLabel = tokens.MatchLabel(name);
	if (!nameLabel) return NONMATCH();

	return new Name{ name };
}

PResult<VarDeclaration> Parser::ParseVarDeclaration(TokenStream& tokens)
{
	MARK();

	bool varKw = tokens.Match(TokenType::KwLet);
	if (!varKw) return NONMATCH();

	auto varName = ParseName(tokens);
	if (!varName) return ERROR("Expected an identifier");

	if (!tokens.Match(TokenType::OpColon)) return ERROR("Expected ':' after var declaration");

	auto varTypeName = ParseName(tokens);
	if (!varTypeName) return ERROR("Expected type identifier");

	Expression* rhs = NULL;
	if (tokens.Match(TokenType::OpEqual))
	{
		rhs = ParseExpression(tokens);
		if (!rhs) return ERROR("Expected an expression as assignment");
	}

	return new VarDeclaration{ varName, varTypeName, rhs };
}