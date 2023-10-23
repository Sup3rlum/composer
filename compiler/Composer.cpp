// Eye.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "tokenizer.h"
#include "parser.h"

int main()
{
	auto tokenList = Tokenizer().Tokenize("func afunc() -> int { }");
	auto ast = Parser().GenerateAST(tokenList);


	printf("Hello world!\n");
}
