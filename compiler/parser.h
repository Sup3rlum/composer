#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <span>
#include "tokenizer.h"
#include "matcher.h"


class ASTNode
{
  public:
    virtual ~ASTNode()
    {
    }
};

class Block : public ASTNode
{
public:
    std::vector<class Statement*> Statements;
    Block(const std::vector<class Statement*>& stmnts) : Statements(stmnts) {}
    ~Block()
    {
    }
};

class Expression : public ASTNode
{
public:
    ~Expression()
    {
    }
};

class Comparison : public ASTNode
{
public:
    enum class Type
    {
        Less,
        LessThan,
        Greater,
        GreaterThan,
        Eq,
        NotEq
    } CompType;

    Expression* Left;
    Expression* Right;

    Comparison(Expression* left, Expression* right, Type compType) :
        Left(left),
        Right(right),
        CompType(compType)
    {}

    ~Comparison()
    {

    }
};

class Term : public ASTNode
{
public:
    ~Term()
    {

    }
};

class Factor : public ASTNode
{
public:



    ~Factor()
    {

    }
};

class CompoundFactor : public Factor
{
public:
    Expression* Inner;
    CompoundFactor(Expression* expr) : Inner(expr) {}
};

class NamedFactor : public Factor
{
public:
    std::string Name;

    NamedFactor(const std::string& str) : Name(str)
    {

    }
};

class NumericFactor : public Factor
{
public:
    std::string Name;

    NumericFactor(const std::string& str) : Name(str)
    {

    }
};

class FunctionCall : public Expression
{
public:
    std::string FuncName;
    std::vector<Expression*> Arguments;

    FunctionCall(const std::string& funcName, const std::vector<Expression*>& args) : FuncName(funcName), Arguments(args) {}
    ~FunctionCall() {}
};


/* Statements */

class Statement : public ASTNode
{
public:
};

class TopLevelStatement : public ASTNode
{
public:
};

class FunctionParameter : public ASTNode
{
public:
    std::string Name;
    std::string TypeName;

    FunctionParameter(const std::string& name, const std::string& typeName) : Name(name), TypeName(typeName) {}

};

class VarDeclaration : public Statement
{
public:
    std::string Name;
    std::string TypeName;

    Expression* RHS;



};

class IfStatement : public Statement
{
public:
    Expression* Condition;
    Block* Body;
    IfStatement(Expression* cond, Block* body) : Condition(cond), Body(body) {}
    ~IfStatement()
    {
    }
};

class ForStatement : public Statement
{
public:
    Statement* Init;
    Expression* Condition;
    Statement* Iter;
    Block* Body;

    ForStatement(Statement* init, Expression* cond, Statement* iter, Block* body) : 
        Init(init), 
        Condition(cond), 
        Body(body), 
        Iter(iter) {}

    ~ForStatement()
    {
    }
};

class WhileStatement : public Statement
{
public:
    Expression* Condition;
    Block* Body;

    WhileStatement(Expression* cond,Block* body) :
        Condition(cond),
        Body(body) {}

    ~WhileStatement()
    {
    }
};

/*
*  TOP LEVEL STATEMENTS
*/

class FunctionDefinition : public TopLevelStatement
{
public:
    std::string ReturnType;
    std::string FuncName;
    std::vector<FunctionParameter*> Args;
    Block* Body;

    FunctionDefinition(const std::string& ret, const std::string& funcName, const std::vector<FunctionParameter*>& args, Block* body) :
        ReturnType(ret),
        FuncName(funcName),
        Args(args),
        Body(body)
    {}

};

class StructDefinition : public TopLevelStatement
{
public:
    std::string Name;
    Block* Body;

    StructDefinition(const std::string& name, Block* body) : Name(name), Body(body)
    {
    }
    ~StructDefinition() {}
};


class EnumDefinition : public TopLevelStatement
{
public:
    Block* Body;
    EnumDefinition(Block* body) : Body(body)
    {
    }
    ~EnumDefinition() {}
};





class AST
{
public:
    std::vector<TopLevelStatement*> Statements;
    AST(const std::vector<TopLevelStatement*> stmnts) : Statements(stmnts)
    {}
};

class Parser
{
    typedef std::span<Token> TokenSpan;
public:
    Parser()
    {
    }
    AST* GenerateAST(const std::vector<Token>& tokens);
private:

    Expression* ParseExpression(TokenStream& tokens);
    Statement* ParseStatement(TokenStream& tokens);
    TopLevelStatement* ParseTopLevelStatement(TokenStream& tokens);

    Block* ParseBlock(TokenStream& tokens);

    IfStatement* ParseIf(TokenStream& tokens);
    WhileStatement* ParseWhile(TokenStream& tokens);
    ForStatement* ParseFor(TokenStream& tokens);


    FunctionCall* ParseFunctionCall(TokenStream& tokens);
    FunctionParameter* ParseFunctionParameter(TokenStream& tokns);
    FunctionDefinition* ParseFunctionDefinition(TokenStream& tokens);
    StructDefinition* ParseStructDefinition(TokenStream& tokens);
    EnumDefinition* ParseEnumDefinition(TokenStream& tokens);

    Term* ParseTerm(TokenStream& tokens);
    Factor* ParseFactor(TokenStream& tokens);
    CompoundFactor* ParseCompoundFactor(TokenStream& tokens);
    NamedFactor* ParseNamedFactor(TokenStream& tokens);
    NumericFactor* ParseNumericFactor(TokenStream& tokens);
};

