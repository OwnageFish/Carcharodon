#ifndef BSP_VIEWER_H
#define BSP_VIEWER_H

//#include "BspProcessor.h"
#include <string>
#include <fstream>

// This will use opengl and/or imgui to generate a preview of the bsp
//	I don't know how any of this works :)
//	Probably just start with imgui and try to make a nice little box :)
/*class BspViewer {
public:
	BspViewer();
	BspViewer(std::string file_name);

	// Wrapper functions :)
	void open(std::string file_name);
	void open();

	void render();
	void render(std::string file_name);

	~BspViewer();
private:
	std::string m_file_name;
	std::ifstream m_file_ptr;
	BspProcessor m_map_data;

	// Reads the current file
	//	Will use BspProcessor.h stuff here.
	void read();

};*/


#endif /* BSP_VIEWER_H */