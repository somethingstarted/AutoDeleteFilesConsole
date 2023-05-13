#include "LoggingTool.h"
#include "Formatting.h"



void LoggingTool::AppendToLog(std::string TextForPrintingToLog, uint16 LogOutputType, uint16 WhichClassUsed )
{

    std::chrono::time_point RightNow = chrono_sys_clock::now();
    std::string FileNameOfLog = {};
    FileNameOfLog = "AutoFileDeleter LOGGING TOOL"; //exclude the .txt extension
    std::ofstream outputFile{};
    int LogFileVers{};
    std::exception e{};
    DWORD errorMessageID = GetLastError();
    std::stringstream thiserror{};
    std::stringstream errorbuffer{};

    if (thiserror.str() == LastErrorGotten.str())
    {
        errorbuffer << thiserror.str() << "\t";
    }
    else
    {
        errorbuffer << "";
    }

    //try to open the file, if can't, create a new one with same file name but incremented +01
    try 
    {
		outputFile.open(FileNameOfLog + ".txt", std::ios::app);
	}
    catch (std::exception& e)
    {
                //TODO - create a functional system w/ a vector that queues up functions. 
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
          outputFile << RightNow << "\t" << errorbuffer.str() << ClassReferenced(WhichClassUsed) << CheckOutputType(LogOutputType) << "\t" << TextForPrintingToLog << "\n";
           outputFile.close();
    }
  

    LastErrorGotten << thiserror.str(); 
    return;
}

std::string LoggingTool::ClassReferenced(uint16 WhichClassUsed)
{

    std::string s{};
    //match WhichClassUsed in "class WhichClassUsed" to the corresponding string except for the _Which portion of the text
    if (WhichClassUsed & WhichClassUsed::AutoDeleteFiles_Which)
    {
        s =  "AutoDeleteFiles\t\t";
    }
    else if (WhichClassUsed & WhichClassUsed::DirectoryIndexing_Which)
    {
        s = "DirectoryIndexing\t";
    }
    else if (WhichClassUsed & WhichClassUsed::FileDeleterFrame_Which)
    {
        s = "FileDeleterFrame\t";
	}
    else if (WhichClassUsed & WhichClassUsed::Formatting_Which)
    {
        s = "Formatting\t\t";
	}
    else if (WhichClassUsed & WhichClassUsed::LoggingTool_Which)
    {
        s = "LoggingTool\t\t";
	}
    else if (WhichClassUsed & WhichClassUsed::FileSystemWatcher_Which)
    {
		s = "FileSystemWatcher\t";
    }
    else if (WhichClassUsed & WhichClassUsed::WatcherThread_Which)
    {
        s = "WatcherThread\t\t";
    }
    else if (WhichClassUsed & WhichClassUsed::FileSystemWatcherThread_Which)
    {
		s = "FileSystemWatcherThread\t";
	}
    else if (WhichClassUsed & WhichClassUsed::MiscClass_Which)
    {
		s = "MiscClass\t\t";
	}
    else
    {
        s = "Unknown/ext\t\t";
	}

    return s;
}

std::string LoggingTool::CheckOutputType(uint16 LogOutputType)
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
    else if (LogOutputType & OutputType::VECTOR)
    {
        a = "VECTOR";
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
    else if (LogOutputType & OutputType::EXITING_APP)
    {
        a = "EXITING_APP";
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


LoggingTool* logging_tool = nullptr; // Initialize the pointer to nullptr





