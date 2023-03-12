#include "DirectoryIndexer.h"




std::vector<FileMetaData> FolderIndex{}; //eventually store this index into a file.
int64 SizeOfDisk;
int64 FreeSpace;
int64 AvaliableSpace;
FileMetaData metadata;




void CheckForDeletedFilesInVector()
{
	std::cout << "\n\n\tchecking for deleted files still in Index:\n";
	for (auto& metadata : FolderIndex)
	{
		if (fs::exists(metadata.FileName))
		{
			return;
		}
		else
		{
			std::cout << "***Does not exist:\t" << metadata.FileName.string() << "\n";
		}

	}
	std::cout << "\n";
	return;
}

void CalculateDailySpaceUsage()
{
	/*
	work through file list
	 - add first day and date to a temp buffer,
	 - do while the next entry is from the same day, add space size to the buffer,
	 - if/when the next entry is different:
			-pushback buffer 


	 - when it is done: pushback this vector to a global filesize usage tracking vector. eventually this vector will need to be stored in a file. 
			for now it'll only last while the program runs. 
	*/
	return;
}

int CreateListFromFiles(fs::path const& dir)
{
	std::cout << "\n\nBuild files in the directory... " << "\n"
		<< dir.string() << "\n";

	FileMetaData metadata = {};



	if (!fs::exists(dir) && !fs::is_directory(dir))		//make sure program is running in valid directory
	{
		std::cout << "failed at: CreateListFromFiles() > if (!fs::exists(dir) && !fs::is_directory(dir))";
		std::cin.get();
		exit(-1);
	}

	for (auto const& entry : fs::directory_iterator(dir))		//build entry for file's index
	{	
			//file order
		metadata.OriginalFileOrder;

			//file name
		metadata.FileName = entry.path().filename();

			//file extension
		metadata.FileExtension = entry.path().extension();

			//have permission to edit
		metadata.HasPermissionToEdit = static_cast<bool>(fs::status(entry).permissions());

			//flagged for deletion (decided later)
		metadata.IsFlaggedForNextDeletion = false;

			//file size
		unsigned long long int fileSize = static_cast<int>(fs::file_size(entry.path()) / 1024);
		metadata.FileSizeKB = fileSize;

			//date last modified
		metadata.TimeLastModified = fs::last_write_time(entry.path());

			//list files as they are found.
		/*std::cout << "#" << metadata.OriginalFileOrder << "\t" << metadata.FileName << metadata.FileExtension
			<< "\n\t"; */

				//ignore files with these qualities, no matter what -- swish cheese method. be careful
		std::vector<std::string> IgnoreList = { ".exe", ".pdb"};
				//only allow files with these in it. 
		std::vector<std::string> OkayList = { ".mp3", ".bvr"};

		if (std::any_of(IgnoreList.cbegin(), IgnoreList.cend(), [&metadata](std::string p) { return p == metadata.FileExtension; }))
		{
			std::cout << "** ignoring:\t" << metadata.FileName.string() << "\n";
		}

		else if (std::any_of(FolderIndex.cbegin(), FolderIndex.cend(), [&metadata](const FileMetaData& file) { return file.FileName == metadata.FileName; }))
		{
			std::cout << "** Skipping:\t" << metadata.FileName.string() << "\tFile's already in vector." << "\n";
		}
				 
				//only add ones of this extension  -- extra careful
		else if (std::any_of(OkayList.cbegin(), OkayList.cend(), [&metadata](std::string p) { return p == metadata.FileExtension; }))
		{
			std::cout << "adding: " << metadata.FileName.string() << "\n";
			FolderIndex.push_back(metadata);
		}
		

			//don't let metadata go above 1,000.
		if (metadata.OriginalFileOrder < 1000)
		{
			metadata.OriginalFileOrder++;
		}
		else 
		{
			metadata.OriginalFileOrder = 0;
		}

	}
	
	return 0;
}



std::string IntergerWithCommas(int64 vv)		//move to seperate class named "formatting" someday. 
{
	std::string s = std::to_string(vv);

	int64 n = s.length() - 3;
	int64 end = (vv >= 0) ? 0 : 1; // Support for negative numbers
	while (n > end) {
		s.insert(n, ",");
		n -= 3;
	}

	return s;
}

int64 GetPercentage(int64 a, int64 b, bool IsVerbose) //stupid. doesn't work.
{
	
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

int8  CheckHDDSizeAndSpace(fs::path, bool IsVerbose)
{

	std::error_code ec;
	auto MyPath = fs::current_path();
	std::string s = "";

	const std::filesystem::space_info si = std::filesystem::space(MyPath, ec);

	fs::space_info MyDiskSpace = std::filesystem::space(MyPath, ec);
	 SizeOfDisk = static_cast<int64>(MyDiskSpace.capacity);
	 FreeSpace = static_cast<int64>(MyDiskSpace.free);
	 AvaliableSpace = static_cast<int64>(MyDiskSpace.available);

	 double HDDPercentageLeft = GetPercentage(FreeSpace, SizeOfDisk, IsVerbose); //stupid. doesn't work

	 //uint64 HDDPercentageLeft = ((FreeSpace * 100) / (FreeSpace + SizeOfDisk)); //unreliable. different every time

	 if (IsVerbose == 1)
	 {
		 const int ArrayLength = 3;
		 std::string DiskSizeNumbers[ArrayLength]
		 {
			 IntergerWithCommas(SizeOfDisk),
			 IntergerWithCommas(FreeSpace),
			 IntergerWithCommas(AvaliableSpace),
		 };
	 
		 int16 width = IntergerWithCommas(SizeOfDisk).size();

		 std::stringstream OSS1;
		 std::stringstream OSSa;
		 std::stringstream OSS2;
		 std::stringstream OSSb;
		 std::stringstream OSS3;
		 std::stringstream OSSc;
		 std::stringstream OSS4;
		 std::stringstream OSSd;

		 OSS1 << "Size Of Disk\t\t";
		 OSSa << std::right << std::setw(width) << IntergerWithCommas(SizeOfDisk) << "\n";
		 OSS2 << "Free Space\t\t";
		 OSSb << std::right << std::setw(width) << IntergerWithCommas(FreeSpace) << "\n";
		 OSS3 << "Avaliable Space:\t";
		 OSSc << std::right << std::setw(width) << IntergerWithCommas(AvaliableSpace) << "\n";
		 OSS4 << "Disk Percent Left:\t";
		 OSSd << std::right << std::setw(width) << HDDPercentageLeft << "%" << "\n";

		 s = OSS1.str() + OSSa.str() + OSS2.str() + OSSb.str() + OSS3.str() + OSSc.str() + OSS4.str() + OSSd.str();

		 std::cout << s;

		 std::this_thread::sleep_for(std::chrono::seconds(4));
	 }





	 return HDDPercentageLeft;
}


std::tuple <int16, int16, int16>  GetLengthOf()
{					
	//initialize things used below
	int16 sizeAA = 0;
	int16 sizeBB = 0;
	int16 sizeCC = 0;
	int16 sizeAAA = 0;
	int16 sizeBBB = 0;
	int16 sizeCCC = 0;

	//formatting, get length of...
	for (auto& metadata : FolderIndex)
	{
		std::string strA = std::to_string(metadata.OriginalFileOrder);
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


std::stringstream GetFileAge(chrono_ftt TimeLastModified2) //time last modified 
{
		auto diff = std::chrono::file_clock::now() - TimeLastModified2;

		auto diffDaysOld = std::chrono::round<std::chrono::days>(diff);
		//auto diffMonthsOld = std::chrono::round<std::chrono::months>(diff);
		//auto diffYearsOld = std::chrono::round<std::chrono::years>(diff);
		std::stringstream ss_buffer;	

			std::cout << diffDaysOld << " old";

	return ss_buffer;
}

int64 ListFolderIndex(bool DisplayFileAge, bool DisplayFileSize, bool DisplayLastModifiedTime, bool DisplayOriginalFileOrder) //replace with bitwise please please
{
	GetLengthOf();


	int16 sizeAAAA = std::get<0>(GetLengthOf());
	int16 sizeBBBB = std::get<1>(GetLengthOf());
	int16 sizeCCCC  = std::get<2>(GetLengthOf());




	for (auto& metadata : FolderIndex) 
	{

				// extract this someday so i can format files once and be done with it, then just feed all through this: 
		if (DisplayOriginalFileOrder == true)
		{
			std::cout << "#";
			std::cout << std::right << std::setw(sizeAAAA) << IntergerWithCommas(metadata.OriginalFileOrder);
		}

		std::cout << "  ";
		std::cout << std::right << std::setw(sizeBBBB) << metadata.FileName.string() << "  ";

		if (DisplayFileSize == true)
		{
			std::cout << "  ";
			std::cout << std::right << std::setw(sizeCCCC) << IntergerWithCommas(metadata.FileSizeKB) << " KB";
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
		std::cout << "\n";
	}
	return 0;
}




void SortListChronologically()
{
	std::cout << "Sorting chronologically: \n";

	std::sort(FolderIndex.begin(), FolderIndex.end(), [](const auto& struct1, const auto& struct2) //error 'class FileMetaData' has no member 'begin' or 'end'
	{
		return struct1.TimeLastModified < struct2.TimeLastModified;
	});




	return;
}

void SortListAlphabetically()
{
	std::cout << "Sorting alpabetically: \n";

	std::sort(FolderIndex.begin(), FolderIndex.end(), [](const auto& struct1, const auto& struct2) //error 'class FileMetaData' has no member 'begin' or 'end'
	{
		return struct1.FileName > struct2.FileName;
	});




	return;
}



int DirectoryIndexer()  //entry point 
{
	auto MyPath = fs::current_path();
	int8 DiskPercentageLeft = CheckHDDSizeAndSpace(MyPath, false);
	fs::path::auto_format(MyPath);




	CheckForDeletedFilesInVector();



	CreateListFromFiles(MyPath);
	SortListChronologically();

	ListFolderIndex(true, false, false, false);




	//average daily file size amount written, for files in database are:




	//HDD usage is / # days left expected before deletion required: 

	

	//the oldest file is: 



	return 0;
}