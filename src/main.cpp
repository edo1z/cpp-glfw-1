#ifdef __APPLE__
#  define GL_SILENCE_DEPRECATION
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
#endif
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "Shape.h"
using namespace std;

GLboolean printShaderInfoLog(GLuint shader, const char* str)
{
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) cerr << "Compile Error in " << str << endl;
  GLsizei bufSize;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
  if (bufSize > 1) {
    vector<GLchar> infoLog(bufSize);
    GLsizei        length;
    glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
    cerr << &infoLog[0] << endl;
  }
  return static_cast<GLboolean>(status);
}

GLboolean printProgramInfoLog(GLuint program)
{
  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) cerr << "Link Error." << endl;
  GLsizei bufSize;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
  if (bufSize > 1) {
    vector<GLchar> infoLog(bufSize);
    GLsizei        length;
    glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
    cerr << &infoLog[0] << endl;
  }
  return static_cast<GLboolean>(status);
}

GLuint createProgram(const char* vsrc, const char* fsrc)
{
  const GLuint program(glCreateProgram());
  if (vsrc != NULL) {
    const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
    glShaderSource(vobj, 1, &vsrc, NULL);
    glCompileShader(vobj);
    if (printShaderInfoLog(vobj, "vertex shader")) {
      glAttachShader(program, vobj);
    }
    glDeleteShader(vobj);
  }
  if (fsrc != NULL) {
    const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
    glShaderSource(fobj, 1, &fsrc, NULL);
    glCompileShader(fobj);
    if (printShaderInfoLog(fobj, "fragment shader")) {
      glAttachShader(program, fobj);
    }
    glDeleteShader(fobj);
  }
  glBindAttribLocation(program, 0, "position");
  glBindFragDataLocation(program, 0, "fragment");
  glLinkProgram(program);
  if (printProgramInfoLog(program)) {
    return program;
  }
  glDeleteProgram(program);
  return 0;
}

bool readShaderSource(const char* name, vector<GLchar>& buffer)
{
  if (name == NULL) return false;
  ifstream file(name, ios::binary);
  if (file.fail()) {
    cerr << "Error: Can't open source file: " << name << endl;
    return false;
  }
  file.seekg(0L, ios::end);
  GLsizei length = static_cast<GLsizei>(file.tellg());
  buffer.resize(length + 1);
  file.seekg(0L, ios::beg);
  file.read(buffer.data(), length);
  buffer[length] = '\0';

  if (file.fail()) {
    cerr << "Error: Could not read souce file: " << name << endl;
    file.close();
    return false;
  }

  file.close();
  return true;
}

GLuint loadProgram(const char* vert, const char* frag)
{
  vector<GLchar> vsrc;
  const bool     vstat(readShaderSource(vert, vsrc));
  vector<GLchar> fsrc;
  const bool     fstat(readShaderSource(frag, fsrc));
  return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

static void key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

constexpr Object::Vertex rectangleVertex[] = {
  {-0.5f, -0.5f},
  { 0.5f, -0.5f},
  { 0.5f,  0.5f},
  {-0.5f,  0.5f},
};

int main(void)
{
  GLFWwindow* window;
  if (! glfwInit()) return 1;
  atexit(glfwTerminate);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (! window) {
    cout << "failed to create window." << endl;
    return 1;
  }
  cout << "opened window" << endl;
  glfwSetKeyCallback(window, key_callback);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    cerr << "Can't initialize GLEW" << endl;
    return 1;
  }

  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version  = glGetString(GL_VERSION);
  std::cout << "Renderer: " << renderer << std::endl;
  std::cout << "OpenGL version supported: " << version << std::endl;

  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  const GLuint            program(loadProgram("point.vert", "point.frag"));

  unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex));

  while (! glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    shape->draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  cout << "closed window" << endl;
}
