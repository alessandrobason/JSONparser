#include <iostream>
#include "json/json.hpp"

int main() {
    // create parser object
    json::Parser p = json::Parser();
    // parse json from file
    json::Value v = p.parse(json::fromFile("example.json"));
    // parse json from string
    json::Value s = p.parse("{ \"test\": \"value\" }");

    // pretty print the json
    std::cout << v.to_string() << "\n";
    // get specific value with a STL-like api
    std::cout << "->" << v["widget"]["window"]["array"][4]["fuck"].as_string() << "\n";
}