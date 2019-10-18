# GLM

> [https://learnopengl-cn.github.io/01%20Getting%20started/07%20Transformations/](https://learnopengl-cn.github.io/01 Getting started/07 Transformations/)

## Demo

> **GLM库从<u>0.9.9版本</u>起，默认会将矩阵类型初始化为一个零矩阵。**

#### 主要include

```c++
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
```

#### 位移

```c++
glm::vec4 vec(1, 1, 1, 1);
// x正向移动1格，得到转移矩阵
auto trans = glm::translate(glm::mat4(1), glm::vec3(1, 0, 0));
vec = trans * vec; // Yes!
```

#### 旋转+缩放

```c++
glm::mat4 trans(1);
// glm::rotate(原矩阵, 弧度(角度), axis);
trans = glm::rotate(trans, glm::radians(90), glm::vec3(1, 0, 0));

// 缩放，所有轴都缩小一倍
trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
```

#### 正射投影



#### 透视投影

![ perspective_frustum](https://learnopengl-cn.github.io/img/01/08/perspective_frustum.png)



```c++
glm::mat4 proj = glm::perspective(
  glm::radians(45.0f), 
  (float)width/(float)height,
  0.1f,     // near
  100.0f    // far
);
```



## 传递矩阵给着色器

> `GLSL`也有一个`mat4`类型数据，让其uniform即可；

```c++
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 transform; // uniform!!!

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);
    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
}
```

```c++
// your code.
unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
```

