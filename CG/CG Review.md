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

A **primitive** is the basic element of geometric shapes and rendering, it is composed of vertices. It can be points, lines, polygons or polyhedrons. 

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
- **Correctness**: The orginal depth info in kept in clip space, not is normalized in NDC;

### Line clipping: Cohen-Sutherland Algorithm 

- Outcode;

### Polygon clipping: Weiler-Atherton Clipping 

- Walk(逆时针，遇到边界且是入射，那么就OK，连续的出射点+入射点=>边界直连)
- 如果polygon的一个角点在screen的边上就麻烦了；

> 其他方法就拆三角形就完事了。

## **Visible Surface Detection**

### 💖 Z-Buffer

简单来说就是在viewport上的每个像素：

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