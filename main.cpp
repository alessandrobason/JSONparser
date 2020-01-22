#include "JSONparser.h"

int main(){
	std::string path = "test.json";
	JSONparser* j = new JSONparser(path);
	//std::cout << "->" << j->doc["glossary"].obj["GlossDiv"].obj["GlossList"].obj["GlossEntry"].obj["GlossDef"].obj["GlossSeeAlso"].arr[0].str << "\n";
	std::cout << "->" << j->doc["layers"].arr[1].obj["data"].type << "\n";

	delete j;
	j = NULL;
}