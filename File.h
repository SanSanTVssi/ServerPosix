//
// Created by AAI2002 on 30.11.2021.
//

#ifndef HT1_FILE_H
#define HT1_FILE_H

#include <fstream>
#include <codecvt>
#include <utility>
#include <string>

class FileGuard {
private:
	std::wfstream* file;
public:
	FileGuard() = delete;

	explicit FileGuard(const std::wstring& filename);

	~FileGuard();

	std::wfstream* operator->() { return file; }
};

class File {
private:
	FileGuard file;
public:
	File() = delete;

	explicit File(const std::wstring& filename) : file(filename) {}

	/**
	* Method for read one (next) string from file
	* @return next line
	* Can throw exceptions if
	* 1) File is clear
	* 2) An attempt to read data from a file if the file has already ended
	*/
	std::wstring ReadString();

	/**
	* Method string into file (after inserts std::endl)
	* @param string for write into file
	*/
	void WriteString(const std::wstring& string);

	/**
	 * @return pointer for work with file stream
	 */
	[[maybe_unused]] std::wfstream* operator->() { return file.operator->(); }

	/**
	 * Checks end of file
	 * @return true if the end of file
	 */
	bool eof() { return file->eof(); }
};

#endif //HT1_FILE_H