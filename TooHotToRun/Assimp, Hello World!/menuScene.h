#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>
#include "shader_m.h"

//File che gestisce la scena di titleScreen

float vertices[] = {
	// positions          // colors           // texture coords
	1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};



class MenuScene {

	unsigned int VBO, VAO, EBO;
	unsigned int texture;
	glm::vec3 color = getRandomColor();
	float colorLife = 2.0f;
	
	void loadtexture(std::string filename)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
																						// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}

	void prepare(Shader& myShader) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		myShader.use();
		myShader.setInt("texture_diffuse1", 0);
		myShader.setFloat("alpha", 1.0f);
		glm::mat4 model = glm::mat4(1.0f);//Anche stavolta devo muovere il quadrato davanti la camera, problema dovuto a scelte implementative precedenti
		model = glm::rotate(model, glm::radians(27.35f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 6.27f));
		model = glm::scale(model, glm::vec3(2.0f, 1.5f, 1.0f));

		myShader.setMat4("model", model);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	
	}

public:

	MenuScene() {}

	void init(std::string filename) {
		loadtexture(filename);
	}

	void render(Shader& shader,float deltaTime) {
		prepare(shader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDisable(GL_BLEND);
		if (colorLife <= 0) {
			color = getRandomColor();
			colorLife = 2.0f;
		}
		colorLife -= deltaTime;
		RenderText(string("Premi SpaceBar per iniziare una nuova partita").c_str(), SCR_WIDTH * (1.0f / 9.0f), SCR_HEIGHT*(1.0f / 5.0f), 1.0f, color);
		RenderText(string("Premi H per leggere le istruzioni").c_str(), SCR_WIDTH * (1.0f / 4.0f), SCR_HEIGHT * (1.0f / 8.0f), 1.0f, color);
	}





};