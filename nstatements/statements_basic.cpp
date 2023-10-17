#include <vector>
#include <iostream>
#include <string>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <regex>
class Statement { 
    
    public:
    int time_type = BEING;

    Statement () {

    }

    virtual void eval (map<string, NObject*>* vm) {
        
    }
};

