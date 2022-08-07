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
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    cout << "failed to create window." << endl;
    glfwTerminate();
    return -1;
  }
  cout << "opened window" << endl;
  glfwSetKeyCallback(window, key_callback);
  glfwMakeContextCurrent(window);
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  cout << "closed window" << endl;
}
