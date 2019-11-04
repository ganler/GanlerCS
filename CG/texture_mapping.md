# Texture Mapping

[TOC]

## Texture

- 一般来说就是一个BitMap Image(2D，我们不讨论3D的情况)
- Pixels of the texture are called *texels*;
- Texel coordinates are in 2D, in the range `[0,1]`（注意这里的坐标是连续的，而图片是离散的，所以要得到真实的像素值需要进行一些采样）
  - OpenGL: `(s, t)`
  - Commonly: `(u, v)`

## UV坐标

- 基本坐标`(X, Y, Z)`用于表示点的位置；
- 现在给出一个2D纹理，其坐标是`(U, V)`
- 我们将其加载点坐标上得到了：`(X, Y, Z, U, V)`
- 其含义是，当前点`(X, Y, Z)`使用了纹理上`(U, V)`点的像素
- 而得到UV后，我们通过采样，得到一个颜色值(RGBA)

## Mapping

对于一个Mapping Function $f$，我们要得到一个uv坐标(0~1):
$$
[u, v]=f(x, y,z)
$$
对于一个采样函数$g$，我们要得到一个颜色：
$$
[r,g,b,a]=g(u,v)
$$

### Mapping Funcs

- 对于不同的几何有不同的Mapping方法（复杂的集合的mapping方法通常过于复杂，以至于一般都让人手工来做（得到一个对应表））

### Sampling Funcs

> 图像处理那一套

- Nearest Neighbor（选择最近的像素）
- Bilinear（双线性插值）
- Hermite（在Bilinear的基础上，使用了不同的权重）

## OpenGL Texture Mapping

- 初始化
  - `glEnable(GL_TEXTURE_2D)`
  - 选择纹理
  - 选择参数
  - 定义并启动纹理
- Draw: 将texture和坐标对应起来

```c++
glEnable(GL_TEXTURE_2D); // Begin

// Create a texture obj
GLuint texture_id;
glGenTextures(1, &texture_id);
glBindTexture(GL_TEXTURE_2D, texture_id);

/*
You can do glBindTexture(0) to have no active texture; 
This is equivalent to having a solid white texture. 
You can do this to avoid disabling texturing.
 */

glTexImage2D(
  GL_TEXTURE_2D, 
  0,
  GL_RGB,            // layout
  width,             // 宽
  height,            // 高
  0, 
  GL_BGR,            // layout
  GL_UNSIGNED_BYTE,  // 每个pixel的大小
  data               // image buffer 的ptr
);

// glTexParameter*

glDisable(GL_TEXTURE_2D);// End
```

### 定义当Texture Coordinates Out of Bounds的行为

![img](https://learnopengl-cn.github.io/img/01/06/texture_wrapping.png)

```c++
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
GL_REPEAT); 
// S, T ~ U, V
```

### 插值采样

- `GL_NEAREST`
- `GL_LINEAR` 

```c++
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // 放大过滤
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // 缩小过滤
```

- `GL_TEXTURE_MAG_FILTER`(Magnify)：当图像被放大的时候，如何过滤；
- `GL_TEXTURE_MAG_FILTER`(Minify): 当图像被缩小的时候，如何过滤；

### 多级渐远纹理 Mipmapping

> 远处的东西不要高分辨率了啦！
>
> 远处的纹理片段少，OpenGL从高分辨率的纹理中为其获取正确的颜色很困难，因为它需要对一个跨过纹理很大部分的片段只拾取一个纹理颜色。
>
> OpenGL使用Mipmap的概念来解决该问题。
>
> <u>即使用不同分辨率的照片，越远的分辨率越低；</u>

![img](https://learnopengl-cn.github.io/img/01/06/mipmaps.png)

不同分辨率：意味着有采样技术：

| 过滤方式                    | 描述                                                         |
| --------------------------- | :----------------------------------------------------------- |
| `GL_NEAREST_MIPMAP_NEAREST` | 使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样 |
| `GL_LINEAR_MIPMAP_NEAREST`  | 使用最邻近的多级渐远纹理级别，并使用线性插值进行采样         |
| `GL_NEAREST_MIPMAP_LINEAR`  | 在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样 |
| `GL_LINEAR_MIPMAP_LINEAR`   | 在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样 |

> 这个其实可以看做一个三维的采样：
> `GL_UV采样_MIPMAP_Z采样`

```c++
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

// ... glTexImage2D
```



### Color Blending（混合）

- `GL_REPLACE`: Use texture color only;
- `GL_BLEND`: Linear combination of texture and object color;
- `GL_MODULATE`: Multiply texture and object color;

```c++
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
```



![](https://i.loli.net/2019/11/03/a7DjnxRqZsTOSMA.png)

### Defining/Activating a Texture

```c++
void glTexImage2D(
    GLenum  target, 
    GLint   level, 
    GLint   internalFormat, 
    int     width, 
    int     height, 
    GLint   border, 
    GLenum  format, 
    GLenum  type, 
    GLvoid* image
);

/*

glBindTexture(GL_TEXTURE_2D, texture_id);
glTexImage2D(
		GL_TEXTURE_2D,
    0, 
    GL_RGBA,
    256, 
    256, 
    0, 
    GL_RGBA, 
    GL_UNSIGNED_BYTE,
    ptr2image);
 */
```

### Sample

```c++
glEnable(GL_TEXTURE_2D);

GLuint texture_id;

glGenTextures(1, &texture_id);
glBindTexture(GL_TEXTURE_2D, texture_id);

glTexParameteri(GL_TEXUTRE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXUTRE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

glTexImage2D(
  	GL_TEXTURE_2D,
    0, 
    GL_RGBA,
    width, 
    height, 
    0, 
    GL_RGBA, 
    GL_UNSIGNED_BYTE,
    ptr2image);

glGenerateMipmap(GL_TEXTURE_2D); // Really easy.
```

### With Shader

```c
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}
```

```c
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
    // 如果要和颜色混合，可以直接：
    // FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
}
```

之后只需要在`glBindTexture`即可。

## Ref

> https://www.cs.cmu.edu/afs/cs/academic/class/15462-f09/www/lec/lec10.pdf