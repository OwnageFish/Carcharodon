
#include <iostream>
#include <string>
#include <functional>
#include <fstream>
#include <vector>

namespace {

	// NEED TO FIX THIS
	//	Somehow we need to pass this function the length and offset.
	template < typename bsp_struct_T >
	void struct_io(std::fstream& file_stream,
				   std::function < void(std::fstream&, char*, std::streamsize) > file_op,
				   std::vector< bsp_struct_T >& bsp_vec, int lump_indx) ) {
		unsigned int num_structs = m_head.lumps[lump_indx].length / sizeof(bsp_struct_T);
		// Check for error
		if (sizeof(bsp_struct_T) * num_structs != m_head.lumps[lump_indx].length) {
			std::cerr << "ERROR: Reading, num_struts * sizeof(struct) doesn't equal the expected length" << std::endl;
			//this->~BspProcessor();
			std::exit(EXIT_FAILURE);
		}
		// Should probably clear this and resize. So if it was already being used it can be reused.
		bsp_vec.resize(num_structs);
		file_stream.seekg(m_head.lumps[lump_indx].offset, std::ios::beg);
		file_op(file_stream, reinterpret_cast <char*> (&bsp_vec[0]), m_head.lumps[lump_indx].length);
	}

	//////////////////////////////////////////////////////// This is fucked :(
	// Why doesn't this one get all the nice highlighting????
	void io(std::fstream& file_stream, std::function < void(std::fstream&, char*, std::streamsize) > file_op, bsp in_bsp) {
		file_op(file_stream, reinterpret_cast <char*> (&in_bsp.m_head), sizeof(bsp_header));

		std::cout << "Magic Number: " << in_bsp.m_head.magic << std::endl;
		std::cout << "Version Number: " << in_bsp.m_head.version << std::endl;
		std::cout << "Revision Number: " << in_bsp.m_head.map_revision << std::endl;
		std::cout << "Lumps: {" << std::endl;
		for (int iter = 0; iter < VHEADERLUMPS; iter++)
			std::cout << "\t" << BSP_FILE::lump_names.at(static_cast <BSP_FILE::LUMP> (iter))
			<< ": offset = " << in_bsp.m_head.lumps[iter].offset
			<< ", length = " << in_bsp.m_head.lumps[iter].length
			<< ", version = " << in_bsp.m_head.lumps[iter].version
			<< ", lump ID = " << in_bsp.m_head.lumps[iter].lmpID
			<< std::endl;
		std::cout << "}" << std::endl;

		// I don't know why this looks like highlighting doesn't know that this function exists.
		//	Is there some way that you have to reference functions in the current namespace???
		struct_io(file_stream, file_op, in_bsp.m_edges, BSP_FILE::EDGES);
		struct_io(file_stream, file_op, in_bsp.m_vertexes, BSP_FILE::VERTEXES);

		std::cout << "Num edges: " << m_edges.size() << std::endl;
		for (int i = 0; i < in_bsp.m_edges.size(); i++)
			std::cout << i << ": " << in_bsp.m_edges[i].vert[0] << " - x=" << in_bsp.m_vertexes[m_edges[i].vert[0]].x << " y=" << in_bsp.m_vertexes[m_edges[i].vert[0]].y << " z=" << in_bsp.m_vertexes[m_edges[i].vert[0]].z
			<< ", " << in_bsp.m_edges[i].vert[1] << " - x=" << in_bsp.m_vertexes[m_edges[i].vert[1]].x << " y=" << in_bsp.m_vertexes[m_edges[i].vert[1]].y << " z=" << in_bsp.m_vertexes[m_edges[i].vert[1]].z
			<< std::endl;

	}

} /* anonymous namespace */


namespace BSP {
	// Anonymous namespace for "private" helper functions
	

	struct point3f {
		float x;
		float y;
		float z;
	};
	struct point3s {
		short x;
		short y;
		short z;
	};

	struct bsp_edge {
		uint16_t vert[2];		// Index into vertex array
	};

	struct bsp_header {
		char		magic[4];
		int32_t		version;
		bsp_lump	lumps[VHEADERLUMPS];
		int32_t		map_revision;
	};

	struct bsp_lump {
		int32_t offset;
		int32_t length;
		char	lump_ID[4];
		int32_t version;
	};

	// All of the things that are contained in a bsp.
	//	This will get passed into the read/write functions.
	struct bsp {
		struct m_head;
		std::vector < bsp_edge > m_edges;
		std::vector < point3f >  m_vertexes;

	};

	void read_from_file(std::string file_path) {
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
		io(fs, read_f, )
	}

	void write_to_rile(std::string file_path) {
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

		io(fs, write_f);
	}

} /* BSP */