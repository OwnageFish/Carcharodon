#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"



class ScreenFramebuffer {

public:

	// Screen space quad for rendering stuff to
	const float screenQuad[16] = {	// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions	// texCoords
		-1.0f, 1.0f,	0.0f, 1.0f,
		-1.0f, -1.0f,	0.0f, 0.0f,
		1.0f,  -1.0f,	1.0f, 0.0f,
		1.0f,  1.0f,	1.0f, 1.0f };
	GLuint vaoScreenQuad, vboScreenQuad;
	
	// Initial render target, with different resolution than current screen, and multisampling
	unsigned int samples;
	float renderResPercent;
	GLuint multi_fb, multi_texture, multi_rb;

	// Screen-based render target on which post-process effects are applied
	GLuint postproc_fb, postproc_texture, postproc_rb;

	GLuint scWidth, scHeight;

	ScreenFramebuffer( unsigned int numSamples, GLuint screenWidth, GLuint screenHeight )
	{
		samples = numSamples;
		scWidth = screenWidth;
		scHeight = screenHeight;

		glGenVertexArrays(1, &vaoScreenQuad);
		glBindVertexArray(vaoScreenQuad);
		glGenBuffers(1, &vboScreenQuad);
		glBindBuffer(GL_ARRAY_BUFFER, vboScreenQuad);
		glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuad), &screenQuad, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glGenFramebuffers(1, &multi_fb);
		glBindFramebuffer(GL_FRAMEBUFFER, multi_fb);

		glGenTextures(1, &multi_texture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multi_texture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, scWidth, scHeight, GL_TRUE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multi_texture, 0);

		glGenRenderbuffers(1, &multi_rb);
		glBindRenderbuffer(GL_RENDERBUFFER, multi_rb);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, scWidth, scHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, multi_rb); // now actually attach it


		glGenFramebuffers(1, &postproc_fb);
		glBindFramebuffer(GL_FRAMEBUFFER, postproc_fb);

		glGenTextures(1, &postproc_texture);
		glBindTexture(GL_TEXTURE_2D, postproc_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scWidth, scHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postproc_texture, 0);

		glGenRenderbuffers(1, &postproc_rb);
		glBindRenderbuffer(GL_RENDERBUFFER, postproc_rb);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, scWidth, scHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, postproc_rb);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Recreate( GLuint newScWidth, GLuint newScHeight) {

		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multi_texture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, newScWidth, newScHeight, GL_TRUE);
		glBindRenderbuffer(GL_RENDERBUFFER, multi_rb);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, newScWidth, newScHeight);
		glBindTexture(GL_TEXTURE_2D, postproc_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newScWidth, newScHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindRenderbuffer(GL_RENDERBUFFER, postproc_rb);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newScWidth, newScHeight);

		scWidth = newScWidth;
		scHeight = newScHeight;
	}

	~ScreenFramebuffer() {

	}
};