#include "Parser.h"

PResult<Expression> Parser::ParseExpression(TokenStream &tokens)
{
    MARK();
    auto lor = ParseLogicalOr(tokens);
    if (!lor)
        return NONMATCH();

    return new Expression{lor};
}

PResult<LogicalOr> Parser::ParseLogicalOr(TokenStream &tokens)
{
    MARK();
    auto land = ParseLogicalAnd(tokens);
    if (!land)
        return NONMATCH();

    LogicalOr *tail = NULL;
    bool op = tokens.Match(TokenType::OpAmpersand, TokenType::OpAmpersand);
    if (op)
    {
        tail = ParseLogicalOr(tokens);
        if (!tail)
            return ERROR("Expected an expression");
    }
    return new LogicalOr{land, tail};
}
PResult<LogicalAnd> Parser::ParseLogicalAnd(TokenStream &tokens)
{
    MARK();
    auto eq = ParseEq(tokens);
    if (!eq)
        return NONMATCH();

    LogicalAnd *tail = NULL;
    bool op = tokens.Match(TokenType::OpAmpersand, TokenType::OpAmpersand);
    if (op)
    {
        tail = ParseLogicalAnd(tokens);
        if (!tail)
            return ERROR("Expected an expression");
    }
    return new LogicalAnd{eq, tail};
}
PResult<Eq> Parser::ParseEq(TokenStream &tokens)
{
    MARK();
    auto cond = ParseCond(tokens);
    if (!cond)
        return NONMATCH();

    Eq *tail = NULL;
    bool neq = false;
    bool eq = tokens.Match(TokenType::OpEqual, TokenType::OpEqual);
    if (!eq)
        neq = tokens.Match(TokenType::OpMinus);
    bool op = eq || neq;

    if (op)
    {
        tail = ParseEq(tokens);
        if (!tail)
            return ERROR("Expected an expression");
    }
    return new Eq{cond, tail};
}
PResult<Cond> Parser::ParseCond(TokenStream &tokens)
{
    MARK();
    auto sum = ParseSum(tokens);
    if (!sum)
        return NONMATCH();

    Cond *tail = NULL;
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
        if (!tail)
            return ERROR("Expected an expression");
    }
    return new Cond{sum, tail};
}
PResult<Sum> Parser::ParseSum(TokenStream &tokens)
{
    MARK();
    auto term = ParseTerm(tokens);
    if (!term)
        return NONMATCH();

    Sum *tail = NULL;
    bool minus = false;
    bool plus = tokens.Match(TokenType::OpPlus);
    if (!plus)
        minus = tokens.Match(TokenType::OpMinus);
    bool op = plus || minus;

    if (op)
    {
        tail = ParseSum(tokens);
        if (!tail)
            return ERROR("Expected an expression");
    }
    return new Sum{term, tail};
}

PResult<ExprFactor> Parser::ParseExprFactor(TokenStream &tokens)
{
    MARK();
    if (!tokens.Match(TokenType::LParen))
        return NONMATCH();

    auto expr = ParseExpression(tokens);
    if (!expr)
        return ERROR("Expected an expression");

    if (!tokens.Match(TokenType::RParen))
        return ERROR("Expected ')'");

    return new ExprFactor{expr};
}

PResult<Term> Parser::ParseTerm(TokenStream &tokens)
{
    MARK();
    auto factor = ParseFactor(tokens);
    if (!factor)
        return NONMATCH();

    Term *tail = NULL;
    bool div = false;
    bool mul = tokens.Match(TokenType::OpAsterisk);
    if (!mul)
        div = tokens.Match(TokenType::OpSlash);
    bool op = mul || div;

    if (op)
    {
        tail = ParseTerm(tokens);
        if (!tail)
            return ERROR("Expected an expression");
    }
    return new Term{factor, tail};
}

PResult<Factor> Parser::ParseFactor(TokenStream &tokens)
{
    MARK();
    auto exprFactor = ParseExprFactor(tokens);
    if (exprFactor)
        return (Factor *)exprFactor;

    auto atomFactor = ParseAtomicFactor(tokens);
    if (atomFactor)
        return (Factor *)atomFactor;

    return NONMATCH();
}
PResult<AtomicFactor> Parser::ParseAtomicFactor(TokenStream &tokens)
{
    MARK();
    auto atom = ParseAtom(tokens);
    if (!atom)
        return NONMATCH();

    return new AtomicFactor{atom};
}

PResult<Atom> Parser::ParseAtom(TokenStream &tokens)
{
    MARK();
    auto dispatch = ParseFunctionCall(tokens);
    if (dispatch)
        return (Atom *)dispatch;

    auto number = ParseNumber(tokens);
    if (number)
        return (Atom *)number;

    auto name = ParseName(tokens);
    if (name)
        return (Atom *)name;

    return NONMATCH();
}

PResult<Number> Parser::ParseNumber(TokenStream &tokens)
{
    MARK();
    std::string numVal = "";
    bool decimal = false;
    auto number = tokens.MatchNumber(numVal, decimal);
    if (!number)
        return NONMATCH();

    auto t = Number::Type::Int32;
    std::string suffix = "";
    bool hasSuffix = tokens.MatchLabel(suffix);

    if (!hasSuffix && decimal)
        t = Number::Type::Float64;

    if (hasSuffix)
    {
        if (suffix == "d" || suffix == "D")
            t = Number::Type::Float64;
        else if (suffix == "f" || suffix == "F")
            t = Number::Type::Float32;
        else if (suffix == "l" || suffix == "L")
            t = Number::Type::Int64;
        else if (suffix == "u" || suffix == "U")
            t = Number::Type::UInt32;
        else if (suffix == "ul" || suffix == "UL")
            t = Number::Type::UInt64;
    }

    return new Number{numVal, t};
}

PResult<Name> Parser::ParseName(TokenStream &tokens)
{
    MARK();
    std::string name = "";
    auto nameLabel = tokens.MatchLabel(name);
    if (!nameLabel)
        return NONMATCH();

    return new Name{name};
}

PResult<VarDeclaration> Parser::ParseVarDeclaration(TokenStream &tokens)
{
    MARK();

    bool varKw = tokens.Match(TokenType::KwLet);
    if (!varKw)
        return NONMATCH();

    auto varName = ParseName(tokens);
    if (!varName)
        return ERROR("Expected an identifier");

    if (!tokens.Match(TokenType::OpColon))
        return ERROR("Expected ':' after var declaration");

    auto varTypeName = ParseName(tokens);
    if (!varTypeName)
        return ERROR("Expected type identifier");

    Expression *rhs = NULL;
    if (tokens.Match(TokenType::OpEqual))
    {
        rhs = ParseExpression(tokens);
        if (!rhs)
            return ERROR("Expected an expression as assignment");
    }

    return new VarDeclaration{varName, varTypeName, rhs};
}
