#pragma once

#include <glew.h>
#include <glm.hpp>

#include <math.h>

#include <algorithm>
#include <numeric>

#include "Shader.h"
#include "BspProcessor.h"

class BspPlane {

public:

	//GridInternal * grid;
	//Shader* shade;
	unsigned int VAO, VBO, EBO;

	std::vector < int > indxs;
	std::vector < float > verts;

	BspPlane() {}

	BspPlane(const BspProcessor& b, const std::size_t& f_idx) { Generate(b, f_idx); }

	void Generate(const BspProcessor& b, const std::size_t& f_idx) {

		// Our index list is just a vector of numbers counting from 0 upwards
		// We are guaranteed to be drawing a convex shape with number of sides defined by face's num_edges
		//  and we are ordering them according to offsets into our generated vertex array.
		indxs.resize(b.m_faces[f_idx].num_edges);
		for (std::size_t it_indxs = 0; it_indxs < b.m_faces[f_idx].num_edges; it_indxs++)
			indxs[it_indxs] = b.m_faces[f_idx].num_edges - it_indxs - 1;

		// If we want the winding order to be in the other direction (we don't ever, I think?), then this is a one liner that does this
		//	std::iota(indxs.begin(), indxs.end(), 0);

		// Determine first and second vert for first edge
		int32_t cur_edge = b.m_surfedges[b.m_faces[f_idx].first_edge].edge;
		uint16_t past_second = (cur_edge >= 0 ? b.m_edges[cur_edge].vert[1] : b.m_edges[-cur_edge].vert[0]);
		uint16_t this_second = (cur_edge >= 0 ? b.m_edges[cur_edge].vert[0] : b.m_edges[-cur_edge].vert[1]);

		// Start accumulating vertexes specified by edges, by pushing them back from the vertex lump into our vertex vector
		//std::vector < uint16_t > inter_vert_indxs;
		//inter_vert_indxs.push_back(this_second);
		verts.push_back(-b.m_vertexes[this_second].x); verts.push_back(b.m_vertexes[this_second].z); verts.push_back(b.m_vertexes[this_second].y);
		for (std::size_t ie = 1; ie < b.m_faces[f_idx].num_edges; ie++)
		{
			// Determine successive edge's second vertex. If it is equal to the previous edge's second vertex, then winding order is specified incorrectly, and an error is tallied.
			std::size_t ie_ind = ie + abs(b.m_faces[f_idx].first_edge);
			cur_edge = b.m_surfedges[ie_ind].edge;
			this_second = (cur_edge >= 0 ? b.m_edges[cur_edge].vert[1] : b.m_edges[-cur_edge].vert[0]);
			//inter_vert_indxs.push_back(past_second);
			verts.push_back(-b.m_vertexes[past_second].x); verts.push_back(b.m_vertexes[past_second].z); verts.push_back(b.m_vertexes[past_second].y);
			past_second = this_second;
		}

		// Our vertex list is a vector of floats given by indexes in the inter_vert_indxs vector
		/*verts.reserve(b.m_faces[f_idx].num_edges * 3);
		for (uint16_t vert_idx : inter_vert_indxs)
		{
			verts.push_back(b.m_vertexes[vert_idx].x); verts.push_back(b.m_vertexes[vert_idx].y); verts.push_back(b.m_vertexes[vert_idx].z);
		}*/

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), &verts[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indxs.size(), &indxs[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindVertexArray(0);

		//shade = new Shader("Shaders/bsp_plane_vert.glsl", "Shaders/bsp_plane_frag.glsl");
	}

	void Draw() {

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLE_FAN, sizeof(int) * indxs.size (), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	~BspPlane() {

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
};