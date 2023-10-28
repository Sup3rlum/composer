#pragma once

#include <iostream>
#include <stdlib.h>

enum ColorCode {
    FG_DEFAULT = 39, 
    FG_BLACK = 30, 
    FG_RED = 31, 
    FG_GREEN = 32, 
    FG_YELLOW = 33, 
    FG_BLUE = 34, 
    FG_MAGENTA = 35, 
    FG_CYAN = 36, 
    FG_LIGHT_GRAY = 37, 
    FG_DARK_GRAY = 90, 
    FG_LIGHT_RED = 91, 
    FG_LIGHT_GREEN = 92, 
    FG_LIGHT_YELLOW = 93, 
    FG_LIGHT_BLUE = 94, 
    FG_LIGHT_MAGENTA = 95, 
    FG_LIGHT_CYAN = 96, 
    FG_WHITE = 97, 
    BG_RED = 41, 
    BG_GREEN = 42, 
    BG_BLUE = 44, 
    BG_DEFAULT = 49
};

template<ColorCode pCode>
struct CText
{
	std::string text;
public:
    CText(const std::string& text) : text(text) {}
    friend std::ostream& operator<<(std::ostream& os, const CText& mod) {
        return os << "\033[" << pCode << "m" << mod.text << "\033[" << FG_DEFAULT << "m";
    }
};

typedef CText<FG_BLUE> CBlue;
typedef CText<FG_RED> CRED;
typedef CText<FG_GREEN> CGreen;
typedef CText<FG_BLUE> CBlue;