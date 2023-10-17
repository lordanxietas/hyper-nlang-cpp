#include <vector>
#include <iostream>
#include <string>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <regex>

using namespace std;

class PrintStatement : public Statement {

    public:
        BaseExpression* expression;

    void eval (map<string, NObject*>* vm) override {
        // try {
        cout << this->expression->eval(vm)->get_string() << endl;
        // } catch (out_of_range) {
        //     cout << "Print не удался, переменной не существует" << endl;
        // }
        
    }
};

