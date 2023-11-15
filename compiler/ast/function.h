#pragma once

#include "common.h"

class Name;
class Block;

class FunctionParameter : public ASTNode
{
  public:
    Name *ParamName;
    Name *TypeName;

    FunctionParameter(Name *name, Name *typeName) : ParamName(name), TypeName(typeName)
    {
    }
    llvm::Value *Codegen(CModule *module) override;
};

class FunctionProto : public ASTNode
{
  public:
    Name *RetTypeName;
    Name *FuncName;
    std::vector<FunctionParameter *> Args;

    FunctionProto(Name *ret, Name *funcName, const std::vector<FunctionParameter *> &args)
        : RetTypeName(ret), FuncName(funcName), Args(args)
    {
    }
    llvm::Value *Codegen(CModule *module) override;
};

class MemberFunctionDefinition : public Member
{
  public:
    Name *ParentName;
    FunctionProto *FuncProto;
    Block *Body;

    MemberFunctionDefinition(Name *parent, FunctionProto *funcProto, Block *body)
        : ParentName(parent), FuncProto(funcProto), Body(body)
    {
    }
    llvm::Value *Codegen(CModule *module) override;
};

class ModuleFunctionDefinition : public ModuleStatement
{
  public:
    FunctionProto *FuncProto;
    Block *Body;

    ModuleFunctionDefinition(FunctionProto *funcProto, Block *body) : FuncProto(funcProto), Body(body)
    {
    }
    llvm::Value *Codegen(CModule *module) override;
};
