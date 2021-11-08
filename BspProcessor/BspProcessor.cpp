

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
	//file_op(file_stream, (char*) m_head, sizeof(bsp_header));
	//file_op(file_stream, buff, 10);
	file_op(file_stream, reinterpret_cast < char * > ( &m_head ), sizeof(bsp_header));
	std::cout << "Magic Number: " << m_head.magic << std::endl;
	std::cout << "Version Number: " << m_head.version << std::endl;
	std::cout << "Revision Number: " << m_head.map_revision << std::endl;
	std::cout << "Lumps: {" << std::endl;
	for (int iter = 0; iter < VHEADERLUMPS; iter++)
		std::cout << "\t" << BSP_FILE::lump_names.at(static_cast <BSP_FILE::LUMP> (iter))
			<< ": offset = " << m_head.lumps[iter].offset
			<< ", length = " << m_head.lumps[iter].length
			<< ", versn  = " << m_head.lumps[iter].version
			<< ", versn  = " << m_head.lumps[iter].lmpID
			<< std::endl;
	std::cout << "}" << std::endl;
	//std::cout << std::to_string(m_head.magic) << std::endl;
	//std::cout << VBSPHEADER << std::endl;
	//std::cout << VBSPHEADERBIG << std::endl;
}

void BspProcessor::read_from_file(std::string file_path) {
	//std::function<std::fstream& (std::fstream&, const char*, std::streamsize)> read_f = [](std::fstream& file, const char* s, std::streamsize n) {
	
	std::function < void(std::fstream&, char *, std::streamsize) > read_f = [](std::fstream& file, char* dest, std::streamsize dest_sz) {
		file.read ( dest, dest_sz);
	};
	

	std::fstream fs;
	fs.open(file_path, std::fstream::in); //| std::fstream::binary);
	if (fs.is_open()) {
		std::cout << "File opened: " << file_path << std::endl;
		this->io(fs, read_f);
	}
	else
		std::cout << "File " << file_path << " could not be opened." << std::endl;
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