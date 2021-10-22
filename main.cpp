#include <unordered_set>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

typedef std::vector<unsigned char> minterm;
typedef std::set<minterm> minset;

std::vector<char> alphabet = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
};

enum OutTypes { LOGIC, C, VHDL } outType = LOGIC;
bool printImpTables = false;

std::string vecToString(const std::vector<unsigned char>& v) {
	std::string rtn = "";

	for(auto it = v.rbegin(); it != v.rend(); ++it) {

		switch(*it) {
			case 0:
				rtn += "0 ";
				break;
			case 1:
				rtn += "1 ";
				break;
			case 2:
				rtn += "x ";
				break;
		}
	}
	return rtn;
}

std::string vecToAlpha(const std::vector<unsigned char>& v) {
	std::string rtn1 = "";
	std::string rtn2 = "";

	int count = 0;

	for(auto it = v.rbegin(); it != v.rend(); ++it, count++) {

		switch(*it) {
			case 0:
				rtn1 += "_";
				rtn2 += alphabet[count];
				break;
			case 1:
				rtn1 += " ";
				rtn2 += alphabet[count];
				break;
			case 2:
				//Do nothing here
				break;
		}
	}
	rtn1 += "\n";
	//rtn2 += "\n";
	return rtn1 + rtn2;
}

void printImplicants(const std::set<std::vector<unsigned char>>& imps) {
	if(outType == LOGIC) {

		std::string rtn1 = "";
		std::string rtn2 = "";

		for(auto itt = imps.begin(); itt != imps.end(); ++itt) {
			auto v = *itt;
			int count = 0;

			for(auto it = v.rbegin(); it != v.rend(); ++it, count++) {

				switch(*it) {
					case 0:
						rtn1 += "_";
						rtn2 += alphabet[count];
						break;
					case 1:
						rtn1 += " ";
						rtn2 += alphabet[count];
						break;
					case 2:
						//Do nothing here
						break;
				}
			}

			rtn1 += "  ";
			rtn2 += ", ";

		}

		std::cout << rtn1 << std::endl;
		std::cout << rtn2 << std::endl;
	} else if(outType == C) {
		std::string rtn = "";

		for(auto itt = imps.begin(); itt != imps.end(); ++itt) {
			auto v = *itt;
			int count = 0;

			for(auto it = v.rbegin(); it != v.rend(); ++it, count++) {
				switch(*it) {
					case 0:
						//Has to be std::string("!"), not "!", because
						//otherwise wierd memory access occurs
						//(In this case began printing "nsufficient arguments for: " and 
						//"sufficient arguments for: ", a string which appears later in the program)
						//Probably because const char* + std::string operator
						//is not defined or is beign misused
						rtn += std::string("!") + alphabet[count];
						break;
					case 1:
						rtn += alphabet[count];
						break;
					case 2:
						//Do nothing here
						break;
				}

				if(it + 1 != v.rend()) { //Don't output separator if on last element
					rtn += " && ";
				}

			}

			//if((imps.end() - itt) != 1) { //Don't output separator if on last element
			if(std::next(itt) != imps.end()) { //Don't output separator if on last element
				rtn += " || ";
			}

		}

		std::cout << rtn << std::endl;
	} else if(outType == VHDL) {
		std::string rtn = "";

		for(auto itt = imps.begin(); itt != imps.end(); ++itt) {
			auto v = *itt;
			int count = 0;

			if(v.rbegin() != v.rend()) rtn += "(";

			for(auto it = v.rbegin(); it != v.rend(); ++it, count++) {
				switch(*it) {
					case 0:
						//See comparable comment in (outType == C) section
						rtn += std::string("NOT ") + alphabet[count];
						break;
					case 1:
						rtn += alphabet[count];
						break;
					case 2:
						//Do nothing here
						break;
				}

				if(it + 1 != v.rend()) { //Don't output separator if on last element
					rtn += " AND ";
				}

			}
			if(v.rbegin() != v.rend()) rtn += ")";

			//if((imps.end() - itt) != 1) { //Don't output separator if on last element
			if(std::next(itt) != imps.end()) { //Don't output separator if on last element
				rtn += " OR ";
			}

		}

		std::cout << rtn << std::endl;
	}
}

//int hammingDistance

unsigned int count(const std::vector<unsigned char>& v) {
	unsigned int rtn = 0;
	for(auto it = v.begin(); it != v.end(); ++it) {
		rtn += (*it == 1);
	}

	return rtn;
}

struct List {
	std::map<unsigned int, std::set<std::vector<unsigned char>>> groups;
	std::set<std::vector<unsigned char>> implicants;
	int maxPower = 5;

	std::string toString(int listnum = -1) {
		std::string rtn;
		if(listnum != -1) {
			for(int i = 0; i < maxPower; i++) {
				rtn += "===";
			}
			rtn += "\n";
			rtn += "List " + std::to_string(listnum) + "\n";
			for(int i = 0; i < maxPower; i++) {
				rtn += "===";
			}
			rtn += "\n";
		}
		for(auto it = groups.begin(); it != groups.end(); ++it) {
			rtn += "Group " + std::to_string(it->first) + "\n";
			for(auto itt = it->second.rbegin(); itt != it->second.rend(); ++itt) {
				rtn += vecToString(*itt) + "\n";
			}

			for(int i = 0; i < maxPower; i++) {
				rtn += "==";
			}
			rtn += "\n";
		}

		{
			rtn += "Implicants\n";
			for(auto it = implicants.rbegin(); it != implicants.rend(); ++it) {
				rtn += vecToString(*it) + "\n";
			}
			for(int i = 0; i < maxPower; i++) {
				rtn += "====";
			}
			rtn += "\n";
		}

		return rtn;
	}

	void add(std::vector<unsigned char> v) {
		unsigned int c = count(v);
		groups[c].insert(v);
	}

	void addImplicant(std::vector<unsigned char> v) {
		implicants.insert(v);
	}

	void removeImplicant(std::vector<unsigned char> v) {
		implicants.erase(v);
	}

	bool isEmpty() {
		return groups.empty();
	}
};

std::string impToString(std::set<std::vector<unsigned char>> implicants) {
	std::cout << "Prime Implicants" << std::endl;
	std::string rtn;
	int count = 0;
	for(auto it = implicants.rbegin(); it != implicants.rend(); ++it) {
		rtn += vecToAlpha(*it) + "\n";
	}
	return rtn;
}



void initializeList(List& l, std::vector<unsigned int>& entries) {
	std::sort(entries.begin(), entries.end());

	unsigned int max = entries.back();
	//std::cout << max << std::endl;

	int maxPower = 0;

	while(max > 0) {
		max >>= 1;
		maxPower++;
	}

	//std::cout << maxPower << std::endl;

	for(auto it = entries.begin(); it != entries.end(); ++it) {
		std::vector<unsigned char> v;
		for(int i = 0; i < maxPower; i++) {
			v.push_back( ((1 << i) & *it) >> i);
		}
		l.add(v);
		l.addImplicant(v);
	}

	l.maxPower = maxPower;
}

std::set<std::vector<unsigned char>> vectorize(std::vector<unsigned int> entries) {
	std::sort(entries.begin(), entries.end());

	unsigned int max = entries.back();
	//std::cout << max << std::endl;

	int maxPower = 0;

	while(max > 0) {
		max >>= 1;
		maxPower++;
	}

	//std::cout << maxPower << std::endl;

	std::set<std::vector<unsigned char>> rtn;

	for(auto it = entries.begin(); it != entries.end(); ++it) {
		std::vector<unsigned char> v;
		for(int i = 0; i < maxPower; i++) {
			v.push_back( ((1 << i) & *it) >> i);
		}
		rtn.insert(v);
	}
	return rtn;
}

int areAdjacent(const std::vector<unsigned char>& a, const std::vector<unsigned char>& b) {
	int hd = 0;
	int lastChange;
	int i = 0;
	for(auto ita = a.begin(), itb=b.begin(); ita != a.end(); ita++, itb++) {
		if(*ita == *itb) {
			//Do nothing
		} else if(*ita == 2 || *itb == 2) { //If compare blank to number, fail
			return -1;

		} else {
			hd += 1;
			lastChange = i;
		}
		i++;
	}

	if(hd == 1) { //Adjacent
		return lastChange;
	} else {
		return -1;
	}

}

List iteration(List& in) {
	std::set<std::vector<unsigned char>> values;

	List out;

	for(auto it = in.groups.begin(); it != in.groups.end(); it++) {
		for(auto itt = std::next(it); itt != in.groups.end(); itt++) {
			for(auto x = it->second.begin(); x != it->second.end(); ++x) {
				for(auto y = itt->second.begin(); y != itt->second.end(); ++y) {
					int adj = areAdjacent(*x, *y);

					if(adj != -1) {
						std::vector<unsigned char> add = *x;
						add[adj] = 2;
						out.add(add);
						out.addImplicant(add);
						in.removeImplicant(*x);
						in.removeImplicant(*y);
					}


				}
			}
		}
	}

	//If empty, still copy over implicants
	if(out.isEmpty()) {
		for(auto it = in.groups.begin(); it != in.groups.end(); ++it) {
			out.implicants.insert(it->second.begin(), it->second.end());
		}
	}

	return out;

}

bool matches(const std::vector<unsigned char>& val, const std::vector<unsigned char>& implicant) {
	for(auto itv = val.begin(), iti = implicant.begin(); itv != val.end(); itv++, iti++) {
		if(*iti != 2 && *iti != *itv) {
			return false;
		}
	}
	return true;
}

void petricksMethod(const minset& values, const minset& implicants, minset::iterator valueIt, std::set<minset>& results, std::vector<minterm>& working) {
	if(valueIt == values.end()) {

		minset temp;

		for(auto it = working.begin(); it != working.end(); ++it) {
			temp.insert(*it);
		}

		if(results.size() == 0) { //Insert if zero, to intialize
			results.insert(temp);
		} else if(temp.size() < (results.begin())->size()) { //If smaller than element in set, clear and insert
			results.clear();
			results.insert(temp);
		} else if(temp.size() == (results.begin())->size()) { //Just insert if same size
			results.insert(temp);
		} //Don't insert if greater

		//Need to fix
		return;
	}

	for(auto it = implicants.begin(); it != implicants.end(); ++it) {
		if(matches(*valueIt, *it)) {
			working.push_back(*it);
			petricksMethod(values, implicants, std::next(valueIt), results, working);
			working.pop_back();
		}
	}

}

std::set<minset> quineMcClusky(std::vector<unsigned int> careentries, std::vector<unsigned int> dontcare) {
	std::vector<unsigned int> entries;

	entries.insert(entries.end(), careentries.begin(), careentries.end());
	entries.insert(entries.end(), dontcare.begin(), dontcare.end());

	//Initialize list
	List l1;
	initializeList(l1, entries);

	std::vector<List> its;

	int count = 1;
	its.push_back(l1);

	//Iteratively apply implicant identification algoirthm
	while(!its.back().isEmpty()) {
		its.push_back(iteration(its.back()));
		count++;
		if(printImpTables) std::cout << its[its.size() - 2].toString(its.size() - 1) << std::endl;

	}

	//All implicants
	std::set<std::vector<unsigned char>> implicants;
	std::set<std::vector<unsigned char>> essentialResults;

	for(auto it = its.begin(); it != its.end(); ++it) {
		implicants.insert(it->implicants.begin(), it->implicants.end());

	}

	//According to research, do not use don't care entries when minimizing in table
	std::set<std::vector<unsigned char>> values = vectorize(careentries);

	//Identify columns with only one row
	for(auto x = values.begin(); x != values.end(); ++x) {
		auto last = implicants.end();
		int count = 0;
		for(auto y = implicants.begin(); y != implicants.end(); ++y) {
			if(matches(*x, *y)) {
				count += 1;
				last = y;
			}
		}

		if(count == 1) {
			essentialResults.insert(*last);
			//implicants.erase(*last);
		}
	}

	//Remove essentialResults from implicants
	//Also remove matching columns from values
	for(auto it = essentialResults.begin(); it != essentialResults.end(); ++it) {
		implicants.erase(*it);

		auto itt = values.begin();
		while(itt != values.end()) {
			auto current = itt++;

			if(matches(*current, *it)) {
				values.erase(*current);
			}
		}
	}

	std::set<minset> results;
	std::vector<minterm> working;
	petricksMethod(values, implicants, values.begin(), results, working);

	std::set<minset> rtn;

	for(auto it = results.begin(); it != results.end(); ++it) {
		minset output;
		std::merge(essentialResults.begin(), essentialResults.end(), it->begin(), it->end(), std::inserter(output, output.begin()));
		rtn.insert(output);
	}

	return rtn;
}

typedef bool (*arg_handler)(std::vector<std::string> args);

bool handleOut(std::vector<std::string> args) {

	if(args[0] == "logic") {
		outType = LOGIC;
	} else if(args[0] == "C") {
		outType = C;
	} else if(args[0] == "VHDL") {
		outType = VHDL;
	} else {
		std::cerr << "Invalid output type: " << args[0] << std::endl;
		return false;
	}
	return true;
}

bool handleImpTables(std::vector<std::string> args) {
	printImpTables = true;
	return true;	
}

std::map<std::string, arg_handler> argHandlers = {
	{ "-out", handleOut },
	{ "-imptables", handleImpTables }
};

std::map<std::string, unsigned int> argCounts = {
	{ "-out", 1 },
	{ "-imptables", 0 }
};

int main(int argc, char** argv) {
	//{ 0, 2, 6, 7, 8, 10, 11, 12, 13, 14, 16, 18, 19, 29, 30 };
	//{ 0, 1, 3, 7, 8, 9, 11, 15 };
	//{ 1, 3, 5, 10, 12, 11, 13, 14, 15 };

	std::vector<unsigned int> entries;
	std::vector<unsigned int> dontcare;

	for(int i = 1; i < argc; i++) {

		if(argv[i][0] == '-') { //If argument
			if(argHandlers.find(argv[i]) != argHandlers.end()) {
				std::string arg = argv[i];
				int count = argCounts[arg];

				i++;
				std::vector<std::string> args;
				for(int j = 0; j < count; j++) {
					if(i == argc) {
						std::cerr << "Insufficient arguments for: " << arg << std::endl;
						std::cout << "HERE" << std::endl;
						return -1;
					}
					args.push_back(argv[i]);
				
					i++;
				}

				if(!argHandlers[arg](args)) {
					return -1;
				}

				i--;
			} else {
				std::cerr << "Invalid argument: " << argv[i] << std::endl;
			}

		} else if(std::string(argv[i]).find('d') != std::string::npos) {
			std::string temp = std::string(argv[i], std::strlen(argv[i]) - 1); //Clear d
			dontcare.push_back(std::stoul(temp));
		} else {
			entries.push_back(std::stoul(argv[i]));
		}
	}

	if(argc == 0) {
		std::cerr << "Please specify terms to minimize" << std::endl;
	}

	std::set<minset> output = quineMcClusky(entries, dontcare);

	for(auto it = output.begin(); it != output.end(); ++it) {
		printImplicants(*it);
	}
}
