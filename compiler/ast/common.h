#pragma once

#include <fstream>
#include <iostream>
#include <span>
#include <vector>

class CModule;

namespace llvm
{
class Value;
}

class ASTNode
{
  public:
    virtual llvm::Value *Codegen(CModule *module) = 0;
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
    enum class Type
    {
        Float32,
        Float64,
        Int32,
        Int64,
        UInt32,
        UInt64
    } NumType;

    std::string Value;
    Number(std::string &value, Type type) : Value(value), NumType(type)
    {
    }

    llvm::Value *Codegen(CModule *module);
};

class Name : public Atom
{
  public:
    std::string Value;
    Name(std::string &value) : Value(value)
    {
    }

    llvm::Value *Codegen(CModule *module);
    operator std::string()
    {
        return Value;
    }
};

class ModuleStatement : public ASTNode
{
  public:
    llvm::Value *Codegen();
};
