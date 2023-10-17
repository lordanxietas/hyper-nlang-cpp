#include <map>
#include <iostream>
#include <string>

using namespace std;

int STRING = 2;
int VARIABLE = 3;
int FUNCTION = 4;
int CLASS = 5;
int PLUS = 6;
int MINUS= 7;
int EQUAL= 8;
int MULTIPLY = 9;
int DIVIDE = 10;
int BEING = 11;
int ANTIBEING = 12;
int NOTHING = 13;
int ANTINOTHING = 14;
int DOTCOMMA = 15;
int LBRACE = 16;
int RBRACE = 17;
int LPAR = 18;
int RPAR = 19;
int PUBLIC = 20;
int PRIVATE = 21;
int STATIC = 22;
int DYNAMIC = 23;
int DOT = 24;
int QUESTION = 25;
int PRINT = 26;
int NONE = 27;
int INT = 28;
int LOGICAL_OR = 29;
int BOOL = 30;
int LOGICAL_AND = 31;
int EQEQUAL = 32;
int LT = 33;
int GT = 34;
int STAR = 33;
int SLASH = 34;
int RETURN = 35;
int COMMA = 36;

map<string, int> KEYWORDS = {
    {"function", FUNCTION},
    {"class", CLASS},
    {"being", BEING},
    {"antibeing", ANTIBEING},
    {"nothing", NOTHING},
    {"antinothing", ANTINOTHING},
    {"return", RETURN},

    {"public", PUBLIC},
    {"private", PRIVATE},
    {"static", STATIC},
    {"dynamic", DYNAMIC},

    {"print", PRINT},
    {"int", INT},
    {"or", LOGICAL_OR},
    {"and", LOGICAL_AND},
    {"true", BOOL},
    {"false", BOOL},
    {"bool", BOOL}
    


    // add more keywords as needed
};

map<char, int> OPERATOR_CHARS = {
    {'+', PLUS},
    {'-', MINUS},
    {'=', EQUAL},
    {'*', STAR},
    {'/', SLASH},
    {';', DOTCOMMA},
    {'{', LBRACE},
    {'}', RBRACE},
    {'(', LPAR},   
    {')', RPAR},
    {'.', DOT},
    {'?', QUESTION},
    {'>', GT},
    {'<', LT},
    {',', COMMA}

    
    // add more keywords as needed
};

class Token {
    public:
        int type;
        string symbol;
    Token (int type, string symbol) {
        this->type = type;
        this->symbol = symbol;
    }
    Token() {
    
    }
};