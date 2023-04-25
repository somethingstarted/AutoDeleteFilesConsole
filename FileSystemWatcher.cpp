#include "FileSystemWatcher.h"


class FileDeleterFrame;


LoggingTool* logging_tool{};


FileSystemWatcher::FileSystemWatcher(const std::wstring& directory, DirIndexing& indexer)
    : directory_(directory), directoryHandle_(nullptr), threadHandle_(nullptr), exitEvent_(nullptr), indexer(indexer), myprogramframe(myprogramframe)
{
    //is commented out
    logging_tool->AppendToLog("\t\t ** commented out >>> FileSystemWatcher::FileSystemWatcher()", OutputType::_INFO);
 /*   logging_tool->AppendToLog("created new FileSystemWatcher thread", OutputType::_INFO);

    directoryHandle_ = CreateFileW
    (
        directory_.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        nullptr
    );


    if (directoryHandle_ == INVALID_HANDLE_VALUE) 
    {
        std::cerr << "Failed to create directory handle: " << GetLastError() << std::endl;
        throw std::runtime_error("Invalid directory handle");
        logging_tool->AppendToLog("Failed to create directory handle: " + GetLastError(), OutputType::_logERROR);
    
    }*/
 
}


FileSystemWatcher::~FileSystemWatcher() 
{
  
    if (directoryHandle_ != nullptr && directoryHandle_ != INVALID_HANDLE_VALUE)
    {
        logging_tool->AppendToLog("\t\tdirectoryHandle_ != nullptr && directoryHandle_ != INVALID_HANDLE_VALUE \n\t\t\t>>>> CloseHandle(directoryHandle_)", OutputType::_INFO);
        CloseHandle(directoryHandle_);
    }
 
    if (directoryHandle_ != nullptr) 
    {
        logging_tool->AppendToLog("\t\tdirectoryHandle_ != nullptr \n\t\t\t>>>> CloseHandle(directoryHandle_)", OutputType::_INFO);
        CloseHandle(directoryHandle_);
    }
    if (exitEvent_ != nullptr) 
    {
        logging_tool->AppendToLog("\t\texitEvent_ != nullptr \n\t\t\t>>>> SetEvent(exitEvent_)", OutputType::_INFO);
        SetEvent(exitEvent_);
    }
    if (threadHandle_ != nullptr) 
    {
        logging_tool->AppendToLog("\t\tthreadHandle_ != nullptr \n\t\t\t>>>> WaitForSingleObject(threadHandle_, INFINITE)", OutputType::_INFO);
        WaitForSingleObject(threadHandle_, INFINITE);
        CloseHandle(threadHandle_);
    }
    if (exitEvent_ != nullptr) 
    {
        logging_tool->AppendToLog("\t\texitEvent_ != nullptr \n\t\t\t>>>> CloseHandle(exitEvent_)", OutputType::_INFO);
        CloseHandle(exitEvent_);
    }
}





DWORD WINAPI FileSystemWatcher::StaticWatcherThread(LPVOID param)
{

    FileSystemWatcher* watcher = static_cast<FileSystemWatcher*>(param);
    if (!watcher)
    {
        logging_tool->AppendToLog("FileSystemWatcher pointer is null. Exiting StaticWatcherThread.", OutputType::_logERROR);
        return 1; // Return an error code to indicate the issue
    }


    const DWORD bufferSize = 1024;
    BYTE buffer[bufferSize] = { 0 };
    DWORD bytesRead = 0;


    std::stringstream thiserror;
    thiserror << "watcher thread started. last error: " << GetLastError() << std::endl;
    logging_tool->AppendToLog(thiserror.str(), OutputType::_logERROR);

   int DoWhileCounter{};

    //check to see if DirIndexing::FolderIndex2 is empty 
                //long to see if it is empty too
    logging_tool->AppendToLog("if (watcher->indexer.FolderIndex2.empty())...", OutputType::VERBOSE);
    for (auto& i : watcher->indexer.FolderIndex2)
    {   //get file id and file name from DirIndexing::FolderIndex2.FileIDnumber 
        std::stringstream ss;
        ss << "FileIDnumber: " << i.FileIDnumber << " FileName: " << i.FileName << std::endl;
	    logging_tool -> AppendToLog(ss.str(), OutputType::VERBOSE);
		
	}           //end of logging
                //do the real work now. 

    if (watcher->indexer.FolderIndex2.empty())

    {
     
		logging_tool->AppendToLog("\t\tempty if initiated.", OutputType::VERBOSE);
		return 0;
	}
    else
    {
        logging_tool->AppendToLog("\t\tnot empty. else {... ", OutputType::VERBOSE);
        do
        {
            logging_tool->AppendToLog("\t\tdo {... ", OutputType::VERBOSE);
            //watcher->FileSystemIsWatched = true;
            logging_tool->AppendToLog(" watcher->FileSystemIsWatched = true;");
            if (watcher->directoryHandle_ == nullptr || buffer == nullptr || bufferSize == 0 || bytesRead == 0)
            {
                if (watcher->directoryHandle_ == nullptr)
                {
                    logging_tool->AppendToLog("Directory handle is nullptr. Cannot watch the directory.", OutputType::_logERROR);
                }
                else if (buffer == nullptr)
                {
                    logging_tool->AppendToLog("buffer is nullptr. Cannot watch the directory.", OutputType::_logERROR);
                }
                else if (bufferSize == 0)
                {
                    logging_tool->AppendToLog("bufferSize is 0. Cannot watch the directory.", OutputType::_logERROR);
                }
                else if (bytesRead == 0)
                {
                    logging_tool->AppendToLog("bytesRead is 0. Cannot watch the directory.", OutputType::_logERROR);
                }

                break; // Exit the loop
            }

            //i'm tempted to make this a try catch block
            if (ReadDirectoryChangesW(
                watcher->directoryHandle_, //*new question* what happens if this is nullptr? it threw an exception just now
                buffer,
                bufferSize,
                TRUE,
                FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE, //*new qustion* this should work no matter what kind of change happens
                &bytesRead,
                nullptr,
                nullptr))
            {
                logging_tool->AppendToLog(" if (ReadDirectoryChangesW(", OutputType::VERBOSE);
                DWORD offset = 0;
                FILE_NOTIFY_INFORMATION* fni = nullptr;

                do
                {
                    logging_tool->AppendToLog("\t\t\tdo", OutputType::VERBOSE);
                    fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer + offset);
                    std::wstring fileName(fni->FileName, fni->FileNameLength / sizeof(wchar_t));

                    std::wcout << L"Action: " << fni->Action << L", File: " << fileName << std::endl;

                    offset += fni->NextEntryOffset;
                    watcher->OnFileSystemChange(fni->Action, fileName);
                } while (fni->NextEntryOffset != 0);
            }
            else
            {
                logging_tool->AppendToLog("\t\telse", OutputType::VERBOSE);
                DWORD error = GetLastError();
                std::cerr << "ReadDirectoryChangesW failed: " << error << std::endl;
                std::stringstream thiserror;
                thiserror << "ReadDirectoryChangesW failed: " << GetLastError() << std::endl;
                logging_tool->AppendToLog(thiserror.str(), OutputType::_logERROR);

                //recoverable errors go here - maybe just one if, catch all errors, and then print all errors to log?
                if (error == 5 || error == 6)
                {
                    // Sleep for 1 second and try again
                    //Sleep(1000);


                    continue;
                }


                wxSleep(1); //seconds
            }

        } while (true);
    }
    watcher->FileSystemIsWatched = false;

    logging_tool->AppendToLog(" watcher->FileSystemIsWatched = false;");
    


    
    return 0;
}

void FileSystemWatcher::StartMonitoring()
{
     logging_tool->AppendToLog("void FileSystemWatcher::StartMonitoring() started", OutputType::VERBOSE);
    directoryHandle_ = CreateFileW(
        directory_.c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        nullptr);

    if (directoryHandle_ == INVALID_HANDLE_VALUE) 
    {
        std::stringstream error{};
        error << "\tINVALID_HANDLE_VALUE\tFailed to open directory: " << GetLastError() << std::endl;
        logging_tool->AppendToLog(error.str(), OutputType::_logERROR);
        return;
    }



    //threadHandle_ = CreateThread(nullptr, 0, StaticWatcherThread, this, 0, nullptr);
    WatcherThread* thread = new WatcherThread(this);


    if (thread == nullptr)
    {
        logging_tool->AppendToLog("thread == nullptr\tFailed to create WatcherThread instance", OutputType::_WARNING);
        return;
    }
    if (thread->Create() == wxTHREAD_NO_ERROR)
    {
        auto ID = thread->GetId();
        //convert ID to something suitable for logging tool
        std::stringstream ss;
        ss << ID;
        logging_tool->AppendToLog("thread->Create() == wxTHREAD_NO_ERROR\tWatcherThread created successfully. Thread ID == " + ss.str(), OutputType::VERBOSE);
        //get watcher thread thread id


        thread->Run();
        
    }
    else
    {
        logging_tool->AppendToLog("thread->Create() != wxTHREAD_NO_ERROR\tFailed to create WatcherThread", OutputType::_logERROR);
        // Handle thread creation error
        std::stringstream thiserror;
        thiserror << "ReadDirectoryChangesW failed: " << GetLastError() << std::endl;
        wxString thiserrror = thiserror.str();
        
    }
}

void FileSystemWatcher::OnFileSystemChange(DWORD action, const std::wstring& fileName) 
{
    //wxMessageBox("OnFileSystemChange", "", wxICON_INFORMATION);
    std::wcout << L"Action: " << action << L", File: " << fileName << std::endl;
    logging_tool->AppendToLog("void FileSystemWatcher::OnFileSystemChange(DWORD action, const std::wstring& fileName) started", OutputType::VERBOSE);
    /*switch (action)
    {
	case FILE_ACTION_ADDED:
		std::wcout << L"File added: " << fileName << std::endl;
		break;
	case FILE_ACTION_REMOVED:
		std::wcout << L"File removed: " << fileName << std::endl;
		break;
	case FILE_ACTION_MODIFIED:
		std::wcout << L"File modified: " << fileName << std::endl;
		break;
	case FILE_ACTION_RENAMED_OLD_NAME:
		std::wcout << L"File renamed (old name): " << fileName << std::endl;
		break;
	case FILE_ACTION_RENAMED_NEW_NAME:
		std::wcout << L"File renamed (new name): " << fileName << std::endl;
		break;
	default:
		std::wcout << L"Unknown action: " << action << std::endl;
		break;
	}*/

}