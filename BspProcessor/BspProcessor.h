#ifndef BSP_READER_H
#define BSP_READER_H

#include "BspLumps.h"
#include <fstream>		// For reading and writing the bsp file
#include <cstdint>		// For specific int types
#include <functional>
#include <vector>		// For keeping lump values stored.

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
	short x;
	short y;
	short z;
};

struct bsp_lump {
	int32_t offset;
	int32_t length;
	int32_t version;
	/*
	int32_t offset;		// offset (in bytes) of the data from the beginning of the file 
	int32_t length;		// length (in bytes) of the data
	int32_t version;
	*/
	char	 lmpID[4];		// Lump ID 
};

struct bsp_header {
	//uint32_t magic;
	char magic[4];

	int32_t version;
	bsp_lump lumps[VHEADERLUMPS];
	int32_t map_revision;
	//int32_t version;
	//bsp_lump lumps[VHEADERLUMPS];
	//int32_t map_revision;
};

struct bsp_plane {
	point3f normal;
	float	  dist;
	int32_t		  type;
};

struct surfedge {
	int32_t edge;	// Index into edge array.
					//	**** Need to take abs of this value before using it to index.
					// Positive means first to second vertx
					//	Negative means second to first vertx
};

struct bsp_edge {
	//unsigned short vert[2];	// Index into vertex array
	uint16_t vert[2];		// Index into vertex array
};

struct bsp_face {
	uint8_t plane_ID;
	int8_t  side;	// I don't remember somehow this tells you what side of the plane the texture is on.
					//	Negative means something special. The oposite side of the plane normal? Or something.
	int8_t	node_bool;  // Pontential values 1 and 0 meaning is this is a node and not a leaf.
	int32_t first_edge; // index into surfedge
	int8_t	num_edge;	// Number of surfedges in the surfedge array.
	int8_t	tex_info;	// index into texture lump
	int8_t	disp_info;	// displacement info??
	int8_t	surf_fog_vol_ID;	// If the wiki's confused then I'm ??????????
	unsigned char styles[4];	// Switchable lighting info????
	int32_t	light_off;		// Offset into lightmap lump??? This sounds like this is an index not offset. Or this is going to get real messy.
	float		 area;		// face are in units^2
	int32_t	lightmap_text_min_in_lux[2];	// Bro wtf even is this. LUX?
	int32_t	lightmap_text_siz_in_lux[2];	// Again :) very nice.
	int32_t	orig_face;		// original face this was split from?
	uint8_t	num_prims;		// Number of primitives, oh boy, that's pretty sweet :)
	uint8_t st_prim_ID;		// first primitive index???? I really don't know what id is sounds like an index.
	uint32_t smooth_group;	// Lightmap smoothing group.
};

struct bsp_texinfo {
	float	texture_vecs[2][4];		//	[start/end] [xyz offset]
	float	lightmp_vecs[2][4];		//	[start/end] [xyz offset] - Length is unit texels????/area????
	int32_t flags;					//	I don't know some flags for some shit, who tf knows.
	int32_t texdata;				//	Pointer to texture name, size, etc?????

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

	//bsp_edge* m_edges;
	std::vector < bsp_edge > m_edges;
	std::vector < point3f > m_vertexes;


	// Probably should add like a allocated flag.
	//	So I can tell when I need to delete the arrays. and reallocate.
	bool alloc;

	//char buff;

	// Function for reading / writing any of the struct types.
	//	typename T should be a one of the bsp_structs
	template<typename bsp_struct_T>
	void struct_io(std::fstream& file_stream, std::function < void(std::fstream&, char*, std::streamsize) > file_op, std::vector< bsp_struct_T > bsp_vec, int lump_indx);


};

#endif /* BSP_READER_H */