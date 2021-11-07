

#include "BspProcessor.h"
#include <iostream>
#include <string>
#include <fstream>
#include <functional>
//#include <ios>

BspProcessor::BspProcessor() {
	buff = 'a';
}

BspProcessor::BspProcessor(std::string file_name) {
	this->read_from_file(file_name);

}

void BspProcessor::io ( std::fstream & file_stream, std::function < void ( std::fstream&, char *, std::streamsize ) > file_op ) {
	// Write or read the header of the bsp
	//file_op(file_stream, (char*)m_head, sizeof(bsp_header));
	//file_op(file_stream, buff, 10);
	//file_op(file_stream, reinterpret_cast < char * > ( &m_head ), sizeof(bsp_header));
	std::cout << buff << std::endl;
	//std::cout << std::to_string(m_head.magic) << std::endl;
	//std::cout << VBSPHEADER << std::endl;
	//std::cout << VBSPHEADERBIG << std::endl;
}

void BspProcessor::read_from_file(std::string file_path) {
	//std::function<std::fstream& (std::fstream&, const char*, std::streamsize)> read_f = [](std::fstream& file, const char* s, std::streamsize n) {
	/*
	std::function < void(std::fstream&, char *, std::streamsize) > read_f = [](std::fstream& file, char* dest, std::streamsize dest_sz) {
		file.read ( dest, dest_sz);
	};
	*/

	std::fstream fs;
	fs.open(file_path, std::fstream::in); //| std::fstream::binary);
	if (fs.is_open()) {
		std::cout << "file is open" << std::endl;
	}
	else
		std::cout << "not open" << std::endl;
	fs.read(&buff, sizeof(char));
	std::cout << buff << std::endl;
	//this->io(fs, read_f);
}

void BspProcessor::write_to_file ( std::string file_path ) {
	std::function < void ( std::fstream&, char *, std::streamsize ) > write_f = [] ( std::fstream& file, char * dest, std::streamsize dest_sz ) {
		file.write ( dest, dest_sz );
	};

	std::fstream fs;
	fs.open(file_path, std::fstream::out); // | std::fstream::binary);

	this->io(fs, write_f);
}

BspProcessor::~BspProcessor() {
	//delete[] buff;
}