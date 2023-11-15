#pragma once

#include "common.h"

class Expression;



class FunctionCall : public Atom
{
public:
    Name* FuncName;
    std::vector<Expression*> Arguments;

    FunctionCall(Name* funcName, const std::vector<Expression*>& args) : FuncName(funcName), Arguments(args) {}
    ~FunctionCall() {}

    llvm::Value* Codegen(CModule* module) override;
};

class Factor : public ASTNode {
public:
};

class AtomicFactor : public Factor
{
public:
    Atom* atom;
    AtomicFactor(Atom* atom) : atom(atom) {}
    llvm::Value* Codegen(CModule* module) override;
};

class ExprFactor : public Factor
{
public:
    Expression* Inner;
    ExprFactor(Expression* inner) : Inner(inner) {}
    llvm::Value* Codegen(CModule* module) override;

};


class Term : public ASTNode {
public:
    Factor* LHS;
    Term* RHS;
    Term(Factor* lhs, Term* rhs) : LHS(lhs), RHS(rhs) {}
    llvm::Value* Codegen(CModule* module) override;

};
class Sum : public ASTNode {
public:
    Term* LHS;
    Sum* RHS;
    Sum(Term* lhs, Sum* rhs) : LHS(lhs), RHS(rhs) {}
    llvm::Value* Codegen(CModule* module) override;

};
class Cond : public ASTNode {
public:
    Sum* LHS;
    Cond* RHS;
    Cond(Sum* lhs, Cond* rhs) : LHS(lhs), RHS(rhs) {}
    llvm::Value* Codegen(CModule* module) override;

};
class Eq : public ASTNode {
public:
    Cond* LHS;
    Eq* RHS;
    Eq(Cond* lhs, Eq* rhs) : LHS(lhs), RHS(rhs) {}
    llvm::Value* Codegen(CModule* module) override;

};
class LogicalAnd : public ASTNode {
public:
    Eq* LHS;
    LogicalAnd* RHS;
    LogicalAnd(Eq* lhs, LogicalAnd* rhs) : LHS(lhs), RHS(rhs) {}
    llvm::Value* Codegen(CModule* module) override;

};
class LogicalOr : public ASTNode {
public:
    LogicalAnd* LHS;
    LogicalOr* RHS;
    LogicalOr(LogicalAnd* lhs, LogicalOr* rhs) : LHS(lhs), RHS(rhs) {}
    llvm::Value* Codegen(CModule* module) override;

};

class Expression : public ASTNode
{
public:
    LogicalOr* Body;
    Expression(LogicalOr* body) : Body(body) {}
    llvm::Value* Codegen(CModule* module) override;

};

