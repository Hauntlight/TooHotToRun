#pragma once
#include "mesh.h"
#include "shader_m.h"
#include "model.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "Utilities.h"



//Questo file contiene gli instancer



class IstanziatorePareti {
	Model* modello;
	vector<float> posizioni;
	int numeroIstanze;
	int posizioneMax;
public:
	IstanziatorePareti(){
		modello = new Model("resources/Corridor/Meshes/LowPoly/SM_Wall_BA.obj");
		numeroIstanze = 21;
		posizioni.reserve(numeroIstanze);
		inizializzaPosizioni();
		updateIstanze();
	}

	void inizializzaPosizioni() {
		float j = -3.0f;
		for (float i = 0.0; i < numeroIstanze; i++) {
			posizioni[i] = j;
			j += 3.0f;
		}
		posizioneMax = numeroIstanze - 1;
	}

	void updatePosizioni(float s) {
		int eliminare = -1;
		for (float i = 0.0; i < numeroIstanze; i++) {
			posizioni[i] += s;
			if (posizioni[i] > posizioni[posizioneMax]) {
				posizioneMax = i;
			}
			if (posizioni[i] < -5) {
				eliminare = i;
			}
		}

		if (eliminare != -1) { //piuttosto che eliminare sposto le pareti in fondo
			posizioni[eliminare] = posizioni[posizioneMax] + 3.0f;
			posizioneMax = eliminare;
		}
	}

	void draw() {


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, modello->textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
		for (unsigned int i = 0; i < modello->meshes.size(); i++)
		{
			glBindVertexArray(modello->meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, modello->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, numeroIstanze*2);
			glBindVertexArray(0);
		}
	}

	void updateIstanze() {

		glm::mat4* arrayModelli;

		arrayModelli = new glm::mat4[numeroIstanze * 2];

		int indicatore = 0;

		for (unsigned int i = 0; i < numeroIstanze * 2; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);

			if (i % 2 == 0) {

				model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::translate(model, glm::vec3(posizioni[indicatore], 00.0f, -4.75f)); // translate it down so it's at the center of the scene
				model = glm::scale(model, glm::vec3(75.0f, 150.0f, 150.0f));
			}
			else {

				model = glm::rotate(model, -45.09f, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::translate(model, glm::vec3(-posizioni[indicatore], 00.0f, -4.75f)); // translate it down so it's at the center of the scene
				model = glm::scale(model, glm::vec3(75.0f, 150.0f, 150.0f));
				indicatore++;
			}			// 4. now add to list of matrices
			arrayModelli[i] = model;
		}

		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, numeroIstanze * 2 * sizeof(glm::mat4), &arrayModelli[0], GL_STATIC_DRAW);


		for (unsigned int i = 0; i < modello->meshes.size(); i++) {

			unsigned int VAO = modello->meshes[i].VAO;
			glBindVertexArray(VAO);
			// set attribute pointers for matrix (4 times vec4)
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);


		}
	}


};


class IstanziatorePavimento  {
	Model* modello;
	vector<float> posizioni;
	int numeroIstanze;
	int posizioneMax;
public:
	IstanziatorePavimento() {
		modello = new Model("resources/Corridor/Meshes/Objs/floor/floor.obj");
		numeroIstanze = 21;
		posizioni.reserve(numeroIstanze);
		inizializzaPosizioni();
		updateIstanze();
	}

	void inizializzaPosizioni() {
		float j = -3.0f;
		for (float i = 0.0; i < numeroIstanze; i++) {
			posizioni[i] = j;
			j += 3.0f;
		}
		posizioneMax = numeroIstanze - 1;
	}

	bool updatePosizioni(float s) {
		int eliminare = -1;
		for (float i = 0.0; i < numeroIstanze; i++) {
			posizioni[i] += s;
			if (posizioni[i] > posizioni[posizioneMax]) {
				posizioneMax = i;
			}
			if (posizioni[i] < -5) {
				eliminare = i;
			}
		}

		if (eliminare != -1) { //piuttosto che eliminare sposto i pavimenti in fondo
			posizioni[eliminare] = posizioni[posizioneMax] + 3.0f;
			posizioneMax = eliminare;
			return true;
		}
		return false;
	}

	void draw() {


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, modello->textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
		for (unsigned int i = 0; i < modello->meshes.size(); i++)
		{
			glBindVertexArray(modello->meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, modello->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, numeroIstanze * 3);
			glBindVertexArray(0);
		}
	}

	void updateIstanze() {

		glm::mat4* arrayModelli;

		arrayModelli = new glm::mat4[numeroIstanze * 3];

		int indicatore = 0;
		int last = 1;

		for (unsigned int i = 0; i < numeroIstanze * 3; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);

			if (last == 1) {
				model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::translate(model, glm::vec3(posizioni[indicatore], 0, -3.0f));
				model = glm::scale(model, glm::vec3(150.0f, 150.0f, 150.0f));
				last = -1;
			}
			else if (last == -1) {
				model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::translate(model, glm::vec3(posizioni[indicatore], 0, 0.0f));
				model = glm::scale(model, glm::vec3(150.0f, 150.0f, 150.0f));
				last = 0;
			}
			else {
				model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::translate(model, glm::vec3(posizioni[indicatore], 0, 3.0f));
				model = glm::scale(model, glm::vec3(150.0f, 150.0f, 150.0f));
				last = 1;
				indicatore++;
			}
			arrayModelli[i] = model;
		}

		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, numeroIstanze * 3 * sizeof(glm::mat4), &arrayModelli[0], GL_STATIC_DRAW);


		for (unsigned int i = 0; i < modello->meshes.size(); i++) {

			unsigned int VAO = modello->meshes[i].VAO;
			glBindVertexArray(VAO);
			// set attribute pointers for matrix (4 times vec4)
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);


		}
	}


};

enum oggetto_mov {
	LEFT_O, //indica un oggetto a sinistra
	RIGHT_O,//indica un oggetto a destra
	CENTER_O,//indica un oggetto al centro
	NO_LINE //indica che non è stato creato nessun oggetto
};

struct Oggetto {
	oggetto_mov linea; //posizione (L-C-R)
	float posizione; // posizione sull'asse
};

class IstanziatoreOggetti {
	Model* modello;
	vector<float> posizioni;
	int numeroIstanze;
	int posizioneMax;
public:

	vector<Oggetto> oggetti;
	float probabilita;
	float rotation;
	glm::vec3 translationLeft;
	glm::vec3 translationCenter;
	glm::vec3 translationRight;
	glm::vec3 scaling;


	int getNumeroIstanze() {
		return numeroIstanze;
	}

	IstanziatoreOggetti() {
		modello = new Model("resources/75-mgs_barriers/barrier1.obj");
		numeroIstanze = 0;
		oggetti.reserve(1);
		probabilita = 10.0f;
		rotation = 0.0f;
		translationLeft = glm::vec3(1.0f);
		translationCenter = glm::vec3(1.0f);
		translationRight = glm::vec3(1.0f);
		scaling = glm::vec3(1.0f);
	}

	IstanziatoreOggetti(string s) {
		modello = new Model(s);
		numeroIstanze = 0;
		oggetti.reserve(1);
		probabilita = 10.0f;
		rotation = 0.0f;
		translationLeft = glm::vec3(1.0f);
		translationCenter = glm::vec3(1.0f);
		translationRight = glm::vec3(1.0f);
		scaling = glm::vec3(1.0f);
	}

	IstanziatoreOggetti(string str, float r, glm::vec3 tl, glm::vec3 tc, glm::vec3 tr, glm::vec3 s,float prob) { 
		//Costruttore a cui si passano, la radiante di rotazione, le tre traslazioni relative alla posizione, lo scaling  di dimensione e la "probabilità di spawn", in realtà maggiore è la variabile prob e minore sono le probabilità di spawn
		//Sono dati fondamentali per il bilanciamento
		modello = new Model(str);
		numeroIstanze = 0;
		oggetti.reserve(1);
		probabilita = prob;
		rotation = r;
		translationLeft = tl;
		translationCenter = tc;
		translationRight = tr;
		scaling = s;
	}

	oggetto_mov generaOstacoloBasedOnOtherPosition(oggetto_mov lastMov) {
		if (lastMov == NO_LINE) {
			return generaOstacolo();
		}
		else {
			float random = RandomNumber(0.0f, probabilita);
			Oggetto o;

			if ((random - 1.0f) <= 0.0f && lastMov != LEFT_O) {
				o.linea = LEFT_O;
				o.posizione = 50.0f;
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::rotate(model, this->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::translate(model, glm::vec3(this->translationLeft.x, this->translationLeft.y, this->translationLeft.z * o.posizione)); //glm::vec3(3.75f, 00.0f, 50.0f)); // translate it down so it's at the center of the scene //3.75 <-> 1.0f <-> -1.75f 
				model = glm::scale(model, this->scaling); //glm::vec3(0.5f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
				oggetti.push_back(o);
				numeroIstanze++;
				probabilita += 2.0f;

			}
			else if ((random - 2.0f) <= 0.0f && lastMov != CENTER_O) {
				o.linea = CENTER_O;
				o.posizione = 50.0f;
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::rotate(model, this->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::translate(model, glm::vec3(this->translationCenter.x, this->translationCenter.y, this->translationCenter.z * o.posizione));//glm::vec3(1.0f, 00.0f, 50.0f)); // translate it down so it's at the center of the scene //3.75 <-> 1.0f <-> -1.75f 
				model = glm::scale(model, scaling);//glm::vec3(0.5f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
				oggetti.push_back(o);
				numeroIstanze++;
				probabilita += 2.0f;
			}
			else if ((random - 3.0f) <= 0.0f && lastMov != RIGHT_O) {
				o.linea = RIGHT_O;
				o.posizione = 50.0f;
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::rotate(model, this->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::translate(model, glm::vec3(this->translationRight.x, this->translationRight.y, this->translationRight.z * o.posizione));//glm::vec3(-1.75f, 00.0f, 50.0f)); // translate it down so it's at the center of the scene //3.75 <-> 1.0f <-> -1.75f 
				model = glm::scale(model, scaling);// glm::vec3(0.5f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
				oggetti.push_back(o);
				numeroIstanze++;
				probabilita += 2.0f;
			}
			else {
				probabilita -= 0.1f;
				return NO_LINE;
			}

			return o.linea;
		}
	}

	oggetto_mov generaOstacolo() {
		float random = RandomNumber(0.0f, probabilita);
		Oggetto o;
		
		if ((random - 1.0f) <= 0.0f) {
			o.linea = LEFT_O;
			o.posizione = 50.0f;
			glm:: mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, this->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(this->translationLeft.x, this->translationLeft.y, this->translationLeft.z*o.posizione)); //glm::vec3(3.75f, 00.0f, 50.0f)); // translate it down so it's at the center of the scene //3.75 <-> 1.0f <-> -1.75f 
			model = glm::scale(model, this->scaling); //glm::vec3(0.5f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
			oggetti.push_back(o);
			numeroIstanze++;
			probabilita += 0.5f;
			
		}
		else if ((random - 2.0f) <= 0.0f) {
			o.linea = CENTER_O;
			o.posizione = 50.0f;
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, this->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(this->translationCenter.x, this->translationCenter.y, this->translationCenter.z * o.posizione));//glm::vec3(1.0f, 00.0f, 50.0f)); // translate it down so it's at the center of the scene //3.75 <-> 1.0f <-> -1.75f 
			model = glm::scale(model, scaling);//glm::vec3(0.5f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
			oggetti.push_back(o);
			numeroIstanze++;
			probabilita += 0.5f;
		}
		else if((random - 3.0f)<= 0.0f){
			o.linea = RIGHT_O;
			o.posizione = 50.0f;
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, this->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(this->translationRight.x, this->translationRight.y, this->translationRight.z * o.posizione));//glm::vec3(-1.75f, 00.0f, 50.0f)); // translate it down so it's at the center of the scene //3.75 <-> 1.0f <-> -1.75f 
			model = glm::scale(model, scaling);// glm::vec3(0.5f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
			oggetti.push_back(o);
			numeroIstanze++;
			probabilita += 0.5f;
		}
		else {
			probabilita -= 0.1f;
			return NO_LINE;
		}

		return o.linea;

	}

	void updatePosizioni(float s) {
		int eliminare = -1;
		for (float i = 0; i < numeroIstanze; i++) {
			oggetti[i].posizione += s;
			if (oggetti[i].posizione < -5) {
				eliminare = i;
			}
		}

		if (eliminare != -1) {
			oggetti.erase(oggetti.begin() + eliminare);
			numeroIstanze--;
		}
	}
	
	void eraseObject(int i) {
		if (i < oggetti.size() && i >= 0) {
			oggetti.erase(oggetti.begin() + i);
			numeroIstanze--;
		}
	}

	

	void draw() {


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, modello->textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
		for (unsigned int i = 0; i < modello->meshes.size(); i++)
		{
			glBindVertexArray(modello->meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, modello->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, numeroIstanze);
			glBindVertexArray(0);
		}
	}

	void updateIstanze() {

		glm::mat4* arrayModelli;

		arrayModelli = new glm::mat4[numeroIstanze];

		int indicatore = 0;

		for (unsigned int i = 0; i < numeroIstanze ; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);


			if (oggetti.at(i).linea == LEFT_O) {
				model = glm::rotate(model, this->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::translate(model, glm::vec3(this->translationLeft.x, this->translationLeft.y, this->translationLeft.z * oggetti[i].posizione)); //glm::vec3(3.75f, 00.0f, 50.0f)); // translate it down so it's at the center of the scene //3.75 <-> 1.0f <-> -1.75f 
				model = glm::scale(model, this->scaling); //glm::vec3(0.5f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
			}
			else if (oggetti[i].linea == CENTER_O) {
				model = glm::rotate(model, this->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::translate(model, glm::vec3(this->translationCenter.x, this->translationCenter.y, this->translationCenter.z * oggetti[i].posizione));//glm::vec3(1.0f, 00.0f, 50.0f)); // translate it down so it's at the center of the scene //3.75 <-> 1.0f <-> -1.75f 
				model = glm::scale(model, scaling);//glm::vec3(0.5f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
			}
			else {
				model = glm::rotate(model, this->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::translate(model, glm::vec3(this->translationRight.x, this->translationRight.y, this->translationRight.z * oggetti[i].posizione));//glm::vec3(-1.75f, 00.0f, 50.0f)); // translate it down so it's at the center of the scene //3.75 <-> 1.0f <-> -1.75f 
				model = glm::scale(model, scaling);// glm::vec3(0.5f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
			}


			arrayModelli[i] = model;
		}


		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, numeroIstanze * sizeof(glm::mat4), &arrayModelli[0], GL_STATIC_DRAW);


		for (unsigned int i = 0; i < modello->meshes.size(); i++) {

			unsigned int VAO = modello->meshes[i].VAO;
			glBindVertexArray(VAO);
			// set attribute pointers for matrix (4 times vec4)
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);


		}
	}




};