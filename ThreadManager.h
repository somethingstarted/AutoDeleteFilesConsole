#pragma once
#include <wx/base64.h>
#include <wx/thread.h>
#include <wx/utils.h>
#include <chrono>
#include "LoggingTool.h"
//#include "WatcherThread.h"
//#include "FileSystemWatcher.h"

class WatcherThread;
class FileSystemWatcher;

class ThreadManager : public wxThread
{
    bool ShouldBeWatchingThreads = true;

public:
    //__declspec(dllexport) ThreadManager(WatcherThread* threadToMonitor, FileSystemWatcher* fileSystemWatcher);
    ThreadManager(WatcherThread* threadToMonitor, FileSystemWatcher* fileSystemWatcher);
    virtual ~ThreadManager() {};
    

protected:
    virtual ExitCode Entry();
    
private:
    WatcherThread*      m_WatcherThread;
    FileSystemWatcher* m_fileSystemWatcher;
};

