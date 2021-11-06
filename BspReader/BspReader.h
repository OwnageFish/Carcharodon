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

enum LUMP {
	ENTITIES,
	PLANES,
	TEXDATA,
	VERTEXES,
	VISIBILITY,
	NODES,
	TEXINFO,
	FACES,
	LIGHTING,
	OCCLUSION,
	LEAFS,
	FACEIDS,
	EDGES,
	SURFEDGES,
	MODELS,
	WORLDLIGHTS,
	LEAFFACES,
	LEAFBRUSHES,
	BRUSHES,
	BRUSHSIDES,
	AREAS,
	AREAPORTALS,
	PORTALS,
	UNUSED0,
	PROPCOLLISION,
	CLUSTERS,
	UNUSED1,
	PROPHULLS,
	PORTALVERTS,
	UNUSED2,
	PROPHULLVERTS,
	CLUSTERPORTALS,
	UNUSED3,
	PROPTRIS,
	DISPINFO,
	ORIGINALFACES,
	PHYSDISP,
	PHYSCOLLIDE,
	VERTNORMALS,
	VERTNORMALINDICES,
	DISP_LIGHTMAP_ALPHAS,
	DISP_VERTS,
	DISP_LIGHTMAP_SAMPLE_POSITIONS,
	GAME_LUMP,
	LEAFWATERDATA,
	PRIMITIVES,
	PRIMVERTS,
	PRIMINDICES,
	PAKFILE,
	CLIPPORTALVERTS,
	CUBEMAPS,
	TEXDATA_STRING_DATA,
	TEXDATA_STRING_TABLE,
	OVERLAYS,
	LEAFMINDISTTOWATER,
	FACE_MACRO_TEXTURE_INFO,
	DISP_TRIS,
	PHYSCOLLIDESURFACE,
	PROP_BLOB,
	WATEROVERLAYS,
	LIGHTMAPPAGES,
	LEAF_AMBIENT_INDEX_HDR,
	LIGHTMAPPAGEINFOS,
	LEAF_AMBIENT_INDEX,
	LIGHTING_HDR,
	WORLDLIGHTS_HDR,
	LEAF_AMBIENT_LIGHTING_HDR,
	LEAF_AMBIENT_LIGHTING,
	XZIPPAKFILE,
	FACES_HDR,
	MAP_FLAGS,
	OVERLAY_FADES,
	OVERLAY_SYSTEM_LEVELS,
	PHYSLEVEL,
	DISP_MULTIBLEND
};

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
	uint32_t version;
	char	lmpID[4];		// Lump ID 
};

st


class BspReader {
public:
	BspReader(char* file_name);
	
	/* Potential future functions to add that are not very difficult.
	BspReader();
	int file(char* file_name);
	*/

	~BspReader();
private:
	// Not sure if the file really even has to be saved once it is loaded into memory. Can probably just forget about this.
	ifstream m_bsp_file;
};

#endif /* BSP_READER_H */