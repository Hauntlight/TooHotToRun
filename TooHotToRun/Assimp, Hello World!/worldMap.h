#pragma once
#include <vector>
#include "istanziatore.h"
#include "gestoreLuci.h"

//File che gestisce la mappa di gioco

class WorldMap {
public:


	IstanziatorePareti* istanziatorePareti;
	IstanziatorePavimento* istanziatorePavimento;
	IstanziatoreOggetti* istanziatoreOstacolo;
	IstanziatoreOggetti* istanziatoreOstacoloLuminoso;
	IstanziatoreOggetti* istanziatoreBuffFiamma;
	IstanziatoreOggetti* istanziatoreBuffGhiaccio;

	WorldMap() {
		istanziatorePavimento = new IstanziatorePavimento();
		istanziatorePareti = new IstanziatorePareti();
		istanziatoreOstacolo = new IstanziatoreOggetti("resources/75-mgs_barriers/barrier1.obj",9.89f,glm::vec3(3.75f, 00.0f, 1.0f), glm::vec3(1.0f, 00.0f, 1.0f),glm::vec3(-1.75f, 00.0f, 1.0f), glm::vec3(0.5f, 1.0f, 1.0f),10.0f);
		istanziatoreOstacoloLuminoso = new IstanziatoreOggetti("resources/lamp/lamp.obj", 0.465f,glm::vec3(-1.5f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, -1.0f),glm::vec3(1.5f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f),10.0f);
		istanziatoreBuffFiamma = new IstanziatoreOggetti("resources/fireEmblem/fire.obj", 0.465f, glm::vec3(-2.25f, 0.5f, -1.0f), glm::vec3(0.0f, 0.5f, -1.0f), glm::vec3(2.25f, 0.5f, -1.0f), glm::vec3(0.5f, 0.5f, 0.5f),50.0f);
		istanziatoreBuffGhiaccio = new IstanziatoreOggetti("resources/iceEmblem/ice.obj", 0.465f, glm::vec3(-2.25f, 0.5f, -1.0f), glm::vec3(0.0f, 0.5f, -1.0f), glm::vec3(2.25f, 0.5f, -1.0f), glm::vec3(0.5f, 0.5f, 0.5f),50.0f);
		//tutti i vettori sono stati trovati empiricamente
	}

	void render(float speed, GestoreLuci& lights) {

		if (istanziatorePavimento->updatePosizioni(-speed)) { //se sto generando una nuova riga di pavimento
			oggetto_mov mov = NO_LINE;
			if (RandomNumber(0.0f, 1.0f)>=0.5) {
				mov = istanziatoreOstacoloLuminoso->generaOstacolo(); //genero Ostacolo luminoso
				if (mov != NO_LINE && lights.listaL.indicatore < MAX_LIGHTS) { //Se ho generato un ostacolo luminoso e posso creare altre luci
					if (mov == LEFT_O) {
						lights.addLuce(glm::vec3(-1.5f,1.0f,-50.0f));
					}else if (mov == RIGHT_O) {
						lights.addLuce(glm::vec3(0.0f, 1.0f, -50.0f));
					}else if (mov == CENTER_O) {
						lights.addLuce(glm::vec3(1.5f, 1.0f, -50.0f));
					}
				}
			}
			else {
				mov = istanziatoreOstacolo->generaOstacolo();
			}

			if (RandomNumber(0.0f, 1.0f) >= 0.5) {//genero buff in modo da non sovrappormi agli ostacoli
				istanziatoreBuffFiamma->generaOstacoloBasedOnOtherPosition(mov);
			}
			else {
				istanziatoreBuffGhiaccio->generaOstacoloBasedOnOtherPosition(mov);
			}
			
		}

		lights.updatePosizioni(-speed); //muovo le luci con gli ostacoli luminosi
		istanziatorePareti->updatePosizioni(-speed); //muovo le pareti
		istanziatorePareti->updateIstanze(); //aggiorno la matrice delle istanze nel VAO 
		istanziatorePavimento->updateIstanze();

		istanziatoreOstacolo->updatePosizioni(-speed); //muovo l'ostacolo
		istanziatoreOstacoloLuminoso->updatePosizioni(-speed);
		istanziatoreBuffGhiaccio->updatePosizioni(-speed);//muovo il buff
		istanziatoreBuffFiamma->updatePosizioni(-speed);
		istanziatoreOstacolo->updateIstanze();
		istanziatoreOstacoloLuminoso->updateIstanze();
		istanziatoreBuffGhiaccio->updateIstanze();
		istanziatoreBuffFiamma->updateIstanze();

		//disegno gli elementi aggiornati
		istanziatorePareti->draw();
		istanziatorePavimento->draw();
		istanziatoreOstacolo->draw();
		istanziatoreOstacoloLuminoso->draw();
		istanziatoreBuffGhiaccio->draw();
		istanziatoreBuffFiamma->draw();

	}


	//tutti i valori sono stati trovati empiricamente
	bool verificaCollisione(Player* bob) {
		//Collisioni con ostacolo 1 
	//							Posizione Centrale -1.63f <-> 1.53f 
	//							Posizione Sinistra -3.51f <-> -1.30f
	//							Posizione Destra	1.20f <-> 3.51f
	//							Verticalmente verifichiamo La collisione se La posizione dell'ostacolo è compresa tra : -0.2f <-> 0.5f
		for (int i = 0; i < istanziatoreOstacolo->getNumeroIstanze(); i++) {
			if (istanziatoreOstacolo->oggetti[i].posizione <= 0.5 && istanziatoreOstacolo->oggetti[i].posizione >= -0.2) {
				if (istanziatoreOstacolo->oggetti[i].linea == LEFT_O && (bob->posizione >= -3.51 && bob->posizione <= -1.30f)) {
					return true;
				}
				else if (istanziatoreOstacolo->oggetti[i].linea == CENTER_O && (bob->posizione >= -1.63 && bob->posizione <= 1.53f)) {
					return true;
				}
				else if (istanziatoreOstacolo->oggetti[i].linea == RIGHT_O && (bob->posizione >= 1.20 && bob->posizione <= 3.51f)) {
					return true;
				}
			}
		}

		//Collisioni con buffs
		//							Posizione Centrale -0.97f <-> 1.03f 
		//							Posizione Sinistra -3.20f <-> -1.20f
		//							Posizione Destra	1.20f <-> 3.20f
		//							Verticalmente verifichiamo La collisione se La posizione dell'ostacolo è compresa tra : -0.2f <-> 0.5f
		int indice = -1;
		for (int i = 0; i < istanziatoreBuffGhiaccio->getNumeroIstanze(); i++) {
			if (istanziatoreBuffGhiaccio->oggetti[i].posizione <= 0.5 && istanziatoreBuffGhiaccio->oggetti[i].posizione >= -0.2) {
				if (istanziatoreBuffGhiaccio->oggetti[i].linea == LEFT_O && (bob->posizione >= -3.20f && bob->posizione <= -1.20f)) {
					indice = i;
				}
				else if (istanziatoreBuffGhiaccio->oggetti[i].linea == CENTER_O && (bob->posizione >= -0.97f && bob->posizione <= 1.03f)) {
					indice = i;
				}
				else if (istanziatoreBuffGhiaccio->oggetti[i].linea == RIGHT_O && (bob->posizione >= 1.20f && bob->posizione <= 3.20f)) {
					indice = i;
				}
			}
		}

		if (indice != -1) {
			bob->velocita -= 1.0f;
			bob->accelerazione = -0.15f;
			bob->temperatura -= 10;
			istanziatoreBuffGhiaccio->eraseObject(indice);
		}

		indice = -1;
		for (int i = 0; i < istanziatoreBuffFiamma->getNumeroIstanze(); i++) {
			if (istanziatoreBuffFiamma->oggetti[i].posizione <= 0.5 && istanziatoreBuffFiamma->oggetti[i].posizione >= -0.2) {
				if (istanziatoreBuffFiamma->oggetti[i].linea == LEFT_O && (bob->posizione >= -3.20f && bob->posizione <= -1.20f)) {
					indice = i;
				}
				else if (istanziatoreBuffFiamma->oggetti[i].linea == CENTER_O && (bob->posizione >= -0.97f && bob->posizione <= 1.03f)) {
					indice = i;
				}
				else if (istanziatoreBuffFiamma->oggetti[i].linea == RIGHT_O && (bob->posizione >= 1.20f && bob->posizione <= 3.20f)) {
					indice = i;
				}
			}
		}

		if (indice != -1) {
			bob->velocita += 2.5f;
			bob->accelerazione = 0.1f;
			bob->temperatura += 5;
			istanziatoreBuffFiamma->eraseObject(indice);
		}

		//Collisioni con ostacolo 2
//							Posizione Centrale -0.85f <-> 0.95f 
//							Posizione Sinistra -2.40f <-> -0.35f
//							Posizione Destra	0.65f <-> 2.48f
//							Verticalmente verifichiamo La collisione se La posizione dell'ostacolo è compresa tra : -0.2f <-> 0.5f
		for (int i = 0; i < istanziatoreOstacoloLuminoso->getNumeroIstanze(); i++) {
			if (istanziatoreOstacoloLuminoso->oggetti[i].posizione <= 0.5 && istanziatoreOstacoloLuminoso->oggetti[i].posizione >= -0.2) {
				if (istanziatoreOstacoloLuminoso->oggetti[i].linea == LEFT_O && (bob->posizione >= -2.40 && bob->posizione <= -0.35f)) {
					return true;
				}
				else if (istanziatoreOstacoloLuminoso->oggetti[i].linea == CENTER_O && (bob->posizione >= -0.85 && bob->posizione <= 0.95f)) {
					return true;
				}
				else if (istanziatoreOstacoloLuminoso->oggetti[i].linea == RIGHT_O && (bob->posizione >= 0.65 && bob->posizione <= 2.48f)) {
					return true;
				}
			}
		}
		return false;
	}



};