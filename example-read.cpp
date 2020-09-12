#include <iostream>
#include "json/json.hpp"

int main() {
    json::Parser p = json::Parser();
    json::Value v = p.parse(json::fromFile("example.json"));

    std::cout << p.getText() << "\n";
    std::cout << v.to_string() << "\n";
    std::cout << "->" << v["widget"]["window"]["array"][4]["fuck"].as_string() << "\n";
}