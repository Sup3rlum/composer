#pragma once

#include "expression.h"
#include "statement.h"
#include "toplevel.h"
#include "function.h"

class Block : public ASTNode
{
public:
    std::vector<class Statement*> Statements;
    Block(const std::vector<class Statement*>& stmnts) : Statements(stmnts) {}
    ~Block()
    {
    }

    llvm::Value* Codegen();
};

/************************************************
*
*   EXPRESSION
*
*************************************************/

/* Statements */

/*
*  TOP LEVEL STATEMENTS
*/


class AST
{
public:
    std::vector<std::string> ErrorList;
    std::vector<TopLevelStatement*> Statements;
    AST(const std::vector<TopLevelStatement*> stmnts) : Statements(stmnts)
    {}
    llvm::Value* Codegen();
};