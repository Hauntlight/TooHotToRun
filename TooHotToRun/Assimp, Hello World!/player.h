#pragma once 

#ifndef PLAYER_H
#define PLAYER_H
#endif

#include "mesh.h"
#include "shader_m.h"
#include "model.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "effects.h"

//File contente le informazioni di Bob

enum Bob_Movement {
	LEFT_BOB,
	RIGHT_BOB,
	NO_DIR
};

class Player {
public:
	Model* modello;
	float temperatura;
	float velocita;
	float direzione;
	float accelerazione;
	float posizione;
	float velocitaNormalizzata;
	float punti = 0.0f;
	SmokeGenerator* smoker;
	ExplosionGenerator* explosion;
	bool dead = false;
	bool exploded = false;

	Player() {}

	Player(string s) {
		modello = new Model(s);
		temperatura = 0.0f;
		velocita = 10.0f;
		direzione = 0.0f;
		posizione = 0.0f;
		accelerazione = 0.1f;
		velocitaNormalizzata = 0;
		smoker =  new SmokeGenerator("resources/cloud/cloud.obj", 75, this->posizione);
		explosion =  new ExplosionGenerator("resources/explosion/explosion.obj", this->posizione);
	}

	void aggiornaValori() {
		velocita += (velocitaNormalizzata / velocita) * accelerazione;

		if (velocita >= 30.0f) {
			velocita = 30.0f;
		}

		if (velocita >= 15.0f && accelerazione > 0.0f) {
			temperatura += (velocitaNormalizzata / velocita) * 0.5;
		}

		if (temperatura > 50) {
			temperatura = 50;
		}

		if (temperatura < 0) {
			temperatura = 0;
		}

		punti += velocitaNormalizzata;

	}

	bool verificaSconfitta() {
		if (temperatura >= 50) {
			return true;
		}
		else if (velocita <= 5.0f) {
			return true;
		}
		return false;
	}

	void draw(Shader shader,Shader baseShader,float dt) {
		exploded = explosion->update(dt, velocitaNormalizzata, posizione);
		if (this->dead == true) {
			explosion->status = true; //dico al generatore che può inziare con l'esplosione
			baseShader.use();
			baseShader.setInt("texture_diffuse1", 0);
			explosion->draw(baseShader);
			return;
		}
		posizione =posizione+ direzione*velocitaNormalizzata;
		if (posizione <= -3.495) {
			posizione = -3.495f;
		}
		if (posizione >= 3.48) {
			posizione = 3.48f;
		}
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, (velocitaNormalizzata/velocita)*1.5f*direzione, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, posizione));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setMat4("model", model);
		modello->Draw(shader);
		smoker->update(dt, velocitaNormalizzata,posizione);
		if (temperatura >= 30.0f) {
			baseShader.use();
			baseShader.setInt("texture_diffuse1", 0);
			smoker->draw(baseShader);
		}
		aggiornaValori();
	}
	
	void ProcessKeyboard(Bob_Movement direction, float deltaTime)
	{
		velocitaNormalizzata = velocita * deltaTime;
		if (direction == LEFT_BOB && posizione >= -3.495) {
			direzione = -1;
			return;
		}
		if (direction == RIGHT_BOB && posizione <= 3.48){
			direzione = 1;
			return;
		}
		if (direction == NO_DIR)
			direzione = 0;
		direzione = 0;
	}

};