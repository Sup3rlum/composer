#include "parser.h"

PResult<CompDefinition> Parser::ParseCompDefinition(TokenStream& tokens)
{
	MARK();

	if (!tokens.Match(TokenType::KwComp))
		return NONMATCH();

	auto compositionName = ParseName(tokens);
	if (!tokens.Match(TokenType::LSquare)) return ERROR("Expected '['");

	std::vector<Name*> CompNames;
	Name* componentName = ParseName(tokens);
	if (!componentName) return ERROR("Expected component identifier");
	CompNames.push_back(componentName);

	bool compComma = tokens.Match(TokenType::OpComma);
	if (compComma) // More than one component
	{
		componentName = ParseName(tokens);
		if (!componentName) return ERROR("Expected a component identifier");
		CompNames.push_back(componentName);

		while (componentName && compComma)
		{
			compComma = tokens.Match(TokenType::OpComma);
			componentName = ParseName(tokens);

			if (compComma && !componentName) return ERROR("Expected a component identifier");
			if (componentName)
				CompNames.push_back(componentName);
		}
	}

	if (!tokens.Match(TokenType::RSquare)) return ERROR("Expected ']'");
	if (!tokens.Match(TokenType::LCurly)) return ERROR("Expected struct body");

	MemberFunctionDefinition* member = 0;
	VarDeclaration* field = 0;
	std::vector<MemberFunctionDefinition*> members;
	std::vector<VarDeclaration*> fields;

	tokens.PushScope(compositionName);
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

	} while (member || field);
	tokens.PopScope();

	if (!tokens.Match(TokenType::RCurly)) return ERROR("Expected '}'");

	return new CompDefinition{ compositionName, CompNames, fields, members };
}