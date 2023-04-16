#include "FileSystemWatcher.h"


class FileDeleterFrame;
class FileDeleterFrame;

LoggingTool* logging_tool{};

FileSystemWatcher::FileSystemWatcher(const std::wstring& directory, DirIndexing& indexer)
    : directory_(directory), directoryHandle_(nullptr), threadHandle_(nullptr), exitEvent_(nullptr), indexer(indexer), myprogramframe(myprogramframe)
{
    logging_tool->AppendToLog("created new FileSystemWatcher thread", OutputType::_INFO);

    directoryHandle_ = CreateFileW(
        directory_.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        nullptr);


    if (directoryHandle_ == INVALID_HANDLE_VALUE) 
    {
        std::cerr << "Failed to create directory handle: " << GetLastError() << std::endl;
        throw std::runtime_error("Invalid directory handle");
    
    }
    //exitEvent_ = CreateEvent(nullptr, TRUE, FALSE, nullptr);
}


FileSystemWatcher::~FileSystemWatcher() 
{
    logging_tool->AppendToLog("deleted a FileSystemWatcher thread", OutputType::_INFO);
    if (directoryHandle_ != nullptr) {
        CloseHandle(directoryHandle_);
    }
    if (exitEvent_ != nullptr) {
        SetEvent(exitEvent_);
    }
    if (threadHandle_ != nullptr) {
        WaitForSingleObject(threadHandle_, INFINITE);
        CloseHandle(threadHandle_);
    }
    if (exitEvent_ != nullptr) {
        CloseHandle(exitEvent_);
    }
}





DWORD WINAPI FileSystemWatcher::StaticWatcherThread(LPVOID param)
{
    //wxMessageBox("FileSystemWatcher::StaticWatcherThread", "", wxICON_INFORMATION);
    //logging_tool->AppendToLog("FileSystemWatcher::StaticWatcherThread", OutputType::_INFO);
    FileSystemWatcher* watcher = static_cast<FileSystemWatcher*>(param);
    
    const DWORD bufferSize = 1024;
    BYTE buffer[bufferSize] = { 0 };
    DWORD bytesRead = 0;


    std::stringstream thiserror;
    thiserror << "watcher thread started. last error though: " << GetLastError() << std::endl;
    logging_tool->AppendToLog(thiserror.str(), OutputType::_logERROR);
    int DoWhileCounter{};

    do 
    {

        //watcher->FileSystemIsWatched = true;
        logging_tool->AppendToLog(" watcher->FileSystemIsWatched = true;");
        if (watcher->directoryHandle_ == nullptr)
        {
            logging_tool->AppendToLog("Directory handle is nullptr. Cannot watch the directory.", OutputType::_logERROR);
            break; // Exit the loop
        } //other errors here?
        else if (buffer == nullptr)
        {
            logging_tool->AppendToLog("buffer is nullptr. Cannot watch the directory.", OutputType::_logERROR);
            break;
        }
        else if (bufferSize == 0)
        {
            logging_tool->AppendToLog("bufferSize is 0. Cannot watch the directory.", OutputType::_logERROR);
            break;
        }
        else if (bytesRead == 0)
        {
            logging_tool->AppendToLog("bytesRead is 0. Cannot watch the directory.", OutputType::_logERROR);
            break;
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
            DWORD offset = 0;
            FILE_NOTIFY_INFORMATION* fni = nullptr;

            do
            {
                fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer + offset);
                std::wstring fileName(fni->FileName, fni->FileNameLength / sizeof(wchar_t));

                std::wcout << L"Action: " << fni->Action << L", File: " << fileName << std::endl;

                offset += fni->NextEntryOffset;
                watcher->OnFileSystemChange(fni->Action, fileName);
            } 
            while (fni->NextEntryOffset != 0);
        }
        else
        {
            DWORD error = GetLastError();
            std::cerr << "ReadDirectoryChangesW failed: " << error << std::endl;
            std::stringstream thiserror;
            thiserror << "ReadDirectoryChangesW failed: " << GetLastError() << std::endl;
            wxString thiserrror = thiserror.str();
            wxMessageBox(thiserrror, "", wxICON_INFORMATION);

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
  
    watcher->FileSystemIsWatched = false;

    logging_tool->AppendToLog(" watcher->FileSystemIsWatched = false;");
    


    
    return 0;
}

void FileSystemWatcher::StartMonitoring()
{
    wxMessageBox("StartMonitoring", "", wxICON_INFORMATION);
    directoryHandle_ = CreateFileW(
        directory_.c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        nullptr);

    if (directoryHandle_ == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open directory: " << GetLastError() << std::endl;
        return;
    }



    //threadHandle_ = CreateThread(nullptr, 0, StaticWatcherThread, this, 0, nullptr);
    WatcherThread* thread = new WatcherThread(this);


    if (thread == nullptr)
    {
        wxMessageBox("Failed to create WatcherThread instance", "", wxICON_ERROR);
        return;
    }
    if (thread->Create() == wxTHREAD_NO_ERROR)
    {
        thread->Run();
        
    }
    else
    {
        // Handle thread creation error
        std::stringstream thiserror;
        thiserror << "ReadDirectoryChangesW failed: " << GetLastError() << std::endl;
        wxString thiserrror = thiserror.str();
        
    }
}

void FileSystemWatcher::OnFileSystemChange(DWORD action, const std::wstring& fileName) 
{
    wxMessageBox("OnFileSystemChange", "", wxICON_INFORMATION);
    std::wcout << L"Action: " << action << L", File: " << fileName << std::endl;
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