#include <vector>
#include <iostream>
#include <string>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <regex>

using namespace std;

class FunctionStatement : public Statement {

    public:
    vector <Statement*> statements;
    string funcname;
    vector<Token> args;
    FunctionStatement (string name, vector<Statement*> statements, vector<Token> args) {
        this->statements = statements;
        this->funcname   = name;
        this->args       = args;
    }
        

    void eval (map<string, NObject*>* vm) override {
        NObject* new_func = new NObject();
        new_func->object_type = FUNCTION;
        new_func->statements_variable_id = (int *)&this->statements;
        new_func->args_variable_id = (int *)&this->args;
        
        vm->insert({this->funcname, new_func});
    }
};

class FunctionCallExpression : public BaseExpression {
    public:
    string funcname;
    vector <BaseExpression*> args;
    FunctionCallExpression (string name, vector<BaseExpression*> args) {
        this->funcname = name;
        this->args = args;
    }

    NObject* eval (map<string, NObject*>* vm) override {

        NObject* function = vm->at(this->funcname);

        vector<Token>& arguments = (vector<Token>&)*function->args_variable_id;

        map<string, NObject*>* virtual_vm = new map<string, NObject*>;
        virtual_vm->insert(vm->begin(), vm->end());
        int i = 0;

        for (auto token: arguments) {
            virtual_vm->insert({token.symbol, this->args[i]->eval(vm)});
            i++;
        }
        
        int* address = function->statements_variable_id;
        // cout << address << endl;
        vector<Statement*>& statements = (vector<Statement*>&)*address;
        for (Statement* statement : statements) {
            try {
                statement->eval(virtual_vm);
            } catch (NObject* object) {
                return object;
            }
        }
        return new NObject();
    }

};

// class ReturnException
// {
// public: 
//     ReturnException(NObject* object): object{}
//     NObject* object;
// };

class ReturnStatement : public Statement {
    public:
    BaseExpression* expression;
    ReturnStatement (BaseExpression* expression) {
        this->expression = expression;
    }
    void eval (map<string, NObject*>* vm) override {
        throw this->expression->eval(vm);
    }
};