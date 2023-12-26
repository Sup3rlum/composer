#pragma once

/*

#include "../ast/ast.h"
#include "llvm.h"
#include <map>



class CModule
{
public:
    std::unique_ptr<llvm::LLVMContext> Context;
    std::unique_ptr<llvm::Module> LlvmModule;
    std::unique_ptr<llvm::IRBuilder<>> Builder;
    //std::map<std::string, llvm::AllocaInst*> NamedValues;

    std::map<std::string, llvm::StructType*> StructTypes;

    CModule(CModule&& other) noexcept :
        Context(std::move(other.Context)),
        LlvmModule(std::move(other.LlvmModule)),
        Builder(std::move(other.Builder))//,
        //NamedValues(std::move(other.NamedValues))
    {}

    llvm::Function* GetFunction(const std::string& Name);
    llvm::Function* SearchFunctionByName(const std::string& name);

    llvm::Type* GetTypeByName(const std::string& name);
    llvm::Type* GetPtrTypeByName(const std::string& name, int addressSpace = 0);

    std::string MangleName(MemberFunctionDefinition* member);
    std::string MangleName(TopLevelFunctionDefinition* member);

};*/
