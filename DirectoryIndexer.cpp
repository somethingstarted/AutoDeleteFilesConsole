#include "DirectoryIndexer.h"


typedef   std::int64_t int64;
typedef   std::int32_t int32;
typedef   std::int16_t int16;
typedef   std::int8_t int8;

std::vector<FileMetaData> FolderIndex{};
int64 SizeOfDisk;
int64 FreeSpace;
int64 AvaliableSpace;
FileMetaData metadata;


//It is considered good C++ style to avoid short, int, long etc.and instead use :
//
//std::int8_t   # exactly  8 bits
//std::int16_t  # exactly 16 bits
//std::int32_t  # exactly 32 bits
//std::int64_t  # exactly 64 bits
//
//std::size_t   # can hold all possible object sizes, used for indexing

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
	return;
}

int CreateListFromFiles(fs::path const& dir)
{

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
	
}



//int64 find_greatest(std::string a, std::string b, std::string c)
//{
//	std::string x = max(a, max(b, c));
//	std::cout << x << " is greatest";
//
//	std::cout << "\n\n";
//
//	return x.size();
//}

std::string IntergerWithCommas(int64 vv)
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
	 int8 HDDPercentageLeft = ((FreeSpace / SizeOfDisk) * 100);

	 if (IsVerbose == true)
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


		 OSS1 << "Size Of Disk\t\t";
		 OSSa << std::right << std::setw(width) << IntergerWithCommas(SizeOfDisk) << "\n";
		 OSS2 << "Free Space\t\t";
		 OSSb << std::right << std::setw(width) << IntergerWithCommas(FreeSpace) << "\n";
		 OSS3 << "Avaliable Space:\t";
		 OSSc << std::right << std::setw(width) << IntergerWithCommas(AvaliableSpace) << "\n";
		 OSS3 << "Disk Percent Left:\t";
		 OSSc << std::right << std::setw(width) << IntergerWithCommas(AvaliableSpace) << "\n";

		 s = OSS1.str() + OSSa.str() + OSS2.str() + OSSb.str() + OSS3.str() + OSSc.str();
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


signed long long GetFileAge(chrono_ftt TimeLastModified2) //time last modified //increase int bit count when time numbers fuck up
{


	// Convert the FileLastModifiedTime to a duration since the epoch
	auto duration_since_epoch = std::chrono::duration_cast<std::chrono::system_clock::duration>(TimeLastModified2.time_since_epoch());

	// Construct a time_point object from the duration since the epoch
	auto LastModified = std::chrono::time_point<std::chrono::system_clock>(duration_since_epoch);

	//auto LastModified = std::chrono::time_point<std::chrono::system_clock>(metadata.TimeLastModified);
	auto TimeRightNow = std::chrono::system_clock::now();
	auto FileAgeRaw = (TimeRightNow - LastModified);

	/*auto FileAgeDaysChrono = std::chrono::duration_cast<std::chrono::days>(FileAgeRaw);*/

	auto FileAgeDaysChrono = std::chrono::time_point_cast<std::chrono::days>(TimeRightNow) - std::chrono::time_point_cast<std::chrono::days>(LastModified);
	signed long long FileAgeDays = FileAgeDaysChrono.count();


	FileAgeDays = FileAgeDaysChrono.count();


	return FileAgeDays;
}

int64 ListFolderIndex(bool DisplayFileAge)		 
{
	GetLengthOf();


	int16 sizeAAAA = std::get<0>(GetLengthOf());
	int16 sizeBBBB = std::get<1>(GetLengthOf());
	int16 sizeCCCC  = std::get<2>(GetLengthOf());




	for (auto& metadata : FolderIndex) 
	{


		std::cout << "#";
		std::cout << std::right << std::setw(sizeAAAA) << metadata.OriginalFileOrder;
		std::cout << "  ";
		std::cout << std::right << std::setw(sizeBBBB) << metadata.FileName.string();
		std::cout << "  ";
		std::cout << std::right << std::setw(sizeCCCC) << metadata.FileSizeKB;
		std::cout << "    " << std::format("{:%a %F %r }", metadata.TimeLastModified);
		 
		if (DisplayFileAge == true)
		{
			int64 FileAgeDays2 = GetFileAge(metadata.TimeLastModified);
			std::cout << "   File Age: " << FileAgeDays2;
		}
		std::cout << "\n";
	}
	return 0;
}




void SortListChronologically()
{


	std::sort(FolderIndex.begin(), FolderIndex.end(), [](const auto& struct1, const auto& struct2) //error 'class FileMetaData' has no member 'begin' or 'end'
	{
		return struct1.TimeLastModified < struct2.TimeLastModified;
	});


	std::cout << "Sorted chronologically: \n";

	return;
}

void SortListAlphabetically()
{


	std::sort(FolderIndex.begin(), FolderIndex.end(), [](const auto& struct1, const auto& struct2) //error 'class FileMetaData' has no member 'begin' or 'end'
	{
		return struct1.FileName > struct2.FileName;
	});


	std::cout << "Sorted chronologically: \n";

	return;
}



int DirectoryIndexer()  //entry point 
{
	auto MyPath = fs::current_path();
	int8 DiskPercentageLeft = CheckHDDSizeAndSpace(MyPath, true);
	fs::path::auto_format(MyPath);

	std::cout << "\n\nBuild files in the directory... " << "\n"
		<< MyPath.string() << "\n";

	CreateListFromFiles(MyPath);


	std::cout << "\n\nlist files in the vector: " << "\n"
		<< MyPath << "\n";
	//std::cin.get();
	//ListFolderIndex();


	std::cout << "\nsort by time: " << "\n";
	/*std::cin.get();*/
	SortListChronologically();
	std::cout << "\toldest:\n";
	ListFolderIndex(true);
	std::cout << "\tnewest ^ \n";


	std::cout << "\n\t CHANGE SOMETHING IN THE FOLDER! **** \n\n";

	std::cin.get();

	CheckForDeletedFilesInVector();

	CreateListFromFiles(MyPath);
	ListFolderIndex(true);

	return 0;
}