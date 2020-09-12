#include "parser.hpp"
#include "deserializer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace json {

    std::string fromFile(std::string path) {
        std::ifstream file = std::ifstream(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    /* Parse the json, returns false if failed */
    Value Parser::parse(std::string jsonstring) {
        text = jsonstring;

        if(text == "") {
            std::cout << "No json string given\n";
            return Value();
        }

        // scan all the tokens
        while(!isAtEnd()) {
            start = current;
            scanToken();
        }

        #ifdef JSONDEBUG
        std::cout << "TOKENS\n";
        for(auto t: tokens) {
            std::cout << t.to_string(text) << "\n";
        }
        std::cout << "END TOKENS\n";
        #endif

        // build the json file from the tokens
        Deserializer des = Deserializer(tokens, text);
        return des.run();
    }

    void Parser::scanToken() {
        char c = text[current++];
        switch(c) {
            case '{':  addToken(TokenType::LEFT_BRACE); break;
            case '}':  addToken(TokenType::RIGHT_BRACE); break;
            case ':':  addToken(TokenType::COLON); break;
            case ',':  addToken(TokenType::COMMA); break;
            case '.':  addToken(TokenType::DOT); break;
            case '[':  addToken(TokenType::LEFT_BRACKET); break;
            case ']':  addToken(TokenType::RIGHT_BRACKET); break;
            case '\"': addString(); break;

            // ignore whitespaces
            case ' ': case '\n': case '\t': case '\r':
                break;
            default:
                if(isDigit(c)) {
                    addNumber();
                    break;
                }
                else if(isAlpha(c)){
                    addOther();
                    break;
                }

                std::cout << "ERROR: unexpected character [" << c << "]\n";
                break;
        }
    }

    void Parser::addToken(TokenType type) {
        #ifdef JSONDEBUG
        std::cout << "adding token: " << typeToString(type) << "\n";
        #endif
        tokens.push_back(Token(type, start, current));
    }

    
    bool Parser::isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    bool Parser::isAlpha(char c) {
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'A') ||
                c == '_';
    }

    void Parser::addNumber() {
        // find the end of the number
        for(; isDigit(peek()); current++);

        if(peek() == '.' && isDigit(peekNext()))
            for(current++; isDigit(peek()); current++);

        #ifdef JSONDEBUG
        std::string DEBUGnum = text.substr(start, current-start);
        std::cout << "number -> " << DEBUGnum << "\n";
        #endif
        
        addToken(TokenType::NUMBER);
    }

    void Parser::addString() {
        // finds the end of the string
        for(; peek() != '\"'; current++) {
            if(peek() == '\\') {
                current++;
            }
        }
        current++;

        #ifdef JSONDEBUG
        std::string DEBUGstr = text.substr(start, current-start);
        std::cout << "string -> [" << DEBUGstr << "]\n";
        #endif
        
        addToken(TokenType::STRING);
    }

    void Parser::addOther() {
        for(; isAlpha(peek()); current++);
        std::string other = text.substr(start, current-start);
        if(other == "true" || other == "false")
            addToken(TokenType::BOOLEAN);
        else if(other == "null")
            addToken(TokenType::JSONNULL);
        else
            std::cout << "error, not recognized: " << other << "\n";
    }

    bool Parser::isAtEnd() {
        return current >= text.length();
    }

    char Parser::peek() {
        if(isAtEnd()) return '\0';
        return text[current];
    }

    char Parser::peekNext() {
        if(current + 1 >= text.length()) return '\0';
        return text[current+1];
    }

    std::string Parser::getText() {
        return text;
    }

}