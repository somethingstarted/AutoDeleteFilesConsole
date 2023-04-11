#pragma once

#include <wx/wx.h>
#include <iostream>
#include <string>
#include <Windows.h>

#include "DirectoryIndexer.h"



class FileSystemWatcher {
public:
    FileSystemWatcher(const std::wstring& directory, DirIndexing& indexer);
    ~FileSystemWatcher();
    void StartMonitoring();
    void OnFileSystemChange(DWORD action, const std::wstring& fileName);

private:
    static DWORD WINAPI WatcherThread(LPVOID param);

    std::wstring directory_;
    HANDLE directoryHandle_;
    HANDLE threadHandle_;
    HANDLE exitEvent_;

    DirIndexing& indexer;
};


