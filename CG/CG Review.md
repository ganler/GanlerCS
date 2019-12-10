# CG Review

[TOC]

### Graphic Pipline

- Application stage
- Geometry stage
- Rasterization stage

<img src="https://i.loli.net/2019/12/09/NCSLMraQ6gyKIj2.png" style="zoom:50%;" />



A **vertex** is a point/node in the space, represented by its coordinate and properties, e.g. color, normal, texture coordinate etc. 

A **primitive** is the basic element of geometric shapes and rendering, it is composed of vertices. It can be points, lines, polygons or polyhedrons. 

A **fragment** is the output data of the geometry shader in <u>rasterization stage</u>. The fragment contains the data necessary to generate <u>a single pixel in the frame buffer</u>, e.g. color, depth, normal, texture coordinate etc.

**Geometry**: locations of the vertices.

**Topology**: organization of the vertices and edges.

### Transformation

- **Modeling** Transformation(local space -> world space)
- **Camera** Transformation(world space -> camera space)
- **Projection** Transformation(camera -> NDC)
- **Viewport** Transformation(NDC -> viewport)