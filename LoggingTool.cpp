#include "LoggingTool.h"


void LoggingTool::AppendToLog(std::string TextForPrintingToLog, int16 LogOutputType)
{

    std::chrono::time_point RightNow = chrono_sys_clock::now();
    std::string FileNameOfLog = {};
    FileNameOfLog = "AutoFileDeleter LOGGING TOOL"; //exclude the .txt extension
    std::ofstream outputFile{};
    int LogFileVers{};
    std::exception e{};
    //try to open the file, if can't, create a new one with same file name but incremented +01
    try 
    {
		outputFile.open(FileNameOfLog + ".txt", std::ios::app);
	}
    catch (std::exception& e)
    {
                //TODO - create a functional system w/ a vector that queues up functions. 
                    //https://chat.openai.com/c/17cda66b-9571-4c6c-b7d8-fca4a66f9581
		outputFile.open(FileNameOfLog + std::to_string(LogFileVers) + ".txt", std::ios::app);
	}	
	
	
    if (LogOutputType & OutputType::SkipLine)
    {
            outputFile << "\n\n"
                << "*** \t *** application reopened at:\t"
                << RightNow
                << "\t   ***\n";
    }
    else if (outputFile.is_open()) 
    {
        // Write the string to the file
        outputFile << RightNow << "\t" << CheckOutputType(LogOutputType) << "\t" << TextForPrintingToLog << "\n";

        // Close the file
        outputFile.close();
        //std::cout << "Successfully wrote to the file." << std::endl;
    }
  

    return;
}

std::string LoggingTool::CheckOutputType(int16 LogOutputType)
{
    std::string a{};
    if (LogOutputType & OutputType::_logERROR)
    {
        a = "ERROR";
    }
    else if (LogOutputType & OutputType::_WARNING)
    {
        a = "WARNING";
    }
    else if (LogOutputType & OutputType::_INFO)
    {
        a = "INFO";
    }
    else if (LogOutputType & OutputType::DEBUG)
    {
        a = "DEBUG";
    }
    else if (LogOutputType & OutputType::AUDIT)
    {
        a = "AUDIT";
    }
    else if (LogOutputType & OutputType::CRITICAL)
    {
        a = "CRITICAL";
    }
    else if (LogOutputType & OutputType::VERBOSE)
    {
        a = "VERBOSE";
    }
    else if (LogOutputType & OutputType::PERFORMANCE)
    {
        a = "PERFORMANCE";
    }
    else if (LogOutputType & OutputType::SECURITY)
    {
        a = "SECURITY";
    }
    else if (LogOutputType & OutputType::METRICS)
    {
        a = "METRICS";
    }
    else if (LogOutputType & OutputType::MAINTENANCE)
    {
        a = "MAINTENANCE";
    }
    else if (LogOutputType & OutputType::SYS_INFO)
    {
        a = "SYS_INFO";
    }
    else if (LogOutputType & OutputType::SkipLine)
    {
        a = "SkipLine";
    }
    else
    {
        a = "MISC";
    }

    return a;
}








