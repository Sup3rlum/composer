#pragma once

#include "../AST/AST.h"
#include "../Lexer/Lexer.h"
#include <fstream>
#include <iostream>
#include <span>
#include <vector>

#define MARK() tokens.Mark()
#define NONMATCH() GenerateNONMATCH(tokens)
#define ERROR(str) GenerateERROR(tokens, str)

typedef std::stack<Name *> ScopeStack;

struct ParseError
{
    SourceLocation sourceLocation;
    std::string message;
};

template <typename TNode> 
struct PResult
{
    TNode *Ptr;
    std::vector<ParseError> parseErrors;

    PResult(TNode *ptr) : Ptr(ptr)
    {
    }
    PResult(int i) : Ptr((TNode *)i)
    {
    }
    PResult(const PResult<int>& p) : Ptr((TNode*)p.Ptr)
    {
    }

    PResult(const ParseError &parseError) : Ptr(0)
    {
        parseErrors.push_back(parseError);
    }
    operator bool()
    {
        return bool(Ptr);
    }
    operator TNode *()
    {
        return Ptr;
    }
};

class Parser
{
  private:
    PResult<int> GenerateERROR(TokenStream &tokens, const std::string &str)
    {
        return PResult<int>(ParseError{tokens.CurrentToken().location, str});
    }
    PResult<int> GenerateNONMATCH(TokenStream &tokens)
    {
        tokens.Backtrack();
        return PResult<int>(NULL);
    }

  public:
    Parser()
    {
    }

    PResult<AST> GenerateAST(TokenStream &tokens);
    PResult<Block> ParseBlock(TokenStream &tokens);

    /* Statements */
    PResult<Statement> ParseStatement(TokenStream &tokens);
    PResult<Statement> ParseBlockStatement(TokenStream &tokens);
    PResult<Statement> ParseNormalStatement(TokenStream &tokens);
    PResult<IfStatement> ParseIf(TokenStream &tokens);
    PResult<WhileStatement> ParseWhile(TokenStream &tokens);
    PResult<ForStatement> ParseFor(TokenStream &tokens);
    PResult<ReturnStatement> ParseReturn(TokenStream &tokens);
    PResult<VarDeclaration> ParseVarDeclaration(TokenStream &tokens);
    PResult<FunctionCall> ParseFunctionCall(TokenStream &tokens);

    /* Top Level  & Function*/
    PResult<FunctionProto> ParseFunctionProto(TokenStream &tokens);
    PResult<FunctionParameter> ParseFunctionParameter(TokenStream &tokens);
    PResult<ModuleStatement> ParseModuleStatement(TokenStream &tokens);
    PResult<ModuleFunctionDefinition> ParseModuleFunctionDefinition(TokenStream &tokens);
    PResult<MemberFunctionDefinition> ParseMemberFunctionDefinition(TokenStream &tokens);
    PResult<StructDefinition> ParseStructDefinition(TokenStream &tokens);
    PResult<CompDefinition> ParseCompDefinition(TokenStream &tokens);
    // PResult<Member> ParseStructMember(TokenStream& tokens);
    PResult<EnumDefinition> ParseEnumDefinition(TokenStream &tokens);

    /* Expression */
    PResult<Expression> ParseExpression(TokenStream &tokens);
    PResult<LogicalOr> ParseLogicalOr(TokenStream &tokens);
    PResult<LogicalAnd> ParseLogicalAnd(TokenStream &tokens);
    PResult<Eq> ParseEq(TokenStream &tokens);
    PResult<Cond> ParseCond(TokenStream &tokens);
    PResult<Sum> ParseSum(TokenStream &tokens);
    PResult<Term> ParseTerm(TokenStream &tokens);
    PResult<Factor> ParseFactor(TokenStream &tokens);
    PResult<AtomicFactor> ParseAtomicFactor(TokenStream &tokens);
    PResult<ExprFactor> ParseExprFactor(TokenStream &tokens);

    /* Building blocks */

    PResult<Atom> ParseAtom(TokenStream &tokens);
    PResult<Number> ParseNumber(TokenStream &tokens);
    PResult<Name> ParseName(TokenStream &tokens);
};
