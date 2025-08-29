#pragma once
#include "Utilities.h"
#include "player.h"
#include "worldMap.h"
#include "gestoreLuci.h"
#include <glad/glad.h> 
#include "mesh.h"
#include "shader_m.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

//File Che gestisce la partita

class GameScene {
	Player* bob;
	WorldMap* world;
	GestoreLuci lights;

public:
	GameScene() {

	}

	//Inizializzo i componenti
	void init() {
		bob = new Player("resources/bot/RobotSentinel.obj");
		world = new WorldMap();
		lights.init();
	}

	void play(Shader& modelShader, Shader& instancingShader,Shader& basicShader,Bob_Movement mov,float deltaTime) {
		
		modelShader.use();
		lights.updateShader(modelShader);

		bob->ProcessKeyboard(mov, deltaTime);

		if (!bob->exploded) {//Verifico se bob è esploso
			bob->draw(modelShader, basicShader, deltaTime);
		}
		else {
			soundManager.stopMusic();
			RenderText(("Punteggio: " + std::to_string((int)(bob->punti + 0.5f))).c_str(), SCR_WIDTH * (1.0f / 3.0f), SCR_HEIGHT - SCR_HEIGHT / 2, 1.0f, glm::vec3(0.1f, 1.0f, 0.1f));
		}

		instancingShader.use();
		lights.updateShader(instancingShader);

		world->render(bob->velocitaNormalizzata, lights);
		if (world->verificaCollisione(bob) || bob->verificaSconfitta()) {//Se bob è stato sconfitto lo informo che può iniziare l'animazione dell'esplosione al prossimo frame
			soundManager.playMusic(fxMusic);
			bob->accelerazione = 0;
			bob->velocita = 0;
			bob->dead = true;
		}

		//Stampe dei dati
		if (bob->temperatura < 40.0f) {
			RenderText(("Temperatura: " + std::to_string(bob->temperatura)).c_str(), 0, SCR_HEIGHT - 50, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
		}
		else {
			RenderText(("Temperatura: " + std::to_string(bob->temperatura)).c_str(), 0, SCR_HEIGHT - 50, 0.5f, glm::vec3(1.0f, 0.1f, 0.1f));
		}
		RenderText(("Velocita: " + std::to_string(bob->velocita)).c_str(), 0, SCR_HEIGHT - 70, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
		RenderText(("Punteggio: " + std::to_string((int)(bob->punti + 0.5f))).c_str(), 0, SCR_HEIGHT - 90, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));


	}

};