#include "FileSystemWatcher.h"

FileSystemWatcher::FileSystemWatcher(const std::wstring& directory, DirIndexing& indexer)
    : directory_(directory), directoryHandle_(nullptr), threadHandle_(nullptr), exitEvent_(nullptr), indexer(indexer)
{
    exitEvent_ = CreateEvent(nullptr, TRUE, FALSE, nullptr);
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

    threadHandle_ = CreateThread(nullptr, 0, WatcherThread, this, 0, nullptr);
    if (threadHandle_ == nullptr) 
    {
        std::cerr << "Failed to create watcher thread: " << GetLastError() << std::endl;
        return;
    }
}

DWORD WINAPI FileSystemWatcher::WatcherThread(LPVOID param) 
{
    FileSystemWatcher* watcher = static_cast<FileSystemWatcher*>(param);
    const DWORD bufferSize = 1024;
    BYTE buffer[bufferSize] = { 0 };
    DWORD bytesRead = 0;
    OVERLAPPED overlapped = { 0 };

    overlapped.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr); // <-- Create an event for the OVERLAPPED structure

    while (true) { // <-- Changed the loop condition to 'true'
        if (ReadDirectoryChangesW(
            watcher->directoryHandle_,
            buffer,
            bufferSize,
            TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE,
            &bytesRead,
            &overlapped,
            nullptr)) {

            DWORD offset = 0;
            FILE_NOTIFY_INFORMATION* fni = nullptr;

            do {
                fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer + offset);
                std::wstring fileName(fni->FileName, fni->FileNameLength / sizeof(wchar_t));

                std::wcout << L"Action: " << fni->Action << L", File: " << fileName << std::endl;

                offset += fni->NextEntryOffset;
            } while (fni->NextEntryOffset != 0);
        }
      

        HANDLE handles[] = { watcher->exitEvent_, overlapped.hEvent }; // <-- Add handles array with exitEvent_ and overlapped.hEvent
        DWORD waitResult = WaitForMultipleObjects(2, handles, FALSE, INFINITE); // <-- Wait for either exitEvent_ or overlapped.hEvent to be signaled

        if (waitResult == WAIT_OBJECT_0) {
            // exitEvent_ was signaled
            break; // <-- Exit the loop if exitEvent_ is signaled
        }
        else if (waitResult == WAIT_OBJECT_0 + 1) {
            // overlapped.hEvent was signaled, meaning ReadDirectoryChangesW has data
            ResetEvent(overlapped.hEvent); // <-- Reset the overlapped.hEvent for the next iteration
        }
    }

    CloseHandle(overlapped.hEvent); // <-- Close the event handle for the OVERLAPPED structure
    return 0;
}

void FileSystemWatcher::OnFileSystemChange(DWORD action, const std::wstring& fileName) {
    // Add your custom logic here, e.g.:
    // 1. Process the file change
    // 2. Perform some action based on the change
}