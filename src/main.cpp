#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#endif

#include <iostream>
using namespace std;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void) {
  GLFWwindow* window;
  if (!glfwInit()) return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    cout << "failed to create window." << endl;
    glfwTerminate();
    return -1;
  }
  cout << "opened window" << endl;
  glfwSetKeyCallback(window, key_callback);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  std::cout << "Renderer: " << renderer << std::endl;
  std::cout << "OpenGL version supported: " << version << std::endl;

  glClearColor(0.0f, 0.3f, 0.6f, 0.3f);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  cout << "closed window" << endl;
}
