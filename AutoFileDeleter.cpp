#include "AutoFileDeleter.h"
#include "FileDeleterFrame.h"

bool AutoDeleteFiles::OnInit() {
	MyFrame* frame = new MyFrame("Auto File Deleter");
	frame->Show(true);
	return true;
}



wxIMPLEMENT_APP(AutoDeleteFiles); //only do once. 