#include <vector>
#include <iostream>
#include <string>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <regex>

using namespace std;




class Lexer {
    public:
        int position;
        string text;
        vector<Token> tokens;
        Lexer (string text) {
            this->position = 0;
            this->text = text;
        }
    
    vector<Token> tokenize () {
        while (this->position < this->text.length()) {
            
            string current_string(1, this->peek());

            if (isdigit(this->peek())) {
                this->tokenize_number();
            } else if (this->peek() == '"') {
                this->next();
                this->tokenize_string();
            } else if (std::regex_match(current_string, std::regex("\\w|_"))) {
                this->tokenize_variable();
            }

            if (OPERATOR_CHARS.find(this->peek()) != OPERATOR_CHARS.end()) {
                this->tokenize_operator();
            }
            
            this->next();
        }


        // for (Token token : this->tokens)
        //     std::cout << token.symbol << ' ';
        
        return this->tokens;
    }

    void tokenize_operator () {
        char current = this->peek();
        int kwd = this->get_operator_char_keyword(current);
        string current_string(1, current);
        this->add_token(kwd, current_string);
    }

    void tokenize_number () {
        string number = "";
        while (isdigit(this->peek(0))) {
            number += this->peek(0);
            this->next();
        }   
        this->add_token(INT, number);
    }

    void tokenize_string() {
        string strval = "";
        char current = this->peek();

        while (current != '"') {
            strval += current;
            
            this->next();
            current = this->peek();
            
        }
        this->add_token(STRING, strval);
    }

    void tokenize_variable() {
        string variable = "";
        char current = this->peek();
        while (isalnum(current) || current == '_') {
            variable += current;
            this->next();
            current = this->peek();
            
        }
        int kwd = this->keyword(variable);
        if (kwd) {
            add_token(kwd, variable);
        } else {
            add_token(VARIABLE, variable);
        }
    }
    int keyword(string name) {
        return KEYWORDS[name];
    }
    int get_operator_char_keyword(char name) {
        return OPERATOR_CHARS[name];
    }

    
    void add_token (int code, string value) {
        Token token = Token();
        token.symbol = value;
        token.type = code;
        this->tokens.push_back(token);
    }

    void next () {
        this->position += 1;
    }

    char peek (int relative_position = 0) {

        int position = this->position + relative_position;
        // if (position < this->text.length()) {
        //     return '\0';
        // }
        return this->text[position];
    }
};