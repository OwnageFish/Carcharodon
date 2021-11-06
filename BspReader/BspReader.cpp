

#include "BspReader.h"
#include <string>

BspReader::BspReader(char *file_name) {
	m_bsp_file.open(file_name, ios::binary | ios::in);
	m_bsp_file.read(&m_head, sizeof(bsp_header));
	std::cout << std::to_string(m_head.magic) << std::endl;

}

BspReader::~BspReader() {

}