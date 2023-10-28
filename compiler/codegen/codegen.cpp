#include "codegen.h"


using namespace llvm;

/*
static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<IRBuilder<>> Builder;
static std::map<std::string, Value*> NamedValues;*/

Value* IfStatement::Codegen()
{
    return NULL;
}

Value* MemberFunctionDefinition::Codegen()
{
    /*// First, check for an existing function from a previous 'extern' declaration.
    Function* TheFunction = TheModule->getFunction(FuncName);

    // Create Func Signature
    std::vector<Type*> Doubles(Args.size(), Type::getDoubleTy(*TheContext));
    FunctionType* FuncSignature = FunctionType::get(Type::getDoubleTy(*TheContext), Doubles, false);

    // Function
    Function* TheFunction = Function::Create(FuncSignature, Function::ExternalLinkage, FuncName, TheModule.get());

    // Set names for all arguments.
    unsigned Idx = 0;
    for (auto& Arg : TheFunction->args())
        Arg.setName(Args[Idx++]->Name);

    if (!TheFunction)
        return nullptr;

    // Create a new basic block to start insertion into.
    BasicBlock* BB = BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    NamedValues.clear();
    for (auto& Arg : TheFunction->args())
        NamedValues[std::string(Arg.getName())] = &Arg;

    if (Value* RetVal = Body->codegen()) 
    {
        // Finish off the function.
        Builder->CreateRet(RetVal);

        // Validate the generated code, checking for consistency.
        verifyFunction(*TheFunction);

        return TheFunction;
    }

    // Error reading body, remove function.
    TheFunction->eraseFromParent();*/
    return nullptr;
}

Value* StructDefinition::Codegen()
{
    return NULL;
}

Value* ForStatement::Codegen()
{
    return NULL;
}

Value* WhileStatement::Codegen()
{
    return NULL;
}

Value* Block::Codegen()
{
    return NULL;
}

Value* FunctionCall::Codegen()
{
    return NULL;
}

Value* AtomicFactor::Codegen()
{
    return NULL;
}

Value* ExprFactor::Codegen()
{
    return NULL;
}

Value* Term::Codegen()
{
    return NULL;
}

Value* Cond::Codegen()
{
    return NULL;
}

Value* Eq::Codegen()
{
    return NULL;
}
Value* LogicalAnd::Codegen()
{
    return NULL;
}
Value* LogicalOr::Codegen()
{
    return NULL;
}
Value* Expression::Codegen()
{
    return NULL;
}
Value* VarDeclaration::Codegen()
{
    return NULL;
}

Value* ReturnStatement::Codegen()
{
    return NULL;
}

Value* AST::Codegen()
{
    return NULL;
}