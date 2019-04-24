#pragma once
#ifndef Textures
#define Textures

#include <iostream>
#include <string>

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>

class Texture
{
private:
	GLuint texture;
	int width;
	int height;
	unsigned int type;

public:

	Texture(const char* fileName, GLenum type)
	{
		this->type = type;

		unsigned char *image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);

		if (image) {
			glGenTextures(1, &texture);
			glBindTexture(type, texture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "ERROR::FAILED TO LOAD IMAGE FILE" << std::endl;
		}
		SOIL_free_image_data(image);
		glActiveTexture(0);
		glBindTexture(type, 0);
	}

	GLuint get_tex() { return this->texture; }

	void bind(GLuint id, GLuint shader_program) {
		//activates the 0 + id texture unit
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(this->type, texture);
		//tells the program to sample pixel color from the texture unit corresponding to the second argument
		glUniform1i(glGetUniformLocation(shader_program, "ourTexture"), 0 + id);
	}
	
	void unbind() {
		glActiveTexture(0);
		glBindTexture(type, 0);
	}



	void load_from_file(const char* fileName) {
		if (texture)
		{
			glDeleteTextures(1, &texture);
		}
		else
		{
			unsigned char *image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);
			glGenTextures(1, &texture);
			glBindTexture(type, texture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);

			SOIL_free_image_data(image);
			glActiveTexture(0);
			glBindTexture(type, 0);
		}
	}

	~Texture() {
		glDeleteTextures(1, &texture);
	}

};

#endif // !Textures

