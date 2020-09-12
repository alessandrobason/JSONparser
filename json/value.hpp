#pragma once
#include <vector>
#include <map>
#include <string>

namespace json {

    enum class valueType {
        JSONNULL,
        NUMBER, BOOL, STRING, ARRAY, OBJECT
    };

    constexpr const char* valueTypeToString(valueType type) {
        switch(type) {
            case valueType::JSONNULL: return "NULL";
            case valueType::NUMBER:   return "NUMBER";
            case valueType::BOOL:     return "BOOL";
            case valueType::STRING:   return "STRING";
            case valueType::ARRAY:    return "ARRAY";
            case valueType::OBJECT:   return "OBJECT";
        }
        return "ERROR";
    }

    class Value {
        // wrap POD in union
        union {
            double d;
            bool b;
        };
        std::string str;
        std::vector<Value> arr;
        std::map<std::string, Value> obj;
        valueType type;
        
    public:
        Value() : 
        type(valueType::JSONNULL) {}

        Value(int i) : 
        d(i), type(valueType::NUMBER) {}

        Value(double d) : 
        d(d), type(valueType::NUMBER) {}

        Value(bool b) :
        b(b), type(valueType::BOOL) {}

        Value(const char* str) : 
        str(str), type(valueType::STRING) {}

        Value(std::string str) : 
        str(str), type(valueType::STRING) {}

        Value(std::initializer_list<Value> arr) : 
        arr(arr), type(valueType::ARRAY) {}

        Value(std::vector<Value> arr) : 
        arr(arr), type(valueType::ARRAY) {}

        Value(std::initializer_list<std::pair<const std::string, Value>> obj) :
        obj(obj), type(valueType::OBJECT) {}

        Value(std::map<std::string, Value> obj) :
        obj(obj), type(valueType::OBJECT) {}

        Value& operator[](std::string key) {
            return obj[key];
        }

        Value& operator[](size_t index) {
            return arr[index];
        }

        double as_number() {
            return d;
        }

        bool as_bool() {
            return b;
        }

        std::string as_string() {
            return str;
        }

        valueType getType() {
            return type;
        }

        std::string to_string(unsigned int tabs = 1) {
            switch(type) {
                case valueType::JSONNULL:   return "null"; 
                case valueType::NUMBER:     return std::to_string(d); 
                case valueType::BOOL:       return std::to_string(b); 
                case valueType::STRING:     return printString(); 
                case valueType::ARRAY:      return printArray(tabs); 
                case valueType::OBJECT:     return printObject(tabs); 
            }
        }

        std::string printString() {
            return "\"" + str + "\"";
        }

        std::string printArray(unsigned int& tabs) {
            std::string result = "[ ";
            for(Value v: arr) 
                result += v.to_string(tabs) + ", ";
            result[result.length() - 2] = ' ';
            result[result.length() - 1] = ']';
            return result;
        }

        std::string printObject(unsigned int& tabs) {
            std::string result = "{\n";
            for(auto o: obj) {
                // print tabs [\t] before the value
                for(unsigned int i=0; i<tabs; i++)
                    result += "\t";
                result += "\"" + o.first + "\": ";
                result += o.second.to_string(tabs+1);
                result += ",\n";
            }
            result[result.length() - 2] = ' ';
            // print tabs before the closing bracket
            for(unsigned int i=1; i<tabs; i++)
                result += "\t";
            result += "}";
            return result;
        }
    };

}