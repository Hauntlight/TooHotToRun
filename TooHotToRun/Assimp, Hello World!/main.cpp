#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shader_m.h"
#include "camera.h"
#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "effects.h"
#include <iostream>
#include "player.h"
#include "WorldMap.h"
#include "renderText.h"
#include "GestoreLuci.h"
#include "SoundManager.h"
#include "gameScene.h"
#include "menuScene.h"
#include <windows.h>
#include <ShellAPI.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos); Non Abbiamo Bisogno del Mouse
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
Bob_Movement processInputGame(GLFWwindow* window);
bool processInputMenu(GLFWwindow* window);


string linkChar = "https://github.com/Hauntlight/TooHotToRun/blob/main/README.MD"; //link alla BREVISSIMA guida

Camera camera(camPos);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int frameCount = 0;
double previousTime = 0;
double timeInterval = 0;
unsigned int fps = 0;

bool statusGame = false; //False -> Render Menu / True -> Render Game
bool musicOn = true;// Se è False allora la musica è già in play


void calculateFPS() {
    //  Incrementiamo il contatore
    frameCount++;
    //  Determiniamo il numero di millisecondi trascorsi dalla glutInit
    double currentTime = glfwGetTime();
    //  Calcoliamo il tempo trascorso
    timeInterval = currentTime - previousTime;

    // Se è passato un secondo aggiorna la variabile fps
    if (timeInterval > 1.0f) {
        //  frameCount mantiene il numero di frame generati in un secondo
        fps = frameCount;

        //  Salviamo il tempo trascorso per riutilizzarlo la prossima volta
        previousTime = currentTime;

        //  Azzeriamo il contatore dei tempi
        frameCount = 0;
    }
}



int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assimp, Hello World! (ver 5.0.1)", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  //glfwSetCursorPosCallback(window, mouse_callback);
  //glfwSetScrollCallback(window, scroll_callback);
  //Non abbiamo Bisogno del Mouse
  // tell GLFW to capture our mouse
  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }


  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);


  // build and compile shaders
  // -------------------------
  Shader modelShader("model_loading.vs", "model_loading.fs"); //Shader usato per Bob
  Shader instancingShader ("instanced.vs", "instanced.fs"); //Shader usato per wordmap e ostacoli (Date le poche istanze è un po' esagerato ma comunque legittimo)
  Shader baseShader("basicShader.vs", "basicShader.fs"); //Shader usato per renderizzazioni di fumo,esplosione etitle-screen
  
  initRenderText(SCR_WIDTH, SCR_HEIGHT); // Inizializzo lo shader del testo e carico i Font; File preso dal codice fornitoci

  glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
  glm::mat4 view = camera.GetViewMatrix();

  modelShader.use();
  modelShader.setInt("material.diffuse", 0);
  modelShader.setInt("material.specular", 1);
  modelShader.setFloat("material.shininess", 32.0f);
  modelShader.setMat4("projection", projection);
  modelShader.setMat4("view", view);
  modelShader.setVec3("viewPos", camera.Position);

  instancingShader.use();
  instancingShader.setInt("material.diffuse", 0);
  instancingShader.setInt("material.specular", 1);
  instancingShader.setFloat("material.shininess", 32.0f);
  instancingShader.setMat4("projection", projection);
  instancingShader.setMat4("view", view);
  instancingShader.setVec3("viewPos", camera.Position);

  baseShader.use();
  baseShader.setMat4("projection", projection);
  baseShader.setMat4("view", view);
 
  GameScene game;
  MenuScene menu;

  game.init();
  menu.init("resources/screen/MainScreen.jpeg");

  //Dato che la Camera è fissa non abbiamo bisogno di ridefinire view e projection ad ogni frame

  while (!glfwWindowShouldClose(window))
  {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

  
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    
    if (statusGame == false) {
        menu.render(baseShader,deltaTime);
        statusGame = processInputMenu(window);
    }
    else {
        if (musicOn) {
            soundManager.playMusicLoop(soundMusic);
            musicOn = false;
        }
        game.play(modelShader, instancingShader, baseShader, processInputGame(window), deltaTime);
    }

    calculateFPS();
    RenderText(("FPS: " + std::to_string(fps)).c_str(), 0, SCR_HEIGHT - 30, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f)); //Gli fps sono sempre in stampa
    //Risultati dai nostri benchmark:
    //Ram impegnata 1.2GB + 1.2GB su GPU
    // FPS Medi: 
    // 60 Fps Stabili su Ryzen 7 4800H + GTX1650 TI (Mobile)
    // 60 Fps Stabili su i5-10gen + GTX 1660 (Mobile)
    // 60 Fps con cali sui 50 con i5-8Gen + GTX 1050 TI (Mobile)
    // 60 Fps con cali sui 45 con GTX 970M
    // Integrata : Giocabile Solo senza Effetto particellare del fumo
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
Bob_Movement processInputGame(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      return LEFT_BOB;
  }
      
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
      return RIGHT_BOB;

  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
      return NO_DIR;
  }

      
}

bool processInputMenu(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        return true;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        std::wstring stemp = std::wstring(linkChar.begin(), linkChar.end());
        ShellExecute(NULL, NULL, stemp.c_str(), NULL, NULL, SW_SHOWNORMAL);
    }
    return false;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  //camera.ProcessMouseScroll(yoffset); TODO REMOVE
}






