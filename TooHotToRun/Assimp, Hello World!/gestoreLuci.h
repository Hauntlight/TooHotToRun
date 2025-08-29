#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader_m.h"
#include "mesh.h"
#include "Utilities.h"




const int MAX_LIGHTS = 5;

struct pointLight{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float constant;
	float linear;
	float quadratic;
	glm::vec3 position;
};

struct listaLuci {
	pointLight lista[MAX_LIGHTS];
	glm::vec3 posizioni[MAX_LIGHTS];
	int indicatore;
};

//Considereremo una luce direzionale come una luce pointLight solo che il vettore direzione lo inserirmeo nella proprietà posizione della Struttura
//La sua posizione negli indici dovrà sempre rimanere 0

class GestoreLuci {
public:
	listaLuci listaL;
	
	GestoreLuci() {

	}

	void init() {
		listaL.indicatore = 1;
		listaL.lista[0].ambient = glm::vec3(0.5, 0.5, 0.5);
		listaL.lista[0].diffuse = glm::vec3(1.0, 1.0, 1.0);
		listaL.lista[0].specular = glm::vec3(0.1, 0.1, 0.1);
		listaL.lista[0].position = glm::vec3(0.2, 1.0, 0.3); //qui si intende direzione
	}

	//Questa funzione aggiorna gli shaders sulla posizione delle luci e i loro valori

	void updateShader(Shader& s) {
		s.setVec3("dirLight.direction", listaL.lista[0].position);
		s.setVec3("dirLight.ambient", listaL.lista[0].ambient);
		s.setVec3("dirLight.diffuse", listaL.lista[0].diffuse);
		s.setVec3("dirLight.specular", listaL.lista[0].diffuse);

		s.setInt("numberOfPoints", listaL.indicatore - 1);

		for (int i = 1; i < listaL.indicatore; i++) {
			//tratto le luci come oggetti per muoverle
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, 0.465f, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec4 punto_origine = glm::vec4(listaL.posizioni[i],1.0f);
			punto_origine = model * punto_origine;


			s.setVec3("pointLights["+std::to_string(i-1)+"].position", glm::vec3(punto_origine.x, punto_origine.y, punto_origine.z));
			s.setVec3("pointLights[" + std::to_string(i - 1) + "].ambient", listaL.lista[i].ambient);
			s.setVec3("pointLights[" + std::to_string(i - 1) + "].diffuse", listaL.lista[i].diffuse);
			s.setVec3("pointLights[" + std::to_string(i - 1) + "].specular", listaL.lista[i].specular);
			s.setFloat("pointLights[" + std::to_string(i - 1) + "].constant", listaL.lista[i].constant);
			s.setFloat("pointLights[" + std::to_string(i - 1) + "].linear", listaL.lista[i].linear);
			s.setFloat("pointLights[" + std::to_string(i - 1) + "].quadratic", listaL.lista[i].quadratic);
			
		}

	}

	void updatePosizioni(float spostamento) {
		for (int i = 1; i < listaL.indicatore; i++) {
			listaL.posizioni[i].z -= spostamento;
		}

		removeUselessLights();
	}

	


	bool addLuce(glm::vec3 posizione) {
		if (listaL.indicatore == MAX_LIGHTS) {
			return false;
		}
		
			
		listaL.lista[listaL.indicatore].diffuse = getRandomColor();

		listaL.lista[listaL.indicatore].ambient = glm::vec3(RandomNumber(0.4,1.0)* listaL.lista[listaL.indicatore].diffuse);
		listaL.lista[listaL.indicatore].specular = glm::vec3((RandomNumber(0.0,0.5), RandomNumber(0.0, 0.5), RandomNumber(0.0, 0.5)));
		listaL.lista[listaL.indicatore].constant = 1.0f;
		listaL.lista[listaL.indicatore].linear = 0.22f;
		listaL.lista[listaL.indicatore].quadratic = 0.20f;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, 0.465f, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 punto_origine = glm::vec4(posizione, 1.0f);
		punto_origine = model * punto_origine;
		listaL.lista[listaL.indicatore].position = glm::vec3(punto_origine.x, punto_origine.y, punto_origine.z);
		listaL.posizioni[listaL.indicatore] = posizione;
		listaL.indicatore++;
		return true;
	}

	void removeUselessLights() {
		int eliminare = -1;
		for (int i = 1; i < listaL.indicatore; i++) {
			if (listaL.posizioni[i].z > 5.0f) {
				eliminare = i;
			}
		}

		if (eliminare != -1) {
			if (eliminare == (listaL.indicatore - 1)) {
				listaL.indicatore--;
				return;
			}

			listaL.lista[eliminare].ambient = listaL.lista[listaL.indicatore - 1].ambient;
			listaL.lista[eliminare].diffuse = listaL.lista[listaL.indicatore - 1].diffuse;
			listaL.lista[eliminare].specular = listaL.lista[listaL.indicatore - 1].specular;
			listaL.lista[eliminare].constant = listaL.lista[listaL.indicatore - 1].constant;
			listaL.lista[eliminare].linear = listaL.lista[listaL.indicatore - 1].linear;
			listaL.lista[eliminare].quadratic = listaL.lista[listaL.indicatore - 1].quadratic;
			listaL.lista[eliminare].position = listaL.lista[listaL.indicatore - 1].position;
			listaL.posizioni[eliminare] = listaL.posizioni[listaL.indicatore - 1];
			
			listaL.indicatore--;
		}
		
	}

};
