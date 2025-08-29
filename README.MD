# Too Hot To Run

![Language](https://img.shields.io/badge/language-C%2B%2B-blue.svg)
![Renderer](https://img.shields.io/badge/renderer-OpenGL-green.svg)

A small endless runner game developed in **C++** and **OpenGL** for the *Advanced Computer Graphics* course at the University of Basilicata.

---

## 🎮 About the Game

**Too Hot To Run** is an endless runner where survival is not just about dodging obstacles.  
You control **Bob**, a character who is constantly heating up!  
Your objective is to last as long as possible by:

- Navigating a dangerous track  
- Avoiding barriers  
- Managing Bob’s internal temperature  

Power-ups can help you—but each comes with its own trade-offs. How long can you survive before overheating, crashing, or freezing in place?

---

## 🕹️ How to Play

### Objective
Survive for as long as possible to achieve the highest score. Avoid obstacles and carefully manage Bob’s temperature.

### Core Mechanics
- **Temperature:** If Bob’s temperature exceeds **50°C**, the game ends.  
- **Obstacles:** Collisions with barriers, lamps, or other objects result in instant failure.  
- **Keep Moving:** If Bob’s speed drops to zero, he explodes.  

### Power-Ups
- 🔥 **Fire Bonus:** Boosts speed and acceleration but quickly increases Bob’s temperature.  
- ❄️ **Ice Bonus:** Lowers Bob’s temperature but reduces speed and acceleration. Use carefully to avoid stalling.  

### Controls

#### Main Menu
| Key         | Action                         |
|-------------|--------------------------------|
| **SPACE**   | Start the game                 |
| **H**       | Open the in-game guide         |
| **ESC**     | Quit the application           |

#### In-Game
| Key         | Action                         |
|-------------|--------------------------------|
| **A**       | Move left                      |
| **D**       | Move right                     |
| **ESC**     | Quit the application           |

---

## 🚀 Getting Started

### Play the Game
Download the latest precompiled version from the **[Releases](https://github.com/Hauntlight/TooHotToRun/releases)** page.

### Build from Source
To build the project, ensure you have a C++ compiler and the following dependencies:

- [GLAD](https://github.com/Dav1dde/glad) & [GLFW](https://www.glfw.org/) – Windowing and OpenGL context  
- [GLM](https://github.com/g-truc/glm) – Mathematics library  
- [Assimp](https://github.com/assimp/assimp) – Model loading  
- [stb_image](https://github.com/nothings/stb) – Texture loading  

Clone the repository and compile using your preferred build system.

---

## ⚙️ Technical Features

This project demonstrates key computer graphics concepts:

- **Model Loading**: Uses *Assimp* to import character and environment models.  
- **Instanced Rendering**: Efficiently renders the world and obstacles with fewer draw calls.  
- **Multiple Light Sources**: Includes point lights and a directional light for a dynamic scene.  
- **Particle System**: Provides smoke effects.  
- **Scene Management**: Implements a state machine for menus and game logic.  
- **Text Rendering**: Uses *FreeType* for UI elements (e.g., FPS counter).  
- **Sound Management**: Adds background music and sound effects.  

---

## 📊 Performance Benchmarks

Tested on various configurations:

- **Stable 60 FPS**
  - Ryzen 7 4800H + GTX 1650 Ti (Mobile)  
  - Intel i5-10th Gen + GTX 1660 (Mobile)  

- **~50–60 FPS**
  - Intel i5-8th Gen + GTX 1050 Ti (Mobile)  

- **~45–60 FPS**
  - NVIDIA GTX 970M  

- **Integrated Graphics**
  - Playable, but disabling smoke particles is recommended  
