#pragma once
#include <vector>
#include <string>
#include "token.hpp"
#include "value.hpp"

namespace json {

    class Deserializer {
    public:
        Deserializer(std::vector<Token>& tokens, std::string& text) :
        tokens(tokens), text(text) {}

        Value run();

    private:
        bool isAtEnd();

        Value createObject();

        std::string createString();
        double createNumber();
        bool createBoolean();
        std::vector<Value> createArray();

        size_t current = 0;
        std::vector<Token>& tokens;
        std::string& text;
    };

}