# JSONparser
A json parser with STL-like api

### Example usage
Read from json
```cpp
#include <iostream>
#include "json.hpp"

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
    std::cout << "->" << v["widget"]["window"]["array"][4]["hello"].as_string() << "\n";
}
```

Write to json
```cpp
#include <iostream>
#include <fstream>
#include "json.hpp"

int main() {
    // create parser object
    json::Parser p = json::Parser();
    // create json object
    json::Value v;
    // initialize as object
    v = json::Object();
    // insert value with = operator
    v["key"] = "value";
    v["number"] = 15.4;
    v["boolean"] = true;
    // initialize array
    v["array"] = json::Array { v["key"], v["number"], v["boolean"] };
    // can also leave json::Array out
    v["array2"] = { v["key"], v["number"], v["boolean"] };
    // initialize object
    v["object"] = json::Object {
        { "test", json::Null() },
        { "number", 9915 }
    };
    // pretty print json
    std::cout << v.to_string() << "\n";
    // write to file
    std::ofstream out("output.json");
    out << v.to_string();
}
```