#pragma once

#include <wx/wx.h>
#include "LoggingTool.h"
#include "ThreadManager.h"
#include "FileSystemWatcher.h"
#include "ThreadManager.h"

class LoggingTool;

class AutoDeleteFiles : public wxApp 
{
public:
	virtual bool OnInit();

private:
	LoggingTool* logging_tool{};
	WatcherThread* m_WatcherThread;
	ThreadManager* m_ThreadManager;
};

