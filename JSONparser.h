#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

class JSONparser
{
public:
	JSONparser();
	~JSONparser();

	struct keytypes {
		int i;
		std::string str;
	};

	struct datatypes {
		int i;
		float f;
		double d;
		bool b;
		std::string str;
		std::map<std::string, datatypes> obj;
	};

	struct comptypes {
		bool operator() (keytypes a, keytypes b) const
		{
			return a.i < b.i;
		}
	};

	void readJSON(const std::string json);

	std::map<std::string, datatypes> result;
	std::string jsonText;

private:
	bool check_if_primitive();



	enum jsontype {
		JSON_UNDEFINED,
		JSON_OBJECT,
		JSON_ARRAY,
		JSON_STRING,
		JSON_PRIMITIVE
	};

	struct jsontoken {
		jsontype type;
		int parent;
		int start;
		int end;
		int size;
	};

	struct jsonparser {
		int pos;
		int next_token;
		int super_token;
	};

	void parseData(std::map<std::string, datatypes> &r, int &i, std::string json, bool &is_key, bool &is_array);

	std::vector<jsontoken> tokens;
	jsonparser parser = { 0, 0, 0 };
	std::string currentKey;
};

