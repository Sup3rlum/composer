/*#include "codegen.h"
using namespace llvm;

Value *FunctionCall::Codegen(CModule *module)
{
    Function *CalleeF = module->GetFunction(*FuncName);
    if (!CalleeF)
        return CGError("Unknown function referenced");

    // If argument mismatch error.
    if (CalleeF->arg_size() != Arguments.size())
        return CGError("Incorrect # arguments passed");

    std::vector<Value *> ArgsV;
    for (unsigned i = 0; i != Arguments.size(); ++i)
    {
        ArgsV.push_back(Arguments[i]->Codegen(module));
        if (!ArgsV.back())
            return nullptr;
    }

    return module->Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

Value *MemberFunctionDefinition::Codegen(CModule *module)
{
    // First, check if function already exists
    auto mangledName = module->MangleName(this);
    Function *TheFunction = module->GetFunction(mangledName);

    // Generate Function Signature
    std::vector<Type *> ArgTypes;

    ArgTypes.push_back(module->GetPtrTypeByName("this")); // Add this as arg
    for (auto &arg : Args)
    {
        ArgTypes.push_back(module->GetTypeByName(*arg->TypeName));
    }
    Type *ReturnType = module->GetTypeByName(*this->ReturnType);
    FunctionType *FuncSignature = FunctionType::get(ReturnType, ArgTypes, false);

    // Function
    Function *TheFunction =
        Function::Create(FuncSignature, Function::ExternalLinkage, mangledName, module->LlvmModule.get());

    // Set names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg : TheFunction->args())
        Arg.setName(Args[Idx++]->ParamName->Value);

    if (!TheFunction)
        return nullptr;

    // Create a new basic block to start insertion into.
    BasicBlock *BB = BasicBlock::Create(*module->Context, "entry", TheFunction);
    module->Builder->SetInsertPoint(BB);

    if (Value *RetVal = Body->Codegen(module))
    {
        // Finish off the function.
        module->Builder->CreateRet(RetVal);

        // Validate the generated code, checking for consistency.
        verifyFunction(*TheFunction);

        return TheFunction;
    }

    // Error reading body, remove function.
    TheFunction->eraseFromParent();
    return nullptr;
}

Value *TopLevelFunctionDefinition::Codegen(CModule *module)
{
    // First, check if function already exists
    auto mangledName = module->MangleName(this);
    Function *TheFunction = module->GetFunction(mangledName);

    // Generate Function Signature
    std::vector<Type *> ArgTypes;
    for (auto &arg : Args)
    {
        ArgTypes.push_back(module->GetTypeByName(*arg->TypeName));
    }
    Type *ReturnType = module->GetTypeByName(*this->ReturnType);
    FunctionType *FuncSignature = FunctionType::get(ReturnType, ArgTypes, false);

    // Function
    Function *TheFunction =
        Function::Create(FuncSignature, Function::ExternalLinkage, mangledName, module->LlvmModule.get());

    // Set names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg : TheFunction->args())
        Arg.setName(Args[Idx++]->ParamName->Value);

    if (!TheFunction)
        return nullptr;

    // Create a new basic block to start insertion into.
    BasicBlock *BB = BasicBlock::Create(*module->Context, "entry", TheFunction);
    module->Builder->SetInsertPoint(BB);

    if (Value *RetVal = Body->Codegen(module))
    {
        // Finish off the function.
        module->Builder->CreateRet(RetVal);

        // Validate the generated code, checking for consistency.
        verifyFunction(*TheFunction);

        return TheFunction;
    }

    // Error reading body, remove function.
    TheFunction->eraseFromParent();
    return nullptr;
}*/
