#pragma once

#include <glew.h>
#include <glm.hpp>

#include <math.h>

#include "Shader.h"

class GridInternal {

public:

	//GridInternal * grid;
	Shader * shade;
	unsigned int VAO, VBO, EBO;
	double width, invUnits;

	float vertices[12] = {
		-65536.0, 0.0, -65536.0,
		-65536.0, 0.0,  65536.0,
		65536.0,  0.0,  65536.0,
		65536.0,  0.0, -65536.0
	};
	int indices[4] = { 0, 1, 2, 3 };

	GridInternal(float w = 1, int u = 4) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindVertexArray(0);

		shade = new Shader("Shaders/ref_grid_vert.glsl", "Shaders/ref_grid_frag.glsl");

		SetSpacing(u);
		SetLineWidth(w);
	}

	void SetSpacing(int p) {

		double units = pow(2, p);
		invUnits = 1 / units;
		shade->use();
		glUniform1f(glGetUniformLocation(shade->ID, "invUn"), invUnits);

		if (units < width) {

			width = units - 0.001;
			glUniform1f(glGetUniformLocation(shade->ID, "width"), width);
		}
	}

	void SetLineWidth(double w) {

		width = std::min ( w, 1 / invUnits - 0.001 );
		shade->use();
		glUniform1f(glGetUniformLocation(shade->ID, "width"), w);
	}

	void Draw() {

		//glDisable(GL_CULL_FACE);
		shade->use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLE_FAN, sizeof(indices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//glEnable(GL_CULL_FACE);
	}

	~GridInternal() {

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		delete shade;
	}
};