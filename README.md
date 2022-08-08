# M1 macのc++でGLFWを使ってみる

## 環境

```shell
> uname -m
arm64
> brew config
macOS: 12.5-arm64
Rosetta 2: false
```

## GLFWをインストール

```shell
> brew install glfw
> brew --prefix glfw
/opt/homebrew/opt/glfw
```

## コンパイル・実行してみるコード

- [ここ](https://www.glfw.org/documentation.html)のExample code

## インストールしただけだとエラーになる

```shell
❯ g++ -o main main.cpp -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
main.cpp:1:10: fatal error: 'GLFW/glfw3.h' file not found
#include <GLFW/glfw3.h>
```

## CPATH, LIBRARY_PATHを環境変数に追加

```shell
> vim ~/.zshrc
# 下記を追加
export CPATH=/opt/homebrew/include
export LIBRARY_PATH=/opt/homebrew/lib
```

## コンパイル・実行成功

- コンパイル成功したけど、warningが沢山出る。これはAppleがOpenGLを基本廃止（非推奨）にしているかららしい。

```shell
❯ g++ -o main main.cpp -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
main.cpp:23:5: warning: 'glClear' is deprecated: first deprecated in macOS 10.14 - OpenGL API deprecated. (Define GL_SILENCE_DEPRECATION to silence these warnings) [-Wdeprecated-declarations]
    glClear(GL_COLOR_BUFFER_BIT);
    ^
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/OpenGL.framework/Headers/gl.h:2394:13: note: 'glClear' has been explicitly marked deprecated here
extern void glClear (GLbitfield mask) OPENGL_DEPRECATED(10.0, 10.14);
```

## warningを非表示にしてみる

- コードに下記を追加する。
  - `#define GL_SILENCE_DEPRECATION`によって、非表示になる。

```cpp
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#endif
```

## Makefileを作る

```Makefile
CPPFLAGS := -I src/includes -I ${CPATH}
FRAMEWORKS := -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
LIBS := -L ${LIBRARY_PATH} -lglfw -lGLEW

BUILD_DIR := ./bin
SRC_DIRS := ./src
SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(addprefix $(BUILD_DIR)/, $(subst .cpp,.o, $(notdir $(SRCS))))

$(BUILD_DIR)/main: $(OBJS)
	clang++ -o $@ $(LIBS) $(FRAMEWORKS) $^ 
$(BUILD_DIR)/%.o: src/%.cpp
	clang++ -c -o $@ $(CPPFLAGS) $<

do:
	@bin/main
clean:
	rm -rf bin
	mkdir bin

```

## Debugできない

- vimspectorで、`.vimspector.json`を下記のように設定してみたけど、エラーになる。
- `-lglfw`があるとエラーになるっぽい。

```json
{
  "configurations": {
    "CodeLLDB": {
      "adapter": "CodeLLDB",
      "variables": {
        "BUILDME": {
          "shell": "clang++ -o ${workspaceRoot}/bin/debug -g -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo ${workspaceRoot}/src/main.cpp"
        }
      },
      "configuration": {
        "request": "launch",
        "program": "${workspaceRoot}/bin/debug",
        "stdio": [null, null, null]
      }
    }
  }
}

```
