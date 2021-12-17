

#include "BspProcessor.h"
#include <iostream>
#include <string>
#include <fstream>
#include <functional>

#include <cstdlib>


BspProcessor::BspProcessor() {

}

BspProcessor::BspProcessor(std::string file_name) {
	this->read_from_file(file_name);
}


void BspProcessor::io(std::fstream& file_stream, std::function < void(std::fstream&, char*, std::streamsize) > file_op) {
	file_op(file_stream, reinterpret_cast <char*> (&m_head), sizeof(bsp_header));

	std::cout << "Magic Number: " << m_head.magic << std::endl;
	std::cout << "Version Number: " << m_head.version << std::endl;
	std::cout << "Revision Number: " << m_head.map_revision << std::endl;
	std::cout << "Lumps: {" << std::endl;
	for (int iter = 0; iter < VHEADERLUMPS; iter++)
		if (m_head.lumps[iter].length != 0 )
			std::cout << "\t" << BSP_FILE::lump_names.at(static_cast <BSP_FILE::LUMP> (iter))
			<< ": offset = " << m_head.lumps[iter].offset
			<< ", length = " << m_head.lumps[iter].length
			<< std::endl;
	std::cout << "}" << std::endl;

	// All are needed to prepare the scene.
	struct_io < bsp_edge,		BSP_FILE::EDGES >			( file_stream, file_op, m_edges );
	struct_io < bsp_surfedge,	BSP_FILE::SURFEDGES >		( file_stream, file_op, m_surfedges );
	struct_io < point3f,		BSP_FILE::VERTEXES >		( file_stream, file_op, m_vertexes );
	struct_io < bsp_face,		BSP_FILE::FACES >			( file_stream, file_op, m_faces );
	//struct_io < bsp_face,		BSP_FILE::ORIGINALFACES >	( file_stream, file_op, m_faces );
	struct_io < bsp_texinfo,	BSP_FILE::TEXINFO >			( file_stream, file_op, m_texinfo);
}

//		*** NEVER MIND THIS CAN SEEK TO THE CORRECT SPOT.
//		Mby it should clear out 
// This function assumes the filepointer has already been seeked to the offset needed for reading/writing
//	Not sure if this should be moved into this function. I really don't feel like doing that.
template<typename bsp_struct_T, BSP_FILE::LUMP idx>
void BspProcessor::struct_io(std::fstream& file_stream, 
							 std::function < void(std::fstream&, char*, std::streamsize) > file_op, 
							 std::vector < bsp_struct_T > &bsp_vec) {
	unsigned int num_structs = m_head.lumps[idx].length / sizeof(bsp_struct_T);
	// Check for error
	if (sizeof(bsp_struct_T) * num_structs != m_head.lumps[idx].length) {
		std::cerr << "ERROR: Reading " << BSP_FILE::lump_names.at(idx) << ", num_struts * sizeof(struct) (" << num_structs << " * " << sizeof(bsp_struct_T) << " == " << sizeof(bsp_struct_T) * num_structs << ") doesn't equal the expected length (" << m_head.lumps[idx].length << ")" << std::endl;
		this->~BspProcessor();
		std::exit(EXIT_FAILURE);
	}
	// Should probably clear this and resize. So if it was already being used it can be reused.
	bsp_vec.resize(num_structs);
	file_stream.seekg(m_head.lumps[idx].offset, std::ios::beg);
	file_op(file_stream, reinterpret_cast <char*> (&bsp_vec[0]), m_head.lumps[idx].length);

}

void BspProcessor::read_from_file(std::string file_path) {
	//std::function<std::fstream& (std::fstream&, const char*, std::streamsize)> read_f = [](std::fstream& file, const char* s, std::streamsize n) {
	std::function < void(std::fstream&, char *, std::streamsize) > read_f = [](std::fstream& file, char* dest, std::streamsize dest_sz) {
		file.read ( dest, dest_sz );
	};
	
	std::fstream fs;
	fs.open(file_path, std::fstream::in | std::fstream::binary);
	if (!fs.is_open()) {
		std::cerr << "ERROR: file not open" << std::endl;
		this->~BspProcessor();
		std::exit(EXIT_FAILURE);
	}

	this->io(fs, read_f);
}

void BspProcessor::write_to_file ( std::string file_path ) {
	std::function < void ( std::fstream&, char *, std::streamsize ) > write_f = [] ( std::fstream& file, char * dest, std::streamsize dest_sz ) {
		file.write ( dest, dest_sz );
	};

	std::fstream fs;
	fs.open(file_path, std::fstream::out | std::fstream::binary);
	if (!fs.is_open()) {
		std::cerr << "ERROR: file not open" << std::endl;
		this->~BspProcessor();
		std::exit(EXIT_FAILURE);
	}

	this->io(fs, write_f);
}

BspProcessor::~BspProcessor() {
	//delete[] buff;
}