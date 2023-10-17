#include <vector>
#include <iostream>
#include <string>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <regex>

class Parser {
public:
    vector<Token> tokens;
    vector<Statement*> statements;
    int size = 0;
    int position = 0;

    Parser (vector<Token> tokens) {
        this->tokens = tokens;
        this->size = tokens.size();
    }

    vector<Statement*> parse () {
        while (this->position < this->size) {
            Statement* statement = this->function_statement();
            //В зависимости от типа statement будет засовываться в начало или в конец списка
            //А потом дальше уже разберусь 
            if (statement->time_type == BEING) {
                this->statements.push_back(statement);
            } else {
                this->statements.insert(this->statements.begin(), statement);
                
            }
            if (this->verify(DOTCOMMA))
                this->consume();
        }
        
        return this->statements;
    }
    Statement* function_statement () {
        if ((this->verify(BEING)|this->verify(NOTHING))
            && (this->verify(INT, 1)|this->verify(STRING, 1))
            && this->verify(FUNCTION, 2)) {

            int time_type = this->consume().type;
            this->consume();
            this->consume();
            this->verify(VARIABLE);
            string funcname = this->consume().symbol;
            this->verify(LPAR);
            this->consume();

            //Здесь будем получать именованные и типованные аргументы
            vector<Token> args;

            while (!this->verify(RPAR)) {
                int arg_type = this->consume().type;
                string arg_varname= this->consume().symbol;
                args.push_back(Token(arg_type, arg_varname));
                if (this->verify(COMMA))
                    this->consume(); // , между аргументами
            }
            
            this->consume();
            this->verify(LBRACE);
            this->consume();
            vector<Statement*> statements;
            while (!this->verify(RBRACE)) {
                Statement* statement = this->function_statement();

                if (statement->time_type == BEING) {
                    statements.push_back(statement);
                } else {
                    statements.insert(statements.begin(), statement);
                }
                if (this->verify(DOTCOMMA)) {
                    this->consume();
                }
            }
            this->consume();
            FunctionStatement* funcstate = new FunctionStatement(funcname, statements, args);
            funcstate->time_type = time_type;
            return funcstate;
        }
        return this->print_statement();
    }
    Statement* print_statement() {
        if (
            (this->verify(BEING)|
            this->verify(NOTHING)) & this->verify(PRINT, 1)) {
            int time_type = this->consume().type;
            this->consume();
            this->verify(LPAR);
            this->consume();
            BaseExpression* expression = this->expression();
            PrintStatement* print_statement = new PrintStatement();
            print_statement->time_type = time_type;
            print_statement->expression = expression;
            this->verify(RPAR);
            this->consume();
            return print_statement;
        }
        
        return this->return_statement();
    }

    Statement* return_statement () {
        if ((this->verify(BEING)|this->verify(NOTHING)) & this->verify(RETURN, 1)) {
            this->consume();
            this->consume();
            BaseExpression* expression = this->logicalor();
            return new ReturnStatement(expression);
        }
        return this->declaration_statement();
    }
    Statement* declaration_statement() {
        Token current = this->get();
        if (this->verify(BEING)|this->verify(NOTHING)) {
            Token timetype = this->consume();
            // Declaration being variable.
            // Эта переменная реально существует как бы
            BeingDeclarativeStatement* declarative = new BeingDeclarativeStatement();
            declarative->time_type = timetype.type;
            declarative->variable_type = this->get().type;
            this->consume();
            declarative->variable_name = this->get().symbol;
            this->consume();
            this->verify(EQUAL);
            this->consume();
            BaseExpression* expr = this->expression();
            declarative->expression = expr;
            return declarative;
        }
        return this->commentary_statement();
    }

    Statement* commentary_statement() {
        if ((this->verify(SLASH)) & this->verify(STAR, 1)) {
            Token tok = this->consume();
            Token tok1 = this->consume();
            while (!(this->verify(STAR) & this->verify(SLASH, 1))) {
                this->consume();
            }
            Token tok2 = this->consume();
            Token tok3 = this->consume();
        }
        return this->statement();
    } 

    

    Statement* statement () {
        return new Statement();
    }

    BaseExpression* expression() {
        return this->logicalor();
    }


    // Expression new_() {
    //     if (match(NEW)) {
    //         Getter getter = getter();
    //         return NewExpression(getter, getter.list[0].args);
    //     }
    //     return logicalor();
    // }

    BaseExpression* logicalor() {
        BaseExpression* result = this->logicaland();
        while (true) {
            if (this->verify(LOGICAL_OR)) {
                Token current = this->consume();
                result = new ConditionalExpression(current.type, result, this->logicaland());
                continue;
            }
            break;
        }
        return result;
    }

    BaseExpression* logicaland() {
        BaseExpression* result = this->equality();
        while (true) {
            if (this->verify(LOGICAL_AND)) {
                Token current = this->consume();
                result = new ConditionalExpression(current.type, result, this->equality());
                continue;
            }
            break;
        }
        return result;
    }

    BaseExpression* equality() {
        BaseExpression* result = this->conditional();
        while (true) {
            if (this->verify(EQUAL), this->verify(EQUAL, 1)) {
                this->consume();
                this->consume();
                result = new ConditionalExpression(EQEQUAL, result, this->conditional());
                continue;
            }
            break;
        }
        return result;
    }

    BaseExpression* conditional() {
        BaseExpression* result = this->additive();
        while (true) {
            if (this->verify(LT)) {
                this->consume();
                result = new ConditionalExpression(LT, result, this->additive());
                continue;
            }
            if (this->verify(GT)) {
                this->consume();
                result = new ConditionalExpression(GT, result, this->additive());
                continue;
            }
            break;
        }
        return result;
    }

    BaseExpression* additive() {
        BaseExpression* result = this->multiplicative();
        while (true) {
            if (this->verify(PLUS)) {
                this->consume();
                result = new BinaryExpression(PLUS, result, this->multiplicative());
                continue;
            }
            if (this->verify(MINUS)) {
                this->consume();
                result = new BinaryExpression(MINUS, result, this->multiplicative());
                continue;
            }
            break;
        }
        return result;
    }

    BaseExpression* multiplicative() {
        BaseExpression* result = this->basic();
        while (true) {
            if (this->verify(STAR)) {
                this->consume();
                result = new BinaryExpression(STAR, result, this->basic());
                continue;
            }
            if (this->verify(SLASH)) {
                this->consume();
                result = new BinaryExpression(SLASH, result, this->basic());
                continue;
            }
            break;
        }
        return result;
    }

    BaseExpression* basic () {
        if (this->verify(INT)) {
            return new NumberExpression(this->consume().symbol);
        }
        if (this->verify(STRING)) {
            return new StringExpression(this->consume().symbol);
        }
        if (this->verify(VARIABLE) && this->verify(LPAR, 1)) {
            string func_name = this->consume().symbol;
            Token tok1 = this->consume();

            // Здесь аргументы функции
            vector<BaseExpression*> args;
            Token tok = this->get();
            while (!this->verify(RPAR)) {
                BaseExpression* result = this->expression();
                args.push_back(result);
                if (this->verify(COMMA))
                    this->consume();
            }
            this->consume();
            return new FunctionCallExpression(func_name, args);
        }
        
        if (this->verify(VARIABLE)) {
            return new VariableExpression(this->consume().symbol);
        }
        
        if (this->verify(BOOL)) {
            return new BooleanExpression(this->consume().symbol);
        }
        return new BaseExpression();
    }
    
    Token get (int relativePosition = 0) {
        int pos = this->position + relativePosition;
        return this->tokens[pos];
    }
    void next () {
        this->position += 1;
    }

    Token consume () {
        
        Token tok = this->tokens[this->position];
    
        this->next();

        return tok;
    }

    bool verify (int token_type, int relative_pos= 0) {
        if (this->position+relative_pos >= this->size) return false;
        if (this->get(relative_pos).type == token_type) {
            return true;
        }
        return false;
    }
};