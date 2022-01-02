#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "stb_image.h"
using namespace std;

GLuint loadCubeMap(vector<const GLchar*> faces);

#endif
