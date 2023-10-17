#include <vector>
#include <iostream>
#include <string>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <regex>


class NObject {
public:
    int object_type;
    string strval;
    int intval;
    
    string varname;
    bool boolean_value;

    int* args_variable_id;
    int* statements_variable_id;

    string get_string () {
        if (this->object_type == STRING) {
            return this->strval;
        }
        if (this->object_type == BOOL) {
            if (this->boolean_value == true) return "true";
            else return "false";
        }
        if (this->object_type == INT) {
            return to_string(this->intval);
        }
        return "Нет значения";
    }
};