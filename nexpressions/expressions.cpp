#include <vector>
#include <iostream>
#include <string>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <regex>

class BaseExpression { 
    
    public:
    BaseExpression () {

    }

    virtual NObject* eval (map<string, NObject*>* vm) {
        return new NObject();
    }
};

class NumberExpression : public BaseExpression { 
    
    public:
    int number;
    NumberExpression (string number) {
        this->number = stoi(number);
    }

    NObject* eval (map<string, NObject*>* vm) override {
        NObject* n_object = new NObject();
        n_object->object_type = INT;
        n_object->intval = this->number;

        return n_object;
    }
};

class VariableExpression : public BaseExpression { 
    
    public:
    string varname;
    VariableExpression (string name) {
        this->varname = name;
    }

    NObject* eval (map<string, NObject*>* vm) override {
        try {
            return vm->at(this->varname);
        } catch (out_of_range) {
            cout << "Переменной " << this->varname << " не существует" << endl;
        }
        return new NObject();
    }
};



class StringExpression : public BaseExpression { 
    
    public:
    string str;
    StringExpression (string str) {
        this->str = str;
    }

    NObject* eval (map<string, NObject*>* vm) override {
        NObject* n_object = new NObject();
        n_object->object_type = STRING;
        n_object->strval = this->str;
        return n_object;
    }
};

class BooleanExpression : public BaseExpression { 
    
    public:
    string str;
    BooleanExpression (string str) {
        this->str = str;
    }

    NObject* eval (map<string, NObject*>* vm) override {
        NObject* n_object = new NObject();
        n_object->object_type = BOOL;
        if (this->str == "true") {
            n_object->boolean_value = true;
        } else if (this->str == "false") {
            n_object->boolean_value = false;
        }
        return n_object;
    }
};

class ConditionalExpression : public BaseExpression { 
    
    public:
    BaseExpression* first;
    BaseExpression* second;
    int operation;
    ConditionalExpression (int operation, BaseExpression* first, BaseExpression* second) {
        this->operation = operation;
        this->first = first;
        this->second = second;
    }

    NObject* eval (map<string, NObject*>* vm) override {
        if (this->operation == LOGICAL_OR) {
            NObject* first_result = this->first->eval(vm);
            NObject* second_result= this->second->eval(vm);


            if (first_result->object_type != BOOL or second_result->object_type != BOOL) {
                cout << "Операция может быть проведена только с типом boolean" << endl;
                return new NObject();
            }
            NObject* n_object = new NObject();
            n_object->object_type = BOOL;
            n_object->boolean_value = first_result->boolean_value | second_result->boolean_value;
            return n_object;
        } else if (operation == LOGICAL_AND) {
            NObject* first_result = this->first->eval(vm);
            NObject* second_result= this->second->eval(vm);

            NObject* n_object = new NObject();
            n_object->object_type = BOOL;
            n_object->boolean_value = first_result->boolean_value & second_result->boolean_value;
            return n_object;
        } else if (operation == EQEQUAL) {
            NObject* first_result = this->first->eval(vm);
            NObject* second_result= this->second->eval(vm);

            NObject* n_object = new NObject();
            n_object->object_type = BOOL;
            if (first_result->object_type == second_result->object_type) {
                if (first_result->object_type == INT) {
                    n_object->boolean_value = first_result->intval == second_result->intval;
                } else if (first_result->object_type == STRING) {
                    n_object->boolean_value = first_result->strval == second_result->strval;
                } else if (first_result->object_type == BOOL) {
                    n_object->boolean_value = first_result->boolean_value == second_result->boolean_value;
                }
            }
            
            return n_object;
        } else if (operation == LT) {
            NObject* first_result = this->first->eval(vm);
            NObject* second_result= this->second->eval(vm);

            NObject* n_object = new NObject();
            n_object->object_type = BOOL;
            if (first_result->object_type == second_result->object_type) {
                if (first_result->object_type == INT) {
                    n_object->boolean_value = first_result->intval < second_result->intval;
                } else if (first_result->object_type == STRING) {
                    n_object->boolean_value = first_result->strval < second_result->strval;
                }
            } else {
                cout << "Ошибка. Сравниваемые типы могут быть только одинаковыми\n";
            }
            
            return n_object;
        } else if (operation == GT) {
            NObject* first_result = this->first->eval(vm);
            NObject* second_result= this->second->eval(vm);

            NObject* n_object = new NObject();
            n_object->object_type = BOOL;
            if (first_result->object_type == second_result->object_type) {
                if (first_result->object_type == INT) {
                    n_object->boolean_value = first_result->intval > second_result->intval;
                } else if (first_result->object_type == STRING) {
                    n_object->boolean_value = first_result->strval > second_result->strval;
                }
            } else {
                cout << "Ошибка. Сравниваемые типы могут быть только одинаковыми\n";
            }
            
            
            return n_object;
        }
        
        return new NObject();
    }
};


class BinaryExpression : public BaseExpression { 
    
    public:
    BaseExpression* first;
    BaseExpression* second;
    int operation;
    BinaryExpression (int operation, BaseExpression* first, BaseExpression* second) {
        this->operation = operation;
        this->first = first;
        this->second = second;
    }

    NObject* eval (map<string, NObject*>* vm) override {
        if (this->operation == PLUS) {
            NObject* first_result = this->first->eval(vm);
            NObject* second_result= this->second->eval(vm);

            NObject* n_object = new NObject();
            if (first_result->object_type == second_result->object_type) {
                if (first_result->object_type == INT) {
                    n_object->intval = first_result->intval + second_result->intval;
                } else if (first_result->object_type == STRING) {
                    n_object->strval = first_result->strval + second_result->strval;
                }
            } else {
                cout << first_result->object_type << " " << second_result->object_type << endl;
                cout << "Ошибка. Сравниваемые типы могут быть только одинаковыми\n";
            }
            
            n_object->object_type = first_result->object_type;
            return n_object;
        } else if (this->operation == MINUS) {
            NObject* first_result = this->first->eval(vm);
            NObject* second_result= this->second->eval(vm);

            NObject* n_object = new NObject();
            if (first_result->object_type == second_result->object_type) {
                if (first_result->object_type == INT) {
                    n_object->intval = first_result->intval - second_result->intval;
                }
            } else {
                cout << "Ошибка. Сравниваемые типы могут быть только одинаковыми\n";
            }
            
            n_object->object_type = first_result->object_type;
            return n_object;
        } else if (this->operation == STAR) {
            NObject* first_result = this->first->eval(vm);
            NObject* second_result= this->second->eval(vm);

            NObject* n_object = new NObject();
            if (first_result->object_type == second_result->object_type) {
                if (first_result->object_type == INT) {
                    n_object->intval = first_result->intval * second_result->intval;
                }
            } else {
                cout << "Ошибка. Сравниваемые типы могут быть только одинаковыми\n";
            }
            
            n_object->object_type = first_result->object_type;
            return n_object;
        } else if (this->operation == SLASH) {
            NObject* first_result = this->first->eval(vm);
            NObject* second_result= this->second->eval(vm);

            NObject* n_object = new NObject();
            if (first_result->object_type == second_result->object_type) {
                if (first_result->object_type == INT) {
                    n_object->intval = first_result->intval / second_result->intval;
                }
            } else {
                cout << "Ошибка. Сравниваемые типы могут быть только одинаковыми\n";
            }
            
            n_object->object_type = first_result->object_type;
            return n_object;
        }
        return new NObject();
    }
};

class UnaryExpression : public BaseExpression { 
    
    public:
    string str;
    UnaryExpression (string str) {
        this->str = str;
    }

    NObject* eval (map<string, NObject*>* vm) override {
        return new NObject();
    }
};



