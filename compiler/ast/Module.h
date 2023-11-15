#pragma once

#include "Common.h"

class MemberFunctionDefinition;
class VarDeclaration;

class EnumDefinition : public ModuleStatement
{
  public:
    Name *EnumName;
    std::vector<Name *> Values;
    EnumDefinition(Name *enumName, const std::vector<Name *> values) : EnumName(enumName), Values(values)
    {
    }
    ~EnumDefinition()
    {
    }
    llvm::Value *Codegen(CModule *module);
};

class StructDefinition : public ModuleStatement
{
  public:
    Name *StructName;
    std::vector<VarDeclaration *> Fields;
    std::vector<MemberFunctionDefinition *> Members;

    StructDefinition(Name *name, const std::vector<VarDeclaration *> &fields,
                     const std::vector<MemberFunctionDefinition *> &members)
        : StructName(name), Fields(fields), Members(members)
    {
    }
    ~StructDefinition()
    {
    }
    llvm::Value *Codegen(CModule *module);
};

class CompDefinition : public ModuleStatement
{
  public:
    Name *CompName;
    std::vector<Name *> Components;
    std::vector<VarDeclaration *> Fields;
    std::vector<MemberFunctionDefinition *> Members;

    CompDefinition(Name *compName, const std::vector<Name *> &components, const std::vector<VarDeclaration *> &fields,
                   const std::vector<MemberFunctionDefinition *> &members)
        : CompName(compName), Components(components), Fields(fields), Members(members)
    {
    }

    llvm::Value *Codegen(CModule *module);
};
