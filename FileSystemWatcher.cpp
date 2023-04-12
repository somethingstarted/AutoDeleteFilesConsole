#include "FileSystemWatcher.h"
#include "WatcherThread.h"

class FileDeleterFrame;
class MyProgramFrame;

FileSystemWatcher::FileSystemWatcher(const std::wstring& directory, DirIndexing& indexer)
    : directory_(directory), directoryHandle_(nullptr), threadHandle_(nullptr), exitEvent_(nullptr), indexer(indexer), myprogramframe(myprogramframe)
{
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

//void FileSystemWatcher::StartMonitoring() 
//{
//    directoryHandle_ = CreateFileW(
//        directory_.c_str(),
//        FILE_LIST_DIRECTORY,
//        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
//        nullptr,
//        OPEN_EXISTING,
//        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
//        nullptr);
//
//    if (directoryHandle_ == INVALID_HANDLE_VALUE) {
//        std::cerr << "Failed to open directory: " << GetLastError() << std::endl;
//        return;
//    }
//
//    threadHandle_ = CreateThread(nullptr, 0, WatcherThread, this, 0, nullptr);
//    if (threadHandle_ == nullptr) 
//    {
//        
//        std::stringstream thiserror;
//        thiserror << "ReadDirectoryChangesW failed: " << GetLastError() << std::endl;
//        wxString thiserrror = thiserror.str();
//        wxMessageBox(thiserrror, "", wxICON_INFORMATION);
//
//        return;
//    }
//}
void FileSystemWatcher::StartMonitoring()
{
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

    // Create a new WatcherThread instance and start it
    WatcherThread* thread = new WatcherThread(this);
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
        wxMessageBox(thiserrror, "", wxICON_INFORMATION);
    }
}




DWORD WINAPI FileSystemWatcher::WatcherThread(LPVOID param)
{
    FileSystemWatcher* watcher = static_cast<FileSystemWatcher*>(param);
    const DWORD bufferSize = 1024;
    BYTE buffer[bufferSize] = { 0 };
    DWORD bytesRead = 0;


    std::stringstream thiserror;
    thiserror << "watcher thread started. last error though: " << GetLastError() << std::endl;
    wxString thiserrror = thiserror.str();
    wxMessageBox(thiserrror, "", wxICON_INFORMATION);
    while (true)
    {
        if (ReadDirectoryChangesW(
            watcher->directoryHandle_,
            buffer,
            bufferSize,
            TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE,
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
            } while (fni->NextEntryOffset != 0);
        }
        else
        {
            DWORD error = GetLastError();
            std::cerr << "ReadDirectoryChangesW failed: " << error << std::endl;
            std::stringstream thiserror;
            thiserror << "ReadDirectoryChangesW failed: " << GetLastError() << std::endl;
            wxString thiserrror = thiserror.str();
            wxMessageBox(thiserrror, "", wxICON_INFORMATION);
            if (error == 5 || error == 6) {
                // Sleep for 1 second and try again
                Sleep(1000);
                continue;
            }
            else {
                break;
            }
        }
    }

    return 0;
}

void FileSystemWatcher::StartMonitoring()
{
    WatcherThread* thread = new WatcherThread(this);
    if (thread->Create() == wxTHREAD_NO_ERROR)
    {
        thread->Run();
    }
    else
    {
        // Handle thread creation error
    }
}

void FileSystemWatcher::OnFileSystemChange(DWORD action, const std::wstring& fileName) 
{

    wxMessageBox("on file sys change WORKS!", "", wxICON_INFORMATION);

}