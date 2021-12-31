// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <iostream>

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

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) // 初始化 GLEW
  {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // 初始化 GLAD
  // {
  //   std::cout << "Failed to initialize GLAD" << std::endl;
  //   return -1;
  // }

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height); // 设置渲染窗口大小

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 注册窗口大小调整回调函数

  while (!glfwWindowShouldClose(window)) // 渲染循环
  {
    glfwPollEvents(); // 检测触发事件

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT); // 清空屏幕颜色缓冲

    glfwSwapBuffers(window); // 交换颜色缓冲 (双缓冲)
  }

  glfwTerminate(); // 关闭窗口, 释放资源

  return 0;
}