#include <iostream>
#include <string>
#include <fstream>

#include "ntypes/types.cpp"
#include "nvm/nvm.cpp"
#include "nobject/nobject.cpp"


#include "nexpressions/expressions.cpp"

#include "nstatements/statements_basic.cpp"
#include "nstatements/declarative_statement.cpp"
#include "nstatements/print_statement.cpp"
#include "nstatements/function_statement.cpp"


#include "nlexer/lexer.cpp"
#include "nparser/parser.cpp"

using namespace std;

bool run (string text) {
    Lexer lexer = Lexer(text);
    vector<Token> tokens = lexer.tokenize();
    
    // for (Token token : tokens) {
    //     cout << token.symbol + ' ';
    // }
    Parser parser = Parser(tokens);
    vector<Statement*> statements = parser.parse();
    map<string, NObject*>* vm = new map<string, NObject*>;
    for (Statement* statement : statements) {
        statement->eval(vm);
    }
    return false;
}

string read_file(const string &fileName)
{
    ifstream ifs(fileName.c_str(), ios::in | ios::binary | ios::ate);

    ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, ios::beg);

    vector<char> bytes(fileSize);
    ifs.read(bytes.data(), fileSize);

    return string(bytes.data(), fileSize);
}
int main () {
    setlocale(LC_ALL, "Russian");
    string program = read_file("test.n");
    //Есть 2 варианта: изначально создать инвертированную версию файла программы и обычную версию,
    //и кооперировать их по одной виртуальной машине объектов

    //2 вариант - делать инвертирование уже внутри
    run(program);
}