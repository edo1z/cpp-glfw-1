#ifdef __APPLE__
#  define GL_SILENCE_DEPRECATION
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
#endif

#include <cstdlib>
#include <iostream>
#include <vector>
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

static void key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

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

  glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

  static const GLchar vsrc[] = "#version 150 core¥n"
                               "in vec4 position;\n"
                               "void main()\n"
                               "{\n"
                               " gl_Position = position;\n"
                               "}\n";
  // フラグメントシェーダのソースプログラム
  static const GLchar fsrc[] = "#version 150 core\n"
                               "out vec4 fragment;\n"
                               "void main()\n"
                               "{\n"
                               " fragment = vec4(1.0, 0.0, 0.0, 1.0);\n"
                               "}\n";
  // プログラムオブジェクトを作成する
  const GLuint        program(createProgram(vsrc, fsrc));

  while (! glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  cout << "closed window" << endl;
}
