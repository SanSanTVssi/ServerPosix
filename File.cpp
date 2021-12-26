#include "File.h"

FileGuard::FileGuard(const std::wstring& filename) : file(new std::wfstream) {
	// Need for processing exceptions connected with files.
	file->exceptions(std::ifstream::badbit | std::ifstream::failbit);
	// Try open file. If file is not exist or just or not available
	// Throw exception (without processing, because then the program has nothing to work with)
	try {
		file->open(filename);
	}
	catch (const std::exception& fileException) {
		std::string failtureMessage;
		// Used to convert std :: wstring to std :: string because exception cannot contain std :: wstring
		// Not ASCII symbols will be lost, but it's still better than nothing. 
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		std::string str_filename = converter.to_bytes(filename);
		failtureMessage += "Exception while opening file! [FileGuard::FileGuard] Filename: " + str_filename + " \nError type:\t";
		failtureMessage += fileException.what();
		throw std::wfstream::failure(failtureMessage);
	}
}

FileGuard::~FileGuard() {
	file->close();
	delete file;
}

std::wstring File::ReadString() {
	// Throw exception if the file has already ended.
	if (eof()) {
		throw std::runtime_error("Error: Attempting to read data after the end of the file!\n "
			"Perhaps the program does not correctly define the file limits ! [File::ReadString]");
	}
	std::wstring temp;
	// Throw exception if the file is empty or size of sting bigger than maximum number of elements the string is able to hold.
	try {
		std::getline(*file.operator->(), temp);
	}
	catch (const std::exception& exception) {
		std::string failtureMessage = "Error: while file read! [File::ReadString]. Exception code: ";
		failtureMessage += exception.what();
		throw std::runtime_error(failtureMessage);
	}
	return temp;
}

void File::WriteString(const std::wstring& string) {
	try {
		*file.operator->() << string << std::endl;
	}
	catch (const std::exception& exception) {
		std::string failtureMessage = "Error: while write write! [File::WriteString]. Exception code: ";
		failtureMessage += exception.what();
		throw std::runtime_error(failtureMessage);
	}
}
