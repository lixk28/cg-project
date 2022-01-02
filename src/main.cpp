#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "SkyBox.h"

#include <iostream>

#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 50.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 modelPositions[] =
    {
        glm::vec3(0.0f, 20.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 8.0f),
        glm::vec3(8.0f, 0.0f, 0.0f),
        glm::vec3(20.0f, 0.0f, 0.0f),
};

glm::vec3 lightPos(0.0f, 10.0f, 0.0f);

GLfloat skyBoxVertex[] = {
    -100.0f, 100.0f, -100.0f,
    -100.0f, -100.0f, -100.0f,
    100.0f, -100.0f, -100.0f,
    100.0f, -100.0f, -100.0f,
    100.0f, 100.0f, -100.0f,
    -100.0f, 100.0f, -100.0f,

    -100.0f, -100.0f, 100.0f,
    -100.0f, -100.0f, -100.0f,
    -100.0f, 100.0f, -100.0f,
    -100.0f, 100.0f, -100.0f,
    -100.0f, 100.0f, 100.0f,
    -100.0f, -100.0f, 100.0f,

    100.0f, -100.0f, -100.0f,
    100.0f, -100.0f, 100.0f,
    100.0f, 100.0f, 100.0f,
    100.0f, 100.0f, 100.0f,
    100.0f, 100.0f, -100.0f,
    100.0f, -100.0f, -100.0f,

    -100.0f, -100.0f, 100.0f,
    -100.0f, 100.0f, 100.0f,
    100.0f, 100.0f, 100.0f,
    100.0f, 100.0f, 100.0f,
    100.0f, -100.0f, 100.0f,
    -100.0f, -100.0f, 100.0f,

    -100.0f, 100.0f, -100.0f,
    100.0f, 100.0f, -100.0f,
    100.0f, 100.0f, 100.0f,
    100.0f, 100.0f, 100.0f,
    -100.0f, 100.0f, 100.0f,
    -100.0f, 100.0f, -100.0f,

    -100.0f, -100.0f, -100.0f,
    -100.0f, -100.0f, 100.0f,
    100.0f, -100.0f, -100.0f,
    100.0f, -100.0f, -100.0f,
    -100.0f, -100.0f, 100.0f,
    100.0f, -100.0f, 100.0f};

glm::vec3 mul_vec3(int n, glm::vec3 vec)
{
  glm::vec3 sum(0.0f, 0.0f, 0.0f);
  for (int i = 0; i < n; i++)
  {
    sum += vec;
  }
  return sum;
}

//两向量夹角
float getAngle(glm::vec3 a, glm::vec3 b)
{
  float dot = 0, a_len = 0, b_len = 0;
  dot = a.x * b.x + a.y * b.y + a.z * b.z;
  a_len = a.x * a.x + a.y * a.y + a.z * a.z;
  b_len = b.x * b.x + b.y * b.y + b.z * b.z;
  float cos_angle = dot / (sqrt(a_len) * sqrt(b_len));
  return acosf(cos_angle);
}

int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw window creation
  // --------------------
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "cg-project", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
  stbi_set_flip_vertically_on_load(true);

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

  // build and compile shaders
  // -------------------------
  Shader robotShader("shader/robot.vs", "shader/robot.fs");
  Shader skyShader("shader/skybox.vs", "shader/skybox.fs");
  Shader graphShader("shader/graph.vs", "shader/graph.fs");
  Shader houseShader("shader/graph.vs", "shader/graph.fs");

  // load models
  // -----------
  Model robotModel("resource/ironman/IronMan.obj");
  Model houseModel("resource/house/house.obj");
  Model cityModel("resource/city/casa.obj");
  Model jeniModel("resource/jeni/BR_Squirtle.obj");

  // draw in wireframe
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // render loop
  // -----------

  //  天空盒
  GLuint skyBoxVAO, skyBoxVBO;
  glGenVertexArrays(1, &skyBoxVAO);
  glGenBuffers(1, &skyBoxVBO);
  glBindVertexArray(skyBoxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyBoxVertex), skyBoxVertex, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  vector<const GLchar *> faces;
  faces.push_back("resource/skybox/CloudyCrown_Midnight_Right.png");
  faces.push_back("resource/skybox/CloudyCrown_Midnight_Left.png");
  faces.push_back("resource/skybox/CloudyCrown_Midnight_Up.png");
  faces.push_back("resource/skybox/CloudyCrown_Midnight_Down.png");
  faces.push_back("resource/skybox/CloudyCrown_Midnight_Back.png");
  faces.push_back("resource/skybox/CloudyCrown_Midnight_Front.png");
  GLuint skyBoxTexture = loadCubeMap(faces);

  while (!glfwWindowShouldClose(window))
  {
    // per-frame time logic
    // --------------------
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.12f, 0.63f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    skyShader.use();
    glm::mat4 model0(1.0f);
    model0 = glm::translate(model0, glm::vec3(0.0, 50, 0.0));
    glm::mat4 projection1 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
    glm::mat4 view1 = camera.GetViewMatrix();
    skyShader.setMat4("model", model0);
    skyShader.setMat4("projection", projection1);
    skyShader.setMat4("view", view1);

    glBindVertexArray(skyBoxVAO);
    glActiveTexture(GL_TEXTURE0);
    skyShader.setInt("skyBox", 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);

    // don't forget to enable shader before setting uniforms
    robotShader.use();

    robotShader.setVec3("light.position", lightPos);
    robotShader.setVec3("viewPos", camera.Position);

    //light properties
    robotShader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
    robotShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
    robotShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    robotShader.setMat4("projection", projection);
    robotShader.setMat4("view", view);

    // render the loaded model
    glm::mat4 model1 = glm::mat4(1.0f);
    model1 = glm::translate(model1, modelPositions[0]);          // translate it down so it's at the center of the scene
    model1 = glm::scale(model1, glm::vec3(0.01f, 0.01f, 0.01f)); // it's a bit too big for our scene, so scale it down
    robotShader.setMat4("model", model1);
    // robotModel.Draw(robotShader);

    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::translate(model2, modelPositions[0]); // translate it down so it's at the center of the scene
    model2 = glm::scale(model2, glm::vec3(0.35f, 0.35f, 0.35f));
    // model2 = glm::rotate(model2, 30.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
    // angle
    float angle = getAngle(camera.Front, glm::vec3(0.0f, 1.0f, 0.0f));
    model2 = glm::rotate(model2, angle, glm::cross(glm::vec3(0.0f, 1.0f, 0.0f),camera.Front));
    graphShader.use(); // it's a bit too big for our scene, so scale it down
    graphShader.setMat4("model", model2);
    graphShader.setMat4("projection", projection);
    graphShader.setMat4("view", view);
    graphShader.setVec3("light.position", lightPos);
    graphShader.setVec3("viewPos", camera.Position);

    //light properties
    graphShader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
    graphShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
    graphShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    jeniModel.Draw(graphShader);

    robotShader.use();
    glm::mat4 model4 = glm::mat4(1.0f);
    model4 = glm::translate(model4, modelPositions[2]);
    model4 = glm::scale(model4, glm::vec3(10.0f, 10.0f, 10.0f));
    robotShader.setMat4("model", model4);
    cityModel.Draw(robotShader);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
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
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);

  modelPositions[0] = camera.Position + mul_vec3(10, camera.Front) - camera.Up;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
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
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(yoffset);
}
