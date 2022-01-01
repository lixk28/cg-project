// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"

// 键盘回调函数
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

// 窗口大小调整回调函数
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

int main()

{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 设置 OpenGL 版本号为 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 设置为核心模式

  GLFWwindow *window = glfwCreateWindow(1024, 768, "cg-project", NULL, NULL); // 创建窗口并绑定至上下文
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 注册窗口大小调整回调函数
  glfwSetKeyCallback(window, key_callback);                          // 注册键盘回调函数

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) // 初始化 GLEW
  {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height); // 设置渲染窗口大小

  // GLfloat vertices[] = 
  // {
  //   -0.5f, -0.5f, 0.0f,
  //   0.5f, -0.5f, 0.0f,
  //   0.0f,  0.5f, 0.0f,
  // };

  // GLuint vertexArrayID;
  // glGenVertexArrays(1, &vertexArrayID);
  // glBindVertexArray(vertexArrayID);

  // GLuint vertexBuffer;
  // glGenBuffers(1, &vertexBuffer);
  // glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  while (!glfwWindowShouldClose(window)) // 渲染循环
  {
    glfwPollEvents(); // 检测触发事件

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT); // 清空屏幕颜色缓冲

    // glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDisableVertexAttribArray(0);

    glfwSwapBuffers(window); // 交换颜色缓冲 (双缓冲)
  }

  glfwTerminate(); // 关闭窗口, 释放资源

  return 0;
}