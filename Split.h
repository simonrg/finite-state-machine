#include <vector>
#include <string>
#include <iostream>
using namespace std;

class Split{
public:
	vector<string> split(string rawstr, char split_on){
		vector<string> bits;

		int size = 0;
		int start = 0;
		for (size_t i = 0; i < rawstr.length(); i++){
			if (rawstr[i] == split_on){
				size = i - start;
				bits.push_back(rawstr.substr(start, size));
				start = i + 1;
			}
			if (i == rawstr.length() - 1){
				size = i - start;
				bits.push_back(rawstr.substr(start, size + 1));
			}
		}
		return bits;
	}
};