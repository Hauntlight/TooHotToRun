#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader_m.h"
#include "mesh.h"
#include "Utilities.h"

//Questo header contiene gli "effetti speciali/particellari"
//Abbiamo la definizione di Particle, dello SmokeGenerator e dell'Esplosione

class Particle {
public:
	float life;
	float offset;
	float altezza = 1.0f;
	float spostamento = 0.0f;
	float alpha = 0.3f;
	
	Particle() :  offset(0.0f),life(1.5f) { }

	Particle(float f,float posizione) : offset(posizione),life(f),altezza(0.5f){}
};

const int MAX_SMOKE = 75;//Definisco il numero massimo di Particles

class SmokeGenerator
{

public: 
	Model modello;
	Particle particles[MAX_SMOKE];
	int amount;

	SmokeGenerator() {
	
	}

	SmokeGenerator(string path,int amount,float posizione,bool stat = false) {
		modello = Model(path);
		this->amount = amount;
		for (int i = 0; i < amount; i++) {
			particles[i]=Particle(1.0f + RandomNumber(-0.2, 0.2), posizione+RandomNumber(-0.2, 0.2)); //Riempio il vettore particles
		}
	}

	void update(float dt, float velocitaNormalizzata, float posizione) {
			for (int i = 0; i < amount; i++) {
				if (particles[i].life>0) {
					particles[i].altezza += 1.0 * dt;
					particles[i].spostamento += 1.0 * dt+velocitaNormalizzata;
					particles[i].alpha -= dt *0.075* particles[i].life;
					particles[i].life -= dt;
				}
				else { //Qui usiamo un trucco, piuttosto che eliminare le particles a fine vita le spostiamo vicino al bot
					particles[i].spostamento = 0.0f + RandomNumber(-0.2, 0.2);
					particles[i].altezza = 1.0f+ RandomNumber(-0.2, 0.2);
					particles[i].alpha = 0.1f;
					particles[i].life = RandomNumber(1.2, 1.8);
					particles[i].offset = posizione+ RandomNumber(-0.2,0.2);
				}

			}
	}

	//Qui viene passato un BasicShader
	void draw(Shader shader) {
		for (int i = 0; i < amount; i++) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, 9.89f, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(-particles[i].offset, particles[i].altezza, -particles[i].spostamento-RandomNumber(-0.5,0.5)));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	
			shader.setFloat("alpha", particles[i].alpha);
			shader.setMat4("model", model);
			modello.Draw(shader);
			glDisable(GL_BLEND);
		}

	}


};


//Qui abbiamo solo una "Particle", che fondamentalmente è il modello dell'esplosione
class ExplosionGenerator
{

public:
	Model modello;
	Particle particle;
	bool status = false;

	ExplosionGenerator() {

	}

	ExplosionGenerator(string path, float posizione, bool stat = false) {
		modello = Model(path);
		particle = Particle(4.0f, posizione);
		particle.alpha = 0.8f;
		particle.altezza = 0.5f;
		status = stat;
	}

	bool update(float dt, float velocitaNormalizzata, float posizione) { //Restituisce un bool e ci informa di quando l'animazione dell'esplosione è terminata

		if (particle.life > 0) {
				particle.offset = posizione + RandomNumber(-0.01, 0.01);
				if (status == true) {
					particle.altezza += 0.1 * dt;
					particle.alpha -= dt * 0.075 * (particle.life/4.0f);
					particle.life -= dt;
				}
				return false;
			}
		return true;

	}
	
	//Qui viene passato un BasicShader
	void draw(Shader shader) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, 9.89f, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(-particle.offset, particle.altezza, 0.0f));
			model = glm::scale(model, glm::vec3(0.010f-particle.life/800.0f, 0.010f - particle.life / 800.0f, 0.010f - particle.life / 800.0f));
			shader.setFloat("alpha", particle.alpha);
			shader.setMat4("model", model);
			modello.Draw(shader);
			glDisable(GL_BLEND);
	}


};
