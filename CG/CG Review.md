# CG Review

[TOC]

## **Computer Graphics**

### 💖 What's CG

The study of creating, manipulating, and using virtual images in the computer.

### 💖 CG App

- Game
- Film effect
- CAD
- Visualization

### 💖 Graphic Pipline

- **Application** stage
- **Geometry** stage
- **Rasterization** stage

<img src="https://i.loli.net/2019/12/09/NCSLMraQ6gyKIj2.png" style="zoom:50%;" />



### 💖 The elements of image formation

- Object
- Viewer
- Light Source(s)
- Attributes

### 💖 Pinhole Camera Model

${NAME} describes **the ideal perspective projection of a camera**, without considering the physics of the **lens**(镜头), the **apeture**（光圈） and the **film**（胶片）.

<img src="https://openmvg.readthedocs.io/en/latest/_images/pinholeCamera.png" style="zoom: 67%;" />

### 💖 Framebuffer

A **framebuffer** is a portion of RAM containing a bitmap that drives a video display. 

### Vertex, primitives & fragment

A **vertex** is a point/node in the space, represented by its <u>coordinate</u> and <u>properties</u>, e.g. color, normal, texture coordinate etc. 

A **primitive** is the basic element of **geometric shapes** and rendering, it is composed of vertices. It can be points, lines, polygons or polyhedrons. 

A **fragment** is the output data of the geometry shader in <u>rasterization stage</u>. The fragment contains the data necessary to generate <u>a single pixel in the frame buffer</u>, e.g. color, depth, normal, texture coordinate etc.

**Geometry**: <u>locations</u> of the vertices.

**Topology**: <u>organization</u> of the vertices and edges.

### Solid Object Representation

- **Surface-based** representation
- **Volume-based** representation
- CSG(constructive solid geometry)
- **Point-based** representation.

## **Transformation**

### 💖 Coordinate Spaces

- Model/Local
- World
- Camera 
- Clip
- NDC

↑ 3D/4D

↓ 2D

- Viewport
- Window

### 💖 Transformation Types

- Translation(<u>*non-linear*</u>) $T(u)=u+v$
- Reflection
- Scaling 
- Shear
- Rotation 

> $$
> T(\alpha u)=\alpha T(u)
> $$
>
> Translation does not follow this rule.
>
> 逆时针旋转$\theta$ 度。
> $$
> \left\{\begin{matrix}
> \cos \theta & -\sin \theta\\
> \sin \theta & \cos \theta
> \end{matrix}\right\}
> $$
> How to remember:
> $$
> x\sim\cos(a+b)=\cos a\ \mathbf{cos b} - \sin a\ \mathbf{sin b}\\
> y\sim\sin(a+b)=\cos a\ \mathbf{sin b} + \sin a\ \mathbf{cos b}
> $$
> Shear:
> $$
> \left\{\begin{matrix}
> 1 & a\\
> 0 & 1
> \end{matrix}\right\}
> $$

### 💖 Affine Transformation

> <u>Translation together with linear transformations.</u>

Translation is not linear transformation. And affine transformation helps translation together with linear transformations.

### 💖 Homogeneous Coordinates

`dimension += 1`

齐次坐标表示是计算机图形学的重要手段之一，它既能够用来明确区分向量和点，同时也更易用于进行仿射（线性）几何变换。

## **Viewing**

### 💖 Viewing Transformation

- **Modeling** Transformation(local space -> world space)
- **Camera** Transformation(world space -> camera space)
- **Projection** Transformation(camera -> NDC)
- **Viewport** Transformation(NDC -> window coordinates)

### 💖 Viewing impl

1. Local space -> world space -> camera space.
2. Projection -> view volume. (3D -> 2D)
3. Clip geometry outside the view volume.
4. Project into screen coordinates.

5. Remove hidden surfaces.

### 💖 Orthographic  projection

Just define a orthographic view volume. (A box $\to [-1, 1]^3$)

**Normalized form**

- Move center to the origin;
- Scale to have sides of length 2;
- Remove camera(z) axis.

**Pros:**

- Shapes preserved;
- Good for measurements;

**Cons:**

- Cannot see what object really looks like as many surfaces hidden from view.
- (我加的) Not realistic from the perspective of humans.

### 💖 Perspective projection

$$
[x_p,y_p,z_p]=[\frac{x_pd}{x}, \frac{y_pd}{y}, d]
$$

**Pros:**

- Realistic;

**Cons:**

- Non-uniform foreshortening;
- More difficult to construct by hand than parallel projections;

> Can be a orthographic projection if $d\to \infin$

**Normalized Form**:

<img src="https://i.loli.net/2019/12/21/7AWkRBmIelynSUt.png" style="zoom:50%;" />

> $$
> P_n=\frac{P_c}{w_c}
> $$

### Diff between NDC & clip space

- **Clip space:** After projection transformation and before perspective division.(w is not 1). 
- **NDC:** The space after perspective division. The homogenous coordinate w of vertex is 1;

### Why does clip performed in the clip space? 

- **Efficiency**;
- **Correctness**: The orginal depth info in kept in clip space, but is normalized in NDC;

### Line clipping: Cohen-Sutherland Algorithm 

- Outcode;

### Polygon clipping: Weiler-Atherton Clipping 

- Walk(逆时针，遇到边界且是入射，那么就OK，连续的出射点+入射点=>边界直连)
- 如果polygon的一个角点在screen的边上就麻烦了；

> 其他方法就拆三角形就完事了。

## **Visible Surface Detection**

### 💖 Z-Buffer

简单来说就是在viewport上的每个像素： => Draw if closer.

- 记录当前「最近深度」；
- 以后的像素，如果远于最近深度，就不画；
- 近于最近深度就画&更新当前位置像素的「最近深度」。

#### Pros & Cons

- Advantage
  - Simple to implement in hardware. 
  - Memory for z-buffer is now not expensive
  - Diversity of primitives – not just polygons.
  - Unlimited scene complexity
  - Don’t need to calculate object-object intersections.
-  Disadvantage
  - Extra memory and bandwidth
  - Waste time drawing hidden objects
  - Z-precision errors
  - May have to use point sampling 

## **Rasterization**

### 💖 Line Ras

#### DDA

Select pixel vertically closest to line segment.

```c++
dx = x2 - x1, dy = y2 - y1;
step = min(abs(dx), abs(dy)); // 走小的
for(int i=0; i<step; ++i)
{
  	x += dx / (step);
  	y += dy / (step);
  	setp(round(x), round(y));
}
```

#### Bresenham

转整数运算；

```c++
void bresenham(int x0, int y0, int x1, int y1) 
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;

    while (setpixel(x0, y0), x0 != x1 || y0 != y1) 
    {
        int e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 <  dy) { err += dx; y0 += sy; }
    }
}
```

### 💖 Scan Conversion = Fill Polygons 

- Split triagnles
- Fill the spans 

### 💖 Inside test

Odd-Even rule.

<img src="https://i.loli.net/2019/12/22/Na5HROkSiWf7DJK.png" style="zoom:35%;" />

### 💖 Flood Fill

```c++
flood_fill(int x, int y) 
{
    if(read_pixel(x,y) == WHITE) 
    {
				write_pixel(x,y,BLACK); 
      	flood_fill(x-1, y); 
      	flood_fill(x+1, y); 
      	flood_fill(x, y+1); 
      	flood_fill(x, y-1);
		}
}
```

### 💖 Triangulation -> Ear-clipping

选一个点v，和他邻居组成三角形然后如果这个三角形是inside，那么把这个三角形记录，然后删去v，往复循环。

## **Illumination**

### 💖 Shading

- Variation in observed color across an obj.
- Cause by how a material reflects light.
  - geometry
  - lighting
  - material

### 💖 Color models

- RGB
- CMY(Cyan, Magenta, Yellow)
- HSV(Hue, Saturation, value)

### 💖 Phong Reflection Model

<img src="https://i.loli.net/2019/12/22/mIj6AsgrYuE2iKJ.png" style="zoom:50%;" />



#### Ambient Reflection

$$
I_a = k_a*L_a
$$

环境颜色 * 强度系数；

#### Diffuse

$$
I_d=k_d*(\cos<l, n>)*L_d
$$

#### Specular

$$
I_s=\max (k_s L_s \cos <v, r>, 0)
$$

Then add them all.

### 💖 Blinn-Phong reflection model

Replace **cos<v,r>** using **cos<h, n>**, where **h = (v+l)/2**

<img src="https://i.loli.net/2019/11/19/MYSZIlNsLr98qwJ.png" style="zoom:50%;" />

Before any computation, we know $v, n, l$. Computation of $h$ is easy: $h = \frac{l+v}{||l+v||}$ and computation of $r$ is a little bit complex: $r=2*n*(l*n)-l$ .

- The purpose of the modification is to speed up the computation.
- Blinn-Phong reflection model only modified the **specular reflection** stage $\to L_sK_s\max(\frac{l+v}{||l+v||}*n, 0)$

### 💖 Flat shading

Assign all pixels inside each polygon the same color, therefore reveal polygons and fast.

### 💖 Gourand shading

Gourand Shading uses **vertex shader** and **interpolation methods** on fragment color. <u>The color of vertexes are given and the color of fragments are computed by interpolation.</u>

There're mainly 3 steps:

1. Normal averaging;（对于每个vertex的法向量用周围边的平均值来估计）
2. Vertex lighting;（compute the color for each vertex based on a shading model）
3. Interpolation;（既然算出了点的颜色，那么图元内部像素就通过双线性插值来算）

### 💖 Phong Shading

- Normal interpolation
- Fragment lighting

---



### 💖 Edge Collapse 

### 💖 Vertex removal 

贝塞尔插值就2个射线，Hermit插值poly插值；

<img src="https://i.loli.net/2019/12/23/LNmyKJw8eAnFtxM.png" style="zoom:33%;" />

就一组合公式；

## OpenGL vs RT

OpenGL is based on a pipeline model in which primitives are rendered one at time 

- No shadows (shadow maps etc.)
- No multiple reflections (environment maps etc.) 

Global approaches 

- Rendering equation 
- Ray tracing 
- Radiosity 
- Easily making effects: shadows, reflections, transparency 

