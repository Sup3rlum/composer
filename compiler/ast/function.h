#pragma once

#include "common.h"

class Name;
class Block;


class FunctionParameter : public ASTNode
{
public:
    Name* ParamName;
    Name* TypeName;

    FunctionParameter(Name* name, Name* typeName) : ParamName(name), TypeName(typeName) {}
    llvm::Value* Codegen();
};


class MemberFunctionDefinition : public Member
{
public:
    Name* ReturnType;
    Name* FuncName;
    std::vector<FunctionParameter*> Args;
    Block* Body;

    MemberFunctionDefinition(Name* ret, Name* funcName, const std::vector<FunctionParameter*>& args, Block* body) :
        ReturnType(ret),
        FuncName(funcName),
        Args(args),
        Body(body)
    {}
    llvm::Value* Codegen();
};



class TopLevelFunctionDefinition : public TopLevelStatement
{
public:
    Name* ReturnType;
    Name* FuncName;
    std::vector<FunctionParameter*> Args;
    Block* Body;

    TopLevelFunctionDefinition(Name* ret, Name* funcName, const std::vector<FunctionParameter*>& args, Block* body) :
        ReturnType(ret),
        FuncName(funcName),
        Args(args),
        Body(body)
    {}
    llvm::Value* Codegen();
};
