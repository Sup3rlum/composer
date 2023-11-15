#include "parser.h"


PResult<StructDefinition> Parser::ParseStructDefinition(TokenStream& tokens)
{
	MARK();

	if (!tokens.Match(TokenType::KwStruct))
		return NONMATCH();

	auto structName = ParseName(tokens);
	if (!structName) return ERROR("Expected identifier");

	if (!tokens.Match(TokenType::LCurly)) return ERROR("Expected struct body");
	MemberFunctionDefinition* member = 0;
	VarDeclaration* field = 0;

	std::vector<MemberFunctionDefinition*> members;
	std::vector<VarDeclaration*> fields;

	tokens.PushScope(structName);
	{
		do
		{
			member = ParseMemberFunctionDefinition(tokens);
			if (member)
			{
				members.push_back(member);
				continue;
			}

			field = ParseVarDeclaration(tokens);
			if (field)
			{
				if (!tokens.Match(TokenType::OpSemicolon)) return ERROR("Expected ';'");
				fields.push_back(field);
				continue;
			}

		} while (field || member);

	}
	tokens.PopScope();

	if (!tokens.Match(TokenType::RCurly)) return ERROR("Expected '}'");

	return new StructDefinition{ structName, fields, members};
}
