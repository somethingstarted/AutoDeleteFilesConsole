#pragma once

#include <wx/wx.h>
#include "LoggingTool.h"
//#include "ThreadManager.h"
#include "FileSystemWatcher.h"
//#include "ThreadManager.h"
#include "WatcherThread.h"

class LoggingTool;
//class WatcherThread;

class AutoDeleteFiles : public wxApp 
{
public:
	virtual bool OnInit();

private:
	LoggingTool* logging_tool{};
	WatcherThread* watcher_Thread{};
	//ThreadManager* m_ThreadManager;
};

