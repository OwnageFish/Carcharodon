

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
		std::cout << "\t" << BSP_FILE::lump_names.at(static_cast <BSP_FILE::LUMP> (iter))
		<< ": offset = " << m_head.lumps[iter].offset
		<< ", length = " << m_head.lumps[iter].length
		<< ", version = " << m_head.lumps[iter].version
		<< ", lump ID = " << m_head.lumps[iter].lmpID
		<< std::endl;
	std::cout << "}" << std::endl;



	struct_io(file_stream, file_op, m_edges, BSP_FILE::EDGES);
	struct_io(file_stream, file_op, m_vertexes, BSP_FILE::VERTEXES);

	/*
	unsigned int num_vertexes = m_head.lumps[BSP_FILE::VERTEXES].length / sizeof(point3f);
	file_stream.seekg(m_head.lumps[BSP_FILE::VERTEXES].offset, std::ios::beg);
	m_vertexes.resize(num_vertexes);
	file_op(file_stream, reinterpret_cast <char*> (&m_vertexes[0]), m_head.lumps[BSP_FILE::VERTEXES].length );
	*/

	std::cout << "Num edges: " << m_edges.size() << std::endl;
	for (int i = 0; i < m_edges.size(); i++)
		std::cout << i << ": " << m_edges[i].vert[0] << " - x=" << m_vertexes[m_edges[i].vert[0]].x << " y=" << m_vertexes[m_edges[i].vert[0]].y << " z=" << m_vertexes[m_edges[i].vert[0]].z
				  << ", "      << m_edges[i].vert[1] << " - x=" << m_vertexes[m_edges[i].vert[1]].x << " y=" << m_vertexes[m_edges[i].vert[1]].y << " z=" << m_vertexes[m_edges[i].vert[1]].z
				  << std::endl;

	/*
	// Trying to read in edges
	file_stream.seekg(m_head.lumps[BSP_FILE::EDGES].offset, std::ios::beg);
	unsigned int num_edges = m_head.lumps[BSP_FILE::EDGES].length / sizeof(bsp_edge);
	std::cout << num_edges << std::endl;
	m_edges = new bsp_edge[num_edges];
	//std::cout << std::to_string(m_edges[0].vert[0]) << ", " << std::to_string(m_edges[0].vert[1]) << std::endl;
	// I think this should work
	file_op(file_stream, reinterpret_cast <char*> (&m_edges), m_head.lumps[BSP_FILE::EDGES].length);
	//file_op(file_stream, reinterpret_cast < char * > (&m_edges), sizeof(bsp_edge) * num_edges);
	std::cout << num_edges << std::endl;
	std::cout << m_edges[0].vert[0] << ", " << m_edges[0].vert[1] << std::endl;
	*/
	
}

//		*** NEVER MIND THIS CAN SEEK TO THE CORRECT SPOT.
//		Mby it should clear out 
// This function assumes the filepointer has already been seeked to the offset needed for reading/writing
//	Not sure if this should be moved into this function. I really don't feel like doing that.
template<typename bsp_struct_T>
void BspProcessor::struct_io(std::fstream& file_stream, 
							 std::function < void(std::fstream&, char*, std::streamsize) > file_op, 
							 std::vector < bsp_struct_T > &bsp_vec, int lump_indx) {
	unsigned int num_structs = m_head.lumps[lump_indx].length / sizeof(bsp_struct_T);
	// Check for error
	if (sizeof(bsp_struct_T) * num_structs != m_head.lumps[lump_indx].length) {
		std::cerr << "ERROR: Reading, num_struts * sizeof(struct) doesn't equal the expected length" << std::endl;
		this->~BspProcessor();
		std::exit(EXIT_FAILURE);
	}
	// Should probably clear this and resize. So if it was already being used it can be reused.
	bsp_vec.resize(num_structs);
	file_stream.seekg(m_head.lumps[lump_indx].offset, std::ios::beg);
	file_op(file_stream, reinterpret_cast <char*> (&bsp_vec[0]), m_head.lumps[lump_indx].length);

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