# 杰尼龟的<s>奇妙之旅</s> Horror Palace

![](gallery/cg_demo.gif)

## 环境配置

开发和测试环境为 Ubuntu 20.04.3 LTS x86_64，其他操作系统并未做测试。

下载安装下面的库和头文件。

```shell
sudo apt install build-essential
sudo apt install libglfw3 libglfw3-dev
sudo apt install libglew2.1 libglew-dev
sudo apt install libglm-dev
sudo apt install libassimp5 libassimp-dev
```

## 使用方法

使用 glfw：

```C++
#include <GLFW/glfw3>
```

使用 glew：

```C++
#include <GL/glew.h>
```

使用 glm：

```C++
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
```

## 编写代码

使用 VSCode。

在 `include` 目录下创建头文件，比如 `foo.h`：

```C++
#ifndef FOO_H
#define FOO_H

// declarations

#endif
```

然后在 `src` 目录下编写对应的源文件 `foo.cpp`：

```C++
#include "foo.h"

// implementations
```

## 编译运行

第一次编译需要创建 `build` 目录。此后直接 `make` 即可。

```shell
mkdir build
make
```

运行程序：

```shell
make test
```

清理目标文件和可执行文件：

```shell
make clean
```

## 参考资料

- https://learnopengl.com/
