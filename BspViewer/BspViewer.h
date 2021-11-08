#ifndef BSP_VIEWER_H
#define BSP_VIEWER_H

#include "BspProcessor.h"

// This will use opengl and/or imgui to generate a preview of the bsp
//	I don't know how any of this works :)
//	Probably just start with imgui and try to make a nice little box :)
class BspViewer {
public:
	BspViewer();
	BspViewer(std::string file_name);

	void open(std::string file_name);
	void open();

	void render();
	void render(std::string file_name);

	~BspViewer();
private:
	BspProcessor m_map_data;

};


#endif /* BSP_VIEWER_H */