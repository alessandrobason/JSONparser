#pragma once
#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include <type_traits>
#include "typedefs.hpp"

namespace json {
    enum class valueType {
        JSONNULL,
        NUMBER, BOOL, STRING, ARRAY, OBJECT
    };

    inline const char *valueTypeToString(valueType type) {
        switch (type) {
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
        Array arr;
        Object obj;
        valueType type;

    public:
        Value() :
            type(valueType::JSONNULL) {}

        template<
            typename T,
            typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
        >
        Value(T v) :
            d(v), type(valueType::NUMBER) {}

        Value(bool b) :
            b(b), type(valueType::BOOL) {}

        Value(const char *str) :
            str(str), type(valueType::STRING) {}

        Value(const std::string &str) :
            str(str), type(valueType::STRING) {}

        Value(const std::string &&str) :
            str(str), type(valueType::STRING) {}

        Value(const std::initializer_list<Value> &arr) :
            arr(arr), type(valueType::ARRAY) {}

        Value(const Array &arr) :
            arr(arr), type(valueType::ARRAY) {}

        Value(const Array &&arr) :
            arr(arr), type(valueType::ARRAY) {}

        Value(const std::initializer_list<std::pair<const std::string, Value>> &obj) :
            obj(obj), type(valueType::OBJECT) {}

        Value(const Object &obj) :
            obj(obj), type(valueType::OBJECT) {}

        Value(const Object &&obj) :
            obj(obj), type(valueType::OBJECT) {}

        Value &operator[](std::string key) {
            return obj[key];
        }

        Value &operator[](size_t index) {
            return arr[index];
        }

        template<typename... Targs>
        void push_back(Targs ...args) {
            arr.emplace_back(args...);
        }

        double &as_number() {
            return d;
        }

        bool &as_bool() {
            return b;
        }

        std::string &as_string() {
            return str;
        }

        Array &as_array() {
            return arr;
        }

        Object &as_object() {
            return obj;
        }

        const double &as_number() const {
            return d;
        }

        const bool &as_bool() const {
            return b;
        }

        const std::string &as_string() const {
            return str;
        }

        const Array &as_array() const {
            return arr;
        }

        const Object &as_object() const {
            return obj;
        }

        template<typename T>
        T &as() = delete;

        template<>
        double &as() {
            return d;
        }

        template<>
        bool &as() {
            return b;
        }

        template<>
        std::string &as() {
            return str;
        }

        template<>
        Array &as() {
            return arr;
        }

        template<>
        Object &as() {
            return obj;
        }

        template<typename T, typename Q>
        const Q &cast() {
            return (Q)as<T>();
        }

        template<
            typename T,
            typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
        >
            const T cast() {
            return (T)as<double>();
        }

        valueType getType() const {
            return type;
        }

        std::string to_string(unsigned int tabs = 1) const {
            switch (type) {
            case valueType::JSONNULL:   return "null";
            case valueType::NUMBER:     return std::to_string(d);
            case valueType::BOOL:       return b ? "true" : "false";
            case valueType::STRING:     return printString();
            case valueType::ARRAY:      return printArray(tabs);
            case valueType::OBJECT:     return printObject(tabs);
            }
            return "ERROR";
        }

        std::string printString() const {
            return "\"" + str + "\"";
        }

        std::string printArray(unsigned int &tabs) const {
            std::string result = "[ ";
            for (Value v : arr)
                result += v.to_string(tabs) + ", ";
            result[result.length() - 2] = ' ';
            result[result.length() - 1] = ']';
            return result;
        }

        std::string printObject(unsigned int &tabs) const {
            std::string result = "{\n";
            for (auto o : obj) {
                // print tabs [\t] before the value
                for (unsigned int i = 0; i < tabs; i++)
                    result += "\t";
                result += "\"" + o.first + "\": ";
                result += o.second.to_string(tabs + 1);
                result += ",\n";
            }
            result[result.length() - 2] = ' ';
            // print tabs before the closing bracket
            for (unsigned int i = 1; i < tabs; i++)
                result += "\t";
            result += "}";
            return result;
        }
    };

}