#ifndef BSP_READER_H
#define BSP_READER_H

// Big-endian
//	How do I know when to use which?
constexpr auto VBPSHEADERBIG = (('V'<<24)+('B'<<16)+('S'<<8)+'P');

// Little-endian
constexpr auto VBSPHEADER = (('P'<<24)+('S'<<16)+('B'<<8)+'V');
constexpr auto VHEADERLUMPS = 64;

// Create lookup table for the chunks names to indicies
//	Enum type of whatever it's called 

#include <string>
#include <fstream>

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



struct bsp_header {
	uint32_t magic;
	uint32_t version;
	bsp_lump lumps[VHEADERLUMPS];
	uint32_t map_revision;
};

struct bsp_lump {
	uint32_t offset;		// offset (in bytes) of the data from the beginning of the file 
	uint32_t length;		// length (in bytes) of the data
};

st


class BspReader {
public:
	BspReader(char* file_name);
	
	BspReader();
	int file(char* file_name);

	~BspReader();
private:
	ifstream m_bsp_file;
};

#endif /* BSP_READER_H */