#pragma once

#include <wx/wx.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <wx/thread.h>

#include "DirectoryIndexer.h"
#include "FileDeleterFrame.h"
#include "WatcherThread.h"

class MyProgramFrame;





class FileSystemWatcher {
public:
    FileSystemWatcher(const std::wstring& directory, DirIndexing& indexer);
    ~FileSystemWatcher();
    void StartMonitoring();
    void OnFileSystemChange(DWORD action, const std::wstring& fileName);
    

    //define "thread" from FileSystemWatcher.cpp
    WatcherThread* thread;
    static DWORD WINAPI StaticWatcherThread(LPVOID param = {});

private:
    //void WatcherThreadFunction();

    

    std::wstring directory_;
    HANDLE directoryHandle_;
    HANDLE threadHandle_;
    HANDLE exitEvent_;

    DirIndexing& indexer;
    MyProgramFrame& myprogramframe;

    WatcherThread* m_thread;
};


