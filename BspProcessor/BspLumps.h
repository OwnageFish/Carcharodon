#ifndef BSP_LUMPS_H
#define BSP_LUMPS_H

#include <map>
#include <string>
#include <typeinfo>		// For typeid function

namespace BSP_FILE
{
	enum LUMP {
		ENTITIES = 0,
		PLANES = 1,
		TEXDATA = 2,
		VERTEXES = 3,
		VISIBILITY = 4,
		NODES = 5,
		TEXINFO = 6,
		FACES = 7,
		LIGHTING = 8,
		OCCLUSION = 9,
		LEAFS = 10,
		FACEIDS = 11,
		EDGES = 12,
		SURFEDGES = 13,
		MODELS = 14,
		WORLDLIGHTS = 15,
		LEAFFACES = 16,
		LEAFBRUSHES = 17,
		BRUSHES = 18,
		BRUSHSIDES = 19,
		AREAS = 20,
		AREAPORTALS = 21,
		PROPCOLLISION = 22,
		PROPHULLS = 23,
		PROPHULLVERTS = 24,
		PROPTRIS = 25,
		DISPINFO = 26,
		ORIGINALFACES = 27,
		PHYSDISP = 28,
		PHYSCOLLIDE = 29,
		VERTNORMALS = 30,
		VERTNORMALINDICES = 31,
		DISP_LIGHTMAP_ALPHAS = 32,
		DISP_VERTS = 33,
		DISP_LIGHTMAP_SAMPLE_POSITIONS = 34,
		GAME_LUMP = 35,
		LEAFWATERDATA = 36,
		PRIMITIVES = 37,
		PRIMVERTS = 38,
		PRIMINDICES = 39,
		PAKFILE = 40,
		CLIPPORTALVERTS = 41,
		CUBEMAPS = 42,
		TEXDATA_STRING_DATA = 43,
		TEXDATA_STRING_TABLE = 44,
		OVERLAYS = 45,
		LEAFMINDISTTOWATER = 46,
		FACE_MACRO_TEXTURE_INFO = 47,
		DISP_TRIS = 48,
		PROP_BLOB = 49,
		WATEROVERLAYS = 50,
		LEAF_AMBIENT_INDEX_HDR = 51,
		LEAF_AMBIENT_INDEX = 52,
		LIGHTING_HDR = 53,
		WORLDLIGHTS_HDR = 54,
		LEAF_AMBIENT_LIGHTING_HDR = 55,
		LEAF_AMBIENT_LIGHTING = 56,
		XZIPPAKFILE = 57,
		FACES_HDR = 58,
		MAP_FLAGS = 59,
		OVERLAY_FADES = 60,
		OVERLAY_SYSTEM_LEVELS = 61,
		PHYSLEVEL = 62,
		DISP_MULTIBLEND = 63
	};

	const std::map < LUMP, std::string > lump_names = {
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

	enum TEXINFO_FLAGS {
		SURF_LIGHT		= 0x0001,	// Value will hold the light strength
		SURF_SKY2D		= 0x0002,	// Don't draw, but add as a 2D skybox drawing surface (no 3D skybox visible)
		SURF_SKY		= 0x0004,	// Don't draw, but add as a 3D skybox drawing surface
		SURF_WARP		= 0x0008,	// Turbulent water warping effect
		SURF_TRANS		= 0x0010,	// Texture is transluscent
		SURF_NOPORTAL	= 0x0020,	// This surface can't have a portal placed on it
		SURF_TRIGGER	= 0x0040,	// Xbox hack to work around elimination of trigger surfaces, which breaks occludars. Probably ignore this flag.
		SURF_NODRAW		= 0x0080,	// Don't bother referencing the texture, or drawing one
		SURF_HINT		= 0x0100,	// Make a primary BSP splitter face/plane
		SURF_SKIP		= 0x0200,	// Completely ignore, allowing non-closed brushes
		SURF_NOLIGHT	= 0x0400,	// Don't calculate light
		SURF_BUMPLIGHT	= 0x0800,	// Calculate 3 lightmaps for the surface, for bumpmapping purposes
		SURF_NOSHADOWS	= 0x1000,	// Don't receive shadows
		SURF_NODECALS	= 0x2000,	// Don't receive decals
		SURF_NOCHOP		= 0x4000,	// Don't subdivide patches on this surface
		SURF_HITBOX		= 0x8000	// This surface is part of a hitbox
	};

	enum BRUSH_CONTENTS_FLAGS {
		CONTENTS_EMPTY					= 0x00000000,	// No contents, at all
		CONTENTS_SOLID					= 0x00000001,	// Solid; eye is never valid here
		CONTENTS_WINDOW					= 0x00000002,	// Transluscent (glass, not water)
		CONTENTS_AUX					= 0x00000004,	// 
		CONTENTS_GRATE					= 0x00000008,	// Alpha-tested "grate" textures. Bullets/sight pass through, solids do not.
		CONTENTS_SLIME					= 0x00000010,	// 
		CONTENTS_WATER					= 0x00000020,	// 
		CONTENTS_MIST					= 0x00000040,	// 
		CONTENTS_OPAQUE					= 0x00000080,	// Block AI line-of-sight
		CONTENTS_TESTFOGVOLUME			= 0x00000100,	// Volumes that cannot be seen through (possibly non-solid)
		CONTENTS_UNUSED					= 0x00000200,	// Unused
		CONTENTS_UNUSED6				= 0x00000400,	// Unused
		CONTENTS_TEAM1					= 0x00000800,	// Per-team contents used to differentiate collisions between 
		CONTENTS_TEAM2					= 0x00001000,	//  players and objects on different teams
		CONTENTS_IGNORE_NODRAW_OPAQUE	= 0x00002000,	// Ignore CONTENTS_OPAQUE on surfaces that have SURF_NODRAW
		CONTENTS_MOVEABLE				= 0x00004000,	// Hits entities which are MOVETYPE_PUSH (doors, platforms, etc.)
		CONTENTS_AREAPORTAL				= 0x00008000,	// All remaining contents are non-visible, and don't eat brushes
		CONTENTS_PLAYERCLIP				= 0x00010000,	// 
		CONTENTS_MONSTERCLIP			= 0x00020000,	// 
		CONTENTS_CURRENT_0				= 0x00040000,	// { //Currents can be added to any other contents, may be mixed (what are they though?)
		CONTENTS_CURRENT_90				= 0x00080000,	//  |
		CONTENTS_CURRENT_180			= 0x00100000,	//  |
		CONTENTS_CURRENT_270			= 0x00200000,	//  |
		CONTENTS_CURRENT_UP				= 0x00400000,	//  |
		CONTENTS_CURRENT_DOWN			= 0x00800000,	// }
		CONTENTS_ORIGIN					= 0x01000000,	// Removed before bsping an entity
		CONTENTS_MONSTER				= 0x02000000,	// Monster? This should never be on a brush, only in game
		CONTENTS_DEBRIS					= 0x04000000,	// 
		CONTENTS_DETAIL					= 0x08000000,	// Brushes to be added after vis leafs
		CONTENTS_TRANSLUCENT			= 0x10000000,	// Auto-set if any surface has transparency
		CONTENTS_LADDER					= 0x20000000,	// 
		CONTENTS_HITBOX					= 0x40000000	// Use accurate hitboxes on physics/damage trace
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
}
#endif /* BSP_LUMPS_H */