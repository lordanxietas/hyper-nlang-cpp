#include <vector>
#include <iostream>
#include <string>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <regex>
class BasicDeclarativeStatement : public Statement {
public:
    int variable_type;
    string variable_name;
    BaseExpression* expression;

};


class BeingDeclarativeStatement : public BasicDeclarativeStatement { 
public:
    void eval (map<string, NObject*>* vm) {
        NObject* n_object = new NObject();
        n_object->object_type = this->variable_type;
        n_object->varname = this->variable_name;

        if (this->variable_type == INT) {
            n_object->intval = this->expression->eval(vm)->intval;
        } else if (this->variable_type == BOOL) {
            n_object->object_type == BOOL;
            n_object->boolean_value = true;
            this->expression->eval(vm);
        }
        vm->insert({n_object->varname, n_object});
    }
};
