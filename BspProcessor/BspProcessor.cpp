

#include "BspProcessor.h"
#include <string>
#include <fstream>
#include <functional>
#include <ios>

BspProcessor::BspProcessor() {

}

BspProcessor::BspProcessor(char* file_name) {
	this->load(file_name);

}

BspProcessor::load(char* file_name) {
	std::function<std::fstream& (std::fstream&, const char*, std::streamsize)> read_f = [](std::fstream& file, const char* s, std::streamsize n) {
		file.read(s, n);
		return file;
	};

	this->io(file_name, read_f);
}

BspProcessor::write(char* file_name) {
	std::function<std::fstream& (std::fstream&, const char*, std::streamsize)> write_f = [](std::fstream& file, const char* s, std::streamsize n) {
		file.write(s, n);
		return file;
	};

	this->io(file_name, write_f);
}

BspProcessor::io(char* file_name, std::function<std::fstream& (std::fstream&, const char*, std::streamsize)> rw_f) {
	std::fstream bsp_file(file_name, ios::binary);
	// Write or read the header of the bsp
	rw_f(bsp_file, &m_head, sizeof(bsp_header));
	std::cout << std::to_string(m_head.magic) << std::endl;
}

BspProcessor::~BspProcessor() {

}