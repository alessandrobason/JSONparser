#include "JSONparser.h"

/*
enum jsontype:
	undefined
	object
	array
	string
	primitive

struct token:
	jsontype type
	int start
	int end

struct parser:
	int pos
	int next_token
	int super_token (outer token)

*/

JSONparser::JSONparser() {
	//const char* json = "{ \"ciao\": {\"bitch\": 2} }";
	//const char* json = "{\"ciao\": {\"bitch\": 2, \"scemo\": {\"aiuto\": 12} } }";
	//const char* json = "{\"ciao\":2}";
	//std::string json = "{\"ci5ao\":2}";
	//std::string json = "{\"a\":{\"b\":2.3, \"c\":4, \"d\":{\"f\":12}}}, \"e\":3}";
	std::string json = ""

	/*
	"{"
		"\"glossary\": \"bella zio\""
	"}";
	*/

	"{"
	    "\"glossary\": {"
	        "\"title\": true,"
			"\"GlossDiv\": {"
	            "\"title\": \"S\","
				"\"GlossList\": {"
	                "\"GlossEntry\": {"
	                    "\"ID\": \"SGML\","
						"\"SortAs\": \"SGML\","
						"\"GlossTerm\": \"Standard Generalized Markup Language\","
						"\"Acronym\": \"SGML\","
						"\"Abbrev\": \"ISO 8879:1986\","
						"\"GlossDef\": {"
	                        "\"para\": \"A meta-markup language, used to create markup languages such as DocBook.\""
	                    "},"
						"\"GlossSee\": \"markup\""
	                "}"
	            "}"
	        "}"
	    "}"
	"}";
	std::cout << json << std::endl;
	readJSON(json);
}

JSONparser::~JSONparser() {

}

void JSONparser::readJSON(const std::string json) {

	for (; parser.pos < json.length(); parser.pos++) {
		char c;
		c = json[parser.pos];

		switch (c) {
		case '{':
			// check if the token before was a primitive
			if (parser.next_token > 0 && check_if_primitive()) {
				tokens[parser.next_token - 1].end = parser.pos - 1;
				parser.super_token = tokens[parser.next_token - 1].parent;
			}
			// create OBJECT token
			tokens.push_back({ JSON_OBJECT, parser.super_token, parser.pos, 0, 1 });
			// set the super token for next objects
			parser.super_token = parser.next_token;
			parser.next_token++;
			break;
		case '}':
			// check if the token before was a primitive
			if (parser.next_token > 0 && check_if_primitive()) {
				tokens[parser.next_token - 1].end = parser.pos - 1;
				parser.super_token = tokens[parser.next_token - 1].parent;
			}
			// closes super token
			tokens[parser.super_token].end = parser.pos;
			// set new super token to parent
			parser.super_token = tokens[parser.super_token].parent;
			break;
		case '[':
			break;
		case ':':
			break;
		case '"':
			if (tokens[parser.next_token - 1].type == JSON_STRING && tokens[parser.next_token - 1].end == 0) {
				tokens[parser.next_token - 1].end = parser.pos;
			}
			else {
				tokens.push_back({ JSON_STRING, parser.super_token, parser.pos });
				parser.next_token++;
			}
			break;
		case ',':
			// check if the token before was a primitive
			if (parser.next_token > 0 && check_if_primitive()) {
				tokens[parser.next_token - 1].end = parser.pos - 1;
				parser.super_token = tokens[parser.next_token - 1].parent;
			}
			tokens[parser.super_token].size ++;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 't':
		case 'f':
			// if the parent is NOT a key
			// meaning that this is a key
			if (tokens[parser.next_token - 1].type != JSON_STRING) {
				break;
			}
			// if the previous token is an open string
			if (tokens[parser.next_token - 1].type == JSON_STRING && tokens[parser.next_token - 1].end == 0) {
				break;
			}
			// if the previous token is a primitive, meaning that this is still a number
			if (tokens[parser.next_token - 1].type == JSON_PRIMITIVE) {
				break;
			}

			tokens.push_back({ JSON_PRIMITIVE, parser.super_token, parser.pos });
			parser.next_token++;
			break;
		case '\0':
			break;
		default:
			break;
		}
	}

	/*
	for(int i=0; i<tokens.size(); i++){
		jsontype type;
		int parent;
		int start;
		int end;
		int size;
		std::cout << tokens[i].type << " ";
		std::cout << tokens[i].parent << " ";
		std::cout << tokens[i].start << " ";
		std::cout << tokens[i].end << " ";
		std::cout << tokens[i].size << "\n";
	}
	*/
	
	bool is_key = true;
	bool is_array = false;

	// std::map<keytypes, datatypes, comptypes> dict;

	for (int i = 1; i < parser.next_token; i++) {
		datatypes data;
		parseData(result, i, json, is_key, is_array);
	}

	std::cout << "->" << result["glossary"].obj["GlossDiv"].obj["GlossList"].obj["GlossEntry"].obj["GlossDef"].obj["para"].str << "\n";
}

void JSONparser::parseData(std::map<std::string, datatypes> &r, int &i, std::string json, bool &is_key, bool &is_array) {
	int start = tokens[i].start;
	int end = tokens[i].end;
	int len = end - start + 1;

	switch(tokens[i].type){
		case JSON_OBJECT:
		{
			is_key = true;
			const int SIZE = tokens[i].size;
			std::string objKey = currentKey;
			for(int j=0; j<SIZE*2; j++){
				i ++;
				parseData(r[objKey].obj, i, json, is_key, is_array);
			}

			break;
		}
		case JSON_ARRAY:
			break;
		case JSON_STRING:
		{
			std::string s = json.substr(start+1, len-2);
			// it's a key
			if(is_key && !is_array){ 
				is_key = false;
				currentKey = s;
				r[currentKey];
			}
			// it's data
			else if(!is_array){	
				is_key = true;
				r[currentKey].str = json.substr(start+1, len-2);
			}
			// it's an array value
			else{
			}
			break;
		}
		case JSON_PRIMITIVE:
		{
			std::string data = json.substr(start, len);
			datatypes dat;

			bool fin = false;

			for(int j=0; j<data.length() && !fin; j++){
				char c = data[j];
				switch(c){
					case 't':
						dat.b = true;
						fin = true;
						break;
					case 'f':
						dat.b = false;
						fin = true;
						break;
					case '.':
						dat.d = stod(data);
						fin = true;
						break;
				}
			}
			if(!fin) dat.i = stoi(data);

			r[currentKey] = dat;
			is_key = true;
			break;
		}
	}
}

bool JSONparser::check_if_primitive() {
	// check if the token before is a primitive and if it's not finished
	return (tokens[parser.next_token - 1].type == JSON_PRIMITIVE && tokens[parser.next_token - 1].end == 0);
}