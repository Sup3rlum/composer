/*#include "module.h"
#include "types.h"
#include <sstream>

using namespace llvm;

llvm::Type *CGetBaseNumericTypes(Number::Type NumericType, llvm::LLVMContext *context)
{
    switch (NumericType)
    {
    case Number::Type::Int32:
        return llvm::Type::getInt32Ty(*context);
    case Number::Type::Int64:
        return llvm::Type::getInt64Ty(*context);
    default:
        break;
    }

    return NULL;
}

Type *CGBaseTypeToLLVM(CBaseType baseType, LLVMContext &context)
{
    switch (baseType)
    {
    case CBaseType::Unknown:
        return NULL;
    case CBaseType::Int8:
        return Type::getInt8Ty(context);

    case CBaseType::UInt32:
    case CBaseType::Int32:
        return Type::getInt32Ty(context);

    case CBaseType::UInt64:
    case CBaseType::Int64:
        return Type::getInt64Ty(context);

    case CBaseType::Float32:
        return Type::getFloatTy(context);
    case CBaseType::Float64:
        return Type::getDoubleTy(context);
    }
    return NULL;
}

Type *CModule::GetTypeByName(const std::string &name)
{
    auto baseType = CGStrToBaseType(name);
    auto llvmType = CGBaseTypeToLLVM(baseType, *Context.get());
    if (llvmType)
        return llvmType;

    if (StructTypes.contains(name))
        return StructTypes[name];

    return NULL;
}

Type *CModule::GetPtrTypeByName(const std::string &name, int addressSpace)
{
    auto type = GetTypeByName(name);
    if (!type)
        return NULL;

    return PointerType::get(type, addressSpace);
}

std::string CModule::MangleName(MemberFunctionDefinition *member)
{
    auto part = member->ReturnType->Value + "#" + member->ParentName->Value + "::" + member->FuncName->Value;

    for (auto arg : member->Args)
    {
        part += "@" + arg->ParamName->Value + "&" + arg->TypeName->Value;
    }
    return part;
}

std::string CModule::MangleName(TopLevelFunctionDefinition *member)
{
    auto part = member->ReturnType->Value + "#" + member->FuncName->Value;

    for (auto arg : member->Args)
    {
        part += "@" + arg->ParamName->Value + "&" + arg->TypeName->Value;
    }
    return part;
}

llvm::Function *CModule::GetFunction(const std::string &Name)
{
    if (auto *F = LlvmModule->getFunction(Name))
        return F;

    return nullptr;
}

llvm::Function *CModule::SearchFunctionByName(const std::string &name)
{
    auto &funcList = LlvmModule->getFunctionList();

    for (auto &func : funcList)
    {
        auto vals = SplitString(func.getName().data());
        auto funcName = vals[1];
    }
}*/
