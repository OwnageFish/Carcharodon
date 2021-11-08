

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