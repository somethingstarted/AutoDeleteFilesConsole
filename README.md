# AutoDeleteFilesConsole
For security cam folders. it is supposed to automatically delete the least amount of files possible to keep the cameras recording, while maximizing file retention time. 
    a. i dont like how some programs manage files, especially the ones that generate loads of data without any proper cleanup. 
    b. i want the footage to last as long as it can, and every time the number of cameras or the types of cameras change, so does the math on how to maximize space usage on the hdd. 
    c. the goal of this program, once it gets to a functional state, is to do all that for you. 
        1. calculate your hdd space avaliable
	2. calculate how much space your cameras are using every (period of time) 
	3. empty just enough space (plus some extra) for your cameras to keep recording
	4. keep a historical log of how much space is being filled on the hdd, and in the folder(s) being saved. 
	5. keep track of folders and either allow or disallow deletions from certain folders. 
	6. predict future usage and adjust how much it deletes accordingly. the file(s) it removes at any given point in time are going to be the OLDEST. 
	7. give all of this information in a well designed, easy to understand UI windows function. 
    d. eventually be multi-platform. it's currently win only, but it'd be nice if it worked on other platforms. 

please help me. i think this is a good idea. it could be useful in other circumstances- like plex libraries, or servers with lots of data, or scentific stuff. there's uses out there for something like this. 


currently inoperable. the latest commit doesn't compile lol. 

frameworks: 
  wxWidgets
  c++ 2020

I'm in the middle of converting it from wxthread to jthread, because i like jthread better and since the project is young, now is the best time to redo that. but, in the process of switching from wxthread to jthread, i broke it. i broke it so bad. 



help is definately welcome here. 

just use your own forks- and use pull requests. don't push directly to this project. it shouldn't let you, but just in case. 



