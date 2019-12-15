# CG Review

[TOC]

## Computer Graphics

### Graphic Pipline

- **Application** stage
- **Geometry** stage
- **Rasterization** stage

<img src="https://i.loli.net/2019/12/09/NCSLMraQ6gyKIj2.png" style="zoom:50%;" />



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

## Transformation

### Types

- Translation(*non-linear*) $T(u)=u+v$
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

### Affine Transformation

Translation is not linear transformation. And affine transformation helps translation together with linear transformations.

## Viewing

### 4 transformations

- **Modeling** Transformation(local space -> world space)
- **Camera** Transformation(world space -> camera space)
- **Projection** Transformation(camera -> NDC)
- **Viewport** Transformation(NDC -> viewport)

### The elements of image formation

- Object
- Viewer
- Light Source(s)
- Attributes

### Viewing impl

1. Local space -> world space -> camera space.
2. Projection -> view. (3D -> 2D)
3. Clip geometry outside the view volume.
4. Perform perspective-division into NDC.
5. Remove hidden surfaces.

### Orthographic  projection

Just define a orthographic view volume. (A box)

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

### Perspective projection

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

和orthographic差不多，就是l, r, b, t的scale会和n&f有关。

### Diff between NDC & clip space

- **Clip space:** After projection transformation and before perspective division.(w is not 1)
- **NDC:** The space after perspective division. The homogenous coordinate w of vertex is 1;

### Why does clip performed in the clip space? 

- **Efficiency**;
- **Correctness**: The orginal depth info in kept in clip space, not is normalized in NDC;

### Line clipping: Cohen-Sutherland Algorithm 

- Outcode;

### Polygon clipping: Weiler-Atherton Clipping 

- Walk(逆时针，遇到边界且是入射，那么就OK，连续的出射点+入射点=>边界直连)
- 如果边界和polygon只有但交点那就凉了；

> 其他方法就拆三角形就完事了。

