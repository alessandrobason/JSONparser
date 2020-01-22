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

JSONparser::JSONparser(const std::string path) {
	readJSON(path);
}

JSONparser::~JSONparser() {

}

void JSONparser::readJSON(const std::string path) {
	std::string json = "";

	std::ifstream is(path);
	char c;

	while (is.get(c)) {
		json += c;
		switch (c) {
		case '{':
			// check if the token before was a primitive
			if (parser.next_token > 0 && check_if_primitive()) {
				tokens[parser.next_token - 1].end = parser.pos - 1;
				parser.super_token = tokens[parser.next_token - 1].parent;
			}
			// create OBJECT token
			tokens.push_back({ JSON_OBJECT, parser.super_token, parser.pos, 0 });
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
			tokens.push_back({JSON_ARRAY, parser.super_token, parser.pos});
			parser.super_token = parser.next_token;
			parser.next_token++;
			break;
		case ']':
			// check if the token before was a primitive
			if (parser.next_token > 0 && check_if_primitive()) {
				tokens[parser.next_token - 1].end = parser.pos - 1;
				parser.super_token = tokens[parser.next_token - 1].parent;
			}
			tokens[tokens[parser.next_token - 1].parent].end = parser.pos;
			parser.super_token = tokens[tokens[parser.next_token - 1].parent].parent;
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
			// if its not an array check if its still an open variable (string/primitive)
			if (tokens[parser.next_token - 1].type != JSON_ARRAY && tokens[parser.next_token - 1].end == 0) {
				break;
			}
			
			tokens.push_back({ JSON_PRIMITIVE, parser.super_token, parser.pos });
			parser.next_token++;
			break;
		default:
			break;
		}
		parser.pos++;
	}

	/*
	std::cout << "i\tt\tp\ts\te\tsz\n";
	for(int i=0; i<tokens.size(); i++){
		jsontype type;
		int parent;
		int start;
		int end;
		int size;
		std::cout << i << "\t";
		std::cout << tokens[i].type << "\t";
		std::cout << tokens[i].parent << "\t";
		std::cout << tokens[i].start << "\t";
		std::cout << tokens[i].end << "\t";
		std::cout << tokens[i].size << "\n";
	}
	*/
	
	bool is_key = true;
	bool is_array = false;

	for (int i = 1; i < tokens.size(); i++) {
		parseData(doc, i, json, is_key, false);
	}

}

void JSONparser::parseData(std::map<std::string, datatypes>& r, int& i, std::string json, bool& is_key, bool is_array) {
	int start = tokens[i].start;
	int end = tokens[i].end;
	int len = end - start + 1;

	switch(tokens[i].type){
		case JSON_OBJECT:
		{
			is_key = true;
			std::string objKey = currentKey;
			int self_token = i;

			if (is_array) {
				datatypes dt;
				dt.type = VAR_OBJECT;
				while (i + 1 < tokens.size() && tokens[i + 1].parent == self_token) {
					i++;
					parseData(dt.obj, i, json, is_key, false);
				}
				r[objKey].arr.push_back(dt);
			}
			else {
				r[objKey].type = VAR_OBJECT;
				while (i + 1 < tokens.size() && tokens[i + 1].parent == self_token) {
					i++;
					parseData(r[objKey].obj, i, json, is_key, false);
				}
			}
			
			break;
		}
		case JSON_ARRAY:
		{
			int self_token = i; 
			if (is_array) {
				// create a temporary map to store current array
				std::string arrKey = currentKey;
				std::map<std::string, datatypes> temp;
				while (i + 1 < tokens.size() && tokens[i + 1].parent == self_token) {
					i++;
					parseData(temp, i, json, is_key, true);
				}

				datatypes vec_arr;
				vec_arr.type = VAR_ARRAY;
				// copy the data from the temp map to the final
				for (int k = 0; k < temp[arrKey].arr.size(); k++) {
					datatypes dt;
					dt.type = temp[arrKey].arr[k].type;
					switch (dt.type) {
					case VAR_INT:
						dt.i = temp[arrKey].arr[k].i;
						break;
					case VAR_DOUBLE:
						dt.d = temp[arrKey].arr[k].d;
						break;
					case VAR_TRUE:
					case VAR_FALSE:
						dt.b = temp[arrKey].arr[k].b;
						break;
					case VAR_STRING:
						dt.str = temp[arrKey].arr[k].str;
						break;
					}
					vec_arr.arr.push_back(dt);
				}
				r[arrKey].arr.push_back(vec_arr);
			}
			else {
				r[currentKey].type = VAR_ARRAY;
				while (i + 1 < tokens.size() && tokens[i + 1].parent == self_token) {
					i++;
					parseData(r, i, json, is_key, true);
				}
			}
			
			break;
		}
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
				r[currentKey].str = s;
				r[currentKey].type = VAR_STRING;
			}
			// it's an array value
			else{
				datatypes at;
				at.str = s;
				at.type = VAR_STRING;
				r[currentKey].arr.push_back(at);
			}
			break;
		}
		case JSON_PRIMITIVE:
		{
			std::string data = json.substr(start, len);

			if (is_array) {
				datatypes dt;
				dt.type = parsePrimitive(data);
				switch (dt.type) {
				case VAR_INT:
					dt.i = stoi(data);
					break;
				case VAR_DOUBLE:
					dt.d = stod(data);
					break;
				case VAR_TRUE:
					dt.b = true;
					break;
				case VAR_FALSE:
					dt.b = false;
					break;
				default:
					break;
				}
				r[currentKey].arr.push_back(dt);
				break;
			}
			else {
				r[currentKey].type = parsePrimitive(data);
				switch (r[currentKey].type) {
				case VAR_INT:
					r[currentKey].i = stoi(data);
					break;
				case VAR_DOUBLE:
					r[currentKey].d = stod(data);
					break;
				case VAR_TRUE:
					r[currentKey].b = true;
					break;
				case VAR_FALSE:
					r[currentKey].b = false;
					break;
				default:
					break;
				}
				is_key = true;
				break;
			}
		}
	}
}

bool JSONparser::check_if_primitive() {
	// check if the token before is a primitive and if it's not finished
	return (tokens[parser.next_token - 1].type == JSON_PRIMITIVE && tokens[parser.next_token - 1].end == 0);
}

int JSONparser::parsePrimitive(std::string data) {
	for (int j = 0; j < data.length(); j++) {
		char c = data[j];
		switch (c) {
		case 't':
			return VAR_TRUE;
			//r[currentKey].b = true;
			break;
		case 'f':
			return VAR_FALSE;
			//r[currentKey].b = false;
			break;
		case '.':
			return VAR_DOUBLE;
			//r[currentKey].d = stod(data);
			break;
		}
	}
	return VAR_INT;
}