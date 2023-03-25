#include "OperatingSystemCheck.h"
#include "Formatting.h"
#include "FileDeleterFrame.h"

void OperatingSystemCheck::IsThisWindows()
{

	int ThisIsWindowsRight = 0;

#if __linux__
	ThisIsWindowsRight--;

#elif _WIN32
#if defined(_WIN64)
	ThisIsWindowsRight++;
	std::cout << "Win 64-bit? yes. good.\n";
#else
	std::cout << "Win 32-bit? yes. good.\n";
	ThisIsWindowsRight++;
#endif
#else
	ThisIsWindowsRight--;
#endif

	if (ThisIsWindowsRight <= 0)
	{
		std::cout << "sorry, this is not windows and may do bad things. exiting is reccomended...";
		std::cout << "\t\npress any button to continue... \n";

		std::cin.get();
	}
	return;
}