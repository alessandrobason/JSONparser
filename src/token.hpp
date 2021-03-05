#pragma once

#include <string>

namespace json {
    enum class TokenType {
        NOTHING,

        // object
        LEFT_BRACE, RIGHT_BRACE, COLON, 
        COMMA, DOT,
        // array
        LEFT_BRACKET, RIGHT_BRACKET,
        // value
        DOUBLE_QUOTES, NUMBER, STRING,
        BOOLEAN, JSONNULL
    };

    constexpr const char* typeToString(TokenType type);

    struct Token {
        TokenType type;
        size_t start, end;

        Token() = default;
        Token(TokenType type, size_t start, size_t end) {
            this->type = type;
            this->start = start;
            this->end = end;
        }

        inline std::string as_string(std::string& text) const {
            if(type == TokenType::STRING)
                return text.substr(start+1, end-start-2);
            else
                return text.substr(start, end-start);
        }

        std::string to_string(std::string& text) const {
            return  std::string(typeToString(type)) + 
                    std::string(": ") + 
                    as_string(text);
        }
    };

    constexpr const char* typeToString(TokenType type) {
        switch(type) {
            case TokenType::NOTHING:        return "NOTHING";
            case TokenType::LEFT_BRACE:     return "LEFT_BRACE";
            case TokenType::RIGHT_BRACE:    return "RIGHT_BRACE";
            case TokenType::COLON:          return "COLON";
            case TokenType::COMMA:          return "COMMA";
            case TokenType::DOT:            return "DOT";
            case TokenType::LEFT_BRACKET:   return "LEFT_BRACKET";
            case TokenType::RIGHT_BRACKET:  return "RIGHT_BRACKET";
            case TokenType::DOUBLE_QUOTES:  return "DOUBLE_QUOTES";
            case TokenType::NUMBER:         return "NUMBER";
            case TokenType::STRING:         return "STRING";
            case TokenType::BOOLEAN:        return "BOOLEAN";
            case TokenType::JSONNULL:       return "JSONNULL";
        }
        return "ERROR";
    }

}