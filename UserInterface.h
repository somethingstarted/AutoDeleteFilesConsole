#pragma once

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/frame.h>
#include "DirectoryIndexer.h"


class AutoDeleteFiles : public wxApp {
public:
	virtual bool OnInit();
};

class MyFrame : public wxFrame {
public:
	MyFrame(const wxString& title);

private:
	DirectoryIndexingClass indexer;
};

