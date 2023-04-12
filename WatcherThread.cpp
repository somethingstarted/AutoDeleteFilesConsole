#include "WatcherThread.h"
#include "FileSystemWatcher.h"

WatcherThread::WatcherThread(FileSystemWatcher* fileSystemWatcher)
    : wxThread(wxTHREAD_JOINABLE), m_fileSystemWatcher(fileSystemWatcher)
{
}

wxThread::ExitCode WatcherThread::Entry()
{
    // Call the original WatcherThread function here
    m_fileSystemWatcher->WatcherThreadFunction();
    return (wxThread::ExitCode)0;
}

