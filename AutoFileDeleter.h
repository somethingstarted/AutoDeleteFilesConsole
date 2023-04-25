#pragma once

#include <wx/wx.h>
#include "LoggingTool.h"

class LoggingTool;

class AutoDeleteFiles : public wxApp 
{
public:
	virtual bool OnInit();

private:
	LoggingTool* logging_tool{};
};

