#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <span>

namespace llvm
{
    class Value;
}

class ASTNode
{
public:
    virtual ~ASTNode()
    {
    }
};

class Member : public ASTNode
{
};

class Atom : public ASTNode 
{
};

class Number : public Atom 
{
public:
    std::string Value;
    Number(std::string& value) : Value(value) {}
};

class Name : public Atom 
{
public:
    std::string Value;
    Name(std::string& value) : Value(value) {}
};

class TopLevelStatement : public ASTNode
{
public:
    llvm::Value* Codegen();
};
