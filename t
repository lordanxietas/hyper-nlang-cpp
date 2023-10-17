

    // BaseExpression* unary() {
    //     if (this->verify(MINUS)) {
    //         return UnaryExpression(MINUS, this->basic());
    //     }
    //     if (this->verify(PLUS)) {
    //         return basic();
    //     }
    //     return basic();
    // }


    // if (this->match(LQB)) {
        //     vector<Expression*> array;
        //     while (!this->match(RQB)) {
        //         array.push_back(this->expression());
        //         this->match(COMMA);
        //     }
        //     return ListExpression(array);
        // }
        // if (this->match(LBRACE)) {
        //     map<string, Expression*> array;
        //     while (!this->match(RBRACE)) {
        //         // array.push_back(this->expression());
        //         string name = this->primary();
        //         this->consume(COLON);
        //         array[name] = this->expression();
        //         this->match(COMMA);
        //     }
        //     return DictExpression(array);
        // }

        // if (this->match(LPAR)) {
        //     Expression* result = this->expression();
        //     this->match(RPAR);
        //     return result;
        // }