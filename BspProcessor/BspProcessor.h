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
	int32_t offset;		//   offset (in bytes) of the data from the beginning of the file 
	int32_t length;		//   length (in bytes) of the data
	int32_t	version;
	char	lmpID[4];	//   Lump ID 
};

struct bsp_header {
	char magic[4];

	int32_t version;
	bsp_lump lumps[VHEADERLUMPS];
	int32_t map_revision;
};

struct bsp_plane {
	point3f normal;
	float	dist;
	int32_t	type;
};

struct bsp_edge {
	uint16_t vert[2];	//   Index into vertex lump
};

struct bsp_surfedge {
	int32_t edge;		//   Index into edge lump. abs(edge) to use it as index. Sign determines winding order:
						//    + means first to second vertex, - means second to first vertex
};

// NOT WELL DESCRIBED
struct bsp_face {
	uint16_t plane_ID;	//   Index into plane lump representing that plane.
	int8_t  side;		//   0 if plane faces same direction as face, non-zero otherwise
	int8_t	node_bool;	//   1 if this is on a node, 0 if it's part of a leaf
	int32_t first_edge;	//   Index into surfedge lump, and quantity to use from that index
	int16_t	num_edges;
	int16_t	tex_info;	//   Index into texinfo lump
	int16_t	disp_info;	//   Index into dispinfo lump, -1 if this face isn't a displacement. If it is, this face represents boundaries of that displacement surface
	int16_t	surf_fog_vol_ID;				// * Appears to be related to be fog drawing when played is underwater/looking through water.
	uint8_t styles[4];						//[* Switchable lighting info????
	int32_t	light_off;						//[* Offset into lightmap lump??? This sounds like this is an index not offset. Or this is going to get real messy.
	float		 area;						//[* face are in units^2
	int32_t	lightmap_text_min_in_lux[2];	//[* Bro wtf even is this. LUX?
	int32_t	lightmap_text_siz_in_lux[2];	//[* Again :) very nice.
	int32_t	orig_face;						//   Index into original face lump, from which this face was derived. 0 for all faces in original face lump
	uint16_t num_prims;						// * Number of primitives. Used to prevent appearance of cracks between adjacent faces.
	uint16_t first_prim_ID;					// *  Used to index into Primitives, PrimVerts, PrimIndices lumps (37, 38 and 39). These are undocumented...
											// *  See http://web.archive.org/web/20071110230828/http://www.chatbear.com/board.plm?a=viewthread&b=4991&t=137,1118051039,3296&s=0&id=862840
	uint32_t smooth_group;					//   Lightmap smoothing group.
};

// NOT WELL DESCRIBED
struct bsp_texinfo {
	float	texture_vecs[2][4];		// * [start/end] [xyz offset]
	float	lightmp_vecs[2][4];		// * [start/end] [xyz offset] - Length is unit texels????/area????
	int32_t flags;					//   Surface type. See TEXINFO_FLAGS enumerator for different flags
	int32_t texdata;				//   Pointer to texture entry in TEXDATA lump
};

// NOT WELL DESCRIBED
struct bsp_texdata {
	point3f	reflectivity;			// * RGB reflectivity of the texture
	int	nameStringTableID;			// * index into TexdataStringTable lump
	int	width, height;				// * Source image dimensions
	int	view_width, view_height;	// * Source image dimensions
};

struct bsp_node {
	int32_t		planenum;			// Index into plane lump; which plane subdivides the world
	int32_t		children[2];		// Children of this node. Positive values designate node indices into this node lump
									//  Negative numbers are leaves, with index -(children[x] + 1) into the leaf lump (index -100 becomes 99)
	int16_t		mins[3];			// Rough AABB bounding box for frustum culling
	int16_t		maxs[3];
	uint16_t	firstface;			// Index into the face lump
	uint16_t	numfaces;			//  counting both sides (what does this mean?)
	int16_t		area;				// If all leaves below this node are in the same area, then
									//  this is the area index. If not, this is -1
	int16_t		paddding;			// Padding struct to multiple of 32 bytes in length
};

// NOT WELL DESCRIBED
struct bsp_leaf {
	int32_t			contents;			// * Logical OR of all brushes (not needed? what does this even mean? OR of brush ids?)
	int16_t			cluster;			// * Cluster this leaf is in (what is a cluster)
	int16_t			area  : 9;			// * Area this leaf is in (what does this mean)
	int16_t			flags : 7;			// * Flags (there is no explaination for these)
	int16_t			mins[3];			//   Rough AABB bounding box for frustum culling
	int16_t			maxs[3];
	uint16_t		firstleafface;		//   Index into leaffaces lump (16), and quantity to use from that index
	uint16_t		numleaffaces;		//    Leaffaces lump contains indices into face array. Only used from here.
	uint16_t		firstleafbrush;		//   Index into leafbrushes lump, and quantity to use from that index
	uint16_t		numleafbrushes;		//    Leafbrushes lump contains indices into brush array. Only used from here.
	int16_t			leafWaterDataID;	// * -1 for not in water, undocumented for other values
};

/*	
 *	Struct name:	bsp_model
 *	Lumps:			LUMP::MODELS (14)
 *	References:		headnode	- LUMP::NODES (5)
 *					firstface	- LUMP::FACES (7), with quantity in numfaces
 *	Original name:	dmodel_t
 *	Descriptions:	Collection of brushess and faces that represent a brush entity.
 *					Index 0 representes worldspawn (overall level geometry, func_details)
 *					Subsequent indices represent brush entities. These are referenced in entity lump.
 *	Constraints:	Including worldspawn, max limit of 1024 bsp_models in this lump (const std::size_t MAX_MAP_MODELS = 1024;)
 */
struct bsp_model
{
	point3f	mins;		// Rough AABB bounding box for frustum culling
	point3f maxs;
	point3f	origin;		// Origin for the brush, specified by level designer (if non-zero). Relative to actual faces, not world. Used for sounds or lights
	int32_t	headnode;	// Index into node lump; top node in BSP which describes this model
	int32_t	firstface;	// Index into face lump; gives faces which make up this model
	int32_t numfaces;	//  Quantity of faces to use from face lump, starting from index given by firstface
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

	// Not sure if the file really even has to be saved once it is loaded into memory. Can probably just forget about this.
	//std::ifstream m_bsp_file;
	bsp_header m_head;

	std::vector < point3f > m_vertexes;
	std::vector < bsp_edge > m_edges;
	std::vector < bsp_surfedge > m_surfedges;
	std::vector < bsp_face > m_faces;
	std::vector < bsp_face > m_originalfaces;

	std::vector < bsp_texinfo > m_texinfos;
	std::vector < bsp_texdata > m_texdatas;

	std::vector < bsp_node > m_nodes;
	std::vector < bsp_leaf > m_leafs;
	std::vector < uint16_t > m_leaffaces;
	std::vector < uint16_t > m_leafbrushes;

	std::vector < bsp_model > m_brushmodels;

	// Probably should add like a allocated flag.
	//	So I can tell when I need to delete the arrays. and reallocate.
	bool alloc;

	//char buff;

	// Function for reading / writing any of the struct types.
	//	typename T should be a one of the bsp_structs
	template<typename bsp_struct_T, BSP_FILE::LUMP idx>
	void struct_io(std::fstream& file_stream, std::function < void(std::fstream&, char*, std::streamsize) > file_op, std::vector< bsp_struct_T > &bsp_vec);


};

#endif /* BSP_READER_H */