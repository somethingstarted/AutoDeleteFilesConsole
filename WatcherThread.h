#pragma once
 
#include <wx/thread.h>

class FileSystemWatcher;

class WatcherThread : public wxThread
{
public:
    WatcherThread(FileSystemWatcher* fileSystemWatcher);
    virtual ~WatcherThread() {} // Optional: Add a virtual destructor if needed

protected:
    virtual ExitCode Entry();

private:
    FileSystemWatcher* m_fileSystemWatcher;
};

