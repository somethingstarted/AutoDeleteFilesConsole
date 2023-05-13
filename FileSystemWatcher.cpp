#include "FileSystemWatcher.h"


class FileDeleterFrame;





FileSystemWatcher::FileSystemWatcher(
    const std::wstring& directory, 
    DirIndexing& indexer)
    : directory_(directory), 
        directoryHandle_(nullptr), 
        threadHandle_(nullptr), 
        exitEvent_(nullptr), 
        indexer(indexer), 
        myprogramframe(myprogramframe)
{
    //is commented out
    logging_tool->AppendToLog("fgfd -\t\t ** commented out >>> FileSystemWatcher::FileSystemWatcher()", OutputType::_INFO, WhichClassUsed::FileSystemWatcher_Which);
 
}


FileSystemWatcher::~FileSystemWatcher() 
{
  
    if (directoryHandle_ != nullptr && directoryHandle_ != INVALID_HANDLE_VALUE)
    {
        logging_tool->AppendToLog("gfdgfd -\tdirectoryHandle_ != nullptr && directoryHandle_ != INVALID_HANDLE_VALUE \n\t>>>> CloseHandle(directoryHandle_)", OutputType::EXITING_APP, WhichClassUsed::FileSystemWatcher_Which);
        CloseHandle(directoryHandle_);
    }
 
    if (directoryHandle_ != nullptr) 
    {
        logging_tool->AppendToLog("fgd -\tdirectoryHandle_ != nullptr \n\t>>>> CloseHandle(directoryHandle_)", OutputType::EXITING_APP, WhichClassUsed::FileSystemWatcher_Which);
        CloseHandle(directoryHandle_);
    }
    if (exitEvent_ != nullptr) 
    {
        logging_tool->AppendToLog("msmsag -\texitEvent_ != nullptr \n\t>>>> SetEvent(exitEvent_)", OutputType::_INFO, WhichClassUsed::FileSystemWatcher_Which);
        SetEvent(exitEvent_);
    }
    if (threadHandle_ != nullptr) 
    {
        logging_tool->AppendToLog("gryrw -\tthreadHandle_ != nullptr \n\t>>>> WaitForSingleObject(threadHandle_, INFINITE)", OutputType::_INFO);
        WaitForSingleObject(threadHandle_, INFINITE);
        CloseHandle(threadHandle_);
    }
    if (exitEvent_ != nullptr) 
    {
        logging_tool->AppendToLog("ajajba156 - \texitEvent_ != nullptr \n\t>>>> CloseHandle(exitEvent_)", OutputType::_INFO, WhichClassUsed::FileSystemWatcher_Which);
        CloseHandle(exitEvent_);
    }
}


int FileSystemWatcher::GetFolderIndexSize()
{
    
    int size = indexer.FolderIndex2.size();
            std::stringstream ss;
            ss << "arfds156 - FolderIndex2 size: " << size;
            logging_tool->AppendToLog(ss.str(), OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);
    return size;
}

bool KeepWatchingFolder = true;

DWORD WINAPI FileSystemWatcher::StaticWatcherThread(LPVOID param)
{

    FileSystemWatcher* watcher = static_cast<FileSystemWatcher*>(param);



    const DWORD bufferSize = 1024;
    BYTE buffer[bufferSize] = { 0 };
    DWORD bytesRead = 0;

    if (GetLastError() != 0)
    {
        //Get the error message ID, if any.
        DWORD errorMessageID = ::GetLastError();
        std::stringstream thiserror;
        thiserror << "watcher thread started. last error though: " << GetLastError() << std::endl;
        //watcher->logging_tool->AppendToLog(thiserror.str(), OutputType::_logERROR, WhichClassUsed::FileSystemWatcher_Which);
        LPSTR messageBuffer = nullptr;

        //Ask Win32 to give us the string version of that message ID.
        //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        //Copy the error message into a std::string.
        std::string message(messageBuffer, size);
        std::stringstream errcodenum = {};
        errcodenum << "\tError Code Num:  " << GetLastError();
        watcher->logging_tool->AppendToLog(message + errcodenum.str(), OutputType::_logERROR, WhichClassUsed::FileSystemWatcher_Which);

        //Free the Win32's string's buffer.
        LocalFree(messageBuffer);
    }
    



    //check to see if DirIndexing::FolderIndex2 is empty 
                //long to see if it is empty too
   watcher->logging_tool->AppendToLog("g48df9 - if (watcher->indexer.FolderIndex2.empty())...", OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);
   //get FolderIndex2's ram location and print to logging tool
   std::stringstream ss;
   int siiiize = (watcher->indexer.FolderIndex2.size());
   ss << "FolderIndex2's ram location: " << &watcher->indexer.FolderIndex2 << "\tvec size: " << siiiize << std::endl;
   watcher->logging_tool->AppendToLog(ss.str(), OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);
   
   int DoWhileCounter{};


    if (watcher->indexer.FolderIndex2.empty())
    {

        watcher->logging_tool->AppendToLog("d4f8sd9 - \t\tempty if initiated.", OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);
	}
    else if (!watcher->indexer.FolderIndex2.empty())
    {
        watcher->logging_tool->AppendToLog("3fs5 - \t\tnot empty. else {... ", OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);
        do
        {
            std::stringstream thiserror;
            thiserror << "513132 watcher->FileSystemIsWatched = true; last error: " << GetLastError() << std::endl;
            watcher->logging_tool->AppendToLog(thiserror.str(), OutputType::_logERROR, WhichClassUsed::FileSystemWatcher_Which);

            if (watcher->directoryHandle_ == nullptr || buffer == nullptr || bufferSize == 0 || bytesRead == 0)
            {
                if (watcher->directoryHandle_ == nullptr)
                {
                    watcher->logging_tool->AppendToLog("3gwg - Directory handle is nullptr. Cannot watch the directory.", OutputType::_logERROR, WhichClassUsed::FileSystemWatcher_Which);
                }
                else if (buffer == nullptr)
                {
                    watcher->logging_tool->AppendToLog("w34td -buffer is nullptr. Cannot watch the directory.", OutputType::_logERROR, WhichClassUsed::FileSystemWatcher_Which);
                }
                else if (bufferSize == 0)
                {
                    watcher->logging_tool->AppendToLog("dgds -bufferSize is 0. Cannot watch the directory.", OutputType::_logERROR, WhichClassUsed::FileSystemWatcher_Which);
                }
                else if (bytesRead == 0)
                {
                    watcher->logging_tool->AppendToLog("dds3o - bytesRead is 0. Cannot watch the directory.", OutputType::_logERROR, WhichClassUsed::FileSystemWatcher_Which);
                }

                break; // Exit the loop
            }

            
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
                watcher->logging_tool->AppendToLog("684d6d- if (ReadDirectoryChangesW(", OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);
                DWORD offset = 0;
                FILE_NOTIFY_INFORMATION* fni = nullptr;

                do
                {
                    watcher->logging_tool->AppendToLog("sefm -\t\tdo", OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);

                    fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer + offset);
                    std::wstring fileName(fni->FileName, fni->FileNameLength / sizeof(wchar_t));

                    std::wcout << L"Action: " << fni->Action << L", File: " << fileName << std::endl;

                    offset += fni->NextEntryOffset;
                    watcher->OnFileSystemChange(fni->Action, fileName);
                } while (fni->NextEntryOffset != 0);
            }
            else
            {
                watcher->logging_tool->AppendToLog("pu80y -\t\telse", OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);

                DWORD error = GetLastError();
                std::cerr << "0876lj -ReadDirectoryChangesW failed: " << error << std::endl;
                std::stringstream thiserror;
                thiserror << "ljhkljh -ReadDirectoryChangesW failed: " << GetLastError() << std::endl;

                watcher->logging_tool->AppendToLog(thiserror.str(), OutputType::_logERROR, WhichClassUsed::FileSystemWatcher_Which);

                //recoverable errors go here - maybe just one if, catch all errors, and then print all errors to log?
                if (error == 5 || error == 6)
                {
 
                    //wxSleep(1); //second


                    continue;
                }
                 
            }

           // wxSleep(1);//seconds
           //sleep this thread for one second
            std::this_thread::sleep_for(std::chrono::seconds(1));
        } while (KeepWatchingFolder == true);
    }
    watcher->FileSystemIsWatched = false;

    watcher->logging_tool->AppendToLog("51518 watcher->FileSystemIsWatched loop ended;");  //it's crashing because the thread isn't being handled,
                                                                                        //and it is expecting something from the thread. 
                                                                                        //fix this, then fix the thread problems. 
    


    
    return 0;
}

void FileSystemWatcher::StartMonitoring()
{
    logging_tool->AppendToLog("d456m4q - void FileSystemWatcher::StartMonitoring() started", OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);
    //logging_tool->AppendToLog("z nbng - void FileSystemWatcher::StartMonitoring() started - whole thing commented out" , OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);
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
        logging_tool->AppendToLog(error.str(), OutputType::_logERROR, WhichClassUsed::FileSystemWatcher_Which);
        return;
    }

}

void FileSystemWatcher::OnFileSystemChange(DWORD action, const std::wstring& fileName) 
{
    //wxMessageBox("OnFileSystemChange", "", wxICON_INFORMATION);
    std::wcout << L"Action: " << action << L", File: " << fileName << std::endl;
    logging_tool->AppendToLog("zcxvbt56 -  void FileSystemWatcher::OnFileSystemChange(DWORD action, const std::wstring& fileName) started", OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);
            //need to get these working again, because that's what will make the program keep working. i think this is why it's crashing. 
    //write quick windows alert box 
    wxMessageBox("ihtvds - OnFileSystemChange", "", wxICON_INFORMATION);

     //rewrite this but instead of std::wcout, use logging_tool->AppendToLog
    std::stringstream ss;
     switch (action)
     {
         case FILE_ACTION_ADDED:
             
             ss << "File added: " << fileName << std::endl;
             logging_tool->AppendToLog(ss.str(), OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);
			 break;
         case FILE_ACTION_REMOVED:
            
             ss << "File removed: " << fileName << std::endl;
             logging_tool->AppendToLog(ss.str(), OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);
             break;
        case FILE_ACTION_MODIFIED:
			
			 ss << "File modified: " << fileName << std::endl;
			 logging_tool->AppendToLog(ss.str(), OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);
			 break;
		 case FILE_ACTION_RENAMED_OLD_NAME:
			
			 ss << "File renamed (old name): " << fileName << std::endl;
			 logging_tool->AppendToLog(ss.str(), OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);
			 break;
		 case FILE_ACTION_RENAMED_NEW_NAME:
			
			 ss << "File renamed (new name): " << fileName << std::endl;
			 logging_tool->AppendToLog(ss.str(), OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);
			 break;
		 default:
			
			 ss << "Unknown action: " << action << std::endl;
			 logging_tool->AppendToLog(ss.str(), OutputType::VERBOSE, WhichClassUsed::FileSystemWatcher_Which);
			 break;
     }

}