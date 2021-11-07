#ifndef BSP_READER_H
#define BSP_READER_H

#include "BspLumps.h"
#include <fstream>		// For reading and writing the bsp file
#include <cstdint>		// For specific int types
#include <functional>

// Big-endian
//	How do I know when to use which?
constexpr auto VBSPHEADERBIG = (('V'<<24)+('B'<<16)+('S'<<8)+'P');

// Little-endian
constexpr auto VBSPHEADER = (('P'<<24)+('S'<<16)+('B'<<8)+'V');
constexpr auto VHEADERLUMPS = 64;

// Create lookup table for the chunks names to indicies
//	Enum type of whatever it's called 

struct point3f {
	float x;
	float y;
	float z;
};
struct point3s {
	int16_t x;
	int16_t y;
	int16_t z;
};

struct bsp_lump {
	uint32_t offset;		// offset (in bytes) of the data from the beginning of the file 
	uint32_t length;		// length (in bytes) of the data
	uint32_t version;
	char	lmpID[4];		// Lump ID 
};

struct bsp_header {
	uint32_t magic;
	uint32_t version;
	bsp_lump lumps[VHEADERLUMPS];
	uint32_t map_revision;
};

struct bsp_plane {
	point3f normal;
	float	  dist;
	int		  type;
};

struct bsp_edge {
	uint8_t vert[2];		// Index into vertex array
};

class BspProcessor {
public:
	BspProcessor();
	BspProcessor(std::string file_name);
	
	
	void read_from_file(std::string file_name);
	void write_to_file(std::string file_name);
	void io(std::fstream& file_stream, std::function < void(std::fstream&, char*, std::streamsize) > file_op);
	/* Potential future functions to add that are not very difficult.
	BspReader();
	int file(char* file_name);
	*/

	~BspProcessor();
private:
	// Not sure if the file really even has to be saved once it is loaded into memory. Can probably just forget about this.
	//std::ifstream m_bsp_file;
	bsp_header m_head;


};

#endif /* BSP_READER_H */