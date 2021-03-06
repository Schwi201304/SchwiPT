# Schwi Ray Tracing

[toc]

## TODO

1. 实现三角网格
2. 在已有AABB上开发BVH加速
3. 加入日志
4. 实现Frame的缩放，法线有待实现
4. UI界面，考虑自由生成场景而非编码

## Development Plan

### Shape

- [x] Sphere
- [x] Cylinder
- [x] Disk&Rectangle&Box
- [ ] Triangle Mesh

### Light

- [x] Area Light
- [ ] Point Light
- [ ] Direct Light

### Reflection Models
- [x] Lambertion&Specular&Fresnel
- [ ] Sub-Surface Scattering

### Materials

- [x] Matte&Mirror&Glass
- [x] Plastic
- [ ] Skin
- [ ] Stockings
- [ ] Mix

### Texture

- [x] Constant

- [x] Imagemap
- [ ] Mipmap

### Sampler
- [x] Random Sampler

### Camera

- [x] Perpective Camera

- [ ] Environment Camera

### Accelerator

- [ ] BVH
- [ ] KD-Tree

### Filter

- [x] Gaussian

## References

1. [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
2. [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
3. [_Ray Tracing: The Rest of Your Life_](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)
4. [Physically Based Rendering: From Theory to Implementation ](https://www.pbr-book.org/)

