#pragma once

#include "Common.h"

/*
 *   Regular statements;
 */

class Name;
class Expression;
class Block;
class CModule;

class Statement : public ASTNode
{
  public:
    llvm::Value *Codegen();
};

class VarDeclaration : public Statement
{
  public:
    Name *VarName;
    Name *TypeName;
    Expression *RHS;
    llvm::Value *Codegen();
    VarDeclaration(Name *name, Name *typeName, Expression *init) : VarName(name), TypeName(typeName), RHS(init)
    {
    }

    llvm::Value *Codegen(CModule *module);
};

class IfStatement : public Statement
{
  public:
    Expression *Condition;
    Block *Body;
    IfStatement(Expression *cond, Block *body) : Condition(cond), Body(body)
    {
    }
    ~IfStatement()
    {
    }
    llvm::Value *Codegen(CModule *module);
};

class ForStatement : public Statement
{
  public:
    Statement *Init;
    Expression *Condition;
    Statement *Iter;
    Block *Body;

    ForStatement(Statement *init, Expression *cond, Statement *iter, Block *body)
        : Init(init), Condition(cond), Body(body), Iter(iter)
    {
    }

    ~ForStatement()
    {
    }

    llvm::Value *Codegen(CModule *module);
};

class WhileStatement : public Statement
{
  public:
    Expression *Condition;
    Block *Body;

    WhileStatement(Expression *cond, Block *body) : Condition(cond), Body(body)
    {
    }

    ~WhileStatement()
    {
    }
    llvm::Value *Codegen(CModule *module);
};

class Assignment : public Statement
{
  public:
    Expression *LHS;
    Expression *RHS;

    Assignment(Expression *lhs, Expression *rhs) : LHS(lhs), RHS(rhs)
    {
    }
};

class ReturnStatement : public Statement
{
  public:
    Expression *Expr;
    ReturnStatement(Expression *expr) : Expr(expr)
    {
    }
    llvm::Value *Codegen(CModule *module);
};
