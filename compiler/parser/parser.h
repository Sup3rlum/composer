#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <span>
#include "../tokenizer.h"
#include "../ast/ast.h"

#define ERROR(str) tokens.Error(str)
#define MARK() tokens.Mark()
#define NONMATCH() tokens.NonMatch()

template<typename TNode>
struct PResult
{
    TNode* Ptr;

    PResult(TNode* ptr) : Ptr(ptr) {}
    PResult(int i) : Ptr((TNode*)i) {}

    operator bool()
    {
        return bool(Ptr);
    }
    operator TNode* ()
    {
        return Ptr;
    }
};


class Parser
{
public:
    Parser()
    {
    }
    AST* GenerateAST(const std::vector<Token>& tokens);
private:

    PResult<Block> ParseBlock(TokenStream& tokens);

    /* Statements */
    PResult<Statement> ParseStatement(TokenStream& tokens);
    PResult<Statement> ParseBlockStatement(TokenStream& tokens);
    PResult<Statement> ParseNormalStatement(TokenStream& tokens);
    PResult<IfStatement> ParseIf(TokenStream& tokens);
    PResult<WhileStatement> ParseWhile(TokenStream& tokens);
    PResult<ForStatement> ParseFor(TokenStream& tokens);
    PResult<ReturnStatement> ParseReturn(TokenStream& tokens);
    PResult<VarDeclaration> ParseVarDeclaration(TokenStream& tokens);
    PResult<FunctionCall> ParseFunctionCall(TokenStream& tokens);

    /* Top Level  */
    PResult<FunctionParameter> ParseFunctionParameter(TokenStream& tokns);
    PResult<TopLevelStatement> ParseTopLevelStatement(TokenStream& tokens);
    PResult<TopLevelFunctionDefinition> ParseTopLevelFunctionDefinition(TokenStream& tokens);
    PResult<MemberFunctionDefinition> ParseMemberFunctionDefinition(TokenStream& tokens);
    PResult<StructDefinition> ParseStructDefinition(TokenStream& tokens);
    PResult<CompDefinition> ParseCompDefinition(TokenStream& tokens);
    //PResult<Member> ParseStructMember(TokenStream& tokens);
    PResult<EnumDefinition> ParseEnumDefinition(TokenStream& tokens);

    /* Expression */
    PResult<Expression> ParseExpression(TokenStream& tokens);
    PResult<LogicalOr> ParseLogicalOr(TokenStream& tokens);
    PResult<LogicalAnd> ParseLogicalAnd(TokenStream& tokens);
    PResult<Eq> ParseEq(TokenStream& tokens);
    PResult<Cond> ParseCond(TokenStream& tokens);
    PResult<Sum> ParseSum(TokenStream& tokens);
    PResult<Term> ParseTerm(TokenStream& tokens);
    PResult<Factor> ParseFactor(TokenStream& tokens);
    PResult<AtomicFactor> ParseAtomicFactor(TokenStream& tokens);
    PResult<ExprFactor> ParseExprFactor(TokenStream& tokens);

    /* Building blocks */

    PResult<Atom> ParseAtom(TokenStream& tokens);
    PResult<Number> ParseNumber(TokenStream& tokens);
    PResult<Name> ParseName(TokenStream& tokens);
};

