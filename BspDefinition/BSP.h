#ifndef BSP_H
#define BSP_H

#include "BspLumps.h"

#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstdlib>

#include <string.h>
#include <typeinfo>

namespace {
	// Return value is the new updated offset.
	//	The offset for the next lump
	template < typename bsp_struct >
	int32_t calcLumpOffLen(BSP_FILE::bsp_lump& lump, const std::vector < bsp_strut >& lump_data, int32_t offset) {
		lump.offset = offset;
		lump.length = lump_data.size() * sizeof(bsp_struct);
		return offset + lump.length;
	}

	// Calculate and return a new header, based on the input bsp.
	//	used for the writing functions.
	BSP_FILE::bsp_header calcHeader(const BSP::bsp_data& bsp) {
		BSP_FILE::bsp_header temp = bsp.m_head;

		// Set all lumps offset and length to 0
		for (int iter = 0; iter < BSP_FILE::VHEADERLUMPS; i++) {
			temp.lumps[iter].length = 0;
			temp.lumps[iter].offset = 0;
			temp.lumps[iter].lump_ID = bsp.m_head.lumps[iter].lump_ID;
			temp.lumps[iter].version = bsp.m_head.lumps[iter].version;
		}

		// Calculate new offsets and lengths
		//	First offset starts after the bsp_header
		int32_t temp_offset = sizeof(BSP_FILE::bsp_header);
		temp_offset = calcLumpOffLen(temp.lumps[BSP_FILE::LUMP::PLANES], bsp.m_planes, temp_offset);
		temp_offset = calcLumpOffLen(temp.lumps[BSP_FILE::LUMP::VERTEXES], bsp.m_planes, temp_offset);
		temp_offset = calcLumpOffLen(temp.lumps[BSP_FILE::LUMP::EDGES], bsp.m_planes, temp_offset);
		temp_offset = calcLumpOffLen(temp.lumps[BSP_FILE::LUMP::SURFEDGES], bsp.m_planes, temp_offset);
		temp_offset = calcLumpOffLen(temp.lumps[BSP_FILE::LUMP::FACES], bsp.m_planes, temp_offset);
		temp_offset = calcLumpOffLen(temp.lumps[BSP_FILE::LUMP::ORIGINALFACES], bsp.m_planes, temp_offset);
		temp_offset = calcLumpOffLen(temp.lumps[BSP_FILE::LUMP::BRUSHES], bsp.m_planes, temp_offset);
		temp_offset = calcLumpOffLen(temp.lumps[BSP_FILE::LUMP::BRUSHSIDES], bsp.m_planes, temp_offset);
	}

	// Return the new header and let other code decide what to do with it.
	return temp;
}

namespace BSP {
	// All of the things that are contained in a bsp.
	//	This will get passed into the read/write functions.
	struct bsp_data {
		BSP_FILE::bsp_header m_head;
		std::vector < BSP_FILE::bsp_plane >		m_planes;
		std::vector < BSP_FILE::bsp_vertex >	m_vertexes;
		std::vector < BSP_FILE::bsp_edge >		m_edges;
		std::vector < BSP_FILE::bsp_surfedge >	m_surfedges;
		std::vector < BSP_FILE::bsp_face >		m_faces;
		std::vector < BSP_FILE::bsp_face >		m_origfaces;
		std::vector < BSP_FILE::bsp_brush >		m_brushes;
		std::vector < BSP_FILE::bsp_brushside >	m_brushsides;
	};


	template < typename bsp_struct >
	void fileToBuffer(std::fstream& file_stream, const BSP_FILE::bsp_lump& lump, std::vector < bsp_struct >& output)												
	{
		// Resize synchronization buffer
		if (lump.length % sizeof(bsp_struct)) {
			std::cerr << "ERROR: Reading, " << typeid(bsp_struct).name() << " structs, length not evenly divisible by struct size..." << std::endl;
			std::exit(EXIT_FAILURE);
		}
		output.resize(lump.length / sizeof(bsp_struct));

		// Actually copy the data
		file_stream.seekg(lump.offset, std::ios::beg);
		file_stream.read(reinterpret_cast <char*> (&output[0]), lump.length);
	}

	void readFromFile(std::fstream& file_stream, BSP::bsp_data& in_bsp) {

		// Read in the header
		file_stream.read(reinterpret_cast <char*> (&in_bsp.m_head), sizeof(BSP_FILE::bsp_header));

		// Use header info to grab only the stuff we need
		fileToBuffer(file_stream, in_bsp.m_head.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::PLANES)],			in_bsp.m_planes);
		fileToBuffer(file_stream, in_bsp.m_head.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::VERTEXES)],		in_bsp.m_vertexes);
		fileToBuffer(file_stream, in_bsp.m_head.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::EDGES)],			in_bsp.m_edges);
		fileToBuffer(file_stream, in_bsp.m_head.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::SURFEDGES)],		in_bsp.m_surfedges);
		fileToBuffer(file_stream, in_bsp.m_head.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::FACES)],			in_bsp.m_faces);
		fileToBuffer(file_stream, in_bsp.m_head.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::ORIGINALFACES)],	in_bsp.m_origfaces);
		fileToBuffer(file_stream, in_bsp.m_head.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::BRUSHES)],			in_bsp.m_brushes);
		fileToBuffer(file_stream, in_bsp.m_head.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::BRUSHSIDES)],		in_bsp.m_brushsides);
	}

	template < typename bsp_struct >
	void bufferToFile(std::fstream& file_stream, const BSP_FILE::bsp_lump& lump, const std::vector < bsp_struct >& intput) {
		if (lump.length % sizeof(bsp_struct)) {
			std::cerr << "ERROR: Writting, " << typeid(bsp_struct).name() << " structs, lenght not evenly divisible by struct size..." << std::endl;
			std::exit(EXIT_FAILURE);
		}

		// Write the data using the info from the lump header. Which is stored in the bsp_lump
		file_stream.seekg(lump.offset, std::ios::beg);
		file_stream.write(reinterpret_cast <char*> (&input), lump.length);
	}

	void writeToFile(std::fstream& file_stream, const BSP::bsp_data& out_bsp) {
		BSP_FILE::bsp_header temp = calcHeader(out_bsp);
		//	Need to decide what to do with the header.
		//	Probably shouldn't change the out_bsp header.
		file_stream.write(reinterpret_cast <char*> (&out_bsp.m_head), sizeof(BSP_FILE::bsp_header));

		// Use temp header to write only the stuff we are dealing with so far.
		//	Want to be able to write out a bsp file that is openable in csgo.
		bufferToFile(file_stream, temp.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::PLANES)],			out_bsp.m_planes);
		bufferToFile(file_stream, temp.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::VERTEXES)],			out_bsp.m_vertexes);
		bufferToFile(file_stream, temp.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::EDGES)],			out_bsp.m_edges);
		bufferToFile(file_stream, temp.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::SURFEDGES)],		out_bsp.m_surfedges);
		bufferToFile(file_stream, temp.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::FACES)],			out_bsp.m_faces);
		bufferToFile(file_stream, temp.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::ORIGINALFACES)],	out_bsp.m_origfaces);
		bufferToFile(file_stream, temp.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::BRUSHES)],			out_bsp.m_brushes);
		bufferToFile(file_stream, temp.lumps[static_cast <uint8_t> (BSP_FILE::LUMP::BRUSHSIDES)],		out_bsp.m_brushsides);
	}

	void coutData(const bsp_data& in_bsp)
	{
		std::cout << "Magic Number: " << in_bsp.m_head.magic << std::endl;
		std::cout << "Version Number: " << in_bsp.m_head.version << std::endl;
		std::cout << "Revision Number: " << in_bsp.m_head.map_revision << std::endl;
		std::cout << "Lumps: {" << std::endl;
		for (int iter = 0; iter < BSP_FILE::VHEADERLUMPS; iter++)
			if (in_bsp.m_head.lumps[iter].length != 0)
				std::cout << "\t" << BSP_FILE::lump_names.at(static_cast <BSP_FILE::LUMP> (iter))
				<< ": offset = " << in_bsp.m_head.lumps[iter].offset
				<< ", length = " << in_bsp.m_head.lumps[iter].length
				<< ", version = " << in_bsp.m_head.lumps[iter].version
				<< ", lump ID = " << static_cast <int> (in_bsp.m_head.lumps[iter].lump_ID[0]) << "." << static_cast <int> (in_bsp.m_head.lumps[iter].lump_ID[1] ) << "." << static_cast <int> (in_bsp.m_head.lumps[iter].lump_ID[2]) << "." << static_cast <int> (in_bsp.m_head.lumps[iter].lump_ID[3])
				<< std::endl;
		std::cout << "}" << std::endl;

		std::cout << "Num edges: " << in_bsp.m_edges.size() << std::endl;
		for (int i = 0; i < in_bsp.m_edges.size(); i++)
		{
			std::cout << i << ": " << in_bsp.m_edges[i].vert[0] << " - x=" << in_bsp.m_vertexes[in_bsp.m_edges[i].vert[0]].x << " y=" << in_bsp.m_vertexes[in_bsp.m_edges[i].vert[0]].y << " z=" << in_bsp.m_vertexes[in_bsp.m_edges[i].vert[0]].z
				<< ", " << in_bsp.m_edges[i].vert[1] << " - x=" << in_bsp.m_vertexes[in_bsp.m_edges[i].vert[1]].x << " y=" << in_bsp.m_vertexes[in_bsp.m_edges[i].vert[1]].y << " z=" << in_bsp.m_vertexes[in_bsp.m_edges[i].vert[1]].z
				<< std::endl;
		}
	}

} /* BSP */

#endif /* BSP_H */


//// START OF THE OVER-ENGINEERING PART
// 
// I don't understand the need for over-engineering...
// This is my problem with following paradigms that make things fast and easy to implement, it ends up being weird and overly complicated to write the base framework
// Besides that, the BSP format is fairly fixed. We literally want a small subset of the actually available information. And we know how to grab it if needed.
// I want to get things implemented quickly for now...
// 
// Anonymous namespace for "private" helper functions
/*namespace {

	// NEED TO FIX THIS
	//	Somehow we need to pass this function the length and offset.
	template < typename bsp_struct_T >
	void struct_io(std::fstream& file_stream,
				   std::function < void(std::fstream&, char*, std::streamsize) > file_op,
				   , int32_t length ){
				   //std::vector< bsp_struct_T >& bsp_vec, int32_t length ) {
		unsigned int num_structs = length / sizeof(bsp_struct_T);
		// Check for error
		if (sizeof(bsp_struct_T) * num_structs != length) {
			std::cerr << "ERROR: Reading, num_struts * sizeof(struct) doesn't equal the expected length" << std::endl;
			//this->~BspProcessor();
			std::exit(EXIT_FAILURE);
		}
		// Should probably clear this and resize. So if it was already being used it can be reused.
		bsp_vec.resize(num_structs);
		//file_stream.seekg(m_head.lumps[lump_indx].offset, std::ios::beg);
		//file_op(file_stream, reinterpret_cast <char*> (&bsp_vec[0]), length);
		file_op(file_stream, struct_vector, length);
	}

	const std::map < LUMP, bsp_lump_parameters > lump_get_parameters = {

		// World parameters
		//{ BSP_FILE::LUMP::ENTITIES, "ENTITIES" }, This needs to be figured out, as this is completely dynamic

		// Stuff needed for basic geometry rendering
		{ BSP_FILE::LUMP::PLANES,			{ sizeof(bsp_plane),		"PLANES",			1 } },
		{ BSP_FILE::LUMP::VERTEXES,			{ sizeof(bsp_vertex),		"VERTEXES",			3 } },
		{ BSP_FILE::LUMP::FACES,			{ sizeof(bsp_face),			"FACES",			7 } },
		{ BSP_FILE::LUMP::EDGES,			{ sizeof(bsp_edge),			"EDGES",			12 } },
		{ BSP_FILE::LUMP::SURFEDGES,		{ sizeof(bsp_surfedge),		"SURFEDGES",		13 } },
		{ BSP_FILE::LUMP::BRUSHES,			{ sizeof(bsp_brush),		"BRUSHES",			18 } },
		{ BSP_FILE::LUMP::BRUSHSIDES,		{ sizeof(bsp_brushside),	"BRUSHSIDES",		19 } },
		{ BSP_FILE::LUMP::ORIGINALFACES,	{ sizeof(bsp_face),			"ORIGINALFACES",	27 } },

		// Displacements
		//{ BSP_FILE::LUMP::DISPINFO,		{ sizeof(int),	"DISPINFO", 26 } },
		//{ BSP_FILE::LUMP::DISP_VERTS,		{ sizeof(int),	"DISP_VERTS", 33 } },
		//{ BSP_FILE::LUMP::DISP_TRIS,		{ sizeof(int),	"DISP_TRIS", 48 } }
	};

	
	template < typename bsp_struct >
	void getBufferAndCopy(std::fstream& file_stream, const BSP_FILE::bsp_lump& lump,
		std::function < void(std::fstream&, char*, std::streamsize) > file_op,	
		std::vector < bsp_struct >& output)										
	{
		// Resize synchronization buffer
		if (lump.length % sizeof(bsp_struct)) {
			std::cerr << "ERROR: Reading, " << typeid(bsp_struct).name() << " structs, length not evenly divisible by struct size..." << std::endl;
			std::exit(EXIT_FAILURE);
		}
		output.resize(lump.length / sizeof(bsp_struct));

		file_stream.seekg(lump.offset, std::ios::beg);
		file_op(file_stream, &output[0], lump.length);
	}

	void io(std::fstream& file_stream, std::function < void(std::fstream&, char*, std::streamsize) > file_op, BSP::bsp_data& in_bsp) {
		file_op(file_stream, reinterpret_cast <char*> (&in_bsp.m_head), sizeof(bsp_header));

		// I don't know why this looks like highlighting doesn't know that this function exists.
		//	Is there some way that you have to reference functions in the current namespace???
		file_stream.seekg(in_bsp.lump[BSP_FILE::EDGES].offset, std::ios::beg);
		struct_io(file_stream, file_op, in_bsp.m_edges, BSP_FILE::EDGES);
		file_stream.seekg(in_bsp.lump[BSP_FILE::VERTEXES].offset, std::ios::beg);
		struct_io(file_stream, file_op, in_bsp.m_vertexes, BSP_FILE::VERTEXES);

		for (uint8_t iter = 0; iter < VHEADERLUMPS; iter++)
		{
			file_stream.seekg(in_bsp.m_head.lumps[iter].offset, std::ios::beg);
			struct_io (file_stream, file_op, )
			in_bsp.m_head.lumps [ iter ].

		}

		//std::cout << "Num edges: " << m_edges.size() << std::endl;
		for (int i = 0; i < in_bsp.m_edges.size(); i++)
		{
			std::cout << i << ": " << in_bsp.m_edges[i].vert[0] << " - x=" << in_bsp.m_vertexes[m_edges[i].vert[0]].x << " y=" << in_bsp.m_vertexes[m_edges[i].vert[0]].y << " z=" << in_bsp.m_vertexes[m_edges[i].vert[0]].z
				<< ", " << in_bsp.m_edges[i].vert[1] << " - x=" << in_bsp.m_vertexes[m_edges[i].vert[1]].x << " y=" << in_bsp.m_vertexes[m_edges[i].vert[1]].y << " z=" << in_bsp.m_vertexes[m_edges[i].vert[1]].z
				<< std::endl;
		}

	}



	void read_from_file(std::string file_path, const bsp_data& in_bsp) {
		std::function < void(std::fstream&, char*, std::streamsize) > read_f = [](std::fstream& file, char* dest, std::streamsize dest_sz) {
			file.read(dest, dest_sz);
		};

		std::fstream fs;
		fs.open(file_path, std::fstream::in | std::fstream::binary);
		if (!fs.is_open()) {
			std::cerr << "ERROR: file not open" << std::endl;
			//this->~BspProcessor();
			std::exit(EXIT_FAILURE);
		}
		io(fs, read_f, in_bsp);
	}

	void write_to_file(std::string file_path, bsp_data& out_bsp) {
		std::function < void(std::fstream&, char*, std::streamsize) > write_f = [](std::fstream& file, char* dest, std::streamsize dest_sz) {
			file.write(dest, dest_sz);
		};

		std::fstream fs;
		fs.open(file_path, std::fstream::out | std::fstream::binary);
		if (!fs.is_open()) {
			std::cerr << "ERROR: file not open" << std::endl;
			//this->~BspProcessor();
			std::exit(EXIT_FAILURE);
		}

		//bsp_header temp = out_bsp.m_head;

		// This is copied here b/c I was going to use something like this to calculate the header offsets and lengths.
		// I think we just calculate the header before writing. This makes writing really easy.
		for (int iter = 0; iter < VHEADERLUMPS; iter++) {
			std::cout << "\t" << BSP_FILE::lump_names.at(static_cast <BSP_FILE::LUMP> (iter))
				<< ": offset = " << in_bsp.m_head.lumps[iter].offset
				<< ", length = " << in_bsp.m_head.lumps[iter].length
				<< ", version = " << in_bsp.m_head.lumps[iter].version
				<< ", lump ID = " << in_bsp.m_head.lumps[iter].lmpID
				<< std::endl;
			std::cout << "}" << std::endl;
		}


		//temp = out_bsp.m_head.lumps[BSP_FILE::EDGES];
		//temp.offset = sizeof(bsp_header);
		// Need to calculate offsets for each lump. And generate a new
		io(fs, write_f, out_bsp);
	}

	// Calculate header for the bsp struct
	BSP::bsp_header calc_header(BSP::bsp in_bsp) {


	}
}*/ /* anonymous namespace */