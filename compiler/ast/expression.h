#pragma once

#include "common.h"

class Expression;



class FunctionCall : public Atom
{
public:
    std::string FuncName;
    std::vector<Expression*> Arguments;

    FunctionCall(const std::string& funcName, const std::vector<Expression*>& args) : FuncName(funcName), Arguments(args) {}
    ~FunctionCall() {}

    llvm::Value* Codegen();
};

class Factor : public ASTNode {
public:
};

class AtomicFactor : public Factor
{
public:
    Atom* atom;
    AtomicFactor(Atom* atom) : atom(atom) {}
    llvm::Value* Codegen();
};

class ExprFactor : public Factor
{
public:
    Expression* Inner;
    ExprFactor(Expression* inner) : Inner(inner) {}
    llvm::Value* Codegen();
};


class Term {
public:
    Factor* LHS;
    Term* RHS;
    Term(Factor* lhs, Term* rhs) : LHS(lhs), RHS(rhs) {}
    llvm::Value* Codegen();
};
class Sum {
public:
    Term* LHS;
    Sum* RHS;
    Sum(Term* lhs, Sum* rhs) : LHS(lhs), RHS(rhs) {}
    llvm::Value* Codegen();
};
class Cond {
public:
    Sum* LHS;
    Cond* RHS;
    Cond(Sum* lhs, Cond* rhs) : LHS(lhs), RHS(rhs) {}
    llvm::Value* Codegen();
};
class Eq {
public:
    Cond* LHS;
    Eq* RHS;
    Eq(Cond* lhs, Eq* rhs) : LHS(lhs), RHS(rhs) {}
    llvm::Value* Codegen();
};
class LogicalAnd {
public:
    Eq* LHS;
    LogicalAnd* RHS;
    LogicalAnd(Eq* lhs, LogicalAnd* rhs) : LHS(lhs), RHS(rhs) {}
    llvm::Value* Codegen();
};
class LogicalOr {
public:
    LogicalAnd* LHS;
    LogicalOr* RHS;
    LogicalOr(LogicalAnd* lhs, LogicalOr* rhs) : LHS(lhs), RHS(rhs) {}
    llvm::Value* Codegen();
};

class Expression : public ASTNode
{
public:
    LogicalOr* Body;
    Expression(LogicalOr* body) : Body(body) {}
    llvm::Value* Codegen();
};

