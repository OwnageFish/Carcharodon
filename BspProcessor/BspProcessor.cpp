

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



	unsigned int num_edges = m_head.lumps[BSP_FILE::EDGES].length / sizeof(bsp_edge);
	file_stream.seekg(m_head.lumps[BSP_FILE::EDGES].offset, std::ios::beg);
	bsp_edge in_edge;
	m_edges_wut.resize(num_edges);
	for (int i = 0; i < num_edges; i++) {
		file_op(file_stream, reinterpret_cast <char*> (&in_edge), sizeof(bsp_edge));
		m_edges_wut[i] = in_edge;
	}

	std::cout << "Edge info: " << std::endl;
	std::cout << m_edges_wut.size() << std::endl;
	std::cout << m_edges_wut[0].vert[0] << ", " << m_edges_wut[0].vert[1] << std::endl;
	std::cout << m_edges_wut[1].vert[0] << ", " << m_edges_wut[1].vert[1] << std::endl;
	std::cout << m_edges_wut[2].vert[0] << ", " << m_edges_wut[2].vert[1] << std::endl;

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
							 std::vector < bsp_struct_T > bsp_vec, int lump_indx) {
	unsigned int num_structs = m_head.lumps[lump_indx].length / sizeof(T);
	bsp_struct_T temp;
	// Should probably clear this and resize. So if it was already being used it can be reused.
	bsp_vec.resize(num_structs);
	for (int i = 0; i < num_structs; i++) {
		file_op(file_stream, reinterpret_cast <char*> (&temp), sizeof(bsp_struct_T));
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// This sort of assignment will not work for all the struct types. Might need to instead write some copy function for each of the types?????
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		bsp_vec[i] = temp;
	}


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