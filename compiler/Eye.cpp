// Eye.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "tokenizer.h"

int main()
{
	auto tokenList = Tokenizer().Tokenize("if (a>5) { ExecuteOrder66(palpatinePassword); } else { force.RestoreBalance(); }");

	for (auto& t : tokenList)
	{
		std::cout << t.data << std::endl;
	}
}
