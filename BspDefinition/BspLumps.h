#ifndef BSP_LUMPS_H
#define BSP_LUMPS_H

#include <map>
#include <string>
#include <typeinfo>		// For typeid function

namespace BSP_FILE
{
	// Little-endian
	constexpr auto VBSPHEADER = (('P' << 24) + ('S' << 16) + ('B' << 8) + 'V');
	constexpr auto VHEADERLUMPS = 64;

	enum class LUMP {
		ENTITIES, // 0
		PLANES, // 1
		TEXDATA, // 2
		VERTEXES, // 3
		VISIBILITY, // 4
		NODES, // 5
		TEXINFO, // 6
		FACES, // 7
		LIGHTING, // 8
		OCCLUSION, // 9
		LEAFS, // 10
		FACEIDS, // 11
		EDGES, // 12
		SURFEDGES, // 13
		MODELS, // 14
		WORLDLIGHTS, // 15
		LEAFFACES, // 16
		LEAFBRUSHES, // 17
		BRUSHES, // 18
		BRUSHSIDES, // 19
		AREAS, // 20
		AREAPORTALS, // 21
		PROPCOLLISION, // 22
		PROPHULLS, // 23
		PROPHULLVERTS, // 24
		PROPTRIS, // 25
		DISPINFO, // 26
		ORIGINALFACES, // 27
		PHYSDISP, // 28
		PHYSCOLLIDE, // 29
		VERTNORMALS, // 30
		VERTNORMALINDICES, // 31
		DISP_LIGHTMAP_ALPHAS, // 32
		DISP_VERTS, // 33
		DISP_LIGHTMAP_SAMPLE_POSITIONS, // 34
		GAME_LUMP, // 35
		LEAFWATERDATA, // 36
		PRIMITIVES, // 37
		PRIMVERTS, // 38
		PRIMINDICES, // 39
		PAKFILE, // 40
		CLIPPORTALVERTS, // 41
		CUBEMAPS, // 42
		TEXDATA_STRING_DATA, // 43
		TEXDATA_STRING_TABLE, // 44
		OVERLAYS, // 45
		LEAFMINDISTTOWATER, // 46
		FACE_MACRO_TEXTURE_INFO, // 47
		DISP_TRIS, // 48
		PROP_BLOB, // 49
		WATEROVERLAYS, // 50
		LEAF_AMBIENT_INDEX_HDR, // 51
		LEAF_AMBIENT_INDEX, // 52
		LIGHTING_HDR, // 53
		WORLDLIGHTS_HDR, // 54
		LEAF_AMBIENT_LIGHTING_HDR, // 55
		LEAF_AMBIENT_LIGHTING, // 56
		XZIPPAKFILE, // 57
		FACES_HDR, // 58
		MAP_FLAGS, // 59
		OVERLAY_FADES, // 60
		OVERLAY_SYSTEM_LEVELS, // 61
		PHYSLEVEL, // 62
		DISP_MULTIBLEND // 63
	};

	const std::map < LUMP, std::string > lump_names {
		{ LUMP::ENTITIES, "ENTITIES" },
		{ LUMP::PLANES, "PLANES" },
		{ LUMP::TEXDATA, "TEXDATA" },
		{ LUMP::VERTEXES, "VERTEXES" },
		{ LUMP::VISIBILITY, "VISIBILITY" },
		{ LUMP::NODES, "NODES" },
		{ LUMP::TEXINFO, "TEXINFO" },
		{ LUMP::FACES, "FACES" },
		{ LUMP::LIGHTING, "LIGHTING" },
		{ LUMP::OCCLUSION, "OCCLUSION" },
		{ LUMP::LEAFS, "LEAFS" },
		{ LUMP::FACEIDS, "FACEIDS" },
		{ LUMP::EDGES, "EDGES" },
		{ LUMP::SURFEDGES, "SURFEDGES" },
		{ LUMP::MODELS, "MODELS" },
		{ LUMP::WORLDLIGHTS, "WORLDLIGHTS" },
		{ LUMP::LEAFFACES, "LEAFFACES" },
		{ LUMP::LEAFBRUSHES, "LEAFBRUSHES" },
		{ LUMP::BRUSHES, "BRUSHES" },
		{ LUMP::BRUSHSIDES, "BRUSHSIDES" },
		{ LUMP::AREAS, "AREAS" },
		{ LUMP::AREAPORTALS, "AREAPORTALS" },
		{ LUMP::PROPCOLLISION, "PROPCOLLISION" },
		{ LUMP::PROPHULLS, "PROPHULLS" },
		{ LUMP::PROPHULLVERTS, "PROPHULLVERTS" },
		{ LUMP::PROPTRIS, "PROPTRIS" },
		{ LUMP::DISPINFO, "DISPINFO" },
		{ LUMP::ORIGINALFACES, "ORIGINALFACES" },
		{ LUMP::PHYSDISP, "PHYSDISP" },
		{ LUMP::PHYSCOLLIDE, "PHYSCOLLIDE" },
		{ LUMP::VERTNORMALS, "VERTNORMALS" },
		{ LUMP::VERTNORMALINDICES, "VERTNORMALINDICES" },
		{ LUMP::DISP_LIGHTMAP_ALPHAS, "DISP_LIGHTMAP_ALPHAS" },
		{ LUMP::DISP_VERTS, "DISP_VERTS" },
		{ LUMP::DISP_LIGHTMAP_SAMPLE_POSITIONS, "DISP_LIGHTMAP_SAMPLE_POSITIONS" },
		{ LUMP::GAME_LUMP, "GAME_LUMP" },
		{ LUMP::LEAFWATERDATA, "LEAFWATERDATA" },
		{ LUMP::PRIMITIVES, "PRIMITIVES" },
		{ LUMP::PRIMVERTS, "PRIMVERTS" },
		{ LUMP::PRIMINDICES, "PRIMINDICES" },
		{ LUMP::PAKFILE, "PAKFILE" },
		{ LUMP::CLIPPORTALVERTS, "CLIPPORTALVERTS" },
		{ LUMP::CUBEMAPS, "CUBEMAPS" },
		{ LUMP::TEXDATA_STRING_DATA, "TEXDATA_STRING_DATA" },
		{ LUMP::TEXDATA_STRING_TABLE, "TEXDATA_STRING_TABLE" },
		{ LUMP::OVERLAYS, "OVERLAYS" },
		{ LUMP::LEAFMINDISTTOWATER, "LEAFMINDISTTOWATER" },
		{ LUMP::FACE_MACRO_TEXTURE_INFO, "FACE_MACRO_TEXTURE_INFO" },
		{ LUMP::DISP_TRIS, "DISP_TRIS" },
		{ LUMP::PROP_BLOB, "PROP_BLOB" },
		{ LUMP::WATEROVERLAYS, "WATEROVERLAYS" },
		{ LUMP::LEAF_AMBIENT_INDEX_HDR, "LEAF_AMBIENT_INDEX_HDR" },
		{ LUMP::LEAF_AMBIENT_INDEX, "LEAF_AMBIENT_INDEX" },
		{ LUMP::LIGHTING_HDR, "LIGHTING_HDR" },
		{ LUMP::WORLDLIGHTS_HDR, "WORLDLIGHTS_HDR" },
		{ LUMP::LEAF_AMBIENT_LIGHTING_HDR, "LEAF_AMBIENT_LIGHTING_HDR" },
		{ LUMP::LEAF_AMBIENT_LIGHTING, "LEAF_AMBIENT_LIGHTING" },
		{ LUMP::XZIPPAKFILE, "XZIPPAKFILE" },
		{ LUMP::FACES_HDR, "FACES_HDR" },
		{ LUMP::MAP_FLAGS, "MAP_FLAGS" },
		{ LUMP::OVERLAY_FADES, "OVERLAY_FADES" },
		{ LUMP::OVERLAY_SYSTEM_LEVELS, "OVERLAY_SYSTEM_LEVELS" },
		{ LUMP::PHYSLEVEL, "PHYSLEVEL" },
		{ LUMP::DISP_MULTIBLEND, "DISP_MULTIBLEND" }
	};

	

	/****** DATA STRUCTURES USED IN INDIVIDUAL LUMP STRUCTURES THAT MAKE UP LUMP ARRAYS ******/

	// 3d point in space, with floating point precision
	struct bsp_point3f {
		float x;
		float y;
		float z;
	};

	// CURRENTLY UNUSED, 3d point in space, with 2 byte integer precision
	/*struct bsp_point3s {
		int16_t x;
		int16_t y;
		int16_t z;
	};*/

	/****** DATA STRUCTURES THAT COMPOSE LUMP ARRAYS ******/ 
	
	// Plane (Lump 1): mathematical definition of plane in 3d space.
	//  A * (normal.x) + B * (normal.y) + C * (normal.z) = distance
	//  type specifies flags that are seemingly unused
	struct bsp_plane
	{
		bsp_point3f normal;
		float		distance;
		int32_t		type;
	};

	// Vertex (Lump 3): vertices of map geometry, just array of 3d points
	typedef bsp_point3f bsp_vertex;

	// Edge (Lump 12): edges that compose map geometry
	//  vert specifies two indices into vertex lump (Lump 3)
	struct bsp_edge {
		uint16_t vert[2];		// Index into vertex array
	};

	// Surfedge (Lump 13): signed index into edge array.
	//  Positive value indicates edge defined from vert[0] to vert [1]
	//  Negative value indicates edge defined from vert[1] to vert [0]
	typedef int32_t bsp_surfedge;

	// Face (Lumps 7 and 27): faces that compose map geometry
	//  EXPLANATION PENDING
	struct bsp_face {
		uint16_t	planenum;		// the plane number
		uint8_t		side;			// faces opposite to the node's plane direction
		uint8_t		onNode;			// 1 of on node, 0 if in leaf
		int32_t		firstedge;		// index into surfedges
		int16_t		numedges;		// number of surfedges
		int16_t		texinfo;		// texture info
		int16_t		dispinfo;		// displacement info
		int16_t		surfaceFogVolumeID;	// ?
		uint8_t		styles[4];		// switchable lighting info
		int32_t		lightofs;		// offset into lightmap lump
		float		area;			// face area in units^2
		int32_t		LightmapTextureMinsInLuxels[2];	// texture lighting info
		int32_t		LightmapTextureSizeInLuxels[2];	// texture lighting info
		int32_t		origFace;		// original face this was split from
		uint16_t	numPrims;		// primitives
		uint16_t	firstPrimID;	// ?
		uint32_t	smoothingGroups;	// lightmap smoothing group
	};

	// Brush (Lump 18): specifies original brushes
	//  firstside is index into brushside array lump, numsides specifies how many subsequent sides make the brush
	//  EXPLANATION PENDING: contents flags have various meanings, but
	struct bsp_brush {
		int	firstside;	// first brushside
		int	numsides;	// number of brushsides
		int	contents;	// contents flags
	};

	// Brushside (Lump 19): specifies sides and parameters for each brush face
	//  EXPLANATION PENDING
	struct bsp_brushside {
		uint16_t	planenum;	// facing out of the leaf, index into plane lump
		int16_t		texinfo;	// texture info, from texture lump
		int16_t		dispinfo;	// displacement info, from displacement info lump
		int16_t		bevel;		// is the side a bevel plane? used for collision
	};

	/****** DATA STRUCTURES THAT HAVE BSP INFO ******/

	// Lump descriptor
	//  EXPLANATION PENDING
	struct bsp_lump {
		int32_t offset;
		int32_t length;
		char	lump_ID[4];
		int32_t version;
	};

	// BSP and lump location/size descriptor
	//  EXPLANATION PENDING
	struct bsp_header {
		char		magic[4];
		int32_t		version;
		bsp_lump	lumps[VHEADERLUMPS];
		int32_t		map_revision;
	};

	/*
	//	It's sort of a mapping of the struct types to the lumps that they are relevent for???
	// I think this is a bad idea and won't really be used.
	//	Problem is multiple lumps can use the same struct type.
	//	It's not a 1:1 mapping.
	//	I don't think this will be used. Can probably just remove, it was a bad idea.
	const std::map < std::type_info, LUMP > type_names{
		{ typeid(std::bsp_edge), LUMP::EDGES }
	};
	*/
} /* BSP_FILE */
#endif /* BSP_LUMPS_H */