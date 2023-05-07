#pragma once
 
#include <wx/thread.h>
#include <wx/wx.h>
#include "LoggingTool.h"
#include "FileSystemWatcher.h"

class FileSystemWatcher;

class WatcherThread : public wxThread
{
public:
    WatcherThread(FileSystemWatcher* fileSystemWatcher);
    virtual ~WatcherThread() {} 

protected:
    virtual ExitCode Entry();

private:
    FileSystemWatcher* m_fileSystemWatcher;
};

