#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader_m.h"
#include "mesh.h"
#include "soundManager.h"

//File con funzioni e variabili di Utilità generica

glm::vec3 camPos = glm::vec3(3.15f, 6.0f, 6.27f);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

SoundManager soundManager = SoundManager();
std::string soundMusic = "resources/sounds/endless-music.wav";
std::string fxMusic = "resources/sounds/expl.wav";

const glm::vec3 RED = glm::vec3(1.0f, 0.1f, 0.1f);
const glm::vec3 BLACK = glm::vec3(0.1f, 0.1f, 0.1f);
const glm::vec3 WHITE = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 GREEN = glm::vec3(0.1f, 1.0f, 0.1f);
const glm::vec3 BLUE = glm::vec3(0.1f, 0.1f, 1.0f);

float RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

glm::vec3 getRandomColor() {
	float r = RandomNumber(0.0f, 4.0f);
	if (r <= 1.0) {
		return WHITE;
	}
	if (r <= 2.0f) {
		return RED;
	}
	if (r <= 3.0f) {
		return GREEN;
	}
	return BLUE;

}