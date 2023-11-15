#pragma once

#include "Common.h"
#include "Expression.h"
#include "Function.h"
#include "Module.h"
#include "Statement.h"

class Block : public ASTNode
{
  public:
    std::vector<class Statement *> Statements;
    Block(const std::vector<class Statement *> &stmnts) : Statements(stmnts)
    {
    }
    ~Block()
    {
    }

    llvm::Value *Codegen(CModule *module);
};

class AST
{
  public:
    std::vector<std::string> ErrorList;
    std::vector<ModuleStatement *> Statements;
    AST(const std::vector<ModuleStatement *> stmnts) : Statements(stmnts)
    {
    }
    llvm::Value *Codegen(CModule *module);
};
