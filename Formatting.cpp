#include "Formatting.h"


std::string Formatting::IntergerWithCommas(int64 vv = {})		//move to seperate class named "formatting" someday. 
{		
		//if vv is empty, but vvv isn't, use vvv. 
	/*if (vvv != 0 && vv == 0)
		vv = vvv;*/

	std::string s = std::to_string(vv);

	int64 n = s.length() - 3;
	int64 end = (vv >= 0) ? 0 : 1; // Support for negative numbers
	while (n > end) {
		s.insert(n, ",");
		n -= 3;
	}

	return s;
}

std::stringstream Formatting::GetFileAge(chrono_ftt TimeLastModified2) //time last modified 
{
	std::string answer{};
	std::stringstream ss_buffer{};


	auto diff = std::chrono::file_clock::now() - TimeLastModified2;

			
	auto diffHoursOld = std::chrono::round<std::chrono::hours>(diff);
	auto diffDaysOld = std::chrono::round<std::chrono::days>(diff);
	auto diffMonthsOld = std::chrono::round<std::chrono::months>(diff);
	auto diffYearsOld = std::chrono::round<std::chrono::years>(diff);

	//hours
	if (diffHoursOld.count() <= 24)
		ss_buffer << diffHoursOld.count() << " hours old";

	else if ((diffDaysOld.count() +1 ) <= 31)
		ss_buffer << diffDaysOld.count() << " days old";
			
	else if ((diffMonthsOld.count() + 1) <= 12)
		ss_buffer << diffMonthsOld.count() << " months old";

	else if ((diffYearsOld.count() + 1) <= 12)
		ss_buffer << diffYearsOld.count() << " years old";


	//auto diffMonthsOld = std::chrono::round<std::chrono::months>(diff);
	//auto diffYearsOld = std::chrono::round<std::chrono::years>(diff);
	
	//std::chrono::hours



	//ss_buffer << answer;
	
	return ss_buffer;
}

std::string Formatting::BitsWithSpaces(std::bitset<SizeOfBits> vv)		//move to seperate class named "formatting" someday. 
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
