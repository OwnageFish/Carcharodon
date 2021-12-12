#ifndef SHADER_H
#define SHADER_H

#include <glew.h>
#include <glm.hpp>

#include <fstream>
#include <sstream>
#include <map>
class Shader
{
public:
	unsigned int ID;

	Shader() {}

	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
	{
		// 1. retrieve the vertex/fragment/geometry source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;

		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;

		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			vShaderFile.close();
			fShaderFile.close();

			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

			// if geometry shader path is present, also load a geometry shader
			if (geometryPath != nullptr)
			{
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		catch (std::ifstream::failure& e)
		{
			fprintf(stderr, "Shader file not successfully read. ");
			fprintf(stderr, "%s\n ", e.what());
		}

		// 2. compile shaders
		unsigned int vertex, fragment, geometry;

		const char* vShaderCode = vertexCode.c_str();
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		const char * fShaderCode = fragmentCode.c_str();
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		if (geometryPath != nullptr)
		{
			const char * gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY");
		}

		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		if (geometryPath != nullptr)
			glAttachShader(ID, geometry);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (geometryPath != nullptr)
			glDeleteShader(geometry);
	}

	void operator=(const Shader &s) {
		ID = s.ID;
	};

	~Shader()
	{
		glDeleteShader(ID);
	}



	// activate the shader
	// ------------------------------------------------------------------------
	void use()
	{
		glUseProgram(ID);
	}
	// utility uniform functions
	void setBool(const std::string &name, bool value) const
	{ glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); }

	void setInt(const std::string &name, int value) const
	{ glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }

	void setFloat(const std::string &name, float value) const
	{ glUniform1f(glGetUniformLocation(ID, name.c_str()), value); }

	void setFloatArr(const std::string &name, const float * value, int count) const
	{ glUniform1fv(glGetUniformLocation(ID, name.c_str()), count, value); }

	void setVec2(const std::string &name, const glm::vec2 &value) const
	{ glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }

	void setVec2(const std::string &name, float x, float y) const
	{ glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); }

	void setVec3(const std::string &name, const glm::vec3 &value) const
	{ glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }

	void setVec3(const std::string &name, float x, float y, float z) const
	{ glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); }

	void setVec4(const std::string &name, const glm::vec4 &value) const
	{ glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }

	void setVec4(const std::string &name, float x, float y, float z, float w)
	{ glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); }

	void setIVec3(const std::string &name, const glm::ivec3 &value) const
	{ glUniform3iv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }

	void setIVec3(const std::string &name, int x, int y, int z) const
	{ glUniform3i(glGetUniformLocation(ID, name.c_str()), x, y, z); }

	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{ glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{ glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{ glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				GLint logSize = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				fprintf(stderr, "Shader compilation error of type %s\n >> %s\n -- --------------------------------------------------- -- \n", type.c_str(), infoLog);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				fprintf(stderr, "Shader program linking error of type %s\n >> %s\n -- --------------------------------------------------- -- \n", type.c_str(), infoLog);
			}
		}
	}
};
#endif