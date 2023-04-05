#include "AutoFileDeleter.h"
#include "FileDeleterFrame.h"

bool AutoDeleteFiles::OnInit() {
	// Instantiate the DirIndexing class
	//DirIndexing indexer;
	Formatting formatting;
	DirIndexing indexer(formatting);
	
	MyFrame* frame = new MyFrame("Auto File Deleter", indexer);

	
	 

	frame->Show(true);


	
	 

	

	return true;
}



wxIMPLEMENT_APP(AutoDeleteFiles); //only do once. 