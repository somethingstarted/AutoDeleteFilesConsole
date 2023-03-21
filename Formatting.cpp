#include "Formatting.h"


std::string IntergerWithCommas(int64 vv)		//move to seperate class named "formatting" someday. 
{
	std::string s = std::to_string(vv);

	int64 n = s.length() - 3;
	int64 end = (vv >= 0) ? 0 : 1; // Support for negative numbers
	while (n > end) {
		s.insert(n, ",");
		n -= 3;
	}

	return s;
}



std::string IntergerWithCommas(std::bitset<SizeOfBits> vv)		//move to seperate class named "formatting" someday. 
{
	//std::string s = std::to_string(vv);
	std::string s = vv.to_string();

	int64 n = s.length() - 4;
	//int64 end = (vv >= 0) ? 0 : 1; // Support for negative numbers
	//int64 end = 0;
	while (n > 0) {
		s.insert(n, " ");
		n -= 4;
	}

	return s;
}
