#include "DirectoryIndexer.h"





//std::vector<FileMetaData> FolderIndexLegacy{}; //eventually store this index into a file.
Formatting formatting;

int64 SizeOfDisk; //add these to the class please
int64 FreeSpace;

int64 AvaliableSpace;
FileMetaData metadata;
std::vector<FileMetaData> DailyAverageUsage{}; //add to the class before finishing please for the love of god 

		//legacy purposes
//std::string formatting.IntergerWithCommas(int);
 

 
void DirIndexing::CheckForDeletedFilesInVector() //didn't really do anything i think. could be removed later. 
{
	logging_tool->AppendToLog("Checking for deleted files in vector.", OutputType::VERBOSE);

	//std::cout << "\n\n\tchecking for deleted files still in Index:\n";
	int i{};
	for (auto& metadata : DirIndexing::FolderIndex2)
	{
		logging_tool->AppendToLog("\t\tnext file", OutputType::VERBOSE);
		
		if (fs::exists(metadata.FileName))
		{
			std::stringstream ss;
			ss << "\t\t\t" << i << "already exsists. skipping file: " << metadata.FileName.string();
			logging_tool->AppendToLog(ss.str(), OutputType::_INFO);
			return;
		}
		else
		{
			std::stringstream ss;
			ss << "\t\t\t" << i << "Does not exist:\t" << metadata.FileName.string();
			logging_tool->AppendToLog(ss.str(), OutputType::_INFO);
			FolderIndex2.erase(FolderIndex2.begin() + i);
		}
		i++;
	}
	
	return;
}
 

auto DirIndexing::CalculateDailySpaceUsage() //unfinished
{
	logging_tool->AppendToLog("Calculating daily space usage.", OutputType::VERBOSE);
	int64 BufferSize = 0;
	chrono_ftt BufferDay1{};

	if (!DirIndexing::FolderIndex2.empty())
	{
		
		BufferDay1 = std::chrono::floor<std::chrono::days>(DirIndexing::FolderIndex2.at(0).TimeLastModified);
		logging_tool->AppendToLog("calculating daily space usage:\n", OutputType::VERBOSE);
		
	}
	else
		std::cerr << "Calculating daily usage failed for some reason.\n";

	int TotalFilesCounted = 1;
	int FilesSameDay = 0;

	for (auto& metadata : DirIndexing::FolderIndex2)
	{
		chrono_ftt BufferDay2 = std::chrono::floor<std::chrono::days>(metadata.TimeLastModified); //this file
				
		if (BufferDay1 != BufferDay2)
		{
			FilesSameDay = 0;
			BufferSize = metadata.FileSizeKB;
		}
		
		if (BufferDay1 == BufferDay2)
		{
			BufferSize += metadata.FileSizeKB;
			//FilesSameDay++;
		}


		if (BufferDay2 == BufferDay1)
		{
			
			FilesSameDay++;
		}

		BufferDay1 = BufferDay2; // 1 is last file for next time

			//resetting for next round
		TotalFilesCounted++;
	
				//days were not same, so...	
	}
	return;
}

bool HasWritePermissions(const fs::path& entry) {
	if (fs::exists(entry)) {
		fs::perms permissions = fs::status(entry).permissions();

		// Check if the file has write permissions for owner, group, or others
		bool owner_write = (permissions & fs::perms::owner_write) != fs::perms::none;
		bool group_write = (permissions & fs::perms::group_write) != fs::perms::none;
		bool others_write = (permissions & fs::perms::others_write) != fs::perms::none;

		return owner_write || group_write || others_write;
	}
	else {
		std::cerr << "File does not exist!" << std::endl;
		return false;
	}
}


int DirIndexing::CreateListFromFiles(fs::path const& dir) //this is the main version, which will stay
{


	FileMetaData metadata = {};


	
	if (!fs::exists(dir) && !fs::is_directory(dir))		//make sure program is running in valid directory
	{

		std::stringstream sss;
		sss << "failed at: CreateListFromFiles()";
		logging_tool->AppendToLog(sss.str(), OutputType::VERBOSE);
		//exit(-1);
	}


	for (auto const& entry : fs::directory_iterator(dir))		//build entry for file's index
	{	
			//file ID number set after the fact, because it checks file types first before committing to vector.
		metadata.FileIDnumber;

			//file name
		metadata.FileName = entry.path().filename();

			//file extension
		metadata.FileExtension = entry.path().extension();

			//have permission to edit
		//std::cerr << "before: " << std::bitset<16>(metadata.FileChoicesFlags) ;
		if (HasWritePermissions(entry) == true)	
			metadata.FileChoicesFlags = metadata.FileChoicesFlags | FileFlags::ff_CanBeWrittenTo;
		else
			metadata.FileChoicesFlags = metadata.FileChoicesFlags & ~FileFlags::ff_CanBeWrittenTo;


				//these shouldn't matter, since it's not supposed to re-add new files to the directory. 
				//however, if it's found that it's re-adding files, it needs to be fixed, because it can't 
				// work if it's constantly resetting files every few hours when it runs.
				// 
				//set as NOT ready to be deleted. that's for a different function. this one only adds to list. 
		metadata.FileChoicesFlags = metadata.FileChoicesFlags & ~FileFlags::ff_IsReadyToBeDeleted;
				//set ff_ShouldBeArchived as "no / 0" for now. different function chooses that one.
		metadata.FileChoicesFlags = metadata.FileChoicesFlags & ~FileFlags::ff_ShouldBeArchived;


	

			//file size
		uint64 FileSize = static_cast<int>(fs::file_size(entry.path()) / 1024);
		metadata.FileSizeKB = FileSize;

			//date last modified
		metadata.TimeLastModified = fs::last_write_time(entry.path());


									//ignore the ignore list for now
									// 
				//ignore files with these qualities, no matter what -- swish cheese method. be careful
		std::vector<std::string> IgnoreList = { ".exe", ".pdb"};
				//only allow files with these in it. 
		std::vector<std::string> OkayList = { ".mp3", ".bvr"};

		if (std::any_of(IgnoreList.cbegin(), IgnoreList.cend(), [&metadata](std::string p) { return p == metadata.FileExtension.string(); }))
		{
			std::stringstream sss;
			sss << "\t\t** ignoring:\t" << metadata.FileName.string();
			logging_tool->AppendToLog(sss.str(), OutputType::VERBOSE);
		}

		else if (std::any_of(DirIndexing::FolderIndex2.cbegin(), DirIndexing::FolderIndex2.cend(), [&metadata](const FileMetaData& file) { return file.FileName == metadata.FileName; }))
		{
			
			std::stringstream sss;
			sss << "\t\t** Skipping:\t" << metadata.FileName.string() << "\tFile's already in vector." ;
			logging_tool->AppendToLog(sss.str(), OutputType::VERBOSE);
		}
				 
				//only add ones of this extension  -- extra careful
		else 
		if (std::any_of(OkayList.cbegin(), OkayList.cend(), [&metadata](std::string p) { return p == metadata.FileExtension.string(); }))
		{
		 
			DirIndexing::FolderIndex2.push_back(metadata);
			std::stringstream sss;
			sss << "\t\tadding: " << metadata.FileIDnumber << "\t" << metadata.FileName.string() ;
			logging_tool->AppendToLog(sss.str(), OutputType::VERBOSE);
		
											//
											//end of ignore list 
		

			//don't let metadata go above 99'999.
			//only count up if it adds the file to the vector
			if (metadata.FileIDnumber < 99'999)
			{
				metadata.FileIDnumber++;

			}
			else if (metadata.FileIDnumber > 99'999 || metadata.FileIDnumber <= 0)
			{
				metadata.FileIDnumber = 1;
			}

		}

			//reset bits back to zero?
		metadata.FileChoicesFlags = metadata.FileChoicesFlags & ~FileFlags::ff_CanBeWrittenTo;

	}


	return 0;
}



int64 DirIndexing::GetPercentageOf(int64 a, int64 b, bool IsVerbose) //should be moved to formatting.cpp someday
{
	//70 out of 100 returns as 70, not 30. 
	double Aa = a;
	double Bb = b;

	while (Aa >= 100 && Bb >= 100)
	{
		Aa = Aa / static_cast<double>(100);
		Bb = Bb / static_cast<double>(100);

		if (IsVerbose == true)
		{
			std::cout << Aa << " / " << Bb << "\n";
		}
		
	}

	int64 result = 100 - ((Aa * 100) / Bb);


	if (IsVerbose == true)
	{
		std::cout << "result: " << result << "\n";
	}

	return result;
}

std::string  DirIndexing::CheckHDDSizeAndSpace(fs::path, bool IsVerbose) //new UI version
{
	std::error_code ec;
	auto MyPath = fs::current_path();
	std::string s{};

	const std::filesystem::space_info si = std::filesystem::space(MyPath, ec);

	fs::space_info MyDiskSpace = std::filesystem::space(MyPath, ec);
	 SizeOfDisk = static_cast<int64>(MyDiskSpace.capacity);
	 FreeSpace = static_cast<int64>(MyDiskSpace.free);
	 AvaliableSpace = static_cast<int64>(MyDiskSpace.available);

	 double HDDPercentageLeft = GetPercentageOf(FreeSpace, SizeOfDisk, IsVerbose); //stupid. doesn't work

	 //uint64 HDDPercentageLeft = ((FreeSpace * 100) / (FreeSpace + SizeOfDisk)); //unreliable. different every time

	 if (IsVerbose == 1)
	 {
		 const int ArrayLength = 3;
		 std::string DiskSizeNumbers[ArrayLength]
		 {
			 formatting.IntergerWithCommas(SizeOfDisk),
			 formatting.IntergerWithCommas(FreeSpace),
			 formatting.IntergerWithCommas(AvaliableSpace),
		 };
	 
		 int16 width = formatting.IntergerWithCommas(SizeOfDisk).size();

		 std::stringstream OSS1;
		 std::stringstream OSSa;
		 std::stringstream OSS2;
		 std::stringstream OSSb;
		 std::stringstream OSS3;
		 std::stringstream OSSc;
		 std::stringstream OSS4;
		 std::stringstream OSSd;

				//add more choices later.
				//default is KB, so you're dividing against KB.
		 const int OneGB = 1'048'576;
		 const int DivideBy = OneGB;
		 std::string UnitsOfMeasure{};
				//just add the ones you want here. maybe make it an enum or something later idk.
		 if (DivideBy == OneGB)
			 UnitsOfMeasure = " GB";
		 else
			 UnitsOfMeasure = " KB";


		 OSS1 << "Size Of Disk\t";
		 OSSa << std::right << std::setw(width) << formatting.IntergerWithCommas((SizeOfDisk / DivideBy)) << UnitsOfMeasure << "\n";
		 /*OSS2 << "Free Space\t\t";
		 OSSb << std::right << std::setw(width) << formatting.IntergerWithCommas(FreeSpace) << "\n";*/
		 OSS3 << "Avaliable Space:\t";
		 OSSc << std::right << std::setw(width) << formatting.IntergerWithCommas((AvaliableSpace / DivideBy)) << UnitsOfMeasure << "\n";
		 OSS4 << "Disk Percent used:\t";
		 OSSd << std::right << std::setw(width) << HDDPercentageLeft << "%" << "\n";

		 s = OSS1.str() + OSSa.str() + /*OSS2.str() + OSSb.str() +*/ OSS3.str() + OSSc.str() + OSS4.str() + OSSd.str();
		 logging_tool->AppendToLog(s, OutputType::VERBOSE);
		 //std::cout << s;

		 //std::this_thread::sleep_for(std::chrono::seconds(4));
	 }
	 return s;
}

		//legacy CONSOLE version. idk which veriosn gets called in WX tho
int8  DirIndexing::CheckHDDSizeAndSpaceConsole(fs::path, bool IsVerbose) // legacy
{
	std::error_code ec;
	auto MyPath = fs::current_path();
	std::string s = "";

	const std::filesystem::space_info si = std::filesystem::space(MyPath, ec);

	fs::space_info MyDiskSpace = std::filesystem::space(MyPath, ec);
	SizeOfDisk = static_cast<int64>(MyDiskSpace.capacity);
	FreeSpace = static_cast<int64>(MyDiskSpace.free);
	AvaliableSpace = static_cast<int64>(MyDiskSpace.available);

	double HDDPercentageLeft = GetPercentageOf(FreeSpace, SizeOfDisk, IsVerbose); //stupid. doesn't work

	//uint64 HDDPercentageLeft = ((FreeSpace * 100) / (FreeSpace + SizeOfDisk)); //unreliable. different every time

	if (IsVerbose == 1)
	{
		const int ArrayLength = 3;
		std::string DiskSizeNumbers[ArrayLength]
		{
			formatting.IntergerWithCommas(SizeOfDisk),
			formatting.IntergerWithCommas(FreeSpace),
			formatting.IntergerWithCommas(AvaliableSpace),
		};

		int16 width = formatting.IntergerWithCommas(SizeOfDisk).size();

		std::stringstream OSS1;
		std::stringstream OSSa;
		std::stringstream OSS2;
		std::stringstream OSSb;
		std::stringstream OSS3;
		std::stringstream OSSc;
		std::stringstream OSS4;
		std::stringstream OSSd;

		OSS1 << "Size Of Disk\t\t";
		OSSa << std::right << std::setw(width) << formatting.IntergerWithCommas(SizeOfDisk) << "\n";
		OSS2 << "Free Space\t\t";
		OSSb << std::right << std::setw(width) << formatting.IntergerWithCommas(FreeSpace) << "\n";
		OSS3 << "Avaliable Space:\t";
		OSSc << std::right << std::setw(width) << formatting.IntergerWithCommas(AvaliableSpace) << "\n";
		OSS4 << "Disk Percent Left:\t";
		OSSd << std::right << std::setw(width) << HDDPercentageLeft << "%" << "\n";

		s = OSS1.str() + OSSa.str() + OSS2.str() + OSSb.str() + OSS3.str() + OSSc.str() + OSS4.str() + OSSd.str();

		std::cout << s;

		std::this_thread::sleep_for(std::chrono::seconds(4));
	}
	return HDDPercentageLeft;
}

std::tuple <int16, int16, int16>  DirIndexing::GetLengthOf() //some day: this should in the future take in an array/vector, compare ALL parts of array/vector, then return a single number. this looks like shit. 
{					
	//initialize things used below
	int16 sizeAA = 0;
	int16 sizeBB = 0;
	int16 sizeCC = 0;
	int16 sizeAAA = 0;
	int16 sizeBBB = 0;
	int16 sizeCCC = 0;

	//formatting, get length of...
	for (auto& metadata : DirIndexing::FolderIndex2)
	{
		std::string strA = std::to_string(metadata.FileIDnumber);
		std::string strB = metadata.FileName.string();
		std::string strC = std::to_string(metadata.FileSizeKB);

		//get num of letters of current file 
		int16 sizeA = strA.length();
		int16 sizeB = strB.length();
		int16 sizeC = strC.length();

		//if larger than ... replace with ^ 

		if (sizeA > sizeAA)
		{
			sizeAAA = sizeA;
		}
		else { sizeAAA = sizeAA; }
		
		if (sizeB > sizeBB)
		{
			sizeBBB = sizeB;
		}
		else { sizeBBB = sizeBB; }

		if (sizeC > sizeCC)
		{
			sizeCCC = sizeC;
		}
		else { sizeCCC = sizeCC; }

	}

	return std::make_tuple(sizeAAA, sizeBBB, sizeCCC);
}

		//can be moved to formatting someday? it doesn't create it's own data for the vector, just converts it. IE formatting
std::stringstream DirIndexing::GetFileAge(chrono_ftt TimeLastModified2) //time last modified 
{
		auto diff = std::chrono::file_clock::now() - TimeLastModified2;

		auto diffDaysOld = std::chrono::round<std::chrono::days>(diff);
		//auto diffMonthsOld = std::chrono::round<std::chrono::months>(diff);
		//auto diffYearsOld = std::chrono::round<std::chrono::years>(diff);
		std::stringstream ss_buffer;	

			std::cout << diffDaysOld << " old";

	return ss_buffer;
}

int64 DirIndexing::ListFolderIndexConsole(bool DisplayFileAge, bool DisplayFileSize, bool DisplayLastModifiedTime, bool DisplayOriginalFileOrder) //replace with bitwise please please
{
	GetLengthOf();

	int16 sizeAAAA = std::get<0>(GetLengthOf());
	int16 sizeBBBB = std::get<1>(GetLengthOf());
	int16 sizeCCCC  = std::get<2>(GetLengthOf());

	for (auto& metadata : DirIndexing::FolderIndex2)
	{

				// extract this someday so i can format files once and be done with it, then just feed all through this: 
		if (DisplayOriginalFileOrder == true)
		{
			std::cout << "#";
			std::cout << std::right << std::setw(sizeAAAA) << formatting.IntergerWithCommas(metadata.FileIDnumber);
		}

		std::cout << "  ";
		std::cout << std::right << std::setw(sizeBBBB) << metadata.FileName.string() << "  ";

		if (DisplayFileSize == true)
		{
			std::cout << "  ";
			std::cout << std::right << std::setw(sizeCCCC) << formatting.IntergerWithCommas(metadata.FileSizeKB) << " KB";
		}
		if (DisplayLastModifiedTime == true)
		{
			std::cout << "    " << std::format("{:%a %F %r }", metadata.TimeLastModified);
		}
		 
		if (DisplayFileAge == true)
		{
			auto FileAgeDays2 = GetFileAge(metadata.TimeLastModified);
			std::cout << "\t\t" << FileAgeDays2.str();
		}
		
	}
	return 0;
}



		//might still need
void DirIndexing::SortListChronologically()
{
	std::cout << "Sorting chronologically: \n";

	std::sort(DirIndexing::FolderIndex2.begin(), DirIndexing::FolderIndex2.end(), [](const auto& struct1, const auto& struct2) //error 'class FileMetaData' has no member 'begin' or 'end'
	{
		return struct1.TimeLastModified > struct2.TimeLastModified;
	});




	return;
} //dont need anymore.
		//dont need
void DirIndexing::SortListAlphabetically()
{
	std::cout << "Sorting alpabetically: \n";

	std::sort(DirIndexing::FolderIndex2.begin(), DirIndexing::FolderIndex2.end(), [](const auto& struct1, const auto& struct2) //error 'class FileMetaData' has no member 'begin' or 'end'
	{
		return struct1.FileName > struct2.FileName;
	});




	return;
}	  //dont need anymore?


void DirIndexing::ResetVectorCounter(bool YesResetCounter = true)
{

	DirIndexing::CountChangesSinceGridRefresh = 0;
	return;
}

void DirIndexing::AddToRefreshCounter(uint16 ChangesToAddToRefreshCounter = {})
{

		DirIndexing::CountChangesSinceGridRefresh += ChangesToAddToRefreshCounter;
		


	return;
}


uint64 DirIndexing::DirectoryIndexBuilderUpdater()  //keep. BUILD and REBUILD and UPDATE the index of files here in background to make rest of program work.
{
	//std::stringstream Foobar{};
	//Foobar << "DirectoryIndexBuilderUpdater sucessfully ran.";
	//wxMessageBox(Foobar.str());

	auto MyPath = fs::current_path();
	int8 DiskPercentageLeft = CheckHDDSizeAndSpaceConsole(MyPath, false);
	fs::path::auto_format(MyPath);


	CheckForDeletedFilesInVector();

	CreateListFromFiles(MyPath);

	//SortListChronologically();

	//ListFolderIndexConsole(true, false, false, false);

	// CalculateDailySpaceUsage(); // unfinished


	//average daily file size amount written, for files in database are:




	//HDD usage is / # days left expected before deletion required: 

	

	//the oldest file is: 



	return 0;
}